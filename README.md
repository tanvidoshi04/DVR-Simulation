# Distance Vector Routing Simulation

## Overview

This project implements a simulation of the Distance Vector Routing (DVR) algorithm with optional mechanisms to prevent the count-to-infinity problem: **Poisoned Reverse** and **Split Horizon**. The program allows you to:

- Simulate a network of routers using the DVR algorithm.
- Observe how the Poisoned Reverse and Split Horizon techniques affect routing.
- Simulate link failures and see how the network adapts.
- Detect the count-to-infinity problem when it occurs.

## Files

- `dvr_simulation.cpp`: The main source code file containing the implementation of the DVR algorithm, Poisoned Reverse, and Split Horizon mechanisms.

## Prerequisites

- A C++ compiler that supports C++11 standard or later (e.g., `g++`).

## Compilation Instructions

You can compile the program using the provided Makefile or manually using `g++`.

### Using Makefile

- To compile the program, run:

  ```bash
  make
  ```

  This will compile `dvr_simulation.cpp` and produce an executable named `dvr_simulation`.

- To clean up the compiled executable and object files, run:

  ```bash
  make clean
  ```

### Manual Compilation

If you prefer to compile manually without the Makefile, use the following command:

```bash
g++ -o dvr_simulation dvr_simulation.cpp -std=c++11 -Wall -Wextra
```

## Running the Program

Execute the compiled program from the command line:

```bash
./dvr_simulation
```

The program will prompt you for input.

## Usage Instructions

When you run the program, you will be prompted to:

1. **Enter the number of nodes and edges:**

   ```plaintext
   Enter number of nodes (N) and edges (M): 
   ```

2. **Enter the edges (source, destination, cost):**

   For each edge, input the source node, destination node, and the cost between them:

   ```plaintext
   Enter the edges (source destination cost):
   ```

3. **Select the method to use:**

   ```plaintext
   Select method:
   1. None
   2. Poisoned Reverse
   3. Split Horizon
   Enter your choice (1-3): 
   ```

   - **1**: Run the basic DVR algorithm without any mechanism to prevent the count-to-infinity problem.
   - **2**: Use the **Poisoned Reverse** technique.
   - **3**: Use the **Split Horizon** technique.

## Example Input and Output

### Sample Input

```plaintext
Enter number of nodes (N) and edges (M): 5 7
Enter the edges (source destination cost):
1 2 2
1 3 5
2 3 3
2 4 1
3 4 2
3 5 3
4 5 1
Select method:
1. None
2. Poisoned Reverse
3. Split Horizon
Enter your choice (1-3): 2
```

### Sample Output

The program will display the routing tables after running the DVR algorithm, simulate a link failure, and display the updated routing tables.

```plaintext
Routing tables after running DVR algorithm with Poisoned Reverse:
Routing table for Node 1:
Destination	Cost	Next Hop
1		0	1
2		2	2
3		5	2
4		3	2
5		4	2

...

Simulating link failure between Node 4 and Node 5.

Routing tables after link failure with Poisoned Reverse:
Routing table for Node 1:
Destination	Cost	Next Hop
1		0	1
2		2	2
3		5	2
4		3	2
5		7	2

...

```

## Simulating Link Failure

- The program simulates a link failure between **Node 4** and **Node 5**.
- This is hardcoded in the source code for demonstration purposes.
- You can modify the nodes involved in the link failure by editing the following lines in `dvr_simulation.cpp`:

  ```cpp
  int failSrc = 4, failDest = 5;
  ```

## Notes

- The program assumes nodes are numbered starting from **1**.
- The maximum cost to represent infinity is set to **999**.
- The count-to-infinity threshold is set to **100** for detection purposes.


## Instructions for Using the Makefile:

1. **Compile the Program:**

   - Open a terminal and navigate to the directory containing the `dvr_simulation.cpp` and `Makefile` files.
   - Run the following command:

     ```bash
     make
     ```

   - This will compile the source code and create an executable named `dvr_simulation`.

2. **Run the Program:**

   - Execute the program by running:

     ```bash
     ./dvr_simulation
     ```

   - Follow the on-screen prompts to input the network configuration and select the method.

3. **Clean Up:**

   - To remove the compiled executable and object files, run:

     ```bash
     make clean
     ```

---

