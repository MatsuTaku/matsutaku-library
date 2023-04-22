---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: include/mtl/ordinal_range_search.hpp
    title: Ordinal Range Search
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/binary_set.hpp
    title: include/mtl/succinct/binary_set.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/bit_vector.hpp
    title: include/mtl/succinct/bit_vector.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/rrr.hpp
    title: Succinct bit vector in memory of B(n, u) + O(u log log n / log n) bits
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/wavelet_matrix.hpp
    title: include/mtl/succinct/wavelet_matrix.hpp
  - icon: ':warning:'
    path: include/mtl/succinct/wavelet_tree.hpp
    title: include/mtl/succinct/wavelet_tree.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/succinct/range_kth_smallest.test.cpp
    title: test/succinct/range_kth_smallest.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/rectangle_sum.test.cpp
    title: test/yosupo/rectangle_sum.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/static_range_frequency.test.cpp
    title: test/yosupo/static_range_frequency.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/static_rectangle_add_rectangle_sum.test.cpp
    title: test/yosupo/static_rectangle_add_rectangle_sum.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
    title: test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
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
    \ ((x & 0xAAAAAAAAAAAAAAAA) >> 1);\n  return x;\n}\n\n} // namespace bm\n#line\
    \ 3 \"include/mtl/succinct/select.hpp\"\n#include <array>\n\nstruct select64 {\n\
    \    using size_type = uint8_t;\n    static struct make_select_table {\n     \
    \   using bitmap_type = uint8_t;\n        std::array<std::array<size_type, 9>,\
    \ 1<<8> tb;\n        make_select_table() {\n            for (int i = 0; i < 1<<8;\
    \ i++) {\n                int c = 0;\n                int x = i;\n           \
    \     tb[i].fill(8);\n                for (int j = 0; j < 8; j++) {\n        \
    \            if (x & 1)\n                    tb[i][++c] = j;\n               \
    \     x >>= 1;\n                }\n            }\n        }\n        size_type\
    \ operator()(bitmap_type bitmap, size_type ith) const {\n            return tb[bitmap][ith];\n\
    \        }\n    } sel_tb;\n    template<bool B>\n    static constexpr size_type\
    \ select(size_type ith, uint64_t bitmap) { // 0-indexed\n        assert(ith <\
    \ 64);\n        ith++; // to 1-index\n        // Find byte\n        uint64_t w\
    \ = bitmap;\n        if constexpr (!B) w = ~w;\n        auto _bs = (uint64_t)\
    \ bm::popcnt_e8(w) * 0x0101010101010100ull;\n        auto bs = (const uint8_t*)\
    \ &_bs;\n        size_type b = 0;\n        auto o = ith;\n        /* Following\
    \ bit-manipulates code is same as ... */\n        // {\n        //     auto d\
    \ = 8;\n        //     while (d > 1) {\n        //     auto c = b + d/2;\n   \
    \     //     if (bs[c] < o)\n        //         b = c;\n        //     d /= 2;\n\
    \        //     }\n        // }\n        {\n            uint64_t x = (uint64_t)\
    \ o * 0x0101010101010101ull;\n            uint64_t bmx = (_bs | 0x8080808080808080ull)\
    \ - x;\n            uint64_t y = ((bmx & 0x8080808080808080ull) * 0x02040810204081ull)\
    \ >> (64-8);\n            size_type nb = bm::ctz8(y) - 1;\n            // assert(b\
    \ == nb);\n            b = nb;\n        }\n        // Calc select\n        auto\
    \ r = o - bs[b];\n        uint8_t byte = ((const uint8_t*)(&w))[b];\n        assert(r\
    \ and r <= (size_type)bm::popcnt(byte));\n        return b * 8 + sel_tb(byte,\
    \ r);\n    }\n    static constexpr size_type select1(size_type ith, uint64_t bitmap)\
    \ {\n        return select<1>(ith, bitmap);\n    }\n    static constexpr size_type\
    \ select0(size_type ith, uint64_t bitmap) {\n        return select<0>(ith, bitmap);\n\
    \    }\n};\n\ntypename select64::make_select_table select64::sel_tb;\n"
  code: "#pragma once\n#include \"../bit_manip.hpp\"\n#include <array>\n\nstruct select64\
    \ {\n    using size_type = uint8_t;\n    static struct make_select_table {\n \
    \       using bitmap_type = uint8_t;\n        std::array<std::array<size_type,\
    \ 9>, 1<<8> tb;\n        make_select_table() {\n            for (int i = 0; i\
    \ < 1<<8; i++) {\n                int c = 0;\n                int x = i;\n   \
    \             tb[i].fill(8);\n                for (int j = 0; j < 8; j++) {\n\
    \                    if (x & 1)\n                    tb[i][++c] = j;\n       \
    \             x >>= 1;\n                }\n            }\n        }\n        size_type\
    \ operator()(bitmap_type bitmap, size_type ith) const {\n            return tb[bitmap][ith];\n\
    \        }\n    } sel_tb;\n    template<bool B>\n    static constexpr size_type\
    \ select(size_type ith, uint64_t bitmap) { // 0-indexed\n        assert(ith <\
    \ 64);\n        ith++; // to 1-index\n        // Find byte\n        uint64_t w\
    \ = bitmap;\n        if constexpr (!B) w = ~w;\n        auto _bs = (uint64_t)\
    \ bm::popcnt_e8(w) * 0x0101010101010100ull;\n        auto bs = (const uint8_t*)\
    \ &_bs;\n        size_type b = 0;\n        auto o = ith;\n        /* Following\
    \ bit-manipulates code is same as ... */\n        // {\n        //     auto d\
    \ = 8;\n        //     while (d > 1) {\n        //     auto c = b + d/2;\n   \
    \     //     if (bs[c] < o)\n        //         b = c;\n        //     d /= 2;\n\
    \        //     }\n        // }\n        {\n            uint64_t x = (uint64_t)\
    \ o * 0x0101010101010101ull;\n            uint64_t bmx = (_bs | 0x8080808080808080ull)\
    \ - x;\n            uint64_t y = ((bmx & 0x8080808080808080ull) * 0x02040810204081ull)\
    \ >> (64-8);\n            size_type nb = bm::ctz8(y) - 1;\n            // assert(b\
    \ == nb);\n            b = nb;\n        }\n        // Calc select\n        auto\
    \ r = o - bs[b];\n        uint8_t byte = ((const uint8_t*)(&w))[b];\n        assert(r\
    \ and r <= (size_type)bm::popcnt(byte));\n        return b * 8 + sel_tb(byte,\
    \ r);\n    }\n    static constexpr size_type select1(size_type ith, uint64_t bitmap)\
    \ {\n        return select<1>(ith, bitmap);\n    }\n    static constexpr size_type\
    \ select0(size_type ith, uint64_t bitmap) {\n        return select<0>(ith, bitmap);\n\
    \    }\n};\n\ntypename select64::make_select_table select64::sel_tb;"
  dependsOn:
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/succinct/select.hpp
  requiredBy:
  - include/mtl/succinct/wavelet_matrix.hpp
  - include/mtl/succinct/binary_set.hpp
  - include/mtl/succinct/wavelet_tree.hpp
  - include/mtl/succinct/bit_vector.hpp
  - include/mtl/succinct/rrr.hpp
  - include/mtl/ordinal_range_search.hpp
  timestamp: '2023-04-10 16:54:08+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/succinct/range_kth_smallest.test.cpp
  - test/yosupo/static_rectangle_add_rectangle_sum.test.cpp
  - test/yosupo/static_range_frequency.test.cpp
  - test/yosupo/rectangle_sum.test.cpp
  - test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
documentation_of: include/mtl/succinct/select.hpp
layout: document
redirect_from:
- /library/include/mtl/succinct/select.hpp
- /library/include/mtl/succinct/select.hpp.html
title: include/mtl/succinct/select.hpp
---
