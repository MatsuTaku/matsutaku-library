#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"
#include "include/mtl/succinct/wavelet_matrix.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
  int n,q; cin>>n>>q;
  vector<int> A(n);
  for (int i = 0; i < n; i++) cin>>A[i];
  WaveletMatrix<int> wa(A.begin(), A.end());
  for (int i = 0; i < q; i++) {
    int l,r,k; cin>>l>>r>>k;
    cout << wa.quantile(l, r, k) << endl;
  }
}