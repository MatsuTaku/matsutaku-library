#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2450"
#include "../../include/mtl/lazy_segment_tree.hpp"
#include "../../include/mtl/hld.hpp"
#include <bits/stdc++.h>

using namespace std;

constexpr int MINF = -1e9;
struct M {
  int l,r,sum,v,sz;
  M() : v(MINF), sz(0) {}
  M(int w) : l(w),r(w),sum(w),v(w), sz(1) {}
  friend M operator*(const M& lhs, const M& rhs) {
    if (lhs.v == MINF) return rhs;
    if (rhs.v == MINF) return lhs;
    M ret;
    ret.l = max(lhs.l, lhs.sum + rhs.l);
    ret.r = max(rhs.r, lhs.r + rhs.sum);
    ret.v = max({lhs.v, rhs.v, lhs.r + rhs.l});
    ret.sum = lhs.sum + rhs.sum;
    ret.sz = lhs.sz + rhs.sz;
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
  M act(const M& m) const {
    if (!f) return m;
    M ret = m;
    ret.sum = v*m.sz;
    ret.l = ret.r = ret.v = (v >= 0 ? ret.sum : v);
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
  vector<int> X(n*2);
  for (int i = 0; i < n; i++)
    X[T.in[i]] = X[n+n-1-T.in[i]] = W[i];
  LazySegmentTree<M,A> RQ(X.begin(), X.end());
  auto range_update = [&](int l, int r, int v) {
    RQ.update(l,r,v);
    RQ.update(n+n-r, n+n-l, v);
  };
  auto query = [&](int l, int r) {
    return RQ.query(l,r);
  };
  auto reverse_query = [&](int l, int r) {
    return RQ.query(n+n-r, n+n-l);
  };
  for (int i = 0; i < q; i++) {
    int t; cin>>t;
    if (t == 1) {
      int a,b,c; cin>>a>>b>>c; a--; b--;
      T.update(a,b,range_update,c);
    } else if (t == 2) {
      int a,b,c; cin>>a>>b>>c; a--; b--;
      cout << T.query(a,b,query,reverse_query).v << endl;
    }
  }
}