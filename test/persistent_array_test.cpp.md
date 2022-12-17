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
  bundledCode: "#line 1 \"test/persistent_array_test.cpp\"\n#include <persistent_array.hpp>\r\
    \n\r\n#include <vector>\r\n#include <stack>\r\n#include <algorithm>\r\n#include\
    \ <numeric>\r\n#include <iostream>\r\n#include <cassert>\r\n\r\nconstexpr int\
    \ N = 1e5;\r\nint main() {\r\n  std::vector<std::pair<int,int>> U(N);\r\n  for\
    \ (int i = 0; i < N; i++) {\r\n    U[i] = {rand()%N, rand()%N};\r\n  }\r\n  std::vector<std::pair<int,int>>\
    \ Q(N);\r\n  for (int i = 0; i < N; i++) {\r\n    Q[i] = {rand()%N+1, rand()%N};\r\
    \n  }\r\n  std::vector<int> expected(N+1, -1);\r\n  std::vector<int> st(N);\r\n\
    \  int k = 0;\r\n  std::vector<int> qi(N);\r\n  std::iota(qi.begin(), qi.end(),\
    \ 0);\r\n  std::sort(qi.begin(), qi.end(),\r\n            [&](int l, int r) {\
    \ return Q[l].first < Q[r].first; });\r\n  for (int i = 0; i < (int)U.size();\
    \ i++) {\r\n    st[U[i].first] = U[i].second;\r\n    while (k < N and Q[qi[k]].first\
    \ == i+1) {\r\n      expected[qi[k]] = st[Q[qi[k]].second];\r\n      k++;\r\n\
    \    }\r\n  }\r\n\r\n  std::vector<PersistentArray<int>> ps(N+1);\r\n  for (int\
    \ i = 0; i < N; i++)\r\n    ps[0] = ps[0].set(i, 0);\r\n  for (int i = 0; i <\
    \ N; i++) {\r\n    ps[i+1] = ps[i].set(U[i].first, U[i].second);\r\n  }\r\n\r\n\
    \  for (int i = 0; i < (int)Q.size(); i++) {\r\n    int v = ps[Q[i].first].get(Q[i].second);\r\
    \n    if (expected[i] != v) {\r\n      std::cout << i<<\", \"<<Q[i].first<<\"\
    , \"<<Q[i].second<<\" expected[i] != ps[Q[i].first].get(Q[i].second): \" << expected[i]\
    \ << ' ' << v << std::endl;\r\n      exit(EXIT_FAILURE);\r\n    }\r\n  }\r\n \
    \ std::cout << \"OK\" << std::endl;\r\n}\r\n"
  code: "#include <persistent_array.hpp>\r\n\r\n#include <vector>\r\n#include <stack>\r\
    \n#include <algorithm>\r\n#include <numeric>\r\n#include <iostream>\r\n#include\
    \ <cassert>\r\n\r\nconstexpr int N = 1e5;\r\nint main() {\r\n  std::vector<std::pair<int,int>>\
    \ U(N);\r\n  for (int i = 0; i < N; i++) {\r\n    U[i] = {rand()%N, rand()%N};\r\
    \n  }\r\n  std::vector<std::pair<int,int>> Q(N);\r\n  for (int i = 0; i < N; i++)\
    \ {\r\n    Q[i] = {rand()%N+1, rand()%N};\r\n  }\r\n  std::vector<int> expected(N+1,\
    \ -1);\r\n  std::vector<int> st(N);\r\n  int k = 0;\r\n  std::vector<int> qi(N);\r\
    \n  std::iota(qi.begin(), qi.end(), 0);\r\n  std::sort(qi.begin(), qi.end(),\r\
    \n            [&](int l, int r) { return Q[l].first < Q[r].first; });\r\n  for\
    \ (int i = 0; i < (int)U.size(); i++) {\r\n    st[U[i].first] = U[i].second;\r\
    \n    while (k < N and Q[qi[k]].first == i+1) {\r\n      expected[qi[k]] = st[Q[qi[k]].second];\r\
    \n      k++;\r\n    }\r\n  }\r\n\r\n  std::vector<PersistentArray<int>> ps(N+1);\r\
    \n  for (int i = 0; i < N; i++)\r\n    ps[0] = ps[0].set(i, 0);\r\n  for (int\
    \ i = 0; i < N; i++) {\r\n    ps[i+1] = ps[i].set(U[i].first, U[i].second);\r\n\
    \  }\r\n\r\n  for (int i = 0; i < (int)Q.size(); i++) {\r\n    int v = ps[Q[i].first].get(Q[i].second);\r\
    \n    if (expected[i] != v) {\r\n      std::cout << i<<\", \"<<Q[i].first<<\"\
    , \"<<Q[i].second<<\" expected[i] != ps[Q[i].first].get(Q[i].second): \" << expected[i]\
    \ << ' ' << v << std::endl;\r\n      exit(EXIT_FAILURE);\r\n    }\r\n  }\r\n \
    \ std::cout << \"OK\" << std::endl;\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/persistent_array_test.cpp
  requiredBy: []
  timestamp: '2022-11-27 14:51:20+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/persistent_array_test.cpp
layout: document
redirect_from:
- /library/test/persistent_array_test.cpp
- /library/test/persistent_array_test.cpp.html
title: test/persistent_array_test.cpp
---
