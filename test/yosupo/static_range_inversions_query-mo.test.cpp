#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"
#include "../../include/mtl/mo.hpp"
#include "../../include/mtl/fenwick_tree.hpp"
#include <bits/stdc++.h>
using namespace std;
using lint = long long;

constexpr int N = 1e5;
FenwickTree<lint> C(N);
int A[N];
map<int,int> id;
lint pushl(lint inv, int i) {
  int vi = id[A[i]];
  inv += C.range_sum(0, vi);
  C.add(vi, 1);
  return inv;
}
lint pushr(lint inv, int i) {
  int vi = id[A[i]];
  inv += C.range_sum(vi+1, N);
  C.add(vi, 1);
  return inv;
}
lint popl(lint inv, int i) {
  int vi = id[A[i]];
  C.add(vi, -1);
  inv -= C.range_sum(0, vi);
  return inv;
}
lint popr(lint inv, int i) {
  int vi = id[A[i]];
  C.add(vi, -1);
  inv -= C.range_sum(vi+1, N);
  return inv;
}
lint e() { return 0; }

int main() {
  int n,q; cin>>n>>q;
  for (int i = 0; i < n; i++) {
    cin>>A[i];
    id[A[i]] = 0;
  }
  {
    int k = 0;
    for (auto& [i,v] : id) v = k++;
  }
  MoSolver<lint, pushl, pushr, popl, popr, e> mo;
  for (int i = 0; i < q; i++) {
    int l,r; cin>>l>>r;
    mo.add_segment(l, r);
  }
  vector<lint> ans;
  mo.solve(ans);
  for (lint v : ans) cout << v << endl;
}
