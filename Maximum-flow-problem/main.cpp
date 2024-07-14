#include<iostream>
#include<tuple>
#include<vector>

int main(int argc, char const *argv[])
{
    int x, y;
    // Row 1 - number of vertices in x and y
    std::cin >> x;
    std::cin >> y;

    // Row 2 - number of edges
    int edges;
    std::cin >> edges;

    std::vector<std::tuple<int, int>> edgeVector;
    
    for(int i = 0;i < edges;i++){
        int startNode, endNode;

        std::cin >> startNode;
        std::cin >> endNode;

       edgeVector.push_back(std::tuple<int,int>(startNode,endNode));
    }
    

    for (int i = 0; i < edgeVector.size(); ++i) {
        int j = std::get<0>(edgeVector.at(i));
        int k = std::get<1>(edgeVector.at(i));
        std::cout << j << " " << k << std::endl;
    }
    std::cout << x << y << edges;

    std::cout << std::endl;
    return 0;
}
