#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

enum Color {white, gray, black};

struct Graph {
    int numVertices;
    std::vector<std::vector<int>> adjMatrix;
    Graph(int vertices) : numVertices(vertices) {
        adjMatrix.resize(vertices, std::vector<int>(vertices, 0));
    }

    void addEdge(int u, int v) {
        if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
            adjMatrix[u][v] = 1;
            adjMatrix[v][u] = 1; 
        }
    }

    void printMatrix() {
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                std::cout << adjMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

void BFS(Graph &graph, int s) {
    std::vector<int> queue;
    queue.push_back(s);
    std::vector<Color> color(graph.numVertices, white);
    color[s] = gray;
    std::cout << (char)(s + 'A') << " ";
    while(!queue.empty()) {
        int u = queue.front();
        queue.erase(queue.begin());
        for (int v = 0; v < graph.numVertices; ++v) {
            if (graph.adjMatrix[u][v] == 1 && color[v] == white) {
                color[v] = gray;
                std::cout << (char)(v + 'A') << " ";
                queue.push_back(v);
            }
        }
        color[u] = black;
    }
}

int main() {
    std::ifstream inputFile("data.txt");
    std::string line;
    std::getline(inputFile, line);
    std::istringstream vertexStream(line);

    int vertices = 1, edges;
    char c;
    vertexStream >> c;
    while(vertexStream >> c && c == ',') {
        vertices++;
        vertexStream >> c;
    }

    Graph graph(vertices);

    while (std::getline(inputFile, line)) {
        std::istringstream edgeStream(line);
        char u, dash, v;
        edgeStream >> u >> dash >> v;
        int a, b;
        a = u - 'A';
        b = v - 'A';
        graph.addEdge(a, b);
    }

    std::cout << "Adjacency Matrix:" << std::endl;
    graph.printMatrix();
    std::cout << std::endl;

    std::cout << "BFS Traversal: ";
    BFS(graph, 0);
    std::cout << std::endl;
    return 0;
}