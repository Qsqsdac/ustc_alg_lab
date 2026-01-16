#include <stdio.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <float.h>
#include <cmath>
#include <iomanip>
#include <algorithm>

typedef struct {
    int num;
    float x;
    float y;
} point;

typedef struct {
    point a;
    point b;
} point_pair;

float dis(point a, point b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

point_pair Naive(point p[], int n) {  // 朴素算法
    float min_dis = FLT_MAX;
    point_pair ans;
    for(int i=0; i<n; i++) {
        for(int j=i+1; j<n; j++) {
            if(dis(p[i], p[j]) < min_dis) {
                min_dis = dis(p[i], p[j]);
                ans.a = p[i];
                ans.b = p[j];
            }
        }
    }
    return ans;
}

bool compare_by_x(const point &a, const point &b) {
    return a.x < b.x;
}

bool compare_by_y(const point &a, const point &b) {
    return a.y < b.y;
}

point_pair Divide(point p[], int l, int r) {  // 分治算法
    point_pair ans;
    if(r - l == 1) {
        ans.a = p[0];
        ans.b = p[1];
        return ans;
    }
    std::sort(p + l, p + r, compare_by_x);
    int half = (l+r)/2;
    point_pair leftpp = {p[l], p[half]}, rightpp = {p[half], p[r]};
    float left_dis, right_dis;
    if(half + 1 < r)
        rightpp = Divide(p, half + 1, r);
    if(half - 1 > l)
        leftpp = Divide(p, l, half - 1);
    left_dis = dis(leftpp.a, leftpp.b);
    right_dis = dis(rightpp.a, rightpp.b);
    float dis_min;
    if(left_dis < right_dis) {
        dis_min = left_dis;
        ans = leftpp;
    }
    else {
        dis_min = right_dis;
        ans = rightpp;
    }
    std::vector<point> left_near, right_near;
    for (int i = l; i <= r; i++) {
        if(dis_min > p[half].x - p[i].x && p[i].x <= p[half].x)
            left_near.push_back(p[i]);
        else if(p[i].x > p[half].x && p[i].x - p[half].x < dis_min)
            right_near.push_back(p[i]);
    }
    if(left_near.empty() || right_near.empty())  return ans;
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
}

int main(){
    std::ifstream input("data.txt");
    std::vector<point> data;
    point input_point;
    while (input >> input_point.num) {
        input >> input_point.x;
        input >> input_point.y;
        data.push_back(input_point);
    }
    input.close();

    point* arr1 = new point[data.size()];
    point* arr2 = new point[data.size()];
    std::copy(data.begin(), data.end(), arr1);
    std::copy(data.begin(), data.end(), arr2);

    auto start = std::chrono::high_resolution_clock::now();
    point_pair ans = Naive(arr1, data.size());
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> naive_duration = end - start;

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Result of Naive algorithm:" << std::endl;
    std::cout << ans.a.num << ' ' << ans.a.x << ' ' << ans.a.y << std::endl;
    std::cout << ans.b.num << ' ' << ans.b.x << ' ' << ans.b.y << std::endl;

    start = std::chrono::high_resolution_clock::now();
    ans = Divide(arr1, 0, data.size()-1);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> divide_duration = end - start;

    std::cout << "Result of Divide algorithm:" << std::endl;
    std::cout << ans.a.num << ' ' << ans.a.x << ' ' << ans.a.y << std::endl;
    std::cout << ans.b.num << ' ' << ans.b.x << ' ' << ans.b.y << std::endl;
    std::cout << "Naive duration: " << naive_duration.count() << " ms" << std::endl;
    std::cout << "Divide duration: " << divide_duration.count() << " ms" << std::endl;
}

