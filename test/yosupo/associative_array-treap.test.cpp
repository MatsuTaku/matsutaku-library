#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"
#include <iostream>                        
#include "../../include/mtl/treap.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int q; cin>>q;
    using ull = unsigned long long;
    TreapMap<ull, ull> mp;
    for (int i = 0; i < q; i++) {
        int t; cin>>t;
        if (t == 0) {
            ull k,v; cin>>k>>v;
            mp[k] = v;
        } else {
            ull k; cin>>k;
            auto i = mp.find(k);
            cout << (i != mp.end() ? i->second : 0) << endl;
        }
    }
}