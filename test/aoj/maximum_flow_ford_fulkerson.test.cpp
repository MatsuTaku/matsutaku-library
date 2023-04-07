#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_A"
#include "../../include/mtl/max_flow.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
  int V,E; cin>>V>>E;
  FordFulkerson G(V);
  for (int i = 0; i < E; i++) {
    int u,v,c; cin>>u>>v>>c;
    G.add_edge(u, v, c);
  }
  auto f = G.flow(0,V-1);
  cout << f << endl;
}