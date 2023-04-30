#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"
#include "../../../include/mtl/matrix.hpp"
#include "../../../include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;
using mint = Modular998244353;

int main() {
    int n,m; cin>>n>>m;
    Matrix<mint,500,500> A;
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
        cin>>A[i][j];
    }
    Matrix<mint,500,1> b;
    for (int i = 0; i < n; i++) {
        cin>>b[i][0];
    }
    auto [rank,ans_basis,suc] = A.solve(b, n, m);
    if (!suc) {
        cout << -1 << endl;
        return 0;
    }
    cout << rank << endl;
    for (int i = 0; i < m; i++) {
        cout << ans_basis[0][i] << ' ';
    }
    cout << endl;
    for (int i = 0; i < (int)rank; i++) {
        for (int j = 0; j < m; j++) {
            cout << ans_basis[i+1][j] << ' ';
        }
        cout << endl;
    }
}