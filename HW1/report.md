# 41141104

作業一

## Problem 1

### 解題說明：

1. 遞迴實作 Ackermann函數 
2. 非遞迴演算法（比如用堆疊）實作 Ackermann函數 

$$
A(m,n) = \begin{cases}
n + 1 & \text{if } m = 0 \\
A(m-1, 1) & \text{if } n = 0 \\
A(m-1, A(m, n-1)) & \text{otherwise}
\end{cases}
$$



### 解題策略

**遞迴版本：**
1. 直接根據數學定義實現三個條件
2. 如果 m = 0 時：回傳 n + 1
3. 如果 n=0：回傳 A(m−1,1)
4. 其他狀況：回傳 A(m−1,A(m,n−1))

**非遞迴版本：**
1. 利用堆疊模擬遞迴
2. 非遞迴解法主要目的是「手動」模擬程式語言中的呼叫堆疊
3. 利用 while 迴圈，每次取出 stack 頂端 (m, n)，依據 Ackermann 規則展開與推進。

### 程式實作

**遞迴版本：**

```cpp
#include <iostream>
using namespace std;

// Ackermann 遞迴函數定義
int ackermann(int m, int n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ackermann(m - 1, 1);
    else
        return ackermann(m - 1, ackermann(m, n - 1));
}

int main() {
    int m, n;
    cout << "Enter m and n: ";
    cin >> m >> n;
    cout << "Ackermann(" << m << ", " << n << ") = " << ackermann(m, n) << endl;
    return 0;
}



```

**非遞迴版本：**

```cpp
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

```

### 效能分析

**遞迴版本：**
- 時間複雜度：O(A(m,n))，運算次數呈超指數甚至更快的成長
- 空間複雜度：O(A(m,n))，會與最大的遞迴深度成正比

**非遞迴版本：**
- 時間複雜度：O(A(m,n))，和遞迴相同
- 空間複雜度：O(A(m,n))，不受堆疊大小限制

### 測試與驗證

| 測試案例 | 輸入 (m,n) | 預期輸出 | 實際輸出 |
|---------|------------|----------|----------|
| 測試一   | (0,1)      | 2        | 2        |
| 測試二   | (0,4)      | 5        | 5        |
| 測試三   | (1,2)      | 4        | 4        |
| 測試四   | (1,4)      | 6        | 6        |
| 測試五   | (3,4)      | 125      | 125      |
| 測試六   | (3,5)      | 253      | 253      |

### 編譯與執行指令
```shell
1.遞迴版本
g++ -o ackermann ackermann.cpp
./ackermann
2. 非遞迴版本
g++ -o ackermann.exe ackermann.cpp
```
## Problem 2

### 解題說明：　

1. 給定一個有 n 個元素的集合 S，請用遞迴方法產生S 的所有子集。
2. 若 S={a,b,c}：為：{ },{a},{b},{c},{a,b},{a,c},{b,c},{a,b,c}

### 解題策略

1. 給定一個長度為 n 的集合 S，要找出 S 的「所有子集」
2. 對每個元素，子集一定有「包含」或「不包含」此元素兩種選擇，所以總子集數為 $(2^n)$

### 程式實作

```cpp
#include <iostream>
#include <string>

const int MAXN = 100;

void printSubset(std::string subset[], int subsetSize) {
    std::cout << "{";
    for (int i = 0; i < subsetSize; i++) {
        std::cout << subset[i];
        if (i != subsetSize - 1) std::cout << ",";
    }
    std::cout << "}" << std::endl;
}

// 列舉所有大小為 subsetSize 的子集
void combine(std::string S[], int n, int start, std::string subset[], int idx, int subsetSize) {
    if (idx == subsetSize) {
        printSubset(subset, subsetSize);
        return;
    }
    for (int i = start; i < n; ++i) {
        subset[idx] = S[i];
        combine(S, n, i + 1, subset, idx + 1, subsetSize);
    }
}

int main() {
    std::string S[] = {"a", "b", "c"};
    int n = 3;
    std::string subset[MAXN];

    // 空集合
    printSubset(subset, 0);

    // 1 ~ n 元素
    for (int k = 1; k <= n; ++k) {
        combine(S, n, 0, subset, 0, k);
    }
    return 0;
}

```

### 效能分析

- 時間複雜度：$O(2^n)$，因為需要生成所有 2^n 個子集
- 空間複雜度： $O(n)$，遞迴深度最多為 n

### 測試與驗證

**測試案例：S = {a, b, c}**

- 預期輸出：{{}
{a}
{b}
{c}
{a,b}
{a,c}
{b,c}
{a,b,c}}
- 實際輸出：{{}
{a}
{b}
{c}
{a,b}
{a,c}
{b,c}
{a,b,c}}

### 編譯與執行指令

```shell
g++ -o subsets.exe subsets.cpp
subsets.exe

```

## 申論及開發報告

### 選擇遞迴而不是選擇非遞迴的原因

1. 遞迴法直觀易懂、實作快速，非常適用於問題規模不大且解法主軸貼合遞迴結構的情境。因此在像 power set 這類組合生成問題，只要輸入元素不多，選擇遞迴能更加快速。

2. 遞迴的系統堆疊資源只要呼叫層數不是極大，絕大多數小型組合問題都能在可接受時間與空間資源內解決
3. 使用遞迴，程式邏輯能自然、直接對應到問題的數學式