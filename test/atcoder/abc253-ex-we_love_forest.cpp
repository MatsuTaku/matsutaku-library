#define PROBLEM "https://atcoder.jp/contests/abc253/tasks/abc253_h"
#define IGNORE
#include "../../include/mtl/sps.hpp"
#include "../../include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;
constexpr int LIM = 14;

void solve() {
    int n,m; cin>>n>>m;
    vector<vector<int>> mat(n, vector<int>(n));
    for (int i = 0; i < m; i++) {
        int u,v; cin>>u>>v;
        u--; v--;
        mat[u][v]++;
        mat[v][u]++;
    }

    // dp_i: number of trees with vertex set i
    vector<mint> dp(1<<n);
    // f.reserve(1<<n);
    dp[0] = 0;
    for (int i = 0; i < n; i++) {
        for (int s = 0; s < 1<<i; s++) {
            int k = 0;
            for (int j = 0; j < i; j++) if ((s>>j)&1) k += mat[i][j];
            dp[s|(1<<i)] = dp[s] * k;
        }
        SpsExp<mint,LIM>(i, dp.cbegin()+(1<<i), dp.cbegin()+(2<<i), dp.begin()+(1<<i));
    }

    vector<mint> x(1<<n);
    x.back() = 1;
    // y_i: number of forests with i connected components
    // thats why, define f(s): number of forest with vertex set s, f = exp(dp)
    auto y = TransposedSpsCompositionEgf<mint,LIM>(dp, x);

    auto im = mint(m).inv();
    mint coeff = 1;
    for (int k = 1; k < n; k++) {
        auto ans = y[n-k];
        coeff *= k * im;
        ans *= coeff;
        cout<<ans<<endl;
    }
}

int main() {
    solve();
    return 0;
}