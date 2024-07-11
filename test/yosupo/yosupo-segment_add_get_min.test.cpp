#define PROBLEM "https://judge.yosupo.jp/problem/segment_add_get_min"
#include "include/mtl/li_chao_tree.hpp"
#include "include/mtl/compress_int.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
  int n,q; cin>>n>>q;
  vector<array<long long, 5>> Q(n+q);
  for (int i = 0; i < n; i++) {
    long long l,r,a,b; cin>>l>>r>>a>>b;
    Q[i] = {0,l,r,a,b};
  }
  Compressor<int> xcmp;
  for (int i = 0; i < q; i++) {
    int t; cin>>t;
    if (t == 0) {
      long long l,r,a,b; cin>>l>>r>>a>>b;
      Q[n+i] = {0, l, r, a, b};
    } else {
      int p; cin>>p;
      Q[n+i] = {1, p};
      xcmp.add(p);
    }
  }
  auto [xc,cx] = xcmp.release_tie();
  LiChaoTree<long long, greater<>> lct(cx.begin(), cx.end());
  for (int i = 0; i < n+q; i++) {
    if (Q[i][0] == 0) {
      long long l = Q[i][1], r = Q[i][2], a = Q[i][3], b = Q[i][4];
      lct.add_segment(a, b, l, r);
    } else {
      int p = Q[i][1];
      auto ans = lct.get(p);
      if (ans != LiChaoTree<long long, greater<>>::INF)
        cout << ans << endl;
      else
        cout << "INFINITY" << endl;
    }
  }
}