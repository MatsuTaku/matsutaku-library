#define PROBLEM "https://judge.yosupo.jp/problem/static_range_sum"

#include "../cpl/fenwick_tree.hpp"
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
  cin.tie(nullptr); ios::sync_with_stdio(false);

  int n,q; cin>>n>>q;
  FenwickTree<ll> rsq(n);
  for (int i = 0; i < n; i++) {
    int a; cin>>a;
    rsq.add(i, a);
  }
  for (int i = 0; i < q; i++) {
    int l,r; cin>>l>>r;
    cout << rsq.range_sum(l,r) << endl;
  }

  return 0;
}
