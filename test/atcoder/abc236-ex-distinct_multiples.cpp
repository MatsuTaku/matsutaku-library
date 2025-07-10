#include <bits/stdc++.h>
#include "../../include/mtl/modular.hpp"
#include "../../include/mtl/subset_convolution.hpp"
using namespace std;
using lint = long long int;
using mint = Modular998244353;
// using mint = double;

vector<mint> inv,fact;

// F=expf => F'=f'F
array<mint,21> fps_exp(array<mint,21>& f, int n) {
    assert(f[0]==mint(0));
    // f'
    for (int i = 0; i < n; i++)
        f[i] = f[i+1]*(i+1);
    array<mint,21> F;
    // [x^0]f=0 => [x^0]expf=1
    F[0] = 1;
    for (int k = 1; k <= n; k++) {
        mint x = 0;
        for (int i = 0; i < k; i++) {
            x += f[i] * F[k-1-i];
        }
        F[k] = x * inv[k];
    }
    return F;
}

lint m_lcm(lint a, lint b, lint m) {
    if (a>m or b>m) return m+1;
    a /= gcd(a,b);
    if (a>(m-1)/b+1) return m+1;
    return min(a*b, m+1);
}

void solve() {
    lint n,m; cin>>n>>m;
    vector<lint> D(n);
    for (int i = 0; i < n; i++) cin>>D[i];
    vector<lint> LCM(1<<n);
    LCM[0] = 1;
    for (int i = 1; i < 1<<n; i++) {
        auto k = bm::ctz(i);
        LCM[i] = m_lcm(LCM[i^(1<<k)], D[k], m);
    }
    vector<mint> gh(1<<n);
    for (int i = 1; i < 1<<n; i++) {
        auto pc = __builtin_popcount(i);
        gh[i] = mint(m/LCM[i]) * fact[pc-1];
        if (pc%2==0) gh[i] = -gh[i];
    }

    //// O(3^n)
    // vector<mint> dp(1<<n);
    // dp[0] = 1;
    // for (int i = 1; i < 1<<n; i++) {
    //     auto f = i&(-i);
    //     auto j = i^f;
    //     for (int k = j; k >= 0; k = (k-1)&j) {
    //         auto l = i^k;
    //         dp[i] += gh[l] * dp[k];
    //         if (k==0) break;
    //     }
    // }
    // auto ans = dp.back();

    // O(2^n n^2)
    auto rdp = SubsetRankedZeta<mint,20>(n, gh.begin(), gh.end());
    for (int i = 0; i < 1<<n; i++) {
        rdp[i] = fps_exp(rdp[i], n);
    }
    vector<mint> dp(1<<n);
    SubsetRankedMobius<mint,20>(n, rdp, dp.begin());
    auto ans = dp.back();
    //// Faster version mobius transform using for point n using Inclusion-Exclusion Principle
    // mint ans = 0;
    // for (int i = 0; i < 1<<n; i++) {
    //     mint sgn = ((n-__builtin_popcount(i))&1==1?-1:1);
    //     ans += rdp[i][n]*sgn;
    // }

    cout<<ans<<endl;
}

int main() {
    fact.resize(20);
    fact[0] = fact[1] = 1;
    for (int i = 2; i < 20; i++)
        fact[i] = fact[i-1]*i;
    inv.resize(20);
    for (int i = 1; i < 20; i++)
        inv[i] = (mint)1/i;
    solve();
}