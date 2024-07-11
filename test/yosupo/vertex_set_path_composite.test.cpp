#define PROBLEM "https://judge.yosupo.jp/problem/vertex_set_path_composite"
#include "include/mtl/hld.hpp"
#include "include/mtl/segment_hld.hpp"
#include "include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular<998244353>;

struct Composite {
    mint a, b;
    Composite(mint a=1, mint b=0):a(a),b(b) {}
    Composite(pair<int,int> p):a(p.first),b(p.second) {}
    Composite operator*(const Composite& o) const { 
        return Composite(a*o.a, b*o.a + o.b);
    }
    mint eval(mint x) const {
        return a*x+b;
    }
};

int main() {
    int n,q; cin>>n>>q;
    vector<pair<int,int>> C(n);
    for (int i = 0; i < n; i++) {
        int a,b; cin>>a>>b;
        C[i] = {a,b};
    }
    Hld T(n);
    for (int i = 0; i < n-1; i++) {
        int u,v; cin>>u>>v;
        T.add_edge(u,v);
    } 
    T.build();
    decltype(C) D(n);
    for (int i = 0; i < n; i++) 
        D[T.in[i]] = C[i];
    SegmentHld<Composite> path_sum(T, D.begin(), D.end());
    for (int i = 0; i < q; i++) {
        int t; cin>>t;
        if (t == 0) {
            int p,c,d; cin>>p>>c>>d;
            T.set(p, [&](auto i, auto v) {path_sum.set(i, v);}, Composite(c, d));
        } else {
            int u,v,x; cin>>u>>v>>x;
            auto lq = [&](int l, int r) { 
                return path_sum.query(l, r); 
            };
            auto rq = [&](int l, int r) { 
                return path_sum.reverse_query(l, r); 
            };
            auto ret = T.query(u,v,lq,rq).eval(x);
            cout << ret << endl;
        }
    }
}
