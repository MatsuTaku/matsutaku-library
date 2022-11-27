#pragma once
#include <queue>
#include <vector>

struct Graph {
  int n;
  std::vector<std::vector<int>> g;
  std::vector<int> in;
  std::vector<int> tour, L, R; // For Euler tour.

  Graph(int n) : n(n), g(n), in(n) {}

  void add_edge_dir(int a, int b) {
    g[a].push_back(b);
    in[b]++;
  }
  void add_edge(int a, int b) {
    add_edge_dir(a, b);
    add_edge_dir(b, a);
  }

  template <typename T> void bfs(int s, std::vector<T> &dist, T INF) const;
  template <typename T> bool topological_sort(std::vector<T> &order) const;
  void euler_tour(int root);
};

template <typename T>
void Graph::bfs(int s, std::vector<T> &dist, T INF) const {
  dist.assign(n, INF);
  std::queue<int> qs;
  dist[s] = 0;
  qs.push(s);
  while (!qs.empty()) {
    auto c = qs.front();
    qs.pop();
    auto d = dist[c];
    for (auto t : g[c]) {
      if (dist[t] <= d + 1)
        continue;
      dist[t] = d + 1;
      qs.push(t);
    }
  }
}

template <typename T>
bool Graph::topological_sort(std::vector<T> &order) const {
  order.resize(0);
  order.reserve(n);
  std::vector<int> incnt(n);
  auto dfs = [&](auto dfs, int s) -> void {
    order.push_back(s);
    for (auto t : g[s]) {
      if (++incnt[t] == in[t]) {
        dfs(dfs, t);
      }
    }
  };
  for (int i = 0; i < n; i++) {
    if (in[i] == 0)
      dfs(dfs, i);
  }
  return (int)order.size() == n;
}

void Graph::euler_tour(int root = 0) {
  tour.clear();
  L.resize(n);
  R.resize(n);
  auto dfs = [&](auto &f, int s, int p, int k) -> int {
    tour.push_back(s);
    L[s] = k;
    R[s] = k++;
    for (int t : g[s]) {
      if (t == p)
        continue;
      k = f(f, t, s, k);
      tour.push_back(s);
      R[s] = k++;
    }
    return k;
  };
  dfs(dfs, root, -1, 0);
}

template <typename W> struct Network {
  using weight_type = W;
  int n;
  struct Path {
    int t, w;
    Path(int t, W w) : t(t), w(w) {}
  };
  std::vector<std::vector<Path>> g;
  std::vector<int> in;

  Network(int n) : n(n), g(n), in(n) {}

  void add_edge_dir(int a, int b, W c) {
    g[a].emplace_back(b, c);
    in[b]++;
  }
  void add_edge(int a, int b, W c) {
    add_edge_dir(a, b, c);
    add_edge_dir(b, a, c);
  }

  template <typename T>
  void dijkstra_search(int s, std::vector<T> &dist, T INF) const;
};
