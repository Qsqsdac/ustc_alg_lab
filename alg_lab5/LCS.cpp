#include <iostream>
#include <string>

using namespace std;

void printLCS(char **b, string x, int i, int j) {
    if(i == 0 || j == 0) 
        return;
    if(b[i][j] == '\\') {
        printLCS(b, x, i-1, j-1);
        cout << x[i-1];
    } else if(b[i][j] == '|') {
        printLCS(b, x, i-1, j);
    } else {
        printLCS(b, x, i, j-1);
    }
}

int LCS_1(string x, string y) {  // S = O(mn)
    int m = x.length();
    int n = y.length();
    char **b = new char*[m+1];
    for(int i = 0; i <= m; ++i) {
        b[i] = new char[n+1];     // '\', '|', '-'
    }    
    int c[m+1][n+1];
    for(int i=0; i<=m; i++) 
        c[i][0] = 0;
    for(int j=0; j<=n; j++) 
        c[0][j] = 0;
    for(int i=1; i<=m; i++) {
        for(int j=1; j<=n; j++) {
            if(x[i-1] == y[j-1]) {
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = '\\';
            } else if(c[i-1][j] >= c[i][j-1]) {
                c[i][j] = c[i-1][j];
                b[i][j] = '|';
            } else {
                c[i][j] = c[i][j-1];
                b[i][j] = '-';
            }
        }
    }
    if(c[m][n] > 0) {
        cout << "LCS: ";
        printLCS(b, x, m, n);
        cout << endl << "长度: ";
    }
    return c[m][n];
}

int LCS_2(string x, string y) {  // S = O(2*min(m,n))  
    int m = x.length();
    int n = y.length();
    int c[2][n+1];  // 这里已经保证了m>=n
    for(int i=0; i<=n; i++) 
        c[0][i] = 0;
    for(int i=1; i<=m; i++) {
        for(int j=0; j<=n; j++) {
            if(j == 0) 
                c[i%2][j] = 0;
            else if(x[i-1] == y[j-1]) 
                c[i%2][j] = c[(i-1)%2][j-1] + 1;
            else if(c[(i-1)%2][j] >= c[i%2][j-1]) 
                c[i%2][j] = c[(i-1)%2][j];
            else 
                c[i%2][j] = c[i%2][j-1];
        }
    }
    return c[m%2][n];
}

int LCS_3(string x, string y) {  // S = O(min(m,n))
    int m = x.length();
    int n = y.length();
    int c[n+1];
    int temp = 0;  // 当前正在计算的值的左上位置值
    for(int i=0; i<=n; i++) 
        c[i] = 0;
    for(int i=1; i<=m; i++) {
        for(int j=0; j<=n; j++) {
            if(j == 0) {
                c[j] = 0;
                temp = 0;
            }
            else if(x[i-1] == y[j-1]) {
                swap(c[j], temp);
                c[j]++;
            }
            else if(c[j] >= c[j-1]) 
                temp = c[j];
            else {
                temp = c[j];
                c[j] = c[j-1];
            }
        }
    }
    return c[n];
}

int main() {
    string x, y;
    cout << "请输入字符串x: ";
    cin >> x;
    cout << "请输入字符串y: ";
    cin >> y;
    if(x.length() < y.length()) 
        swap(x, y);
    int size = LCS_1(x, y);
    if(size == LCS_2(x, y) && LCS_2(x, y) == LCS_3(x, y)) 
        cout << size << endl;
    else 
        cout << "错误" << endl;
    return 0;
}