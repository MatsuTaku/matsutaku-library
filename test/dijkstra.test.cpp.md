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
  bundledCode: "#line 1 \"test/dijkstra.test.cpp\"\n#define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_A\"\
    \n\n#include <bits/stdc++.h>\nusing namespace std;\n\nconstexpr int INF = 11e8;\n\
    \nint main() {\n  int v,e,r; cin>>v>>e>>r;\n  vector<pair<int,int>> G[v];\n  for\
    \ (int i = 0; i < e; i++) {\n    int s,t,d; cin>>s>>t>>d;\n    G[s].emplace_back(t,\
    \ d);\n  }\n\n  vector<int> dist(v, INF);\n  priority_queue<pair<int,int>, vector<pair<int,int>>,\
    \ greater<>> qs;\n  dist[r] = 0;\n  qs.emplace(0, r);\n  while (!qs.empty()) {\n\
    \    auto [c, s] = qs.top(); qs.pop();\n    if (dist[s] < c) continue;\n    for\
    \ (auto [t, d] : G[s]) {\n      if (dist[t] <= c + d) continue;\n      dist[t]\
    \ = c + d;\n      qs.emplace(c + d, t);\n    }\n  }\n  for (int i = 0; i < v;\
    \ i++) {\n    if (dist[i] != INF) {\n      cout << dist[i] << endl;\n    } else\
    \ {\n      cout << \"INF\" << endl;\n    }\n  }\n\n}\n"
  code: "#define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_A\"\n\n\
    #include <bits/stdc++.h>\nusing namespace std;\n\nconstexpr int INF = 11e8;\n\n\
    int main() {\n  int v,e,r; cin>>v>>e>>r;\n  vector<pair<int,int>> G[v];\n  for\
    \ (int i = 0; i < e; i++) {\n    int s,t,d; cin>>s>>t>>d;\n    G[s].emplace_back(t,\
    \ d);\n  }\n\n  vector<int> dist(v, INF);\n  priority_queue<pair<int,int>, vector<pair<int,int>>,\
    \ greater<>> qs;\n  dist[r] = 0;\n  qs.emplace(0, r);\n  while (!qs.empty()) {\n\
    \    auto [c, s] = qs.top(); qs.pop();\n    if (dist[s] < c) continue;\n    for\
    \ (auto [t, d] : G[s]) {\n      if (dist[t] <= c + d) continue;\n      dist[t]\
    \ = c + d;\n      qs.emplace(c + d, t);\n    }\n  }\n  for (int i = 0; i < v;\
    \ i++) {\n    if (dist[i] != INF) {\n      cout << dist[i] << endl;\n    } else\
    \ {\n      cout << \"INF\" << endl;\n    }\n  }\n\n}\n"
  dependsOn: []
  isVerificationFile: true
  path: test/dijkstra.test.cpp
  requiredBy: []
  timestamp: '2022-11-27 14:51:20+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/dijkstra.test.cpp
layout: document
redirect_from:
- /verify/test/dijkstra.test.cpp
- /verify/test/dijkstra.test.cpp.html
title: test/dijkstra.test.cpp
---
