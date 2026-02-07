#pragma once
#include <string>
#include <vector>

class Thread;  // Forward declaration

class Process {
private:
    int pid;
    std::string name;
    std::vector<Thread*> threads;
    int memoryStart;  // Start of allocated memory region
    int memorySize;   // Size of allocated memory

public:
    Process(int pid, const std::string& name);
    ~Process();

    // Thread management
    void addThread(Thread* thread);
    bool removeThread(int threadId);

    // Getters
    int getPid() const;
    std::string getName() const;
    const std::vector<Thread*>& getThreads() const;
    int getThreadCount() const;
    int getMemoryStart() const;
    int getMemorySize() const;

    // Memory allocation (set by Kernel)
    void setMemory(int start, int size);
};
