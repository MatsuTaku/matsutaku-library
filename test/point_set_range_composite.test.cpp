#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include "../cpl/segment_tree.hpp"
#include "../cpl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

constexpr ll MOD = 998244353;
using mint = Modular<MOD>;

struct Monoid {
  mint a=1, b=0;
  Monoid operator*(Monoid r) const {
    return {a*r.a, b*r.a+r.b};
  }
  Monoid& operator*=(Monoid r) {return *this = *this * r;}
};

int main() {
  cin.tie(nullptr); ios::sync_with_stdio(false);

  int N,Q; cin>>N>>Q;
  vector<Monoid> F(N); for (auto& f : F) cin>>f.a>>f.b;
  SegmentTree<Monoid> st(F.begin(), F.end());

  for (int q = 0; q < Q; q++) {
    int t; cin>>t;
    if (t == 0) {
      int p,c,d; cin>>p>>c>>d;
      st.set(p, {c,d});
    } else if (t == 1) {
      int l,r,x; cin>>l>>r>>x;
      auto comp = st.query(l,r);
      auto ans = comp.a*x + comp.b;
      cout << ans << endl;
    }
  }

  return 0;
}
