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
  bundledCode: "#line 1 \"test/combination.cpp\"\n#include <iostream>\r\n\r\n#include\
    \ <enumerate.hpp>\r\n#include <modular.hpp>\r\n\r\nusing namespace std;\r\nusing\
    \ ll = long long;\r\n\r\nint main() {\r\n  cin.tie(nullptr);\r\n  ios::sync_with_stdio(false);\r\
    \n\r\n  Enumerate<Modular<int(1e9) + 7>> enm(1e5);\r\n  auto a = enm.cmb(4, 2);\r\
    \n  if (a != 6) {\r\n    cerr << \"Error cmb(4,2) is not equal to \" << a << endl;\r\
    \n  }\r\n  auto b = enm.cmb(5, 3);\r\n  if (b != 10) {\r\n    cerr << \"Error\
    \ cmb(5,3) is not equal to \" << b << endl;\r\n  }\r\n  auto c = enm.cmb(6, 3);\r\
    \n  if (c != 20) {\r\n    cerr << \"Error cmb(6,3) is not equal to \" << c <<\
    \ endl;\r\n  }\r\n\r\n  return 0;\r\n}\r\n"
  code: "#include <iostream>\r\n\r\n#include <enumerate.hpp>\r\n#include <modular.hpp>\r\
    \n\r\nusing namespace std;\r\nusing ll = long long;\r\n\r\nint main() {\r\n  cin.tie(nullptr);\r\
    \n  ios::sync_with_stdio(false);\r\n\r\n  Enumerate<Modular<int(1e9) + 7>> enm(1e5);\r\
    \n  auto a = enm.cmb(4, 2);\r\n  if (a != 6) {\r\n    cerr << \"Error cmb(4,2)\
    \ is not equal to \" << a << endl;\r\n  }\r\n  auto b = enm.cmb(5, 3);\r\n  if\
    \ (b != 10) {\r\n    cerr << \"Error cmb(5,3) is not equal to \" << b << endl;\r\
    \n  }\r\n  auto c = enm.cmb(6, 3);\r\n  if (c != 20) {\r\n    cerr << \"Error\
    \ cmb(6,3) is not equal to \" << c << endl;\r\n  }\r\n\r\n  return 0;\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/combination.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/combination.cpp
layout: document
redirect_from:
- /library/test/combination.cpp
- /library/test/combination.cpp.html
title: test/combination.cpp
---
