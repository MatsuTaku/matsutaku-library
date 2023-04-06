#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2450"
#include "../include/mtl/lazy_segment_tree.hpp"
#include "../include/mtl/hld.hpp"
#include <bits/stdc++.h>

using namespace std;

constexpr int MINF = -1e9;
struct M {
  int l,r,sum,v;
  M() : v(MINF) {}
  M(int w) : l(w),r(w),sum(w),v(w) {}
  friend M operator*(const M& lhs, const M& rhs) {
    if (lhs.v == MINF) return rhs;
    if (rhs.v == MINF) return lhs;
    M ret;
    ret.l = max(lhs.l, lhs.sum + rhs.l);
    ret.r = max(rhs.r, lhs.r + rhs.sum);
    ret.v = max({lhs.v, rhs.v, lhs.r + rhs.l});
    ret.sum = lhs.sum + rhs.sum;
    return ret;
  }
  M operator~() {
    M ret = *this;
    swap(ret.l, ret.r);
    return ret;
  }
};
struct A {
  int v;
  bool f;
  A() : f(false) {}
  A(int v) : v(v), f(true) {}
  bool operator()() const { return f; }
  A& operator*=(const A& r) {
    if (r.f) *this = r;
    return *this;
  }
  M act(const M& m,int sz) const {
    assert(f);
    M ret;
    ret.sum = v*sz;
    if (v >= 0) {
      ret.l = ret.r = ret.v = ret.sum;
    } else {
      ret.l = ret.r = ret.v = v;
    }
    return ret;
  }
};

int main() {
  int n,q; cin>>n>>q;
  vector<int> W(n);
  for (auto& w:W) cin>>w;
  Hld T(n);
  for (int i = 0; i < n-1; i++) {
    int s,e; cin>>s>>e; s--; e--;
    T.add_edge(s,e);
  }
  T.build();
  vector<int> X(n);
  for (int i = 0; i < n; i++)
    X[T.in[i]] = W[i];
  LazySegmentTree<M,A> RQ(X.begin(), X.end());
  auto range_update = [&](int l, int r, int v) {
    RQ.update(l,r,v);
  };
  auto query = [&](int l, int r) {
    return RQ.query(l,r);
  };
  for (int i = 0; i < q; i++) {
    int t; cin>>t;
    if (t == 1) {
      int a,b,c; cin>>a>>b>>c; a--; b--;
      T.update(a,b,c,range_update);
    } else if (t == 2) {
      int a,b,c; cin>>a>>b>>c; a--; b--;
      cout << T.query<M>(a,b,query).v << endl;
    }
  }
}