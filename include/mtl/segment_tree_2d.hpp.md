---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':x:'
    path: include/mtl/segment_tree.hpp
    title: include/mtl/segment_tree.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/bit_manip.hpp\"\n#include <cstdint>\n#include\
    \ <cassert>\n\nnamespace bm {\n\ninline constexpr uint64_t popcnt_e8(uint64_t\
    \ x) {\n  x = (x & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);\n  x =\
    \ (x & 0x3333333333333333) + ((x>>2) & 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F)\
    \ + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n  return x;\n}\n// Count 1s\ninline constexpr\
    \ unsigned popcnt(uint64_t x) {\n  return (popcnt_e8(x) * 0x0101010101010101)\
    \ >> 56;\n}\n// Count trailing 0s. s.t. *11011000 -> 3\ninline constexpr unsigned\
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
    \ 3 \"include/mtl/segment_tree.hpp\"\n#include <cstddef>\n#include <vector>\n\
    #include <stack>\n#if __cplusplus >= 202002L\n#include <concepts>\n\ntemplate<class\
    \ M>\nconcept SegmentTreeMonoid = requires (M m) {\n  {m * m} -> std::same_as<M>;\n\
    };\n#endif\n\ntemplate <class M>\nclass SegmentTree {\n#if __cplusplus >= 202002L\n\
    \  static_assert(SegmentTreeMonoid<M>);\n#endif\n private:\n  size_t size_;\n\
    \  std::vector<M> tree_;\n\n public:\n  explicit SegmentTree(size_t size) : size_(size),\
    \ tree_(size*2) {}\n\n  template <class Iter>\n  explicit SegmentTree(Iter begin,\
    \ Iter end) : SegmentTree(std::distance(begin, end)) {\n    if (size_==0) return;\n\
    \    std::copy(begin, end, tree_.begin() + size_);\n    for (size_t i = size_-1;\
    \ i > 0; i--)\n      tree_[i] = tree_[i<<1] * tree_[(i<<1)+1];\n  }\n\n  M get(size_t\
    \ index) const {\n    return tree_[size_ + index];\n  }\n  M operator[](size_t\
    \ index) const {\n    return get(index);\n  }\n\n private:\n  template<class T>\n\
    \  void _set(size_t index, T&& val) {\n    auto i = size_ + index;\n    tree_[i]\
    \ = std::forward<T>(val);\n    i >>= 1;\n    while (i > 0) {\n      tree_[i] =\
    \ tree_[i<<1] * tree_[(i<<1)+1];\n      i >>= 1;\n    }\n  }\n public:\n  template<class\
    \ T>\n  void set(size_t index, T&& val) {\n    return _set(index, std::forward<T>(val));\n\
    \  }\n  void set(size_t index, const M& val) {\n    _set(index, val);\n  }\n \
    \ void set(size_t index, M&& val) {\n    _set(index, std::move(val));\n  }\n\n\
    \  M query(size_t l, size_t r) const {\n    M lhs,rhs;\n    for (auto _l = l+size_,\
    \ _r = r+size_; _l < _r; _l>>=1, _r>>=1) {\n      if (_l&1) lhs = lhs * tree_[_l++];\n\
    \      if (_r&1) rhs = tree_[--_r] * rhs;\n    }\n    return lhs * rhs;\n  }\n\
    \n  template<class F>\n  size_t max_right(size_t begin, size_t end, F f) const\
    \ {\n    if (begin == end) return end;\n    M p;\n    std::stack<std::pair<size_t,\
    \ M>> rps;\n    auto l = size_ + begin;\n    auto r = size_ + end;\n    while\
    \ (l < r and f(p * tree_[l])) {\n      if (l&1) p = p * tree_[l++];\n      if\
    \ (r&1) {\n        rps.emplace(r, tree_[r-1]);\n        r--;\n      }\n      l>>=1;\
    \ r>>=1;\n    }\n    if (l >= r) {\n      while (rps.size()) {\n        auto&\
    \ [r, rp] = rps.top();\n        if (!f(p * rp)) {\n          l = r-1;\n      \
    \    break;\n        }\n        p = p * rp;\n        rps.pop();\n      }\n   \
    \   if (rps.empty()) return end;\n    }\n    while (l < size_) {\n      assert(!f(p\
    \ * tree_[l]));\n      l <<= 1;\n      if (f(p * tree_[l]))\n        p = p * tree_[l++];\n\
    \    }\n    return l - size_;\n  }\n  template<bool (*F)(M)>\n  size_t max_right(size_t\
    \ begin, size_t end) const {\n    return max_right(begin, end, [](M x) { return\
    \ F(x); });\n  }\n\n  template<class F>\n  size_t min_left(size_t begin, size_t\
    \ end, F f) const {\n    if (end == begin) return begin;\n    M p;\n    std::stack<std::pair<size_t,\
    \ M>> lps;\n    auto l = size_ + begin;\n    auto r = size_ + end;\n    while\
    \ (l < r and f(tree_[r-1] * p)) {\n      if (l&1) {\n        lps.emplace(l, tree_[l]);\n\
    \        l++;\n      }\n      if (r&1) p = tree_[r-1] * p;\n      l>>=1; r>>=1;\n\
    \    }\n    if (l >= r) {\n      while (lps.size()) {\n        auto& [l, lp] =\
    \ lps.top();\n        if (!f(lp * p)) {\n          r = l+1;\n          break;\n\
    \        }\n        p = lp * p;\n        lps.pop();\n      }\n      if (lps.empty())\
    \ return begin;\n    }\n    while (r <= size_) {\n      assert(!f(tree_[r-1] *\
    \ p));\n      r <<= 1;\n      if (f(tree_[r-1] * p)) \n        p = tree_[--r]\
    \ * p;\n    }\n    return r - size_;\n  }\n  template<bool (*F)(M)>\n  size_t\
    \ min_left(size_t begin, size_t end) const {\n    return min_left(begin, [](M\
    \ x) { return F(x); });\n  }\n\n};\n#line 5 \"include/mtl/segment_tree_2d.hpp\"\
    \n\r\n// In-memory 2D segment tree.\r\n// It requires memory size of O(nm).\r\n\
    template<typename M>\r\nclass SegmentTree2D {\r\n public:\r\n  using seg_type\
    \ = SegmentTree<M>;\r\n private:\r\n  size_t n_,m_;\r\n  std::vector<seg_type>\
    \ tb_;\r\n public:\r\n  explicit SegmentTree2D(int n, int m) : n_(n), m_(m), tb_(n*2,\
    \ seg_type(m)) {}\r\n  void set(int i, int j, M val) {\r\n    assert(i >= 0 and\
    \ i < (int)n_ and j >= 0 and j < (int)m_);\r\n    int x = n_ + i;\r\n    tb_[x].set(j,\
    \ val);\r\n    x >>= 1;\r\n    while (x > 0) {\r\n      tb_[x].set(j, tb_[x*2][j]\
    \ * tb_[x*2+1][j]);\r\n      x >>= 1;\r\n    }\r\n  }\r\n  M query(int rl, int\
    \ cl, int rr, int cr) const {\r\n    assert(rl >= 0 and rl <= (int)n_ and rr >=\
    \ 0 and rr <= (int)n_);\r\n    assert(cl >= 0 and cl <= (int)m_ and cr >= 0 and\
    \ cr <= (int)m_);\r\n    M lhs, rhs;\r\n    for (auto l = rl+n_, r = rr+n_; l\
    \ < r; l>>=1, r>>=1) {\r\n      if (l&1) lhs = lhs * tb_[l++].query(cl,cr);\r\n\
    \      if (r&1) rhs = tb_[--r].query(cl,cr) * rhs;\r\n    }\r\n    return lhs\
    \ * rhs;\r\n  }\r\n};\n"
  code: "#pragma once\r\n#include \"segment_tree.hpp\"\r\n#include <cassert>\r\n#include\
    \ <vector>\r\n\r\n// In-memory 2D segment tree.\r\n// It requires memory size\
    \ of O(nm).\r\ntemplate<typename M>\r\nclass SegmentTree2D {\r\n public:\r\n \
    \ using seg_type = SegmentTree<M>;\r\n private:\r\n  size_t n_,m_;\r\n  std::vector<seg_type>\
    \ tb_;\r\n public:\r\n  explicit SegmentTree2D(int n, int m) : n_(n), m_(m), tb_(n*2,\
    \ seg_type(m)) {}\r\n  void set(int i, int j, M val) {\r\n    assert(i >= 0 and\
    \ i < (int)n_ and j >= 0 and j < (int)m_);\r\n    int x = n_ + i;\r\n    tb_[x].set(j,\
    \ val);\r\n    x >>= 1;\r\n    while (x > 0) {\r\n      tb_[x].set(j, tb_[x*2][j]\
    \ * tb_[x*2+1][j]);\r\n      x >>= 1;\r\n    }\r\n  }\r\n  M query(int rl, int\
    \ cl, int rr, int cr) const {\r\n    assert(rl >= 0 and rl <= (int)n_ and rr >=\
    \ 0 and rr <= (int)n_);\r\n    assert(cl >= 0 and cl <= (int)m_ and cr >= 0 and\
    \ cr <= (int)m_);\r\n    M lhs, rhs;\r\n    for (auto l = rl+n_, r = rr+n_; l\
    \ < r; l>>=1, r>>=1) {\r\n      if (l&1) lhs = lhs * tb_[l++].query(cl,cr);\r\n\
    \      if (r&1) rhs = tb_[--r].query(cl,cr) * rhs;\r\n    }\r\n    return lhs\
    \ * rhs;\r\n  }\r\n};"
  dependsOn:
  - include/mtl/segment_tree.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/segment_tree_2d.hpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/segment_tree_2d.hpp
layout: document
redirect_from:
- /library/include/mtl/segment_tree_2d.hpp
- /library/include/mtl/segment_tree_2d.hpp.html
title: include/mtl/segment_tree_2d.hpp
---
