#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"
#include "../../include/mtl/compress_int.hpp"
#include "../../include/mtl/mo.hpp"
#include "../../include/mtl/fenwick_tree.hpp"
#include <bits/stdc++.h>
using namespace std;
using lint = long long;

constexpr int N = 1e5;

int main() {
  int n,q; cin>>n>>q;
  vector<int> A(n);
  for (int i = 0; i < n; i++) {
    cin>>A[i];
  }
  auto id = Compressor<int>::compress(A.begin(), A.end());
  auto k = id.size();

  MoSolver mo;
  for (int i = 0; i < q; i++) {
    int l,r; cin>>l>>r; 
    mo.add_segment(l, r);
  }

  FenwickTree<int> D(k);
  vector<lint> ans(q);
  lint inv_sum = 0;
  auto _pushl = [&](int i) {
    int vi = id[A[i]];
    inv_sum += D.sum(0, vi);
    D.add(vi, 1);
  };
  auto _pushr = [&](int i) {
    int vi = id[A[i]];
    inv_sum += D.sum(vi+1, k);
    D.add(vi, 1);
  }; 
  auto _popl = [&](int i) {
    int vi = id[A[i]];
    inv_sum -= D.sum(0, vi);
    D.add(vi, -1);
  };
  auto _popr = [&](int i) {
    int vi = id[A[i]];
    inv_sum -= D.sum(vi+1, k);
    D.add(vi, -1);
  };
  auto rem = [&](int t) {
    ans[t] = inv_sum;   
  }; 
  mo.solve(_pushl, _pushr, _popl, _popr, rem);

  for (lint v : ans) cout << v << endl;
}
