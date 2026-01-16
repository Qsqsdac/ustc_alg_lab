#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

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

std::unordered_map<char, int> Count(std::FILE* file) {
    std::unordered_map<char, int> charCount; // 用于存储字符及其出现次数

    int ch;
    while ((ch = std::fgetc(file)) != EOF) { // 逐个读取字符直到文件结束
        if(ch != '\n' && ch != '\r' && ch != ' ' && ch != '\t')
            charCount[static_cast<char>(ch)]++;
    }
    return charCount;
}

std::unordered_map<char, std::string> Coding(node* T, std::unordered_map<char, std::string> charCode, std::string code){ 
    if(T->isLeaf){
        charCode[T->ch] = code;
        return charCode;
    }
    charCode = Coding(T->left, charCode, code + "0");
    charCode = Coding(T->right, charCode, code + "1");
    return charCode;
}

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

int main() {
    const char* fileName = "orignal.txt"; 
    std::FILE* input = std::fopen(fileName, "r");
    if (!input) {
        std::cerr << "无法打开文件: " << fileName << std::endl;
        return 1;
    }
    std::ofstream outFile("table.txt");

    std::unordered_map<char, int> count_table = Count(input);
    std::unordered_map<char, std::string> code_table = HuffmanCode(count_table);

    outFile << "字符 频率   编码" << std::endl;
    for (const auto& pair : code_table) {
        char ch = pair.first;
        outFile << ch << "    ";
        outFile << std::left << std::setw(5) << count_table[ch] << " ";
        outFile << pair.second << std::endl;
    }

    // 计算压缩率
    int num = count_table.size();
    int sum = 0;
    for (const auto& pair : count_table) {
        sum += pair.second;
    }
    int bits = static_cast<int>(std::ceil(std::log2(num)));
    int original = bits * sum;
    int huffman = 0;
    for (const auto& pair : code_table) {
        huffman += pair.second.size() * count_table[pair.first];
    }
    std::cout << "压缩率为" << std::fixed << std::setprecision(4) << 100.0 * huffman / original << "%" << std::endl;

    std::fclose(input); // 关闭文件
    return 0;
}
