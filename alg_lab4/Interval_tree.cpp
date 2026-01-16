#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

enum Color { RED, BLACK };

struct Interval {
    int low, high;
};

struct TNode {
    int key, max;
    Interval Int;         
    Color color;      
    TNode *left, *right, *parent;
    
    TNode(int k, int m, Interval i) : key(k), max(m), Int(i),
            color(RED), left(NIL), right(NIL), parent(NIL) {}
    static TNode *NIL;
};

Interval empty = {0, 0};
TNode* TNode::NIL = new TNode(0, 0, empty);
TNode *root = nullptr;

void leftRotate(TNode* &root, TNode *x) {  // 左旋 
    TNode *y = x->right;
    y->max = x->max;
    x->max = std::max({x->left->max, y->left->max, x->Int.high});
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
    y->max = x->max;
    x->max = std::max({x->right->max, y->right->max, x->Int.high});
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
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else { 
                if (z == z->parent->right) { // case 2
                    z = z->parent;
                    leftRotate(root, z);
                }
                // case 3
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        } else { 
            TNode *y = z->parent->parent->left;
            if (y->color == RED) {  // case 4
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {  // case 5
                    z = z->parent;
                    rightRotate(root, z);
                }
                // case 6
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    root->color = BLACK; 
}

void RBInsert(TNode* &root, int min, int max) {
    TNode *z = new TNode(min, max, {min, max});  // 创建新节点
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
    
    z->left = TNode::NIL;
    z->right = TNode::NIL;
    z->color = RED; 
    RBInsertFixup(root, z);  
}

void cleanup(TNode* node) {
    if (node != TNode::NIL) { 
        cleanup(node->left);
        cleanup(node->right);
        delete node;
    }
}

void IntervalSearch(TNode *x, Interval i) {
    if (x == TNode::NIL)
        return;
    else {
        if (i.high >= x->Int.low && x->Int.high >= i.low) 
            std::cout << "Interval [" << x->Int.low << ", " << x->Int.high << 
                      "] overlaps with [" << i.low << ", " << i.high << "]" << std::endl;
        if (x->left != TNode::NIL && x->left->max >= i.low) 
            IntervalSearch(x->left, i);
        if (x->right != TNode::NIL && x->key <= i.high) 
            IntervalSearch(x->right, i);     
    }
    return;
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
    std::vector<Interval> ints(n);
    for (int i = 0; i < n; i++) { 
        inputFile >> ints[i].low >> ints[i].high;
    }
    inputFile.close(); 
    
    // 插入数据并构建红黑树
    for (Interval i : ints) {
        RBInsert(root, i.low, i.high);
    }
    
    // 待查询区间输入
    Interval target;
    std::cout << "Please input the interval you want to search: ";
    std::cin >> target.low >> target.high;
    IntervalSearch(root, target);
    
    // 清理内存
    cleanup(root);
    delete TNode::NIL;
    
    return 0;
}