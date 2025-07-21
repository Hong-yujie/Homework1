#include <iostream>
#include <stack>
using namespace std;

// 非遞迴 Ackermann 函數，利用 stack 模擬遞迴
int ackermann_nonrec(int m, int n) {
    stack<int> s;
    s.push(m);
    while (!s.empty()) {
        m = s.top();
        s.pop();
        if (m == 0) {
            n = n + 1;
        } else if (n == 0) {
            s.push(m - 1);
            n = 1;
        } else {
            s.push(m - 1); // 先計算含 m-1
            s.push(m);     // 等待完成 ackermann(m, n-1)
            n = n - 1;
        }
    }
    return n;
}

int main() {
    int m, n;
    cout << "Enter m and n: ";
    cin >> m >> n;
    cout << "Ackermann(" << m << ", " << n << ") = " << ackermann_nonrec(m, n) << endl;
    return 0;
}
