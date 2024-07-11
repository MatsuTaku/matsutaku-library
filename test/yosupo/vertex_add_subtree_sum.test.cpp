#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_subtree_sum"
#include "include/mtl/hld.hpp"
#include "include/mtl/fenwick_tree.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n,q; cin>>n>>q;
    vector<int> A(n);
    for (int i = 0; i < n; i++) cin>>A[i];
    Hld T(n);
    for (int i = 1; i < n; i++) {
        int p; cin>>p;
        T.add_edge(p, i);
    }
    T.build(0);
    decltype(A) B(n);
    for (int i = 0; i < n; i++) B[T.in[i]] = A[i];
    FenwickTree<long long> ft(B.begin(), B.end());
    auto add = [&](int i, auto v) { ft.add(i, v); };
    auto sum = [&](int l, int r) { return ft.range_sum(l, r); };
    T.subtree_build(add, sum);
    for (int i = 0; i < q; i++) {
        int t; cin>>t;
        if (t == 0) {
            int u,x; cin>>u>>x;
            T.subtree_point_add(u, add, x);
        } else {
            int u; cin>>u;
            cout << T.subtree_sum<long long>(u, sum) << endl;
        }
    }
}