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
  bundledCode: "#line 1 \"test/integer_set_test.cpp\"\n#include <integer_set.hpp>\r\
    \n\r\n#include <iostream>\r\n#include <vector>\r\n#include <cassert>\r\n\r\nconstexpr\
    \ int Max = 4e5;\r\nconstexpr int W = 20;\r\n//  const int Max = 1<<10;\r\n\r\n\
    template<typename Set>\r\nint test_set() {\r\n  std::vector<int> values;\r\n \
    \ while (values.empty()) {\r\n    for (int i = 0; i < Max; i++) {\r\n      if\
    \ (i == 0 or rand()%4 == 0)\r\n        values.push_back(i);\r\n    }\r\n  }\r\n\
    \  int n = values.size();\r\n  std::vector<int> shuffled = values;\r\n  for (int\
    \ i = 0; i < n; i++) {\r\n    std::swap(shuffled[i], shuffled[rand()%n]);\r\n\
    \  }\r\n\r\n  Set S;\r\n  for (int v : shuffled) {\r\n    S.insert(v);\r\n  }\r\
    \n//  S.print_for_debug();\r\n  int target = -1;\r\n  int pred = -1;\r\n  int\
    \ succ = values[0];\r\n  int k = 0;\r\n  auto log = [&]() {\r\n    std::cout <<\
    \ pred << ' ' << target << ' ' << succ << std::endl;\r\n    assert(false);\r\n\
    \  };\r\n  for (int i = 0; i < Max; i++) {\r\n    if (k < n and values[k] == i)\
    \ {\r\n      target = values[k];\r\n      pred = k-1 >= 0 ? values[k-1] : -1;\r\
    \n      succ = k+1 < n ? values[k+1] : -1;\r\n      k++;\r\n    } else {\r\n \
    \     pred = k-1 >= 0 ? values[k-1] : -1;\r\n      target = -1;\r\n    }\r\n\r\
    \n    auto fit = S.find(i);\r\n    if (fit != S.end()) {\r\n      if ((int) *fit\
    \ != i) {\r\n        std::cout << \"find: \" << i << std::endl;\r\n        log();\r\
    \n        return 1;\r\n      }\r\n    } else {\r\n      if (target != -1) {\r\n\
    \        log();\r\n        return 1;\r\n      }\r\n    }\r\n\r\n    auto sucit\
    \ = S.successor(i);\r\n    if (sucit != S.end()) {\r\n      if ((int) *sucit !=\
    \ succ) {\r\n        std::cout << \"succ: \" << *sucit << std::endl;\r\n     \
    \   log();\r\n        return 1;\r\n      }\r\n    } else {\r\n      if (succ !=\
    \ -1) {\r\n        log();\r\n        return 1;\r\n      }\r\n    }\r\n\r\n   \
    \ auto predit = S.predecessor(i);\r\n    if (predit != S.end()) {\r\n      if\
    \ ((int) *predit != pred) {\r\n        std::cout << \"pred: \" << *predit << std::endl;\r\
    \n        log();\r\n        return 1;\r\n      }\r\n    } else {\r\n      if (pred\
    \ != -1) {\r\n        log();\r\n        return 1;\r\n      }\r\n    }\r\n  }\r\
    \n\r\n  int size = n;\r\n  if ((int) S.size() != size) {\r\n    std::cout << S.size()\
    \ << ' ' << size<< std::endl;\r\n    log();\r\n    return 1;\r\n  }\r\n  for (int\
    \ v : shuffled) {\r\n    S.erase(v);\r\n    size--;\r\n    if ((int) S.size()\
    \ != size) {\r\n      std::cout << S.size() << ' ' << size<< std::endl;\r\n  \
    \    assert(false);\r\n      return 1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\
    \r\nint main() {\r\n  if (test_set<XFastTrieSet<W>>() != 0)\r\n    return 1;\r\
    \n  if (test_set<YFastTrieSet<W>>() != 0)\r\n    return 1;\r\n  std::cout << \"\
    OK\" << std::endl;\r\n}\r\n"
  code: "#include <integer_set.hpp>\r\n\r\n#include <iostream>\r\n#include <vector>\r\
    \n#include <cassert>\r\n\r\nconstexpr int Max = 4e5;\r\nconstexpr int W = 20;\r\
    \n//  const int Max = 1<<10;\r\n\r\ntemplate<typename Set>\r\nint test_set() {\r\
    \n  std::vector<int> values;\r\n  while (values.empty()) {\r\n    for (int i =\
    \ 0; i < Max; i++) {\r\n      if (i == 0 or rand()%4 == 0)\r\n        values.push_back(i);\r\
    \n    }\r\n  }\r\n  int n = values.size();\r\n  std::vector<int> shuffled = values;\r\
    \n  for (int i = 0; i < n; i++) {\r\n    std::swap(shuffled[i], shuffled[rand()%n]);\r\
    \n  }\r\n\r\n  Set S;\r\n  for (int v : shuffled) {\r\n    S.insert(v);\r\n  }\r\
    \n//  S.print_for_debug();\r\n  int target = -1;\r\n  int pred = -1;\r\n  int\
    \ succ = values[0];\r\n  int k = 0;\r\n  auto log = [&]() {\r\n    std::cout <<\
    \ pred << ' ' << target << ' ' << succ << std::endl;\r\n    assert(false);\r\n\
    \  };\r\n  for (int i = 0; i < Max; i++) {\r\n    if (k < n and values[k] == i)\
    \ {\r\n      target = values[k];\r\n      pred = k-1 >= 0 ? values[k-1] : -1;\r\
    \n      succ = k+1 < n ? values[k+1] : -1;\r\n      k++;\r\n    } else {\r\n \
    \     pred = k-1 >= 0 ? values[k-1] : -1;\r\n      target = -1;\r\n    }\r\n\r\
    \n    auto fit = S.find(i);\r\n    if (fit != S.end()) {\r\n      if ((int) *fit\
    \ != i) {\r\n        std::cout << \"find: \" << i << std::endl;\r\n        log();\r\
    \n        return 1;\r\n      }\r\n    } else {\r\n      if (target != -1) {\r\n\
    \        log();\r\n        return 1;\r\n      }\r\n    }\r\n\r\n    auto sucit\
    \ = S.successor(i);\r\n    if (sucit != S.end()) {\r\n      if ((int) *sucit !=\
    \ succ) {\r\n        std::cout << \"succ: \" << *sucit << std::endl;\r\n     \
    \   log();\r\n        return 1;\r\n      }\r\n    } else {\r\n      if (succ !=\
    \ -1) {\r\n        log();\r\n        return 1;\r\n      }\r\n    }\r\n\r\n   \
    \ auto predit = S.predecessor(i);\r\n    if (predit != S.end()) {\r\n      if\
    \ ((int) *predit != pred) {\r\n        std::cout << \"pred: \" << *predit << std::endl;\r\
    \n        log();\r\n        return 1;\r\n      }\r\n    } else {\r\n      if (pred\
    \ != -1) {\r\n        log();\r\n        return 1;\r\n      }\r\n    }\r\n  }\r\
    \n\r\n  int size = n;\r\n  if ((int) S.size() != size) {\r\n    std::cout << S.size()\
    \ << ' ' << size<< std::endl;\r\n    log();\r\n    return 1;\r\n  }\r\n  for (int\
    \ v : shuffled) {\r\n    S.erase(v);\r\n    size--;\r\n    if ((int) S.size()\
    \ != size) {\r\n      std::cout << S.size() << ' ' << size<< std::endl;\r\n  \
    \    assert(false);\r\n      return 1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\
    \r\nint main() {\r\n  if (test_set<XFastTrieSet<W>>() != 0)\r\n    return 1;\r\
    \n  if (test_set<YFastTrieSet<W>>() != 0)\r\n    return 1;\r\n  std::cout << \"\
    OK\" << std::endl;\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/integer_set_test.cpp
  requiredBy: []
  timestamp: '2022-11-27 14:51:20+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/integer_set_test.cpp
layout: document
redirect_from:
- /library/test/integer_set_test.cpp
- /library/test/integer_set_test.cpp.html
title: test/integer_set_test.cpp
---
