/*
Steg 2: Lös flödesproblemet
(Create a solution to max flow problem using Ford-Fulkersons algorithm)

* Authors: Abhinav Sasikumar & Farhan Syed
* Year: 2023
*/

#include<iostream>
#include<tuple>
#include<vector>
#include<queue>
#include<climits>
#include<sstream>

using namespace std;

// Edge object
struct Edge {

    Edge(int from, int to, int capacity, int residualCap, int flow){
        this->from = from;
        this -> to = to;
        this -> capacity = capacity;
        this -> residualCap = residualCap;
        this -> flow = flow;
    }
    int from;
    int to;
    int capacity;
    int residualCap;
    int flow;
    Edge *oppositeEdge;
};

/*  
    Adds a forward and backward edge between two nodes

*/
void addEdge(vector<Edge *> adjacencyList[], int from, int to, int capacity) {
    Edge *forwardEdge = new Edge(from, to, capacity, capacity, 0); 
    Edge *backwardEdge = new Edge(to, from,0,capacity,0);
    forwardEdge -> oppositeEdge = backwardEdge;
    backwardEdge -> oppositeEdge = forwardEdge;

    adjacencyList[from - 1].push_back(forwardEdge);
    adjacencyList[to - 1].push_back(backwardEdge);
}
/*  
    Calculates the max flow in a graph using the Edward-Karp implementation of the Ford-Fulkersson algorithm,
    which consists of a breadth-first search and the standard Ford-Fulkersson algorithm. 

*/
int ffa(vector<Edge *> adjacencyList[], int s, int t, int listSize) {
    int maxFlow = 0;

    while (true) {
        // BFS PART
        // Used to find augmenting path between s and t
        queue<int> bfsQueue;            // Stores nodes that should be visited

        bfsQueue.push(s);               // Starting node
        Edge *previousEdge = nullptr;
        Edge *parentList[listSize] = {nullptr}; // Keeps track of parents of each node so that we can get whole path

        while (!bfsQueue.empty()) {
            int current = bfsQueue.front();     // Get first mode in queue and pop it
            bfsQueue.pop();
            vector<Edge *> edges = adjacencyList[current-1]; // All edges connected to current node

            for (Edge *thisEdge: edges) { // Explore all edges connected to current node
                thisEdge->residualCap = (thisEdge->capacity) - (thisEdge->flow);   // res = cap - flow
                // Check if to-node has not been visited, that there is res cap available, and that we are not going back to s
                // If so, we want to visit the to-node, and save current node to parent list
                if (parentList[thisEdge->to - 1] == nullptr && thisEdge->residualCap > 0 && thisEdge->to != s) {
                    parentList[thisEdge->to - 1] = thisEdge;  
                    bfsQueue.push(thisEdge->to); 
                }
            }
        }

        // FFA PART
        if (parentList[t-1] != nullptr) { // if there exists path from s to t
            int r = INT_MAX;
            // Go through the augmenting path. Start at t, as long as we are not at s, go to the current node's parent. 
            // Goal is to get smallest r
            for (int currentNode = t; currentNode != s; currentNode = parentList[currentNode-1]-> from) {
                r = min(r, parentList[currentNode-1]->residualCap); // Minimum residual capacity along the path, which is same as maxflow through path
            }
            // Same loop but update values of each edge
            for (int currentNode = t; currentNode != s; currentNode = parentList[currentNode-1]->from) {
                parentList[currentNode-1]->flow += r;  // Forward edge's flow is increased by r
                parentList[currentNode-1]->oppositeEdge->flow -= r; // Backward edge's flow is decreased by r
            }
            maxFlow += r; 
        } else {
            break;
        }
    }
    return maxFlow;
}

int main(int argc, char const *argv[]) {
    std::ios::sync_with_stdio(false);

    int v, s, t, numEdges;
    // Input: nodes, node number of s, node number of t, number of edges
    cin >> v >> s >> t >> numEdges;
    vector<Edge *> adjacencyList[v]; // Adjacency list that stores the edges that each node is adjacent to

    // Read the edges and their capacities, and store in adjacency list
    for (int i = 0; i < numEdges; i++) {
        int from, to, capacity;

        cin >> from >> to >> capacity;
        addEdge(adjacencyList, from, to, capacity);
    }
    // Calculate the max flow 
    int maxFlow = ffa(adjacencyList, s, t, v);

    // Output: nodes, s,t, flow from s to t
    cout << v << "\n";  
    cout << s << " " << t << " " << maxFlow << "\n";

    vector<Edge> edgesInMaxFlowPath;

    // Go through all edges and save ones with positive flow
    for(int i = 0;i<v;i++){
        for(Edge *thisEdge : adjacencyList[i]){
            
            if(thisEdge->flow > 0){
                edgesInMaxFlowPath.push_back(*thisEdge);
            }

        }
    }
    // Output: number of edges with positive flow
    cout << edgesInMaxFlowPath.size() << "\n";

    // Output: every edge and its flow
    for(int i;i<edgesInMaxFlowPath.size();i++){
         cout << edgesInMaxFlowPath[i].from << " " << edgesInMaxFlowPath[i].to << " " << edgesInMaxFlowPath[i].flow << "\n";
    }

    return 0;
}
