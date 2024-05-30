###### [English Version Below]
## Umgebung einrichten:
- Docker-Container ausführen
- Öffnen von CLion
- Wechseln zur richtigen CMake-Datei in CLion

## Server starten:
- Starten des CLion-Programms
- Überprüfen des Debug-Protokolls

## Mit dem Server verbinden:
- Öffnen des Terminals (Wenn es mehrere sind, mehrere Terminals öffnen)
- Verbindung zum Docker-Container herstellen
    - `docker exec -it [containerID] /bin/bash`
- Verbindung zum Server über Telnet herstellen
    - `telnet [IP] [PORT]`
- Der über CLion verbundene Client ist sichtbar

## Befehle ausführen:
- Ausführen von PUT-, GET-, DEL- und QUIT-Befehlen über das Client-Terminal


## Environment Setup:
- Running the Docker container
- Opening CLion
- Navigating to the correct CMake file in CLion

## Running the Server:
- Launching the CLion program
- Checking the debug log

## Connecting to the Server:
- Opening the terminal (If it's multi, open multiple terminals)
- Connecting to the Docker container
    - `docker exec -it [containerID] /bin/bash`
- Connecting to the server via telnet
    - `telnet [IP] [PORT]`
- The client connected through CLion is visible

## Executing Commands:
- Running PUT, GET, DEL, and QUIT commands via the client terminal

<h2 style="color:pink;">Semaphores</h2>
- Semaphores are synchronization tools used to 
control concurrent access in multi-threaded or 
multi-processed systems. 
In many cases, multiple processes or threads may 
attempt to access shared resources simultaneously, 
and it's necessary to ensure orderly access and prevent collisions.

<h2 style="color:papayawhip;">Task</h2>
- Multiclient capability
  - Extend your socket server's program so that instead of 
    a queue for socket connection for multiple clients, 
    all clients can interact with the server at the same time. 
    Think about how you can use processes and shared memory to 
    manage the exchange of data in the key-value store.
<br> <br>
- Solution
  - We created a new process for each new connection using 
    the fork() system call.
  <br> <br>
  - Shared memory was created using the mmap() system call
  to create the main memory area.
  <br> <br>
  - We used semaphores to synchronize data access. 
  Specifically, semaphores were used to control concurrent 
  access to the shared memory where the data resides. 
  Semaphores regulate the access of multiple threads or 
  processes to the shared memory and maintain controlled 
  concurrent access.
<br>
<br>
<br>
<br>