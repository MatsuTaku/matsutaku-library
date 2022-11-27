#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include "../include/mtl/lazy_segment_tree.hpp"
#include "../include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

constexpr ll MOD = 998244353;
using mint = Modular<MOD>;

struct SumMonoid {
  mint a=0;
  SumMonoid operator*(SumMonoid r) const {
    return {a+r.a};
  }
  SumMonoid& operator*=(SumMonoid r) {return *this = *this * r;}
};

struct FnMonoid {
  mint b=1, c=0;
  FnMonoid operator*(FnMonoid r) const {
    return {b*r.b, c*r.b+r.c};
  }
  FnMonoid& operator*=(FnMonoid r) {return *this = *this * r;}
  bool operator==(FnMonoid r) const {return b==r.b and c==r.c;}
  SumMonoid act(SumMonoid a, size_t num) const {
    return {b*a.a + c*num};
  }
};

int main() {
  cin.tie(nullptr); ios::sync_with_stdio(false);

  int N,Q; cin>>N>>Q;
  vector<SumMonoid> A(N); for (auto& a : A) cin>>a.a;
  LazySegmentTree<SumMonoid, FnMonoid> rsq(A.begin(), A.end());

  for (int q = 0; q < Q; q++) {
    int t; cin>>t;
    if (t == 0) {
      int l,r,b,c; cin>>l>>r>>b>>c;
      rsq.range_update(l,r, {b,c});
    } else if (t == 1) {
      int l,r; cin>>l>>r;
      auto ans = rsq.query(l,r);
      cout << ans.a << endl;
    }
  }

  return 0;
}
