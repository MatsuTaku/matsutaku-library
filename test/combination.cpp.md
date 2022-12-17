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
  bundledCode: "#line 1 \"test/combination.cpp\"\n#include <iostream>\n\n#include\
    \ <enumerate.hpp>\n#include <modular.hpp>\n\nusing namespace std;\nusing ll =\
    \ long long;\n\nint main() {\n  cin.tie(nullptr);\n  ios::sync_with_stdio(false);\n\
    \n  Enumerate<Modular<int(1e9) + 7>> enm(1e5);\n  auto a = enm.cmb(4, 2);\n  if\
    \ (a != 6) {\n    cerr << \"Error cmb(4,2) is not equal to \" << a << endl;\n\
    \  }\n  auto b = enm.cmb(5, 3);\n  if (b != 10) {\n    cerr << \"Error cmb(5,3)\
    \ is not equal to \" << b << endl;\n  }\n  auto c = enm.cmb(6, 3);\n  if (c !=\
    \ 20) {\n    cerr << \"Error cmb(6,3) is not equal to \" << c << endl;\n  }\n\n\
    \  return 0;\n}\n"
  code: "#include <iostream>\n\n#include <enumerate.hpp>\n#include <modular.hpp>\n\
    \nusing namespace std;\nusing ll = long long;\n\nint main() {\n  cin.tie(nullptr);\n\
    \  ios::sync_with_stdio(false);\n\n  Enumerate<Modular<int(1e9) + 7>> enm(1e5);\n\
    \  auto a = enm.cmb(4, 2);\n  if (a != 6) {\n    cerr << \"Error cmb(4,2) is not\
    \ equal to \" << a << endl;\n  }\n  auto b = enm.cmb(5, 3);\n  if (b != 10) {\n\
    \    cerr << \"Error cmb(5,3) is not equal to \" << b << endl;\n  }\n  auto c\
    \ = enm.cmb(6, 3);\n  if (c != 20) {\n    cerr << \"Error cmb(6,3) is not equal\
    \ to \" << c << endl;\n  }\n\n  return 0;\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: test/combination.cpp
  requiredBy: []
  timestamp: '2022-11-27 14:51:20+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/combination.cpp
layout: document
redirect_from:
- /library/test/combination.cpp
- /library/test/combination.cpp.html
title: test/combination.cpp
---
