---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 1 \"test/succinct/rrr_test.cpp\"\n#include <succinct/rrr.hpp>\r\
    \n#include <bits/stdc++.h>\r\n\r\nstatic constexpr int u = 1e9;\r\nstatic constexpr\
    \ int n = 1e6;\r\n\r\nint main() {\r\n  std::vector<bool> B(u);\r\n  int c = 0;\r\
    \n  std::vector<int> select(n);\r\n  std::map<int,int> _rank;\r\n  auto rank =\
    \ [&](int i) { return _rank.lower_bound(i)->second; };\r\n  auto pop_rate = u/n;\r\
    \n  for (int i = 0; i < u; i++) {\r\n    if (rand()%pop_rate == 0) {\r\n     \
    \ B[i] = 1;\r\n      _rank.emplace_hint(_rank.end(), i, c);\r\n      select[c]\
    \ = i;\r\n      ++c;\r\n    }\r\n  }\r\n  _rank[u] = c;\r\n\r\n  RRR<> bm;\r\n\
    \  for (auto [i,_]:_rank)\r\n    bm.set(i, 1);\r\n  bm.build();\r\n  // get\r\n\
    \  int k = 0;\r\n  for (int i = 0; i < u; i++) {\r\n    auto v = bm.get(i);\r\n\
    \    if (v != B[i]) {\r\n      std::cout << \"Failed get: \" << i << \" bm.get\
    \ \" << v << \" != B \" << B[i] << std::endl;\r\n      return 1;\r\n    }\r\n\
    \  }\r\n  std::cout << \"OK\" << std::endl;\r\n\r\n}\r\n"
  code: "#include <succinct/rrr.hpp>\r\n#include <bits/stdc++.h>\r\n\r\nstatic constexpr\
    \ int u = 1e9;\r\nstatic constexpr int n = 1e6;\r\n\r\nint main() {\r\n  std::vector<bool>\
    \ B(u);\r\n  int c = 0;\r\n  std::vector<int> select(n);\r\n  std::map<int,int>\
    \ _rank;\r\n  auto rank = [&](int i) { return _rank.lower_bound(i)->second; };\r\
    \n  auto pop_rate = u/n;\r\n  for (int i = 0; i < u; i++) {\r\n    if (rand()%pop_rate\
    \ == 0) {\r\n      B[i] = 1;\r\n      _rank.emplace_hint(_rank.end(), i, c);\r\
    \n      select[c] = i;\r\n      ++c;\r\n    }\r\n  }\r\n  _rank[u] = c;\r\n\r\n\
    \  RRR<> bm;\r\n  for (auto [i,_]:_rank)\r\n    bm.set(i, 1);\r\n  bm.build();\r\
    \n  // get\r\n  int k = 0;\r\n  for (int i = 0; i < u; i++) {\r\n    auto v =\
    \ bm.get(i);\r\n    if (v != B[i]) {\r\n      std::cout << \"Failed get: \" <<\
    \ i << \" bm.get \" << v << \" != B \" << B[i] << std::endl;\r\n      return 1;\r\
    \n    }\r\n  }\r\n  std::cout << \"OK\" << std::endl;\r\n\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/succinct/rrr_test.cpp
  requiredBy: []
  timestamp: '2023-04-10 20:47:47+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/succinct/rrr_test.cpp
layout: document
redirect_from:
- /library/test/succinct/rrr_test.cpp
- /library/test/succinct/rrr_test.cpp.html
title: test/succinct/rrr_test.cpp
---
