#include <iostream>

template <typename T>
struct Node {
    Node<T> * next;
    T val;

    Node() : next(nullptr), val() {}

    ~Node() {
        delete next;
    }
};

template <typename T>
int size(Node<T> * cur) {
    int sz = 0;
    while (cur != nullptr) {
        ++sz;
        cur = cur->next;
    }
    return sz;
}

template <typename T>
Node<T> * advance(Node<T> * x, int cnt) {
    for (int i = 0; i < cnt; ++i) {
        x = x->next;
    }
    return x;
}

template <typename T>
Node<T> * merge(Node<T> * l, Node<T> * r) {
    Node<T> * cur, * res;
    if (l->val < r->val) {
        cur = l;
        l = l->next;
    } else {
        cur = r;
        r = r->next;
    }
    res = cur;
    while (l != nullptr && r != nullptr) {
        if (l->val < r->val) {
            cur->next = l;
            l = l->next;
        } else {
            cur->next = r;
            r = r->next;
        }
        cur = cur->next;
    }
    if (l != nullptr) {
        cur->next = l;
    } else if (r != nullptr) {
        cur->next = r;
    }
    return res;
}

template <typename T>
void mysort(Node<T> *& x) {
    int sz = size(x);
    if (sz < 2) {
        return;
    }
    Node<T> * l, * r, * cur;
    l = x;
    cur = advance(x, sz / 2 - 1);
    r = cur->next;
    cur->next = nullptr;
    mysort(l);
    mysort(r);
    x = merge(l, r);
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    int n;
    std::cin >> n;
    Node<int> * beg, * cur;
    beg = cur = new Node<int>();
    std::cin >> cur->val;
    --n;
    for (int i = 0; i < n; ++i) {
        cur->next = new Node<int>();
        std::cin >> cur->next->val;
        cur = cur->next;        
    }
    mysort(beg);
    cur = beg;
    while (cur != nullptr) {
        std::cout << cur->val << ' ';
        cur = cur->next;
    }
    std::cout << std::endl;
    delete beg;
    return 0;
} 
