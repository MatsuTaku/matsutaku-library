---
data:
  _extendedDependsOn:
  - icon: ':warning:'
    path: test/set_test.hpp
    title: test/set_test.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 1 \"test/treap_test.cpp\"\n#include <treap.hpp>\r\n\r\n#include\
    \ <iostream>\r\n#include <vector>\r\n#include <set>\r\n#include <cassert>\r\n\
    #include <algorithm>\r\n#line 4 \"test/set_test.hpp\"\n#include <type_traits>\r\
    \n#line 7 \"test/set_test.hpp\"\n\r\nnamespace mtl {\r\n\r\nusing std::cout;\r\
    \nusing std::cerr;\r\nusing std::endl;\r\n\r\ntemplate<typename Set, int Max =\
    \ (int)4e5, bool Shuffle = true>\r\nvoid integer_set_test() {\r\n  std::vector<int>\
    \ values;\r\n  while (values.empty()) {\r\n    for (int i = 0; i < Max; i++)\r\
    \n      if (rand()%4 == 0)\r\n        values.push_back(i);\r\n  }\r\n  int n =\
    \ values.size();\r\n  auto insertions = values;\r\n  if constexpr (Shuffle)\r\n\
    \    std::random_shuffle(insertions.begin(), insertions.end());\r\n\r\n  Set S(insertions.begin(),\
    \ insertions.end());\r\n\r\n  if (values != std::vector<int>(S.begin(), S.end()))\
    \ {\r\n    cout << \"after insert order broken\" << endl;\r\n    exit(EXIT_FAILURE);\r\
    \n  }\r\n\r\n//  S.print_for_debug();\r\n  int target = -1;\r\n  int pred = -1;\r\
    \n  int succ = values[0];\r\n  int k = 0;\r\n  auto log = [&]() {\r\n    std::cout\
    \ << pred << ' ' << target << ' ' << succ << std::endl;\r\n  };\r\n  for (int\
    \ i = 0; i < Max; i++) {\r\n    if (k < n and values[k] == i) {\r\n      target\
    \ = values[k];\r\n      pred = k-1 >= 0 ? values[k-1] : -1;\r\n      succ = k+1\
    \ < n ? values[k+1] : -1;\r\n      k++;\r\n    } else {\r\n      pred = k-1 >=\
    \ 0 ? values[k-1] : -1;\r\n      target = -1;\r\n    }\r\n\r\n    auto fit = S.find(i);\r\
    \n    if (fit != S.end()) {\r\n      if ((int)*fit != i) {\r\n        std::cout\
    \ << \"find: \" << i << std::endl;\r\n        log();\r\n        exit(EXIT_FAILURE);\r\
    \n      }\r\n    } else {\r\n      if (target != -1) {\r\n        log();\r\n \
    \       exit(EXIT_FAILURE);\r\n      }\r\n    }\r\n\r\n    auto sucit = S.upper_bound(i);\r\
    \n    if (sucit != S.end()) {\r\n      if ((int)*sucit != succ) {\r\n        std::cout\
    \ << \"succ: \" << *sucit << std::endl;\r\n        log();\r\n        exit(EXIT_FAILURE);\r\
    \n      }\r\n    } else {\r\n      if (succ != -1) {\r\n        log();\r\n   \
    \     exit(EXIT_FAILURE);\r\n      }\r\n    }\r\n\r\n    auto predit = S.lower_bound(i);\r\
    \n    if (predit != S.begin()) {\r\n      --predit;\r\n      if ((int)*predit\
    \ != pred) {\r\n        std::cout << \"pred: \" << *predit << std::endl;\r\n \
    \       log();\r\n        exit(EXIT_FAILURE);\r\n      }\r\n    } else {\r\n \
    \     if (pred != -1) {\r\n        log();\r\n        exit(EXIT_FAILURE);\r\n \
    \     }\r\n    }\r\n  }\r\n\r\n  int size = n;\r\n  if ((int) S.size() != size)\
    \ {\r\n    std::cout << S.size() << ' ' << size<< std::endl;\r\n    log();\r\n\
    \    exit(EXIT_FAILURE);\r\n  }\r\n\r\n  for (int v : insertions) {\r\n    auto\
    \ f = S.find(v);\r\n    assert(f != S.end());\r\n    auto p = f;\r\n    auto m\
    \ = std::next(f);\r\n    for (int i = 0; i < 2 and p != S.begin(); i++)\r\n  \
    \    --p;\r\n    for (int i = 0; i < 2 and m != S.end(); i++)\r\n      ++m;\r\n\
    \    std::vector<unsigned> o(p,m);\r\n    o.erase(find(o.begin(), o.end(), v));\r\
    \n    S.erase(v);\r\n    size--;\r\n\r\n    {\r\n      auto lb = S.lower_bound(v);\r\
    \n      auto p = lb, m = lb;\r\n      for (int i = 0; i < 2 and p != S.begin();\
    \ i++)\r\n        --p;\r\n      for (int i = 0; i < 2 and m != S.end(); i++)\r\
    \n        ++m;\r\n      if (o != std::vector<unsigned>(p,m)) {\r\n        std::cout\
    \ << n-size<<\" after erase \"<<v<<\" order broken \" << endl;\r\n        for\
    \ (auto v:o)\r\n          cerr<<v<<' ';\r\n        cerr<<endl;\r\n        for\
    \ (auto it = p; it != m; ++it) {\r\n          cerr<<*it<<' ';\r\n        }\r\n\
    \        cerr<<endl;\r\n        exit(EXIT_FAILURE);\r\n      }\r\n    }\r\n  \
    \  if ((int) S.size() != size) {\r\n      std::cout << S.size() << ' ' << size<<\
    \ std::endl;\r\n      exit(EXIT_FAILURE);\r\n    }\r\n  }\r\n  cerr<<\"integer_set_test\
    \ ok\"<<endl;\r\n}\r\n\r\n}\r\n#line 9 \"test/treap_test.cpp\"\n\r\nvoid test_insert()\
    \ {\r\n  constexpr int n = 300;\r\n  for (int t = 0; t < 300; t++) {\r\n    std::set<int>\
    \ _v{0,\r\n                     std::numeric_limits<int>::max(),\r\n         \
    \            std::numeric_limits<int>::min()};\r\n    while (_v.size() < n)\r\n\
    \      _v.insert(rand());\r\n    std::vector<int> v(_v.begin(), _v.end());\r\n\
    \    TreapSet<int> s;\r\n    for (int i = 0; i < n; i++) {\r\n      s.insert(v[i]);\r\
    \n      if ((int)s.size() != i+1) {\r\n        std::cout << \"insert \" << t <<\
    \ \",\" << i << \": \" << s.size() << \" < \" << i+1 << std::endl;\r\n       \
    \ assert(false);\r\n        exit(EXIT_FAILURE);\r\n      }\r\n      int k = 0;\r\
    \n      for (auto x : s) {\r\n        if (x != v[k]) {\r\n          std::cout\
    \ << \"insert \" << t << \",\" << i << ' ' << k << \": \" << x << \" != \" <<\
    \ v[k] << std::endl;\r\n          assert(false);\r\n          exit(EXIT_FAILURE);\r\
    \n        }\r\n        k++;\r\n      }\r\n    }\r\n  }\r\n}\r\n\r\nvoid test_split()\
    \ {\r\n  constexpr int n = 1000;\r\n  for (int t = 0; t < 1000; t++) {\r\n   \
    \ std::set<int> _v;\r\n    while (_v.size() < n)\r\n      _v.insert(rand());\r\
    \n    std::vector<int> v(_v.begin(), _v.end());\r\n    TreapSet<int> r(v.begin(),\
    \ v.end());\r\n    auto l = r.split(std::next(r.begin(), n/2));\r\n    int k =\
    \ 0;\r\n    for (auto x : l) {\r\n      if (x != v[k]) {\r\n        std::cout\
    \ << \"split \" << t << \",left,\" << k << \": \" << x << \" != \" << v[k] <<\
    \ std::endl;\r\n        assert(false);\r\n        exit(EXIT_FAILURE);\r\n    \
    \  }\r\n      k++;\r\n    }\r\n    for (auto x : r) {\r\n      if (x != v[k])\
    \ {\r\n        std::cout << \"split \" << t << \",right,\" << k << \": \" << x\
    \ << \" != \" << v[k] << std::endl;\r\n        assert(false);\r\n        exit(EXIT_FAILURE);\r\
    \n      }\r\n      k++;\r\n    }\r\n  }\r\n}\r\n\r\nvoid test_absorb() {\r\n \
    \ constexpr int n = 1000;\r\n  for (int t = 0; t < 1000; t++) {\r\n    std::set<int>\
    \ _v;\r\n    while (_v.size() < n)\r\n      _v.insert(rand());\r\n    std::vector<int>\
    \ v(_v.begin(), _v.end());\r\n    TreapSet<int> l(v.begin(), v.begin()+n/2);\r\
    \n    TreapSet<int> r(v.begin()+n/2, v.end());\r\n    r.absorb(&l);\r\n    int\
    \ k = 0;\r\n    for (auto x : r) {\r\n      if (x != v[k]) {\r\n        std::cout\
    \ << t << \", \" << k << \": \" << x << \" != \" << v[k] << std::endl;\r\n   \
    \     assert(false);\r\n        exit(EXIT_FAILURE);\r\n      }\r\n      k++;\r\
    \n    }\r\n  }\r\n}\r\n\r\nvoid unit_test() {\r\n  test_insert();\r\n  test_split();\r\
    \n  test_absorb();\r\n}\r\n\r\nint main() {\r\n  unit_test();\r\n  mtl::integer_set_test<Treap<int>>();\r\
    \n  std::cout << \"OK\" << std::endl;\r\n}\r\n"
  code: "#include <treap.hpp>\r\n\r\n#include <iostream>\r\n#include <vector>\r\n\
    #include <set>\r\n#include <cassert>\r\n#include <algorithm>\r\n#include \"set_test.hpp\"\
    \r\n\r\nvoid test_insert() {\r\n  constexpr int n = 300;\r\n  for (int t = 0;\
    \ t < 300; t++) {\r\n    std::set<int> _v{0,\r\n                     std::numeric_limits<int>::max(),\r\
    \n                     std::numeric_limits<int>::min()};\r\n    while (_v.size()\
    \ < n)\r\n      _v.insert(rand());\r\n    std::vector<int> v(_v.begin(), _v.end());\r\
    \n    TreapSet<int> s;\r\n    for (int i = 0; i < n; i++) {\r\n      s.insert(v[i]);\r\
    \n      if ((int)s.size() != i+1) {\r\n        std::cout << \"insert \" << t <<\
    \ \",\" << i << \": \" << s.size() << \" < \" << i+1 << std::endl;\r\n       \
    \ assert(false);\r\n        exit(EXIT_FAILURE);\r\n      }\r\n      int k = 0;\r\
    \n      for (auto x : s) {\r\n        if (x != v[k]) {\r\n          std::cout\
    \ << \"insert \" << t << \",\" << i << ' ' << k << \": \" << x << \" != \" <<\
    \ v[k] << std::endl;\r\n          assert(false);\r\n          exit(EXIT_FAILURE);\r\
    \n        }\r\n        k++;\r\n      }\r\n    }\r\n  }\r\n}\r\n\r\nvoid test_split()\
    \ {\r\n  constexpr int n = 1000;\r\n  for (int t = 0; t < 1000; t++) {\r\n   \
    \ std::set<int> _v;\r\n    while (_v.size() < n)\r\n      _v.insert(rand());\r\
    \n    std::vector<int> v(_v.begin(), _v.end());\r\n    TreapSet<int> r(v.begin(),\
    \ v.end());\r\n    auto l = r.split(std::next(r.begin(), n/2));\r\n    int k =\
    \ 0;\r\n    for (auto x : l) {\r\n      if (x != v[k]) {\r\n        std::cout\
    \ << \"split \" << t << \",left,\" << k << \": \" << x << \" != \" << v[k] <<\
    \ std::endl;\r\n        assert(false);\r\n        exit(EXIT_FAILURE);\r\n    \
    \  }\r\n      k++;\r\n    }\r\n    for (auto x : r) {\r\n      if (x != v[k])\
    \ {\r\n        std::cout << \"split \" << t << \",right,\" << k << \": \" << x\
    \ << \" != \" << v[k] << std::endl;\r\n        assert(false);\r\n        exit(EXIT_FAILURE);\r\
    \n      }\r\n      k++;\r\n    }\r\n  }\r\n}\r\n\r\nvoid test_absorb() {\r\n \
    \ constexpr int n = 1000;\r\n  for (int t = 0; t < 1000; t++) {\r\n    std::set<int>\
    \ _v;\r\n    while (_v.size() < n)\r\n      _v.insert(rand());\r\n    std::vector<int>\
    \ v(_v.begin(), _v.end());\r\n    TreapSet<int> l(v.begin(), v.begin()+n/2);\r\
    \n    TreapSet<int> r(v.begin()+n/2, v.end());\r\n    r.absorb(&l);\r\n    int\
    \ k = 0;\r\n    for (auto x : r) {\r\n      if (x != v[k]) {\r\n        std::cout\
    \ << t << \", \" << k << \": \" << x << \" != \" << v[k] << std::endl;\r\n   \
    \     assert(false);\r\n        exit(EXIT_FAILURE);\r\n      }\r\n      k++;\r\
    \n    }\r\n  }\r\n}\r\n\r\nvoid unit_test() {\r\n  test_insert();\r\n  test_split();\r\
    \n  test_absorb();\r\n}\r\n\r\nint main() {\r\n  unit_test();\r\n  mtl::integer_set_test<Treap<int>>();\r\
    \n  std::cout << \"OK\" << std::endl;\r\n}\r\n"
  dependsOn:
  - test/set_test.hpp
  isVerificationFile: false
  path: test/treap_test.cpp
  requiredBy: []
  timestamp: '2022-12-21 18:43:34+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/treap_test.cpp
layout: document
redirect_from:
- /library/test/treap_test.cpp
- /library/test/treap_test.cpp.html
title: test/treap_test.cpp
---
