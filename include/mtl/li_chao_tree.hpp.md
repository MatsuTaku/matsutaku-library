---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/yosupo-line_add_get_min.test.cpp
    title: test/yosupo-line_add_get_min.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo-segment_add_get_min.test.cpp
    title: test/yosupo-segment_add_get_min.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/li_chao_tree.hpp\"\n#include <utility>\r\n#include\
    \ <vector>\r\n#include <limits>\r\n#include <numeric>\r\n#include <functional>\r\
    \n#include <cstring>\r\n#include <iostream>\r\n#line 2 \"include/mtl/bit_manip.hpp\"\
    \n#include <cstdint>\n#include <cassert>\n\nnamespace bm {\n\ninline constexpr\
    \ uint64_t popcnt_e8(uint64_t x) {\n  x = (x & 0x5555555555555555) + ((x>>1) &\
    \ 0x5555555555555555);\n  x = (x & 0x3333333333333333) + ((x>>2) & 0x3333333333333333);\n\
    \  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n  return x;\n\
    }\n// Count 1s\ninline constexpr unsigned popcnt(uint64_t x) {\n  return (popcnt_e8(x)\
    \ * 0x0101010101010101) >> 56;\n}\n// Count trailing 0s. ...01101000 -> 3\ninline\
    \ constexpr unsigned ctz(uint64_t x) {\n  return popcnt((x & (-x)) - 1);\n}\n\
    inline constexpr unsigned ctz8(uint8_t x) {\n  return x == 0 ? 8 : popcnt_e8((x\
    \ & (-x)) - 1);\n}\n// [00..0](8bit) -> 0, [**..*](not only 0) -> 1\ninline constexpr\
    \ uint8_t summary(uint64_t x) {\n  constexpr uint64_t hmask = 0x8080808080808080ull;\n\
    \  constexpr uint64_t lmask = 0x7F7F7F7F7F7F7F7Full;\n  auto a = x & hmask;\n\
    \  auto b = x & lmask;\n  b = hmask - b;\n  b = ~b;\n  auto c = (a | b) & hmask;\n\
    \  c *= 0x0002040810204081ull;\n  return uint8_t(c >> 56);\n}\n// Extract target\
    \ area of bits\ninline constexpr uint64_t bextr(uint64_t x, unsigned start, unsigned\
    \ len) {\n  uint64_t mask = len < 64 ? (1ull<<len)-1 : 0xFFFFFFFFFFFFFFFFull;\n\
    \  return (x >> start) & mask;\n}\n// 00101101 -> 00111111 -count_1s-> 6\ninline\
    \ constexpr unsigned log2p1(uint8_t x) {\n  if (x & 0x80)\n    return 8;\n  uint64_t\
    \ p = uint64_t(x) * 0x0101010101010101ull;\n  p -= 0x8040201008040201ull;\n  p\
    \ = ~p & 0x8080808080808080ull;\n  p = (p >> 7) * 0x0101010101010101ull;\n  p\
    \ >>= 56;\n  return p;\n}\n// 00101100 -mask_mssb-> 00100000 -to_index-> 5\ninline\
    \ constexpr unsigned mssb8(uint8_t x) {\n  assert(x != 0);\n  return log2p1(x)\
    \ - 1;\n}\n// 00101100 -mask_lssb-> 00000100 -to_index-> 2\ninline constexpr unsigned\
    \ lssb8(uint8_t x) {\n  assert(x != 0);\n  return popcnt_e8((x & -x) - 1);\n}\n\
    // Count leading 0s. 00001011... -> 4\ninline constexpr unsigned clz(uint64_t\
    \ x) {\n  if (x == 0)\n    return 64;\n  auto i = mssb8(summary(x));\n  auto j\
    \ = mssb8(bextr(x, 8 * i, 8));\n  return 63 - (8 * i + j);\n}\ninline constexpr\
    \ unsigned clz8(uint8_t x) {\n  return x == 0 ? 8 : 7 - mssb8(x);\n}\ninline constexpr\
    \ uint64_t bit_reverse(uint64_t x) {\n  x = ((x & 0x00000000FFFFFFFF) << 32) |\
    \ ((x & 0xFFFFFFFF00000000) >> 32);\n  x = ((x & 0x0000FFFF0000FFFF) << 16) |\
    \ ((x & 0xFFFF0000FFFF0000) >> 16);\n  x = ((x & 0x00FF00FF00FF00FF) << 8) | ((x\
    \ & 0xFF00FF00FF00FF00) >> 8);\n  x = ((x & 0x0F0F0F0F0F0F0F0F) << 4) | ((x &\
    \ 0xF0F0F0F0F0F0F0F0) >> 4);\n  x = ((x & 0x3333333333333333) << 2) | ((x & 0xCCCCCCCCCCCCCCCC)\
    \ >> 2);\n  x = ((x & 0x5555555555555555) << 1) | ((x & 0xAAAAAAAAAAAAAAAA) >>\
    \ 1);\n  return x;\n}\n\n} // namespace bm\n#line 10 \"include/mtl/li_chao_tree.hpp\"\
    \n\r\ntemplate<typename T, typename Comp = std::less<T>>\r\nstruct LiChaoTree\
    \ {\r\n  using Line = std::pair<T, T>;\r\n  static constexpr T INF = std::numeric_limits<T>::max();\r\
    \n  static constexpr T MINF = std::numeric_limits<T>::min();\r\n  static constexpr\
    \ T INV = Comp()(MINF, INF) ? MINF : INF;\r\n  size_t n, N;\r\n  std::vector<T>\
    \ X;\r\n  std::vector<Line> seg;\r\n  Comp cmp;\r\n\r\n  LiChaoTree() = default;\r\
    \n  explicit LiChaoTree(size_t _n) : n(_n), N(_n ? 1ull<<(64-bm::clz(_n-1)) :\
    \ 0), X(n), seg(N*2, {0, INV}) {\r\n    std::iota(X.begin(), X.begin()+n, 0ull);\r\
    \n  }\r\n  void set_x(size_t i, T x) {\r\n    X[i] = x;\r\n  }\r\n  template<typename\
    \ It>\r\n  LiChaoTree(It begin, It end) : LiChaoTree(std::distance(begin, end))\
    \ {\r\n    std::copy(begin, end, X.begin());\r\n  }\r\n\r\n private:\r\n  inline\
    \ static T f(const Line& line, T x) {\r\n    return line.first * x + line.second;\r\
    \n  }\r\n\r\n  void _add_line(Line line, size_t u, size_t l, size_t r) {\r\n \
    \   if (line.second == INV)\r\n      return;\r\n    auto mid = l + (r-l)/2;\r\n\
    \    bool enabled_left = l < n and cmp(f(seg[u], X[l]), f(line, X[l]));\r\n  \
    \  bool enabled_mid = mid < n and cmp(f(seg[u], X[mid]), f(line, X[mid]));\r\n\
    \    if (enabled_mid)\r\n      std::swap(line, seg[u]);\r\n    if (r-l == 1)\r\
    \n      return;\r\n    if (enabled_left != enabled_mid)\r\n      _add_line(line,\
    \ u<<1, l, mid);\r\n    else\r\n      _add_line(line, (u<<1)|1u, mid, r);\r\n\
    \  }\r\n public:\r\n  void add_line(T a, T b) {\r\n    _add_line({a, b}, 1, 0,\
    \ N);\r\n  }\r\n\r\n  void add_segment_idx(T a, T b, size_t l, size_t r) {\r\n\
    \    auto u = l+N, v = r+N;\r\n    auto L = l, R = r;\r\n    size_t range = 1;\r\
    \n    Line line{a,b};\r\n    while (u < v) {\r\n      if (u&1u) {\r\n        _add_line(line,\
    \ u++, L, L+range);\r\n        L += range;\r\n      }\r\n      if (v&1u) {\r\n\
    \        R -= range;\r\n        _add_line(line, --v, R, R+range);\r\n      }\r\
    \n      u >>= 1;\r\n      v >>= 1;\r\n      range <<= 1;\r\n    }\r\n    assert(L\
    \ == R);\r\n  }\r\n  void add_segment(T a, T b, T xl, T xr) {\r\n    assert(xl\
    \ <= xr);\r\n    auto xlit = std::lower_bound(X.begin(), X.end(), xl);\r\n   \
    \ auto xrit = std::lower_bound(xlit, X.end(), xr);\r\n    add_segment_idx(a, b,\
    \ xlit-X.begin(), xrit-X.begin());\r\n  }\r\n\r\n  T get_idx(T i) const {\r\n\
    \    size_t u = i + N;\r\n    T x = X[i];\r\n    T ret = INV;\r\n    while (u\
    \ > 0) {\r\n      auto v = f(seg[u], x);\r\n      if (cmp(ret, v)) ret = v;\r\n\
    \      u /= 2;\r\n    }\r\n    return ret;\r\n  }\r\n  T get(T x) const {\r\n\
    \    auto it = std::lower_bound(X.begin(), X.end(), x);\r\n    assert(it != X.end());\r\
    \n    return get_idx(it - X.begin());\r\n  }\r\n};\r\ntemplate<typename T, typename\
    \ Comp>\r\nconstexpr T LiChaoTree<T, Comp>::INF;\r\ntemplate<typename T, typename\
    \ Comp>\r\nconstexpr T LiChaoTree<T, Comp>::MINF;\r\ntemplate<typename T, typename\
    \ Comp>\r\nconstexpr T LiChaoTree<T, Comp>::INV;\r\n"
  code: "#pragma once\r\n#include <utility>\r\n#include <vector>\r\n#include <limits>\r\
    \n#include <numeric>\r\n#include <functional>\r\n#include <cstring>\r\n#include\
    \ <iostream>\r\n#include \"bit_manip.hpp\"\r\n\r\ntemplate<typename T, typename\
    \ Comp = std::less<T>>\r\nstruct LiChaoTree {\r\n  using Line = std::pair<T, T>;\r\
    \n  static constexpr T INF = std::numeric_limits<T>::max();\r\n  static constexpr\
    \ T MINF = std::numeric_limits<T>::min();\r\n  static constexpr T INV = Comp()(MINF,\
    \ INF) ? MINF : INF;\r\n  size_t n, N;\r\n  std::vector<T> X;\r\n  std::vector<Line>\
    \ seg;\r\n  Comp cmp;\r\n\r\n  LiChaoTree() = default;\r\n  explicit LiChaoTree(size_t\
    \ _n) : n(_n), N(_n ? 1ull<<(64-bm::clz(_n-1)) : 0), X(n), seg(N*2, {0, INV})\
    \ {\r\n    std::iota(X.begin(), X.begin()+n, 0ull);\r\n  }\r\n  void set_x(size_t\
    \ i, T x) {\r\n    X[i] = x;\r\n  }\r\n  template<typename It>\r\n  LiChaoTree(It\
    \ begin, It end) : LiChaoTree(std::distance(begin, end)) {\r\n    std::copy(begin,\
    \ end, X.begin());\r\n  }\r\n\r\n private:\r\n  inline static T f(const Line&\
    \ line, T x) {\r\n    return line.first * x + line.second;\r\n  }\r\n\r\n  void\
    \ _add_line(Line line, size_t u, size_t l, size_t r) {\r\n    if (line.second\
    \ == INV)\r\n      return;\r\n    auto mid = l + (r-l)/2;\r\n    bool enabled_left\
    \ = l < n and cmp(f(seg[u], X[l]), f(line, X[l]));\r\n    bool enabled_mid = mid\
    \ < n and cmp(f(seg[u], X[mid]), f(line, X[mid]));\r\n    if (enabled_mid)\r\n\
    \      std::swap(line, seg[u]);\r\n    if (r-l == 1)\r\n      return;\r\n    if\
    \ (enabled_left != enabled_mid)\r\n      _add_line(line, u<<1, l, mid);\r\n  \
    \  else\r\n      _add_line(line, (u<<1)|1u, mid, r);\r\n  }\r\n public:\r\n  void\
    \ add_line(T a, T b) {\r\n    _add_line({a, b}, 1, 0, N);\r\n  }\r\n\r\n  void\
    \ add_segment_idx(T a, T b, size_t l, size_t r) {\r\n    auto u = l+N, v = r+N;\r\
    \n    auto L = l, R = r;\r\n    size_t range = 1;\r\n    Line line{a,b};\r\n \
    \   while (u < v) {\r\n      if (u&1u) {\r\n        _add_line(line, u++, L, L+range);\r\
    \n        L += range;\r\n      }\r\n      if (v&1u) {\r\n        R -= range;\r\
    \n        _add_line(line, --v, R, R+range);\r\n      }\r\n      u >>= 1;\r\n \
    \     v >>= 1;\r\n      range <<= 1;\r\n    }\r\n    assert(L == R);\r\n  }\r\n\
    \  void add_segment(T a, T b, T xl, T xr) {\r\n    assert(xl <= xr);\r\n    auto\
    \ xlit = std::lower_bound(X.begin(), X.end(), xl);\r\n    auto xrit = std::lower_bound(xlit,\
    \ X.end(), xr);\r\n    add_segment_idx(a, b, xlit-X.begin(), xrit-X.begin());\r\
    \n  }\r\n\r\n  T get_idx(T i) const {\r\n    size_t u = i + N;\r\n    T x = X[i];\r\
    \n    T ret = INV;\r\n    while (u > 0) {\r\n      auto v = f(seg[u], x);\r\n\
    \      if (cmp(ret, v)) ret = v;\r\n      u /= 2;\r\n    }\r\n    return ret;\r\
    \n  }\r\n  T get(T x) const {\r\n    auto it = std::lower_bound(X.begin(), X.end(),\
    \ x);\r\n    assert(it != X.end());\r\n    return get_idx(it - X.begin());\r\n\
    \  }\r\n};\r\ntemplate<typename T, typename Comp>\r\nconstexpr T LiChaoTree<T,\
    \ Comp>::INF;\r\ntemplate<typename T, typename Comp>\r\nconstexpr T LiChaoTree<T,\
    \ Comp>::MINF;\r\ntemplate<typename T, typename Comp>\r\nconstexpr T LiChaoTree<T,\
    \ Comp>::INV;\r\n"
  dependsOn:
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/li_chao_tree.hpp
  requiredBy: []
  timestamp: '2022-12-18 04:26:00+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/yosupo-line_add_get_min.test.cpp
  - test/yosupo-segment_add_get_min.test.cpp
documentation_of: include/mtl/li_chao_tree.hpp
layout: document
redirect_from:
- /library/include/mtl/li_chao_tree.hpp
- /library/include/mtl/li_chao_tree.hpp.html
title: include/mtl/li_chao_tree.hpp
---
