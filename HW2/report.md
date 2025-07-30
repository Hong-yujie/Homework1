# 41141104

HomeWork2

### 題目一

## 解題說明

本題要求做出多項式相加、相乘與代入值。

### 解題策略
本題要求實作一個可儲存、運算及輸入/輸出多項式的類別（Polynomial），其中每個多項式由多個 Term 結構組成，並算出多項式的加法、乘法，以及變數帶入求值。

#### 多項式運算
1. operator>>：先讀"項數n"，接下來 n 行讀「係數 次方」，呼叫 newTerm 實作
2. operator<<：依序印出，注意正負號、同次、不輸出0係數
3. add : 兩多項式所有項次合起來，同次方合併
4. muti : 兩多項式所有項彼此乘法，遇到同次自動合併
5. Eval：全部累加
 

## 程式實作

以下為遞迴程式碼：

```cpp
#include <iostream>
#include <cmath>
#include <algorithm>

// Term 結構
// 代表多項式中的一個項目，形式是：coef * x^exp
class Polynomial; // 前置宣告（這裡其實可以不需，因為Term沒有friend Polynomial）

class Term {
public:
    float coef; // 項的係數
    int exp;    // 項的指數
};

// Term 的輸入運算子重載
// 從輸入流讀取一個 Term，如：3.5 2 表示 3.5x^2
std::istream& operator>>(std::istream& is, Term& term) {
    is >> term.coef >> term.exp;
    return is;
}

// Term 的輸出運算子重載
// 將一個 Term 以 “係數x^指數” 輸出，如：3.5x^2
std::ostream& operator<<(std::ostream& os, const Term& term) {
    os << term.coef << "x^" << term.exp;
    return os;
}


// Polynomial 類別
// 用動態陣列管理多項式的多個 Term
class Polynomial {
    // 輸入輸出運算子設定成朋友函式，能讀寫 private 成員
    friend std::ostream& operator<<(std::ostream&, const Polynomial&);
    friend std::istream& operator>>(std::istream&, Polynomial&);
private:
    Term* termArray; // 用動態陣列存放多項式的項
    int capacity;    // 陣列總容量 (可容納的最大項數)
    int terms;       // 實際目前多項式的項數

    // 複製陣列資料（自訂 copy）
    void copy(Term* target, Term* source, int n) {
        for(int i=0; i<n; ++i)
            target[i] = source[i];
    }

    // 新增一個項到多項式
    // 若已有同指數的項，則累加係數
    // 不夠容量會自動擴大陣列空間
    void newTerm(float coef, int exp) {
        // 先找是否有同指數的項
        for(int i=0; i<terms; ++i) {
            if(termArray[i].exp == exp) {
                termArray[i].coef += coef;  // 合併係數
                return;
            }
        }
        // 若無同指數項，新增新項，先擴容(容量加倍)
        if (terms >= capacity) {
            capacity *= 2;
            Term* newArr = new Term[capacity];
            copy(newArr, termArray, terms);
            delete[] termArray;
            termArray = newArr;
        }
        // 新增項目
        termArray[terms].coef = coef;
        termArray[terms++].exp = exp;
    }

    // 對多項式的項按指數由大到小排序
    void sortTerms() {
        std::sort(termArray, termArray + terms, [](const Term& a, const Term& b) {
            return a.exp > b.exp;
        });
    }
public:
    // 建構子（可設定初始容量，預設2）
    Polynomial(int initialCapacity=2) : capacity(initialCapacity), terms(0) {
        if (capacity <= 0) throw "Invalid capacity";
        termArray = new Term[capacity];
    }
    // 解構子，釋放動態記憶體
    ~Polynomial() { delete[] termArray; }
    // 拷貝建構子（深拷貝）
    Polynomial(const Polynomial& rhs) : capacity(rhs.capacity), terms(rhs.terms) {
        termArray = new Term[capacity];
        copy(termArray, rhs.termArray, terms);
    }
    // 指派運算子（同樣深拷貝）
    Polynomial& operator=(const Polynomial& rhs) {
        if (this != &rhs) {
            delete[] termArray;
            capacity = rhs.capacity;
            terms = rhs.terms;
            termArray = new Term[capacity];
            copy(termArray, rhs.termArray, terms);
        }
        return *this;
    }

    // 多項式加法，回傳新 Polynomial
    Polynomial Add(Polynomial poly) {
        Polynomial res(std::max(terms, poly.terms)); // 初始化容量為兩者項數最大值
        // 把本多項式所有項加入結果
        for (int i = 0; i < terms; ++i)
            res.newTerm(termArray[i].coef, termArray[i].exp);
        // 把傳入多項式所有項加入結果（自動合併同指數）
        for (int i = 0; i < poly.terms; ++i)
            res.newTerm(poly.termArray[i].coef, poly.termArray[i].exp);
        res.sortTerms();
        return res;
    }

    // 多項式乘法，回傳新 Polynomial
    Polynomial Mult(Polynomial poly) {
        Polynomial res(terms * poly.terms); // 容量預設乘積
        // 每項相乘並合併同指數
        for (int i = 0; i < terms; ++i)
            for (int j = 0; j < poly.terms; ++j)
                res.newTerm(termArray[i].coef * poly.termArray[j].coef,
                            termArray[i].exp + poly.termArray[j].exp);
        res.sortTerms();
        return res;
    }

    // 多項式在 x 的值，計算多項式結果
    float Eval(float x) {
        float sum = 0;
        for(int i=0; i<terms; ++i)
            sum += termArray[i].coef * std::pow(x, termArray[i].exp);
        return sum;
    }

    // 輸入多項式（先輸入項數 n，再輸入 n 行「係數 指數」）
    friend std::istream& operator>>(std::istream& is, Polynomial& poly) {
        poly.terms = 0;
        int n;
        is >> n;
        float c;
        int e;
        for(int i=0; i<n; ++i) {
            is >> c >> e;
            poly.newTerm(c, e);
        }
        poly.sortTerms();
        return is;
    }

    // 輸出多項式 (格式：係數 x^指數，正負號處理）
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly) {
        bool first = true;
        for(int i=0; i<poly.terms; ++i) {
            if(poly.termArray[i].coef == 0) continue; // 跳過零係數項
            if(!first) {
                if(poly.termArray[i].coef > 0) os << " + ";
                else os << " - ";
            } else if(poly.termArray[i].coef < 0) {
                os << "-";
            }
            float absCoef = std::abs(poly.termArray[i].coef);
            os << absCoef;
            if (poly.termArray[i].exp > 0) os << "x";
            if(poly.termArray[i].exp > 1) os << "^" << poly.termArray[i].exp;
            first = false;
        }
        if(first) os << "0"; // 如果全部係數為 0，輸出 0
        return os;
    }
};


// main 主程式：輸入兩個多項式，計算加法、乘法及評值
int main() {
    Polynomial a(8), b(8);
    std::cout << "請輸入多項式 a: (輸入格式：先給項數，n 行 係數 次方)\n";
    std::cin >> a;
    std::cout << "請輸入多項式 b: (格式同上)\n";
    std::cin >> b;

    std::cout << "多項式 a: " << a << std::endl;
    std::cout << "多項式 b: " << b << std::endl;
    std::cout << "a(x) + b(x): " << a.Add(b) << std::endl;
    std::cout << "a(x) * b(x): " << a.Mult(b) << std::endl;

    float x;
    std::cout << "請輸入 x 的值: "; std::cin >> x;
    std::cout << "a(" << x << ") = " << a.Eval(x) << std::endl;
    std::cout << "b(" << x << ") = " << b.Eval(x) << std::endl;
    std::cout << "[a+b](" << x << ") = " << a.Add(b).Eval(x) << std::endl;
    std::cout << "[a*b](" << x << ") = " << a.Mult(b).Eval(x) << std::endl;
    return 0;
}

```

