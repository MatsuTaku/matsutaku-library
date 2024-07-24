#define PROBLEM "https://judge.yosupo.jp/problem/rectangle_sum"
// #define IGNORE
#include "include/mtl/ordinal_range_search.hpp"
#include "include/mtl/fenwick_tree.hpp"
#include <iostream>
#include <bitset>
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
    for (auto [x,y,w] : N) ors.add({x, y, w});
    FenwickTree<long long> rsq;
    ors.build(
    [&](size_t size) {
        rsq = FenwickTree<long long>(size);
    },
    [&](size_t i, int w) {
        rsq.add(i, w);
    });
    for (auto [l,d,r,u] : Q) {
        cout << ors.sum(l, r, d, u, [&](size_t l, size_t r) { return rsq.sum(l, r); }) << endl;
    }
}