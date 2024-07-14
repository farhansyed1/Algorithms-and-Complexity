/*
Steg 1: Reducera problemet till flödesproblemet
(Reduce matching problem to flow problem)

* Authors: Abhinav Sasikumar & Farhan Syed
* Year: 2023
*/

#include <iostream>
#include<tuple>
#include<vector>

using namespace std;

int x;
int y;

/*
  Prints out the input for the flow problem, using data from the bipartite graph 
*/
void writeFlowGraph(int x, int y, int numOfEdges, vector<tuple<int, int>> edges) {
  int v = x+y;        // Number of nodes
  int e = numOfEdges; // Number of egdes
  int s = v+1;        // Node number for source s
  int t = v+2;        // Node number for sink/target t
  int c = 1;          // Capacity for each edge

  // Print out Nodes, s, t, number of edges
  // Number of nodes is v+2, as we add 2 for s and t
  // Number of edges is e+v because each node gets extra edge to either s or t 
  cout << v+2 << "\n" << s << " " << t << "\n" << e+v << "\n";  
  
  // Print out each edge from bipartite graph
  for (int i = 0; i < e; ++i) {
    int u = std::get<0>(edges.at(i));
    int v = std::get<1>(edges.at(i));

    // Edge from u to v with capacity c
    cout << u << " " << v << " " << c << "\n";
  }
  // Print out each extra edge from X to s
  for(int i = 1; i <= x; i++){
    cout << s << " " << i << " " << c << "\n";
  }
  // Print out each extra edge from Y to t
  for(int i = x+1; i <= x+y; i++){
    cout << i << " " << t << " " << c << "\n";
  }

  // Flush the output
  cout.flush();

  // Debugutskrift
  // cerr << "Skickade iväg flödesgrafen\n";
}

/*
  Reads the input i.e the bipartite graph 
*/
void readBipartiteGraph() {
  int e;
  // Input: Nodes in X, Nodes in Y, Number of Edges
  cin >> x >> y >> e;

  // Input: Tuples of edges
  vector<tuple<int, int>> edgeVector;
  for (int i = 0; i < e; ++i) {
    int a, b;
    cin >> a >> b;

    edgeVector.push_back(tuple<int,int>(a,b));
  }
  // Send data to writing function
  writeFlowGraph(x, y, e, edgeVector);
}

/*
  Prints out the solution to the max bipartite match problem  
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
  Reads the input i.e the max flow solution  
*/
void readMaxFlowSolution() {
  int v, e, s, t, f;
  vector<tuple<int, int>> matchVector; // Stores edges that are in max match, but connected to s or t
  
  // Input: Nodes, s, t, total flow between s and t, number of edges with positive flow
  cin >> v >> s >> t >> f >> e;
  int edgeCount = 0;

  // Go through each edge with flow and store it if the edge is from the original bipartite graph 
  for (int i = 0; i < e; ++i) {
    int u, v, f;
    // Input: Flow f from u to v
    cin >> u >> v >> f;
    if(u != s && u!=t && v != s && v != t){ 
     matchVector.push_back(tuple<int,int>(u,v));
     edgeCount++;
    }
    
  }
  writeBipMatchSolution(matchVector, edgeCount);
}

/*
  Order of execution: 
  1. readBipartiteGraph
  2. writeFlowGraph
  -- Black box gives the max flow solution
  3. readMaxFlowSolution
  4. writeBipMatchSolution

*/
int main(void) {
  std::ios::sync_with_stdio(false);
  cin.tie(0);

  readBipartiteGraph();
  readMaxFlowSolution();

  return 0;
}
