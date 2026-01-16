#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <chrono>

enum Color {white, gray, black};

// 图
std::unordered_map<long, std::unordered_set<long>> adjList;
std::unordered_map<long, Color> vertices;

long count = 0;  // 遍历节点计数

void BFS(long s) {
    std::vector<long> queue;
    queue.push_back(s);
    vertices[s] = gray;
    count++;
    while(!queue.empty()) {
        long u = queue.front();
        queue.erase(queue.begin());
        for (auto v = adjList[u].begin(); v != adjList[u].end(); ++v) {
            if (vertices[*v] == white) {
                vertices[*v] = gray;
                count++;
                queue.push_back(*v);
            }
        }
        vertices[u] = black;
    }
}

int main() {
    std::ifstream inputFile("twitter_small.txt");
    std::string line;
    std::getline(inputFile, line);

    while (std::getline(inputFile, line)) {
        std::istringstream edgeStream(line);
        long u, v;
        edgeStream >> u >> v;
        adjList[u].insert(v);
        vertices.insert({u, white});
        vertices.insert({v, white});
    }
    std::cout << "Number of vertices: " << vertices.size() << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();

    for(auto v : vertices) {
        if (v.second == white) {
            BFS(v.first);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Number of vertices visited: " << count << std::endl;
    std::cout << "BFS Duration: " << duration.count() << " ms" << std::endl;

    return 0;
}