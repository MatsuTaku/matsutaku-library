---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':question:'
    path: include/mtl/segment_hld.hpp
    title: include/mtl/segment_hld.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
    title: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp
    title: test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp
  - icon: ':x:'
    path: test/yosupo/vertex_add_path_sum.test.cpp
    title: test/yosupo/vertex_add_path_sum.test.cpp
  - icon: ':x:'
    path: test/yosupo/vertex_add_subtree_sum.test.cpp
    title: test/yosupo/vertex_add_subtree_sum.test.cpp
  - icon: ':x:'
    path: test/yosupo/vertex_set_path_composite.test.cpp
    title: test/yosupo/vertex_set_path_composite.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':question:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/hld.hpp\"\n#include <cstddef>\n#include <vector>\n\
    \nstruct Hld {\n  int r,n;\n  std::vector<std::vector<int>> edge;\n  std::vector<int>\
    \ size, in, out, head, rev, par, depth, clen;\n private:\n  void dfs_sz(int v,\
    \ int p, int d) {\n    par[v] = p;\n    size[v] = 1;\n    if (!edge[v].empty()\
    \ and edge[v][0] == p)\n      std::swap(edge[v][0], edge[v].back());\n    for\
    \ (auto& t:edge[v]) {\n      if (t == p) continue;\n      dfs_sz(t, v, d+1);\n\
    \      size[v] += size[t];\n      if (size[edge[v][0]] < size[t])\n        std::swap(edge[v][0],\
    \ t);\n    }\n  }\n  void dfs_hld(int v, int p, int& times) {\n    in[v] = times++;\n\
    \    rev[in[v]] = v;\n    clen[v] = 1;\n    if (!edge[v].empty() and edge[v][0]\
    \ != p) {\n      int t = edge[v][0];\n      head[t] = head[v];\n      depth[t]\
    \ = depth[v];\n      dfs_hld(t, v, times);\n      clen[v] += clen[t];\n    }\n\
    \    for (size_t i = 1; i < edge[v].size(); i++) {\n      int t = edge[v][i];\n\
    \      if (t == p) continue;\n      head[t] = t;\n      depth[t] = depth[v] +\
    \ 1;\n      dfs_hld(t, v, times);\n    }\n    out[v] = times;\n  }\n\n public:\n\
    \  Hld(int n) : r(0), n(n), edge(n), size(n), in(n, -1), out(n, -1), head(n, -1),\
    \ rev(n, -1), par(n, -1), depth(n, -1), clen(n) {}\n\n  inline void add_edge(int\
    \ a, int b) {\n    edge[a].push_back(b);\n    edge[b].push_back(a);\n  }\n\n \
    \ void build(int root = 0) {\n    r = root;\n    dfs_sz(root, -1, 0);\n    int\
    \ t = 0;\n    head[root] = root;\n    depth[root] = 0;\n    dfs_hld(root, -1,\
    \ t);\n  }\n\n  inline int lca(int a, int b) const {\n    if (depth[a] > depth[b])\
    \ std::swap(a, b);\n    while (depth[a] < depth[b]) {\n      b = par[head[b]];\n\
    \    }\n    while (head[a] != head[b]) {\n      a = par[head[a]];\n      b = par[head[b]];\n\
    \    }\n    return in[a] < in[b] ? a : b;\n  }\n\n private:\n  template<class\
    \ Query, class ReverseQuery>\n  auto _query(int u, int v, Query Q, ReverseQuery\
    \ RQ, bool include_lca) const -> decltype(Q(0,0)) {\n    using T = decltype(Q(0,0));\n\
    \    T um, vm;\n    auto u_up = [&]() {\n      um = um * (T)RQ(in[head[u]], in[u]+1);\n\
    \      u = par[head[u]];\n    };\n    auto v_up = [&]() {\n      vm = (T)Q(in[head[v]],\
    \ in[v]+1) * vm;\n      v = par[head[v]];\n    };\n    while (depth[u] > depth[v])\n\
    \      u_up();\n    while (depth[u] < depth[v])\n      v_up();\n    while (head[u]\
    \ != head[v]) {\n      u_up();\n      v_up();\n    }\n    if (in[u] < in[v]) {\n\
    \      int l = include_lca ? in[u] : in[u]+1;\n      return um * (T)Q(l, in[v]+1)\
    \ * vm;\n    } else {\n      int l = include_lca ? in[v] : in[v]+1;\n      return\
    \ um * (T)RQ(l, in[u]+1) * vm;\n    }\n  }\n\n public:\n  template<class Query,\
    \ class ReverseQuery>\n  auto query(int u, int v, Query Q, ReverseQuery RQ, bool\
    \ include_lca = true) const -> decltype(Q(0,0)) {\n    return _query(u, v, Q,\
    \ RQ, include_lca);\n  }\n\n  /// Query for commutative monoid\n  template<class\
    \ Query>\n  auto query(int u, int v, Query Q, bool include_lca = true) const ->\
    \ decltype(Q(0,0)) {\n    return _query(u, v, Q, Q, include_lca);\n  }\n\n  template<class\
    \ Set, class T>\n  void set(int i, Set S, T&& val) const {\n    S(in[i], std::forward<T>(val));\n\
    \  }\n\n  template<typename Upd, typename T>\n  void update(int u, int v, Upd\
    \ U, const T& val, bool include_lca = true) const {\n    if (depth[u] > depth[v])\
    \ std::swap(u,v);\n    auto up = [&](int& v) {\n      U(in[head[v]], in[v]+1,\
    \ val);\n      v = par[head[v]];\n    };\n    while (depth[u] < depth[v]) {\n\
    \      up(v);\n    }\n    while (head[u] != head[v]) {\n      up(u);\n      up(v);\n\
    \    }\n    if (in[u] > in[v]) std::swap(u,v);\n    int l = include_lca ? in[u]\
    \ : in[u]+1;\n    U(l, in[v]+1, val);\n  }\n\npublic:\n  template<class Add, class\
    \ Sum>\n  void subtree_build(Add A, Sum S) const {\n    dfs_subtree_build(A, S,\
    \ r);\n  }\n private:\n  template<class Add, class Sum>\n  void dfs_subtree_build(Add\
    \ A, Sum S, int u) const {\n    for (size_t i = 0; i < edge[u].size(); i++) {\n\
    \      auto v = edge[u][i];\n      if (v == par[u]) continue;\n      dfs_subtree_build(A,\
    \ S, v);\n      if (i > 0)\n        A(in[u], S(in[v], in[v]+clen[v]));\n    }\n\
    \  }\n public:\n  template<class T, class Sum>\n  T subtree_sum(int r, Sum S)\
    \ const {\n    return (T)S(in[r], in[r]+clen[r]);\n  }\n  template<class T, class\
    \ Add>\n  void subtree_point_add(int u, Add A, const T& val) const {\n    while\
    \ (u != -1) {\n      A(in[u], val);\n      u = par[head[u]];\n    }\n  }\n};\n"
  code: "#pragma once\n#include <cstddef>\n#include <vector>\n\nstruct Hld {\n  int\
    \ r,n;\n  std::vector<std::vector<int>> edge;\n  std::vector<int> size, in, out,\
    \ head, rev, par, depth, clen;\n private:\n  void dfs_sz(int v, int p, int d)\
    \ {\n    par[v] = p;\n    size[v] = 1;\n    if (!edge[v].empty() and edge[v][0]\
    \ == p)\n      std::swap(edge[v][0], edge[v].back());\n    for (auto& t:edge[v])\
    \ {\n      if (t == p) continue;\n      dfs_sz(t, v, d+1);\n      size[v] += size[t];\n\
    \      if (size[edge[v][0]] < size[t])\n        std::swap(edge[v][0], t);\n  \
    \  }\n  }\n  void dfs_hld(int v, int p, int& times) {\n    in[v] = times++;\n\
    \    rev[in[v]] = v;\n    clen[v] = 1;\n    if (!edge[v].empty() and edge[v][0]\
    \ != p) {\n      int t = edge[v][0];\n      head[t] = head[v];\n      depth[t]\
    \ = depth[v];\n      dfs_hld(t, v, times);\n      clen[v] += clen[t];\n    }\n\
    \    for (size_t i = 1; i < edge[v].size(); i++) {\n      int t = edge[v][i];\n\
    \      if (t == p) continue;\n      head[t] = t;\n      depth[t] = depth[v] +\
    \ 1;\n      dfs_hld(t, v, times);\n    }\n    out[v] = times;\n  }\n\n public:\n\
    \  Hld(int n) : r(0), n(n), edge(n), size(n), in(n, -1), out(n, -1), head(n, -1),\
    \ rev(n, -1), par(n, -1), depth(n, -1), clen(n) {}\n\n  inline void add_edge(int\
    \ a, int b) {\n    edge[a].push_back(b);\n    edge[b].push_back(a);\n  }\n\n \
    \ void build(int root = 0) {\n    r = root;\n    dfs_sz(root, -1, 0);\n    int\
    \ t = 0;\n    head[root] = root;\n    depth[root] = 0;\n    dfs_hld(root, -1,\
    \ t);\n  }\n\n  inline int lca(int a, int b) const {\n    if (depth[a] > depth[b])\
    \ std::swap(a, b);\n    while (depth[a] < depth[b]) {\n      b = par[head[b]];\n\
    \    }\n    while (head[a] != head[b]) {\n      a = par[head[a]];\n      b = par[head[b]];\n\
    \    }\n    return in[a] < in[b] ? a : b;\n  }\n\n private:\n  template<class\
    \ Query, class ReverseQuery>\n  auto _query(int u, int v, Query Q, ReverseQuery\
    \ RQ, bool include_lca) const -> decltype(Q(0,0)) {\n    using T = decltype(Q(0,0));\n\
    \    T um, vm;\n    auto u_up = [&]() {\n      um = um * (T)RQ(in[head[u]], in[u]+1);\n\
    \      u = par[head[u]];\n    };\n    auto v_up = [&]() {\n      vm = (T)Q(in[head[v]],\
    \ in[v]+1) * vm;\n      v = par[head[v]];\n    };\n    while (depth[u] > depth[v])\n\
    \      u_up();\n    while (depth[u] < depth[v])\n      v_up();\n    while (head[u]\
    \ != head[v]) {\n      u_up();\n      v_up();\n    }\n    if (in[u] < in[v]) {\n\
    \      int l = include_lca ? in[u] : in[u]+1;\n      return um * (T)Q(l, in[v]+1)\
    \ * vm;\n    } else {\n      int l = include_lca ? in[v] : in[v]+1;\n      return\
    \ um * (T)RQ(l, in[u]+1) * vm;\n    }\n  }\n\n public:\n  template<class Query,\
    \ class ReverseQuery>\n  auto query(int u, int v, Query Q, ReverseQuery RQ, bool\
    \ include_lca = true) const -> decltype(Q(0,0)) {\n    return _query(u, v, Q,\
    \ RQ, include_lca);\n  }\n\n  /// Query for commutative monoid\n  template<class\
    \ Query>\n  auto query(int u, int v, Query Q, bool include_lca = true) const ->\
    \ decltype(Q(0,0)) {\n    return _query(u, v, Q, Q, include_lca);\n  }\n\n  template<class\
    \ Set, class T>\n  void set(int i, Set S, T&& val) const {\n    S(in[i], std::forward<T>(val));\n\
    \  }\n\n  template<typename Upd, typename T>\n  void update(int u, int v, Upd\
    \ U, const T& val, bool include_lca = true) const {\n    if (depth[u] > depth[v])\
    \ std::swap(u,v);\n    auto up = [&](int& v) {\n      U(in[head[v]], in[v]+1,\
    \ val);\n      v = par[head[v]];\n    };\n    while (depth[u] < depth[v]) {\n\
    \      up(v);\n    }\n    while (head[u] != head[v]) {\n      up(u);\n      up(v);\n\
    \    }\n    if (in[u] > in[v]) std::swap(u,v);\n    int l = include_lca ? in[u]\
    \ : in[u]+1;\n    U(l, in[v]+1, val);\n  }\n\npublic:\n  template<class Add, class\
    \ Sum>\n  void subtree_build(Add A, Sum S) const {\n    dfs_subtree_build(A, S,\
    \ r);\n  }\n private:\n  template<class Add, class Sum>\n  void dfs_subtree_build(Add\
    \ A, Sum S, int u) const {\n    for (size_t i = 0; i < edge[u].size(); i++) {\n\
    \      auto v = edge[u][i];\n      if (v == par[u]) continue;\n      dfs_subtree_build(A,\
    \ S, v);\n      if (i > 0)\n        A(in[u], S(in[v], in[v]+clen[v]));\n    }\n\
    \  }\n public:\n  template<class T, class Sum>\n  T subtree_sum(int r, Sum S)\
    \ const {\n    return (T)S(in[r], in[r]+clen[r]);\n  }\n  template<class T, class\
    \ Add>\n  void subtree_point_add(int u, Add A, const T& val) const {\n    while\
    \ (u != -1) {\n      A(in[u], val);\n      u = par[head[u]];\n    }\n  }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/hld.hpp
  requiredBy:
  - include/mtl/segment_hld.hpp
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_SOME_WA
  verifiedWith:
  - test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
  - test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp
  - test/yosupo/vertex_add_path_sum.test.cpp
  - test/yosupo/vertex_add_subtree_sum.test.cpp
  - test/yosupo/vertex_set_path_composite.test.cpp
documentation_of: include/mtl/hld.hpp
layout: document
redirect_from:
- /library/include/mtl/hld.hpp
- /library/include/mtl/hld.hpp.html
title: include/mtl/hld.hpp
---
