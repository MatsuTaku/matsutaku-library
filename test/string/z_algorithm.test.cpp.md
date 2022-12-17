---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/string/z_algorithm.hpp
    title: include/mtl/string/z_algorithm.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/zalgorithm
    links:
    - https://judge.yosupo.jp/problem/zalgorithm
  bundledCode: "#line 1 \"test/string/z_algorithm.test.cpp\"\n#define PROBLEM \"https://judge.yosupo.jp/problem/zalgorithm\"\
    \r\n#line 2 \"include/mtl/string/z_algorithm.hpp\"\n#include <vector>\r\n#include\
    \ <iterator>\r\n\r\ntemplate<typename It>\r\nstd::vector<int> z_algorithm(It begin,\
    \ It end) {\r\n  using trait = std::iterator_traits<It>;\r\n  static_assert(std::is_base_of<std::random_access_iterator_tag,\
    \ typename trait::iterator_category>::value, \"\");\r\n  int n = end - begin;\r\
    \n  std::vector<int> z(n);\r\n  z[0] = n;\r\n  int i = 1, j = 0;\r\n  while (i\
    \ < n) {\r\n    while (i+j < n and *(begin + i+j) == *(begin + j))\r\n      ++j;\r\
    \n    if (j == 0) {\r\n      ++i;\r\n      continue;\r\n    }\r\n    z[i] = j;\r\
    \n    int k = 1;\r\n    while (k < j and k + z[k] < j) {\r\n      z[i+k] = z[k];\r\
    \n      ++k;\r\n    }\r\n    i += k;\r\n    j -= k;\r\n  }\r\n  return z;\r\n\
    }\n#line 3 \"test/string/z_algorithm.test.cpp\"\n#include <iostream>\r\nusing\
    \ namespace std;\r\n\r\nint main() {\r\n  string s; cin>>s;\r\n  auto z = z_algorithm(s.begin(),\
    \ s.end());\r\n  for (int i = 0; i < (int) s.size(); i++)\r\n    cout << z[i]\
    \ << ' ';\r\n  cout << endl;\r\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/zalgorithm\"\r\n#include\
    \ \"../../include/mtl/string/z_algorithm.hpp\"\r\n#include <iostream>\r\nusing\
    \ namespace std;\r\n\r\nint main() {\r\n  string s; cin>>s;\r\n  auto z = z_algorithm(s.begin(),\
    \ s.end());\r\n  for (int i = 0; i < (int) s.size(); i++)\r\n    cout << z[i]\
    \ << ' ';\r\n  cout << endl;\r\n}"
  dependsOn:
  - include/mtl/string/z_algorithm.hpp
  isVerificationFile: true
  path: test/string/z_algorithm.test.cpp
  requiredBy: []
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/string/z_algorithm.test.cpp
layout: document
redirect_from:
- /verify/test/string/z_algorithm.test.cpp
- /verify/test/string/z_algorithm.test.cpp.html
title: test/string/z_algorithm.test.cpp
---
