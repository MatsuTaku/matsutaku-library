#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"
#include "include/mtl/segment_tree.hpp"
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

constexpr int INF = 11e8;
struct Min {
  int x = INF;
  Min operator*(const Min& r) const {
    return {std::min(x, r.x)};
  }
};

int main() {
  cin.tie(nullptr); ios::sync_with_stdio(false);

  int N,Q; cin>>N>>Q;

  std::vector<Min> A(N); for (auto& a : A) cin>>a.x;
  SegmentTree<Min> rmq(A.begin(), A.end());

  for (int q = 0; q < Q; q++) {
    int l,r; cin>>l>>r;
    auto ans = rmq.query(l, r).x;
    cout << ans << endl;
  }

  return 0;
}
