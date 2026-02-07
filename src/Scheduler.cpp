#include "../include/Scheduler.hpp"
#include <iostream>

Scheduler::Scheduler() :
  currentThread(nullptr) {
}

void Scheduler::addThread(Thread* thread) {
  if (thread->getPriority() == 0) {
      readyQueueHigh.push(thread);
  } else {
      readyQueueLow.push(thread);
  }
}

// yield() performs scheduling based on Priority
void Scheduler::yield() {
  
  // 1. Save current thread context
  if (currentThread != nullptr) {
      if (currentThread->getState() == ThreadState::RUNNING) {
        currentThread->setState(ThreadState::READY);
        // Re-queue based on priority
        if (currentThread->getPriority() == 0) {
            readyQueueHigh.push(currentThread);
        } else {
            readyQueueLow.push(currentThread);
        }
      } 
      // If BLOCKED or TERMINATED, do nothing (context already saved/irrelevant)
  }
  
  // 2. Pick next thread (Strict Priority)
  if (!readyQueueHigh.empty()) {
      currentThread = readyQueueHigh.front();
      readyQueueHigh.pop();
  } else if (!readyQueueLow.empty()) {
      currentThread = readyQueueLow.front();
      readyQueueLow.pop();
  } else {
      currentThread = nullptr; 
      std::cout << "Scheduler: No ready threads." << std::endl;
      return;
  }

  if (currentThread) {
      currentThread->setState(ThreadState::RUNNING);
      std::cout << "Context Switch: Running Thread " << currentThread->getId() 
                << " (PID " << currentThread->getParentPid() << ")"
                << " [" << (currentThread->getPriority() == 0 ? "HIGH" : "LOW") << "] " 
                << "(" << currentThread->getName() << ")" << std::endl;
  }
}

void Scheduler::wakeup(Thread* thread) {
    if (thread && thread->getState() == ThreadState::BLOCKED) {
        thread->setState(ThreadState::READY);
        if (thread->getPriority() == 0) {
            readyQueueHigh.push(thread);
            std::cout << "Scheduler: Waking up HIGH Priority Thread " << thread->getId() << std::endl;
        } else {
            readyQueueLow.push(thread);
            std::cout << "Scheduler: Waking up LOW Priority Thread " << thread->getId() << std::endl;
        }
    }
}

void Scheduler::blockCurrentThread() {
    if (currentThread) {
        currentThread->setState(ThreadState::BLOCKED);
    }
}

Thread* Scheduler::getCurrentThread() {
  return currentThread;
}

std::vector<Thread*> Scheduler::getAllThreads() {
    std::vector<Thread*> allThreads;
    
    // Add current thread if exists
    if (currentThread) {
        allThreads.push_back(currentThread);
    }
    
    // Copy from high priority queue
    std::queue<Thread*> tempHigh = readyQueueHigh;
    while (!tempHigh.empty()) {
        allThreads.push_back(tempHigh.front());
        tempHigh.pop();
    }
    
    // Copy from low priority queue
    std::queue<Thread*> tempLow = readyQueueLow;
    while (!tempLow.empty()) {
        allThreads.push_back(tempLow.front());
        tempLow.pop();
    }
    
    return allThreads;
}

bool Scheduler::removeThread(int id) {
    // Check current thread
    if (currentThread && currentThread->getId() == id) {
        currentThread = nullptr;
        return true;
    }
    
    // Helper to remove from queue
    auto removeFromQueue = [id](std::queue<Thread*>& q) -> bool {
        std::queue<Thread*> temp;
        bool found = false;
        while (!q.empty()) {
            Thread* t = q.front();
            q.pop();
            if (t->getId() == id) {
                found = true;
            } else {
                temp.push(t);
            }
        }
        q = temp;
        return found;
    };
    
    if (removeFromQueue(readyQueueHigh)) return true;
    if (removeFromQueue(readyQueueLow)) return true;
    
    return false;
}
