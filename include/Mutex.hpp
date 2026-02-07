#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <queue>
#include "Task.hpp"
#include "Scheduler.hpp"

class Mutex {
private:
    bool locked;
    Task* owner;
    std::queue<Task*> waitingQueue;

public:
    Mutex();

    // Attempt to lock the mutex. 
    // If successful, returns true.
    // If held by another, blocks the current task via scheduler and returns false (should yield).
    bool lock(Scheduler& scheduler);

    // Release the mutex.
    // If tasks are waiting, wakes up the next one.
    void unlock(Scheduler& scheduler);
};

#endif
