#define PROBLEM "https://atcoder.jp/contests/abc294/tasks/abc294_h"
#define TLE
#include "../../include/mtl/bitwise_convolution.hpp"
#include "../../include/mtl/sps.hpp"
#include "../../include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;

void solve() {
    int n,m,k; cin>>n>>m>>k;
    vector<mint> edges(1<<n);
    edges[0] = 0;
    for (int i = 0; i < m; i++) {
        int u,v; cin>>u>>v; u--; v--;
        edges[(1<<u)|(1<<v)] = 1;
    }
    SubsetZetaInline(edges);
    vector<mint> ind(1<<n);
    for (int i = 0; i < 1<<n; i++) {
        ind[i] = edges[i]==0;
    }
    vector<mint> y(1<<n);
    y.back() = 1;
    auto x = TransposedSpsCompositionEgf<mint,30>(ind, y);
    mint ans = 0;
    mint p = 1;
    for (int i = 1; i <= n; i++) {
        p *= k-(i-1);
        ans += p * x[i];
    }
    cout<<ans<<endl;
}

int main() {
    solve();
    return 0;
}