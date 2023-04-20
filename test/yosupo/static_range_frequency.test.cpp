#define PROBLEM "https://judge.yosupo.jp/problem/static_range_frequency"
#include "../../include/mtl/succinct/wavelet_matrix.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n,q; cin>>n>>q;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin>>a[i];
    }
    WaveletMatrix<int> wm(a.begin(), a.end());
    while (q--) {
        int l,r,x; cin>>l>>r>>x;
        cout << wm.range_rank(x,l,r) << endl;
    }
}