#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"
#include "include/mtl/lazy_segment_tree.hpp"
#include "include/mtl/modular.hpp"
#include <bits/stdc++.h>

using namespace std;
using ll = long long;
constexpr ll MOD = 998244353;
using mint = Modular<MOD>;

struct Sum {
  mint a=0;
  int sz=0;
  Sum operator*(const Sum& r) const {
    return {a+r.a, sz+r.sz};
  }
};

struct Affine {
  mint b=1, c=0;
  Affine& operator*=(const Affine& r) {
    b *= r.b;
    c = c*r.b + r.c;
    return *this;
  }
  Sum act(const Sum& a) const {
    return {b*a.a + c*a.sz, a.sz};
  }
};

int main() {
  cin.tie(nullptr); ios::sync_with_stdio(false);

  int N,Q; cin>>N>>Q;
  vector<Sum> A(N); 
  for (auto& a : A) {
    cin>>a.a;
    a.sz = 1;
  }
  SegmentTreebase<Sum, Affine> rsq(A.begin(), A.end());

  for (int q = 0; q < Q; q++) {
    int t; cin>>t;
    if (t == 0) {
      int l,r,b,c; cin>>l>>r>>b>>c;
      rsq.update(l,r, {b,c});
    } else if (t == 1) {
      int l,r; cin>>l>>r;
      auto ans = rsq.query(l,r);
      cout << ans.a << endl;
    }
  }

  return 0;
}
