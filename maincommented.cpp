#include <iostream>   // Include the standard input/output stream library
#include <vector>     // Include the vector container class
#include <map>        // Include the map container class
#include <algorithm>  // Include standard algorithms (e.g., remove_if)
using namespace std;  // Use the standard namespace

const int INFINITY = 999; // Define a constant to represent infinity (unreachable distance)

// Define a structure to represent an edge in the network graph
struct Edge {
    int src;    // Source node identifier
    int dest;   // Destination node identifier
    int cost;   // Cost associated with the edge
};

// Define a structure to represent a node (router) in the network
struct Node {
    int id;                             // Unique identifier for the node
    map<int, int> distanceVector;       // Distance vector: Destination -> Cost
    map<int, int> nextHop;              // Next hop table: Destination -> Next Hop
    vector<int> neighbors;              // List of neighboring nodes
};

// Function to initialize the nodes and their neighbors based on the edges
void initializeNodes(vector<Node>& nodes, vector<Edge>& edges, int N) {
    nodes.resize(N + 1);            // Resize the nodes vector to accommodate N nodes (1-based indexing)
    for (int i = 1; i <= N; ++i) {  // Loop over each node identifier from 1 to N
        nodes[i].id = i;            // Assign the node ID
    }
    for (auto& edge : edges) {                              // Iterate over each edge in the edges vector
        nodes[edge.src].neighbors.push_back(edge.dest);     // Add the destination node to the source node's neighbors
        nodes[edge.dest].neighbors.push_back(edge.src);     // Add the source node to the destination node's neighbors (undirected graph)
    }
}

// Function to initialize the distance vectors and next hop tables for each node
void initializeDistanceVectors(vector<Node>& nodes, vector<Edge>& edges, int N) {
    for (int i = 1; i <= N; ++i) {  // Loop over each node from 1 to N
        nodes[i].distanceVector.clear();  // Clear the existing distance vector for the node
        nodes[i].nextHop.clear();         // Clear the existing next hop table for the node
        for (int j = 1; j <= N; ++j) {    // Loop over each possible destination node
            if (i == j) {                 // If the destination is the node itself
                nodes[i].distanceVector[j] = 0;  // Set the distance to self as 0
                nodes[i].nextHop[j] = j;         // Set the next hop to self
            } else {                       // If the destination is another node
                nodes[i].distanceVector[j] = INFINITY;  // Initialize the distance to infinity
                nodes[i].nextHop[j] = -1;               // Initialize the next hop as unknown (-1)
            }
        }
    }

    for (auto& edge : edges) {                                    // Iterate over each edge
        nodes[edge.src].distanceVector[edge.dest] = edge.cost;    // Set the distance to the neighbor
        nodes[edge.src].nextHop[edge.dest] = edge.dest;           // Set the next hop to the neighbor

        nodes[edge.dest].distanceVector[edge.src] = edge.cost;    // Set the distance in the reverse direction
        nodes[edge.dest].nextHop[edge.src] = edge.src;            // Set the next hop in the reverse direction
    }
}

// Function to update the distance vectors of all nodes based on the selected method
bool updateDistanceVectors(vector<Node>& nodes, int N, int method) {
    bool updated = false;                   // Flag to indicate if any updates occurred
    for (int i = 1; i <= N; ++i) {          // Loop over each node
        for (int neighbor : nodes[i].neighbors) {  // Loop over each neighbor of the node
            map<int, int> dvToSend = nodes[i].distanceVector;  // Create a copy of the node's distance vector to send

            if (method == 2) {  // If the method is Poisoned Reverse
                for (auto& entry : dvToSend) {  // Iterate over each entry in the distance vector
                    int dest = entry.first;     // Get the destination node
                    if (nodes[i].nextHop[dest] == neighbor && dest != neighbor) {  // If the neighbor is the next hop to dest
                        dvToSend[dest] = INFINITY;  // Set the distance to infinity (poison the route)
                    }
                }
            } else if (method == 3) {  // If the method is Split Horizon
                for (auto it = dvToSend.begin(); it != dvToSend.end();) {  // Use an iterator to modify the map during iteration
                    int dest = it->first;  // Get the destination node
                    if (nodes[i].nextHop[dest] == neighbor && dest != neighbor) {  // If the neighbor is the next hop to dest
                        it = dvToSend.erase(it);  // Remove the entry from the map and update the iterator
                    } else {
                        ++it;  // Move to the next entry
                    }
                }
            }
            // Now, update the neighbor's distance vector based on the information sent
            for (auto& entry : dvToSend) {  // Iterate over each entry in the distance vector to send
                int dest = entry.first;      // Get the destination node
                int cost = entry.second;     // Get the cost to the destination
                int newCost = nodes[neighbor].distanceVector[i] + cost;  // Calculate the new cost via node i
                if (newCost < nodes[neighbor].distanceVector[dest]) {    // If the new cost is less than the existing cost
                    nodes[neighbor].distanceVector[dest] = newCost;  // Update the distance vector with the new cost
                    nodes[neighbor].nextHop[dest] = i;               // Update the next hop to node i
                    updated = true;                                  // Set the updated flag to true
                }
            }
        }
    }
    return updated;  // Return whether any updates occurred
}

