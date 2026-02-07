# MyOS - Educational Operating System Simulation

<p align="center">
  <img src="docs/images/OS.png" alt="MyOS Logo" width="400"/>
</p>

A user-space simulation of core operating system components, built in C++17. This project demonstrates fundamental OS concepts including process management, synchronization primitives, scheduling algorithms, memory management, and file systems.

## 🎯 Project Overview

This simulation implements the key abstractions found in real operating systems, allowing you to understand how they work without dealing with hardware-level complexity.

```
┌─────────────────────────────────────────────────────────────┐
│                         Kernel                              │
├──────────────┬──────────────┬───────────────┬───────────────┤
│  Scheduler   │    Mutex     │ MemoryManager │  FileSystem   │
│              │              │               │               │
│ • Priority   │ • Lock/      │ • First-Fit   │ • I-node      │
│   Queues     │   Unlock     │   Allocation  │   Table       │
│ • Context    │ • Blocking   │ • Coalescing  │ • disk.bin    │
│   Switch     │   Queue      │   Free        │   Persistence │
└──────────────┴──────────────┴───────────────┴───────────────┘
                              │
              ┌───────────────┴───────────────┐
              │           Processes           │
              │  • PID, Name, Memory Region   │
              │  • Contains multiple Threads  │
              └───────────────┬───────────────┘
                              │
              ┌───────────────┴───────────────┐
              │            Threads            │
              │  • TID, Parent PID            │
              │  • State (READY/RUNNING/      │
              │    BLOCKED)                   │
              │  • Priority (HIGH/LOW)        │
              │  • Program Counter            │
              └───────────────────────────────┘
```

## ✨ Features

### Phase 1: Process & Thread Manager
- **Process Control Block (PCB)**: Stores PID, name, memory region, and thread list
- **Thread Control Block (TCB)**: Stores TID, parent PID, state, and program counter
- **Cooperative Scheduling**: Threads yield control voluntarily via `yield()`
- **State Machine**: READY → RUNNING → BLOCKED transitions

### Phase 2: Synchronization & IPC
- **Mutex Implementation**: `lock()` and `unlock()` with blocking semantics
- **Wait Queue**: Blocked tasks are queued and woken in FIFO order
- **Ownership Handoff**: Fair scheduling prevents starvation

### Phase 3: Scheduling Algorithms
- **Priority Scheduling**: HIGH (0) and LOW (1) priority levels
- **Multi-Level Queues**: Separate ready queues per priority
- **Strict Priority**: High-priority tasks always run first

### Phase 4: Memory Management
- **Simulated RAM**: 1KB heap managed by MemoryManager
- **First-Fit Allocation**: Efficient block searching
- **Coalescing**: Adjacent free blocks are merged automatically

### Phase 5: Virtual File System
- **Persistent Storage**: Data saved to `disk.bin`
- **I-node Table**: Maps filenames to disk offsets
- **File Operations**: `my_open()`, `my_write()`, `my_read()`, `my_close()`

### Phase 6: Interactive Shell
- **REPL Interface**: Command-line shell for managing the OS
- **Process Commands**: `fork`, `thread`, `procs`, `spawn`
- **System Commands**: `ps`, `kill`, `run`, `mem`, `files`, `help`, `exit`
- **Dynamic Process/Thread Creation**: Create processes and threads at runtime with priority

## 🚀 Quick Start

### Prerequisites
- GCC with C++17 support
- Make

### Build & Run
```bash
git clone https://github.com/yourusername/MyOS.git
cd MyOS
make
./bin/os_sim
```

### Clean Build
```bash
make clean && make
```

### Run Shortcut
```bash
make run
```

## 📁 Project Structure

```
MyOS/
├── include/               # Header files
│   ├── Process.hpp
│   ├── Thread.hpp
│   ├── Scheduler.hpp
│   ├── Mutex.hpp
│   ├── MemoryManager.hpp
│   ├── FileSystem.hpp
│   ├── Shell.hpp
│   └── Kernel.hpp
├── src/                   # Source files
│   ├── *.cpp
│   └── main.cpp
├── build/                 # Compiled objects
├── bin/                   # Executable output
├── docs/images/           # Documentation assets
├── Makefile
├── .gitignore
├── .clang-format
├── LICENSE
└── README.md
```

