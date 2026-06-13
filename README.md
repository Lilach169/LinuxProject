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

#Responsibilities:

Lilach Rachmani-
* Implemented Dijkstra's algorithm and graph processing.
* Managed input file parsing and validation.
* Designed and improved the graphical user interface.
* Integrated process management and milestone functionality.

Oranne Berrih-
* Implemented traveler animation and movement logic.
* Developed timing and node transition behavior.
* Tested different graph scenarios and edge cases.
* Performed debugging and quality assurance testing.
