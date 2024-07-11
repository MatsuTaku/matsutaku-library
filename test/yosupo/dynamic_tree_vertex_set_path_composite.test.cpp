#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite"
#include "include/mtl/link_cut_tree.hpp"
#include "include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;
struct Fn {
    mint a,b;
    Fn(mint a=1, mint b=0) : a(a), b(b) {}
    Fn(pair<mint,mint> p) : a(p.first), b(p.second) {}
    Fn operator*(const Fn& r) const {
        return {a*r.a, b*r.a + r.b};
    }
    mint eval(int x) const {
        return a * x + b;
    }
};
using LCT = LinkCutTree<Fn>;

int main() {
    int n,q; cin>>n>>q;
    LCT lct(n);
    for (int i = 0; i < n; i++) {
        int a,b; cin>>a>>b;
        lct.set(i,a,b);
    }
    for (int i = 0; i < n-1; i++) {
        int u,v; cin>>u>>v;
        lct.link(u,v);
    }
    while (q--) {
        int t; cin>>t;
        if (t == 0) {
            int u,v,w,x; cin>>u>>v>>w>>x;
            lct.cut(u,v);
            lct.link(w,x);
        } else if (t == 1) {
            int p,c,d; cin>>p>>c>>d;
            lct.set(p,c,d);
        } else {
            int u,v,x; cin>>u>>v>>x;
            cout << lct.prod(u,v).eval(x) << endl; 
        }
    }
}