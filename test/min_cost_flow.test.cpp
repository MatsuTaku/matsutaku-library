#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_B"
#include "../include/mtl/min_cost_flow.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
  int V,E,F; cin>>V>>E>>F;
  MinCostFlowGraph<int> mcfg(V);
  for (int i = 0; i < E; i++) {
    int u,v,c,d; cin>>u>>v>>c>>d;
    mcfg.add_edge(u, v, c, d);
  }
  auto res = mcfg.flow(0, V-1, F);
  cout << (res.first == F ? res.second : -1) << endl;
}
