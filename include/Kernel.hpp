#pragma once
#include <vector>
#include <string>
#include "Scheduler.hpp"
#include "Mutex.hpp"
#include "MemoryManager.hpp"
#include "FileSystem.hpp"
#include "Process.hpp"

class Shell; // Forward declaration

class Kernel {
  private:
    Scheduler scheduler;
    Mutex sharedMutex;
    MemoryManager memoryManager;
    FileSystem fileSystem;
    
    // Process management
    std::vector<Process*> processes;
    int nextPid;
    int nextThreadId;

    struct SleepingThread {
        Thread* thread;
        int wakeAtTick;
    };
    std::vector<SleepingThread> sleepList;
    int currentTick;

  public:
    Kernel();
    ~Kernel();

    void boot();
    void run();  // Legacy mode
    void runCycles(int cycles); // Shell mode
    void executeInstruction(Thread* thread);

    // Process/Thread API
    int createProcess(const std::string& name);
    int spawnThread(int pid, const std::string& name, int priority);
    void listProcesses();
    void listThreads();
    bool killThread(int id);
    bool killProcess(int pid);
    
    // Legacy spawn (creates process with main thread)
    int spawnTask(const std::string& name, int priority);
    
    void showMemory();
    void showFiles();

    MemoryManager& getMemoryManager() { return memoryManager; }
    FileSystem& getFileSystem() { return fileSystem; }
    
private:
    Process* findProcess(int pid);
};
