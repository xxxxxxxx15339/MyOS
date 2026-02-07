#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include "../include/Kernel.hpp"

Kernel::Kernel() : nextPid(1), nextThreadId(1), currentTick(0) {
}

Kernel::~Kernel() {
    for (auto* proc : processes) {
        delete proc;
    }
    processes.clear();
}

void Kernel::boot() {
    std::cout << "[Kernel] MyOS booting up..." << std::endl;
    std::cout << "[Kernel] Memory Manager initialized." << std::endl;
    std::cout << "[Kernel] File System initialized." << std::endl;
    std::cout << "[Kernel] Scheduler ready." << std::endl;
}

void Kernel::run() {
    // Legacy mode - runs fixed cycles
    runCycles(30);
    std::cout << "[Kernel] Halted." << std::endl;
}

void Kernel::runCycles(int cycles) {
    while (cycles > 0) {
        currentTick++;

        // Wake up sleeping threads
        auto it = sleepList.begin();
        while (it != sleepList.end()) {
            if (currentTick >= it->wakeAtTick) {
                scheduler.wakeup(it->thread);
                it = sleepList.erase(it);
            } else {
                ++it;
            }
        }

        scheduler.yield();

        Thread* current = scheduler.getCurrentThread();
        if (current != nullptr) {
            executeInstruction(current);
        }
        cycles--;
    }
}

void Kernel::executeInstruction(Thread* current) {
    std::string name = current->getName();
    int pc = current->getProgramCounter();
    int tid = current->getId();
    int pid = current->getParentPid();
    
    std::cout << "  [CPU] Thread " << tid << " (PID " << pid << ", " << name << ") executing instruction " << pc << std::endl;
    
    // Generic thread simulation - just increment PC
    current->incrementProgramCounter();
    
    // Threads "complete" after 5 instructions for demo
    if (current->getProgramCounter() >= 5) {
        std::cout << "  [CPU] Thread " << tid << " (" << name << ") completed!" << std::endl;
        current->setState(ThreadState::TERMINATED);
    }
}

int Kernel::createProcess(const std::string& name) {
    int pid = nextPid++;
    Process* proc = new Process(pid, name);
    
    // Allocate memory for the process (64 bytes per process for demo)
    void* mem = memoryManager.allocate(64);
    if (mem != nullptr) {
        // Store offset for display purposes
        proc->setMemory(0, 64);
    }
    
    // Create main thread for the process
    int tid = nextThreadId++;
    Thread* mainThread = new Thread(tid, pid, "main", 0); // HIGH priority for main
    proc->addThread(mainThread);
    scheduler.addThread(mainThread);
    
    processes.push_back(proc);
    return pid;
}

int Kernel::spawnThread(int pid, const std::string& name, int priority) {
    Process* proc = findProcess(pid);
    if (!proc) {
        return -1; // Process not found
    }
    
    int tid = nextThreadId++;
    Thread* thread = new Thread(tid, pid, name, priority);
    proc->addThread(thread);
    scheduler.addThread(thread);
    
    return tid;
}

// Legacy spawn - creates a process with a single main thread
int Kernel::spawnTask(const std::string& name, int priority) {
    int pid = nextPid++;
    Process* proc = new Process(pid, name);
    
    int tid = nextThreadId++;
    Thread* thread = new Thread(tid, pid, name, priority);
    proc->addThread(thread);
    scheduler.addThread(thread);
    
    processes.push_back(proc);
    return tid;  // Return thread ID for backward compatibility
}

void Kernel::listProcesses() {
    std::cout << "\n┌────────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│                    Process List                        │" << std::endl;
    std::cout << "├────────────────────────────────────────────────────────┤" << std::endl;
    
    if (processes.empty()) {
        std::cout << "│              (no processes running)                    │" << std::endl;
    } else {
        for (const auto* proc : processes) {
            printf("│ PID %-3d: %-20s [%d threads, %d bytes]  │\n", 
                   proc->getPid(), 
                   proc->getName().substr(0, 20).c_str(),
                   proc->getThreadCount(),
                   proc->getMemorySize());
            
            const auto& threads = proc->getThreads();
            for (size_t i = 0; i < threads.size(); ++i) {
                const Thread* t = threads[i];
                std::string state;
                switch (t->getState()) {
                    case ThreadState::READY: state = "READY"; break;
                    case ThreadState::RUNNING: state = "RUNNING"; break;
                    case ThreadState::BLOCKED: state = "BLOCKED"; break;
                    case ThreadState::TERMINATED: state = "DONE"; break;
                }
                const char* prefix = (i == threads.size() - 1) ? "└─" : "├─";
                printf("│   %s Thread %-3d: %-12s [%-4s] %-8s      │\n",
                       prefix,
                       t->getId(),
                       t->getName().substr(0, 12).c_str(),
                       t->getPriority() == 0 ? "HIGH" : "LOW",
                       state.c_str());
            }
        }
    }
    std::cout << "└────────────────────────────────────────────────────────┘" << std::endl;
}

void Kernel::listThreads() {
    std::cout << "\n┌─────┬─────┬────────────────────┬──────────┬──────────┐" << std::endl;
    std::cout << "│ TID │ PID │ Name               │ Priority │ State    │" << std::endl;
    std::cout << "├─────┼─────┼────────────────────┼──────────┼──────────┤" << std::endl;
    
    auto threads = scheduler.getAllThreads();
    if (threads.empty()) {
        std::cout << "│                (no threads running)                    │" << std::endl;
    } else {
        for (const auto* thread : threads) {
            std::string state;
            switch (thread->getState()) {
                case ThreadState::READY: state = "READY"; break;
                case ThreadState::RUNNING: state = "RUNNING"; break;
                case ThreadState::BLOCKED: state = "BLOCKED"; break;
                case ThreadState::TERMINATED: state = "DONE"; break;
            }
            printf("│ %-3d │ %-3d │ %-18s │ %-8s │ %-8s │\n", 
                   thread->getId(),
                   thread->getParentPid(),
                   thread->getName().substr(0, 18).c_str(),
                   thread->getPriority() == 0 ? "HIGH" : "LOW",
                   state.c_str());
        }
    }
    std::cout << "└─────┴─────┴────────────────────┴──────────┴──────────┘" << std::endl;
}

bool Kernel::killThread(int id) {
    // Find the thread's parent process and remove it
    for (auto* proc : processes) {
        if (proc->removeThread(id)) {
            scheduler.removeThread(id);
            return true;
        }
    }
    return scheduler.removeThread(id);
}

bool Kernel::killProcess(int pid) {
    for (auto it = processes.begin(); it != processes.end(); ++it) {
        if ((*it)->getPid() == pid) {
            // Remove all threads from scheduler
            for (auto* thread : (*it)->getThreads()) {
                scheduler.removeThread(thread->getId());
            }
            // Note: Memory deallocation would require storing actual pointer
            // For now, process memory is not reclaimed in this simplified demo
            delete *it;
            processes.erase(it);
            return true;
        }
    }
    return false;
}

void Kernel::showMemory() {
    memoryManager.printMemoryMap();
}

void Kernel::showFiles() {
    fileSystem.printInodeTable();
}

Process* Kernel::findProcess(int pid) {
    for (auto* proc : processes) {
        if (proc->getPid() == pid) {
            return proc;
        }
    }
    return nullptr;
}
