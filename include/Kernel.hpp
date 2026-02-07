#pragma once
#include <vector>
#include <string>
#include "Scheduler.hpp"
#include "Mutex.hpp"
#include "MemoryManager.hpp"
#include "FileSystem.hpp"

class Shell; // Forward declaration

class Kernel {
  private:
    Scheduler scheduler;
    Mutex sharedMutex;
    MemoryManager memoryManager;
    FileSystem fileSystem;
    int nextTaskId;

    struct SleepingTask {
        Task* task;
        int wakeAtTick;
    };
    std::vector<SleepingTask> sleepList;
    int currentTick;

  public:
    Kernel();

    void boot();
    void run();  // Legacy mode
    void runCycles(int cycles); // Shell mode
    void executeInstruction(Task* task);

    // Shell API
    int spawnTask(const std::string& name, int priority);
    void listTasks();
    bool killTask(int id);
    void showMemory();
    void showFiles();

    MemoryManager& getMemoryManager() { return memoryManager; }
    FileSystem& getFileSystem() { return fileSystem; }
};
