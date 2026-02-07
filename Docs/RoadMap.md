Here is a detailed, phased roadmap for your project. This structure allows you to build a "simulated kernel" in C++ first, which is the most instructive way to understand the logic before worrying about hardware-specific assembly code.

### OS Development Roadmap: From Threads to a Functional System

#### Phase 1: The Process & Thread Manager (Current)

*Focus: Chapters 2.1 & 2.2*
In this phase, you implement the core "Loop" of an OS. Since you don't have a hardware timer yet, you will simulate one.

* **Task Control Block (TCB):** Create a class to store thread ID, state (Ready, Running, Blocked), and a simulated Program Counter.
* **The Run-time System:** Implement a user-space thread library. Create a `thread_create` and `thread_yield` function.
* **Voluntary Switching:** Since you lack interrupts, threads must call `yield()` to let the next thread run.
* **Deliverable:** A program where three different functions (threads) print their IDs in a rotating sequence controlled by your scheduler.

#### Phase 2: Synchronization & IPC

*Focus: Chapter 2.3*
Once you have multiple threads, they will eventually try to access the same data.

* **Race Condition Simulation:** Create a shared counter and show how multiple threads incrementing it leads to errors.
* **The Sleep/Wakeup Mechanism:** Implement `sleep()` and `wakeup()` primitives.
* **Semaphores & Mutexes:** Build a `Mutex` class. If a thread tries to `lock()` a mutex held by another, your scheduler should move that thread to a `BLOCKED` queue.
* **Deliverable:** A working "Producer-Consumer" simulation using your own Mutex/Semaphore classes.

#### Phase 3: Scheduling Algorithms

*Focus: Chapter 2.4*
Now that you have a queue of threads, you need a strategy to pick the next one.

* **Round Robin:** Implement a fixed time slice (e.g., each thread executes 5 "instructions" or loops before being swapped).
* **Priority Scheduling:** Give some tasks higher priority. Implement a "Multiple Queues" system where high-priority tasks are checked first.
* **Deliverable:** A scheduler that handles both "Interactive" (high priority) and "Batch" (low priority) simulated tasks.

#### Phase 4: Basic Memory Management

*Focus: Chapter 3*
In the course, you'll move from Processes to Memory. Your "OS" needs to manage "RAM".

* **The Memory Manager:** Create a large `char` array to represent physical RAM.
* **Fixed Partitioning:** Divide that array into fixed blocks and assign one block to each TCB.
* **Dynamic Allocation:** Implement a simple `malloc` (First-fit or Best-fit algorithm) to manage the simulated RAM.
* **Deliverable:** A system where threads can request "memory blocks" from the kernel and must free them when done.

#### Phase 5: The Virtual File System (VFS)

*Focus: Chapter 4*
Processes need to store data. You will simulate a disk using a real file on your Arch Linux system.

* **The I-node Table:** Create a structure that maps a "filename" to a position in your simulated disk file.
* **File Descriptors:** Add an array to your TCB that keeps track of which "files" a specific thread has open.
* **Deliverable:** Threads can call `my_open()`, `my_write()`, and `my_close()`, with data persisting inside a `disk.bin` file.

---

### Suggested C++ Project Organization

```cpp
// A simplified view of how the components interact
class Kernel {
private:
    Scheduler scheduler;
    MemoryManager memManager;
    FileSystem fileSystem;

public:
    void boot() {
        // 1. Initialize hardware simulations
        // 2. Load initial tasks
        // 3. Start the execution loop
    }

    void interruptHandler() {
        // This is where Phase 1 & 3 logic lives
        scheduler.pickNextTask();
    }
};

```

### Why this approach?

* **Portability:** You can code and debug this entirely on **Arch Linux** using standard tools like `g++` and `gdb`.
* **No "Magic":** By writing the `Scheduler` yourself, you understand exactly how `context switching` works before you eventually have to write it in Assembly for real hardware.
* **Scalability:** When you reach the File System chapter in your course, you already have the `Task` structures ready to receive `File Descriptors`.
