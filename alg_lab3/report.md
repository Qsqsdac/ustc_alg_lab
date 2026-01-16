## 红黑树插入算法

### 实验内容及目的

实现红黑树的插入算法，并通过先序遍历、中序遍历、层次遍历来检验插入的正确性。

### 算法设计思路

首先对红黑树所需要的数据结构做一些准备工作，然后实现红黑树的左旋、右旋、插入、插入调整函数，最后在主函数中从文件中读出节点插入顺序建立红黑树，并使用三种遍历方法验证正确性。

### 代码解释

```C++
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
```

首先定义所需的数据结构，包括颜色、值、左右孩子指针和父节点指针。同时定义了NIL节点、根节点，以及节点初始化函数。

```C++
void leftRotate(TNode* &root, TNode *x) {  // 左旋 
    TNode *y = x->right;  
    x->right = y->left;  
    if (y->left != TNode::NIL) {  
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == TNode::NIL) { 
        root = y;
```
```C++
    } else if (x == x->parent->left) { 
        x->parent->left = y;
    } else { 
        x->parent->right = y;
    }
    y->left = x;  
    x->parent = y;  
}
```

这是左旋函数的实现，完全按照课件上提供的伪代码编写得到。具体来说就是将x和x的右孩子的指针按照特定顺序进行调整。期间对于x为根节点的情况需要特殊处理。右旋函数与之类似，这里不再展示。

```C++
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
        }
```

这是插入调整函数的实现，按照课件中的伪代码编写得到。这里展示了前三种情况，涵盖了z的父节点是z祖父节点的左孩子。case1表示z的叔叔为红色，需要将z的父节点、叔叔节点、祖父节点变色，而后上溯至z的祖父节点循环处理；case2表示z的叔叔为黑色且z为右孩子，上溯至z的父节点并右旋，转换到case3；case3表示z的叔叔为黑色且z为左孩子，需要将z的父节点和祖父节点变色，再在z祖父节点处右旋。case4至case6与case1至case3对应，仅改变方向。根据题目要求，每次进入一个case，都会打印提示信息。由于case2会转换为case3处理，则case2提示信息后一定紧接着case3提示信息，case5同理。

```C++
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
```

插入函数。函数仅传入参数key，因此首先要创建新节点。按照二叉搜索树的查找过程，找到应该插入的叶子节点位置。插入后进行调整。

红黑树插入涉及到的函数如上。此外，程序中还包含三种遍历函数。先序遍历和中序遍历通过递归实现，层次遍历使用队列通过迭代实现。主函数中首先从输入文件中读取节点数据逐个插入到红黑树中，然后对最终生成的红黑树进行三种遍历，检查结果。程序结束前回收红黑树的存储空间，这一过程通过一个递归函数完成。

### 测试结果

对于助教提供的insert.txt文件中的20个节点数据输入，运行测试结果如下。

控制台输出：

```
case 2
case 3
case 1
case 4
case 5
case 6
case 4
case 2
case 3
case 4
case 1
case 1
case 3
case 4
case 5
case 6
case 2
case 3
case 4
case 6
case 6
```

LNR.txt中序遍历结果：

```
0 BLACK
1 RED
2 BLACK
3 RED
4 BLACK
5 BLACK
6 RED
7 BLACK
8 RED
9 BLACK
10 RED
11 BLACK
12 RED
13 BLACK
14 BLACK
15 RED
16 BLACK
17 RED
18 RED
19 BLACK
```

NLR.txt先序遍历结果：

```
9 BLACK
4 BLACK
1 RED
0 BLACK
2 BLACK
3 RED
6 RED
5 BLACK
7 BLACK
8 RED
14 BLACK
12 RED
11 BLACK
10 RED
13 BLACK
18 RED
16 BLACK
15 RED
17 RED
19 BLACK
```

LOT.txt层次遍历结果：

```
9 BLACK
4 BLACK
14 BLACK
1 RED
6 RED
12 RED
18 RED
0 BLACK
2 BLACK
5 BLACK
7 BLACK
11 BLACK
13 BLACK
16 BLACK
19 BLACK
3 RED
8 RED
10 RED
15 RED
17 RED
```

### 反思与收获

本次实验难度不大，算法上基本按照课件伪代码实现即可。我在实验中遇到的困难主要是C++语法上的，由于存储空间的分配和回收问题，曾出现了Segmentation Fault。另外，红黑树的NIL节点怎样定义也是值得注意的细节。收获上，掌握了红黑树这一数据结构的构建方法，可以为后续其他情景下使用红黑树打下基础。