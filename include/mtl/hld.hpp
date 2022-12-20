#pragma once
#include <cstddef>
#include <vector>

struct Hld {
  int n;
  std::vector<std::vector<int>> edge;
  std::vector<int> size, in, out, head, rev, par, depth;
 private:
  void dfs_sz(int v, int p, int d) {
    par[v] = p;
    size[v] = 1;
    if (!edge[v].empty() and edge[v][0] == p)
      std::swap(edge[v][0], edge[v].back());
    for (auto& t:edge[v]) {
      if (t == p) continue;
      dfs_sz(t, v, d+1);
      size[v] += size[t];
      if (size[edge[v][0]] < size[t])
        std::swap(edge[v][0], t);
    }
  }
  void dfs_hld(int v, int p, int& times) {
    in[v] = times++;
    rev[in[v]] = v;
    if (edge[v][0] != p) {
      int t = edge[v][0];
      head[t] = head[v];
      depth[t] = depth[v];
      dfs_hld(t, v, times);
    }
    for (size_t i = 1; i < edge[v].size(); i++) {
      int t = edge[v][i];
      if (t == p) continue;
      head[t] = t;
      depth[t] = depth[v] + 1;
      dfs_hld(t, v, times);
    }
    out[v] = times;
  }

 public:
  Hld(int n) : n(n), edge(n), size(n), in(n), out(n), head(n), rev(n), par(n), depth(n) {}

  inline void add_edge(int a, int b) {
    edge[a].push_back(b);
    edge[b].push_back(a);
  }

  void build(int root = 0) {
    dfs_sz(root, -1, 0);
    int t = 0;
    head[root] = root;
    depth[root] = 0;
    dfs_hld(root, -1, t);
  }

  inline int lca(int a, int b) const {
    if (depth[a] > depth[b]) std::swap(a, b);
    while (depth[a] < depth[b]) {
      b = par[head[b]];
    }
    while (head[a] != head[b]) {
      a = par[head[a]];
      b = par[head[b]];
    }
    return in[a] < in[b] ? a : b;
  }

  template<class T, typename Query,
      bool INCLUDE_LCA = true>
  T query(int u, int v, Query Q) const {
    if (depth[u] > depth[v]) std::swap(u,v);
    T um, vm;
    auto up = [&](int& v, T& ret) {
      ret = Q(in[head[v]], in[v]+1) * ret;
      v = par[head[v]];
    };
    while (depth[u] < depth[v]) {
      up(v, vm);
    }
    while (head[u] != head[v]) {
      up(u, um);
      up(v, vm);
    }
    if (in[u] > in[v]) {
      std::swap(u,v);
      std::swap(um,vm);
    }
    int l = INCLUDE_LCA ? in[u] : in[u]+1;
    return ~um * Q(l, in[v]+1) * vm;
  }

  template<typename T, typename Set>
  void set(int i, T&& val, Set S) const {
    S(in[i], std::forward<T>(val));
  }

  template<typename T, typename Upd,
      bool INCLUDE_LCA = true>
  void update(int u, int v, const T& val, Upd U) const {
    if (depth[u] > depth[v]) std::swap(u,v);
    auto up = [&](int& v) {
      U(in[head[v]], in[v]+1, val);
      v = par[head[v]];
    };
    while (depth[u] < depth[v]) {
      up(v);
    }
    while (head[u] != head[v]) {
      up(u);
      up(v);
    }
    if (in[u] > in[v]) std::swap(u,v);
    int l = INCLUDE_LCA ? in[u] : in[u]+1;
    U(l, in[v]+1, val);
  }
};
