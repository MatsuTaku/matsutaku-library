#define PROBLEM "https://atcoder.jp/contests/xmascon20/tasks/xmascon20_h"
#include "../../include/mtl/modular.hpp"
#include "../../include/mtl/subset_convolution.hpp"
#include "../../include/mtl/fps/pow.hpp"

#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;

void solve() {
    int n; cin>>n;
    string l,r; cin>>l>>r;
    vector<mint> L(1<<n);
    for (int i = 0; i < 1<<n; i++) L[i] = l[i]=='1' ? -2 : 0;
    L[0] = 1;
    auto zL = SubsetRankedZeta<mint,21>(n, L.begin(), L.end());
    for (auto& zl:zL) {
        auto f = Fps<mint::mod()>(zl.begin(), zl.end()).sqrt();
        for (int i = 0; i <= n; i++) zl[i] = f[i];
    }
    auto g = SubsetRankedMobius<mint,21>(n, zL);
    for (auto& v:g) v = -v;
    g[0] += 1;
    
    mint ans = 0;
    for (int i = 0; i < 1<<n; i++) {
        if (r[i] == '1') ans += g[i];
    }
    cout<<ans<<endl;
}

int main() {
    solve();
    return 0;
}