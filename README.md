# Real-Time Task Scheduler

## Objective
Design and implement a real-time task scheduler in C that can simulate scheduling algorithms used in both single-system and distributed real-time operating systems.

## Topics
- Real-time systems
- Distributed systems
- Scheduling algorithms (like Rate Monotonic Scheduling, Earliest Deadline First)
- Task prioritization
- Fault tolerance and redundancy
- Inter-node communication

## Coding Product
A simulator that can create and manage real-time tasks, each with specific execution times and deadlines, and then use different algorithms to schedule these tasks to meet their deadlines across potentially multiple nodes.

## MVP Description: Real-Time Task Scheduler Simulator
### Core Functionality
- **Task Creation and Management:** Allow the user to define a set of tasks, each with specific execution times, priorities, and deadlines.
- **Scheduling Algorithms:** Implement at least two real-time scheduling algorithms such as Rate Monotonic Scheduling (RMS) and Earliest Deadline First (EDF). The system should be able to select and switch between these algorithms dynamically based on user input.
- **Task Execution Simulation:** Simulate the execution of tasks according to the selected scheduling algorithm, showing how tasks are prioritized and handled.
- **Distributed Scheduling:** Extend the scheduler to manage tasks across multiple nodes, handling node-specific task assignments and ensuring effective load balancing and fault tolerance.

### Integration with Class Topics
- **UNIX:** Utilize POSIX threads for task handling within and across UNIX-like environments, demonstrating advanced process and thread management techniques.
- **Process Communication:** Implement mechanisms for efficient inter-thread and inter-node communication using shared memory or message passing.
- **Process Synchronization:** Use mutexes and semaphores to synchronize tasks across different nodes, managing access to shared resources effectively.
- **File Systems:** Log task statuses and scheduler operations to a file system, providing detailed traces of behavior across distributed nodes.
- **Memory Management:** Demonstrate dynamic memory allocation and deallocation for task data structures in a distributed context.

### Suggested MVP Steps
1. **Setup Basic Environment:** Establish the basic C project structure, integrating necessary libraries for thread management and file handling.
2. **Task Structure Definition:** Define a C structure for tasks, including attributes like ID, priority, execution time, and deadline.
3. **Scheduler Implementation:** Write the core functions to add tasks to the system, choose the scheduling algorithm, and execute tasks according to the algorithm's logic.
4. **Distributed Environment Setup:** Simulate a distributed environment with multiple nodes, and implement functionality for tasks to be assigned and executed across these nodes.
5. **Logging System:** Implement a logging system that writes task states and scheduler decisions to a file, including details about inter-node operations and fault handling.
6. **User Interface:** Create a command-line interface that allows users to input task parameters, select a scheduling algorithm, configure nodes, and start the simulation.
7. **Testing and Validation:** Simulate scenarios with varying numbers of tasks and nodes to test the efficiency of the scheduler in a distributed setting.


