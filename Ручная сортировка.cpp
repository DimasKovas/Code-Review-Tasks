#include <bits/stdc++.h>

using namespace std;

struct Node {
    Node * next = nullptr;
    int val = 0;

    ~Node() {
        delete next;
    }
};

void mysort(Node * &x) {
    int cnt = 0;
    Node * cur = x;
    while (cur != nullptr) {
        ++cnt;
        cur = cur->next;
    }
    if (cnt < 2) {
        return;
    }
    Node * l, * r;
    l = x;
    cur = x;
    for (int i = 0; i < cnt / 2 - 1; ++i) {
        cur = cur->next;
    }
    r = cur->next;
    cur->next = nullptr;
    mysort(l);
    mysort(r);
    cur = new Node();
    x = cur;
    Node * curl, * curr;
    curl = l;
    curr = r;
    while (curl != nullptr && curr != nullptr) {
        cur->next = new Node();
        if (curl->val < curr->val) {
            cur->next->val = curl->val;
            curl = curl->next;
        } else {
            cur->next->val = curr->val;
            curr = curr->next;
        }
        cur = cur->next;
    }
    while (curl != nullptr) {
        cur->next = new Node();
        cur->next->val = curl->val;
        curl = curl->next;
        cur = cur->next;
    }
    while (curr != nullptr) {
        cur->next = new Node();
        cur->next->val = curr->val;
        curr = curr->next;
        cur = cur->next;
    }
    delete l;
    delete r;
    cur = x;
    x = x->next;
    cur->next = nullptr;
    delete cur;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n;
    cin >> n;
    Node * beg, * cur;
    beg = cur = new Node();
    cin >> cur->val;
    --n;
    for (int i = 0; i < n; ++i) {
        cur->next = new Node();
        cin >> cur->next->val;
        cur = cur->next;        
    }
    mysort(beg);
    cur = beg;
    while (cur != nullptr) {
        cout << cur->val << ' ';
        cur = cur->next;
    }
    cout << endl;
    return 0;
} 
