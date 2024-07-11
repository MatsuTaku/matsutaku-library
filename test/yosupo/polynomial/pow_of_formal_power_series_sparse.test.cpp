#define PROBLEM "https://judge.yosupo.jp/problem/pow_of_formal_power_series_sparse"
#include "include/mtl/fps/pow.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n,k; long long m; cin>>n>>k>>m;
    vector<int> A(n);
    for (int i = 0; i < k; i++) {
        int j,a; cin>>j>>a;
        A[j] = a;
    }
    Fps<> f(A.begin(), A.end());
    auto g = f.pow(m).inline_pre(n);
    for (int i = 0; i < n; i++)
        cout << g[i] << ' ';
    cout << endl;
}