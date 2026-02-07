#pragma once
#include <vector>
#include <queue> 
#include <memory>
#include <algorithm>
#include "Thread.hpp"

class Scheduler {
  private: 
    // Multi-Level Queues
    std::queue<Thread*> readyQueueHigh; // Priority 0
    std::queue<Thread*> readyQueueLow;  // Priority 1
    
    Thread* currentThread;

  public: 
    Scheduler();

    // Add a new thread to the scheduler 
    void addThread(Thread* thread);
    

    // The Core Function: Switch to the next thread
    void yield();

    // Helper to see who is running
    Thread* getCurrentThread();

    // Block the current thread (transition to BLOCKED state)
    void blockCurrentThread();

    // Wake up a blocked thread (transition to READY and add to queue)
    void wakeup(Thread* thread);

    // Get all threads (for ps command)
    std::vector<Thread*> getAllThreads();

    // Remove a thread by ID (for kill command)
    bool removeThread(int id);
};
