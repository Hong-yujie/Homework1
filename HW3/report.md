# 41141104

HomeWork3

### 題目一

## 解題說明
要製作一個「可以進行多項式（Polynomial）加減乘與帶入數值」，
多項式用循環鏈結串列結構儲存，每個節點存「係數、指數、下一節點指標」，需要：建構/複製/賦值/解構、加減乘三種運算子重載、Evaluate。

### 解題策略
- (a) operator >>  允許用 cin 依序輸入多項式（輸入如下：條數、每項係數指數），並呼叫 InsertTerm 插入，會自動排序合併。
- (b) operator <<  允許直接 cout 輸多項式，格式如 3x^2+2 等，無項就輸 0。
- (c) Polynomial(const Polynomial &) [深複製建構子]，複製復刻傳入的 polynomial，每一節點都新生，保證彼此獨立。
- (d) operator = (const Polynomial &)[深複製賦值]，先清空舊內容，再複製。
- (e) ~Polynomial/clear() 清空所有項目釋放記憶體，避免 memory leak。
- (f) operator+(const Polynomial &b) [加法]，雙指標分別巡訪串列，依指數降序比較，合併同指數，不同指數直接插入。
- (g) operator- [減法]，同加法，差別是第二個多項式係數全取負。
- (h) operator* [乘法]，每一項與另一邊每一項逐一相乘，指數加、係數乘，最後插入（自動歸併同指數）。
- (i) Evaluate(x) 計算多項式在 x 位置的值，將每一項代入 x 再累加。

 

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
            res.newTerm(poly.termArray[i#include <iostream>
#include <cmath>
using namespace std;

struct PolyNode {
    int coef;           // 係數
    int exp;            // 指數
    PolyNode *link;     // 下一個節點
    PolyNode(int c = 0, int e = 0, PolyNode* l = nullptr) : coef(c), exp(e), link(l) {}
};

class Polynomial {
private:
    PolyNode* head;     // 頭節點（環狀鏈結串列）

public:
    Polynomial() {
        head = new PolyNode(0, -1, nullptr);
        head->link = head;  // 初始指向自己
    }
    Polynomial(const Polynomial& rhs) {
        head = new PolyNode(0, -1, nullptr);
        head->link = head;
        PolyNode *q = head;
        PolyNode *p = rhs.head->link;
        while (p != rhs.head) {
            q->link = new PolyNode(p->coef, p->exp, head);
            q = q->link;
            p = p->link;
        }
    }
    Polynomial& operator=(const Polynomial& rhs) {
        if (this != &rhs) {
            clear();
            PolyNode *q = head;
            PolyNode *p = rhs.head->link;
            while (p != rhs.head) {
                q->link = new PolyNode(p->coef, p->exp, head);
                q = q->link;
                p = p->link;
            }
        }
        return *this;
    }
    ~Polynomial() { clear(); delete head; }
    void clear() {
        PolyNode* p = head->link;
        while (p != head) {
            PolyNode* toDel = p;
            p = p->link;
            delete toDel;
        }
        head->link = head;
    }
    void InsertTerm(int coef, int exp) {
        if (coef == 0) return;
        PolyNode *p = head;
        while (p->link != head && p->link->exp > exp) p = p->link;
        if (p->link != head && p->link->exp == exp) {
            p->link->coef += coef;
            if (p->link->coef == 0) {
                PolyNode* toDel = p->link;
                p->link = toDel->link;
                delete toDel;
            }
        } else {
            PolyNode* n = new PolyNode(coef, exp, p->link);
            p->link = n;
        }
    }

    friend istream& operator>>(istream& is, Polynomial& poly) {
        int n;
        cout << "請輸入多項式項數: ";
        is >> n;
        for (int i = 0; i < n; ++i) {
            int c, e;
            cout << "第" << (i+1) << "項 係數 指數: ";
            is >> c >> e;
            poly.InsertTerm(c, e);
        }
        return is;
    }
    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        PolyNode* p = poly.head->link;
        bool first = true;
        if (p == poly.head) os << "0";
        while (p != poly.head) {
            if (!first && p->coef > 0) os << "+";
            if (p->exp == 0)
                os << p->coef;
            else if (p->exp == 1)
                os << p->coef << "x";
            else
                os << p->coef << "x^" << p->exp;
            first = false;
            p = p->link;
        }
        return os;
    }
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        PolyNode* aPtr = head->link;
        PolyNode* bPtr = b.head->link;
        while (aPtr != head && bPtr != b.head) {
            if (aPtr->exp > bPtr->exp) {
                result.InsertTerm(aPtr->coef, aPtr->exp);
                aPtr = aPtr->link;
            } else if (aPtr->exp < bPtr->exp) {
                result.InsertTerm(bPtr->coef, bPtr->exp);
                bPtr = bPtr->link;
            } else {
                result.InsertTerm(aPtr->coef + bPtr->coef, aPtr->exp);
                aPtr = aPtr->link;
                bPtr = bPtr->link;
            }
        }
        while (aPtr != head) {
            result.InsertTerm(aPtr->coef, aPtr->exp);
            aPtr = aPtr->link;
        }
        while (bPtr != b.head) {
            result.InsertTerm(bPtr->coef, bPtr->exp);
            bPtr = bPtr->link;
        }
        return result;
    }
    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        PolyNode* aPtr = head->link;
        PolyNode* bPtr = b.head->link;
        while (aPtr != head && bPtr != b.head) {
            if (aPtr->exp > bPtr->exp) {
                result.InsertTerm(aPtr->coef, aPtr->exp);
                aPtr = aPtr->link;
            } else if (aPtr->exp < bPtr->exp) {
                result.InsertTerm(-bPtr->coef, bPtr->exp);
                bPtr = bPtr->link;
            } else {
                result.InsertTerm(aPtr->coef - bPtr->coef, aPtr->exp);
                aPtr = aPtr->link;
                bPtr = bPtr->link;
            }
        }
        while (aPtr != head) {
            result.InsertTerm(aPtr->coef, aPtr->exp);
            aPtr = aPtr->link;
        }
        while (bPtr != b.head) {
            result.InsertTerm(-bPtr->coef, bPtr->exp);
            bPtr = bPtr->link;
        }
        return result;
    }
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        for (PolyNode* aPtr = head->link; aPtr != head; aPtr = aPtr->link) {
            for (PolyNode* bPtr = b.head->link; bPtr != b.head; bPtr = bPtr->link) {
                int c = aPtr->coef * bPtr->coef;
                int e = aPtr->exp + bPtr->exp;
                result.InsertTerm(c, e);
            }
        }
        return result;
    }
    float Evaluate(float x) const {
        float ans = 0.0f;
        PolyNode* p = head->link;
        while (p != head) {
            ans += p->coef * pow(x, p->exp);
            p = p->link;
        }
        return ans;
    }
};

