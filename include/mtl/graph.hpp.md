---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':warning:'
    path: include/mtl/dijkstra.hpp
    title: include/mtl/dijkstra.hpp
  - icon: ':warning:'
    path: include/mtl/rerooting.hpp
    title: include/mtl/rerooting.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/scc.hpp
    title: include/mtl/scc.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/aoj/scc.test.cpp
    title: test/aoj/scc.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/graph.hpp\"\n#include <queue>\r\n#include <vector>\r\
    \n\r\nstruct Graph {\r\n  int n;\r\n  std::vector<std::vector<int>> g;\r\n  std::vector<int>\
    \ in;\r\n  std::vector<int> tour, L, R; // For Euler tour.\r\n\r\n  Graph(int\
    \ n) : n(n), g(n), in(n) {}\r\n\r\n  void add_edge_dir(int a, int b) {\r\n   \
    \ g[a].push_back(b);\r\n    in[b]++;\r\n  }\r\n  void add_edge(int a, int b) {\r\
    \n    add_edge_dir(a, b);\r\n    add_edge_dir(b, a);\r\n  }\r\n\r\n  template\
    \ <typename T> void bfs(int s, std::vector<T> &dist, T INF) const;\r\n  template\
    \ <typename T> bool topological_sort(std::vector<T> &order) const;\r\n  void euler_tour(int\
    \ root);\r\n};\r\n\r\ntemplate <typename T>\r\nvoid Graph::bfs(int s, std::vector<T>\
    \ &dist, T INF) const {\r\n  dist.assign(n, INF);\r\n  std::queue<int> qs;\r\n\
    \  dist[s] = 0;\r\n  qs.push(s);\r\n  while (!qs.empty()) {\r\n    auto c = qs.front();\r\
    \n    qs.pop();\r\n    auto d = dist[c];\r\n    for (auto t : g[c]) {\r\n    \
    \  if (dist[t] <= d + 1)\r\n        continue;\r\n      dist[t] = d + 1;\r\n  \
    \    qs.push(t);\r\n    }\r\n  }\r\n}\r\n\r\ntemplate <typename T>\r\nbool Graph::topological_sort(std::vector<T>\
    \ &order) const {\r\n  order.resize(0);\r\n  order.reserve(n);\r\n  std::vector<int>\
    \ incnt(n);\r\n  auto dfs = [&](auto dfs, int s) -> void {\r\n    order.push_back(s);\r\
    \n    for (auto t : g[s]) {\r\n      if (++incnt[t] == in[t]) {\r\n        dfs(dfs,\
    \ t);\r\n      }\r\n    }\r\n  };\r\n  for (int i = 0; i < n; i++) {\r\n    if\
    \ (in[i] == 0)\r\n      dfs(dfs, i);\r\n  }\r\n  return (int)order.size() == n;\r\
    \n}\r\n\r\nvoid Graph::euler_tour(int root = 0) {\r\n  tour.clear();\r\n  L.resize(n);\r\
    \n  R.resize(n);\r\n  auto dfs = [&](auto &f, int s, int p, int k) -> int {\r\n\
    \    tour.push_back(s);\r\n    L[s] = k;\r\n    R[s] = k++;\r\n    for (int t\
    \ : g[s]) {\r\n      if (t == p)\r\n        continue;\r\n      k = f(f, t, s,\
    \ k);\r\n      tour.push_back(s);\r\n      R[s] = k++;\r\n    }\r\n    return\
    \ k;\r\n  };\r\n  dfs(dfs, root, -1, 0);\r\n}\r\n\r\ntemplate <typename W> struct\
    \ Network {\r\n  using weight_type = W;\r\n  int n;\r\n  struct Path {\r\n   \
    \ int t, w;\r\n    Path(int t, W w) : t(t), w(w) {}\r\n  };\r\n  std::vector<std::vector<Path>>\
    \ g;\r\n  std::vector<int> in;\r\n\r\n  Network(int n) : n(n), g(n), in(n) {}\r\
    \n\r\n  void add_edge_dir(int a, int b, W c) {\r\n    g[a].emplace_back(b, c);\r\
    \n    in[b]++;\r\n  }\r\n  void add_edge(int a, int b, W c) {\r\n    add_edge_dir(a,\
    \ b, c);\r\n    add_edge_dir(b, a, c);\r\n  }\r\n\r\n  template <typename T>\r\
    \n  void dijkstra_search(int s, std::vector<T> &dist, T INF) const;\r\n};\r\n"
  code: "#pragma once\r\n#include <queue>\r\n#include <vector>\r\n\r\nstruct Graph\
    \ {\r\n  int n;\r\n  std::vector<std::vector<int>> g;\r\n  std::vector<int> in;\r\
    \n  std::vector<int> tour, L, R; // For Euler tour.\r\n\r\n  Graph(int n) : n(n),\
    \ g(n), in(n) {}\r\n\r\n  void add_edge_dir(int a, int b) {\r\n    g[a].push_back(b);\r\
    \n    in[b]++;\r\n  }\r\n  void add_edge(int a, int b) {\r\n    add_edge_dir(a,\
    \ b);\r\n    add_edge_dir(b, a);\r\n  }\r\n\r\n  template <typename T> void bfs(int\
    \ s, std::vector<T> &dist, T INF) const;\r\n  template <typename T> bool topological_sort(std::vector<T>\
    \ &order) const;\r\n  void euler_tour(int root);\r\n};\r\n\r\ntemplate <typename\
    \ T>\r\nvoid Graph::bfs(int s, std::vector<T> &dist, T INF) const {\r\n  dist.assign(n,\
    \ INF);\r\n  std::queue<int> qs;\r\n  dist[s] = 0;\r\n  qs.push(s);\r\n  while\
    \ (!qs.empty()) {\r\n    auto c = qs.front();\r\n    qs.pop();\r\n    auto d =\
    \ dist[c];\r\n    for (auto t : g[c]) {\r\n      if (dist[t] <= d + 1)\r\n   \
    \     continue;\r\n      dist[t] = d + 1;\r\n      qs.push(t);\r\n    }\r\n  }\r\
    \n}\r\n\r\ntemplate <typename T>\r\nbool Graph::topological_sort(std::vector<T>\
    \ &order) const {\r\n  order.resize(0);\r\n  order.reserve(n);\r\n  std::vector<int>\
    \ incnt(n);\r\n  auto dfs = [&](auto dfs, int s) -> void {\r\n    order.push_back(s);\r\
    \n    for (auto t : g[s]) {\r\n      if (++incnt[t] == in[t]) {\r\n        dfs(dfs,\
    \ t);\r\n      }\r\n    }\r\n  };\r\n  for (int i = 0; i < n; i++) {\r\n    if\
    \ (in[i] == 0)\r\n      dfs(dfs, i);\r\n  }\r\n  return (int)order.size() == n;\r\
    \n}\r\n\r\nvoid Graph::euler_tour(int root = 0) {\r\n  tour.clear();\r\n  L.resize(n);\r\
    \n  R.resize(n);\r\n  auto dfs = [&](auto &f, int s, int p, int k) -> int {\r\n\
    \    tour.push_back(s);\r\n    L[s] = k;\r\n    R[s] = k++;\r\n    for (int t\
    \ : g[s]) {\r\n      if (t == p)\r\n        continue;\r\n      k = f(f, t, s,\
    \ k);\r\n      tour.push_back(s);\r\n      R[s] = k++;\r\n    }\r\n    return\
    \ k;\r\n  };\r\n  dfs(dfs, root, -1, 0);\r\n}\r\n\r\ntemplate <typename W> struct\
    \ Network {\r\n  using weight_type = W;\r\n  int n;\r\n  struct Path {\r\n   \
    \ int t, w;\r\n    Path(int t, W w) : t(t), w(w) {}\r\n  };\r\n  std::vector<std::vector<Path>>\
    \ g;\r\n  std::vector<int> in;\r\n\r\n  Network(int n) : n(n), g(n), in(n) {}\r\
    \n\r\n  void add_edge_dir(int a, int b, W c) {\r\n    g[a].emplace_back(b, c);\r\
    \n    in[b]++;\r\n  }\r\n  void add_edge(int a, int b, W c) {\r\n    add_edge_dir(a,\
    \ b, c);\r\n    add_edge_dir(b, a, c);\r\n  }\r\n\r\n  template <typename T>\r\
    \n  void dijkstra_search(int s, std::vector<T> &dist, T INF) const;\r\n};\r\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/graph.hpp
  requiredBy:
  - include/mtl/dijkstra.hpp
  - include/mtl/rerooting.hpp
  - include/mtl/scc.hpp
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/aoj/scc.test.cpp
documentation_of: include/mtl/graph.hpp
layout: document
redirect_from:
- /library/include/mtl/graph.hpp
- /library/include/mtl/graph.hpp.html
title: include/mtl/graph.hpp
---
