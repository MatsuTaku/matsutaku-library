#define PROBLEM "https://judge.yosupo.jp/problem/power_projection_of_set_power_series"
#include "include/mtl/sps.hpp"
#include "include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;

void solve() {
    int n,m; cin>>n>>m;
    vector<mint> A(1<<n), W(1<<n);
    for (int i = 0; i < 1<<n; i++) cin>>A[i];
    for (int i = 0; i < 1<<n; i++) cin>>W[i];
    auto B = TransposedSpsCompositionPoly<mint,20>(A, W, m);
    for (int i = 0; i < m; i++) cout<<B[i]<<' ';
    cout<<endl;
}

int main() {
    solve();
    return 0;
}