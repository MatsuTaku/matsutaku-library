#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"
#include "include/mtl/fenwick_tree.hpp"
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
  cin.tie(nullptr); ios::sync_with_stdio(false);

  int N,Q; cin>>N>>Q;
  std::vector<int> A(N); for (auto& a : A) cin>>a;
  FenwickTree<ll> bit(A.begin(), A.end());

  for (int q = 0; q < Q; q++) {
    int t; cin>>t;
    if (t == 0) {
      int p,x; cin>>p>>x;
      bit.add(p, x);
    } else if (t == 1) {
      int l,r; cin>>l>>r;
      auto ans = bit.range_sum(l,r);
      cout << ans << endl;
    }
  }

  return 0;
}
