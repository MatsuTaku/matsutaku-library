// https://atcoder.jp/contests/abc306/tasks/abc306_h
#include "../../include/mtl/sps.hpp"
#include "../../include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;

void solve() {
    int n,m; cin>>n>>m;
    vector<uint> mat(n);
    for (int i = 0; i < m; i++) { 
        int a,b; cin>>a>>b; a--; b--;
        mat[a] |= 1<<b;
        mat[b] |= 1<<a;
    }
    vector<int> comp(1<<n);
    comp[0] = 1;
    for (uint s = 1; s < 1<<n; s++) {
        // Calc count of connected-components of s
        auto v = s & (-s);
        auto cv = v;
        auto vis = 0u;
        for (uint t = v; t > 0; ) {
            uint u = t & (-t);
            vis |= u;
            auto adj = mat[std::countr_zero(u)] & s;
            cv |= adj;
            t = (t | adj) & ~vis;
        }
        auto t = s ^ cv;
        comp[s] = -comp[t];
    }
    // F = 1 = \sum_{t \in s} (-1)^{comp[t]} G(s\t)
    // Match to definition of sps-inv
    // G = 1 / \sum_{t \in s} (-1)^{comp[t]} x^t
    auto ans = SpsInv<mint,17>(n, comp);
    cout<<ans.back()<<endl;
}

int main() {
    solve();
    return 0;
}