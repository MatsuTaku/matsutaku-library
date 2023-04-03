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
  bundledCode: "#line 1 \"test/clz.cpp\"\n#include <bit_manip.hpp>\r\n#include <iostream>\r\
    \n#include <bitset>\r\n\r\nint main() {\r\n  static constexpr size_t cases = 1e6;\r\
    \n  for (int i = 0; i < cases; i++) {\r\n    uint64_t x = ((unsigned long long)rand()\
    \ << 32) | rand();\r\n    int cnt = 0;\r\n    while (cnt < 64 and (x & (1ull<<(63-cnt)))\
    \ == 0)\r\n      cnt++;\r\n    auto val = clz(x);\r\n    if (cnt != val) {\r\n\
    \      std::cerr<<\"clz(\"<<(std::bitset<64>(x))<<\") is \"<<cnt\r\n      <<\"\
    . calculated: \"<<val<<std::endl;\r\n    }\r\n  }\r\n}\r\n"
  code: "#include <bit_manip.hpp>\r\n#include <iostream>\r\n#include <bitset>\r\n\r\
    \nint main() {\r\n  static constexpr size_t cases = 1e6;\r\n  for (int i = 0;\
    \ i < cases; i++) {\r\n    uint64_t x = ((unsigned long long)rand() << 32) | rand();\r\
    \n    int cnt = 0;\r\n    while (cnt < 64 and (x & (1ull<<(63-cnt))) == 0)\r\n\
    \      cnt++;\r\n    auto val = clz(x);\r\n    if (cnt != val) {\r\n      std::cerr<<\"\
    clz(\"<<(std::bitset<64>(x))<<\") is \"<<cnt\r\n      <<\". calculated: \"<<val<<std::endl;\r\
    \n    }\r\n  }\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/clz.cpp
  requiredBy: []
  timestamp: '2022-11-27 14:51:20+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/clz.cpp
layout: document
redirect_from:
- /library/test/clz.cpp
- /library/test/clz.cpp.html
title: test/clz.cpp
---