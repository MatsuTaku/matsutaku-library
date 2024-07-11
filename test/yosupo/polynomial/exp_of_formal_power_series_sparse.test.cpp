#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_formal_power_series_sparse"
#include "include/mtl/fps/exp.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n,k; cin>>n>>k;
    vector<int> A(n);
    for (int i = 0; i < k; i++) {
        int j,a; cin>>j>>a;
        A[j] = a;
    }
    Fps<> f(A.begin(), A.end());
    auto g = f.exp_sparse();
    for (int i = 0; i < n; i++)
        cout << g[i] << ' ';
    cout << endl;
}