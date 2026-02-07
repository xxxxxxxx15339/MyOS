#include "../include/Process.hpp"
#include "../include/Thread.hpp"
#include <algorithm>

Process::Process(int pid, const std::string& name)
    : pid(pid), name(name), memoryStart(0), memorySize(0) {
}

Process::~Process() {
    for (auto* thread : threads) {
        delete thread;
    }
    threads.clear();
}

void Process::addThread(Thread* thread) {
    threads.push_back(thread);
}

bool Process::removeThread(int threadId) {
    auto it = std::find_if(threads.begin(), threads.end(),
        [threadId](Thread* t) { return t->getId() == threadId; });
    
    if (it != threads.end()) {
        delete *it;
        threads.erase(it);
        return true;
    }
    return false;
}

int Process::getPid() const {
    return pid;
}

std::string Process::getName() const {
    return name;
}

const std::vector<Thread*>& Process::getThreads() const {
    return threads;
}

int Process::getThreadCount() const {
    return static_cast<int>(threads.size());
}

int Process::getMemoryStart() const {
    return memoryStart;
}

int Process::getMemorySize() const {
    return memorySize;
}

void Process::setMemory(int start, int size) {
    memoryStart = start;
    memorySize = size;
}
