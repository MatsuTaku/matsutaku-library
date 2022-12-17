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
  bundledCode: "#line 1 \"test/bitmap_test.cpp\"\n#include <bitmap.hpp>\r\n#include\
    \ <iostream>\r\n\r\nint main() {\r\n  const int n = 1e6;\r\n  std::vector<bool>\
    \ B(n);\r\n  int c = 0;\r\n  int k = 0;\r\n  std::vector<int> rank(n), select(n);\r\
    \n  for (int i = 0; i < n; i++) {\r\n    rank[i] = c;\r\n    B[i] = rand()%2;\r\
    \n    c += B[i];\r\n    if (B[i]) select[k++] = i;\r\n  }\r\n\r\n  Bitmap bm(B.begin(),\
    \ B.end());\r\n  // get\r\n  for (int i = 0; i < n; i++) {\r\n    auto v = bm.get(i);\r\
    \n    if (v != B[i]) {\r\n      std::cout << \"Failed get: \" << i << \" bm.get\
    \ \" << v << \" != B \" << B[i] << std::endl;\r\n      return 1;\r\n    }\r\n\
    \  }\r\n  // rank\r\n  for (int i = 0; i < n; i++) {\r\n    auto v = bm.rank(i);\r\
    \n    if (v != rank[i]) {\r\n      std::cout << \"Failed rank: \" << i << \" bm.rank\
    \ \" << v << \" != rank \" << rank[i] << std::endl;\r\n      return 1;\r\n   \
    \ }\r\n  }\r\n  // select\r\n  for (int i = 1; i < k; i++) {\r\n    auto v = bm.select(i);\r\
    \n    if (v != select[i]) {\r\n      std::cout << \"Failed select: \" << i <<\
    \ \" bm.select \" << v << \" != select \" << select[i] << std::endl;\r\n     \
    \ return 1;\r\n    }\r\n  }\r\n  std::cout << \"OK\" << std::endl;\r\n\r\n}\r\n"
  code: "#include <bitmap.hpp>\r\n#include <iostream>\r\n\r\nint main() {\r\n  const\
    \ int n = 1e6;\r\n  std::vector<bool> B(n);\r\n  int c = 0;\r\n  int k = 0;\r\n\
    \  std::vector<int> rank(n), select(n);\r\n  for (int i = 0; i < n; i++) {\r\n\
    \    rank[i] = c;\r\n    B[i] = rand()%2;\r\n    c += B[i];\r\n    if (B[i]) select[k++]\
    \ = i;\r\n  }\r\n\r\n  Bitmap bm(B.begin(), B.end());\r\n  // get\r\n  for (int\
    \ i = 0; i < n; i++) {\r\n    auto v = bm.get(i);\r\n    if (v != B[i]) {\r\n\
    \      std::cout << \"Failed get: \" << i << \" bm.get \" << v << \" != B \" <<\
    \ B[i] << std::endl;\r\n      return 1;\r\n    }\r\n  }\r\n  // rank\r\n  for\
    \ (int i = 0; i < n; i++) {\r\n    auto v = bm.rank(i);\r\n    if (v != rank[i])\
    \ {\r\n      std::cout << \"Failed rank: \" << i << \" bm.rank \" << v << \" !=\
    \ rank \" << rank[i] << std::endl;\r\n      return 1;\r\n    }\r\n  }\r\n  //\
    \ select\r\n  for (int i = 1; i < k; i++) {\r\n    auto v = bm.select(i);\r\n\
    \    if (v != select[i]) {\r\n      std::cout << \"Failed select: \" << i << \"\
    \ bm.select \" << v << \" != select \" << select[i] << std::endl;\r\n      return\
    \ 1;\r\n    }\r\n  }\r\n  std::cout << \"OK\" << std::endl;\r\n\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/bitmap_test.cpp
  requiredBy: []
  timestamp: '2022-11-27 14:51:20+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/bitmap_test.cpp
layout: document
redirect_from:
- /library/test/bitmap_test.cpp
- /library/test/bitmap_test.cpp.html
title: test/bitmap_test.cpp
---