## 🖥️ Shell Commands

| Command | Example | Description |
|---------|---------|-------------|
| `fork <name>` | `fork WebServer` | Create a new process with main thread |
| `thread <pid> <name> [p]` | `thread 1 Worker 0` | Create thread in process (0=HIGH, 1=LOW) |
| `spawn <name> [priority]` | `spawn Task 0` | Quick spawn (process + thread) |
| `procs` | `procs` | Show process tree with threads |
| `ps` | `ps` | List all threads with TID/PID |
| `run [cycles]` | `run 10` | Execute N CPU cycles |
| `kill <tid>` | `kill 2` | Terminate a thread by TID |
| `mem` | `mem` | Show memory allocation map |
| `files` | `files` | Show file system I-node table |
| `help` | `help` | Show command reference |
| `exit` | `exit` | Shutdown the OS |

### Shell Demo Session

```
MyOS> fork WebServer
[MemoryManager] Allocated 64 bytes at offset 0.
[Shell] Created process 'WebServer' (PID 1) with main thread

MyOS> thread 1 RequestHandler 0
[Shell] Created thread 'RequestHandler' (TID 2) in process 1 [HIGH]

MyOS> thread 1 Logger 1
[Shell] Created thread 'Logger' (TID 3) in process 1 [LOW]

MyOS> procs
┌────────────────────────────────────────────────────────┐
│                    Process List                        │
├────────────────────────────────────────────────────────┤
│ PID 1  : WebServer            [3 threads, 64 bytes]  │
│   ├─ Thread 1  : main         [HIGH] READY           │
│   ├─ Thread 2  : RequestHandl [HIGH] READY           │
│   └─ Thread 3  : Logger       [LOW ] READY           │
└────────────────────────────────────────────────────────┘

MyOS> run 15
[Shell] Running 15 CPU cycles...
Context Switch: Running Thread 1 (PID 1) [HIGH] (main)
  [CPU] Thread 1 (PID 1, main) executing instruction 0
  ...
  [CPU] Thread 1 (main) completed!
Context Switch: Running Thread 2 (PID 1) [HIGH] (RequestHandler)
  [CPU] Thread 2 (PID 1, RequestHandler) executing instruction 0
  ...
  [CPU] Thread 2 (RequestHandler) completed!
Context Switch: Running Thread 3 (PID 1) [LOW] (Logger)
  [CPU] Thread 3 (PID 1, Logger) executing instruction 0
  ...
  [CPU] Thread 3 (Logger) completed!

MyOS> exit
[Shell] Shutting down MyOS...
```

## 🧠 Key Concepts Demonstrated

| Concept | Implementation |
|---------|----------------|
| Context Switching | `Scheduler::yield()` saves/restores task state |
| Mutual Exclusion | `Mutex` with blocking wait queue |
| Priority Inversion | Handled via strict priority scheduling |
| Memory Fragmentation | First-Fit + Coalescing mitigates fragmentation |
| File Persistence | Binary I/O to `disk.bin` |

## 🔧 Technical Details

- **Language**: C++17
- **Concurrency Model**: Cooperative (no preemption)
- **Memory Model**: Flat addressing with offset-based allocation
- **File System**: Simple flat structure (no directories)

## 📚 Learning Resources

This project was built following concepts from:
- Operating Systems courses (Process Management, Synchronization, Memory, File Systems)
- "Operating Systems: Three Easy Pieces" (Arpaci-Dusseau)
- "Modern Operating Systems" (Tanenbaum)

## 🛠️ Future Improvements

- [ ] Round-robin time slicing with simulated timer
- [ ] Virtual memory with page tables
- [ ] Directory hierarchy in file system
- [ ] Multi-level feedback queue scheduler

## 📄 License

MIT License - Feel free to use this for learning!

---

*Built as a hands-on exploration of operating system internals.*
