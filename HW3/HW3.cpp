#include <iostream>
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


