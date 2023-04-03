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
  bundledCode: "#line 1 \"test/fibonacci_heap_test.cpp\"\n#include <fibonacci_heap.hpp>\r\
    \n\r\n#include <vector>\r\n#include <random>\r\n#include <iostream>\r\n#include\
    \ <queue>\r\n#include <set>\r\n\r\nusing P = std::pair<int,int>;\r\nusing Fh =\
    \ FibonacciHeap<int, std::greater<>>;\r\n\r\nint main() {\r\n  int size = 1<<8;\r\
    \n  std::vector<P> num(size);\r\n  std::vector<int> V(size);\r\n  for (int i =\
    \ 0; i < size; i++) {\r\n    num[i] = {i, i};\r\n    V[i] = i;\r\n  }\r\n  for\
    \ (int i = 0; i < size; i++) {\r\n    std::swap(num[i], num[rand()%size]);\r\n\
    \  }\r\n\r\n  std::priority_queue<P, std::vector<P>, std::greater<>> pq;\r\n \
    \ Fh fh(size);\r\n  for (auto p : num) {\r\n    pq.push(p);\r\n    fh.push(p.first,\
    \ p.second);\r\n  }\r\n  std::set<int> mex;\r\n  std::vector<bool> ex(size, 1);\r\
    \n  for (int i = 0; i < size; i++) {\r\n    if (!mex.empty() and rand()%8==0)\
    \ {\r\n      int cnt = rand()%mex.size();\r\n      for (int c = 0; c < cnt; c++)\
    \ {\r\n        auto j = rand()%size;\r\n        if (ex[j]) {\r\n          auto\
    \ v = *mex.begin();\r\n          if (v < V[j]) {\r\n            mex.erase(mex.begin());\r\
    \n            pq.push({v, j});\r\n            fh.update(j, v);\r\n           \
    \ V[j] = v;\r\n          }\r\n        }\r\n      }\r\n    }\r\n    auto pqtop\
    \ = pq.top(); pq.pop();\r\n    while (pqtop.first != V[pqtop.second]) {\r\n  \
    \    pqtop = pq.top(); pq.pop();\r\n    }\r\n    auto fhtop = fh.top(); fh.pop();\r\
    \n    if (V[fhtop.first] != fhtop.second or\r\n        std::make_pair(fhtop.second,\
    \ fhtop.first) != pqtop) {\r\n      std::cout << pqtop.first << \", \" << pqtop.second\
    \ << \" != \" << fhtop.second << \", \" << fhtop.first << std::endl;\r\n     \
    \ return 1;\r\n    }\r\n    mex.insert(fhtop.second);\r\n    ex[fhtop.first] =\
    \ false;\r\n  }\r\n\r\n  std::cout << \"OK\" << std::endl;\r\n}\r\n"
  code: "#include <fibonacci_heap.hpp>\r\n\r\n#include <vector>\r\n#include <random>\r\
    \n#include <iostream>\r\n#include <queue>\r\n#include <set>\r\n\r\nusing P = std::pair<int,int>;\r\
    \nusing Fh = FibonacciHeap<int, std::greater<>>;\r\n\r\nint main() {\r\n  int\
    \ size = 1<<8;\r\n  std::vector<P> num(size);\r\n  std::vector<int> V(size);\r\
    \n  for (int i = 0; i < size; i++) {\r\n    num[i] = {i, i};\r\n    V[i] = i;\r\
    \n  }\r\n  for (int i = 0; i < size; i++) {\r\n    std::swap(num[i], num[rand()%size]);\r\
    \n  }\r\n\r\n  std::priority_queue<P, std::vector<P>, std::greater<>> pq;\r\n\
    \  Fh fh(size);\r\n  for (auto p : num) {\r\n    pq.push(p);\r\n    fh.push(p.first,\
    \ p.second);\r\n  }\r\n  std::set<int> mex;\r\n  std::vector<bool> ex(size, 1);\r\
    \n  for (int i = 0; i < size; i++) {\r\n    if (!mex.empty() and rand()%8==0)\
    \ {\r\n      int cnt = rand()%mex.size();\r\n      for (int c = 0; c < cnt; c++)\
    \ {\r\n        auto j = rand()%size;\r\n        if (ex[j]) {\r\n          auto\
    \ v = *mex.begin();\r\n          if (v < V[j]) {\r\n            mex.erase(mex.begin());\r\
    \n            pq.push({v, j});\r\n            fh.update(j, v);\r\n           \
    \ V[j] = v;\r\n          }\r\n        }\r\n      }\r\n    }\r\n    auto pqtop\
    \ = pq.top(); pq.pop();\r\n    while (pqtop.first != V[pqtop.second]) {\r\n  \
    \    pqtop = pq.top(); pq.pop();\r\n    }\r\n    auto fhtop = fh.top(); fh.pop();\r\
    \n    if (V[fhtop.first] != fhtop.second or\r\n        std::make_pair(fhtop.second,\
    \ fhtop.first) != pqtop) {\r\n      std::cout << pqtop.first << \", \" << pqtop.second\
    \ << \" != \" << fhtop.second << \", \" << fhtop.first << std::endl;\r\n     \
    \ return 1;\r\n    }\r\n    mex.insert(fhtop.second);\r\n    ex[fhtop.first] =\
    \ false;\r\n  }\r\n\r\n  std::cout << \"OK\" << std::endl;\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/fibonacci_heap_test.cpp
  requiredBy: []
  timestamp: '2023-04-03 03:00:14+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/fibonacci_heap_test.cpp
layout: document
redirect_from:
- /library/test/fibonacci_heap_test.cpp
- /library/test/fibonacci_heap_test.cpp.html
title: test/fibonacci_heap_test.cpp
---
