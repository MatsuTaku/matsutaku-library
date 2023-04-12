#define PROBLEM "https://judge.yosupo.jp/problem/rectangle_sum"
#include <iostream>
#include <bitset>
#include "../../include/mtl/ordinal_range_search.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n,q; cin>>n>>q;
    vector<array<int,3>> N(n);
    int max_idx = 0;
    auto chmax = [&](int& a, int b) { a = max(a, b); };
    for (int i = 0; i < n; i++) {
        int x,y,w; cin>>x>>y>>w;
        N[i] = {x,y,w};
        chmax(max_idx, max(x, y));
    }
    vector<array<int,4>> Q(q);
    for (int i = 0; i < q; i++) {
        int l,d,r,u; cin>>l>>d>>r>>u;
        Q[i] = {l,d,r,u};
        chmax(max_idx, max(r, u));
    }
    ORS<int, long long> ors;
    for (auto [x,y,w] : N) ors.add(x, y, w);
    ors.build(max_idx);
    for (auto [l,d,r,u] : Q) {
        cout << ors.sum(l, r, d, u) << endl;
    }
}