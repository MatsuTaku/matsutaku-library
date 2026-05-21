#include "../../include/mtl/sps.hpp"
#include "../../include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;

vector<mint> fact;
void init_factorial(int n) {
    fact.resize(n+1);
    fact[0] = 1;
    for (int i = 1; i <= n; i++) {
        fact[i] = fact[i-1] * i;
    }
}

vector<mint> sps_log(int n, vector<mint> s) {
    assert(s[0] == mint(1));
    // composite log(1-x) with 1-s
    vector<mint> f(n+1);
    for (int i = 1; i <= n; i++) {
        f[i] = -fact[i-1];
    }
    for (auto& x:s) x = -x;
    s[0] = 0;
    return SpsCompositionEgf<mint,17>(n, f, s);
}

void solve() {
    int n,m; cin>>n>>m;
    vector<mint> F(1<<n, 1);
    for (int i = 0; i < m; i++) {
        int a,b; cin>>a>>b; a--; b--;
        int mask = (1<<a)|(1<<b);
        int rm = ((1<<n)-1)^mask;
        for (int j = rm; j >= 0; j = (j-1)&rm) {
            F[j|mask] *= 2;
            if (j==0) break;
        }
    }
    // auto G = sps_log(n, F);
    auto G = SpsLog<mint,17>(n, F);

    vector<mint> ans(n);
    for(int i = 0; i < 1<<(n-1); i++) {
        auto m = (i<<1)|1;
        auto p = G[m] * F[((1<<n)-1)^m];
        for (int j = 1; j < n; j++) if (m&(1<<j)) {
            ans[j] += p;
        }
    }
    for (int i = 1; i < n; i++) cout<<ans[i]<<endl;
}

int main() {
    init_factorial(17);
    solve();
    return 0;
}