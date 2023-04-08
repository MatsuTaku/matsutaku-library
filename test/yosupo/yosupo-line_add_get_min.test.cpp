#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"
#include "../../include/mtl/li_chao_tree.hpp"
#include "../../include/mtl/compress_int.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
  int n,q; cin>>n>>q;
  vector<pair<long long, long long>> L(n);
  for (int i = 0; i < n; i++) {
    long long a,b; cin>>a>>b;
    L[i] = {a,b};
  }
  vector<array<long long,3>> Q(q);
  Compressor<int> xcmp;
  for (int i = 0; i < q; i++) {
    int t; cin>>t;
    if (t == 0) {
      long long a,b; cin>>a>>b;
      Q[i] = {0, a, b};
    } else {
      int p; cin>>p;
      Q[i] = {1, p, 0};
      xcmp.add(p);
    }
  }
  auto [xc,cx] = xcmp.release_tie();
  LiChaoTree<long long, greater<>> lct(cx.begin(), cx.end());
  for (auto [a,b] : L)
    lct.add_line(a, b);
  for (int i = 0; i < q; i++) {
    if (Q[i][0] == 0) {
      long long a = Q[i][1], b = Q[i][2];
      lct.add_line(a, b);
    } else {
      int p = Q[i][1];
      cout << lct.get(p) << endl;
    }
  }
}