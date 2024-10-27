#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

const int INFINITY = 999; // Representation of infinity

struct Edge {
    int src;
    int dest;
    int cost;
};

struct Node {
    int id;
    map<int, int> distanceVector; // Destination -> Cost
    map<int, int> nextHop;        // Destination -> Next hop
    vector<int> neighbors;
};

void initializeNodes(vector<Node>& nodes, vector<Edge>& edges, int N) {
    nodes.resize(N + 1); // Assuming nodes are numbered from 1 to N
    for (int i = 1; i <= N; ++i) {
        nodes[i].id = i;
    }
    for (auto& edge : edges) {
        nodes[edge.src].neighbors.push_back(edge.dest);
        nodes[edge.dest].neighbors.push_back(edge.src);
    }
}

void initializeDistanceVectors(vector<Node>& nodes, vector<Edge>& edges, int N) {
    for (int i = 1; i <= N; ++i) {
        nodes[i].distanceVector.clear();
        nodes[i].nextHop.clear();
        for (int j = 1; j <= N; ++j) {
            if (i == j) {
                nodes[i].distanceVector[j] = 0;
                nodes[i].nextHop[j] = j;
            } else {
                nodes[i].distanceVector[j] = INFINITY;
                nodes[i].nextHop[j] = -1;
            }
        }
    }

    // Set the costs for directly connected neighbors
    for (auto& edge : edges) {
        nodes[edge.src].distanceVector[edge.dest] = edge.cost;
        nodes[edge.src].nextHop[edge.dest] = edge.dest;

        nodes[edge.dest].distanceVector[edge.src] = edge.cost;
        nodes[edge.dest].nextHop[edge.src] = edge.src;
    }
}

bool updateDistanceVectors(vector<Node>& nodes, int N, int method) {
    bool updated = false;
    for (int i = 1; i <= N; ++i) {
        for (int neighbor : nodes[i].neighbors) {
            // For each neighbor, create a copy of the distance vector to send
            map<int, int> dvToSend = nodes[i].distanceVector;

            if (method == 2) {
                // Poisoned Reverse
                for (auto& entry : dvToSend) {
                    int dest = entry.first;
                    if (nodes[i].nextHop[dest] == neighbor && dest != neighbor) {
                        dvToSend[dest] = INFINITY;
                    }
                }
            } else if (method == 3) {
                // Split Horizon
                for (auto it = dvToSend.begin(); it != dvToSend.end();) {
                    int dest = it->first;
                    if (nodes[i].nextHop[dest] == neighbor && dest != neighbor) {
                        // Remove the route from the advertisement
                        it = dvToSend.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
            // Now, neighbor updates its distance vector based on the received dvToSend
            for (auto& entry : dvToSend) {
                int dest = entry.first;
                int cost = entry.second;
                int newCost = nodes[neighbor].distanceVector[i] + cost;
                if (newCost < nodes[neighbor].distanceVector[dest]) {
                    nodes[neighbor].distanceVector[dest] = newCost;
                    nodes[neighbor].nextHop[dest] = i;
                    updated = true;
                }
            }
        }
    }
    return updated;
}

void printRoutingTables(const vector<Node>& nodes, int N) {
    for (int i = 1; i <= N; ++i) {
        cout << "Routing table for Node " << i << ":\n";
        cout << "Destination\tCost\tNext Hop\n";
        for (int j = 1; j <= N; ++j) {
            if (nodes[i].distanceVector.at(j) >= INFINITY) {
                cout << j << "\t\t"
                     << "INF"
                     << "\t"
                     << "-\n";
            } else {
                cout << j << "\t\t" << nodes[i].distanceVector.at(j) << "\t" << nodes[i].nextHop.at(j) << "\n";
            }
        }
        cout << "\n";
    }
}

bool checkCountToInfinity(const vector<Node>& nodes, int N) {
    bool countToInfinity = false;
    for (int i = 1; i <= N; ++i) {
        for (auto& entry : nodes[i].distanceVector) {
            if (entry.second > 100 && entry.second < INFINITY) {
                cout << "Node " << i << " has distance >100 to Node " << entry.first << ".\n";
                countToInfinity = true;
            }
        }
    }
    return countToInfinity;
}

int main() {
    int N, M;
    cout << "Enter number of nodes (N) and edges (M): ";
    cin >> N >> M;

    vector<Edge> edges(M);
    cout << "Enter the edges (source destination cost):\n";
    for (int i = 0; i < M; ++i) {
        cin >> edges[i].src >> edges[i].dest >> edges[i].cost;
    }

    int method;
    cout << "Select method:\n";
    cout << "1. None\n";
    cout << "2. Poisoned Reverse\n";
    cout << "3. Split Horizon\n";
    cout << "Enter your choice (1-3): ";
    cin >> method;

    vector<Node> nodes;
    initializeNodes(nodes, edges, N);
    initializeDistanceVectors(nodes, edges, N);

    // Run the DVR algorithm until convergence
    bool updated;
    do {
        updated = updateDistanceVectors(nodes, N, method);
    } while (updated);

    cout << "\nRouting tables after running DVR algorithm";
    if (method == 2) {
        cout << " with Poisoned Reverse";
    } else if (method == 3) {
        cout << " with Split Horizon";
    }
    cout << ":\n";
    printRoutingTables(nodes, N);

    // Simulate link failure
    int failSrc = 4, failDest = 5;
    cout << "Simulating link failure between Node " << failSrc << " and Node " << failDest << ".\n";

    // Remove the edge from edges list
    edges.erase(remove_if(edges.begin(), edges.end(), [failSrc, failDest](Edge& e) {
        return (e.src == failSrc && e.dest == failDest) || (e.src == failDest && e.dest == failSrc);
    }), edges.end());

    // Update neighbors
    nodes[failSrc].neighbors.erase(remove(nodes[failSrc].neighbors.begin(), nodes[failSrc].neighbors.end(), failDest),
                                   nodes[failSrc].neighbors.end());
    nodes[failDest].neighbors.erase(remove(nodes[failDest].neighbors.begin(), nodes[failDest].neighbors.end(), failSrc),
                                    nodes[failDest].neighbors.end());

    // Re-initialize distance vectors
    initializeDistanceVectors(nodes, edges, N);

    // Re-run the DVR algorithm until convergence or until any distance exceeds 100
    bool countToInfinity = false;
    do {
        updated = updateDistanceVectors(nodes, N, method);
        countToInfinity = checkCountToInfinity(nodes, N);
        if (countToInfinity) {
            cout << "Count-to-infinity problem detected.\n";
            break;
        }
    } while (updated);

    cout << "\nRouting tables after link failure";
    if (method == 2) {
        cout << " with Poisoned Reverse";
    } else if (method == 3) {
        cout << " with Split Horizon";
    }
    cout << ":\n";
    printRoutingTables(nodes, N);

    return 0;
}
