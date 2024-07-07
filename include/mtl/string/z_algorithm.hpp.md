---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/string/z_algorithm.test.cpp
    title: test/string/z_algorithm.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/string/z_algorithm.hpp\"\n#include <vector>\r\
    \n#include <iterator>\r\n\r\ntemplate<typename It>\r\nstd::vector<int> z_algorithm(It\
    \ begin, It end) {\r\n  using trait = std::iterator_traits<It>;\r\n  static_assert(std::is_base_of<std::random_access_iterator_tag,\
    \ typename trait::iterator_category>::value, \"\");\r\n  int n = end - begin;\r\
    \n  std::vector<int> z(n);\r\n  z[0] = n;\r\n  int i = 1, j = 0;\r\n  while (i\
    \ < n) {\r\n    while (i+j < n and *(begin + i+j) == *(begin + j))\r\n      ++j;\r\
    \n    if (j == 0) {\r\n      ++i;\r\n      continue;\r\n    }\r\n    z[i] = j;\r\
    \n    int k = 1;\r\n    while (k < j and k + z[k] < j) {\r\n      z[i+k] = z[k];\r\
    \n      ++k;\r\n    }\r\n    i += k;\r\n    j -= k;\r\n  }\r\n  return z;\r\n\
    }\n"
  code: "#pragma once\r\n#include <vector>\r\n#include <iterator>\r\n\r\ntemplate<typename\
    \ It>\r\nstd::vector<int> z_algorithm(It begin, It end) {\r\n  using trait = std::iterator_traits<It>;\r\
    \n  static_assert(std::is_base_of<std::random_access_iterator_tag, typename trait::iterator_category>::value,\
    \ \"\");\r\n  int n = end - begin;\r\n  std::vector<int> z(n);\r\n  z[0] = n;\r\
    \n  int i = 1, j = 0;\r\n  while (i < n) {\r\n    while (i+j < n and *(begin +\
    \ i+j) == *(begin + j))\r\n      ++j;\r\n    if (j == 0) {\r\n      ++i;\r\n \
    \     continue;\r\n    }\r\n    z[i] = j;\r\n    int k = 1;\r\n    while (k <\
    \ j and k + z[k] < j) {\r\n      z[i+k] = z[k];\r\n      ++k;\r\n    }\r\n   \
    \ i += k;\r\n    j -= k;\r\n  }\r\n  return z;\r\n}"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/string/z_algorithm.hpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/string/z_algorithm.test.cpp
documentation_of: include/mtl/string/z_algorithm.hpp
layout: document
redirect_from:
- /library/include/mtl/string/z_algorithm.hpp
- /library/include/mtl/string/z_algorithm.hpp.html
title: include/mtl/string/z_algorithm.hpp
---
