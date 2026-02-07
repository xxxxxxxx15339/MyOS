#pragma once
#include <string>
#include <vector>
#include <sstream>

class Kernel; // Forward declaration

class Shell {
private:
    Kernel* kernel;
    bool running;

    std::vector<std::string> tokenize(const std::string& input);
    void printPrompt();
    void executeCommand(const std::vector<std::string>& tokens);

    // Command handlers
    void cmdSpawn(const std::vector<std::string>& args);
    void cmdFork(const std::vector<std::string>& args);
    void cmdThread(const std::vector<std::string>& args);
    void cmdPs();
    void cmdProcs();
    void cmdKill(const std::vector<std::string>& args);
    void cmdMem();
    void cmdFiles();
    void cmdHelp();
    void cmdRun(const std::vector<std::string>& args);

public:
    Shell(Kernel* k);
    void run();
};
