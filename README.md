# MyOS - Educational Operating System Simulation

<p align="center">
  <img src="Docs/OS.png" alt="MyOS Logo" width="400"/>
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
              │            Tasks              │
              │  • State (READY/RUNNING/      │
              │    BLOCKED)                   │
              │  • Priority (HIGH/LOW)        │
              │  • Program Counter            │
              └───────────────────────────────┘
```

## ✨ Features

### Phase 1: Process & Thread Manager
- **Task Control Block (TCB)**: Stores task ID, state, name, and simulated program counter
- **Cooperative Scheduling**: Tasks yield control voluntarily via `yield()`
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
- **Commands**: `spawn`, `ps`, `kill`, `run`, `mem`, `files`, `help`, `exit`
- **Dynamic Task Creation**: Spawn tasks at runtime with priority

## 🚀 Quick Start

### Prerequisites
- GCC with C++17 support
- Make

### Build & Run
```bash
git clone https://github.com/yourusername/MyOS.git
cd MyOS
make
./os_sim
```

### Clean Build
```bash
make clean && make
```

## 📁 Project Structure

```
MyOS/
├── include/
│   ├── Task.hpp           # Task Control Block
│   ├── Scheduler.hpp      # Priority scheduler
│   ├── Mutex.hpp          # Synchronization primitive
│   ├── MemoryManager.hpp  # Heap allocator
│   ├── FileSystem.hpp     # VFS with I-nodes
│   ├── Shell.hpp          # Interactive shell
│   └── Kernel.hpp         # Core orchestrator
├── src/
│   ├── Task.cpp
│   ├── Scheduler.cpp
│   ├── Mutex.cpp
│   ├── MemoryManager.cpp
│   ├── FileSystem.cpp
│   ├── Shell.cpp
│   ├── Kernel.cpp
│   └── main.cpp
├── Makefile
└── README.md
```

## 🖥️ Shell Commands

| Command | Example | Description |
|---------|---------|-------------|
| `spawn <name> [priority]` | `spawn Worker 0` | Create a task (0=HIGH, 1=LOW) |
| `ps` | `ps` | List all tasks with status |
| `run [cycles]` | `run 10` | Execute N CPU cycles |
| `kill <id>` | `kill 2` | Terminate a task by ID |
| `mem` | `mem` | Show memory allocation map |
| `files` | `files` | Show file system I-node table |
| `help` | `help` | Show command reference |
| `exit` | `exit` | Shutdown the OS |

### Shell Demo Session

```
MyOS> spawn HighPriorityTask 0
[Shell] Spawned task 'HighPriorityTask' with ID 1 [HIGH]

MyOS> spawn LowPriorityTask 1
[Shell] Spawned task 'LowPriorityTask' with ID 2 [LOW]

MyOS> ps
┌─────┬────────────────────┬──────────┬──────────┐
│ ID  │ Name               │ Priority │ State    │
├─────┼────────────────────┼──────────┼──────────┤
│ 1   │ HighPriorityTask   │ HIGH     │ READY    │
│ 2   │ LowPriorityTask    │ LOW      │ READY    │
└─────┴────────────────────┴──────────┴──────────┘

MyOS> run 15
[Shell] Running 15 CPU cycles...
Context Switch: Running Task 1 [HIGH] (HighPriorityTask)
  [CPU] Task 1 (HighPriorityTask) executing instruction 0
  ...
  [CPU] Task 1 (HighPriorityTask) completed!
Context Switch: Running Task 2 [LOW] (LowPriorityTask)
  [CPU] Task 2 (LowPriorityTask) executing instruction 0
  ...
  [CPU] Task 2 (LowPriorityTask) completed!

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
