#pragma once
#include "graph.hpp"
#include "fibonacci_heap.hpp"

template<typename W>
template<typename T>
void Network<W>::dijkstra_search(int s, std::vector<T>& dist, T INF) const {
  dist.assign(n, INF);
  FibonacciHeap<T, std::greater<>> qs(n);
  dist[s] = 0;
  qs.update(s, 0);
  while (!qs.empty()) {
    auto q = qs.top(); qs.pop();
    auto v = q.first;
    auto d = q.second;
    assert(dist[v] == d);
    for (auto e : g[v]) {
      if (dist[e.t] <= d+e.w) continue;
      dist[e.t] = d+e.w;
      qs.update(e.t, d+e.w);
    }
  }
}