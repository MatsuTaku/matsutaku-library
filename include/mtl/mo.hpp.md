---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/yosupo/static_range_inversions_query-mo.test.cpp
    title: test/yosupo/static_range_inversions_query-mo.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    document_title: 'Mo''s algorithm: solve offline segment queries on a sequence'
    links:
    - https://noshi91.hatenablog.com/entry/2023/04/13/224811
  bundledCode: "#line 2 \"include/mtl/mo.hpp\"\n#include <algorithm>\r\n#include <numeric>\r\
    \n#include <cmath>\r\n#include <tuple>\r\n#include <vector>\r\n#include <iostream>\r\
    \n\r\n/**\r\n * @brief Mo's algorithm: solve offline segment queries on a sequence\r\
    \n * @note  This implementation is optimized by noshi's idea\r\n *          complexity\
    \ of N sqrt(Q) + O(N).\r\n *        - \u5B9A\u6570\u500D\u304C\u6700\u9069\u306A\
    \ Mo's Algorithm. noshi91\u306E\u30E1\u30E2. 2023/04/13.\r\n *          https://noshi91.hatenablog.com/entry/2023/04/13/224811\r\
    \n*/\r\ntemplate <typename T, \r\n          T (*pushl)(T, int), \r\n         \
    \ T (*pushr)(T, int), \r\n          T (*popl)(T, int),\r\n          T (*popr)(T,\
    \ int), \r\n          T (*e)()>\r\nclass MoSolver {\r\n private:\r\n  std::vector<std::tuple<int,\
    \ int, int>> segs;\r\n  int q = 0;\r\n\r\n public:\r\n  void add_segment(int l,\
    \ int r) { \r\n    segs.emplace_back(q++, l, r);\r\n  }\r\n  void calc_mos_move(std::vector<int>&\
    \ dst) {\r\n    using std::get;\r\n    int n = 0;\r\n    for (auto s:segs)\r\n\
    \      n = std::max(n, get<2>(s));\r\n    auto rtq = sqrt(q);\r\n    int b = ceil((double)n\
    \ / rtq);\r\n    auto bf = b-b/2;\r\n    auto get_bo = [&](int x) {\r\n      if\
    \ (x < bf) return 0;\r\n      return (x-bf)/b+1;\r\n    };\r\n    auto EvenComp\
    \ = [&](int u, int v) {\r\n      auto &s = segs[u], &t = segs[v];\r\n      auto\
    \ ls = get<1>(s), rs = get<2>(s), lt = get<1>(t), rt = get<2>(t);\r\n      auto\
    \ bs = ls / b, bt = lt / b;\r\n      return bs != bt ? ls < lt : (bs%2==0 ? rs\
    \ < rt : rs > rt);\r\n    };\r\n    auto OddComp = [&](int u, int v) {\r\n   \
    \   auto &s = segs[u], &t = segs[v];\r\n      auto ls = get<1>(s), rs = get<2>(s),\
    \ lt = get<1>(t), rt = get<2>(t);\r\n      auto bs = get_bo(ls), bt = get_bo(lt);\r\
    \n      return bs != bt ? ls < lt : (bs%2==0 ? rs < rt : rs > rt);\r\n    };\r\
    \n    auto& IE = dst;\r\n    IE.resize(q);\r\n    std::iota(IE.begin(), IE.end(),\
    \ 0);\r\n    sort(IE.begin(), IE.end(), EvenComp);\r\n    auto IO = IE;\r\n  \
    \  sort(IO.begin(), IO.end(), OddComp);\r\n    auto move_distance = [&](const\
    \ std::vector<int>& ids) {\r\n      long long d = 0;\r\n      for (int i = 0;\
    \ i < q-1; i++) {\r\n        int j = ids[i], k = ids[i+1];\r\n        d += abs(get<1>(segs[j])\
    \ - get<1>(segs[k]));\r\n        d += abs(get<2>(segs[j]) - get<2>(segs[k]));\r\
    \n      }\r\n      return d;\r\n    };\r\n    if (move_distance(IE) > move_distance(IO))\r\
    \n      std::swap(IE, IO); // IE is reference of dst\r\n  }\r\n  void solve(std::vector<T>\
    \ &dst) {\r\n    if (q == 0) return;\r\n    std::vector<int> I;\r\n    calc_mos_move(I);\r\
    \n    dst.resize(q);\r\n    int _l = 0, _r = 0;\r\n    T val = e();\r\n    for\
    \ (int i:I) {\r\n      int t,l,r;\r\n      std::tie(t,l,r) = segs[i];\r\n    \
    \  while (l < _l)\r\n        val = pushl(val, --_l);\r\n      while (_r < r)\r\
    \n        val = pushr(val, _r++);\r\n      while (_l < l)\r\n        val = popl(val,\
    \ _l++);\r\n      while (r < _r)\r\n        val = popr(val, --_r);\r\n      dst[t]\
    \ = val;\r\n    }\r\n  }\r\n};\r\n"
  code: "#pragma once\r\n#include <algorithm>\r\n#include <numeric>\r\n#include <cmath>\r\
    \n#include <tuple>\r\n#include <vector>\r\n#include <iostream>\r\n\r\n/**\r\n\
    \ * @brief Mo's algorithm: solve offline segment queries on a sequence\r\n * @note\
    \  This implementation is optimized by noshi's idea\r\n *          complexity\
    \ of N sqrt(Q) + O(N).\r\n *        - \u5B9A\u6570\u500D\u304C\u6700\u9069\u306A\
    \ Mo's Algorithm. noshi91\u306E\u30E1\u30E2. 2023/04/13.\r\n *          https://noshi91.hatenablog.com/entry/2023/04/13/224811\r\
    \n*/\r\ntemplate <typename T, \r\n          T (*pushl)(T, int), \r\n         \
    \ T (*pushr)(T, int), \r\n          T (*popl)(T, int),\r\n          T (*popr)(T,\
    \ int), \r\n          T (*e)()>\r\nclass MoSolver {\r\n private:\r\n  std::vector<std::tuple<int,\
    \ int, int>> segs;\r\n  int q = 0;\r\n\r\n public:\r\n  void add_segment(int l,\
    \ int r) { \r\n    segs.emplace_back(q++, l, r);\r\n  }\r\n  void calc_mos_move(std::vector<int>&\
    \ dst) {\r\n    using std::get;\r\n    int n = 0;\r\n    for (auto s:segs)\r\n\
    \      n = std::max(n, get<2>(s));\r\n    auto rtq = sqrt(q);\r\n    int b = ceil((double)n\
    \ / rtq);\r\n    auto bf = b-b/2;\r\n    auto get_bo = [&](int x) {\r\n      if\
    \ (x < bf) return 0;\r\n      return (x-bf)/b+1;\r\n    };\r\n    auto EvenComp\
    \ = [&](int u, int v) {\r\n      auto &s = segs[u], &t = segs[v];\r\n      auto\
    \ ls = get<1>(s), rs = get<2>(s), lt = get<1>(t), rt = get<2>(t);\r\n      auto\
    \ bs = ls / b, bt = lt / b;\r\n      return bs != bt ? ls < lt : (bs%2==0 ? rs\
    \ < rt : rs > rt);\r\n    };\r\n    auto OddComp = [&](int u, int v) {\r\n   \
    \   auto &s = segs[u], &t = segs[v];\r\n      auto ls = get<1>(s), rs = get<2>(s),\
    \ lt = get<1>(t), rt = get<2>(t);\r\n      auto bs = get_bo(ls), bt = get_bo(lt);\r\
    \n      return bs != bt ? ls < lt : (bs%2==0 ? rs < rt : rs > rt);\r\n    };\r\
    \n    auto& IE = dst;\r\n    IE.resize(q);\r\n    std::iota(IE.begin(), IE.end(),\
    \ 0);\r\n    sort(IE.begin(), IE.end(), EvenComp);\r\n    auto IO = IE;\r\n  \
    \  sort(IO.begin(), IO.end(), OddComp);\r\n    auto move_distance = [&](const\
    \ std::vector<int>& ids) {\r\n      long long d = 0;\r\n      for (int i = 0;\
    \ i < q-1; i++) {\r\n        int j = ids[i], k = ids[i+1];\r\n        d += abs(get<1>(segs[j])\
    \ - get<1>(segs[k]));\r\n        d += abs(get<2>(segs[j]) - get<2>(segs[k]));\r\
    \n      }\r\n      return d;\r\n    };\r\n    if (move_distance(IE) > move_distance(IO))\r\
    \n      std::swap(IE, IO); // IE is reference of dst\r\n  }\r\n  void solve(std::vector<T>\
    \ &dst) {\r\n    if (q == 0) return;\r\n    std::vector<int> I;\r\n    calc_mos_move(I);\r\
    \n    dst.resize(q);\r\n    int _l = 0, _r = 0;\r\n    T val = e();\r\n    for\
    \ (int i:I) {\r\n      int t,l,r;\r\n      std::tie(t,l,r) = segs[i];\r\n    \
    \  while (l < _l)\r\n        val = pushl(val, --_l);\r\n      while (_r < r)\r\
    \n        val = pushr(val, _r++);\r\n      while (_l < l)\r\n        val = popl(val,\
    \ _l++);\r\n      while (r < _r)\r\n        val = popr(val, --_r);\r\n      dst[t]\
    \ = val;\r\n    }\r\n  }\r\n};\r\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/mo.hpp
  requiredBy: []
  timestamp: '2023-04-14 03:06:51+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/yosupo/static_range_inversions_query-mo.test.cpp
documentation_of: include/mtl/mo.hpp
layout: document
redirect_from:
- /library/include/mtl/mo.hpp
- /library/include/mtl/mo.hpp.html
title: 'Mo''s algorithm: solve offline segment queries on a sequence'
---
