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
