#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include "../include/mtl/segment_tree.hpp"
#include "../include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

constexpr ll MOD = 998244353;
using mint = Modular<MOD>;

struct M {
  mint a=1, b=0;
  M operator*(M r) const {
    return {a*r.a, b*r.a+r.b};
  }
  M& operator*=(M r) {return *this = *this * r;}
};

int main() {
  cin.tie(nullptr); ios::sync_with_stdio(false);

  int N,Q; cin>>N>>Q;
  vector<M> F(N); for (auto& f : F) cin>>f.a>>f.b;
  SegmentTree<M> st(F.begin(), F.end());

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
