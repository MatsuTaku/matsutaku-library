#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum"
#include "include/mtl/link_cut_tree.hpp"
#include <bits/stdc++.h>
using namespace std;

struct Sum {
    long long x;
    Sum(long long x = 0) : x(x) {}
    Sum operator*(const Sum& rhs) const {
        return Sum(x + rhs.x);
    }
    Sum& operator*=(const Sum& rhs) {
        return *this = *this * rhs;
    }
    Sum act(const Sum& rhs) const {
        return Sum(x + rhs.x);
    }
};

int main() {
    int n,q; cin>>n>>q;
    vector<int> A(n);
    for (int i = 0; i < n; i++)
        cin>>A[i];
    LinkCutTree<Sum, Sum> T(A.begin(), A.end());
    for (int i = 0; i < n-1; i++) {
        int u,v; cin>>u>>v; 
        T.link(u,v);
    }
    for (int i = 0; i < q; i++) {
        int t; cin>>t;
        if (t == 0) {
            int u,v,w,x; cin>>u>>v>>w>>x;
            T.cut(u,v);
            T.link(w,x);
        } else if (t == 1) {
            int p,x; cin>>p>>x;
            T.update(p, x);
        } else {
            int u,v; cin>>u>>v;
            cout << T.prod(u,v).x << endl;
        }
    }
}