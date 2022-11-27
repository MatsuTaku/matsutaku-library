#pragma once
#include <limits>
#include <algorithm>
#include <vector>
#include <queue>
#include <cassert>
#include <iostream>

struct MaxFlowAlgorithm {
  virtual void add_edge(int u, int v, long long c) = 0;
  virtual long long flow(int s, int t, long long fup) = 0;
};

struct FlowPath {
  int t;
  long long cap;
  int rev;
  FlowPath(int t, long long flow, int rev) :
      t(t), cap(flow), rev(rev) {}
};

struct FordFulkerson : MaxFlowAlgorithm {
  std::vector<std::vector<FlowPath>> g;
  std::vector<bool> visited;
  explicit FordFulkerson(int n) : g(n), visited(n) {}

  void add_edge(int a, int b, long long f) override {
    g[a].emplace_back(b, f, g[b].size());
    g[b].emplace_back(a, 0, g[a].size()-1);
  }
  long long dfs(int t, int u, long long f) {
    if (visited[u])
      return 0;
    visited[u] = true;
    if (u == t)
      return f;
    for (auto& e : g[u]) {
      if (e.cap == 0) continue;
      auto flow = dfs(t, e.t, std::min(f, e.cap));
      if (flow) {
        e.cap -= flow;
        g[e.t][e.rev].cap += flow;
        return flow;
      }
    }
    return 0;
  }
  long long flow(int s, int t, long long fup) override {
    long long flow = 0;
    while (fup > 0) {
      visited.assign(g.size(), false);
      auto f = dfs(t, s, std::numeric_limits<long long>::max());
      if (f > 0) {
        flow += f;
        fup -= f;
      } else {
        break;
      }
    }
    return flow;
  }
  long long flow(int s, int t) {
    return flow(s, t, std::numeric_limits<long long>::max());
  }
};

struct Dinic : MaxFlowAlgorithm {
  std::vector<std::vector<FlowPath>> g;
  std::vector<std::vector<FlowPath>::iterator> nxt;
  std::vector<int> dist;
  explicit Dinic(int n) : g(n), nxt(n), dist(n) {}

  void add_edge(int a, int b, long long f) override {
    g[a].emplace_back(b, f, g[b].size());
    g[b].emplace_back(a, 0, g[a].size()-1);
  }
  long long dfs(int t, int u, long long f) {
    if (u == t) {
      return f;
    }
    long long fsum = 0;
    for (auto& it = nxt[u]; it != g[u].end() and fsum < f; ++it) {
      if (it->cap == 0 or dist[it->t] != dist[u]+1)
        continue;
      auto flow = dfs(t, it->t, std::min(f - fsum, it->cap));
      if (flow) {
        it->cap -= flow;
        g[it->t][it->rev].cap += flow;
        fsum += flow;
      }
    }
    return fsum;
  }
  long long flow(int s, int t, long long fup) override {
    long long flow = 0;
    std::queue<int> qs;
    while (fup) {
      // dual step
      std::transform(g.begin(), g.end(), nxt.begin(), [](auto& e) {
        return e.begin();
      });
      dist.assign(g.size(), std::numeric_limits<int>::max());
      qs.push(s);
      dist[s] = 0;
      while (!qs.empty()) {
        auto u = qs.front(); qs.pop();
        for (auto e : g[u]) {
          if (e.cap == 0 or dist[e.t] <= dist[u]+1)
            continue;
          dist[e.t] = dist[u]+1;
          if (e.t != t)
            qs.push(e.t);
        }
      }
      // primal step
      auto f = dfs(t, s, std::numeric_limits<long long>::max());
      if (f > 0) {
        flow += f;
        fup -= f;
      } else {
        break;
      }
    }
    return flow;
  }
  long long flow(int s, int t) {
    return flow(s, t, std::numeric_limits<long long>::max());
  }
};

using MaxFlowGraph = Dinic;

struct Matching {
  MaxFlowGraph G;
  int n,m,s,g;
  explicit Matching(int n, int m) : G(n+m+2), n(n), m(m), s(n+m), g(s+1) {
    for (int i = 0; i < n; i++)
      G.add_edge(s, i, 1);
    for (int i = 0; i < m; i++)
      G.add_edge(i+n, g, 1);
  }
  void add_edge(int a, int b) {
    G.add_edge(a, b+n, 1);
  }
  int flow() {
    return G.flow(s, g);
  }
  std::vector<std::pair<int,int>> matches() const {
    std::vector<std::pair<int,int>> ret;
    for (int i = 0; i < n; i++) {
      for (auto& e : G.g[i]) {
        if (e.t >= n and e.t < s and e.cap == 0) {
          ret.emplace_back(i, e.t-n);
        }
      }
    }
    return ret;
  }
};

// Project Selection Problem
template<typename P>
struct PSP {
  MaxFlowGraph G;
  int n,s,g;
  P psum;
  explicit PSP(int n) : G(n+2), n(n), s(n), g(s+1), psum(0) {};
  void add_profit(int a, P profit) {
    if (profit > 0)
      psum += profit;
    if (profit > 0)
      G.add_edge(a, g, profit);
    else if (profit < 0)
      G.add_edge(s, a, -profit);
  }
  void add_loss(int a, int b, P loss) {
    G.add_edge(a, b, loss);
  }
  P solve() {
    auto p = G.flow(s, g);
    return psum - p;
  }
  std::vector<bool> selections() const {
    std::vector<bool> sel(n, 1);
    for (auto& e : G.g[g]) {
      if (G.g[e.t][e.rev].cap > 0) {
        sel[e.t] = 0;
      }
    }
    return sel;
  }
};
