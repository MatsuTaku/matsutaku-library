#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_path_sum"
#include "../../include/mtl/hld.hpp"
#include "../../include/mtl/fenwick_tree.hpp"
#include <bits/stdc++.h>
using namespace std;

struct Sum {
    long long x;
    Sum(long long x=0):x(x) {}
    Sum operator*(const Sum& o) const { return Sum(x+o.x); }
    Sum operator~() const { return *this; }
};

int main() {
    int n,q; cin>>n>>q;
    vector<int> A(n);
    for (int i = 0; i < n; i++) cin>>A[i];
    Hld T(n);
    for (int i = 0; i < n-1; i++) {
        int u,v; cin>>u>>v;
        T.add_edge(u,v);
    } 
    T.build();
    vector<int> B(n);
    for (int i = 0; i < n; i++) B[T.in[i]] = A[i];
    FenwickTree<long long> path_sum(B.begin(), B.end());
    for (int i = 0; i < q; i++) {
        int t; cin>>t;
        if (t == 0) {
            int p,x; cin>>p>>x;
            auto tp = T.in[p];
            path_sum.add(tp, x);
        } else {
            int u,v; cin>>u>>v;
            auto ret = T.query<Sum>(u,v,[&](int l, int r) { 
                return path_sum.range_sum(l, r); 
            }).x;
            cout << ret << endl;
        }
    }
}
