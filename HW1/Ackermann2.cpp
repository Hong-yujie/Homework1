#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

int ackermann_nonrec(int m, int n) {
    const int MAX_STACK = 100000; // 你可以視需要增加
    int stack[MAX_STACK];
    int top = 0;

    stack[top++] = m;

    while (top > 0) {
        m = stack[--top];

        if (m == 0) {
            n = n + 1;
        } else if (n == 0) {
            stack[top++] = m - 1;
            n = 1;
        } else {
            stack[top++] = m - 1;
            stack[top++] = m;
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

