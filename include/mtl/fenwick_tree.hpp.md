---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  _extendedRequiredBy:
  - icon: ':warning:'
    path: include/mtl/bitmap.hpp
    title: include/mtl/bitmap.hpp
  - icon: ':question:'
    path: include/mtl/ordinal_range_search.hpp
    title: Ordinal Range Search
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/yosupo/point_add_range_sum.test.cpp
    title: test/yosupo/point_add_range_sum.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/rectangle_sum.test.cpp
    title: test/yosupo/rectangle_sum.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/static_range_inversions_query.test.cpp
    title: test/yosupo/static_range_inversions_query.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/static_range_sum.test.cpp
    title: test/yosupo/static_range_sum.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/static_rectangle_add_rectangle_sum.test.cpp
    title: test/yosupo/static_rectangle_add_rectangle_sum.test.cpp
  - icon: ':x:'
    path: test/yosupo/vertex_add_path_sum.test.cpp
    title: test/yosupo/vertex_add_path_sum.test.cpp
  - icon: ':x:'
    path: test/yosupo/vertex_add_subtree_sum.test.cpp
    title: test/yosupo/vertex_add_subtree_sum.test.cpp
  - icon: ':x:'
    path: test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
    title: test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':question:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/bit_manip.hpp\"\n#include <cstdint>\n#include\
    \ <cassert>\n\nnamespace bm {\n\ninline constexpr uint64_t popcnt_e8(uint64_t\
    \ x) {\n  x = (x & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);\n  x =\
    \ (x & 0x3333333333333333) + ((x>>2) & 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F)\
    \ + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n  return x;\n}\n// Count 1s\ninline constexpr\
    \ unsigned popcnt(uint64_t x) {\n  return (popcnt_e8(x) * 0x0101010101010101)\
    \ >> 56;\n}\n// Count trailing 0s. ...01101000 -> 3\ninline constexpr unsigned\
    \ ctz(uint64_t x) {\n  return popcnt((x & (-x)) - 1);\n}\ninline constexpr unsigned\
    \ ctz8(uint8_t x) {\n  return x == 0 ? 8 : popcnt_e8((x & (-x)) - 1);\n}\n// [00..0](8bit)\
    \ -> 0, [**..*](not only 0) -> 1\ninline constexpr uint8_t summary(uint64_t x)\
    \ {\n  constexpr uint64_t hmask = 0x8080808080808080ull;\n  constexpr uint64_t\
    \ lmask = 0x7F7F7F7F7F7F7F7Full;\n  auto a = x & hmask;\n  auto b = x & lmask;\n\
    \  b = hmask - b;\n  b = ~b;\n  auto c = (a | b) & hmask;\n  c *= 0x0002040810204081ull;\n\
    \  return uint8_t(c >> 56);\n}\n// Extract target area of bits\ninline constexpr\
    \ uint64_t bextr(uint64_t x, unsigned start, unsigned len) {\n  uint64_t mask\
    \ = len < 64 ? (1ull<<len)-1 : 0xFFFFFFFFFFFFFFFFull;\n  return (x >> start) &\
    \ mask;\n}\n// 00101101 -> 00111111 -count_1s-> 6\ninline constexpr unsigned log2p1(uint8_t\
    \ x) {\n  if (x & 0x80)\n    return 8;\n  uint64_t p = uint64_t(x) * 0x0101010101010101ull;\n\
    \  p -= 0x8040201008040201ull;\n  p = ~p & 0x8080808080808080ull;\n  p = (p >>\
    \ 7) * 0x0101010101010101ull;\n  p >>= 56;\n  return p;\n}\n// 00101100 -mask_mssb->\
    \ 00100000 -to_index-> 5\ninline constexpr unsigned mssb8(uint8_t x) {\n  assert(x\
    \ != 0);\n  return log2p1(x) - 1;\n}\n// 00101100 -mask_lssb-> 00000100 -to_index->\
    \ 2\ninline constexpr unsigned lssb8(uint8_t x) {\n  assert(x != 0);\n  return\
    \ popcnt_e8((x & -x) - 1);\n}\n// Count leading 0s. 00001011... -> 4\ninline constexpr\
    \ unsigned clz(uint64_t x) {\n  if (x == 0)\n    return 64;\n  auto i = mssb8(summary(x));\n\
    \  auto j = mssb8(bextr(x, 8 * i, 8));\n  return 63 - (8 * i + j);\n}\ninline\
    \ constexpr unsigned clz8(uint8_t x) {\n  return x == 0 ? 8 : 7 - mssb8(x);\n\
    }\ninline constexpr uint64_t bit_reverse(uint64_t x) {\n  x = ((x & 0x00000000FFFFFFFF)\
    \ << 32) | ((x & 0xFFFFFFFF00000000) >> 32);\n  x = ((x & 0x0000FFFF0000FFFF)\
    \ << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);\n  x = ((x & 0x00FF00FF00FF00FF)\
    \ << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);\n  x = ((x & 0x0F0F0F0F0F0F0F0F) <<\
    \ 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);\n  x = ((x & 0x3333333333333333) << 2)\
    \ | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);\n  x = ((x & 0x5555555555555555) << 1) |\
    \ ((x & 0xAAAAAAAAAAAAAAAA) >> 1);\n  return x;\n}\n\n} // namespace bm\n#line\
    \ 3 \"include/mtl/fenwick_tree.hpp\"\n#include <cstddef>\n#include <vector>\n\n\
    template <class T>\nclass FenwickTree {\n private:\n  std::vector<T> tree_;\n\n\
    \ public:\n  FenwickTree() = default;\n  explicit FenwickTree(size_t size) : tree_(size+1)\
    \ {}\n\n  size_t size() const { return tree_.size()-1; }\n\n  template <class\
    \ Iter>\n  explicit FenwickTree(Iter begin, Iter end) : FenwickTree(std::distance(begin,\
    \ end)) {\n    size_t i = 1;\n    for (auto it = begin; it != end; ++it) {\n \
    \     tree_[i] = tree_[i] + *it;\n      auto j = i + (i&(-i));\n      if (j <\
    \ tree_.size())\n        tree_[j] = tree_[j] + tree_[i];\n      ++i;\n    }\n\
    \  }\n\n  template<class V>\n  void add(size_t index, const V& x) {\n    for (size_t\
    \ i = index+1; i < tree_.size(); i += i&(-i))\n      tree_[i] = tree_[i] + x;\n\
    \  }\n\n  T sum(size_t index) const {\n    T sum = 0;\n    for (size_t i = index+1;\
    \ i > 0; i -= i&(-i))\n      sum = sum + tree_[i];\n    return sum;\n  }\n\n \
    \ T range_sum(size_t l, size_t r) const {\n    auto sl = l > 0 ? sum(l-1) : 0;\n\
    \    auto sr = r > 0 ? sum(r-1) : 0;\n    return sr - sl;\n  }\n\n  template<class\
    \ V>\n  size_t lower_bound(const V& _sum) const {\n    size_t ret = 0;\n    T\
    \ s = 0;\n    for (int k = 63-bm::clz(size()); k >= 0; k--) {\n      size_t j\
    \ = ret | (1ull<<k);\n      if (j < tree_.size() and s + tree_[j] < _sum) {\n\
    \        s = s + tree_[j];\n        ret = j;\n      }\n    }\n    return ret;\n\
    \  }\n\n};\n\n"
  code: "#pragma once\n#include \"bit_manip.hpp\"\n#include <cstddef>\n#include <vector>\n\
    \ntemplate <class T>\nclass FenwickTree {\n private:\n  std::vector<T> tree_;\n\
    \n public:\n  FenwickTree() = default;\n  explicit FenwickTree(size_t size) :\
    \ tree_(size+1) {}\n\n  size_t size() const { return tree_.size()-1; }\n\n  template\
    \ <class Iter>\n  explicit FenwickTree(Iter begin, Iter end) : FenwickTree(std::distance(begin,\
    \ end)) {\n    size_t i = 1;\n    for (auto it = begin; it != end; ++it) {\n \
    \     tree_[i] = tree_[i] + *it;\n      auto j = i + (i&(-i));\n      if (j <\
    \ tree_.size())\n        tree_[j] = tree_[j] + tree_[i];\n      ++i;\n    }\n\
    \  }\n\n  template<class V>\n  void add(size_t index, const V& x) {\n    for (size_t\
    \ i = index+1; i < tree_.size(); i += i&(-i))\n      tree_[i] = tree_[i] + x;\n\
    \  }\n\n  T sum(size_t index) const {\n    T sum = 0;\n    for (size_t i = index+1;\
    \ i > 0; i -= i&(-i))\n      sum = sum + tree_[i];\n    return sum;\n  }\n\n \
    \ T range_sum(size_t l, size_t r) const {\n    auto sl = l > 0 ? sum(l-1) : 0;\n\
    \    auto sr = r > 0 ? sum(r-1) : 0;\n    return sr - sl;\n  }\n\n  template<class\
    \ V>\n  size_t lower_bound(const V& _sum) const {\n    size_t ret = 0;\n    T\
    \ s = 0;\n    for (int k = 63-bm::clz(size()); k >= 0; k--) {\n      size_t j\
    \ = ret | (1ull<<k);\n      if (j < tree_.size() and s + tree_[j] < _sum) {\n\
    \        s = s + tree_[j];\n        ret = j;\n      }\n    }\n    return ret;\n\
    \  }\n\n};\n\n"
  dependsOn:
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/fenwick_tree.hpp
  requiredBy:
  - include/mtl/bitmap.hpp
  - include/mtl/ordinal_range_search.hpp
  timestamp: '2023-04-13 21:51:40+09:00'
  verificationStatus: LIBRARY_SOME_WA
  verifiedWith:
  - test/yosupo/static_range_inversions_query.test.cpp
  - test/yosupo/static_rectangle_add_rectangle_sum.test.cpp
  - test/yosupo/point_add_range_sum.test.cpp
  - test/yosupo/vertex_add_path_sum.test.cpp
  - test/yosupo/rectangle_sum.test.cpp
  - test/yosupo/static_range_sum.test.cpp
  - test/yosupo/vertex_add_subtree_sum.test.cpp
  - test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
documentation_of: include/mtl/fenwick_tree.hpp
layout: document
redirect_from:
- /library/include/mtl/fenwick_tree.hpp
- /library/include/mtl/fenwick_tree.hpp.html
title: include/mtl/fenwick_tree.hpp
---
