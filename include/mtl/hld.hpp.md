---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/hld.hpp\"\n#include <vector>\n\nstruct Hld {\n\
    \  std::vector<std::vector<int>> edge;\n  std::vector<int> size, in, out, head,\
    \ rev, par, depth;\n private:\n  void dfs_sz(int v, int p, int d) {\n    par[v]\
    \ = p;\n    size[v] = 1;\n    if (!edge[v].empty() and edge[v][0] == p)\n    \
    \  std::swap(edge[v][0], edge[v].back());\n    for (auto& t:edge[v]) {\n     \
    \ if (t == p) continue;\n      dfs_sz(t, v, d+1);\n      size[v] += size[t];\n\
    \      if (size[edge[v][0]] < size[t])\n        std::swap(edge[v][0], t);\n  \
    \  }\n  }\n  void dfs_hld(int v, int p, int& times) {\n    in[v] = times++;\n\
    \    rev[in[v]] = v;\n    if (edge[v][0] != p) {\n      int t = edge[v][0];\n\
    \      head[t] = head[v];\n      depth[t] = depth[v];\n      dfs_hld(t, v, times);\n\
    \    }\n    for (size_t i = 1; i < edge[v].size(); i++) {\n      int t = edge[v][i];\n\
    \      if (t == p) continue;\n      head[t] = t;\n      depth[t] = depth[v] +\
    \ 1;\n      dfs_hld(t, v, times);\n    }\n    out[v] = times;\n  }\n\n public:\n\
    \  Hld(int n) : edge(n), size(n), in(n), out(n), head(n), rev(n), par(n), depth(n)\
    \ {}\n\n  void add_edge(int a, int b) {\n    edge[a].push_back(b);\n    edge[b].push_back(a);\n\
    \  }\n\n  void build(int root = 0) {\n    dfs_sz(root, -1, 0);\n    int t = 0;\n\
    \    head[root] = root;\n    depth[root] = 0;\n    dfs_hld(root, -1, t);\n  }\n\
    \n  int lca(int a, int b) const {\n    if (depth[a] > depth[b]) std::swap(a, b);\n\
    \    while (depth[a] < depth[b]) {\n      b = par[head[b]];\n    }\n    while\
    \ (head[a] != head[b]) {\n      a = par[head[a]];\n      b = par[head[b]];\n \
    \   }\n    if (in[a] > in[b]) std::swap(a, b);\n    return a;\n  }\n\n  template<typename\
    \ T, T (*Q)(int, int), T (*M)(T, T), T (*E)(),\n      bool INCLUDE_LCA = true>\n\
    \  T query(int u, int v) const {\n    T ret = E();\n    if (depth[u] > depth[v])\
    \ std::swap(u,v);\n    auto up = [&](int& v) {\n      ret = M(ret, Q(in[head[v]],\
    \ in[v]+1));\n      v = par[head[v]];\n    };\n    while (depth[u] < depth[v])\
    \ {\n      up(v);\n    }\n    while (head[u] != head[v]) {\n      up(u);\n   \
    \   up(v);\n    }\n    if (in[u] > in[v]) std::swap(u,v);\n    int l = INCLUDE_LCA\
    \ ? in[u] : in[u]+1;\n    ret = M(ret, Q(l, in[v]+1));\n    return ret;\n  }\n\
    \n  template<typename T, void (*S)(int, T)>\n  void set(int i, int val) const\
    \ {\n    S(in[i], val);\n  }\n\n  template<typename T, void (*U)(int, int, T),\n\
    \      bool INCLUDE_LCA = true>\n  void update(int u, int v, T val) const {\n\
    \    if (depth[u] > depth[v]) std::swap(u,v);\n    auto up = [&](int& v) {\n \
    \     U(in[head[v]], in[v]+1, val);\n      v = par[head[v]];\n    };\n    while\
    \ (depth[u] < depth[v]) {\n      up(v);\n    }\n    while (head[u] != head[v])\
    \ {\n      up(u);\n      up(v);\n    }\n    if (in[u] > in[v]) std::swap(u,v);\n\
    \    int l = INCLUDE_LCA ? in[u] : in[u]+1;\n    U(l, in[v]+1, val);\n  }\n};\n"
  code: "#pragma once\n#include <vector>\n\nstruct Hld {\n  std::vector<std::vector<int>>\
    \ edge;\n  std::vector<int> size, in, out, head, rev, par, depth;\n private:\n\
    \  void dfs_sz(int v, int p, int d) {\n    par[v] = p;\n    size[v] = 1;\n   \
    \ if (!edge[v].empty() and edge[v][0] == p)\n      std::swap(edge[v][0], edge[v].back());\n\
    \    for (auto& t:edge[v]) {\n      if (t == p) continue;\n      dfs_sz(t, v,\
    \ d+1);\n      size[v] += size[t];\n      if (size[edge[v][0]] < size[t])\n  \
    \      std::swap(edge[v][0], t);\n    }\n  }\n  void dfs_hld(int v, int p, int&\
    \ times) {\n    in[v] = times++;\n    rev[in[v]] = v;\n    if (edge[v][0] != p)\
    \ {\n      int t = edge[v][0];\n      head[t] = head[v];\n      depth[t] = depth[v];\n\
    \      dfs_hld(t, v, times);\n    }\n    for (size_t i = 1; i < edge[v].size();\
    \ i++) {\n      int t = edge[v][i];\n      if (t == p) continue;\n      head[t]\
    \ = t;\n      depth[t] = depth[v] + 1;\n      dfs_hld(t, v, times);\n    }\n \
    \   out[v] = times;\n  }\n\n public:\n  Hld(int n) : edge(n), size(n), in(n),\
    \ out(n), head(n), rev(n), par(n), depth(n) {}\n\n  void add_edge(int a, int b)\
    \ {\n    edge[a].push_back(b);\n    edge[b].push_back(a);\n  }\n\n  void build(int\
    \ root = 0) {\n    dfs_sz(root, -1, 0);\n    int t = 0;\n    head[root] = root;\n\
    \    depth[root] = 0;\n    dfs_hld(root, -1, t);\n  }\n\n  int lca(int a, int\
    \ b) const {\n    if (depth[a] > depth[b]) std::swap(a, b);\n    while (depth[a]\
    \ < depth[b]) {\n      b = par[head[b]];\n    }\n    while (head[a] != head[b])\
    \ {\n      a = par[head[a]];\n      b = par[head[b]];\n    }\n    if (in[a] >\
    \ in[b]) std::swap(a, b);\n    return a;\n  }\n\n  template<typename T, T (*Q)(int,\
    \ int), T (*M)(T, T), T (*E)(),\n      bool INCLUDE_LCA = true>\n  T query(int\
    \ u, int v) const {\n    T ret = E();\n    if (depth[u] > depth[v]) std::swap(u,v);\n\
    \    auto up = [&](int& v) {\n      ret = M(ret, Q(in[head[v]], in[v]+1));\n \
    \     v = par[head[v]];\n    };\n    while (depth[u] < depth[v]) {\n      up(v);\n\
    \    }\n    while (head[u] != head[v]) {\n      up(u);\n      up(v);\n    }\n\
    \    if (in[u] > in[v]) std::swap(u,v);\n    int l = INCLUDE_LCA ? in[u] : in[u]+1;\n\
    \    ret = M(ret, Q(l, in[v]+1));\n    return ret;\n  }\n\n  template<typename\
    \ T, void (*S)(int, T)>\n  void set(int i, int val) const {\n    S(in[i], val);\n\
    \  }\n\n  template<typename T, void (*U)(int, int, T),\n      bool INCLUDE_LCA\
    \ = true>\n  void update(int u, int v, T val) const {\n    if (depth[u] > depth[v])\
    \ std::swap(u,v);\n    auto up = [&](int& v) {\n      U(in[head[v]], in[v]+1,\
    \ val);\n      v = par[head[v]];\n    };\n    while (depth[u] < depth[v]) {\n\
    \      up(v);\n    }\n    while (head[u] != head[v]) {\n      up(u);\n      up(v);\n\
    \    }\n    if (in[u] > in[v]) std::swap(u,v);\n    int l = INCLUDE_LCA ? in[u]\
    \ : in[u]+1;\n    U(l, in[v]+1, val);\n  }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/hld.hpp
  requiredBy: []
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/hld.hpp
layout: document
redirect_from:
- /library/include/mtl/hld.hpp
- /library/include/mtl/hld.hpp.html
title: include/mtl/hld.hpp
---
