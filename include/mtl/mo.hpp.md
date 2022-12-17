---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/static_range_inversions_query.test.cpp
    title: test/static_range_inversions_query.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/mo.hpp\"\n#include <algorithm>\r\n#include <cmath>\r\
    \n#include <tuple>\r\n#include <vector>\r\n\r\ntemplate <typename T, T (*pushl)(T,\
    \ int), T (*pushr)(T, int), T (*popl)(T, int),\r\n          T (*popr)(T, int),\
    \ T (*e)()>\r\nclass MoSolver {\r\n private:\r\n  std::vector<std::tuple<int,\
    \ int, int>> segs;\r\n  int n = 0;\r\n\r\n public:\r\n  MoSolver() = default;\r\
    \n  void add_segment(int l, int r) { segs.emplace_back(n++, l, r); }\r\n  void\
    \ solve(std::vector<T> &dst) {\r\n    using std::get;\r\n    sort(segs.begin(),\
    \ segs.end(), [m = (int)std::sqrt(n)](auto &l, auto &r) {\r\n      int ll = get<1>(l),\
    \ rl = get<2>(l), lr = get<1>(r), rr = get<2>(r);\r\n      int bl = ll / m;\r\n\
    \      int br = lr / m;\r\n      return (bl != br) ? bl < br : (rl < rr) ^ (bl\
    \ % 2);\r\n    });\r\n    dst.resize(n);\r\n    int _l = 0, _r = 0;\r\n    T val\
    \ = e();\r\n    for (auto &ilr : segs) {\r\n      int i = get<0>(ilr), l = get<1>(ilr),\
    \ r = get<2>(ilr);\r\n      while (l < _l)\r\n        val = pushl(val, --_l);\r\
    \n      while (_r < r)\r\n        val = pushr(val, _r++);\r\n      while (_l <\
    \ l)\r\n        val = popl(val, _l++);\r\n      while (r < _r)\r\n        val\
    \ = popr(val, --_r);\r\n      dst[i] = val;\r\n    }\r\n  }\r\n};\r\n"
  code: "#pragma once\r\n#include <algorithm>\r\n#include <cmath>\r\n#include <tuple>\r\
    \n#include <vector>\r\n\r\ntemplate <typename T, T (*pushl)(T, int), T (*pushr)(T,\
    \ int), T (*popl)(T, int),\r\n          T (*popr)(T, int), T (*e)()>\r\nclass\
    \ MoSolver {\r\n private:\r\n  std::vector<std::tuple<int, int, int>> segs;\r\n\
    \  int n = 0;\r\n\r\n public:\r\n  MoSolver() = default;\r\n  void add_segment(int\
    \ l, int r) { segs.emplace_back(n++, l, r); }\r\n  void solve(std::vector<T> &dst)\
    \ {\r\n    using std::get;\r\n    sort(segs.begin(), segs.end(), [m = (int)std::sqrt(n)](auto\
    \ &l, auto &r) {\r\n      int ll = get<1>(l), rl = get<2>(l), lr = get<1>(r),\
    \ rr = get<2>(r);\r\n      int bl = ll / m;\r\n      int br = lr / m;\r\n    \
    \  return (bl != br) ? bl < br : (rl < rr) ^ (bl % 2);\r\n    });\r\n    dst.resize(n);\r\
    \n    int _l = 0, _r = 0;\r\n    T val = e();\r\n    for (auto &ilr : segs) {\r\
    \n      int i = get<0>(ilr), l = get<1>(ilr), r = get<2>(ilr);\r\n      while\
    \ (l < _l)\r\n        val = pushl(val, --_l);\r\n      while (_r < r)\r\n    \
    \    val = pushr(val, _r++);\r\n      while (_l < l)\r\n        val = popl(val,\
    \ _l++);\r\n      while (r < _r)\r\n        val = popr(val, --_r);\r\n      dst[i]\
    \ = val;\r\n    }\r\n  }\r\n};\r\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/mo.hpp
  requiredBy: []
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/static_range_inversions_query.test.cpp
documentation_of: include/mtl/mo.hpp
layout: document
redirect_from:
- /library/include/mtl/mo.hpp
- /library/include/mtl/mo.hpp.html
title: include/mtl/mo.hpp
---
