#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

enum Color { RED, BLACK };

struct TNode {
    int key;         
    Color color;      
    TNode *left, *right, *parent;
    
    TNode(int k) : key(k), color(RED), left(NIL), right(NIL), parent(NIL) {}
    static TNode *NIL;
};

TNode* TNode::NIL = new TNode(0);
TNode *root = nullptr;

void leftRotate(TNode* &root, TNode *x) {  // 左旋 
    TNode *y = x->right;  
    x->right = y->left;  
    if (y->left != TNode::NIL) {  
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == TNode::NIL) { 
        root = y;
    } else if (x == x->parent->left) { 
        x->parent->left = y;
    } else { 
        x->parent->right = y;
    }
    y->left = x;  
    x->parent = y;  
}

void rightRotate(TNode* &root, TNode *x) {  // 右旋 
    TNode *y = x->left;
    x->left = y->right;  
    if (y->right != TNode::NIL) {  
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == TNode::NIL) { 
        root = y;
    } else if (x == x->parent->right) { 
        x->parent->right = y;
    } else { 
        x->parent->left = y;
    }
    y->right = x;  
    x->parent = y;  
}

void RBInsertFixup(TNode* &root, TNode *z) {
    while (z != root && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {  
            TNode *y = z->parent->parent->right;  
            if (y->color == RED) {   // case 1
                std::cout << "case 1" << std::endl; 
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else { 
                if (z == z->parent->right) { // case 2
                    std::cout << "case 2" << std::endl;
                    z = z->parent;
                    leftRotate(root, z);
                }
                // case 3
                std::cout << "case 3" << std::endl;
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        } else { 
            TNode *y = z->parent->parent->left;
            if (y->color == RED) {  // case 4
                std::cout << "case 4" << std::endl;
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {  // case 5
                    std::cout << "case 5" << std::endl;
                    z = z->parent;
                    rightRotate(root, z);
                }
                // case 6
                std::cout << "case 6" << std::endl;
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    root->color = BLACK; 
}

void RBInsert(TNode* &root, int key) {
    TNode *z = new TNode(key);  // 创建新节点
    TNode *y = TNode::NIL;
    TNode *x = root;  
    // 找到插入位置
    while (x != TNode::NIL) { // 二叉搜索树的查找过程
        y = x;
        if (z->key < x->key) { 
            x = x->left;
        } else {
            x = x->right;
        }
    }    
    z->parent = y;
    if (y == TNode::NIL) { 
        root = z;  
    } else if (z->key < y->key) { 
        y->left = z;
    } else {
        y->right = z;
    }   
    RBInsertFixup(root, z);  
}

void LNRTraversal(TNode *node, std::ofstream &outFile) {
    if (node != TNode::NIL) {
        LNRTraversal(node->left, outFile);
        outFile << node->key << " " << (node->color == RED ? "RED" : "BLACK") << std::endl;
        LNRTraversal(node->right, outFile);
    }
}

void NLRTraversal(TNode *node, std::ofstream &outFile) {
    if (node != TNode::NIL) {
        outFile << node->key << " " << (node->color == RED ? "RED" : "BLACK") << std::endl; 
        NLRTraversal(node->left, outFile);
        NLRTraversal(node->right, outFile);
    }
}

void LevelOrderTraversal(TNode *root, std::ofstream &outFile) {
    if (root == TNode::NIL) return; // 根节点为空，直接返回
    std::queue<TNode*> q;
    q.push(root);
    while (!q.empty()) { // 队列不为空，继续遍历
        TNode *node = q.front();
        q.pop();
        outFile << node->key << " " << (node->color == RED ? "RED" : "BLACK") << std::endl;
        if (node->left != TNode::NIL) q.push(node->left); // 将左子节点入队
        if (node->right != TNode::NIL) q.push(node->right); // 将右子节点入队
    }
}

void cleanup(TNode* node) {
    if (node != TNode::NIL) { 
        cleanup(node->left);
        cleanup(node->right);
        delete node;
    }
}

int main() {
    TNode::NIL->color = BLACK;
    TNode::NIL->left = TNode::NIL->right = TNode::NIL->parent = TNode::NIL;
    root = TNode::NIL;
    
    // 打开输入文件
    std::ifstream inputFile("insert.txt");
    
    // 读取输入数据
    int n;
    inputFile >> n;
    std::vector<int> keys(n);
    for (int i = 0; i < n; i++) { // 读取数据,存入keys数组
        if (!(inputFile >> keys[i])) {
            std::cerr << "读取第 " << i+1 << " 个数据失败" << std::endl;
            return 1;
        }
    }
    inputFile.close(); // 关闭输入文件
    
    // 插入数据并构建红黑树
    for (int key : keys) {
        RBInsert(root, key);
    }
    
    // 输出遍历结果到文件
    std::ofstream lnrFile("LNR.txt");
    std::ofstream nlrFile("NLR.txt");
    std::ofstream lotFile("LOT.txt");

    // 中序、先序、层次遍历
    LNRTraversal(root, lnrFile);
    NLRTraversal(root, nlrFile);
    LevelOrderTraversal(root, lotFile);
    
    // 关闭输出文件
    lnrFile.close();
    nlrFile.close();
    lotFile.close();
    
    // 清理内存
    cleanup(root);
    delete TNode::NIL;
    
    return 0;
}