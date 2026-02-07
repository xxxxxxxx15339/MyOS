#pragma once
#include <vector>
#include <queue> 
#include <memory>
#include <algorithm>
#include "Task.hpp"

class Scheduler {
  private: 
    // Multi-Level Queues
    std::queue<Task*> readyQueueHigh; // Priority 0
    std::queue<Task*> readyQueueLow;  // Priority 1
    
    Task* currentTask;

  public: 
    Scheduler();

    // Add a new task to the mix 
    void addTask(Task* task);
    

    // The Core Function: Switch to the next task
    void yield();

    // Helper to see who is running
    Task* getCurrentTask();

    // Block the current task (transition to BLOCKED state)
    void blockCurrentlyRunningTask();

    // Wake up a blocked task (transition to READY and add to queue)
    void wakeup(Task* task);

    // Get all tasks (for ps command)
    std::vector<Task*> getAllTasks();

    // Remove a task by ID (for kill command)
    bool removeTask(int id);
};
