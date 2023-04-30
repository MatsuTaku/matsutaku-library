#define PROBLEM "https://judge.yosupo.jp/problem/inverse_matrix"
#include "../../../include/mtl/matrix.hpp"
#include "../../../include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;
using mint = Modular998244353;

int main() {
    int n; cin>>n;
    Matrix<mint,500,500> A;
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) {
        cin>>A[i][j];
    }
    auto [b,res] = A.inv(n);
    if (res) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << b[i][j] << ' ';
            cout << endl;
        }
    } else {
        cout << -1 << endl;
    }
}