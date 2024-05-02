# Real-Time Task Scheduler

## Objective
Design and implement a real-time task scheduler in C that can simulate scheduling algorithms used in real-time operating systems.

## Topics
- Real-time systems
- Scheduling algorithms (like Rate Monotonic Scheduling, Earliest Deadline First)
- Task prioritization

## Coding Product
A simulator that can create and manage real-time tasks, each with specific execution times and deadlines, and then use different algorithms to schedule these tasks to meet their deadlines.

## MVP Description: Real-Time Task Scheduler Simulator
### Core Functionality
- **Task Creation and Management:** Allow the user to define a set of tasks, each with specific execution times, priorities, and deadlines.
- **Scheduling Algorithms:** Implement at least two real-time scheduling algorithms such as Rate Monotonic Scheduling (RMS) and Earliest Deadline First (EDF). The system should be able to select and switch between these algorithms dynamically based on user input.
- **Task Execution Simulation:** Simulate the execution of tasks according to the selected scheduling algorithm, showing how tasks are prioritized and handled.

### Integration with Class Topics
- **UNIX:** Utilize POSIX threads for task handling, demonstrating process creation and management in a UNIX-like environment. Leverage UNIX system calls for time management and thread synchronization.
- **Process Communication:** Implement mechanisms for inter-thread communication using shared memory or message passing, allowing threads (tasks) to coordinate or exchange status updates.
- **Process Synchronization:** Use mutexes and semaphores to manage access to shared resources between tasks, showcasing critical sections and avoiding race conditions.
- **File Systems:** Log task statuses and scheduler operations to a file system, providing a trace of scheduler behavior and task lifecycle for analysis. This would involve file operations like open, write, read, and close.
- **Memory Management:** Demonstrate dynamic memory allocation and deallocation for task data structures, and potentially integrate custom memory management strategies for task buffers or queues.

### Suggested MVP
1. **Setup Basic Environment:** Establish the basic C project structure, integrating necessary libraries for thread management and file handling.
2. **Task Structure Definition:** Define a C structure for tasks, including attributes like ID, priority, execution time, and deadline.
3. **Scheduler Implementation:** Write the core functions to add tasks to the system, choose the scheduling algorithm, and execute tasks according to the algorithm's logic.
4. **Logging System:** Implement a simple logging system that writes task states and scheduler decisions to a file.
5. **User Interface:** Create a simple command-line interface that allows users to input task parameters, select a scheduling algorithm, and start the simulation.
6. **Testing and Validation:** Simulate scenarios with different tasks to see how each scheduling algorithm performs and ensure the system behaves as expected under various loads.
