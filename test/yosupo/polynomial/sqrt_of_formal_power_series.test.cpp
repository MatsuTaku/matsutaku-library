#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_of_formal_power_series"
#include "../../../include/mtl/fps/pow.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n; cin>>n;
    vector<int> A(n);
    for (int i = 0; i < n; i++) cin>>A[i];
    Fps<> f(A.begin(), A.end());
    if (f.is_square()) {
        auto g = f.sqrt().inline_pre(n);
        for (int i = 0; i < n; i++)
            cout << g[i] << ' ';
        cout << endl;
    } else {
        cout << -1 << endl;
    }
}