#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>

int min; // 当前最小时间
int n, k;  // n个任务，k个机器
std::vector<int> task_time;  // n个任务各自耗时
std::vector<std::vector<int>> best; // 当前最优解
std::vector<std::vector<int>> now; // 当前解
std::vector<int> machine_time; // 当前每台机器耗时

void BackTrack(int t, int now_time) {
    if (t >= n) {
        min = now_time;
        best = now;      
    } else {
        for (int i = 0; i < std::min(k, t + 1); i++) {
            now[i].push_back(t);
            machine_time[i] += task_time[t]; 
            if (machine_time[i] < min) {      
                int new_time = std::max(now_time, machine_time[i]);
                BackTrack(t + 1, new_time);
            }
            machine_time[i] -= task_time[t];    
            now[i].pop_back();
        }
    }
}

int SelectMachine(int used) {
    int min = 1000, i = -1;
    for(int m = 0; m < k; m++) {
        if(machine_time[m] > used && machine_time[m] < min) {
            min = machine_time[m];
            i = m;
        }
    }
    return i;
}

void BackTrack_o(int t, int now_time) {
    if (t >= n) {  // 到达叶节点，记录新的最优解
        min = now_time;
        best = now;
    } else {
        int i, used = -1;
        while(true) {
            i = SelectMachine(used);
            if(i == -1) return;
            now[i].push_back(t);
            machine_time[i] += task_time[t];       
            if (machine_time[i] < min) {   // 满足限界条件，试探性分配
                int new_time = std::max(now_time, machine_time[i]);
                BackTrack_o(t + 1, new_time);
                machine_time[i] -= task_time[t];
                now[i].pop_back();
                used = machine_time[i];
            }
            else {   // 回溯，还原机器状态
                machine_time[i] -= task_time[t];
                now[i].pop_back();
                return;
            }
        }
    }
}

int main() {
    std::ifstream input("test3.txt");
    input >> n;
    input >> k;
    now.resize(k);
    best.resize(k);
    int d;
    while(input >> d) {
        task_time.push_back(d);
    }
    input.close();

    for(int i = 0; i < k; i++)
        machine_time.push_back(0);
    
    min = 1000;

    auto start = std::chrono::high_resolution_clock::now();
    BackTrack_o(0, 0);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "算法耗时: " << duration.count() << " ms" << std::endl;
    for(int i = 0; i < k; i++) {
        int sum = 0;
        for(int j : best[i])
            sum += task_time[j];
        std::cout << "机器" << i << "(total:" << sum << ") ";
        for(int j : best[i]) {
            std::cout << "任务" << j << " " << "耗时" << task_time[j] << "\t";
        }
        std::cout << std::endl;
    }
    return 0;
}