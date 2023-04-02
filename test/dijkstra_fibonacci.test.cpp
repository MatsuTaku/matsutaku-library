#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_A"

#include "../include/mtl/fibonacci_heap.hpp"
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
  FibonacciHeap<int, greater<>> qs(v);
  dist[r] = 0;
  qs.push(r, 0);
  while (!qs.empty()) {
    auto [s, c] = qs.top(); qs.pop();
    assert(c == dist[s]);
    for (auto [t, d] : G[s]) {
      if (dist[t] <= c + d) continue;
      dist[t] = c + d;
      qs.push(t, c + d);
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
