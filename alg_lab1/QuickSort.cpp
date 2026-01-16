#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <chrono>
#include <random>

#define K 8

inline void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

int Partition(int *A, int p, int r) { // 固定基准
    int x = A[r];
    int i = p-1;
    for(int j=p; j<r; j++) {
        if(A[j] <= x) {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i+1], A[r]);
    return i+1;
}

int RandomPartition(int *A, int p, int r, std::mt19937 &gen) { // 随机基准
    std::uniform_int_distribution<> dis(p, r);
    int i = dis(gen);
    swap(A[i], A[r]);
    return Partition(A, p, r);
}

int MedianPartition(int *A, int p, int r) { // 三数取中
    int mid = (p+r)/2;
    if (A[p] < A[mid]) {
        if(A[mid] < A[r])
            swap(A[mid], A[r]);
        else if(A[p] >= A[r])
            swap(A[p], A[r]);
    }
    else {
        if(A[mid] >= A[r])
            swap(A[mid], A[r]);
        else if(A[p] < A[r])
            swap(A[p], A[r]);
    }

    return Partition(A, p, r);
}

void BaseQuickSort(int *A, int p, int r){
    // 基本快排，无优化
    if(p<r) {
        int q = Partition(A, p, r);
        BaseQuickSort(A, p, q-1);
        BaseQuickSort(A, q+1, r);
    }
    return;
}

void RandomQuickSort(int *A, int p, int r, std::mt19937 &gen){
    // 随机基准
    if(p<r) {
        int q = RandomPartition(A, p, r, gen);
        RandomQuickSort(A, p, q-1, gen);
        RandomQuickSort(A, q+1, r, gen);
    }
    return;
}

void InsertSort(int *A, int n) {
    int key, i;
    for (int j=1; j<n; j++) {
        key = A[j];
        i = j-1;
        while(i>=0 && A[i] > key) {
            A[i+1] = A[i];
            i--;
        }
        A[i+1] = key;
    }
}

void InsertQuickSort(int *A, int p, int r) {
    // 插入排序优化
    if(r-p>K) {
        int q = MedianPartition(A, p, r);
        InsertQuickSort(A, p, q-1);
        InsertQuickSort(A, q+1, r);
    }
    else
        InsertSort(A+p, r-p+1);
    return;
}

// 归并排序
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int l, int r) {  // 归并排序
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main(){
    int n, d;
    std::ifstream input("data.txt");
    std::vector<int> data;
    input >> n;
    while (input >> d) {
        data.push_back(d);
    }
    input.close();

    int* arr1 = new int[n];
    int* arr2 = new int[n];
    int* arr3 = new int[n];
    std::copy(data.begin(), data.end(), arr1);
    std::copy(data.begin(), data.end(), arr2);
    std::copy(data.begin(), data.end(), arr3);

    // std::random_device rd;
    // std::mt19937 gen(rd());
    auto start = std::chrono::high_resolution_clock::now();
    // RandomQuickSort(arr1, 0, n-1, gen);
    // BaseQuickSort(arr1, 0, n-1);
    InsertQuickSort(arr1, 0, n-1);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> quicksort_duration = end - start;

    start = std::chrono::high_resolution_clock::now();
    std::sort(arr2, arr2 + n);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> stdsort_duration = end - start;

    start = std::chrono::high_resolution_clock::now();
    mergeSort(arr3, 0, n-1);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> mergesort_duration = end - start;

    bool is_equal = std::equal(arr1, arr1 + n, arr2) && std::equal(arr2, arr2 + n, arr3);   
    std::cout << "QuickSort duration: " << quicksort_duration.count() << " ms" << std::endl;
    std::cout << "std::sort duration: " << stdsort_duration.count() << " ms" << std::endl;
    std::cout << "MergeSort duration: " << mergesort_duration.count() << " ms " << std::endl;
    std::cout << "Sorting results are " << (is_equal ? "equal" : "not equal") << std::endl;

    std::ofstream output("sorted.txt");
    for (int i = 0; i < n; i++) {
        output << arr1[i] << " ";
    }
    output.close();

    delete[] arr1;
    delete[] arr2;
    delete[] arr3;
    return 0;
}