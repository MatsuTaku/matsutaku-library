#pragma once
#include <vector>
#include <limits>
#include "fibonacci_heap.hpp"

template<typename C>
struct MinCostFlowPath {
  int t, rev, cap;
  C cost;
  MinCostFlowPath(int t, int rev, int cap, C cost)
      : t(t), rev(rev), cap(cap), cost(cost) {}
};

template<typename C>
struct MinCostFlowGraph {
  const C INF = std::numeric_limits<C>::max();
  int n;
  std::vector<std::vector<MinCostFlowPath<C>>> g;
  std::vector<C> dist, h;
  std::vector<int> prevv, preve;
  explicit MinCostFlowGraph(int n)
      : n(n), g(n), dist(n), h(n), prevv(n), preve(n) {}

  void add_edge(int a, int b, int cap, C cost) {
    g[a].emplace_back(b, g[b].size(), cap, cost);
    g[b].emplace_back(a, g[a].size()-1, 0, -cost);
  }

  std::pair<int, C> flow(int s, int t, int flow_limit = std::numeric_limits<int>::max()) {
    h.assign(n, 0);
    auto f = 0;
    C res = 0;
    FibonacciHeap<C, std::greater<C>> qs(n);
    while (f < flow_limit) {
      dist.assign(n, INF);
      dist[s] = 0;
      qs.push(s, 0);
      while (!qs.empty()) {
        auto q = qs.top(); qs.pop();
        auto u = q.first;
        auto d = q.second;
        for (size_t i = 0; i < g[u].size(); i++) {
          auto& e = g[u][i];
          if (e.cap == 0) continue;
          auto cost = d + e.cost + h[u] - h[e.t];
          if (cost >= dist[e.t]) continue;
          dist[e.t] = cost;
          qs.push(e.t, cost);
          prevv[e.t] = u;
          preve[e.t] = i;
        }
      }
      if (dist[t] == INF)
        break;
      for (int i = 0; i < n; i++) if (dist[i] != INF)
        h[i] += dist[i];
      int d = flow_limit - f;
      C cost_sum = 0;
      for (int i = t; i != s; i = prevv[i]) {
        auto& e = g[prevv[i]][preve[i]];
        d = std::min(d, e.cap);
        cost_sum += e.cost;
      }
      f += d;
      res += (C) d * cost_sum;
      for (int i = t; i != s; i = prevv[i]) {
        auto& e = g[prevv[i]][preve[i]];
        e.cap -= d;
        g[i][e.rev].cap += d;
      }
    }
    return std::make_pair(f, res);
  }
};