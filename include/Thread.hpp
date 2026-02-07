#pragma once 
#include <string> 

enum class ThreadState {
  READY,
  RUNNING,
  BLOCKED,
  TERMINATED
};

class Thread {
  private:
    int id;                 // Thread ID (TID)
    int parentPid;          // Parent Process ID
    std::string name;       // Debug name  
    ThreadState state;      // Current status 
    int programCounter;     // Simulated Instruction Pointer
    int priority;           // Priority (0=High, 1=Low) 

  public:
    Thread(int id, int parentPid, const std::string& name, int priority = 1);

    // Getters
    int getId() const;
    int getParentPid() const;
    std::string getName() const;
    ThreadState getState() const;
    int getProgramCounter() const; 
    int getPriority() const; 

    // Setters / Control 
    void setState(ThreadState s);
    void setProgramCounter(int pc);
    void incrementProgramCounter();
};
