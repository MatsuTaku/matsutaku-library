---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/max_flow.hpp
    title: include/mtl/max_flow.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_A
    links:
    - https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_A
  bundledCode: "#line 1 \"test/aoj/maximum_flow_dinic.test.cpp\"\n#define PROBLEM\
    \ \"https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_A\"\r\n#line 2 \"include/mtl/max_flow.hpp\"\
    \n#include <limits>\r\n#include <algorithm>\r\n#include <vector>\r\n#include <queue>\r\
    \n#include <cassert>\r\n#include <iostream>\r\n\r\nstruct MaxFlowAlgorithm {\r\
    \n  virtual void add_edge(int u, int v, long long c) = 0;\r\n  virtual long long\
    \ flow(int s, int t, long long fup) = 0;\r\n};\r\n\r\nstruct FlowPath {\r\n  int\
    \ t;\r\n  long long cap;\r\n  int rev;\r\n  FlowPath(int t, long long flow, int\
    \ rev) :\r\n      t(t), cap(flow), rev(rev) {}\r\n};\r\n\r\nstruct FordFulkerson\
    \ : MaxFlowAlgorithm {\r\n  std::vector<std::vector<FlowPath>> g;\r\n  std::vector<bool>\
    \ visited;\r\n  explicit FordFulkerson(int n) : g(n), visited(n) {}\r\n\r\n  void\
    \ add_edge(int a, int b, long long f) override {\r\n    g[a].emplace_back(b, f,\
    \ g[b].size());\r\n    g[b].emplace_back(a, 0, g[a].size()-1);\r\n  }\r\n  long\
    \ long dfs(int t, int u, long long f) {\r\n    if (visited[u])\r\n      return\
    \ 0;\r\n    visited[u] = true;\r\n    if (u == t)\r\n      return f;\r\n    for\
    \ (auto& e : g[u]) {\r\n      if (e.cap == 0) continue;\r\n      auto flow = dfs(t,\
    \ e.t, std::min(f, e.cap));\r\n      if (flow) {\r\n        e.cap -= flow;\r\n\
    \        g[e.t][e.rev].cap += flow;\r\n        return flow;\r\n      }\r\n   \
    \ }\r\n    return 0;\r\n  }\r\n  long long flow(int s, int t, long long fup) override\
    \ {\r\n    long long flow = 0;\r\n    while (fup > 0) {\r\n      visited.assign(g.size(),\
    \ false);\r\n      auto f = dfs(t, s, std::numeric_limits<long long>::max());\r\
    \n      if (f > 0) {\r\n        flow += f;\r\n        fup -= f;\r\n      } else\
    \ {\r\n        break;\r\n      }\r\n    }\r\n    return flow;\r\n  }\r\n  long\
    \ long flow(int s, int t) {\r\n    return flow(s, t, std::numeric_limits<long\
    \ long>::max());\r\n  }\r\n};\r\n\r\nstruct Dinic : MaxFlowAlgorithm {\r\n  std::vector<std::vector<FlowPath>>\
    \ g;\r\n  std::vector<std::vector<FlowPath>::iterator> nxt;\r\n  std::vector<int>\
    \ dist;\r\n  explicit Dinic(int n) : g(n), nxt(n), dist(n) {}\r\n\r\n  void add_edge(int\
    \ a, int b, long long f) override {\r\n    g[a].emplace_back(b, f, g[b].size());\r\
    \n    g[b].emplace_back(a, 0, g[a].size()-1);\r\n  }\r\n  long long dfs(int t,\
    \ int u, long long f) {\r\n    if (u == t) {\r\n      return f;\r\n    }\r\n \
    \   long long fsum = 0;\r\n    for (auto& it = nxt[u]; it != g[u].end() and fsum\
    \ < f; ++it) {\r\n      if (it->cap == 0 or dist[it->t] != dist[u]+1)\r\n    \
    \    continue;\r\n      auto flow = dfs(t, it->t, std::min(f - fsum, it->cap));\r\
    \n      if (flow) {\r\n        it->cap -= flow;\r\n        g[it->t][it->rev].cap\
    \ += flow;\r\n        fsum += flow;\r\n      }\r\n    }\r\n    return fsum;\r\n\
    \  }\r\n  long long flow(int s, int t, long long fup) override {\r\n    long long\
    \ flow = 0;\r\n    std::queue<int> qs;\r\n    while (fup) {\r\n      // dual step\r\
    \n      std::transform(g.begin(), g.end(), nxt.begin(), [](auto& e) {\r\n    \
    \    return e.begin();\r\n      });\r\n      dist.assign(g.size(), std::numeric_limits<int>::max());\r\
    \n      qs.push(s);\r\n      dist[s] = 0;\r\n      while (!qs.empty()) {\r\n \
    \       auto u = qs.front(); qs.pop();\r\n        for (auto e : g[u]) {\r\n  \
    \        if (e.cap == 0 or dist[e.t] <= dist[u]+1)\r\n            continue;\r\n\
    \          dist[e.t] = dist[u]+1;\r\n          if (e.t != t)\r\n            qs.push(e.t);\r\
    \n        }\r\n      }\r\n      // primal step\r\n      auto f = dfs(t, s, std::numeric_limits<long\
    \ long>::max());\r\n      if (f > 0) {\r\n        flow += f;\r\n        fup -=\
    \ f;\r\n      } else {\r\n        break;\r\n      }\r\n    }\r\n    return flow;\r\
    \n  }\r\n  long long flow(int s, int t) {\r\n    return flow(s, t, std::numeric_limits<long\
    \ long>::max());\r\n  }\r\n};\r\n\r\nusing MaxFlowGraph = Dinic;\r\n\r\nstruct\
    \ Matching {\r\n  MaxFlowGraph G;\r\n  int n,m,s,g;\r\n  explicit Matching(int\
    \ n, int m) : G(n+m+2), n(n), m(m), s(n+m), g(s+1) {\r\n    for (int i = 0; i\
    \ < n; i++)\r\n      G.add_edge(s, i, 1);\r\n    for (int i = 0; i < m; i++)\r\
    \n      G.add_edge(i+n, g, 1);\r\n  }\r\n  void add_edge(int a, int b) {\r\n \
    \   G.add_edge(a, b+n, 1);\r\n  }\r\n  int flow() {\r\n    return G.flow(s, g);\r\
    \n  }\r\n  std::vector<std::pair<int,int>> matches() const {\r\n    std::vector<std::pair<int,int>>\
    \ ret;\r\n    for (int i = 0; i < n; i++) {\r\n      for (auto& e : G.g[i]) {\r\
    \n        if (e.t >= n and e.t < s and e.cap == 0) {\r\n          ret.emplace_back(i,\
    \ e.t-n);\r\n        }\r\n      }\r\n    }\r\n    return ret;\r\n  }\r\n};\r\n\
    \r\n// Project Selection Problem\r\ntemplate<typename P>\r\nstruct PSP {\r\n \
    \ MaxFlowGraph G;\r\n  int n,s,g;\r\n  P psum;\r\n  explicit PSP(int n) : G(n+2),\
    \ n(n), s(n), g(s+1), psum(0) {};\r\n  void add_profit(int a, P profit) {\r\n\
    \    if (profit > 0)\r\n      psum += profit;\r\n    if (profit > 0)\r\n     \
    \ G.add_edge(a, g, profit);\r\n    else if (profit < 0)\r\n      G.add_edge(s,\
    \ a, -profit);\r\n  }\r\n  void add_loss(int a, int b, P loss) {\r\n    G.add_edge(a,\
    \ b, loss);\r\n  }\r\n  P solve() {\r\n    auto p = G.flow(s, g);\r\n    return\
    \ psum - p;\r\n  }\r\n  std::vector<bool> selections() const {\r\n    std::vector<bool>\
    \ sel(n, 1);\r\n    for (auto& e : G.g[g]) {\r\n      if (G.g[e.t][e.rev].cap\
    \ > 0) {\r\n        sel[e.t] = 0;\r\n      }\r\n    }\r\n    return sel;\r\n \
    \ }\r\n};\r\n#line 3 \"test/aoj/maximum_flow_dinic.test.cpp\"\n#include <bits/stdc++.h>\r\
    \nusing namespace std;\r\n\r\nint main() {\r\n  int V,E; cin>>V>>E;\r\n  Dinic\
    \ G(V);\r\n  for (int i = 0; i < E; i++) {\r\n    int u,v,c; cin>>u>>v>>c;\r\n\
    \    G.add_edge(u, v, c);\r\n  }\r\n  auto f = G.flow(0,V-1);\r\n  cout << f <<\
    \ endl;\r\n}\n"
  code: "#define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_A\"\r\n\
    #include \"../../include/mtl/max_flow.hpp\"\r\n#include <bits/stdc++.h>\r\nusing\
    \ namespace std;\r\n\r\nint main() {\r\n  int V,E; cin>>V>>E;\r\n  Dinic G(V);\r\
    \n  for (int i = 0; i < E; i++) {\r\n    int u,v,c; cin>>u>>v>>c;\r\n    G.add_edge(u,\
    \ v, c);\r\n  }\r\n  auto f = G.flow(0,V-1);\r\n  cout << f << endl;\r\n}"
  dependsOn:
  - include/mtl/max_flow.hpp
  isVerificationFile: true
  path: test/aoj/maximum_flow_dinic.test.cpp
  requiredBy: []
  timestamp: '2023-04-08 02:15:04+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/aoj/maximum_flow_dinic.test.cpp
layout: document
redirect_from:
- /verify/test/aoj/maximum_flow_dinic.test.cpp
- /verify/test/aoj/maximum_flow_dinic.test.cpp.html
title: test/aoj/maximum_flow_dinic.test.cpp
---
