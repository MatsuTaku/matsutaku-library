#pragma once
#include <cstddef>
#include <vector>

struct Hld {
  int r,n;
  std::vector<std::vector<int>> edge;
  std::vector<int> size, in, out, head, rev, par, depth, clen;
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
    clen[v] = 1;
    if (!edge[v].empty() and edge[v][0] != p) {
      int t = edge[v][0];
      head[t] = head[v];
      depth[t] = depth[v];
      dfs_hld(t, v, times);
      clen[v] += clen[t];
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
  Hld(int n) : r(0), n(n), edge(n), size(n), in(n), out(n), head(n), rev(n), par(n), depth(n), clen(n) {}

  inline void add_edge(int a, int b) {
    edge[a].push_back(b);
    edge[b].push_back(a);
  }

  void build(int root = 0) {
    r = root;
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

 private:
  template<class T, class Query, class ReverseQuery>
  T _query(int u, int v, Query Q, ReverseQuery RQ, bool include_lca) const {
    T um, vm;
    auto u_up = [&]() {
      um = um * (T)RQ(in[head[u]], in[u]+1);
      u = par[head[u]];
    };
    auto v_up = [&]() {
      vm = (T)Q(in[head[v]], in[v]+1) * vm;
      v = par[head[v]];
    };
    while (depth[u] > depth[v])
      u_up();
    while (depth[u] < depth[v])
      v_up();
    while (head[u] != head[v]) {
      u_up();
      v_up();
    }
    if (in[u] < in[v]) {
      int l = include_lca ? in[u] : in[u]+1;
      return um * (T)Q(l, in[v]+1) * vm;
    } else {
      int l = include_lca ? in[v] : in[v]+1;
      return um * (T)RQ(l, in[u]+1) * vm;
    }
  }

 public:
  template<class T, class Query, class ReverseQuery>
  T query(int u, int v, Query Q, ReverseQuery RQ, bool include_lca = true) const {
    return _query<T>(u, v, Q, RQ, include_lca);
  }

  /// Query for commutative monoid
  template<class T, class Query>
  T query(int u, int v, Query Q, bool include_lca = true) const {
    return _query<T>(u, v, Q, Q, include_lca);
  }

  template<class Set, class T>
  void set(int i, Set S, T&& val) const {
    S(in[i], std::forward<T>(val));
  }

  template<typename Upd, typename T>
  void update(int u, int v, Upd U, const T& val, bool include_lca = true) const {
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
    int l = include_lca ? in[u] : in[u]+1;
    U(l, in[v]+1, val);
  }

public:
  template<class Add, class Sum>
  void subtree_build(Add A, Sum S) const {
    dfs_subtree_build(A, S, r);
  }
 private:
  template<class Add, class Sum>
  void dfs_subtree_build(Add A, Sum S, int u) const {
    for (size_t i = 0; i < edge[u].size(); i++) {
      auto v = edge[u][i];
      if (v == par[u]) continue;
      dfs_subtree_build(A, S, v);
      if (i > 0)
        A(in[u], S(in[v], in[v]+clen[v]));
    }
  }
 public:
  template<class T, class Sum>
  T subtree_sum(int r, Sum S) const {
    return (T)S(in[r], in[r]+clen[r]);
  }
  template<class T, class Add>
  void subtree_point_add(int u, Add A, const T& val) const {
    while (u != -1) {
      A(in[u], val);
      u = par[head[u]];
    }
  }
};