## 效能分析

|操作|時間複雜度|空間複雜度
|------|------|------|
|newTerm()|	O(t)  |O(1)/O(t)
|Add()|O(m*t)|O(n+t)
|Mult()|O(m*t *k)|O(n*t)
|Eval()     |O(t)  |O(1)

- t: 當前多項式有效項數
- n: 第一個多項式項數
- m: 第二個多項式項數（若與n併列時）
- k: 合併過程中新項插入時最多需比較的長度

## 測試與驗證

### 測試案例
測試一
```
請輸入多項式 a: (輸入格式：先給項數，n 行 係數 次方)
3
1 3
4 2
3 1
請輸入多項式 b: 
2
5 2
6 1
多項式 a: 1x^3 + 4x^2 + 3x
多項式 b: 5x^2 + 6x
a(x) + b(x): 1x^3 + 9x^2 + 9x
a(x) * b(x): 5x^5 + 26x^4 + 39x^3 + 18x^2
請輸入 x 的值: 2
a(2) = 30
b(2) = 32
[a+b](2) = 62
[a*b](2) = 960

```
測試二
```
請輸入多項式 a: (輸入格式：先給項數，n 行 係數 次方)
4
4 4
3 2
2 1
5 0
請輸入多項式 b:
2
5 2
1 1
多項式 a: 4x^4 + 3x^2 + 2x + 5
多項式 b: 5x^2 + 1x
a(x) + b(x): 4x^4 + 8x^2 + 3x + 5
a(x) * b(x): 20x^6 + 4x^5 + 15x^4 + 13x^3 + 27x^2 + 5x
請輸入 x 的值: 3.5
a(3.5) = 649
b(3.5) = 64.75
[a+b](3.5) = 713.75
[a*b](3.5) = 42022.8

```


### 編譯與執行指令

```shell
$ g++ -std=c++17 -O2 -o polynomial polynomial.cpp
$ ./polynomial


```

### 結論
把每一項都包成一個 Term 結構，再用 Polynomial 類別來管理所有項次、運算和記憶體。這樣的設計方便我們做多項式加法、乘法，以及直接帶入 x 值計算結果，只要照這個架構來做，多項式的管理和運算都能正確完成


## 申論及開發報告

本程式旨在實作一個多項式類別，使使用者能輸入任意多項式，並進行多項式的加法、乘法運算及代入變量計算結果。

1. 程式設計兩個主要類別：

- Term：以結構體表示多項式單一項目，包括係數與指數，並重載輸入輸出運算子方便輸入輸出。
- Polynomial：管理多個 Term 的動態陣列，支援動態儲存空間擴充、複製建構子、指派運算子確保資源管理安全。提供多項式加法與乘法方法，利用新建項目函式合併同指數項，並在運算完成後排序。

2. 運算實作：
- 加法：將兩多項式各項加入結果多項式，合併同指數。
-  乘法：兩兩項目相乘，指數相加，係數相乘並合併。

