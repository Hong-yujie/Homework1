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
