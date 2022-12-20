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
  bundledCode: "#line 1 \"test/treap_test.cpp\"\n#include <treap.hpp>\r\n\r\n#include\
    \ <iostream>\r\n#include <vector>\r\n#include <set>\r\n#include <cassert>\r\n\
    #include <algorithm>\r\n\r\nint test_insert() {\r\n  constexpr int n = 300;\r\n\
    \  for (int t = 0; t < 300; t++) {\r\n    std::set<int> _v{0,\r\n            \
    \         std::numeric_limits<int>::max(),\r\n                     std::numeric_limits<int>::min()};\r\
    \n    while (_v.size() < n)\r\n      _v.insert(rand());\r\n    std::vector<int>\
    \ v(_v.begin(), _v.end());\r\n    TreapSet<int> s;\r\n    for (int i = 0; i <\
    \ n; i++) {\r\n      s.insert(v[i]);\r\n      if ((int)s.size() != i+1) {\r\n\
    \        std::cout << \"insert \" << t << \",\" << i << \": \" << s.size() <<\
    \ \" < \" << i+1 << std::endl;\r\n        assert(false);\r\n        return 1;\r\
    \n      }\r\n      int k = 0;\r\n      for (auto x : s) {\r\n        if (x !=\
    \ v[k]) {\r\n          std::cout << \"insert \" << t << \",\" << i << ' ' << k\
    \ << \": \" << x << \" != \" << v[k] << std::endl;\r\n          assert(false);\r\
    \n          return 1;\r\n        }\r\n        k++;\r\n      }\r\n    }\r\n  }\r\
    \n  return 0;\r\n}\r\n\r\nint test_split() {\r\n  constexpr int n = 1000;\r\n\
    \  for (int t = 0; t < 1000; t++) {\r\n    std::set<int> _v;\r\n    while (_v.size()\
    \ < n)\r\n      _v.insert(rand());\r\n    std::vector<int> v(_v.begin(), _v.end());\r\
    \n    TreapSet<int> r(v.begin(), v.end());\r\n    auto l = r.split(std::next(r.begin(),\
    \ n/2));\r\n    int k = 0;\r\n    for (auto x : l) {\r\n      if (x != v[k]) {\r\
    \n        std::cout << \"split \" << t << \",left,\" << k << \": \" << x << \"\
    \ != \" << v[k] << std::endl;\r\n        assert(false);\r\n        return 1;\r\
    \n      }\r\n      k++;\r\n    }\r\n    for (auto x : r) {\r\n      if (x != v[k])\
    \ {\r\n        std::cout << \"split \" << t << \",right,\" << k << \": \" << x\
    \ << \" != \" << v[k] << std::endl;\r\n        assert(false);\r\n        return\
    \ 1;\r\n      }\r\n      k++;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\nint test_absorb()\
    \ {\r\n  constexpr int n = 1000;\r\n  for (int t = 0; t < 1000; t++) {\r\n   \
    \ std::set<int> _v;\r\n    while (_v.size() < n)\r\n      _v.insert(rand());\r\
    \n    std::vector<int> v(_v.begin(), _v.end());\r\n    TreapSet<int> l(v.begin(),\
    \ v.begin()+n/2);\r\n    TreapSet<int> r(v.begin()+n/2, v.end());\r\n    r.absorb(&l);\r\
    \n    int k = 0;\r\n    for (auto x : r) {\r\n      if (x != v[k]) {\r\n     \
    \   std::cout << t << \", \" << k << \": \" << x << \" != \" << v[k] << std::endl;\r\
    \n        assert(false);\r\n        return 1;\r\n      }\r\n      k++;\r\n   \
    \ }\r\n  }\r\n  return 0;\r\n}\r\n\r\nint unit_test() {\r\n  if (test_insert())\r\
    \n    return 1;\r\n  if (test_split())\r\n    return 1;\r\n  if (test_absorb())\r\
    \n    return 1;\r\n  return 0;\r\n}\r\n\r\ntemplate<typename Set>\r\nint test_set()\
    \ {\r\n  const int Max = 4e5;\r\n  std::vector<int> values;\r\n  while (values.empty())\
    \ {\r\n    for (int i = 0; i < Max; i++)\r\n      if (rand()%4 == 0)\r\n     \
    \   values.push_back(i);\r\n  }\r\n  int n = values.size();\r\n  std::vector<int>\
    \ shuffled = values;\r\n  for (int i = 0; i < n; i++) {\r\n    std::swap(shuffled[i],\
    \ shuffled[rand()%n]);\r\n  }\r\n\r\n  Set S;\r\n  for (int v : shuffled) {\r\n\
    \    S.insert(v);\r\n  }\r\n//  S.print_for_debug();\r\n  int target = -1;\r\n\
    \  int pred = -1;\r\n  int succ = values[0];\r\n  int k = 0;\r\n  auto log = [&]()\
    \ {\r\n    std::cout << pred << ' ' << target << ' ' << succ << std::endl;\r\n\
    \    assert(false);\r\n  };\r\n  for (int i = 0; i < Max; i++) {\r\n    if (k\
    \ < n and values[k] == i) {\r\n      target = values[k];\r\n      pred = k-1 >=\
    \ 0 ? values[k-1] : -1;\r\n      succ = k+1 < n ? values[k+1] : -1;\r\n      k++;\r\
    \n    } else {\r\n      pred = k-1 >= 0 ? values[k-1] : -1;\r\n      target =\
    \ -1;\r\n    }\r\n\r\n    auto fit = S.find(i);\r\n    if (fit != S.end()) {\r\
    \n      if ((int) *fit != i) {\r\n        std::cout << \"find: \" << i << std::endl;\r\
    \n        log();\r\n        return 1;\r\n      }\r\n    } else {\r\n      if (target\
    \ != -1) {\r\n        log();\r\n        return 1;\r\n      }\r\n    }\r\n\r\n\
    \    auto sucit = S.successor(i);\r\n    if (sucit != S.end()) {\r\n      if ((int)\
    \ *sucit != succ) {\r\n        std::cout << \"succ: \" << *sucit << std::endl;\r\
    \n        log();\r\n        return 1;\r\n      }\r\n    } else {\r\n      if (succ\
    \ != -1) {\r\n        log();\r\n        return 1;\r\n      }\r\n    }\r\n\r\n\
    \    auto predit = S.predecessor(i);\r\n    if (predit != S.end()) {\r\n     \
    \ if ((int) *predit != pred) {\r\n        std::cout << \"pred: \" << *predit <<\
    \ std::endl;\r\n        log();\r\n        return 1;\r\n      }\r\n    } else {\r\
    \n      if (pred != -1) {\r\n        log();\r\n        return 1;\r\n      }\r\n\
    \    }\r\n  }\r\n\r\n  int size = n;\r\n  if ((int) S.size() != size) {\r\n  \
    \  std::cout << S.size() << ' ' << size<< std::endl;\r\n    log();\r\n    return\
    \ 1;\r\n  }\r\n  for (int v : shuffled) {\r\n    S.erase(v);\r\n    size--;\r\n\
    \    if ((int) S.size() != size) {\r\n      std::cout << S.size() << ' ' << size<<\
    \ std::endl;\r\n      log();\r\n      return 1;\r\n    }\r\n  }\r\n  return 0;\r\
    \n}\r\n\r\nint main() {\r\n  if (unit_test() != 0)\r\n    return 1;\r\n  if (test_set<Treap<int>>()\
    \ != 0)\r\n    return 1;\r\n  std::cout << \"OK\" << std::endl;\r\n}\r\n"
  code: "#include <treap.hpp>\r\n\r\n#include <iostream>\r\n#include <vector>\r\n\
    #include <set>\r\n#include <cassert>\r\n#include <algorithm>\r\n\r\nint test_insert()\
    \ {\r\n  constexpr int n = 300;\r\n  for (int t = 0; t < 300; t++) {\r\n    std::set<int>\
    \ _v{0,\r\n                     std::numeric_limits<int>::max(),\r\n         \
    \            std::numeric_limits<int>::min()};\r\n    while (_v.size() < n)\r\n\
    \      _v.insert(rand());\r\n    std::vector<int> v(_v.begin(), _v.end());\r\n\
    \    TreapSet<int> s;\r\n    for (int i = 0; i < n; i++) {\r\n      s.insert(v[i]);\r\
    \n      if ((int)s.size() != i+1) {\r\n        std::cout << \"insert \" << t <<\
    \ \",\" << i << \": \" << s.size() << \" < \" << i+1 << std::endl;\r\n       \
    \ assert(false);\r\n        return 1;\r\n      }\r\n      int k = 0;\r\n     \
    \ for (auto x : s) {\r\n        if (x != v[k]) {\r\n          std::cout << \"\
    insert \" << t << \",\" << i << ' ' << k << \": \" << x << \" != \" << v[k] <<\
    \ std::endl;\r\n          assert(false);\r\n          return 1;\r\n        }\r\
    \n        k++;\r\n      }\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\nint test_split()\
    \ {\r\n  constexpr int n = 1000;\r\n  for (int t = 0; t < 1000; t++) {\r\n   \
    \ std::set<int> _v;\r\n    while (_v.size() < n)\r\n      _v.insert(rand());\r\
    \n    std::vector<int> v(_v.begin(), _v.end());\r\n    TreapSet<int> r(v.begin(),\
    \ v.end());\r\n    auto l = r.split(std::next(r.begin(), n/2));\r\n    int k =\
    \ 0;\r\n    for (auto x : l) {\r\n      if (x != v[k]) {\r\n        std::cout\
    \ << \"split \" << t << \",left,\" << k << \": \" << x << \" != \" << v[k] <<\
    \ std::endl;\r\n        assert(false);\r\n        return 1;\r\n      }\r\n   \
    \   k++;\r\n    }\r\n    for (auto x : r) {\r\n      if (x != v[k]) {\r\n    \
    \    std::cout << \"split \" << t << \",right,\" << k << \": \" << x << \" !=\
    \ \" << v[k] << std::endl;\r\n        assert(false);\r\n        return 1;\r\n\
    \      }\r\n      k++;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\nint test_absorb()\
    \ {\r\n  constexpr int n = 1000;\r\n  for (int t = 0; t < 1000; t++) {\r\n   \
    \ std::set<int> _v;\r\n    while (_v.size() < n)\r\n      _v.insert(rand());\r\
    \n    std::vector<int> v(_v.begin(), _v.end());\r\n    TreapSet<int> l(v.begin(),\
    \ v.begin()+n/2);\r\n    TreapSet<int> r(v.begin()+n/2, v.end());\r\n    r.absorb(&l);\r\
    \n    int k = 0;\r\n    for (auto x : r) {\r\n      if (x != v[k]) {\r\n     \
    \   std::cout << t << \", \" << k << \": \" << x << \" != \" << v[k] << std::endl;\r\
    \n        assert(false);\r\n        return 1;\r\n      }\r\n      k++;\r\n   \
    \ }\r\n  }\r\n  return 0;\r\n}\r\n\r\nint unit_test() {\r\n  if (test_insert())\r\
    \n    return 1;\r\n  if (test_split())\r\n    return 1;\r\n  if (test_absorb())\r\
    \n    return 1;\r\n  return 0;\r\n}\r\n\r\ntemplate<typename Set>\r\nint test_set()\
    \ {\r\n  const int Max = 4e5;\r\n  std::vector<int> values;\r\n  while (values.empty())\
    \ {\r\n    for (int i = 0; i < Max; i++)\r\n      if (rand()%4 == 0)\r\n     \
    \   values.push_back(i);\r\n  }\r\n  int n = values.size();\r\n  std::vector<int>\
    \ shuffled = values;\r\n  for (int i = 0; i < n; i++) {\r\n    std::swap(shuffled[i],\
    \ shuffled[rand()%n]);\r\n  }\r\n\r\n  Set S;\r\n  for (int v : shuffled) {\r\n\
    \    S.insert(v);\r\n  }\r\n//  S.print_for_debug();\r\n  int target = -1;\r\n\
    \  int pred = -1;\r\n  int succ = values[0];\r\n  int k = 0;\r\n  auto log = [&]()\
    \ {\r\n    std::cout << pred << ' ' << target << ' ' << succ << std::endl;\r\n\
    \    assert(false);\r\n  };\r\n  for (int i = 0; i < Max; i++) {\r\n    if (k\
    \ < n and values[k] == i) {\r\n      target = values[k];\r\n      pred = k-1 >=\
    \ 0 ? values[k-1] : -1;\r\n      succ = k+1 < n ? values[k+1] : -1;\r\n      k++;\r\
    \n    } else {\r\n      pred = k-1 >= 0 ? values[k-1] : -1;\r\n      target =\
    \ -1;\r\n    }\r\n\r\n    auto fit = S.find(i);\r\n    if (fit != S.end()) {\r\
    \n      if ((int) *fit != i) {\r\n        std::cout << \"find: \" << i << std::endl;\r\
    \n        log();\r\n        return 1;\r\n      }\r\n    } else {\r\n      if (target\
    \ != -1) {\r\n        log();\r\n        return 1;\r\n      }\r\n    }\r\n\r\n\
    \    auto sucit = S.successor(i);\r\n    if (sucit != S.end()) {\r\n      if ((int)\
    \ *sucit != succ) {\r\n        std::cout << \"succ: \" << *sucit << std::endl;\r\
    \n        log();\r\n        return 1;\r\n      }\r\n    } else {\r\n      if (succ\
    \ != -1) {\r\n        log();\r\n        return 1;\r\n      }\r\n    }\r\n\r\n\
    \    auto predit = S.predecessor(i);\r\n    if (predit != S.end()) {\r\n     \
    \ if ((int) *predit != pred) {\r\n        std::cout << \"pred: \" << *predit <<\
    \ std::endl;\r\n        log();\r\n        return 1;\r\n      }\r\n    } else {\r\
    \n      if (pred != -1) {\r\n        log();\r\n        return 1;\r\n      }\r\n\
    \    }\r\n  }\r\n\r\n  int size = n;\r\n  if ((int) S.size() != size) {\r\n  \
    \  std::cout << S.size() << ' ' << size<< std::endl;\r\n    log();\r\n    return\
    \ 1;\r\n  }\r\n  for (int v : shuffled) {\r\n    S.erase(v);\r\n    size--;\r\n\
    \    if ((int) S.size() != size) {\r\n      std::cout << S.size() << ' ' << size<<\
    \ std::endl;\r\n      log();\r\n      return 1;\r\n    }\r\n  }\r\n  return 0;\r\
    \n}\r\n\r\nint main() {\r\n  if (unit_test() != 0)\r\n    return 1;\r\n  if (test_set<Treap<int>>()\
    \ != 0)\r\n    return 1;\r\n  std::cout << \"OK\" << std::endl;\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/treap_test.cpp
  requiredBy: []
  timestamp: '2022-12-19 16:09:09+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/treap_test.cpp
layout: document
redirect_from:
- /library/test/treap_test.cpp
- /library/test/treap_test.cpp.html
title: test/treap_test.cpp
---
