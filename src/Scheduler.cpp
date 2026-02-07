#include "../include/Scheduler.hpp"
#include <iostream>

Scheduler::Scheduler() :
  currentTask(nullptr) {
}

void Scheduler::addTask(Task* task) {
  if (task->getPriority() == 0) {
      readyQueueHigh.push(task);
  } else {
      readyQueueLow.push(task);
  }
}

// yield() performs scheduling based on Priority
void Scheduler::yield() {
  
  // 1. Save current task context
  if (currentTask != nullptr) {
      if (currentTask->getState() == TaskState::RUNNING) {
        currentTask->setState(TaskState::READY);
        // Re-queue based on priority
        if (currentTask->getPriority() == 0) {
            readyQueueHigh.push(currentTask);
        } else {
            readyQueueLow.push(currentTask);
        }
      } 
      // If BLOCKED or TERMINATED, do nothing (context already saved/irrelevant)
  }
  
  // 2. Pick next task (Strict Priority)
  if (!readyQueueHigh.empty()) {
      currentTask = readyQueueHigh.front();
      readyQueueHigh.pop();
  } else if (!readyQueueLow.empty()) {
      currentTask = readyQueueLow.front();
      readyQueueLow.pop();
  } else {
      // Idle
      if (currentTask && currentTask->getState() == TaskState::RUNNING) {
          // Keep running current if it was the only one and we just re-queued it? 
          // Wait, we re-queued it above. So if queues were empty before re-queue, 
          // they are not empty now if we had a running task.
          // BUT, we just popped it back off if it was the only one.
          // So if we are here, it means BOTH queues are empty properly.
      } else {
          currentTask = nullptr; 
          std::cout << "Scheduler: No ready tasks." << std::endl;
          return;
      }
  }

  if (currentTask) {
      currentTask->setState(TaskState::RUNNING);
      std::cout << "Context Switch: Running Task " << currentTask->getId() 
                << " [" << (currentTask->getPriority() == 0 ? "HIGH" : "LOW") << "] " 
                << "(" << currentTask->getName() << ")" << std::endl;
  }
}

void Scheduler::wakeup(Task* task) {
    if (task && task->getState() == TaskState::BLOCKED) {
        task->setState(TaskState::READY);
        if (task->getPriority() == 0) {
            readyQueueHigh.push(task);
            std::cout << "Scheduler: Waking up HIGH Priority Task " << task->getId() << std::endl;
        } else {
            readyQueueLow.push(task);
            std::cout << "Scheduler: Waking up LOW Priority Task " << task->getId() << std::endl;
        }
    }
}

void Scheduler::blockCurrentlyRunningTask() {
    if (currentTask) {
        currentTask->setState(TaskState::BLOCKED);
    }
}

Task* Scheduler::getCurrentTask() {
  return currentTask;
}

std::vector<Task*> Scheduler::getAllTasks() {
    std::vector<Task*> allTasks;
    
    // Add current task if exists
    if (currentTask) {
        allTasks.push_back(currentTask);
    }
    
    // Copy from high priority queue
    std::queue<Task*> tempHigh = readyQueueHigh;
    while (!tempHigh.empty()) {
        allTasks.push_back(tempHigh.front());
        tempHigh.pop();
    }
    
    // Copy from low priority queue
    std::queue<Task*> tempLow = readyQueueLow;
    while (!tempLow.empty()) {
        allTasks.push_back(tempLow.front());
        tempLow.pop();
    }
    
    return allTasks;
}

bool Scheduler::removeTask(int id) {
    // Check current task
    if (currentTask && currentTask->getId() == id) {
        delete currentTask;
        currentTask = nullptr;
        return true;
    }
    
    // Helper to remove from queue
    auto removeFromQueue = [id](std::queue<Task*>& q) -> bool {
        std::queue<Task*> temp;
        bool found = false;
        while (!q.empty()) {
            Task* t = q.front();
            q.pop();
            if (t->getId() == id) {
                delete t;
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
