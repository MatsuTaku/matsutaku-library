#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/GRL_3_C"
#include "include/mtl/scc.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
  int V,E; cin>>V>>E;
  SccGraph G(V);
  for (int i = 0; i < E; i++) {
    int s,t; cin>>s>>t;
    G.add_edge(s, t);
  }
  vector<int> id(V);
  auto scc = G.scc();
  for (int i = 0; i < (int) scc.size(); i++) {
    for (int u : scc[i])
      id[u] = i;
  }
  int Q; cin>>Q;
  for (int i = 0; i < Q; i++) {
    int u,v; cin>>u>>v;
    cout << (id[u] == id[v]) << endl;
  }
}