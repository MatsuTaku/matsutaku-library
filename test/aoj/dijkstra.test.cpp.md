---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_A
    links:
    - https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_A
  bundledCode: "#line 1 \"test/aoj/dijkstra.test.cpp\"\n#define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_A\"\
    \r\n#include <queue>\r\n#include <bits/stdc++.h>\r\nusing namespace std;\r\n\r\
    \nconstexpr int INF = 11e8;\r\n\r\nint main() {\r\n  int v,e,r; cin>>v>>e>>r;\r\
    \n  vector<pair<int,int>> G[v];\r\n  for (int i = 0; i < e; i++) {\r\n    int\
    \ s,t,d; cin>>s>>t>>d;\r\n    G[s].emplace_back(t, d);\r\n  }\r\n\r\n  vector<int>\
    \ dist(v, INF);\r\n  priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>>\
    \ qs;\r\n  dist[r] = 0;\r\n  qs.emplace(0, r);\r\n  while (!qs.empty()) {\r\n\
    \    auto [c, s] = qs.top(); qs.pop();\r\n    if (dist[s] < c) continue;\r\n \
    \   for (auto [t, d] : G[s]) {\r\n      if (dist[t] <= c + d) continue;\r\n  \
    \    dist[t] = c + d;\r\n      qs.emplace(c + d, t);\r\n    }\r\n  }\r\n  for\
    \ (int i = 0; i < v; i++) {\r\n    if (dist[i] != INF) {\r\n      cout << dist[i]\
    \ << endl;\r\n    } else {\r\n      cout << \"INF\" << endl;\r\n    }\r\n  }\r\
    \n\r\n}\r\n"
  code: "#define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_A\"\r\n\
    #include <queue>\r\n#include <bits/stdc++.h>\r\nusing namespace std;\r\n\r\nconstexpr\
    \ int INF = 11e8;\r\n\r\nint main() {\r\n  int v,e,r; cin>>v>>e>>r;\r\n  vector<pair<int,int>>\
    \ G[v];\r\n  for (int i = 0; i < e; i++) {\r\n    int s,t,d; cin>>s>>t>>d;\r\n\
    \    G[s].emplace_back(t, d);\r\n  }\r\n\r\n  vector<int> dist(v, INF);\r\n  priority_queue<pair<int,int>,\
    \ vector<pair<int,int>>, greater<>> qs;\r\n  dist[r] = 0;\r\n  qs.emplace(0, r);\r\
    \n  while (!qs.empty()) {\r\n    auto [c, s] = qs.top(); qs.pop();\r\n    if (dist[s]\
    \ < c) continue;\r\n    for (auto [t, d] : G[s]) {\r\n      if (dist[t] <= c +\
    \ d) continue;\r\n      dist[t] = c + d;\r\n      qs.emplace(c + d, t);\r\n  \
    \  }\r\n  }\r\n  for (int i = 0; i < v; i++) {\r\n    if (dist[i] != INF) {\r\n\
    \      cout << dist[i] << endl;\r\n    } else {\r\n      cout << \"INF\" << endl;\r\
    \n    }\r\n  }\r\n\r\n}\r\n"
  dependsOn: []
  isVerificationFile: true
  path: test/aoj/dijkstra.test.cpp
  requiredBy: []
  timestamp: '2023-04-08 02:15:04+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/aoj/dijkstra.test.cpp
layout: document
redirect_from:
- /verify/test/aoj/dijkstra.test.cpp
- /verify/test/aoj/dijkstra.test.cpp.html
title: test/aoj/dijkstra.test.cpp
---
