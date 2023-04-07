---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: include/mtl/segment_hld.hpp
    title: include/mtl/segment_hld.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
    title: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp
    title: test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/hld.hpp\"\n#include <cstddef>\n#include <vector>\n\
    \nstruct Hld {\n  int n;\n  std::vector<std::vector<int>> edge;\n  std::vector<int>\
    \ size, in, out, head, rev, par, depth;\n private:\n  void dfs_sz(int v, int p,\
    \ int d) {\n    par[v] = p;\n    size[v] = 1;\n    if (!edge[v].empty() and edge[v][0]\
    \ == p)\n      std::swap(edge[v][0], edge[v].back());\n    for (auto& t:edge[v])\
    \ {\n      if (t == p) continue;\n      dfs_sz(t, v, d+1);\n      size[v] += size[t];\n\
    \      if (size[edge[v][0]] < size[t])\n        std::swap(edge[v][0], t);\n  \
    \  }\n  }\n  void dfs_hld(int v, int p, int& times) {\n    in[v] = times++;\n\
    \    rev[in[v]] = v;\n    if (edge[v][0] != p) {\n      int t = edge[v][0];\n\
    \      head[t] = head[v];\n      depth[t] = depth[v];\n      dfs_hld(t, v, times);\n\
    \    }\n    for (size_t i = 1; i < edge[v].size(); i++) {\n      int t = edge[v][i];\n\
    \      if (t == p) continue;\n      head[t] = t;\n      depth[t] = depth[v] +\
    \ 1;\n      dfs_hld(t, v, times);\n    }\n    out[v] = times;\n  }\n\n public:\n\
    \  Hld(int n) : n(n), edge(n), size(n), in(n), out(n), head(n), rev(n), par(n),\
    \ depth(n) {}\n\n  inline void add_edge(int a, int b) {\n    edge[a].push_back(b);\n\
    \    edge[b].push_back(a);\n  }\n\n  void build(int root = 0) {\n    dfs_sz(root,\
    \ -1, 0);\n    int t = 0;\n    head[root] = root;\n    depth[root] = 0;\n    dfs_hld(root,\
    \ -1, t);\n  }\n\n  inline int lca(int a, int b) const {\n    if (depth[a] > depth[b])\
    \ std::swap(a, b);\n    while (depth[a] < depth[b]) {\n      b = par[head[b]];\n\
    \    }\n    while (head[a] != head[b]) {\n      a = par[head[a]];\n      b = par[head[b]];\n\
    \    }\n    return in[a] < in[b] ? a : b;\n  }\n\n  template<class T, typename\
    \ Query,\n      bool INCLUDE_LCA = true>\n  T query(int u, int v, Query Q) const\
    \ {\n    if (depth[u] > depth[v]) std::swap(u,v);\n    T um, vm;\n    auto up\
    \ = [&](int& v, T& ret) {\n      ret = Q(in[head[v]], in[v]+1) * ret;\n      v\
    \ = par[head[v]];\n    };\n    while (depth[u] < depth[v]) {\n      up(v, vm);\n\
    \    }\n    while (head[u] != head[v]) {\n      up(u, um);\n      up(v, vm);\n\
    \    }\n    if (in[u] > in[v]) {\n      std::swap(u,v);\n      std::swap(um,vm);\n\
    \    }\n    int l = INCLUDE_LCA ? in[u] : in[u]+1;\n    return ~um * Q(l, in[v]+1)\
    \ * vm;\n  }\n\n  template<typename T, typename Set>\n  void set(int i, T&& val,\
    \ Set S) const {\n    S(in[i], std::forward<T>(val));\n  }\n\n  template<typename\
    \ T, typename Upd,\n      bool INCLUDE_LCA = true>\n  void update(int u, int v,\
    \ const T& val, Upd U) const {\n    if (depth[u] > depth[v]) std::swap(u,v);\n\
    \    auto up = [&](int& v) {\n      U(in[head[v]], in[v]+1, val);\n      v = par[head[v]];\n\
    \    };\n    while (depth[u] < depth[v]) {\n      up(v);\n    }\n    while (head[u]\
    \ != head[v]) {\n      up(u);\n      up(v);\n    }\n    if (in[u] > in[v]) std::swap(u,v);\n\
    \    int l = INCLUDE_LCA ? in[u] : in[u]+1;\n    U(l, in[v]+1, val);\n  }\n};\n"
  code: "#pragma once\n#include <cstddef>\n#include <vector>\n\nstruct Hld {\n  int\
    \ n;\n  std::vector<std::vector<int>> edge;\n  std::vector<int> size, in, out,\
    \ head, rev, par, depth;\n private:\n  void dfs_sz(int v, int p, int d) {\n  \
    \  par[v] = p;\n    size[v] = 1;\n    if (!edge[v].empty() and edge[v][0] == p)\n\
    \      std::swap(edge[v][0], edge[v].back());\n    for (auto& t:edge[v]) {\n \
    \     if (t == p) continue;\n      dfs_sz(t, v, d+1);\n      size[v] += size[t];\n\
    \      if (size[edge[v][0]] < size[t])\n        std::swap(edge[v][0], t);\n  \
    \  }\n  }\n  void dfs_hld(int v, int p, int& times) {\n    in[v] = times++;\n\
    \    rev[in[v]] = v;\n    if (edge[v][0] != p) {\n      int t = edge[v][0];\n\
    \      head[t] = head[v];\n      depth[t] = depth[v];\n      dfs_hld(t, v, times);\n\
    \    }\n    for (size_t i = 1; i < edge[v].size(); i++) {\n      int t = edge[v][i];\n\
    \      if (t == p) continue;\n      head[t] = t;\n      depth[t] = depth[v] +\
    \ 1;\n      dfs_hld(t, v, times);\n    }\n    out[v] = times;\n  }\n\n public:\n\
    \  Hld(int n) : n(n), edge(n), size(n), in(n), out(n), head(n), rev(n), par(n),\
    \ depth(n) {}\n\n  inline void add_edge(int a, int b) {\n    edge[a].push_back(b);\n\
    \    edge[b].push_back(a);\n  }\n\n  void build(int root = 0) {\n    dfs_sz(root,\
    \ -1, 0);\n    int t = 0;\n    head[root] = root;\n    depth[root] = 0;\n    dfs_hld(root,\
    \ -1, t);\n  }\n\n  inline int lca(int a, int b) const {\n    if (depth[a] > depth[b])\
    \ std::swap(a, b);\n    while (depth[a] < depth[b]) {\n      b = par[head[b]];\n\
    \    }\n    while (head[a] != head[b]) {\n      a = par[head[a]];\n      b = par[head[b]];\n\
    \    }\n    return in[a] < in[b] ? a : b;\n  }\n\n  template<class T, typename\
    \ Query,\n      bool INCLUDE_LCA = true>\n  T query(int u, int v, Query Q) const\
    \ {\n    if (depth[u] > depth[v]) std::swap(u,v);\n    T um, vm;\n    auto up\
    \ = [&](int& v, T& ret) {\n      ret = Q(in[head[v]], in[v]+1) * ret;\n      v\
    \ = par[head[v]];\n    };\n    while (depth[u] < depth[v]) {\n      up(v, vm);\n\
    \    }\n    while (head[u] != head[v]) {\n      up(u, um);\n      up(v, vm);\n\
    \    }\n    if (in[u] > in[v]) {\n      std::swap(u,v);\n      std::swap(um,vm);\n\
    \    }\n    int l = INCLUDE_LCA ? in[u] : in[u]+1;\n    return ~um * Q(l, in[v]+1)\
    \ * vm;\n  }\n\n  template<typename T, typename Set>\n  void set(int i, T&& val,\
    \ Set S) const {\n    S(in[i], std::forward<T>(val));\n  }\n\n  template<typename\
    \ T, typename Upd,\n      bool INCLUDE_LCA = true>\n  void update(int u, int v,\
    \ const T& val, Upd U) const {\n    if (depth[u] > depth[v]) std::swap(u,v);\n\
    \    auto up = [&](int& v) {\n      U(in[head[v]], in[v]+1, val);\n      v = par[head[v]];\n\
    \    };\n    while (depth[u] < depth[v]) {\n      up(v);\n    }\n    while (head[u]\
    \ != head[v]) {\n      up(u);\n      up(v);\n    }\n    if (in[u] > in[v]) std::swap(u,v);\n\
    \    int l = INCLUDE_LCA ? in[u] : in[u]+1;\n    U(l, in[v]+1, val);\n  }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/hld.hpp
  requiredBy:
  - include/mtl/segment_hld.hpp
  timestamp: '2022-12-20 20:34:44+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp
  - test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
documentation_of: include/mtl/hld.hpp
layout: document
redirect_from:
- /library/include/mtl/hld.hpp
- /library/include/mtl/hld.hpp.html
title: include/mtl/hld.hpp
---
