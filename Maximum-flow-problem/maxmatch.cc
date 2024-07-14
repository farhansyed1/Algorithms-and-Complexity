/*
Steg 3: Kombinera steg 1 & 2
(Use the ford-fulkersson algorithm to solve the maximal bipartite matching problem)

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

int x;
int y;

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
    Adds a forward and backward edge between two nodes. 
    Capacity is always 1 for bipartite graph

*/
void addEdge(vector<Edge *> adjacencyList[], int from, int to) {
    Edge *forwardEdge = new Edge(from, to, 1, 1, 0);
    Edge *backwardEdge = new Edge(to, from,0,1,0);
    forwardEdge->oppositeEdge = backwardEdge;
    backwardEdge->oppositeEdge = forwardEdge;

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

/*
  Prints out the solution to the max bipartite matching problem  
*/
void writeBipMatchSolution(vector<tuple<int, int>> matchVector, int edgeCount) {
  // Output: nodes in X, nodes in Y, size of match (number of edges)
  cout << x << " " << y << "\n" << edgeCount << "\n";

  // Print out each edge in the match
  for (int i = 0; i < edgeCount; ++i) {
    int a = std::get<0>(matchVector.at(i));
    int b = std::get<1>(matchVector.at(i));
    cout << a << " " << b << "\n";
  }
}

/*
  Reads the solution from the ffa i.e the max flow solution  
*/
void readMaxFlowSolution(int maxFlow, vector<Edge *> adjacencyList[], int s, int t, int v) {
  vector<tuple<int, int>> matchVector;
  int edgeCount = 0;

  // Go through each edge with flow and store it if the edge is from the original bipartite graph 
  for(int i = 0;i<v;i++){
        for(Edge *thisEdge : adjacencyList[i]){
            if(thisEdge->flow > 0 && thisEdge->to != s && thisEdge->to!=t && thisEdge->from != s && thisEdge->to != t){
                matchVector.push_back(tuple<int,int>(thisEdge->from,thisEdge->to));
                edgeCount++;
            }
        }
    }
    writeBipMatchSolution(matchVector, edgeCount);
}

/*
  Reads the input i.e the bipartite graph 
*/
void readBipartiteGraph() {
  int e;
  // Input: Nodes in X, Nodes in Y, Number of Edges
  cin >> x >> y >> e;
  int v = x+y+2; // Number of nodes is x+y+2, as we add 2 for s and t
  int s = x+y+1; // Node number for source s
  int t = x+y+2; // Node number for sink/target t
  int c = 1;     // Capacity for each edge

  vector<Edge *> adjacencyList[v];

  // Input: Tuples of edges
  for (int i = 0; i < e; ++i) {
    int from, to;
    cin >> from >> to;
    addEdge(adjacencyList, from, to);
  }
  // Adding edges from s to X
  for(int i = 1; i <= x; i++){
    addEdge(adjacencyList, s, i);
  }
  // Adding edges from Y to t
  for(int i = x+1; i <= x+y; i++){
    addEdge(adjacencyList, i, t);
  }

  // Calucate the max flow 
  int maxFlow = ffa(adjacencyList, s, t, v);
  readMaxFlowSolution(maxFlow, adjacencyList, s,t,v);

}

/*
  Order of execution: 
  1. readBipartiteGraph
  -- ffa gives the max flow solution
  2. readMaxFlowSolution
  3. writeBipMatchSolution

*/
int main(int argc, char const *argv[]) {
    std::ios::sync_with_stdio(false);

    readBipartiteGraph();

    return 0;
}
