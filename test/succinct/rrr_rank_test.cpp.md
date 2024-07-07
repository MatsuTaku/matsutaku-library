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
  bundledCode: "#line 1 \"test/succinct/rrr_rank_test.cpp\"\n#include <succinct/rrr.hpp>\r\
    \n#include <bits/stdc++.h>\r\n\r\nstatic constexpr int u = 1e9;\r\nstatic constexpr\
    \ int n = 1e6;\r\n\r\nint main() {\r\n  int c = 0;\r\n  std::vector<int> select(n);\r\
    \n  std::map<int,int> _rank;\r\n  auto rank = [&](int i) { return _rank.lower_bound(i)->second;\
    \ };\r\n  auto pop_rate = u/n;\r\n  for (int i = 0; i < u; i++) {\r\n    if (rand()%pop_rate\
    \ == 0) {\r\n      _rank.emplace_hint(_rank.end(), i, c);\r\n      select[c] =\
    \ i;\r\n      ++c;\r\n    }\r\n  }\r\n  _rank[u] = c;\r\n\r\n  RRR<> bm;\r\n \
    \ for (auto [i,_]:_rank)\r\n    bm.set(i, 1);\r\n  bm.build();\r\n  typename RankSelectTraits<decltype(bm)>::rank_select_type\
    \ rs(&bm);\r\n  // rank1\r\n  size_t i = 0;\r\n  size_t r = 0;\r\n  _rank.emplace(u,\
    \ c);\r\n  for (auto [j,_]:_rank) {\r\n    while (i <= j) {\r\n      auto v =\
    \ rs.rank1(i);\r\n      if (v != r) {\r\n        std::cout << \"Failed rank1:\
    \ \" << i << \" rs.rank1 \" << v << \" != rank1 \" << r << std::endl;\r\n    \
    \    return 1;\r\n      }\r\n      ++i;\r\n    }\r\n    ++r;\r\n  }\r\n  // rank0;\r\
    \n  i = 0;\r\n  r = 0;\r\n  for (auto [j,_]:_rank) {\r\n    while (i <= j) {\r\
    \n      auto v = rs.rank0(i);\r\n      if (v != i-r) {\r\n        std::cout <<\
    \ \"Failed rank0: \" << i << \" rs.rank0 \" << v << \" != rank0 \" << i-r << std::endl;\r\
    \n        return 1;\r\n      }\r\n      ++i;\r\n    }\r\n    ++r;\r\n  }\r\n \
    \ std::cout << \"OK\" << std::endl;\r\n\r\n}\r\n"
  code: "#include <succinct/rrr.hpp>\r\n#include <bits/stdc++.h>\r\n\r\nstatic constexpr\
    \ int u = 1e9;\r\nstatic constexpr int n = 1e6;\r\n\r\nint main() {\r\n  int c\
    \ = 0;\r\n  std::vector<int> select(n);\r\n  std::map<int,int> _rank;\r\n  auto\
    \ rank = [&](int i) { return _rank.lower_bound(i)->second; };\r\n  auto pop_rate\
    \ = u/n;\r\n  for (int i = 0; i < u; i++) {\r\n    if (rand()%pop_rate == 0) {\r\
    \n      _rank.emplace_hint(_rank.end(), i, c);\r\n      select[c] = i;\r\n   \
    \   ++c;\r\n    }\r\n  }\r\n  _rank[u] = c;\r\n\r\n  RRR<> bm;\r\n  for (auto\
    \ [i,_]:_rank)\r\n    bm.set(i, 1);\r\n  bm.build();\r\n  typename RankSelectTraits<decltype(bm)>::rank_select_type\
    \ rs(&bm);\r\n  // rank1\r\n  size_t i = 0;\r\n  size_t r = 0;\r\n  _rank.emplace(u,\
    \ c);\r\n  for (auto [j,_]:_rank) {\r\n    while (i <= j) {\r\n      auto v =\
    \ rs.rank1(i);\r\n      if (v != r) {\r\n        std::cout << \"Failed rank1:\
    \ \" << i << \" rs.rank1 \" << v << \" != rank1 \" << r << std::endl;\r\n    \
    \    return 1;\r\n      }\r\n      ++i;\r\n    }\r\n    ++r;\r\n  }\r\n  // rank0;\r\
    \n  i = 0;\r\n  r = 0;\r\n  for (auto [j,_]:_rank) {\r\n    while (i <= j) {\r\
    \n      auto v = rs.rank0(i);\r\n      if (v != i-r) {\r\n        std::cout <<\
    \ \"Failed rank0: \" << i << \" rs.rank0 \" << v << \" != rank0 \" << i-r << std::endl;\r\
    \n        return 1;\r\n      }\r\n      ++i;\r\n    }\r\n    ++r;\r\n  }\r\n \
    \ std::cout << \"OK\" << std::endl;\r\n\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/succinct/rrr_rank_test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/succinct/rrr_rank_test.cpp
layout: document
redirect_from:
- /library/test/succinct/rrr_rank_test.cpp
- /library/test/succinct/rrr_rank_test.cpp.html
title: test/succinct/rrr_rank_test.cpp
---
