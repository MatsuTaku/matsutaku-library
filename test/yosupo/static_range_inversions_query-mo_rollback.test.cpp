#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"
#include "include/mtl/compress_int.hpp"
#include "include/mtl/mo.hpp"
#include "include/mtl/lazy_segment_tree.hpp"
#include "include/mtl/monoid.hpp"
#include <bits/stdc++.h>
using namespace std;
using lint = long long;

constexpr int N = 1e5;
int sum(int a, int b) { return a+b; }
int e() { return 0; }
using sm = Monoid<int,sum,e>;
using P = pair<int,int>;
sm mapping(P c, sm x) {
  return x.val()*c.first + c.second;
}
P composition(P l, P r) {
  return {l.first*r.first, l.second*r.first + r.second};
}
P Id() { return {1,0}; }
struct um {
  P x;
  um() : x(Id()) {}
  um(P x) : x(x) {}
  um& operator*=(const um& rhs) {
    x = composition(x, rhs.x);
    return *this; 
  }
  sm act(const sm& s) const { 
    return mapping(x, s);
  }
};

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

  SegmentTreebase<sm, um> D(k);
  vector<lint> ans(q, -1);
  lint inv_sum = 0;
  stack<int> hist;
  lint saved_inv_sum = 0;
  int size = 0;
  auto _pushl = [&](int i) {
    int vi = id[A[i]];
    auto count = D.query(0, vi).val();
    assert(count <= size);
    inv_sum += count;
    hist.push(vi); 
    D.update(vi, make_pair(1,1));
    size++;   
  };
  auto _pushr = [&](int i) {
    int vi = id[A[i]];
    auto count = D.query(vi+1, k).val();
    assert(count <= size);
    inv_sum += count;
    hist.push(vi); 
    D.update(vi, make_pair(1,1));   
    size++;
  }; 
  // auto _popl = [&](int i) {
  //   int vi = id[A[i]];
  //   inv_sum -= D.query(0, vi).val();
  //   D.update(vi, make_pair(1,-1));
  // };
  // auto _popr = [&](int i) {
  //   int vi = id[A[i]];
  //   inv_sum -= D.query(vi+1, k).val();
  //   D.update(vi, make_pair(1,-1));
  // };
  auto rem = [&](int t) {
    ans[t] = inv_sum;   
  }; 
  // mo.solve(_pushl, _pushr, _popl, _popr, rem);

  auto init = [&]() {
    D.update(0, k, make_pair(0,0));
    inv_sum = 0;
    size = 0;
  };
  auto snapshot = [&]() {
    hist = {};
    saved_inv_sum = inv_sum;
  };
  auto rollback = [&]() { 
    size -= hist.size();
    while (!hist.empty()) {
      int vi = hist.top(); hist.pop();
      D.update(vi, make_pair(1,-1));
    }
    inv_sum = saved_inv_sum;
  }; 
  mo.solve_rollback(_pushl, _pushr, rem, init, snapshot, rollback);

  for (lint v : ans) cout << v << endl;
}
