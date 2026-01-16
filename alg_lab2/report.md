## Lab2 求平面上n个顶点的最近点对问题

### 算法描述及代码

这个问题用分治法解决。分解的部分很简单，将点集按照横坐标排序，均分为左右两半。由于可以保证每次问题规模都变为1/2，因此这个问题使用分治法具有显著的性能优势。

```C++
if(r - l == 1) {
    ans.a = p[0];
    ans.b = p[1];
    return ans;
}
std::sort(p + l, p + rcompare_by_x);
int half = (l+r)/2;
point_pair leftpp = {p[l],[half]}, rightpp = {p[half], p[r]};
float left_dis, right_dis;
if(half + 1 < r)
    rightpp = Divide(p, half + 1, r);
if(half - 1 > l)
    leftpp = Divide(p, l, half - 1);
left_dis = dis(leftpp.a, leftpp.b);
right_dis = dis(rightpp.a, rightpb);
float dis_min;
if(left_dis < right_dis) {
    dis_min = left_dis;
    ans = leftpp;
}
else {
    dis_min = right_dis;
    ans = rightpp;
}
```

可以看到，这里最小的处理只包含了区间内含有两个点的情况，区间内只有一个点时不会继续递归。这是因为我的递归函数返回值类型为描述点对的结构体类型`point_pair`，区间内只有一个点时无法返回有效的值。对此我也做了初始化的措施。这一步完成后，能得到当前最近点对`ans`和当前最近距离`dis_min`，后者将在下一步中发挥重要作用。

关键在于合并的部分，核心思想是尽量先用x或y的单坐标距离来缩小判断范围，减少计算距离的次数。需要判断左侧点集与右侧点集之间的点对是否会存在比当前最近距离更近的点对。因此，可以首先限制一个范围：以划分点为纵轴中心，左右两侧各延伸出当前最近距离所构成的条状区域。如果存在比当前最近距离更近的点对，只可能在这片区域中。

```C++
std::vector<point> left_near, right_near;
for (int i = l; i <= r; i++) {
    if(dis_min > p[half].x - p[i].x && p[i].x <= p[half].x)
        left_near.push_back(p[i]);
    else if(p[i].x > p[half].x && p[i].x - p[half].x < dis_min)
        right_near.push_back(p[i]);
}
if(left_near.empty() || right_near.empty())  return ans;
```

使用两个向量来存储该区域两侧的点集。如果有一边为空，则可以直接返回当前最小点对。

实际上划分出这个区域后，可以考虑其中点总数较少时直接调用暴力算法，或者交换横纵坐标继续递归（因为上一轮递归显著减少了区域的横坐标距离）。不过为了探索更好的算法，这里不使用递归或者暴力算法。

根据理论分析，对于每个处于左向量的点，满足与其距离小于`dis_min`且两两之间距离不小于`dis_min`的右向量中的点，最多有6个。因此，对每个左向量中的点，至多只需求6次距离。

对每一个左向量中的点，找到这可能存在的6个右向量中的点，这一步可以只计算x或y的单坐标距离来实现。具体来说，先对右向量排序，对每一个左向量中的点，按顺序在右向量中查找横坐标距离小于`dis_min`的点，找到后按顺序对至多6个点求距离并比较。

```C++
std::sort(right_near.begin(), right_near.end(), compare_by_y);
for(auto temp: left_near) {
    int i = 0;
    while(fabs(right_near[i].y - temp.y) > dis_min && i < right_near.size()) i++;          
    for(int j = i; j < i + 6 && j < right_near.size(); j++){
        if(fabs(right_near[i].y - temp.y) > dis_min)
            break;
        if(dis(temp, right_near[i]) < dis_min) {
            ans.a = temp;
            ans.b = right_near[i];
            dis_min = dis(temp, right_near[i]);
        }
    }
}
return ans;
```

由此即可完成全部的分治过程。

程序主函数中，文件操作、数据处理、计时的方法与实验一相同，最终在控制台输出分治算法和暴力算法的耗时。

### 实验结果与分析

![](./)

可以看到，与暴力算法相比，分治算法有非常明显的优越性，性能提高了约25倍。通过分析可以知道，分治算法的时间复杂度为$O(n\log n)$水平。

我实验测试的运行时间还有很大的优化空间，因为我调用的求距离函数`dis(point a, point b)`直接计算了距离，实际上不必进行开根号操作，比较距离的平方就可以。不过我认为求距离是这个问题的元操作，和算法优越性无关，因此尚未修改这部分。
