#define PROBLEM "https://judge.yosupo.jp/problem/pow_of_formal_power_series"
#include "include/mtl/fps/pow.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n; long long m; cin>>n>>m;
    vector<int> A(n);
    for (int i = 0; i < n; i++) cin>>A[i];
    Fps<> f(A.begin(), A.end());
    auto g = f.pow(m).inline_pre(n);
    for (int i = 0; i < n; i++)
        cout << g[i] << ' ';
    cout << endl;
}