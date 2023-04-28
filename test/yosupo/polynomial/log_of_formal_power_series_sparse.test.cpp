#define PROBLEM "https://judge.yosupo.jp/problem/log_of_formal_power_series_sparse"
// #define IGNORE
#include "../../../include/mtl/fps.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n,k; cin>>n>>k;
    vector<int> A(n);
    for (int i = 0; i < k; i++) {
        int j,a; cin>>j>>a;
        A[j] = a;
    }
    Fps f(A.begin(), A.end());
    auto g = f.log_sparse();
    for (int i = 0; i < n; i++)
        cout << g[i] << ' ';
    cout << endl;
}