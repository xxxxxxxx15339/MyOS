#include <iostream>
#include "../include/Mutex.hpp"

Mutex::Mutex() : locked(false), owner(nullptr) {}

bool Mutex::lock(Scheduler& scheduler) {
    Task* current = scheduler.getCurrentTask();
    if (current == nullptr) return false;

    if (locked && owner == current) {
        return true; // recursive lock or handoff
    }

    if (!locked) {
        locked = true;
        owner = current;
        std::cout << "[Mutex] Task " << current->getId() << " acquired lock." << std::endl;
        return true; // Acquired
    } else {
        std::cout << "[Mutex] Task " << current->getId() << " blocked waiting for lock (held by " << (owner ? std::to_string(owner->getId()) : "Unknown") << ")." << std::endl;
        waitingQueue.push(current);
        scheduler.blockCurrentlyRunningTask();
        return false; // Blocked
    }
}

void Mutex::unlock(Scheduler& scheduler) {
    Task* current = scheduler.getCurrentTask();
    if (owner != current) {
        // Technically should be an error if non-owner tries to unlock
        std::cout << "[Mutex] Error: Task " << (current ? std::to_string(current->getId()) : "null") << " tried to unlock mutex owned by " << (owner ? std::to_string(owner->getId()) : "null") << std::endl;
        return;
    }

    std::cout << "[Mutex] Task " << current->getId() << " releasing lock." << std::endl;
    
    if (!waitingQueue.empty()) {
        Task* next = waitingQueue.front();
        waitingQueue.pop();
        
        // Handover ownership directly to the next task to avoid race upon wakeup?
        // Or just wake it up and let it retry lock?
        // Standard Mesa monitor semantics: wake it up, it retries.
        // Standard Hoare semantics: handover.
        
        // Let's go with "Handover" for simplicity in this non-preemptive sim, 
        // OR simply wake it up. 
        // If we wake it up (READY), it will run later. 
        // If we treat it as "unlocked" now, another running task could grab it before the woken one.
        // To guarantee fairness (and simplicity), let's assign it to the next one immediately if queue is not empty.
        
        owner = next;
        scheduler.wakeup(next);
        std::cout << "[Mutex] Ownership transferred to Task " << next->getId() << "." << std::endl;
        // locked remains true
    } else {
        locked = false;
        owner = nullptr;
    }
}
