---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: include/mtl/convex_hull_trick.hpp
    title: include/mtl/convex_hull_trick.hpp
  - icon: ':warning:'
    path: test/tenka1-2016-final-open-e.cpp
    title: test/tenka1-2016-final-open-e.cpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/aoj/aoj-time_table-cht.test.cpp
    title: test/aoj/aoj-time_table-cht.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/search.hpp\"\n#include <utility>\r\n#include\
    \ <type_traits>\r\n#include <functional>\r\n#include <cmath>\r\n#include <vector>\r\
    \n#include <cassert>\r\n\r\nconstexpr double EPS_DEFAULT = 1e-9;\r\n\r\ntemplate<typename\
    \ I, typename F>\r\ntypename std::remove_reference<I>::type\r\nbisect_int(I ok,\
    \ I ng, F f) {\r\n  while (std::abs(ng - ok) > 1) {\r\n    auto c = ok + (ng -\
    \ ok) / 2;\r\n    if (f(c))\r\n      ok = c;\r\n    else\r\n      ng = c;\r\n\
    \  }\r\n  return ok;\r\n}\r\n\r\ntemplate<typename F>\r\ndouble\r\nbisect_float(double\
    \ ok, double ng, F fn, double eps = EPS_DEFAULT) {\r\n  while (std::abs(ok - ng)\
    \ > eps) {\r\n    double c = ok + (ng - ok) / 2;\r\n    if (fn(c))\r\n      ok\
    \ = c;\r\n    else\r\n      ng = c;\r\n  }\r\n  return ok;\r\n}\r\n\r\ntemplate<typename\
    \ T, typename I, typename F, typename C>\r\nstd::pair<typename std::remove_reference<I>::type,\
    \ T>\r\nfibonacci_search(I l, I r, F fn, C cmp = std::less<T>()) {\r\n  using\
    \ idx_type = typename std::remove_reference<I>::type;\r\n  assert(r - l >= 2);\r\
    \n  idx_type d = r-l;\r\n  std::vector<idx_type> fib{1,1};\r\n  while (fib.back()\
    \ < d) fib.push_back(fib[fib.size()-2] + fib[fib.size()-1]);\r\n  auto k = --fib.cend();\r\
    \n  idx_type il = l + *(k-2);\r\n  idx_type ir = l + *(k-1);\r\n  T lv = fn(il);\r\
    \n  T rv = fn(ir);\r\n  std::pair<idx_type, T> ret = cmp(lv, rv) ? std::make_pair(ir,\
    \ rv) : std::make_pair(il, lv);\r\n  while (*k > 3) {\r\n    if (cmp(lv, rv))\
    \ {\r\n      l += *(k-2);\r\n      lv = rv;\r\n      auto i = std::min(r-1, l\
    \ + *(k-2));\r\n      rv = fn(i);\r\n      if (cmp(ret.second, rv))\r\n      \
    \  ret = std::make_pair(i, rv);\r\n    } else {\r\n      rv = lv;\r\n      auto\
    \ i = std::min(r-1, l + *(k-3));\r\n      lv = fn(i);\r\n      if (cmp(ret.second,\
    \ lv))\r\n        ret = std::make_pair(i, lv);\r\n    }\r\n    --k;\r\n  }\r\n\
    \  return ret;\r\n}\r\ntemplate<typename T, typename I, typename F, typename C\
    \ = std::less<T>>\r\nstd::pair<typename std::remove_reference<I>::type, T>\r\n\
    trisect_int(I l, I r, F fn, C cmp = C()) {\r\n  return fibonacci_search<T>(l,\
    \ r, fn, cmp);\r\n}\r\n\r\nconstexpr double GOLDEN_RATIO = 1.61803398875;\r\n\
    template<typename T, typename F, typename C>\r\nstd::pair<double, T>\r\ngolden_ratio_search(double\
    \ l, double r, F fn, double eps = EPS_DEFAULT, C cmp = std::less<T>()) {\r\n \
    \ std::pair<double, T> ret;\r\n  while (std::abs(r - l) > eps) {\r\n    double\
    \ il = (l * GOLDEN_RATIO + r) / (GOLDEN_RATIO + 1.);\r\n    double ir = (l + r\
    \ * GOLDEN_RATIO) / (GOLDEN_RATIO + 1.);\r\n    T vl = fn(il);\r\n    T vr = fn(ir);\r\
    \n    if (cmp(vr, vl)) {\r\n      ret = std::make_pair(il, vl);\r\n      r = ir;\r\
    \n    } else {\r\n      ret = std::make_pair(ir, vr);\r\n      l = il;\r\n   \
    \ }\r\n  }\r\n  return ret;\r\n}\r\ntemplate<typename T, typename F, typename\
    \ C>\r\nstd::pair<double, T>\r\ntrisect_float(double l, double r, F fn, double\
    \ eps = EPS_DEFAULT, C cmp = std::less<T>()) {\r\n  return golden_ratio_search<T>(l,\
    \ r, fn, eps, cmp);\r\n}\r\n"
  code: "#pragma once\r\n#include <utility>\r\n#include <type_traits>\r\n#include\
    \ <functional>\r\n#include <cmath>\r\n#include <vector>\r\n#include <cassert>\r\
    \n\r\nconstexpr double EPS_DEFAULT = 1e-9;\r\n\r\ntemplate<typename I, typename\
    \ F>\r\ntypename std::remove_reference<I>::type\r\nbisect_int(I ok, I ng, F f)\
    \ {\r\n  while (std::abs(ng - ok) > 1) {\r\n    auto c = ok + (ng - ok) / 2;\r\
    \n    if (f(c))\r\n      ok = c;\r\n    else\r\n      ng = c;\r\n  }\r\n  return\
    \ ok;\r\n}\r\n\r\ntemplate<typename F>\r\ndouble\r\nbisect_float(double ok, double\
    \ ng, F fn, double eps = EPS_DEFAULT) {\r\n  while (std::abs(ok - ng) > eps) {\r\
    \n    double c = ok + (ng - ok) / 2;\r\n    if (fn(c))\r\n      ok = c;\r\n  \
    \  else\r\n      ng = c;\r\n  }\r\n  return ok;\r\n}\r\n\r\ntemplate<typename\
    \ T, typename I, typename F, typename C>\r\nstd::pair<typename std::remove_reference<I>::type,\
    \ T>\r\nfibonacci_search(I l, I r, F fn, C cmp = std::less<T>()) {\r\n  using\
    \ idx_type = typename std::remove_reference<I>::type;\r\n  assert(r - l >= 2);\r\
    \n  idx_type d = r-l;\r\n  std::vector<idx_type> fib{1,1};\r\n  while (fib.back()\
    \ < d) fib.push_back(fib[fib.size()-2] + fib[fib.size()-1]);\r\n  auto k = --fib.cend();\r\
    \n  idx_type il = l + *(k-2);\r\n  idx_type ir = l + *(k-1);\r\n  T lv = fn(il);\r\
    \n  T rv = fn(ir);\r\n  std::pair<idx_type, T> ret = cmp(lv, rv) ? std::make_pair(ir,\
    \ rv) : std::make_pair(il, lv);\r\n  while (*k > 3) {\r\n    if (cmp(lv, rv))\
    \ {\r\n      l += *(k-2);\r\n      lv = rv;\r\n      auto i = std::min(r-1, l\
    \ + *(k-2));\r\n      rv = fn(i);\r\n      if (cmp(ret.second, rv))\r\n      \
    \  ret = std::make_pair(i, rv);\r\n    } else {\r\n      rv = lv;\r\n      auto\
    \ i = std::min(r-1, l + *(k-3));\r\n      lv = fn(i);\r\n      if (cmp(ret.second,\
    \ lv))\r\n        ret = std::make_pair(i, lv);\r\n    }\r\n    --k;\r\n  }\r\n\
    \  return ret;\r\n}\r\ntemplate<typename T, typename I, typename F, typename C\
    \ = std::less<T>>\r\nstd::pair<typename std::remove_reference<I>::type, T>\r\n\
    trisect_int(I l, I r, F fn, C cmp = C()) {\r\n  return fibonacci_search<T>(l,\
    \ r, fn, cmp);\r\n}\r\n\r\nconstexpr double GOLDEN_RATIO = 1.61803398875;\r\n\
    template<typename T, typename F, typename C>\r\nstd::pair<double, T>\r\ngolden_ratio_search(double\
    \ l, double r, F fn, double eps = EPS_DEFAULT, C cmp = std::less<T>()) {\r\n \
    \ std::pair<double, T> ret;\r\n  while (std::abs(r - l) > eps) {\r\n    double\
    \ il = (l * GOLDEN_RATIO + r) / (GOLDEN_RATIO + 1.);\r\n    double ir = (l + r\
    \ * GOLDEN_RATIO) / (GOLDEN_RATIO + 1.);\r\n    T vl = fn(il);\r\n    T vr = fn(ir);\r\
    \n    if (cmp(vr, vl)) {\r\n      ret = std::make_pair(il, vl);\r\n      r = ir;\r\
    \n    } else {\r\n      ret = std::make_pair(ir, vr);\r\n      l = il;\r\n   \
    \ }\r\n  }\r\n  return ret;\r\n}\r\ntemplate<typename T, typename F, typename\
    \ C>\r\nstd::pair<double, T>\r\ntrisect_float(double l, double r, F fn, double\
    \ eps = EPS_DEFAULT, C cmp = std::less<T>()) {\r\n  return golden_ratio_search<T>(l,\
    \ r, fn, eps, cmp);\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/search.hpp
  requiredBy:
  - include/mtl/convex_hull_trick.hpp
  - test/tenka1-2016-final-open-e.cpp
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/aoj/aoj-time_table-cht.test.cpp
documentation_of: include/mtl/search.hpp
layout: document
redirect_from:
- /library/include/mtl/search.hpp
- /library/include/mtl/search.hpp.html
title: include/mtl/search.hpp
---
