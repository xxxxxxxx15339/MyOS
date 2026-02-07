#pragma once 
#include <string> 

enum class TaskState {
  READY,
  RUNNING,
  BLOCKED,
  TERMINATED
};

class Task {
  private:
    int id;                 // ID (PID)
    std::string name;       // Debug name  
    TaskState state;        // Current status 
    int ProgramCounter;     // Simulated Instruction Pointer
    int priority;           // Priority (0=High, 1=Low) 

  public:
    Task(int id, std::string name, int priority = 1);

    // Getters
    int getId() const;
    std::string getName() const;
    TaskState getState() const;
    int getProgramCounter() const; 
    int getPriority() const; 

    // Setters / Control 
    void setState(TaskState s);
    void setProgramCounter(int pc);
    void incrementProgramCounter();
};
