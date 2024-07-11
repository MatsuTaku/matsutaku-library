#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"
#include "include/mtl/matrix.hpp"
#include "include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;
using mint = Modular998244353;

int main() {
    int n,m,k; cin>>n>>m>>k;
    Matrix<mint,1024,1024> A,B;
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
        cin>>A[i][j];
    }
    for (int i = 0; i < m; i++) for (int j = 0; j < k; j++) {
        cin>>B[i][j];
    }
    auto C = A*B;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            cout << C[i][j] << ' ';
        }
        cout << endl;
    }
}