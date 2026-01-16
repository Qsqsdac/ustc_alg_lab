## Huffman编码

### 实验内容及目的

编程实现Huffman编码问题，并理解其核心思想。

### 算法设计思路
Huffman编码是一种基于贪心算法的最优前缀编码方法。算法将每个字符及其频率（权重）看作一个节点。每次选择两个权重最小的节点，合并成一个新节点，新节点的权重为两个子节点权重之和。重复此过程，直到所有节点合并为一棵Huffman树。编码时，根据生成的Huffman树，从根节点出发，向左分支赋值0，向右分支赋值1。每个叶子节点对应一个字符，其路径上的编码即为该字符的Huffman编码。

### 代码解释

首先是一些准备工作，包括定义Huffman树的节点结构以及节点之间进行比较的比较器。
```C++
struct node{
    bool isLeaf;  // 是否为叶子节点
    char ch;      // isLeaf为true时，ch为字符    
    int freq;
    node* left;
    node* right;
} ;

// 自定义比较器，用于按优先级排序
struct Compare {
    bool operator()(node *a, node *b) {
        return a->freq > b->freq; // 优先级小的元素优先
    }
};
```

这里定义比较器是为了后面使用`std::priority_queue`作为最小堆，用于Haffman树的构建过程。

```C++
std::unordered_map<char, int> Count(std::FILE* file) {
    std::unordered_map<char, int> charCount; // 用于存储字符及其出现次数

    int ch;
    while ((ch = std::fgetc(file)) != EOF) { // 逐个读取字符直到文件结束
        if(ch != '\n' && ch != '\r' && ch != ' ' && ch != '\t')
            charCount[static_cast<char>(ch)]++;
    }
    return charCount;
}
```
`Count`函数用于统计文件中每种字符出现的频率，将结果存放在一个`unordered_map`中。按照实验要求，这里统计的是除空格、换行等之外的所有字符。

```C++
std::unordered_map<char, std::string> HuffmanCode(std::unordered_map<char, int> charCount) {
    std::unordered_map<char, std::string> charCode; // 用于存储字符及其对应的哈夫曼编码
    std::priority_queue<node*, std::vector<node*>, Compare> minHeap; // 用于存储字符出现次数的最小堆

    for (const auto& pair : charCount) {
        node *n = new node;
        n->isLeaf = true;
        n->ch = pair.first;
        n->freq = pair.second;
        minHeap.push(n);
    }

    // 构建Huffman树
    while(minHeap.size() > 1){
        node* z = new node;
        node* a = minHeap.top();
        minHeap.pop();
        node* b = minHeap.top();
        minHeap.pop();
        z->left = a;
        z->right = b;
        z->freq = a->freq + b->freq;
        z->isLeaf = false;
        minHeap.push(z);
    }

    node *root = minHeap.top();
    charCode = Coding(root, charCode, "");
    return charCode;
}
```
这是算法的主体部分。函数传入的参数是记录字符出现频率的`charCount`，返回的是得到的每个字符的编码串的`charCode`。算法首先遍历整个`charCount`，构建节点并将节点的指针加入到最小堆`minHeap`中。字符节点在初始化时`isLeaf`域为true，`ch`域存放字符，`freq`域存放频率。`minHeap`采用了自定义的比较函数，因此堆顶是所指向的节点中`freq`域最小的指针。

接下来开始构建Huffman树。这个过程中每次从堆顶取出两个指针，将`freq`域相加，构建新节点，新节点的左右孩子指针指向这两个节点。随后将新节点加入到堆中。重复操作直至堆中只剩下一个节点，该节点即为huffman树的根节点。

最后调用`Coding`函数完成编码。`Coding`函数递归地访问Huffman树，从空字符串开始给树中每一个节点赋予一个01串编码。

```C++
std::unordered_map<char, std::string> Coding(node* T, std::unordered_map<char, std::string> charCode, std::string code){ 
    if(T->isLeaf){
        charCode[T->ch] = code;
        return charCode;
    }
    charCode = Coding(T->left, charCode, code + "0");
    charCode = Coding(T->right, charCode, code + "1");
    return charCode;
}
```

递归遍历过程中01串编码以函数参数的形式传递，到达叶节点时将当前编码记录在`charCode`中。

main函数中打开文件，先调用`Count`函数得到频率表`charCount`，然后调用`HuffmanCode`函数进行编码。此外，main函数中还计算了压缩率，这里的压缩率定义为采用Huffman编码的总长度/采用朴素编码的总长度。

### 测试结果

这里以助教提供的`original.txt`文件作为输入，将编码后的结果记录到`table.txt`文件中，并在控制台打印压缩率。

压缩率：
```
压缩率为64.1293%
```
部分编码结果：
```
字符 频率   编码
t    11515 1111
l    5847  11101
.    1512  1110011
J    93    11100101111
L    45    111001011101
z    45    111001011100
D    42    111001011011
:    40    111001011010
O    80    11100101100
S    339   111001010
C    160   1110010011
q    153   1110010010
······
```

### 反思与收获
本次实验难度不大，厘清算法步骤之后很容易实现，不过具体采用什么数据结构还有优化的空间。

