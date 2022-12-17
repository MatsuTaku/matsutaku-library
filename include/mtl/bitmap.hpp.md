---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/fenwick_tree.hpp
    title: include/mtl/fenwick_tree.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/bitmap.hpp\"\n#include <cstddef>\r\n#include\
    \ <cstdint>\r\n#include <array>\r\n#include <vector>\r\n#line 3 \"include/mtl/bit_manip.hpp\"\
    \n#include <cassert>\n\nnamespace bm {\n\ninline constexpr uint64_t popcnt_e8(uint64_t\
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
    \ 5 \"include/mtl/fenwick_tree.hpp\"\n\ntemplate <typename T>\nclass FenwickTree\
    \ {\n private:\n  std::vector<T> tree_;\n\n public:\n  FenwickTree() = default;\n\
    \  explicit FenwickTree(size_t size) : tree_(size+1) {}\n\n  size_t size() const\
    \ { return tree_.size()-1; }\n\n  template <typename Iter>\n  explicit FenwickTree(Iter\
    \ begin, Iter end) : FenwickTree(end-begin) {\n    for (auto it = begin; it !=\
    \ end; ++it)\n      add(it-begin, *it);\n  }\n\n  void add(size_t index, T x)\
    \ {\n    for (size_t i = index+1; i < tree_.size(); i += i&(-i))\n      tree_[i]\
    \ += x;\n  }\n\n  T sum(size_t index) const {\n    T sum = 0;\n    for (size_t\
    \ i = index+1; i > 0; i -= i&(-i))\n      sum += tree_[i];\n    return sum;\n\
    \  }\n\n  T range_sum(size_t l, size_t r) const {\n    auto sl = l > 0 ? sum(l-1)\
    \ : 0;\n    auto sr = r > 0 ? sum(r-1) : 0;\n    return sr - sl;\n  }\n\n  size_t\
    \ lower_bound(T _sum) const {\n    size_t ret = 0;\n    T s = 0;\n    for (int\
    \ k = 63-bm::clz(size()); k >= 0; k--) {\n      size_t j = ret | (1ull<<k);\n\
    \      if (j < tree_.size() and s + tree_[j] < _sum) {\n        s += tree_[j];\n\
    \        ret = j;\n      }\n    }\n    return ret;\n  }\n\n};\n\n#line 8 \"include/mtl/bitmap.hpp\"\
    \n#include <iostream>\r\n\r\nclass Bitmap {\r\n public:\r\n  using size_type =\
    \ int;\r\n  using RSQ = FenwickTree<size_type>;\r\n private:\r\n  RSQ rsq;\r\n\
    \  std::vector<uint64_t> bv;\r\n\r\n  size_t word_size(size_t sz) const {\r\n\
    \    return (sz + 64-1) / 64;\r\n  }\r\n\r\n  std::array<std::array<uint8_t, 9>,\
    \ 1<<8> sel_tb;\r\n  constexpr void init_sel_tb() {\r\n    for (int i = 0; i <\
    \ 1<<8; i++) {\r\n      int c = 0;\r\n      int x = i;\r\n      for (int j = 0;\
    \ j < 8; j++) {\r\n        if (x & 1)\r\n          sel_tb[i][++c] = j;\r\n   \
    \     x >>= 1;\r\n      }\r\n    }\r\n  }\r\n\r\n public:\r\n  explicit Bitmap(size_t\
    \ size = 0, bool init_b = false)\r\n      : rsq(word_size(size)),\r\n        bv(word_size(size),\
    \ init_b ? 0xFFFFFFFFFFFFFFFF : 0) {\r\n    init_sel_tb();\r\n  }\r\n  template<typename\
    \ It>\r\n  explicit Bitmap(It begin, It end) : Bitmap(end-begin) {\r\n    using\
    \ traits = std::iterator_traits<It>;\r\n    static_assert(std::is_convertible<typename\
    \ traits::value_type,\r\n                                      bool>::value, \"\
    \");\r\n    static_assert(std::is_base_of<std::random_access_iterator_tag,\r\n\
    \                                  typename traits::iterator_category>::value,\
    \ \"\");\r\n    size_type i = 0;\r\n    for (auto it = begin; it != end; ++it)\r\
    \n      set(i++, *it);\r\n  }\r\n\r\n  bool get(size_t i) const {\r\n    return\
    \ (bv[i/64] >> (i%64)) & 1ull;\r\n  }\r\n  void set(size_t i, bool b) {\r\n  \
    \  int q = i/64, r = i%64;\r\n    rsq.add(q, (int) b - get(i));\r\n    if (b)\
    \ bv[q] |= 1ull<<r;\r\n    else   bv[q] &= ~(1ull<<r);\r\n  }\r\n\r\n  /* Count\
    \ 1s in [0, i) */\r\n  size_type rank(size_t i) const {\r\n    if (i == 0) return\
    \ 0;\r\n    int b = i/64;\r\n    return (size_type) (b > 0 ? rsq.sum(b-1) : 0)\r\
    \n        + bm::popcnt(bv[i/64] & ((1ull<<(i%64))-1));\r\n  }\r\n  /* Position\
    \ (0-indexed) of the (n+1) th 1 */\r\n  size_type select(size_t n) const { //\
    \ 0-index\r\n    n += 1; // to 1-index\r\n    int s = rsq.lower_bound(n);\r\n\
    \    int m = n - (s > 0 ? rsq.sum(s-1) : 0);\r\n    uint64_t w = bv[s];\r\n  \
    \  auto x = bm::popcnt_e8(w) * 0x0101010101010100ull; // Cumulative sum for 8bit\
    \ blocks.\r\n    auto b_rank = (const uint8_t*) &x;\r\n    int b = 0;\r\n    {\r\
    \n      int d = 8;\r\n      while (d > 1) {\r\n        int c = b + d/2;\r\n  \
    \      if (b_rank[c] < m)\r\n          b = c;\r\n        d /= 2;\r\n      }\r\n\
    \    }\r\n    int k = m - b_rank[b];\r\n    int B = ((const uint8_t*) &w)[b];\r\
    \n    return 64 * s + 8 * b + sel_tb[B][k];\r\n  }\r\n};\n"
  code: "#pragma once\r\n#include <cstddef>\r\n#include <cstdint>\r\n#include <array>\r\
    \n#include <vector>\r\n#include \"fenwick_tree.hpp\"\r\n#include \"bit_manip.hpp\"\
    \r\n#include <iostream>\r\n\r\nclass Bitmap {\r\n public:\r\n  using size_type\
    \ = int;\r\n  using RSQ = FenwickTree<size_type>;\r\n private:\r\n  RSQ rsq;\r\
    \n  std::vector<uint64_t> bv;\r\n\r\n  size_t word_size(size_t sz) const {\r\n\
    \    return (sz + 64-1) / 64;\r\n  }\r\n\r\n  std::array<std::array<uint8_t, 9>,\
    \ 1<<8> sel_tb;\r\n  constexpr void init_sel_tb() {\r\n    for (int i = 0; i <\
    \ 1<<8; i++) {\r\n      int c = 0;\r\n      int x = i;\r\n      for (int j = 0;\
    \ j < 8; j++) {\r\n        if (x & 1)\r\n          sel_tb[i][++c] = j;\r\n   \
    \     x >>= 1;\r\n      }\r\n    }\r\n  }\r\n\r\n public:\r\n  explicit Bitmap(size_t\
    \ size = 0, bool init_b = false)\r\n      : rsq(word_size(size)),\r\n        bv(word_size(size),\
    \ init_b ? 0xFFFFFFFFFFFFFFFF : 0) {\r\n    init_sel_tb();\r\n  }\r\n  template<typename\
    \ It>\r\n  explicit Bitmap(It begin, It end) : Bitmap(end-begin) {\r\n    using\
    \ traits = std::iterator_traits<It>;\r\n    static_assert(std::is_convertible<typename\
    \ traits::value_type,\r\n                                      bool>::value, \"\
    \");\r\n    static_assert(std::is_base_of<std::random_access_iterator_tag,\r\n\
    \                                  typename traits::iterator_category>::value,\
    \ \"\");\r\n    size_type i = 0;\r\n    for (auto it = begin; it != end; ++it)\r\
    \n      set(i++, *it);\r\n  }\r\n\r\n  bool get(size_t i) const {\r\n    return\
    \ (bv[i/64] >> (i%64)) & 1ull;\r\n  }\r\n  void set(size_t i, bool b) {\r\n  \
    \  int q = i/64, r = i%64;\r\n    rsq.add(q, (int) b - get(i));\r\n    if (b)\
    \ bv[q] |= 1ull<<r;\r\n    else   bv[q] &= ~(1ull<<r);\r\n  }\r\n\r\n  /* Count\
    \ 1s in [0, i) */\r\n  size_type rank(size_t i) const {\r\n    if (i == 0) return\
    \ 0;\r\n    int b = i/64;\r\n    return (size_type) (b > 0 ? rsq.sum(b-1) : 0)\r\
    \n        + bm::popcnt(bv[i/64] & ((1ull<<(i%64))-1));\r\n  }\r\n  /* Position\
    \ (0-indexed) of the (n+1) th 1 */\r\n  size_type select(size_t n) const { //\
    \ 0-index\r\n    n += 1; // to 1-index\r\n    int s = rsq.lower_bound(n);\r\n\
    \    int m = n - (s > 0 ? rsq.sum(s-1) : 0);\r\n    uint64_t w = bv[s];\r\n  \
    \  auto x = bm::popcnt_e8(w) * 0x0101010101010100ull; // Cumulative sum for 8bit\
    \ blocks.\r\n    auto b_rank = (const uint8_t*) &x;\r\n    int b = 0;\r\n    {\r\
    \n      int d = 8;\r\n      while (d > 1) {\r\n        int c = b + d/2;\r\n  \
    \      if (b_rank[c] < m)\r\n          b = c;\r\n        d /= 2;\r\n      }\r\n\
    \    }\r\n    int k = m - b_rank[b];\r\n    int B = ((const uint8_t*) &w)[b];\r\
    \n    return 64 * s + 8 * b + sel_tb[B][k];\r\n  }\r\n};"
  dependsOn:
  - include/mtl/fenwick_tree.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/bitmap.hpp
  requiredBy: []
  timestamp: '2022-12-18 04:26:00+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/bitmap.hpp
layout: document
redirect_from:
- /library/include/mtl/bitmap.hpp
- /library/include/mtl/bitmap.hpp.html
title: include/mtl/bitmap.hpp
---
