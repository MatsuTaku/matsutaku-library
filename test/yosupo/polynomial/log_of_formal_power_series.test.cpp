#define PROBLEM "https://judge.yosupo.jp/problem/log_of_formal_power_series"
#include "include/mtl/fps.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n; cin>>n;
    vector<int> A(n);
    for (int i = 0; i < n; i++) cin>>A[i];
    Fps<> f(A.begin(), A.end());
    auto g = f.log_dense();
    for (int i = 0; i < n; i++)
        cout << g[i] << ' ';
    cout << endl;
}