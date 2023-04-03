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
  bundledCode: "#line 1 \"test/modular.cpp\"\n#include <modular.hpp>\r\n\r\n#include\
    \ <iostream>\r\n\r\nusing namespace std;\r\nusing ll = long long;\r\n\r\nint main()\
    \ {\r\n  cin.tie(nullptr); ios::sync_with_stdio(false);\r\n\r\n  constexpr ll\
    \ MOD = 1e9+7;\r\n  using mint = Modular<MOD>;\r\n  mint x = 34567;\r\n  x +=\
    \ 4132;\r\n  x -= 8952;\r\n  x *= 1785;\r\n  x /= 52;\r\n\r\n  return 0;\r\n}\r\
    \n"
  code: "#include <modular.hpp>\r\n\r\n#include <iostream>\r\n\r\nusing namespace\
    \ std;\r\nusing ll = long long;\r\n\r\nint main() {\r\n  cin.tie(nullptr); ios::sync_with_stdio(false);\r\
    \n\r\n  constexpr ll MOD = 1e9+7;\r\n  using mint = Modular<MOD>;\r\n  mint x\
    \ = 34567;\r\n  x += 4132;\r\n  x -= 8952;\r\n  x *= 1785;\r\n  x /= 52;\r\n\r\
    \n  return 0;\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/modular.cpp
  requiredBy: []
  timestamp: '2023-04-03 03:00:14+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/modular.cpp
layout: document
redirect_from:
- /library/test/modular.cpp
- /library/test/modular.cpp.html
title: test/modular.cpp
---
