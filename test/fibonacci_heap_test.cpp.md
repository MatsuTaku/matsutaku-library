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
  bundledCode: "#line 1 \"test/fibonacci_heap_test.cpp\"\n#include <fibonacci_heap.hpp>\n\
    \n#include <vector>\n#include <random>\n#include <iostream>\n#include <queue>\n\
    #include <set>\n\nusing P = std::pair<int,int>;\nusing Fh = FibonacciHeap<int,\
    \ std::greater<>>;\n\nint main() {\n  int size = 1<<8;\n  std::vector<P> num(size);\n\
    \  std::vector<int> V(size);\n  for (int i = 0; i < size; i++) {\n    num[i] =\
    \ {i, i};\n    V[i] = i;\n  }\n  for (int i = 0; i < size; i++) {\n    std::swap(num[i],\
    \ num[rand()%size]);\n  }\n\n  std::priority_queue<P, std::vector<P>, std::greater<>>\
    \ pq;\n  Fh fh(size);\n  for (auto p : num) {\n    pq.push(p);\n    fh.push(p.first,\
    \ p.second);\n  }\n  std::set<int> mex;\n  std::vector<bool> ex(size, 1);\n  for\
    \ (int i = 0; i < size; i++) {\n    if (!mex.empty() and rand()%8==0) {\n    \
    \  int cnt = rand()%mex.size();\n      for (int c = 0; c < cnt; c++) {\n     \
    \   auto j = rand()%size;\n        if (ex[j]) {\n          auto v = *mex.begin();\n\
    \          if (v < V[j]) {\n            mex.erase(mex.begin());\n            pq.push({v,\
    \ j});\n            fh.update(j, v);\n            V[j] = v;\n          }\n   \
    \     }\n      }\n    }\n    auto pqtop = pq.top(); pq.pop();\n    while (pqtop.first\
    \ != V[pqtop.second]) {\n      pqtop = pq.top(); pq.pop();\n    }\n    auto fhtop\
    \ = fh.top(); fh.pop();\n    if (V[fhtop.first] != fhtop.second or\n        std::make_pair(fhtop.second,\
    \ fhtop.first) != pqtop) {\n      std::cout << pqtop.first << \", \" << pqtop.second\
    \ << \" != \" << fhtop.second << \", \" << fhtop.first << std::endl;\n      return\
    \ 1;\n    }\n    mex.insert(fhtop.second);\n    ex[fhtop.first] = false;\n  }\n\
    \n  std::cout << \"OK\" << std::endl;\n}\n"
  code: "#include <fibonacci_heap.hpp>\n\n#include <vector>\n#include <random>\n#include\
    \ <iostream>\n#include <queue>\n#include <set>\n\nusing P = std::pair<int,int>;\n\
    using Fh = FibonacciHeap<int, std::greater<>>;\n\nint main() {\n  int size = 1<<8;\n\
    \  std::vector<P> num(size);\n  std::vector<int> V(size);\n  for (int i = 0; i\
    \ < size; i++) {\n    num[i] = {i, i};\n    V[i] = i;\n  }\n  for (int i = 0;\
    \ i < size; i++) {\n    std::swap(num[i], num[rand()%size]);\n  }\n\n  std::priority_queue<P,\
    \ std::vector<P>, std::greater<>> pq;\n  Fh fh(size);\n  for (auto p : num) {\n\
    \    pq.push(p);\n    fh.push(p.first, p.second);\n  }\n  std::set<int> mex;\n\
    \  std::vector<bool> ex(size, 1);\n  for (int i = 0; i < size; i++) {\n    if\
    \ (!mex.empty() and rand()%8==0) {\n      int cnt = rand()%mex.size();\n     \
    \ for (int c = 0; c < cnt; c++) {\n        auto j = rand()%size;\n        if (ex[j])\
    \ {\n          auto v = *mex.begin();\n          if (v < V[j]) {\n           \
    \ mex.erase(mex.begin());\n            pq.push({v, j});\n            fh.update(j,\
    \ v);\n            V[j] = v;\n          }\n        }\n      }\n    }\n    auto\
    \ pqtop = pq.top(); pq.pop();\n    while (pqtop.first != V[pqtop.second]) {\n\
    \      pqtop = pq.top(); pq.pop();\n    }\n    auto fhtop = fh.top(); fh.pop();\n\
    \    if (V[fhtop.first] != fhtop.second or\n        std::make_pair(fhtop.second,\
    \ fhtop.first) != pqtop) {\n      std::cout << pqtop.first << \", \" << pqtop.second\
    \ << \" != \" << fhtop.second << \", \" << fhtop.first << std::endl;\n      return\
    \ 1;\n    }\n    mex.insert(fhtop.second);\n    ex[fhtop.first] = false;\n  }\n\
    \n  std::cout << \"OK\" << std::endl;\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: test/fibonacci_heap_test.cpp
  requiredBy: []
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/fibonacci_heap_test.cpp
layout: document
redirect_from:
- /library/test/fibonacci_heap_test.cpp
- /library/test/fibonacci_heap_test.cpp.html
title: test/fibonacci_heap_test.cpp
---
