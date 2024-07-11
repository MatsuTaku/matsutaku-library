#define PROBLEM "https://judge.yosupo.jp/problem/set_xor_min"
#include "include/mtl/binary_trie.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int q; cin>>q;
    BinaryTrieSet<unsigned int, 30> bt;
    for (int i = 0; i < q; i++) {
        int t,x; cin>>t>>x;
        if (t == 0) {
            bt.insert(x);
        } else if (t == 1) {
            bt.erase(x);
        } else {
            auto y = *bt.traverse([&](auto d, auto l, auto r) {
                if (l and r) {
                    return x >> d & 1;
                } else if (l) {
                    return 0;
                } else {
                    return 1;
                }
            });
            cout << (x^y) << endl;
        }
    }
}