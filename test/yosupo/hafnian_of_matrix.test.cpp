#define PROBLEM "https://judge.yosupo.jp/problem/hafnian_of_matrix"
#include "../../include/mtl/sps.hpp"
#include "../../include/mtl/modular.hpp"
#include "../../include/mtl/bit_manip.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;

void solve() {
    int n; cin>>n;
    vector<vector<int>> A(n, vector<int>(n));
    vector<long long int> B(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin>>A[i][j];
            if (A[i][j]>0)
                B[i] |= 1ll<<j;
        }
    }
    int N = n/2;
    vector<mint> cyc(1<<N),dp;
    for (int i = 0; i < N; i++) {
        int a = i*2, b = i*2+1;
        int k = b+1;
        dp.assign(k<<i, 0);
        dp[k*0+a] = 1;
        for (int s = 0; s < 1<<i; s++) {
            for (int v = 0; v < k; v++) {
                if (dp[k*s+v]==0) continue;
                cyc[s|(1<<i)] += dp[k*s+v] * A[b][v];
                for (int x = 0; x < a; x++) {
                    auto t = s|(1<<(x/2));
                    if (s==t) continue;
                    auto y = x^1;
                    // consider v - x - y
                    dp[k*t+y] += dp[k*s+v]*A[v][x];
                }
            }
        }
    }
    auto ans = SpsExp<mint,19>(N, cyc).back();
    cout<<ans<<endl;
}

int main() {
    solve();
    return 0;
}