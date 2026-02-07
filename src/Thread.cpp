#include "../include/Thread.hpp"

// Constructor
Thread::Thread(int id, int parentPid, const std::string& name, int priority)
  : id(id), 
    parentPid(parentPid),
    name(name), 
    state(ThreadState::READY), 
    programCounter(0),
    priority(priority)
{}

// Getters 
int Thread::getId() const {
  return id;
}

int Thread::getParentPid() const {
  return parentPid;
}

int Thread::getPriority() const {
    return priority;
}

std::string Thread::getName() const {
  return name;
}

ThreadState Thread::getState() const {
  return state;
}

int Thread::getProgramCounter() const {
  return programCounter;
}

// Setters 
void Thread::setState(ThreadState s) {
  state = s;
}

void Thread::setProgramCounter(int pc) {
  programCounter = pc;
}

void Thread::incrementProgramCounter() {
  programCounter++;
}
