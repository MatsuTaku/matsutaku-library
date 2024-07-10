#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_formal_power_series"
// #define IGNORE
#include "../../../include/mtl/fps/exp.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n; cin>>n;
    vector<int> A(n);
    for (int i = 0; i < n; i++) cin>>A[i];
    Fps<> f(A.begin(), A.end());
    auto g = f.exp_dense();
    for (int i = 0; i < n; i++)
        cout << g[i] << ' ';
    cout << endl;
}