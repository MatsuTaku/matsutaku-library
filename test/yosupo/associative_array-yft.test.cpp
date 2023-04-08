#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"
#include <iostream>                        
#include "../../include/mtl/yft.hpp"
#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;
constexpr ull max_value = 1e18;
constexpr int bits = 64-bm::clz(max_value);

int main() {
    int q; cin>>q;
    YFastTrieMap<ull, ull, bits> mp;
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