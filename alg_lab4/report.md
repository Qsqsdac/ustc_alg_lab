## 区间树上重叠区间的查找算法

### 实验内容及目的

对红黑树进行修改，使其成为一棵区间树，并实现区间树上的重叠区间查找算法。

### 算法设计思路

在上次实验完成的红黑树程序基础上做修改。首先是扩充红黑树节点，需要增加数据域max、区间interval。然后对红黑树插入算法做相应修改，由于插入以及插入调整只在调用左旋右旋时更改了节点之间的父子关系，所以实际上只用修改左旋右旋函数。最后完成遍历查找函数。

### 代码解释

```C++
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
```

区间树节点包含了上述内容。interval表示节点的区间，key表示当前区间低点，附加信息max表示当前区间高点、左子树max域、右子树max域三者的最大值。max能够仅通过当前节点及左右孩子的计算得到，满足定理14.1，因此不会改变红黑树操作的渐进时间$O(log n)$。

```C++
void leftRotate(TNode* &root, TNode *x) {  // 左旋 
    TNode *y = x->right;
    y->max = x->max;  // 修改节点y的max域
    x->max = std::max({x->left->max, y->left->max, x->Int.high});   // 修改节点x的max域
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
```

左旋函数相较于红黑树的左旋函数，增加了对x和y两个节点的max域的修改。我选择在左旋开始调整指针之前修改max域。由于左旋前x是整个左旋部分子树的根，左旋后y是子树的根，而节点的max域也可以理解为以该节点为根的子树中所有节点的区间高点的最大值，所以左旋后y的max域等于左旋前x的max域。左旋后x的左右孩子，分别是左旋前x的左孩子和y的左孩子，因此可以根据定义计算左旋后x的max域。右旋同理可得。

插入函数与插入调整函数整体上与红黑树中一致，唯一的区别在于插入函数增加了一个参数，调用自定义的构造函数生成新节点：`TNode *z = new TNode(min, max, {min, max})`。

```C++
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
```

遍历查找函数通过递归实现。课件中提供的伪代码采用迭代的方式，只用找到一个重叠区间即可，但本实验要求找到所有的重叠区间，因此采用递归更为方便。在递归中，向左子树查找的条件是左孩子非空且左孩子max域超过待查询区间低点，这是因为上文中提到的节点的max域是以该节点为根的子树中所有节点的区间高点的最大值；向右子树查找的条件是右孩子非空且当前节点的key小于待查询区间的高点，这是因为根据二叉搜索树的性质，节点的右子树的域的下界是该节点的域。

main函数中首先从助教提供的文件中依次读取区间，插入节点建立区间树。然后从控制台输入待查找区间，查找全部重叠区间并打印。最后回收空间。

### 测试结果

区间[3, 5]的重叠区间：
```
Please input the interval you want to search: 3 5
Interval [2, 17] overlaps with [3, 5]
Interval [4, 15] overlaps with [3, 5]
```
区间[10, 20]的重叠区间：
```
Please input the interval you want to search: 10 20
Interval [13, 18] overlaps with [10, 20]
Interval [6, 15] overlaps with [10, 20]
Interval [2, 17] overlaps with [10, 20]
Interval [4, 15] overlaps with [10, 20]
Interval [9, 18] overlaps with [10, 20]
Interval [8, 13] overlaps with [10, 20]
Interval [7, 14] overlaps with [10, 20]
Interval [11, 22] overlaps with [10, 20]
Interval [10, 17] overlaps with [10, 20]
Interval [18, 23] overlaps with [10, 20]
Interval [15, 16] overlaps with [10, 20]
Interval [14, 22] overlaps with [10, 20]
Interval [17, 18] overlaps with [10, 20]
Interval [16, 25] overlaps with [10, 20]
Interval [19, 24] overlaps with [10, 20]
```
区间[7, 15]的重叠区间：
```
Please input the interval you want to search: 7 15
Interval [13, 18] overlaps with [7, 15]
Interval [6, 15] overlaps with [7, 15]
Interval [2, 17] overlaps with [7, 15]
Interval [4, 15] overlaps with [7, 15]
Interval [9, 18] overlaps with [7, 15]
Interval [8, 13] overlaps with [7, 15]
Interval [7, 14] overlaps with [7, 15]
Interval [11, 22] overlaps with [7, 15]
Interval [10, 17] overlaps with [7, 15]
Interval [15, 16] overlaps with [7, 15]
Interval [14, 22] overlaps with [7, 15]
```

### 反思与收获

本次实验难度不大，主要在于如何修改现有的区间树遍历搜索算法使得能找出全部的重叠区间。通过本次实验，我了解了扩充现有数据结构（红黑树）以解决实际问题（查找重叠区间）的一般方法。