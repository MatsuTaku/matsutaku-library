#define PROBLEM "https://judge.yosupo.jp/problem/point_add_rectangle_sum"
#include "../../include/mtl/ordinal_range_search.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
  int n,q; cin>>n>>q;
  ORS<int, long long> ors;
  for (int i = 0; i < n; i++) {
    int x,y,w; cin>>x>>y>>w;
    ors.add(x,y,w);
  }
  vector<tuple<int,int,int,int,int>> Q(q);
  for (int i = 0; i < q; i++) {
    int t; cin>>t;
    if (t == 0) {
      int x,y,w; cin>>x>>y>>w;
      Q[i] = {0,x,y,w,0};
      ors.add(x,y,0);
    } else {
      int l,d,r,u; cin>>l>>d>>r>>u;
      Q[i] = {1,l,d,r,u};
    }
  }
  ors.build();
  for (int i = 0; i < q; i++) {
    int t = get<0>(Q[i]);
    if (t == 0) {
      int x,y,w,_;
      std::tie(t,x,y,w,_) = Q[i];
      ors.weight_add(x,y,w);
    } else {
      int l,d,r,u;
      std::tie(t,l,d,r,u) = Q[i];
      cout << ors.sum(l,r,d,u) << endl;
    }
  }
}