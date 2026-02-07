#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include "../include/Kernel.hpp"

Kernel::Kernel() : nextTaskId(1), currentTick(0) {
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

        // Wake up sleeping tasks
        auto it = sleepList.begin();
        while (it != sleepList.end()) {
            if (currentTick >= it->wakeAtTick) {
                scheduler.wakeup(it->task);
                it = sleepList.erase(it);
            } else {
                ++it;
            }
        }

        scheduler.yield();

        Task* current = scheduler.getCurrentTask();
        if (current != nullptr) {
            executeInstruction(current);
        }
        cycles--;
    }
}

void Kernel::executeInstruction(Task* current) {
    std::string name = current->getName();
    int pc = current->getProgramCounter();
    int pid = current->getId();
    
    std::cout << "  [CPU] Task " << pid << " (" << name << ") executing instruction " << pc << std::endl;
    
    // Generic task simulation - just increment PC
    current->incrementProgramCounter();
    
    // Tasks "complete" after 5 instructions for demo
    if (current->getProgramCounter() >= 5) {
        std::cout << "  [CPU] Task " << pid << " (" << name << ") completed!" << std::endl;
        current->setState(TaskState::TERMINATED);
    }
}

int Kernel::spawnTask(const std::string& name, int priority) {
    int id = nextTaskId++;
    Task* task = new Task(id, name, priority);
    scheduler.addTask(task);
    return id;
}

void Kernel::listTasks() {
    std::cout << "\n┌─────┬────────────────────┬──────────┬──────────┐" << std::endl;
    std::cout << "│ ID  │ Name               │ Priority │ State    │" << std::endl;
    std::cout << "├─────┼────────────────────┼──────────┼──────────┤" << std::endl;
    
    auto tasks = scheduler.getAllTasks();
    if (tasks.empty()) {
        std::cout << "│              (no tasks)                       │" << std::endl;
    } else {
        for (const auto& task : tasks) {
            std::string state;
            switch (task->getState()) {
                case TaskState::READY: state = "READY"; break;
                case TaskState::RUNNING: state = "RUNNING"; break;
                case TaskState::BLOCKED: state = "BLOCKED"; break;
                case TaskState::TERMINATED: state = "DONE"; break;
            }
            printf("│ %-3d │ %-18s │ %-8s │ %-8s │\n", 
                   task->getId(), 
                   task->getName().substr(0, 18).c_str(),
                   task->getPriority() == 0 ? "HIGH" : "LOW",
                   state.c_str());
        }
    }
    std::cout << "└─────┴────────────────────┴──────────┴──────────┘" << std::endl;
}

bool Kernel::killTask(int id) {
    return scheduler.removeTask(id);
}

void Kernel::showMemory() {
    memoryManager.printMemoryMap();
}

void Kernel::showFiles() {
    fileSystem.printInodeTable();
}
