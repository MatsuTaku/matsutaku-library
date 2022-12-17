---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/graph.hpp
    title: include/mtl/graph.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
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
    \n  void dijkstra_search(int s, std::vector<T> &dist, T INF) const;\r\n};\r\n\
    #line 4 \"include/mtl/rerooting.hpp\"\n\r\ntemplate <typename T, T (*F)(T, int),\
    \ T (*merge)(T, T), T (*G)(T, int),\r\n          T (*e)()>\r\nstruct ReRootingGraph\
    \ : public Graph {\r\n  std::vector<std::vector<T>> dp;\r\n  using Graph::g;\r\
    \n  using Graph::n;\r\n\r\nprivate:\r\n  T dfs(int s, int p) {\r\n    T m = e();\r\
    \n    for (int i = 0; i < (int)g[s].size(); i++) {\r\n      int t = g[s][i];\r\
    \n      if (t == p)\r\n        continue;\r\n      dp[s][i] = dfs(t, s);\r\n  \
    \    m = merge(m, F(dp[s][i], t));\r\n    }\r\n    return G(m, s);\r\n  }\r\n\
    \  void dfs2(int s, int p, T prop) {\r\n    for (int i = 0; i < (int)g[s].size();\
    \ i++) {\r\n      if (g[s][i] == p) {\r\n        dp[s][i] = prop;\r\n        break;\r\
    \n      }\r\n    }\r\n    std::vector<T> cR(g[s].size(), e());\r\n    for (int\
    \ i = (int)g[s].size() - 2; i >= 0; i--)\r\n      cR[i] = merge(cR[i + 1], F(dp[s][i\
    \ + 1], g[s][i + 1]));\r\n    T cL = e();\r\n    for (int i = 0; i < (int)g[s].size();\
    \ i++) {\r\n      if (g[s][i] != p) {\r\n        dfs2(g[s][i], s, G(merge(cL,\
    \ cR[i]), s));\r\n      }\r\n      cL = merge(cL, F(dp[s][i], g[s][i]));\r\n \
    \   }\r\n  }\r\n\r\npublic:\r\n  ReRootingGraph(int n) : Graph(n), dp(n) {}\r\n\
    \r\n  void calc(int root = 0) {\r\n    for (int i = 0; i < n; i++)\r\n      dp[i].assign(g[i].size(),\
    \ e());\r\n    dfs(root, -1);\r\n    dfs2(root, -1, e());\r\n  }\r\n};\r\n\r\n\
    template <typename W, typename T, T (*F)(T, int, W), T (*merge)(T, T),\r\n   \
    \       T (*G)(T, int), T (*e)()>\r\nstruct ReRootingNetwork : public Network<W>\
    \ {\r\n  using base = Network<W>;\r\n  std::vector<std::vector<T>> dp;\r\n  using\
    \ base::g;\r\n  using base::n;\r\n\r\nprivate:\r\n  T dfs(int s, int p) {\r\n\
    \    T m = e();\r\n    for (int i = 0; i < (int)g[s].size(); i++) {\r\n      auto\
    \ [t, w] = g[s][i];\r\n      if (t == p)\r\n        continue;\r\n      dp[s][i]\
    \ = dfs(t, s);\r\n      m = merge(m, F(dp[s][i], t, w));\r\n    }\r\n    return\
    \ G(m, s);\r\n  }\r\n  void dfs2(int s, int p, T prop) {\r\n    for (int i = 0;\
    \ i < (int)g[s].size(); i++) {\r\n      if (g[s][i].t == p) {\r\n        dp[s][i]\
    \ = prop;\r\n        break;\r\n      }\r\n    }\r\n    std::vector<T> cR(g[s].size(),\
    \ e());\r\n    for (int i = (int)g[s].size() - 2; i >= 0; i--) {\r\n      auto\
    \ [t, w] = g[s][i + 1];\r\n      cR[i] = merge(cR[i + 1], F(dp[s][i + 1], t, w));\r\
    \n    }\r\n    T cL = e();\r\n    for (int i = 0; i < (int)g[s].size(); i++) {\r\
    \n      auto [t, w] = g[s][i];\r\n      if (t != p) {\r\n        dfs2(t, s, G(merge(cL,\
    \ cR[i]), s));\r\n      }\r\n      cL = merge(cL, F(dp[s][i], t, w));\r\n    }\r\
    \n  }\r\n\r\npublic:\r\n  ReRootingNetwork(int n) : base(n), dp(n) {}\r\n\r\n\
    \  void calc(int root = 0) {\r\n    for (int i = 0; i < n; i++)\r\n      dp[i].assign(g[i].size(),\
    \ e());\r\n    dfs(root, -1);\r\n    dfs2(root, -1, e());\r\n  }\r\n};\n"
  code: "#pragma once\r\n#include \"graph.hpp\"\r\n#include <vector>\r\n\r\ntemplate\
    \ <typename T, T (*F)(T, int), T (*merge)(T, T), T (*G)(T, int),\r\n         \
    \ T (*e)()>\r\nstruct ReRootingGraph : public Graph {\r\n  std::vector<std::vector<T>>\
    \ dp;\r\n  using Graph::g;\r\n  using Graph::n;\r\n\r\nprivate:\r\n  T dfs(int\
    \ s, int p) {\r\n    T m = e();\r\n    for (int i = 0; i < (int)g[s].size(); i++)\
    \ {\r\n      int t = g[s][i];\r\n      if (t == p)\r\n        continue;\r\n  \
    \    dp[s][i] = dfs(t, s);\r\n      m = merge(m, F(dp[s][i], t));\r\n    }\r\n\
    \    return G(m, s);\r\n  }\r\n  void dfs2(int s, int p, T prop) {\r\n    for\
    \ (int i = 0; i < (int)g[s].size(); i++) {\r\n      if (g[s][i] == p) {\r\n  \
    \      dp[s][i] = prop;\r\n        break;\r\n      }\r\n    }\r\n    std::vector<T>\
    \ cR(g[s].size(), e());\r\n    for (int i = (int)g[s].size() - 2; i >= 0; i--)\r\
    \n      cR[i] = merge(cR[i + 1], F(dp[s][i + 1], g[s][i + 1]));\r\n    T cL =\
    \ e();\r\n    for (int i = 0; i < (int)g[s].size(); i++) {\r\n      if (g[s][i]\
    \ != p) {\r\n        dfs2(g[s][i], s, G(merge(cL, cR[i]), s));\r\n      }\r\n\
    \      cL = merge(cL, F(dp[s][i], g[s][i]));\r\n    }\r\n  }\r\n\r\npublic:\r\n\
    \  ReRootingGraph(int n) : Graph(n), dp(n) {}\r\n\r\n  void calc(int root = 0)\
    \ {\r\n    for (int i = 0; i < n; i++)\r\n      dp[i].assign(g[i].size(), e());\r\
    \n    dfs(root, -1);\r\n    dfs2(root, -1, e());\r\n  }\r\n};\r\n\r\ntemplate\
    \ <typename W, typename T, T (*F)(T, int, W), T (*merge)(T, T),\r\n          T\
    \ (*G)(T, int), T (*e)()>\r\nstruct ReRootingNetwork : public Network<W> {\r\n\
    \  using base = Network<W>;\r\n  std::vector<std::vector<T>> dp;\r\n  using base::g;\r\
    \n  using base::n;\r\n\r\nprivate:\r\n  T dfs(int s, int p) {\r\n    T m = e();\r\
    \n    for (int i = 0; i < (int)g[s].size(); i++) {\r\n      auto [t, w] = g[s][i];\r\
    \n      if (t == p)\r\n        continue;\r\n      dp[s][i] = dfs(t, s);\r\n  \
    \    m = merge(m, F(dp[s][i], t, w));\r\n    }\r\n    return G(m, s);\r\n  }\r\
    \n  void dfs2(int s, int p, T prop) {\r\n    for (int i = 0; i < (int)g[s].size();\
    \ i++) {\r\n      if (g[s][i].t == p) {\r\n        dp[s][i] = prop;\r\n      \
    \  break;\r\n      }\r\n    }\r\n    std::vector<T> cR(g[s].size(), e());\r\n\
    \    for (int i = (int)g[s].size() - 2; i >= 0; i--) {\r\n      auto [t, w] =\
    \ g[s][i + 1];\r\n      cR[i] = merge(cR[i + 1], F(dp[s][i + 1], t, w));\r\n \
    \   }\r\n    T cL = e();\r\n    for (int i = 0; i < (int)g[s].size(); i++) {\r\
    \n      auto [t, w] = g[s][i];\r\n      if (t != p) {\r\n        dfs2(t, s, G(merge(cL,\
    \ cR[i]), s));\r\n      }\r\n      cL = merge(cL, F(dp[s][i], t, w));\r\n    }\r\
    \n  }\r\n\r\npublic:\r\n  ReRootingNetwork(int n) : base(n), dp(n) {}\r\n\r\n\
    \  void calc(int root = 0) {\r\n    for (int i = 0; i < n; i++)\r\n      dp[i].assign(g[i].size(),\
    \ e());\r\n    dfs(root, -1);\r\n    dfs2(root, -1, e());\r\n  }\r\n};"
  dependsOn:
  - include/mtl/graph.hpp
  isVerificationFile: false
  path: include/mtl/rerooting.hpp
  requiredBy: []
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/rerooting.hpp
layout: document
redirect_from:
- /library/include/mtl/rerooting.hpp
- /library/include/mtl/rerooting.hpp.html
title: include/mtl/rerooting.hpp
---
