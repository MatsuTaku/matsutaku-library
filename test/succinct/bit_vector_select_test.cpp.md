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
  bundledCode: "#line 1 \"test/succinct/bit_vector_select_test.cpp\"\n#include <succinct/bit_vector.hpp>\r\
    \n#include <bits/stdc++.h>\r\n\r\nint main() {\r\n  const int n = 1e6;\r\n  std::vector<bool>\
    \ B(n);\r\n  int c = 0;\r\n  int k = 0;\r\n  std::vector<int> rank(n), select(n),\
    \ select0(n);\r\n  for (int i = 0; i < n; i++) {\r\n    rank[i] = c;\r\n    B[i]\
    \ = rand()%2;\r\n    c += B[i];\r\n    if (B[i]) select[k++] = i;\r\n    else\
    \ select0[i-k] = i;\r\n  }\r\n\r\n  BitVector bm(B.begin(), B.end());\r\n  bm.build();\r\
    \n  // select\r\n  for (int i = 0; i < k; i++) {\r\n    int v = bm.select<1>(i);\r\
    \n    if (v != select[i]) {\r\n      std::cout << \"Failed select: \" << i <<\
    \ \" bm.select \" << v << \" != select \" << select[i] << std::endl;\r\n     \
    \ return 1;\r\n    }\r\n  }\r\n  // select0\r\n  for (int i = 0; i < n-k; i++)\
    \ {\r\n    int v = bm.select<0>(i);\r\n    if (v != select0[i]) {\r\n      std::cout\
    \ << \"Failed select0: \" << i << \" bm.select0 \" << v << \" != select0 \" <<\
    \ select0[i] << std::endl;\r\n      return 1;\r\n    }\r\n  }\r\n  std::cout <<\
    \ \"OK\" << std::endl;\r\n\r\n}\r\n"
  code: "#include <succinct/bit_vector.hpp>\r\n#include <bits/stdc++.h>\r\n\r\nint\
    \ main() {\r\n  const int n = 1e6;\r\n  std::vector<bool> B(n);\r\n  int c = 0;\r\
    \n  int k = 0;\r\n  std::vector<int> rank(n), select(n), select0(n);\r\n  for\
    \ (int i = 0; i < n; i++) {\r\n    rank[i] = c;\r\n    B[i] = rand()%2;\r\n  \
    \  c += B[i];\r\n    if (B[i]) select[k++] = i;\r\n    else select0[i-k] = i;\r\
    \n  }\r\n\r\n  BitVector bm(B.begin(), B.end());\r\n  bm.build();\r\n  // select\r\
    \n  for (int i = 0; i < k; i++) {\r\n    int v = bm.select<1>(i);\r\n    if (v\
    \ != select[i]) {\r\n      std::cout << \"Failed select: \" << i << \" bm.select\
    \ \" << v << \" != select \" << select[i] << std::endl;\r\n      return 1;\r\n\
    \    }\r\n  }\r\n  // select0\r\n  for (int i = 0; i < n-k; i++) {\r\n    int\
    \ v = bm.select<0>(i);\r\n    if (v != select0[i]) {\r\n      std::cout << \"\
    Failed select0: \" << i << \" bm.select0 \" << v << \" != select0 \" << select0[i]\
    \ << std::endl;\r\n      return 1;\r\n    }\r\n  }\r\n  std::cout << \"OK\" <<\
    \ std::endl;\r\n\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/succinct/bit_vector_select_test.cpp
  requiredBy: []
  timestamp: '2023-04-10 16:54:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/succinct/bit_vector_select_test.cpp
layout: document
redirect_from:
- /library/test/succinct/bit_vector_select_test.cpp
- /library/test/succinct/bit_vector_select_test.cpp.html
title: test/succinct/bit_vector_select_test.cpp
---
