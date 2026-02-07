#include "../include/Shell.hpp"
#include "../include/Kernel.hpp"
#include <iostream>
#include <algorithm>

Shell::Shell(Kernel* k) : kernel(k), running(true), nextTaskId(1) {
}

std::vector<std::string> Shell::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void Shell::printPrompt() {
    std::cout << "\n\033[1;32mMyOS>\033[0m ";
}

void Shell::run() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  Welcome to MyOS Interactive Shell" << std::endl;
    std::cout << "  Type 'help' for available commands" << std::endl;
    std::cout << "========================================\n" << std::endl;

    std::string input;
    while (running) {
        printPrompt();
        std::getline(std::cin, input);
        
        if (input.empty()) continue;
        
        auto tokens = tokenize(input);
        if (!tokens.empty()) {
            executeCommand(tokens);
        }
    }
}

void Shell::executeCommand(const std::vector<std::string>& tokens) {
    const std::string& cmd = tokens[0];
    
    if (cmd == "spawn") {
        cmdSpawn(tokens);
    } else if (cmd == "ps") {
        cmdPs();
    } else if (cmd == "kill") {
        cmdKill(tokens);
    } else if (cmd == "mem") {
        cmdMem();
    } else if (cmd == "files") {
        cmdFiles();
    } else if (cmd == "run") {
        cmdRun(tokens);
    } else if (cmd == "help") {
        cmdHelp();
    } else if (cmd == "exit" || cmd == "shutdown") {
        std::cout << "[Shell] Shutting down MyOS..." << std::endl;
        running = false;
    } else {
        std::cout << "[Shell] Unknown command: " << cmd << std::endl;
        std::cout << "        Type 'help' for available commands." << std::endl;
    }
}

void Shell::cmdSpawn(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: spawn <task_name> [priority]" << std::endl;
        std::cout << "       priority: 0 = HIGH, 1 = LOW (default)" << std::endl;
        return;
    }
    
    std::string name = args[1];
    int priority = 1; // Default LOW
    
    if (args.size() >= 3) {
        try {
            priority = std::stoi(args[2]);
            if (priority != 0 && priority != 1) {
                std::cout << "[Shell] Invalid priority. Using LOW (1)." << std::endl;
                priority = 1;
            }
        } catch (...) {
            std::cout << "[Shell] Invalid priority. Using LOW (1)." << std::endl;
        }
    }
    
    int id = kernel->spawnTask(name, priority);
    std::cout << "[Shell] Spawned task '" << name << "' with ID " << id 
              << " [" << (priority == 0 ? "HIGH" : "LOW") << "]" << std::endl;
}

void Shell::cmdPs() {
    kernel->listTasks();
}

void Shell::cmdKill(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: kill <task_id>" << std::endl;
        return;
    }
    
    try {
        int id = std::stoi(args[1]);
        if (kernel->killTask(id)) {
            std::cout << "[Shell] Terminated task " << id << std::endl;
        } else {
            std::cout << "[Shell] Task " << id << " not found." << std::endl;
        }
    } catch (...) {
        std::cout << "[Shell] Invalid task ID." << std::endl;
    }
}

void Shell::cmdMem() {
    kernel->showMemory();
}

void Shell::cmdFiles() {
    kernel->showFiles();
}

void Shell::cmdRun(const std::vector<std::string>& args) {
    int cycles = 10;
    if (args.size() >= 2) {
        try {
            cycles = std::stoi(args[1]);
        } catch (...) {
            cycles = 10;
        }
    }
    std::cout << "[Shell] Running " << cycles << " CPU cycles..." << std::endl;
    kernel->runCycles(cycles);
}

void Shell::cmdHelp() {
    std::cout << "\n┌─────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│             MyOS Shell Commands                     │" << std::endl;
    std::cout << "├─────────────────────────────────────────────────────┤" << std::endl;
    std::cout << "│  spawn <name> [priority]  Create a new task         │" << std::endl;
    std::cout << "│                           (0=HIGH, 1=LOW)           │" << std::endl;
    std::cout << "│  ps                       List all tasks            │" << std::endl;
    std::cout << "│  kill <id>                Terminate a task          │" << std::endl;
    std::cout << "│  run [cycles]             Execute CPU cycles        │" << std::endl;
    std::cout << "│  mem                      Show memory map           │" << std::endl;
    std::cout << "│  files                    Show inode table          │" << std::endl;
    std::cout << "│  help                     Show this help            │" << std::endl;
    std::cout << "│  exit                     Shutdown MyOS             │" << std::endl;
    std::cout << "└─────────────────────────────────────────────────────┘" << std::endl;
}
