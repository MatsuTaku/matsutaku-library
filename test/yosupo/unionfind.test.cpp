#define PROBLEM "https://judge.yosupo.jp/problem/unionfind"
#include "../../include/mtl/unionfind.hpp"
#include <iostream>
using namespace std;
using ll = long long;

int main() {
  cin.tie(nullptr); ios::sync_with_stdio(false);

  int N,Q; cin>>N>>Q;
  UnionFind uf(N);

  for (int q = 0; q < Q; q++) {
    int t; cin>>t;
    int u,v; cin>>u>>v;
    if (t == 0) {
      uf.unite(u,v);
    } else if (t == 1) {
      int ans = uf.are_union(u,v) ? 1 : 0;
      cout << ans << endl;
    }
  }

  return 0;
}
