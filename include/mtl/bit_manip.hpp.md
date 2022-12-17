---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':warning:'
    path: include/mtl/bitmap.hpp
    title: include/mtl/bitmap.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/fenwick_tree.hpp
    title: include/mtl/fenwick_tree.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/fps.hpp
    title: include/mtl/fps.hpp
  - icon: ':warning:'
    path: include/mtl/integer_set.hpp
    title: include/mtl/integer_set.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/lazy_segment_tree.hpp
    title: include/mtl/lazy_segment_tree.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/li_chao_tree.hpp
    title: include/mtl/li_chao_tree.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/ntt.hpp
    title: include/mtl/ntt.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/ordinal_range_search.hpp
    title: include/mtl/ordinal_range_search.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/segment_tree.hpp
    title: include/mtl/segment_tree.hpp
  - icon: ':warning:'
    path: include/mtl/segment_tree_2d.hpp
    title: include/mtl/segment_tree_2d.hpp
  - icon: ':warning:'
    path: include/mtl/skewbinary_list.hpp
    title: include/mtl/skewbinary_list.hpp
  - icon: ':warning:'
    path: include/mtl/skiplist.hpp
    title: include/mtl/skiplist.hpp
  - icon: ':warning:'
    path: include/mtl/sparse_table.hpp
    title: include/mtl/sparse_table.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/bit_vector.hpp
    title: include/mtl/succinct/bit_vector.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/wavelet_matrix.hpp
    title: include/mtl/succinct/wavelet_matrix.hpp
  - icon: ':warning:'
    path: include/mtl/succinct/wavelet_tree.hpp
    title: include/mtl/succinct/wavelet_tree.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/convolution.test.cpp
    title: test/convolution.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/point_add_range_sum.test.cpp
    title: test/point_add_range_sum.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/point_set_range_composite.test.cpp
    title: test/point_set_range_composite.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/range_affine_range_sum.test.cpp
    title: test/range_affine_range_sum.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/static_range_inversions_query.test.cpp
    title: test/static_range_inversions_query.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/static_range_sum.test.cpp
    title: test/static_range_sum.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/static_rmq.test.cpp
    title: test/static_rmq.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/succinct/range_kth_smallest.test.cpp
    title: test/succinct/range_kth_smallest.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo-line_add_get_min.test.cpp
    title: test/yosupo-line_add_get_min.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo-point_add_rectangle_sum-wm.test.cpp
    title: test/yosupo-point_add_rectangle_sum-wm.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo-segment_add_get_min.test.cpp
    title: test/yosupo-segment_add_get_min.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
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
    \ ((x & 0xAAAAAAAAAAAAAAAA) >> 1);\n  return x;\n}\n\n} // namespace bm\n"
  code: "#pragma once\n#include <cstdint>\n#include <cassert>\n\nnamespace bm {\n\n\
    inline constexpr uint64_t popcnt_e8(uint64_t x) {\n  x = (x & 0x5555555555555555)\
    \ + ((x>>1) & 0x5555555555555555);\n  x = (x & 0x3333333333333333) + ((x>>2) &\
    \ 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n\
    \  return x;\n}\n// Count 1s\ninline constexpr unsigned popcnt(uint64_t x) {\n\
    \  return (popcnt_e8(x) * 0x0101010101010101) >> 56;\n}\n// Count trailing 0s.\
    \ ...01101000 -> 3\ninline constexpr unsigned ctz(uint64_t x) {\n  return popcnt((x\
    \ & (-x)) - 1);\n}\ninline constexpr unsigned ctz8(uint8_t x) {\n  return x ==\
    \ 0 ? 8 : popcnt_e8((x & (-x)) - 1);\n}\n// [00..0](8bit) -> 0, [**..*](not only\
    \ 0) -> 1\ninline constexpr uint8_t summary(uint64_t x) {\n  constexpr uint64_t\
    \ hmask = 0x8080808080808080ull;\n  constexpr uint64_t lmask = 0x7F7F7F7F7F7F7F7Full;\n\
    \  auto a = x & hmask;\n  auto b = x & lmask;\n  b = hmask - b;\n  b = ~b;\n \
    \ auto c = (a | b) & hmask;\n  c *= 0x0002040810204081ull;\n  return uint8_t(c\
    \ >> 56);\n}\n// Extract target area of bits\ninline constexpr uint64_t bextr(uint64_t\
    \ x, unsigned start, unsigned len) {\n  uint64_t mask = len < 64 ? (1ull<<len)-1\
    \ : 0xFFFFFFFFFFFFFFFFull;\n  return (x >> start) & mask;\n}\n// 00101101 -> 00111111\
    \ -count_1s-> 6\ninline constexpr unsigned log2p1(uint8_t x) {\n  if (x & 0x80)\n\
    \    return 8;\n  uint64_t p = uint64_t(x) * 0x0101010101010101ull;\n  p -= 0x8040201008040201ull;\n\
    \  p = ~p & 0x8080808080808080ull;\n  p = (p >> 7) * 0x0101010101010101ull;\n\
    \  p >>= 56;\n  return p;\n}\n// 00101100 -mask_mssb-> 00100000 -to_index-> 5\n\
    inline constexpr unsigned mssb8(uint8_t x) {\n  assert(x != 0);\n  return log2p1(x)\
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
    \ 1);\n  return x;\n}\n\n} // namespace bm\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/bit_manip.hpp
  requiredBy:
  - include/mtl/fenwick_tree.hpp
  - include/mtl/fps.hpp
  - include/mtl/skewbinary_list.hpp
  - include/mtl/segment_tree_2d.hpp
  - include/mtl/segment_tree.hpp
  - include/mtl/succinct/bit_vector.hpp
  - include/mtl/succinct/wavelet_matrix.hpp
  - include/mtl/succinct/wavelet_tree.hpp
  - include/mtl/ordinal_range_search.hpp
  - include/mtl/skiplist.hpp
  - include/mtl/sparse_table.hpp
  - include/mtl/ntt.hpp
  - include/mtl/integer_set.hpp
  - include/mtl/bitmap.hpp
  - include/mtl/lazy_segment_tree.hpp
  - include/mtl/li_chao_tree.hpp
  timestamp: '2022-12-18 04:26:00+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/yosupo-line_add_get_min.test.cpp
  - test/succinct/range_kth_smallest.test.cpp
  - test/range_affine_range_sum.test.cpp
  - test/point_set_range_composite.test.cpp
  - test/yosupo-point_add_rectangle_sum-wm.test.cpp
  - test/static_rmq.test.cpp
  - test/convolution.test.cpp
  - test/yosupo-segment_add_get_min.test.cpp
  - test/static_range_sum.test.cpp
  - test/point_add_range_sum.test.cpp
  - test/static_range_inversions_query.test.cpp
documentation_of: include/mtl/bit_manip.hpp
layout: document
redirect_from:
- /library/include/mtl/bit_manip.hpp
- /library/include/mtl/bit_manip.hpp.html
title: include/mtl/bit_manip.hpp
---
