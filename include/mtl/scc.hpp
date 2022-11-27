#pragma once
#include "graph.hpp"
#include <stack>

struct SccGraph {
  int n;
  Graph g,ig;
  explicit SccGraph(int n = 0) : n(n), g(n), ig(n) {}

  void add_edge(int a, int b) {
    g.add_edge_dir(a, b);
    ig.add_edge_dir(b, a);
  }

  std::vector<std::vector<int>> scc() const {
    std::vector<bool> vis(n);
    std::stack<int> st;

    auto dfs = [&](auto& f, int s) {
      if (vis[s]) return;
      vis[s] = true;
      for (int t:g.g[s])
        f(f, t);
      st.push(s);
    };
    for (int i = 0; i < n; i++)
      dfs(dfs, i);

    vis.assign(n, false);
    auto rdfs = [&](auto& f, int s, std::vector<int>& c) {
      if (vis[s]) return;
      vis[s] = true;
      for (int t:ig.g[s])
        f(f, t, c);
      c.push_back(s);
    };
    std::vector<std::vector<int>> ret;
    while (!st.empty()) {
      int u = st.top(); st.pop();
      if (vis[u]) continue;
      ret.emplace_back();
      rdfs(rdfs, u, ret.back());
    }
    return ret;
  }
};