int main() {
    Polynomial p1, p2;
    cout << "輸入第一個多項式" << endl;
    cin >> p1;
    cout << "輸入第二個多項式" << endl;
    cin >> p2;
    cout << "p1: " << p1 << endl;
    cout << "p2: " << p2 << endl;

    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial prod = p1 * p2;

    cout << "加法結果: " << sum << endl;
    cout << "減法結果: " << diff << endl;
    cout << "乘法結果: " << prod << endl;

    float x;
    cout << "請輸入 x 數值: ";
    cin >> x;

    cout << "p1(" << x << ") = " << p1.Evaluate(x) << endl;
    cout << "p2(" << x << ") = " << p2.Evaluate(x) << endl;
    cout << "加法結果(" << x << ") = " << sum.Evaluate(x) << endl;
    cout << "減法結果(" << x << ") = " << diff.Evaluate(x) << endl;
    cout << "乘法結果(" << x << ") = " << prod.Evaluate(x) << endl;

    return 0;
}


```

## 效能分析

### 時間複雜度

- 插入一項（InsertTerm）: O(n) ，n 為多項式現有項目數。
- 加法、減法（operator+, operator-）：O(n + m)，n、m 分別為兩個多項式的項數
- 乘法（operator*）：O(n × m × k)，n: p1 的項數，m: p2 的項數，k: 新多項式
- Evaluate（帶入 x 計算值）：O(n)，n 為該多項式項數，
pow(x, exp) 本身是 O(log exp) 可視 exp 大小略增

### 空間複雜度

- 空間複雜度為：O(n)，其中 n 為該多項式的項數
## 測試與驗證

### 測試案例
測試一
```
輸入第一個多項式
請輸入多項式項數: 2
第1項 係數 指數: 2 2
第2項 係數 指數: 5 0
輸入第二個多項式
請輸入多項式項數: 3
第1項 係數 指數: 4 3
第2項 係數 指數: 5 2
第3項 係數 指數: 8 1
p1: 2x^2+5
p2: 4x^3+5x^2+8x
加法結果: 4x^3+7x^2+8x+5
減法結果: -4x^3-3x^2-8x+5
乘法結果: 8x^5+10x^4+36x^3+25x^2+40x
請輸入 x 數值: 3
p1(3) = 23
p2(3) = 177
加法結果(3) = 200
減法結果(3) = -154
乘法結果(3) = 4071
```
測試二
```
輸入第一個多項式
請輸入多項式項數: 4
第1項 係數 指數: -2 4
第2項 係數 指數: 3 3
第3項 係數 指數: 2 1
第4項 係數 指數: -4 0
輸入第二個多項式
請輸入多項式項數: 2
第1項 係數 指數: -2 2
第2項 係數 指數: 6 1
p1: -2x^4+3x^3+2x-4
p2: -2x^2+6x
加法結果: -2x^4+3x^3-2x^2+8x-4
減法結果: -2x^4+3x^3+2x^2-4x-4
乘法結果: 4x^6-18x^5+18x^4-4x^3+20x^2-24x
請輸入 x 數值: -2
p1(-2) = -64
p2(-2) = -20
加法結果(-2) = -84
減法結果(-2) = -44
乘法結果(-2) = 1280
```


### 編譯與執行指令

```shell
g++ -o poly poly.cpp
./poly
```

### 結論
運用「帶頭節點的循環鏈結串列」，設計出功能完整的多項式類別 Polynomial。該結構有效管理動態多項式項目，並實作加、減、乘、代入等主要運算，滿足了多項式計算的需求。加減法運算透過同步掃描達成 O(n+m) 的高效率；雖然乘法運算因合併項目需多重遍歷，時間複雜度較高，但對於一般應用與課業規模而言仍能快速處理。Evaluate 函式則可快速帶入 x，計算多項式的數值結果。


## 申論及開發報告

### 為什麼用「帶頭循環鏈結串列」？
插入/刪除方便，可以不用考慮特殊的尾端處理或空表處理，提高健壯性。
自動回到 head，代表結尾。這結構利於動態長度（多項式項數可變），也符合物件導向多型。
### 效能與問題討論
本系統加法與減法均為雙串列同步掃描合併，時間複雜度 O(n+m)；Evaluate 跑一次 O(n) 即可。乘法最慢，為 O(nmk)，實際應用時若多項式項數太多，效能會下降。