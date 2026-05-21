#define PROBLEM "https://atcoder.jp/contests/xmascon22/editorial/5438"
#define IGNORE
#include "../../include/mtl/sps.hpp"
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n; cin>>n;
    int en = n+(n%2);
    int N = en/2;
    vector<vector<uint64_t>> A(en, vector<uint64_t>(en));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin>>A[i][j];
        }
    }
    vector<uint64_t> cyc(1<<N),path(1<<N),dp,dp2;
    for (int i = 0; i < N; i++) {
        int a = i*2, b = i*2+1;
        int k = b+1;
        dp.assign(k<<i, 0);
        dp[k*0+a] = 1;
        dp2.assign(k<<i, 0);
        for (int s = 0; s < 1<<i; s++) {
            for (int v = 0; v < k; v++) {
                if (dp[k*s+v]==0) continue;
                cyc[s|(1<<i)] += dp[k*s+v] * A[b][v];
                dp2[k*s+b] += dp[k*s+v];
                for (int x = 0; x < a; x++) {
                    auto t = s|(1<<(x/2));
                    if (s==t) continue;
                    auto y = x^1;
                    // consider v - x - y
                    dp[k*t+y] += dp[k*s+v]*A[v][x];
                }
            }
            for (int v = 0; v < k; v++) {
                if (dp2[k*s+v]==0) continue;
                path[s|(1<<i)] += dp2[k*s+v];
                for (int x = 0; x < a; x++) {
                    auto t = s|(1<<(x/2));
                    if (s==t) continue;
                    auto y = x^1;
                    // v - x - y;
                    dp2[k*t+y] += dp2[k*s+v]*A[v][x];
                }
            }
        }
    }
    auto ecyc = SpsExp<uint64_t,20>(N, cyc);
    // Calc exp(cyc) contribution for path set
    reverse(ecyc.begin(), ecyc.end());
    auto y = TransposedSpsCompositionEgf<uint64_t,20>(path, ecyc);
    for (int k = 0; k <= n/2; k++) {
        auto cnt_path = N-k;
        auto ans = y[cnt_path];
        cout<<ans<<' ';
    }
    cout<<endl;
}

int main() {
    solve();
    return 0;
}