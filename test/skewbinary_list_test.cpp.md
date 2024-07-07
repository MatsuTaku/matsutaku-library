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
  bundledCode: "#line 1 \"test/skewbinary_list_test.cpp\"\n#include <skewbinary_list.hpp>\r\
    \n\r\n#include <iostream>\r\n#include <vector>\r\n#include <cassert>\r\n#include\
    \ <algorithm>\r\n#include <cmath>\r\n#include <random>\r\n\r\nint main() {\r\n\
    \  const int Max = 1e5;\r\n  int n = Max;\r\n  std::vector<int> values(n);\r\n\
    \  for (int& v : values) v = rand();\r\n\r\n//  PersistentSkewbinaryList<int>\
    \ S;\r\n//  for (int v : values) {\r\n//    S = S.pushed(v);\r\n//  }\r\n//  std::reverse(values.begin(),\
    \ values.end());\r\n  PersistentSkewbinaryList<int> S(values.begin(), values.end());\r\
    \n\r\n//  S.print_for_debug();\r\n\r\n  { // Test random-access\r\n//    std::cout\
    \ << \"random-access\" << std::endl;\r\n    for (int i = 0; i < S.size(); i++)\
    \ {\r\n      int get = S.get_at(i);\r\n      if (get != values[i]) {\r\n     \
    \   std::cout << \"get \" << i << \" \" << get << \" != \" << values[i] << std::endl;\r\
    \n        assert(false);\r\n        return 1;\r\n      }\r\n    }\r\n  }\r\n\r\
    \n  { // Test pop\r\n//    std::cout << \"pop\" << std::endl;\r\n    auto P =\
    \ S;\r\n    int popcount = 0;\r\n    while (!P.empty()) {\r\n      auto get =\
    \ P.front();\r\n      if (get != values[popcount]) {\r\n        std::cout << \"\
    pop \" << popcount << \" \" << get << \" != \" << values[popcount] << std::endl;\r\
    \n        assert(false);\r\n        return 1;\r\n      }\r\n      P = P.popped();\r\
    \n      popcount++;\r\n    }\r\n  }\r\n\r\n  { // Test drop\r\n//    std::cout\
    \ << \"drop\" << std::endl;\r\n    auto O = S;\r\n    int dropcount = 0;\r\n \
    \   std::uniform_real_distribution<double> dist(0,1);\r\n    std::default_random_engine\
    \ eng;\r\n    while (!O.empty()) {\r\n      auto get = O.front();\r\n      if\
    \ (get != values[dropcount]) {\r\n        std::cout << \"drop \" << dropcount\
    \ << \" \" << get << \" != \" << values[dropcount] << std::endl;\r\n        assert(false);\r\
    \n        return 1;\r\n      }\r\n      int off = std::round((std::exp(dist(eng))-1)\
    \ * O.size());\r\n      off = std::min(std::max(off, 1), (int)O.size());\r\n \
    \     O = O.dropped(off);\r\n      dropcount += off;\r\n    }\r\n  }\r\n\r\n \
    \ { // Test update\r\n    for (int t = 0; t < 100; t++) {\r\n      int idx = rand()\
    \ % n;\r\n      values[idx] = rand();\r\n      S = S.updated_at(idx, values[idx]);\r\
    \n      for (int i = 0; i < n; i++) {\r\n        auto get = S.get_at(i);\r\n \
    \       if (get != values[i]) {\r\n          std::cout << \"update \" << idx <<\
    \ \" \" << get << \" != \" << values[i] << std::endl;\r\n          assert(false);\r\
    \n          return 1;\r\n        }\r\n      }\r\n    }\r\n  }\r\n\r\n  { // Test\
    \ iterator\r\n//    std::cout << \"iter\" << std::endl;\r\n    int i = 0;\r\n\
    \    for (auto v : S) {\r\n      if (v != values[i]) {\r\n        std::cout <<\
    \ \"iter get \" << i << \" \" << v << \" != \" << values[i] << std::endl;\r\n\
    \        assert(false);\r\n        return 1;\r\n      }\r\n      ++i;\r\n    }\r\
    \n  }\r\n\r\n  std::cout << \"OK\" << std::endl;\r\n}\r\n"
  code: "#include <skewbinary_list.hpp>\r\n\r\n#include <iostream>\r\n#include <vector>\r\
    \n#include <cassert>\r\n#include <algorithm>\r\n#include <cmath>\r\n#include <random>\r\
    \n\r\nint main() {\r\n  const int Max = 1e5;\r\n  int n = Max;\r\n  std::vector<int>\
    \ values(n);\r\n  for (int& v : values) v = rand();\r\n\r\n//  PersistentSkewbinaryList<int>\
    \ S;\r\n//  for (int v : values) {\r\n//    S = S.pushed(v);\r\n//  }\r\n//  std::reverse(values.begin(),\
    \ values.end());\r\n  PersistentSkewbinaryList<int> S(values.begin(), values.end());\r\
    \n\r\n//  S.print_for_debug();\r\n\r\n  { // Test random-access\r\n//    std::cout\
    \ << \"random-access\" << std::endl;\r\n    for (int i = 0; i < S.size(); i++)\
    \ {\r\n      int get = S.get_at(i);\r\n      if (get != values[i]) {\r\n     \
    \   std::cout << \"get \" << i << \" \" << get << \" != \" << values[i] << std::endl;\r\
    \n        assert(false);\r\n        return 1;\r\n      }\r\n    }\r\n  }\r\n\r\
    \n  { // Test pop\r\n//    std::cout << \"pop\" << std::endl;\r\n    auto P =\
    \ S;\r\n    int popcount = 0;\r\n    while (!P.empty()) {\r\n      auto get =\
    \ P.front();\r\n      if (get != values[popcount]) {\r\n        std::cout << \"\
    pop \" << popcount << \" \" << get << \" != \" << values[popcount] << std::endl;\r\
    \n        assert(false);\r\n        return 1;\r\n      }\r\n      P = P.popped();\r\
    \n      popcount++;\r\n    }\r\n  }\r\n\r\n  { // Test drop\r\n//    std::cout\
    \ << \"drop\" << std::endl;\r\n    auto O = S;\r\n    int dropcount = 0;\r\n \
    \   std::uniform_real_distribution<double> dist(0,1);\r\n    std::default_random_engine\
    \ eng;\r\n    while (!O.empty()) {\r\n      auto get = O.front();\r\n      if\
    \ (get != values[dropcount]) {\r\n        std::cout << \"drop \" << dropcount\
    \ << \" \" << get << \" != \" << values[dropcount] << std::endl;\r\n        assert(false);\r\
    \n        return 1;\r\n      }\r\n      int off = std::round((std::exp(dist(eng))-1)\
    \ * O.size());\r\n      off = std::min(std::max(off, 1), (int)O.size());\r\n \
    \     O = O.dropped(off);\r\n      dropcount += off;\r\n    }\r\n  }\r\n\r\n \
    \ { // Test update\r\n    for (int t = 0; t < 100; t++) {\r\n      int idx = rand()\
    \ % n;\r\n      values[idx] = rand();\r\n      S = S.updated_at(idx, values[idx]);\r\
    \n      for (int i = 0; i < n; i++) {\r\n        auto get = S.get_at(i);\r\n \
    \       if (get != values[i]) {\r\n          std::cout << \"update \" << idx <<\
    \ \" \" << get << \" != \" << values[i] << std::endl;\r\n          assert(false);\r\
    \n          return 1;\r\n        }\r\n      }\r\n    }\r\n  }\r\n\r\n  { // Test\
    \ iterator\r\n//    std::cout << \"iter\" << std::endl;\r\n    int i = 0;\r\n\
    \    for (auto v : S) {\r\n      if (v != values[i]) {\r\n        std::cout <<\
    \ \"iter get \" << i << \" \" << v << \" != \" << values[i] << std::endl;\r\n\
    \        assert(false);\r\n        return 1;\r\n      }\r\n      ++i;\r\n    }\r\
    \n  }\r\n\r\n  std::cout << \"OK\" << std::endl;\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/skewbinary_list_test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/skewbinary_list_test.cpp
layout: document
redirect_from:
- /library/test/skewbinary_list_test.cpp
- /library/test/skewbinary_list_test.cpp.html
title: test/skewbinary_list_test.cpp
---
