#include <iostream>
#include "../include/Mutex.hpp"

Mutex::Mutex() : locked(false), owner(nullptr) {}

bool Mutex::lock(Scheduler& scheduler) {
    Thread* current = scheduler.getCurrentThread();
    if (current == nullptr) return false;

    if (locked && owner == current) {
        return true; // recursive lock or handoff
    }

    if (!locked) {
        locked = true;
        owner = current;
        std::cout << "[Mutex] Thread " << current->getId() << " acquired lock." << std::endl;
        return true; // Acquired
    } else {
        std::cout << "[Mutex] Thread " << current->getId() << " blocked waiting for lock (held by " << (owner ? std::to_string(owner->getId()) : "Unknown") << ")." << std::endl;
        waitingQueue.push(current);
        scheduler.blockCurrentThread();
        return false; // Blocked
    }
}

void Mutex::unlock(Scheduler& scheduler) {
    Thread* current = scheduler.getCurrentThread();
    if (owner != current) {
        // Technically should be an error if non-owner tries to unlock
        std::cout << "[Mutex] Error: Thread " << (current ? std::to_string(current->getId()) : "null") << " tried to unlock mutex owned by " << (owner ? std::to_string(owner->getId()) : "null") << std::endl;
        return;
    }

    std::cout << "[Mutex] Thread " << current->getId() << " releasing lock." << std::endl;
    
    if (!waitingQueue.empty()) {
        Thread* next = waitingQueue.front();
        waitingQueue.pop();
        
        // Handover ownership directly to the next thread
        owner = next;
        scheduler.wakeup(next);
        std::cout << "[Mutex] Ownership transferred to Thread " << next->getId() << "." << std::endl;
        // locked remains true
    } else {
        locked = false;
        owner = nullptr;
    }
}
