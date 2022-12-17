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
  bundledCode: "#line 1 \"test/modular.cpp\"\n#include <modular.hpp>\n\n#include <iostream>\n\
    \nusing namespace std;\nusing ll = long long;\n\nint main() {\n  cin.tie(nullptr);\
    \ ios::sync_with_stdio(false);\n\n  constexpr ll MOD = 1e9+7;\n  using mint =\
    \ Modular<MOD>;\n  mint x = 34567;\n  x += 4132;\n  x -= 8952;\n  x *= 1785;\n\
    \  x /= 52;\n\n  return 0;\n}\n"
  code: "#include <modular.hpp>\n\n#include <iostream>\n\nusing namespace std;\nusing\
    \ ll = long long;\n\nint main() {\n  cin.tie(nullptr); ios::sync_with_stdio(false);\n\
    \n  constexpr ll MOD = 1e9+7;\n  using mint = Modular<MOD>;\n  mint x = 34567;\n\
    \  x += 4132;\n  x -= 8952;\n  x *= 1785;\n  x /= 52;\n\n  return 0;\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: test/modular.cpp
  requiredBy: []
  timestamp: '2022-11-27 14:51:20+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/modular.cpp
layout: document
redirect_from:
- /library/test/modular.cpp
- /library/test/modular.cpp.html
title: test/modular.cpp
---
