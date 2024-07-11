#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det"
#include "include/mtl/matrix.hpp"
#include "include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;
using mint = Modular998244353;

int main() {
    int n; cin>>n;
    Matrix<mint,500,500> A;
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) {
        cin>>A[i][j];
    }
    cout << A.det(n) << endl;
}