// Function to print the routing tables for all nodes
void printRoutingTables(const vector<Node>& nodes, int N) {
    for (int i = 1; i <= N; ++i) {  // Loop over each node
        cout << "Routing table for Node " << i << ":\n";  // Print the header for the node's routing table
        cout << "Destination\tCost\tNext Hop\n";          // Print the table headers
        for (int j = 1; j <= N; ++j) {  // Loop over each possible destination node
            if (nodes[i].distanceVector.at(j) >= INFINITY) {  // If the destination is unreachable
                cout << j << "\t\t" << "INF" << "\t" << "-\n";  // Print INF and - for cost and next hop
            } else {  // If the destination is reachable
                cout << j << "\t\t" << nodes[i].distanceVector.at(j) << "\t" << nodes[i].nextHop.at(j) << "\n";  // Print the cost and next hop
            }
        }
        cout << "\n";  // Print a newline for readability
    }
}

// Function to check for the count-to-infinity problem in the network
bool checkCountToInfinity(const vector<Node>& nodes, int N) {
    bool countToInfinity = false;  // Flag to indicate if the problem is detected
    for (int i = 1; i <= N; ++i) {  // Loop over each node
        for (auto& entry : nodes[i].distanceVector) {  // Iterate over each entry in the node's distance vector
            if (entry.second > 100 && entry.second < INFINITY) {  // If the distance exceeds the threshold but is not infinite
                cout << "Node " << i << " has distance >100 to Node " << entry.first << ".\n";  // Report the issue
                countToInfinity = true;  // Set the flag to true
            }
        }
    }
    return countToInfinity;  // Return whether the problem was detected
}

// Main function to execute the DVR simulation
int main() {
    int N, M;  // Variables to store the number of nodes and edges
    cout << "Enter number of nodes (N) and edges (M): ";
    cin >> N >> M;  // Read the number of nodes and edges from the user

    vector<Edge> edges(M);  // Vector to store the edges in the network
    cout << "Enter the edges (source destination cost):\n";
    for (int i = 0; i < M; ++i) {  // Loop to read each edge
        cin >> edges[i].src >> edges[i].dest >> edges[i].cost;  // Read the source, destination, and cost of the edge
    }

    int method;  // Variable to store the user's choice of method
    cout << "Select method:\n";  // Prompt the user to select a method
    cout << "1. None\n";  // Option 1: Basic DVR without any mechanism
    cout << "2. Poisoned Reverse\n";  // Option 2: DVR with Poisoned Reverse
    cout << "3. Split Horizon\n";  // Option 3: DVR with Split Horizon
    cout << "Enter your choice (1-3): ";
    cin >> method;  // Read the user's choice

    vector<Node> nodes;  // Vector to store the nodes in the network
    initializeNodes(nodes, edges, N);  // Initialize the nodes and their neighbors
    initializeDistanceVectors(nodes, edges, N);  // Initialize the distance vectors and next hop tables

    bool updated;  // Flag to indicate if updates occurred during the algorithm
    do {
        updated = updateDistanceVectors(nodes, N, method);  // Update the distance vectors based on the selected method
    } while (updated);  // Continue updating until no changes occur (convergence)

    // Print the routing tables after the DVR algorithm has converged
    cout << "\nRouting tables after running DVR algorithm";
    if (method == 2) {  // If Poisoned Reverse was selected
        cout << " with Poisoned Reverse";
    } else if (method == 3) {  // If Split Horizon was selected
        cout << " with Split Horizon";
    }
    cout << ":\n";
    printRoutingTables(nodes, N);  // Call the function to print the routing tables

    int failSrc = 4, failDest = 5;  // Nodes between which the link will be removed to simulate failure
    cout << "Simulating link failure between Node " << failSrc << " and Node " << failDest << ".\n";

    // Remove the edge between failSrc and failDest from the edges vector
    edges.erase(remove_if(edges.begin(), edges.end(), [failSrc, failDest](Edge& e) {
        return (e.src == failSrc && e.dest == failDest) || (e.src == failDest && e.dest == failSrc);  // Identify the edge to remove
    }), edges.end());  // Erase the edge from the vector

    // Remove failDest from failSrc's neighbors list
    nodes[failSrc].neighbors.erase(remove(nodes[failSrc].neighbors.begin(), nodes[failSrc].neighbors.end(), failDest),
                                   nodes[failSrc].neighbors.end());  // Update failSrc's neighbors

    // Remove failSrc from failDest's neighbors list
    nodes[failDest].neighbors.erase(remove(nodes[failDest].neighbors.begin(), nodes[failDest].neighbors.end(), failSrc),
                                    nodes[failDest].neighbors.end());  // Update failDest's neighbors

    initializeDistanceVectors(nodes, edges, N);  // Re-initialize the distance vectors after the link failure

    bool countToInfinity = false;  // Flag to indicate if the count-to-infinity problem is detected
    do {
        updated = updateDistanceVectors(nodes, N, method);  // Update the distance vectors after the link failure
        countToInfinity = checkCountToInfinity(nodes, N);  // Check for the count-to-infinity problem
        if (countToInfinity) {  // If the problem is detected
            cout << "Count-to-infinity problem detected.\n";  // Report the issue
            break;  // Exit the loop
        }
    } while (updated);  // Continue updating until no changes occur or the problem is detected

    // Print the routing tables after the link failure
    cout << "\nRouting tables after link failure";
    if (method == 2) {  // If Poisoned Reverse was selected
        cout << " with Poisoned Reverse";
    } else if (method == 3) {  // If Split Horizon was selected
        cout << " with Split Horizon";
    }
    cout << ":\n";
    printRoutingTables(nodes, N);  // Call the function to print the routing tables

    return 0;  // Return 0 to indicate successful execution
}
