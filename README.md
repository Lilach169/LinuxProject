##Linux Project - Graph Path Simulation

#Team Members
Lilach Rachmani - 314843145
Oranne Berrih - 332671312

#Project Description:
This project is a visual simulation tool that finds and animates the shortest path between nodes in a graph. The shortest path is calculated using Dijkstra's Algorithm, while the graphical interface and animations are implemented using the Raylib library in C.

#Milestone 1 - Dijkstra Algorithm

What We Did:
* Implemented Dijkstra's shortest path algorithm.
* Built graph data structures from an input file.
* Added input validation and graph parsing.

Compile: make milestone1

Run: ./dijkstra <input_file>

#Milestone 2 - Graph Visualization

What We Did:
* Integrated the Raylib graphics library.
* Displayed graph nodes and edges visually.
* Added a PLAY button and basic GUI controls.
* Created a graphical representation of the loaded graph.

Compile: make milestone2

Run: ./sim <input_file>

#Milestone 3 - Animation System

What We Did:
* Added animated traveler movement along the shortest path.
* Implemented movement timing according to edge weights.
* Added 0.3-second movement steps and 1-second pauses at intermediate nodes.
* Improved graph styling and user interface.
* Added destination arrival indication.

Compile: make milestone3

Run: ./sim <input_file>

#Milestone 4 - Multiple Processes and Travelers

What We Did:
* Added support for multiple travelers moving simultaneously.
* Implemented parent-child architecture using fork().
* The parent process calculates Dijkstra paths and manages the GUI.
* Each traveler is displayed in a unique color.
* Child processes print their PID upon creation and remain active during the simulation.
* The parent process terminates child processes when their travelers reach the destination.
* Added synchronization using waitpid() before program termination.

Compile: make milestone4

Run: ./sim <input_file>

Milestone 5:

In this milestone, each child process calculates its own shortest path using Dijkstra’s algorithm.
The child processes communicate their current node and next node to the parent process using pipes.
The parent process receives these messages, prints the required log format to the terminal, and updates the GUI animation.

IPC choice:
We chose pipes because they are simple, built-in Linux IPC mechanism, and fit the project requirement of sending small messages from each child process to the parent process.
Each traveler has its own pipe, so the parent can read updates from every child separately.

How to compile: make milestone5

How to run: ./sim input.txt

Milestone 6:

Added synchronization for graph nodes.
Each node is protected by a semaphore so only one traveler can stay inside a node at a time.
Travelers arriving while a node is occupied wait outside the node until it becomes available.
The GUI marks waiting travelers with an orange ring.

How to compile: make milestone6

How to run: ./sim input.txt

#Milestone 7:
Added scheduling algorithms for node access management/
When multiple travelers are waiting to enter the same node, the parent process now manages a waiting queue and selects which traveler may enter according to the scheduling algorithm chosen at runtime.

Implemented scheduling algorithms:

1. FCFS (First Come First Served)
    Travelers enter the node according to the order in which they arrived at the waiting queue.

2. SJF (Shortest Job First)
   Each traveler has a burstTime value in the input file.
   When several travelers wait for the same node, the traveler with the smallest burstTime is selected first.

The scheduling algorithm is selected from the command line without changing the code.

Input format for Milestone 7:
source destination burstTime

The GUI displays the active scheduling algorithm and the number of waiting travelers.

How to compile:

make milestone7

How to run:

./sim -schd fcfs input_sjf.txt

./sim -schd sjf input_sjf.txt

Example:
FCFS:
Travelers enter the shared node according to their arrival order.

SJF:
Travelers enter the shared node according to the smallest burstTime.

#Responsibilities:

Lilach Rachmani:

-Implemented Dijkstra's algorithm and graph processing.

-Managed input file parsing and validation.

-Designed and improved the graphical user interface.

-Integrated process management and milestone functionality.

-Implemented node synchronization and semaphore-based access control for Milestone 6.

-Implemented scheduling queues and scheduler selection logic for Milestone 7.

-Integrated FCFS and SJF scheduling algorithms into node access management.

Oranne Berrih:

-Implemented traveler animation and movement logic.

-Developed timing and node transition behavior.

-Tested different graph scenarios and edge cases.

-Performed debugging and quality assurance testing.

-Assisted in synchronization testing and traveler waiting-state visualization.

-Assisted in testing and comparing FCFS and SJF behavior.

-Added traveler status visualization and scheduling-related GUI updates.

-Validated scheduling scenarios and simulation outputs.
