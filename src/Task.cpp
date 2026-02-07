#include "../include/Task.hpp"

// Constructor
// Constructor
Task::Task(int id, std::string name, int priority)
  : id(id), 
    name(name), 
    state(TaskState::READY), 
    ProgramCounter(0),
    priority(priority)
{}

// Getters 
int Task::getId() const {
  return id;
}

int Task::getPriority() const {
    return priority;
}

std::string Task::getName() const {
  return name;
}

TaskState Task::getState() const {
  return state;
}

int Task::getProgramCounter() const {
  return ProgramCounter;
}

// Setters 
void Task::setState(TaskState s) {
  state = s;
}

void Task::setProgramCounter(int pc) {
  ProgramCounter = pc;
}



void Task::incrementProgramCounter() {
  ProgramCounter++;
}
