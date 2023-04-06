#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_A"

#include <bits/stdc++.h>
using namespace std;

constexpr int INF = 11e8;

int main() {
  int v,e,r; cin>>v>>e>>r;
  vector<pair<int,int>> G[v];
  for (int i = 0; i < e; i++) {
    int s,t,d; cin>>s>>t>>d;
    G[s].emplace_back(t, d);
  }

  vector<int> dist(v, INF);
  priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> qs;
  dist[r] = 0;
  qs.emplace(0, r);
  while (!qs.empty()) {
    auto [c, s] = qs.top(); qs.pop();
    if (dist[s] < c) continue;
    for (auto [t, d] : G[s]) {
      if (dist[t] <= c + d) continue;
      dist[t] = c + d;
      qs.emplace(c + d, t);
    }
  }
  for (int i = 0; i < v; i++) {
    if (dist[i] != INF) {
      cout << dist[i] << endl;
    } else {
      cout << "INF" << endl;
    }
  }

}
