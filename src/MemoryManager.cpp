#include "../include/MemoryManager.hpp"
#include <iostream>

MemoryManager::MemoryManager() {
    // Initialize RAM with 0
    ram.resize(MAX_MEMORY, 0);

    // Initially, one large free block covering simulated RAM
    memoryList.push_back({0, MAX_MEMORY, true});
    std::cout << "[MemoryManager] Initialized with " << MAX_MEMORY << " bytes." << std::endl;
}

void* MemoryManager::allocate(size_t size) {
    if (size == 0) return nullptr;

    // First-Fit Algorithm
    for (auto it = memoryList.begin(); it != memoryList.end(); ++it) {
        if (it->isFree && it->size >= size) {
            // Found a suitable block
            
            // Check if we need to split it (if it's bigger than needed)
            if (it->size > size) {
                // Create a new block for the remaining free space
                MemoryBlock newBlock;
                newBlock.offset = it->offset + size;
                newBlock.size = it->size - size;
                newBlock.isFree = true;
                
                // Insert new block after current
                memoryList.insert(std::next(it), newBlock);
            }

            // Update current block to be allocated
            it->size = size;
            it->isFree = false;

            std::cout << "[MemoryManager] Allocated " << size << " bytes at offset " << it->offset << "." << std::endl;
            
            // Return pointer to the start of this block in RAM
            return &ram[it->offset];
        }
    }

    std::cout << "[MemoryManager] Allocation failed: Not enough contiguous memory for " << size << " bytes." << std::endl;
    return nullptr;
}

void MemoryManager::deallocate(void* ptr) {
    if (ptr == nullptr) return;

    // Calculate offset
    // Note: This assumes ptr is within &ram[0] and &ram[MAX]
    // In a real OS, verify this.
    char* ramStart = &ram[0];
    char* ptrChar = static_cast<char*>(ptr);
    
    if (ptrChar < ramStart || ptrChar >= ramStart + MAX_MEMORY) {
        std::cout << "[MemoryManager] Error: Invalid pointer free request." << std::endl;
        return;
    }

    size_t offset = ptrChar - ramStart;

    // Find the block in the list
    for (auto it = memoryList.begin(); it != memoryList.end(); ++it) {
        if (it->offset == offset) {
            if (it->isFree) {
                std::cout << "[MemoryManager] Error: Double free at offset " << offset << "." << std::endl;
                return;
            }

            it->isFree = true;
            std::cout << "[MemoryManager] Freed block at offset " << offset << " (" << it->size << " bytes)." << std::endl;

            // Coalesce (Merge) with next block if free
            auto nextIt = std::next(it);
            if (nextIt != memoryList.end() && nextIt->isFree) {
                 it->size += nextIt->size;
                 memoryList.erase(nextIt);
            }

            // Coalesce with previous block if free
            if (it != memoryList.begin()) {
                auto prevIt = std::prev(it);
                if (prevIt->isFree) {
                    prevIt->size += it->size;
                    memoryList.erase(it);
                }
            }
            return;
        }
    }
     std::cout << "[MemoryManager] Error: Block not found for pointer." << std::endl;
}

void MemoryManager::printMemoryMap() {
    std::cout << "--- Memory Map ---" << std::endl;
    for (const auto& block : memoryList) {
        std::cout << "[" << (block.isFree ? "FREE" : "USED") 
                  << "] Offset: " << block.offset 
                  << ", Size: " << block.size << std::endl;
    }
    std::cout << "------------------" << std::endl;
}
