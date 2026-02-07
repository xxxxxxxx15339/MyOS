#pragma once
#include <vector>
#include <list>
#include <cstddef> // for size_t

struct MemoryBlock {
    size_t offset;
    size_t size;
    bool isFree;
};

class MemoryManager {
private:
    std::vector<char> ram;
    std::list<MemoryBlock> memoryList;
    const size_t MAX_MEMORY = 1024; // 1 KB Simulated RAM

public:
    MemoryManager();

    // Allocate 'size' bytes. Returns pointer to memory or nullptr if failed.
    void* allocate(size_t size);

    // Free memory pointed to by 'ptr'.
    void deallocate(void* ptr);

    // Debug: Print current memory layout
    void printMemoryMap();
};
