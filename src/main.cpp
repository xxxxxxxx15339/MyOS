#include "../include/Kernel.hpp"
#include "../include/Shell.hpp"

int main() {
    Kernel kernel;
    kernel.boot();
    
    Shell shell(&kernel);
    shell.run();
    
    return 0;
}
