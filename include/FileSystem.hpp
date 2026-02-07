#pragma once
#include <string>
#include <vector>
#include <fstream>

const int MAX_FILES = 16;
const int MAX_OPEN_FILES = 8;
const size_t DISK_SIZE = 4096;

struct Inode {
    std::string filename;
    size_t offset;
    size_t size;
    bool inUse;
};

struct OpenFile {
    int inodeIndex;
    size_t readPos;
    bool isOpen;
};

class FileSystem {
private:
    std::string diskPath;
    Inode inodeTable[MAX_FILES];
    OpenFile openFiles[MAX_OPEN_FILES];
    size_t nextFreeOffset;

    void initDisk();
    int findInode(const std::string& filename);
    int allocateInode(const std::string& filename);

public:
    FileSystem(const std::string& path = "disk.bin");

    int my_open(const std::string& filename);
    int my_write(int fd, const char* data, size_t len);
    int my_read(int fd, char* buffer, size_t len);
    void my_close(int fd);
    
    void printInodeTable();
};
