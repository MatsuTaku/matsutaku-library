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
  bundledCode: "#line 1 \"test/integer_set_test.cpp\"\n#include <integer_set.hpp>\r\
    \n#line 1 \"test/set_test.hpp\"\n#include <iostream>\r\n#include <vector>\r\n\
    #include <algorithm>\r\n#include <type_traits>\r\n#include <cassert>\r\n#include\
    \ <set>\r\n\r\nnamespace mtl {\r\n\r\nusing std::cout;\r\nusing std::cerr;\r\n\
    using std::endl;\r\n\r\ntemplate<typename Set, int Max = (int)4e5>\r\nvoid integer_set_test()\
    \ {\r\n  std::vector<int> values;\r\n  while (values.empty()) {\r\n    for (int\
    \ i = 0; i < Max; i++)\r\n      if (rand()%4 == 0)\r\n        values.push_back(i);\r\
    \n  }\r\n  int n = values.size();\r\n  auto shuffled = values;\r\n  std::random_shuffle(shuffled.begin(),\
    \ shuffled.end());\r\n\r\n  Set S;\r\n  for (auto v : shuffled) {\r\n    S.insert(v);\r\
    \n  }\r\n\r\n  if (values != std::vector<int>(S.begin(), S.end())) {\r\n    cout\
    \ << \"after insert order broken\" << endl;\r\n    exit(EXIT_FAILURE);\r\n  }\r\
    \n\r\n//  S.print_for_debug();\r\n  int target = -1;\r\n  int pred = -1;\r\n \
    \ int succ = values[0];\r\n  int k = 0;\r\n  auto log = [&]() {\r\n    std::cout\
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
    \    exit(EXIT_FAILURE);\r\n  }\r\n\r\n  for (int v : shuffled) {\r\n    auto\
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
    \ std::endl;\r\n      exit(EXIT_FAILURE);\r\n    }\r\n  }\r\n\r\n}\r\n\r\n}\r\n\
    #line 3 \"test/integer_set_test.cpp\"\n\r\nconstexpr int W = 20;\r\n//constexpr\
    \ int Max = 4e5;\r\n//\r\n//template<typename Set>\r\n//int test_set() {\r\n//\
    \  std::vector<int> values;\r\n//  while (values.empty()) {\r\n//    for (int\
    \ i = 0; i < Max; i++) {\r\n//      if (i == 0 or rand()%4 == 0)\r\n//       \
    \ values.push_back(i);\r\n//    }\r\n//  }\r\n//  int n = values.size();\r\n//\
    \  std::vector<int> shuffled = values;\r\n//  for (int i = 0; i < n; i++) {\r\n\
    //    std::swap(shuffled[i], shuffled[rand()%n]);\r\n//  }\r\n//\r\n//  Set S;\r\
    \n//  for (int v : shuffled) {\r\n//    S.insert(v);\r\n//  }\r\n////  S.print_for_debug();\r\
    \n//  int target = -1;\r\n//  int pred = -1;\r\n//  int succ = values[0];\r\n\
    //  int k = 0;\r\n//  auto log = [&]() {\r\n//    std::cout << pred << ' ' <<\
    \ target << ' ' << succ << std::endl;\r\n//    assert(false);\r\n//  };\r\n//\
    \  for (int i = 0; i < Max; i++) {\r\n//    if (k < n and values[k] == i) {\r\n\
    //      target = values[k];\r\n//      pred = k-1 >= 0 ? values[k-1] : -1;\r\n\
    //      succ = k+1 < n ? values[k+1] : -1;\r\n//      k++;\r\n//    } else {\r\
    \n//      pred = k-1 >= 0 ? values[k-1] : -1;\r\n//      target = -1;\r\n//  \
    \  }\r\n//\r\n//    auto fit = S.find(i);\r\n//    if (fit != S.end()) {\r\n//\
    \      if ((int) *fit != i) {\r\n//        std::cout << \"find: \" << i << std::endl;\r\
    \n//        log();\r\n//        return 1;\r\n//      }\r\n//    } else {\r\n//\
    \      if (target != -1) {\r\n//        log();\r\n//        return 1;\r\n//  \
    \    }\r\n//    }\r\n//\r\n//    auto sucit = S.successor(i);\r\n//    if (sucit\
    \ != S.end()) {\r\n//      if ((int) *sucit != succ) {\r\n//        std::cout\
    \ << \"succ: \" << *sucit << std::endl;\r\n//        log();\r\n//        return\
    \ 1;\r\n//      }\r\n//    } else {\r\n//      if (succ != -1) {\r\n//       \
    \ log();\r\n//        return 1;\r\n//      }\r\n//    }\r\n//\r\n//    auto predit\
    \ = S.predecessor(i);\r\n//    if (predit != S.end()) {\r\n//      if ((int) *predit\
    \ != pred) {\r\n//        std::cout << \"pred: \" << *predit << std::endl;\r\n\
    //        log();\r\n//        return 1;\r\n//      }\r\n//    } else {\r\n// \
    \     if (pred != -1) {\r\n//        log();\r\n//        return 1;\r\n//     \
    \ }\r\n//    }\r\n//  }\r\n//\r\n//  int size = n;\r\n//  if ((int) S.size() !=\
    \ size) {\r\n//    std::cout << S.size() << ' ' << size<< std::endl;\r\n//   \
    \ log();\r\n//    return 1;\r\n//  }\r\n//  for (int v : shuffled) {\r\n//   \
    \ S.erase(v);\r\n//    size--;\r\n//    if ((int) S.size() != size) {\r\n//  \
    \    std::cout << S.size() << ' ' << size<< std::endl;\r\n//      assert(false);\r\
    \n//      return 1;\r\n//    }\r\n//  }\r\n//  return 0;\r\n//}\r\n\r\nint main()\
    \ {\r\n  mtl::integer_set_test<XFastTrieSet<W>>();\r\n  mtl::integer_set_test<YFastTrieSet<W>>();\r\
    \n  std::cout << \"OK\" << std::endl;\r\n}\r\n"
  code: "#include <integer_set.hpp>\r\n#include \"set_test.hpp\"\r\n\r\nconstexpr\
    \ int W = 20;\r\n//constexpr int Max = 4e5;\r\n//\r\n//template<typename Set>\r\
    \n//int test_set() {\r\n//  std::vector<int> values;\r\n//  while (values.empty())\
    \ {\r\n//    for (int i = 0; i < Max; i++) {\r\n//      if (i == 0 or rand()%4\
    \ == 0)\r\n//        values.push_back(i);\r\n//    }\r\n//  }\r\n//  int n = values.size();\r\
    \n//  std::vector<int> shuffled = values;\r\n//  for (int i = 0; i < n; i++) {\r\
    \n//    std::swap(shuffled[i], shuffled[rand()%n]);\r\n//  }\r\n//\r\n//  Set\
    \ S;\r\n//  for (int v : shuffled) {\r\n//    S.insert(v);\r\n//  }\r\n////  S.print_for_debug();\r\
    \n//  int target = -1;\r\n//  int pred = -1;\r\n//  int succ = values[0];\r\n\
    //  int k = 0;\r\n//  auto log = [&]() {\r\n//    std::cout << pred << ' ' <<\
    \ target << ' ' << succ << std::endl;\r\n//    assert(false);\r\n//  };\r\n//\
    \  for (int i = 0; i < Max; i++) {\r\n//    if (k < n and values[k] == i) {\r\n\
    //      target = values[k];\r\n//      pred = k-1 >= 0 ? values[k-1] : -1;\r\n\
    //      succ = k+1 < n ? values[k+1] : -1;\r\n//      k++;\r\n//    } else {\r\
    \n//      pred = k-1 >= 0 ? values[k-1] : -1;\r\n//      target = -1;\r\n//  \
    \  }\r\n//\r\n//    auto fit = S.find(i);\r\n//    if (fit != S.end()) {\r\n//\
    \      if ((int) *fit != i) {\r\n//        std::cout << \"find: \" << i << std::endl;\r\
    \n//        log();\r\n//        return 1;\r\n//      }\r\n//    } else {\r\n//\
    \      if (target != -1) {\r\n//        log();\r\n//        return 1;\r\n//  \
    \    }\r\n//    }\r\n//\r\n//    auto sucit = S.successor(i);\r\n//    if (sucit\
    \ != S.end()) {\r\n//      if ((int) *sucit != succ) {\r\n//        std::cout\
    \ << \"succ: \" << *sucit << std::endl;\r\n//        log();\r\n//        return\
    \ 1;\r\n//      }\r\n//    } else {\r\n//      if (succ != -1) {\r\n//       \
    \ log();\r\n//        return 1;\r\n//      }\r\n//    }\r\n//\r\n//    auto predit\
    \ = S.predecessor(i);\r\n//    if (predit != S.end()) {\r\n//      if ((int) *predit\
    \ != pred) {\r\n//        std::cout << \"pred: \" << *predit << std::endl;\r\n\
    //        log();\r\n//        return 1;\r\n//      }\r\n//    } else {\r\n// \
    \     if (pred != -1) {\r\n//        log();\r\n//        return 1;\r\n//     \
    \ }\r\n//    }\r\n//  }\r\n//\r\n//  int size = n;\r\n//  if ((int) S.size() !=\
    \ size) {\r\n//    std::cout << S.size() << ' ' << size<< std::endl;\r\n//   \
    \ log();\r\n//    return 1;\r\n//  }\r\n//  for (int v : shuffled) {\r\n//   \
    \ S.erase(v);\r\n//    size--;\r\n//    if ((int) S.size() != size) {\r\n//  \
    \    std::cout << S.size() << ' ' << size<< std::endl;\r\n//      assert(false);\r\
    \n//      return 1;\r\n//    }\r\n//  }\r\n//  return 0;\r\n//}\r\n\r\nint main()\
    \ {\r\n  mtl::integer_set_test<XFastTrieSet<W>>();\r\n  mtl::integer_set_test<YFastTrieSet<W>>();\r\
    \n  std::cout << \"OK\" << std::endl;\r\n}\r\n"
  dependsOn:
  - test/set_test.hpp
  isVerificationFile: false
  path: test/integer_set_test.cpp
  requiredBy: []
  timestamp: '2022-12-21 13:24:57+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/integer_set_test.cpp
layout: document
redirect_from:
- /library/test/integer_set_test.cpp
- /library/test/integer_set_test.cpp.html
title: test/integer_set_test.cpp
---
