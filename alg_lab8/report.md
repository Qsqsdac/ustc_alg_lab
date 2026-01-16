## 图搜索BFS算法及存储优化

### 实验内容及目的

针对图，根据给定的数据集特点选择合适的存储方式（邻接矩阵和邻接表中的一种）进行存储，并进行图的广度优先遍历的过程。

### 算法设计思路

第一个数据集 data.txt 顶点数为9，边数为18，说明节点数较少而边比较密集，适合使用邻接矩阵进行存储；第二个数据集 twitter_small.txt 顶点数为81306，边数为1768149，说明边比较稀疏，适合使用邻接表进行存储。

BFS（广度优先遍历）的算法实现依照课件中的伪代码完成，要点在于通过额外的颜色属性标识节点状态，并且将第一次遍历到的节点置入队列中等待访问邻接点。

### 代码解释

#### 第一个数据集——邻接矩阵
```C++
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
```
定义了一个对象`Graph`，包括节点数量、邻接矩阵、初始化函数、边创建函数、打印函数。邻接矩阵通过一个二维向量来实现。顶点的标识符是整数，因此在`main`函数中读取数据集时会将字符类型的标识符转化为相应的整数。由于第一个数据集是无向图，所以在边创建函数中每次会将矩阵中的两个位置标为1。

```C++
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
            if (graph.adjMatrix[u][v] == 1 && color[v] == white) {   // 找到未被发现的邻居
                color[v] = gray;
                std::cout << (char)(v + 'A') << " ";
                queue.push_back(v);
            }
        }
        color[u] = black;
    }
}
```
函数中使用一个向量来作为队列，并且用一个向量来存储每个顶点的颜色。白色表明顶点未被发现，灰色表示顶点的邻接点未全部被访问，黑色表示顶点访问已完成。从BFS起点开始，每次从队列中取出一个顶点，根据邻接矩阵找到其所有未被发现的邻居，每个邻居变为灰色并加入队列中，然后将该顶点标为黑色。此函数中每次将顶点标为灰色时会将该顶点打印出来，由此可以看到BFS顶点遍历顺序。

主函数中先读取数据集。按照数据文件的格式，首先记录顶点个数，创建`Graph`；然后每次读取一条边，并通过`addEdge`函数向邻接矩阵中添加边。图构建完成后将邻接矩阵打印出来。最后调用BFS函数进行广度优先遍历。

#### 第二个数据集——邻接表
```C++
// 图
std::unordered_map<long, std::unordered_set<long>> adjList;
std::unordered_map<long, Color> vertices;
```
图的邻接表采用了`unordered_map`和`unordered_set`实现。顶点用长整型标识。在主函数中，按照如下方式构建图：
```C++
while (std::getline(inputFile, line)) {
    std::istringstream edgeStream(line);
    long u, v;
    edgeStream >> u >> v;
    adjList[u].insert(v);
    vertices.insert({u, white});
    vertices.insert({v, white});
}
```
每次从数据文件中读取一行，解析为一条有向边，将终点加入到起点的邻接表中，同时将两个顶点加入到顶点集合中并初始化为白色。根据`unordered_map`和`unordered_set`的性质，已有的元素不会重复添加，因此不需要额外考虑重点重边如何处理。

```C++
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
```
BFS函数的实现与邻接矩阵大体类似。主要区别在于，从队列中取出一个顶点访问其邻居的具体操作：循环取该顶点的邻接表元素，对每一个邻居变色并加入队列。另外，由于数据量庞大不好直接打印遍历顺序，因此改为使用一个全局变量记录已被访问的顶点数目，每当顶点由白变灰时计数加一。

主函数中首先读取数据集，并按照上文中提到的方法逐步构建图的邻接表。然后开始计时，进行广度优先遍历。与第一个数据集不同的是，该数据集不是连通图，因此不能只调用一次BFS函数，而是遍历顶点集合，发现白色顶点就作为起点调用BFS函数。最后打印遍历到的顶点总数与算法耗时。

### 测试结果
第一个数据集，打印邻接矩阵和顶点遍历顺序。运行结果如下：
```
Adjacency Matrix:
0 1 1 0 0 0 0 0 0 
1 0 0 1 1 0 1 0 0 
1 0 0 0 0 1 1 1 0 
0 1 0 0 1 0 1 1 1 
0 1 0 1 0 0 0 1 1 
0 0 1 0 0 0 0 1 1 
0 1 1 1 0 0 0 1 0 
0 0 1 1 1 1 1 0 1 
0 0 0 1 1 1 0 1 0 

BFS Traversal: A B C D E G F H I 
```

第二个数据集，打印顶点总数、遍历到的顶点数目、算法耗时。运行结果如下：
```
Number of vertices: 81306
Number of vertices visited: 81306
BFS Duration: 1349.3 ms
```
顶点总数和遍历到的顶点数目相等，说明程序正确。在未开编译优化的情况下算法耗时约为1.3s。

### 反思与收获

本次实验分析了数据集的特征，并针对性地选择了邻接矩阵或者邻接表来实现图的存储。这种按照实际情况进行算法选择与优化的思路给了我很大启发。另外，在邻接矩阵和邻接表的具体实现上，采用了一些C++的高级数据结构，显著简化了代码实现。
