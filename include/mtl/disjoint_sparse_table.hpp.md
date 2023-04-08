---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
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
    \ 3 \"include/mtl/disjoint_sparse_table.hpp\"\n#include <vector>\n#include <algorithm>\n\
    \ntemplate<typename T, T (*op)(T, T), T (*e)()>\nclass DisjointSparseTable {\n\
    private:\n    int log_n_;\n    size_t n_;\n    std::vector<std::vector<T>> tb_;\n\
    public:\n    template<typename It> \n    DisjointSparseTable(It begin, It end)\
    \ : \n            log_n_(64-bm::clz(std::distance(begin, end)-1)),\n         \
    \   n_(1ull<<log_n_), \n            table_(std::max(log_n_, 1), std::vector<T>(n_,\
    \ e())) {\n        std::transform(begin, end, tb_[0].begin());\n        for (int\
    \ i = 1; i < log_n_; i++) {\n            auto d = 1<<i;\n            for (size_t\
    \ j = 0; j < n_; j += d*2) {\n                tb_[i][j+d-1] = tb_[i-1][j+d-1];\n\
    \                for (long long k = j+d-2; k >= j; k--)\n                    tb_[i][k]\
    \ = op(tb_[i-1][k], tb_[i][k+1]);\n                tb_[i][j+d] = tb_[i-1][j+d];\n\
    \                for (long long k = j+d+1; k < j+d*2; k++)\n                 \
    \   tb_[i][k] = op(tb_[i-1][k], tb_[i][k-1]);\n            }\n        }\n    }\n\
    \n    T query(size_t l, size_t r) const {\n        if (l >= r) return E;\n   \
    \     if (l+1 == r) return tb_[0][l];\n        size_t p = 63-bm::clz((r-1)^l);\n\
    \        return op(tb_[p][l], tb_[p][r-1]);\n    }\n}\n"
  code: "#pragma once\n#include \"bit_manip.hpp\"\n#include <vector>\n#include <algorithm>\n\
    \ntemplate<typename T, T (*op)(T, T), T (*e)()>\nclass DisjointSparseTable {\n\
    private:\n    int log_n_;\n    size_t n_;\n    std::vector<std::vector<T>> tb_;\n\
    public:\n    template<typename It> \n    DisjointSparseTable(It begin, It end)\
    \ : \n            log_n_(64-bm::clz(std::distance(begin, end)-1)),\n         \
    \   n_(1ull<<log_n_), \n            table_(std::max(log_n_, 1), std::vector<T>(n_,\
    \ e())) {\n        std::transform(begin, end, tb_[0].begin());\n        for (int\
    \ i = 1; i < log_n_; i++) {\n            auto d = 1<<i;\n            for (size_t\
    \ j = 0; j < n_; j += d*2) {\n                tb_[i][j+d-1] = tb_[i-1][j+d-1];\n\
    \                for (long long k = j+d-2; k >= j; k--)\n                    tb_[i][k]\
    \ = op(tb_[i-1][k], tb_[i][k+1]);\n                tb_[i][j+d] = tb_[i-1][j+d];\n\
    \                for (long long k = j+d+1; k < j+d*2; k++)\n                 \
    \   tb_[i][k] = op(tb_[i-1][k], tb_[i][k-1]);\n            }\n        }\n    }\n\
    \n    T query(size_t l, size_t r) const {\n        if (l >= r) return E;\n   \
    \     if (l+1 == r) return tb_[0][l];\n        size_t p = 63-bm::clz((r-1)^l);\n\
    \        return op(tb_[p][l], tb_[p][r-1]);\n    }\n}"
  dependsOn:
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/disjoint_sparse_table.hpp
  requiredBy: []
  timestamp: '2023-04-04 01:01:39+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/disjoint_sparse_table.hpp
layout: document
redirect_from:
- /library/include/mtl/disjoint_sparse_table.hpp
- /library/include/mtl/disjoint_sparse_table.hpp.html
title: include/mtl/disjoint_sparse_table.hpp
---
