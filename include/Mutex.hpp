#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <queue>
#include "Thread.hpp"
#include "Scheduler.hpp"

class Mutex {
private:
    bool locked;
    Thread* owner;
    std::queue<Thread*> waitingQueue;

public:
    Mutex();

    // Attempt to lock the mutex. 
    // If successful, returns true.
    // If held by another, blocks the current thread via scheduler and returns false (should yield).
    bool lock(Scheduler& scheduler);

    // Release the mutex.
    // If threads are waiting, wakes up the next one.
    void unlock(Scheduler& scheduler);
};

#endif
