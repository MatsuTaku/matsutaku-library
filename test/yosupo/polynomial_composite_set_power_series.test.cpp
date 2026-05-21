#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_composite_set_power_series"
#include "include/mtl/sps.hpp"
#include "include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;

int main() {
    int m,n; cin>>m>>n;
    vector<mint> A(m), B(1<<n);
    for (int i = 0; i < m; i++) cin>>A[i];
    for (int i = 0; i < 1<<n; i++) cin>>B[i];
    auto C = SpsCompositionPoly<mint, 20>(n, A, B);
    for (int i = 0; i < 1<<n; i++) {
        cout<<C[i]<<' ';
    }
    cout<<endl;
}