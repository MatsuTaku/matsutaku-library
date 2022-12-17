---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/unionfind.test.cpp
    title: test/unionfind.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/unionfind.hpp\"\n#include <vector>\n#include\
    \ <numeric>\n#include <cstddef>\n\nclass UnionFind {\n private:\n  // par_[i]\
    \ = { size if par_[i] < 0, root otherwise }\n  std::vector<int> par_;\n\n public:\n\
    \  UnionFind() = default;\n  explicit UnionFind(size_t size) : par_(size, -1)\
    \ {}\n\n  int root(int u) {\n    if (par_[u] < 0)\n      return u;\n    return\
    \ par_[u] = root(par_[u]);\n  }\n\n  bool are_union(int u, int v) {\n    return\
    \ root(u) == root(v);\n  }\n\n  int size_of(int u) {\n    return -par_[root(u)];\n\
    \  }\n\n  bool unite(int u, int v) {\n    if (are_union(u,v))\n      return false;\n\
    \    if (size_of(u) < size_of(v))\n      std::swap(u,v);\n    par_[root(u)] -=\
    \ size_of(v);\n    par_[root(v)] = root(u);\n    return true;\n  }\n};\n"
  code: "#pragma once\n#include <vector>\n#include <numeric>\n#include <cstddef>\n\
    \nclass UnionFind {\n private:\n  // par_[i] = { size if par_[i] < 0, root otherwise\
    \ }\n  std::vector<int> par_;\n\n public:\n  UnionFind() = default;\n  explicit\
    \ UnionFind(size_t size) : par_(size, -1) {}\n\n  int root(int u) {\n    if (par_[u]\
    \ < 0)\n      return u;\n    return par_[u] = root(par_[u]);\n  }\n\n  bool are_union(int\
    \ u, int v) {\n    return root(u) == root(v);\n  }\n\n  int size_of(int u) {\n\
    \    return -par_[root(u)];\n  }\n\n  bool unite(int u, int v) {\n    if (are_union(u,v))\n\
    \      return false;\n    if (size_of(u) < size_of(v))\n      std::swap(u,v);\n\
    \    par_[root(u)] -= size_of(v);\n    par_[root(v)] = root(u);\n    return true;\n\
    \  }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/unionfind.hpp
  requiredBy: []
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/unionfind.test.cpp
documentation_of: include/mtl/unionfind.hpp
layout: document
redirect_from:
- /library/include/mtl/unionfind.hpp
- /library/include/mtl/unionfind.hpp.html
title: include/mtl/unionfind.hpp
---
