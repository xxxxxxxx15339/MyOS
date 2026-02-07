#include "../include/FileSystem.hpp"
#include <iostream>
#include <cstring>

FileSystem::FileSystem(const std::string& path) : diskPath(path), nextFreeOffset(0) {
    for (int i = 0; i < MAX_FILES; i++) {
        inodeTable[i].inUse = false;
    }
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        openFiles[i].isOpen = false;
    }
    initDisk();
    std::cout << "[FileSystem] Initialized with disk: " << diskPath << std::endl;
}

void FileSystem::initDisk() {
    std::ifstream check(diskPath);
    if (!check.good()) {
        std::ofstream create(diskPath, std::ios::binary);
        std::vector<char> zeros(DISK_SIZE, 0);
        create.write(zeros.data(), DISK_SIZE);
        create.close();
        std::cout << "[FileSystem] Created new disk file." << std::endl;
    }
    check.close();
}

int FileSystem::findInode(const std::string& filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (inodeTable[i].inUse && inodeTable[i].filename == filename) {
            return i;
        }
    }
    return -1;
}

int FileSystem::allocateInode(const std::string& filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (!inodeTable[i].inUse) {
            inodeTable[i].filename = filename;
            inodeTable[i].offset = nextFreeOffset;
            inodeTable[i].size = 0;
            inodeTable[i].inUse = true;
            return i;
        }
    }
    return -1;
}

int FileSystem::my_open(const std::string& filename) {
    int inodeIdx = findInode(filename);
    if (inodeIdx == -1) {
        inodeIdx = allocateInode(filename);
        if (inodeIdx == -1) {
            std::cout << "[FileSystem] Error: No free inodes." << std::endl;
            return -1;
        }
        std::cout << "[FileSystem] Created file: " << filename << std::endl;
    }
    for (int fd = 0; fd < MAX_OPEN_FILES; fd++) {
        if (!openFiles[fd].isOpen) {
            openFiles[fd].inodeIndex = inodeIdx;
            openFiles[fd].readPos = 0;
            openFiles[fd].isOpen = true;
            std::cout << "[FileSystem] Opened '" << filename << "' as fd=" << fd << std::endl;
            return fd;
        }
    }
    std::cout << "[FileSystem] Error: No free file descriptors." << std::endl;
    return -1;
}

int FileSystem::my_write(int fd, const char* data, size_t len) {
    if (fd < 0 || fd >= MAX_OPEN_FILES || !openFiles[fd].isOpen) {
        std::cout << "[FileSystem] Error: Invalid fd." << std::endl;
        return -1;
    }
    int inodeIdx = openFiles[fd].inodeIndex;
    Inode& inode = inodeTable[inodeIdx];
    if (inode.offset + inode.size + len > DISK_SIZE) {
        std::cout << "[FileSystem] Error: Disk full." << std::endl;
        return -1;
    }
    std::fstream disk(diskPath, std::ios::in | std::ios::out | std::ios::binary);
    disk.seekp(inode.offset + inode.size);
    disk.write(data, len);
    disk.close();
    inode.size += len;
    nextFreeOffset = inode.offset + inode.size;
    std::cout << "[FileSystem] Wrote " << len << " bytes to fd=" << fd << std::endl;
    return len;
}

int FileSystem::my_read(int fd, char* buffer, size_t len) {
    if (fd < 0 || fd >= MAX_OPEN_FILES || !openFiles[fd].isOpen) {
        std::cout << "[FileSystem] Error: Invalid fd." << std::endl;
        return -1;
    }
    int inodeIdx = openFiles[fd].inodeIndex;
    Inode& inode = inodeTable[inodeIdx];
    OpenFile& of = openFiles[fd];
    size_t bytesToRead = std::min(len, inode.size - of.readPos);
    if (bytesToRead == 0) return 0;
    std::ifstream disk(diskPath, std::ios::binary);
    disk.seekg(inode.offset + of.readPos);
    disk.read(buffer, bytesToRead);
    disk.close();
    of.readPos += bytesToRead;
    std::cout << "[FileSystem] Read " << bytesToRead << " bytes from fd=" << fd << std::endl;
    return bytesToRead;
}

void FileSystem::my_close(int fd) {
    if (fd < 0 || fd >= MAX_OPEN_FILES || !openFiles[fd].isOpen) return;
    openFiles[fd].isOpen = false;
    std::cout << "[FileSystem] Closed fd=" << fd << std::endl;
}

void FileSystem::printInodeTable() {
    std::cout << "--- Inode Table ---" << std::endl;
    for (int i = 0; i < MAX_FILES; i++) {
        if (inodeTable[i].inUse) {
            std::cout << "[" << i << "] " << inodeTable[i].filename 
                      << " | Offset: " << inodeTable[i].offset 
                      << " | Size: " << inodeTable[i].size << std::endl;
        }
    }
    std::cout << "-------------------" << std::endl;
}
