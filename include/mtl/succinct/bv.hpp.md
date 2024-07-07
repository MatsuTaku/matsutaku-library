---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':x:'
    path: include/mtl/ordinal_range_search.hpp
    title: Ordinal Range Search
  - icon: ':x:'
    path: include/mtl/succinct/binary_set.hpp
    title: include/mtl/succinct/binary_set.hpp
  - icon: ':question:'
    path: include/mtl/succinct/bit_vector.hpp
    title: include/mtl/succinct/bit_vector.hpp
  - icon: ':x:'
    path: include/mtl/succinct/rrr.hpp
    title: Succinct bit vector in memory of B(n, u) + O(u log log n / log n) bits
  - icon: ':question:'
    path: include/mtl/succinct/wavelet_matrix.hpp
    title: include/mtl/succinct/wavelet_matrix.hpp
  - icon: ':warning:'
    path: include/mtl/succinct/wavelet_tree.hpp
    title: include/mtl/succinct/wavelet_tree.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/succinct/range_kth_smallest.test.cpp
    title: test/succinct/range_kth_smallest.test.cpp
  - icon: ':x:'
    path: test/yosupo/rectangle_sum.test.cpp
    title: test/yosupo/rectangle_sum.test.cpp
  - icon: ':x:'
    path: test/yosupo/static_range_frequency.test.cpp
    title: test/yosupo/static_range_frequency.test.cpp
  - icon: ':x:'
    path: test/yosupo/static_rectangle_add_rectangle_sum.test.cpp
    title: test/yosupo/static_rectangle_add_rectangle_sum.test.cpp
  - icon: ':x:'
    path: test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
    title: test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':question:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/succinct/bv.hpp\"\n#include <vector>\n#include\
    \ <cstddef>\n#include <cstdint>\n#include <bitset>\n#include <iostream>\n\n#if\
    \ __cpp_concepts >= 202002L\n#include <concepts>\ntemplate<class T>\nconcept ConstructableBV\
    \ = requires(T t, size_t s) {\n  { t.size() } -> std::same_as<size_t>;\n  { t.word_size()\
    \ } -> std::same_as<size_t>;\n  { t.get_word(s) } -> std::convertible_to<uint64_t>;\n\
    };\n#endif\n\ntemplate<\n#if __cpp_concepts >= 202002L\n  ConstructableBV\n#else\n\
    \  class \n#endif\n    BitVec, unsigned WordSize>\nstruct BV {\n  static_assert(WordSize\
    \ <= 64, \"WordSize must be <= 64\");\n  static constexpr unsigned S = WordSize;\n\
    \  static constexpr unsigned S_PER_L = 8;\n  static constexpr unsigned L = S*S_PER_L;\n\
    \  using bitvec_type = BitVec;\n  const bitvec_type* bm_;\n  std::vector<uint64_t>\
    \ _r, _s, _zs;\n  \n  BV() = default;\n  BV(const bitvec_type* bm) {\n    build(bm);\n\
    \  }\n  void set_ptr(const bitvec_type* bm) {\n    bm_ = bm;\n  }\n  void build(const\
    \ bitvec_type* bm) {\n    set_ptr(bm);\n    const auto num_l = (bm->size() + L-1)\
    \ / L;\n    _r.assign((num_l + 1) * 2, 0);\n    _s.clear();\n    _s.push_back(0);\n\
    \    _zs.clear();\n    _zs.push_back(0);\n    uint64_t sum = 0;\n    for (size_t\
    \ l = 0; l < num_l; ++l) {\n      auto psum = sum;\n      uint64_t sum_l = 0;\n\
    \      for (size_t s = 0; s < S_PER_L; ++s) {\n        if (l * S_PER_L + s < bm->word_size())\n\
    \          sum_l += bm::popcnt(bm->get_word(l * S_PER_L + s));\n        if (s\
    \ < S_PER_L-1)\n          _r[l * 2 + 1] |= sum_l << ((7-(s+1)) * 9);\n      }\n\
    \      sum += sum_l;\n      _r[(l + 1) * 2] = sum;\n      if (psum / L != sum\
    \ / L) {\n        _s.push_back(l);\n      }\n      if ((L*l - psum) / L != (L*(l+1)\
    \ - sum) / L) {\n        _zs.push_back(l);\n      }\n    }\n    _s.push_back(num_l);\n\
    \    _zs.push_back(num_l);\n  }\n\n  template<bool B>\n  size_t get_l(size_t l)\
    \ const {\n    auto b = _r[l*2];\n    return B ? b : l * L - b;\n  }\n  static\
    \ size_t s_off(size_t s) {\n    return (7-s) * 9;\n  }\n  template<bool B>\n \
    \ size_t get_s(size_t l, size_t s) const {\n    auto b = (_r[l*2+1] >> s_off(s))\
    \ % (1ull<<9);\n    return B ? b : s * S - b;\n  }\n  uint64_t mask(size_t width)\
    \ const {\n    return width == 64 ? ~0ull : (1ull << width) - 1;\n  }\n  size_t\
    \ rank1(size_t i) const {\n    auto l = i / L;\n    auto s = i % L / S;\n    auto\
    \ q = i / S;\n    auto r = i % S;\n    auto w = bm_->get_word(q) & mask(r);\n\
    \    return get_l<1>(l) + \n           get_s<1>(l, s) + \n           bm::popcnt(w);\n\
    \  }\n  size_t rank0(size_t i) const { \n    return i - rank1(i);\n  }\n  template<bool\
    \ B>\n  size_t rank(size_t i) const {\n    if constexpr (B)\n      return rank1(i);\n\
    \    else\n      return rank0(i);\n  }\n\n  static struct l_block_cap_mask {\n\
    \    uint64_t mask;\n    constexpr l_block_cap_mask() {\n      mask = 0;\n   \
    \   for (unsigned i = 0; i < S_PER_L; i++) {\n        uint64_t cap = i * S;\n\
    \        mask |= cap << s_off(i);\n      }\n    }\n  } l_block_cap;\n\n  template<bool\
    \ B>\n  size_t select(size_t ith) const {\n    auto n = ith+1; // to 1-indexed\n\
    \    if (n > rank<B>(bm_->size()))\n      return bm_->size();\n    // Find L block\n\
    \    auto& idx = B ? _s : _zs;\n    size_t l = idx[n / L];\n    {\n      auto\
    \ r = idx[n / L + 1] + 1;\n      while (l+1 < r) {\n        auto c = l + (r-l)/2;\n\
    \        if (get_l<B>(c) < n)\n          l = c;\n        else\n          r = c;\n\
    \      }\n    }\n    // Find S block\n    size_t s = 0;\n    auto m = n - get_l<B>(l);\n\
    \    /* Following bit-manipulates code is same as ... */\n//    {\n//      auto\
    \ d = 8;\n//      while (d > 1) {\n//        auto c = s + d/2;\n//        if (get_s<B>(l,\
    \ c) < m)\n//          s = c;\n//        d /= 2;\n//      }\n//    }\n    {\n\
    \      uint64_t x = (uint64_t) (m-1) * 0x0040201008040201ull;\n      uint64_t\
    \ a = _r[l*2+1];\n      if constexpr (!B)\n        a = l_block_cap.mask - a; //\
    \ to 0s sum\n      uint64_t xda = x - a;\n      uint64_t sm = 0x4020100804020100ull;\n\
    \      uint64_t ok = (~x | a) & sm;\n      uint64_t ng = (~x & a) & sm;\n    \
    \  uint64_t y = ((x ^ a ^ xda) & ok) | ng;\n      y = y * 0x0001010101010101ull\
    \ >> (64-1-7);\n      auto id = bm::clz8(y)-1;\n      auto clo = bm::clz((~xda\
    \ << 1 | 1) << (9*id));\n      auto ns = id + (clo ? (clo - 1) / 9 : 0);\n   \
    \   s = ns;\n    }\n    // Calc select\n    auto o = m - get_s<B>(l, s);\n   \
    \ uint64_t w = bm_->get_word(l * S_PER_L + s);\n    return l * L + \n        \
    \   s * S + \n           select64::select<B>(o-1, w);\n  }\n  size_t select1(size_t\
    \ ith) const {\n    return select<1>(ith);\n  }\n  size_t select0(size_t ith)\
    \ const {\n    return select<0>(ith);\n  }\n};\n\ntemplate<class BitVec, unsigned\
    \ WordSize>\ntypename BV<BitVec, WordSize>::l_block_cap_mask BV<BitVec, WordSize>::l_block_cap;\n\
    \ntemplate<class T>\nstruct RankSelectTraits : std::false_type {};\n"
  code: "#pragma once\n#include <vector>\n#include <cstddef>\n#include <cstdint>\n\
    #include <bitset>\n#include <iostream>\n\n#if __cpp_concepts >= 202002L\n#include\
    \ <concepts>\ntemplate<class T>\nconcept ConstructableBV = requires(T t, size_t\
    \ s) {\n  { t.size() } -> std::same_as<size_t>;\n  { t.word_size() } -> std::same_as<size_t>;\n\
    \  { t.get_word(s) } -> std::convertible_to<uint64_t>;\n};\n#endif\n\ntemplate<\n\
    #if __cpp_concepts >= 202002L\n  ConstructableBV\n#else\n  class \n#endif\n  \
    \  BitVec, unsigned WordSize>\nstruct BV {\n  static_assert(WordSize <= 64, \"\
    WordSize must be <= 64\");\n  static constexpr unsigned S = WordSize;\n  static\
    \ constexpr unsigned S_PER_L = 8;\n  static constexpr unsigned L = S*S_PER_L;\n\
    \  using bitvec_type = BitVec;\n  const bitvec_type* bm_;\n  std::vector<uint64_t>\
    \ _r, _s, _zs;\n  \n  BV() = default;\n  BV(const bitvec_type* bm) {\n    build(bm);\n\
    \  }\n  void set_ptr(const bitvec_type* bm) {\n    bm_ = bm;\n  }\n  void build(const\
    \ bitvec_type* bm) {\n    set_ptr(bm);\n    const auto num_l = (bm->size() + L-1)\
    \ / L;\n    _r.assign((num_l + 1) * 2, 0);\n    _s.clear();\n    _s.push_back(0);\n\
    \    _zs.clear();\n    _zs.push_back(0);\n    uint64_t sum = 0;\n    for (size_t\
    \ l = 0; l < num_l; ++l) {\n      auto psum = sum;\n      uint64_t sum_l = 0;\n\
    \      for (size_t s = 0; s < S_PER_L; ++s) {\n        if (l * S_PER_L + s < bm->word_size())\n\
    \          sum_l += bm::popcnt(bm->get_word(l * S_PER_L + s));\n        if (s\
    \ < S_PER_L-1)\n          _r[l * 2 + 1] |= sum_l << ((7-(s+1)) * 9);\n      }\n\
    \      sum += sum_l;\n      _r[(l + 1) * 2] = sum;\n      if (psum / L != sum\
    \ / L) {\n        _s.push_back(l);\n      }\n      if ((L*l - psum) / L != (L*(l+1)\
    \ - sum) / L) {\n        _zs.push_back(l);\n      }\n    }\n    _s.push_back(num_l);\n\
    \    _zs.push_back(num_l);\n  }\n\n  template<bool B>\n  size_t get_l(size_t l)\
    \ const {\n    auto b = _r[l*2];\n    return B ? b : l * L - b;\n  }\n  static\
    \ size_t s_off(size_t s) {\n    return (7-s) * 9;\n  }\n  template<bool B>\n \
    \ size_t get_s(size_t l, size_t s) const {\n    auto b = (_r[l*2+1] >> s_off(s))\
    \ % (1ull<<9);\n    return B ? b : s * S - b;\n  }\n  uint64_t mask(size_t width)\
    \ const {\n    return width == 64 ? ~0ull : (1ull << width) - 1;\n  }\n  size_t\
    \ rank1(size_t i) const {\n    auto l = i / L;\n    auto s = i % L / S;\n    auto\
    \ q = i / S;\n    auto r = i % S;\n    auto w = bm_->get_word(q) & mask(r);\n\
    \    return get_l<1>(l) + \n           get_s<1>(l, s) + \n           bm::popcnt(w);\n\
    \  }\n  size_t rank0(size_t i) const { \n    return i - rank1(i);\n  }\n  template<bool\
    \ B>\n  size_t rank(size_t i) const {\n    if constexpr (B)\n      return rank1(i);\n\
    \    else\n      return rank0(i);\n  }\n\n  static struct l_block_cap_mask {\n\
    \    uint64_t mask;\n    constexpr l_block_cap_mask() {\n      mask = 0;\n   \
    \   for (unsigned i = 0; i < S_PER_L; i++) {\n        uint64_t cap = i * S;\n\
    \        mask |= cap << s_off(i);\n      }\n    }\n  } l_block_cap;\n\n  template<bool\
    \ B>\n  size_t select(size_t ith) const {\n    auto n = ith+1; // to 1-indexed\n\
    \    if (n > rank<B>(bm_->size()))\n      return bm_->size();\n    // Find L block\n\
    \    auto& idx = B ? _s : _zs;\n    size_t l = idx[n / L];\n    {\n      auto\
    \ r = idx[n / L + 1] + 1;\n      while (l+1 < r) {\n        auto c = l + (r-l)/2;\n\
    \        if (get_l<B>(c) < n)\n          l = c;\n        else\n          r = c;\n\
    \      }\n    }\n    // Find S block\n    size_t s = 0;\n    auto m = n - get_l<B>(l);\n\
    \    /* Following bit-manipulates code is same as ... */\n//    {\n//      auto\
    \ d = 8;\n//      while (d > 1) {\n//        auto c = s + d/2;\n//        if (get_s<B>(l,\
    \ c) < m)\n//          s = c;\n//        d /= 2;\n//      }\n//    }\n    {\n\
    \      uint64_t x = (uint64_t) (m-1) * 0x0040201008040201ull;\n      uint64_t\
    \ a = _r[l*2+1];\n      if constexpr (!B)\n        a = l_block_cap.mask - a; //\
    \ to 0s sum\n      uint64_t xda = x - a;\n      uint64_t sm = 0x4020100804020100ull;\n\
    \      uint64_t ok = (~x | a) & sm;\n      uint64_t ng = (~x & a) & sm;\n    \
    \  uint64_t y = ((x ^ a ^ xda) & ok) | ng;\n      y = y * 0x0001010101010101ull\
    \ >> (64-1-7);\n      auto id = bm::clz8(y)-1;\n      auto clo = bm::clz((~xda\
    \ << 1 | 1) << (9*id));\n      auto ns = id + (clo ? (clo - 1) / 9 : 0);\n   \
    \   s = ns;\n    }\n    // Calc select\n    auto o = m - get_s<B>(l, s);\n   \
    \ uint64_t w = bm_->get_word(l * S_PER_L + s);\n    return l * L + \n        \
    \   s * S + \n           select64::select<B>(o-1, w);\n  }\n  size_t select1(size_t\
    \ ith) const {\n    return select<1>(ith);\n  }\n  size_t select0(size_t ith)\
    \ const {\n    return select<0>(ith);\n  }\n};\n\ntemplate<class BitVec, unsigned\
    \ WordSize>\ntypename BV<BitVec, WordSize>::l_block_cap_mask BV<BitVec, WordSize>::l_block_cap;\n\
    \ntemplate<class T>\nstruct RankSelectTraits : std::false_type {};"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/succinct/bv.hpp
  requiredBy:
  - include/mtl/ordinal_range_search.hpp
  - include/mtl/succinct/binary_set.hpp
  - include/mtl/succinct/wavelet_tree.hpp
  - include/mtl/succinct/wavelet_matrix.hpp
  - include/mtl/succinct/bit_vector.hpp
  - include/mtl/succinct/rrr.hpp
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_SOME_WA
  verifiedWith:
  - test/yosupo/static_rectangle_add_rectangle_sum.test.cpp
  - test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
  - test/yosupo/rectangle_sum.test.cpp
  - test/yosupo/static_range_frequency.test.cpp
  - test/succinct/range_kth_smallest.test.cpp
documentation_of: include/mtl/succinct/bv.hpp
layout: document
redirect_from:
- /library/include/mtl/succinct/bv.hpp
- /library/include/mtl/succinct/bv.hpp.html
title: include/mtl/succinct/bv.hpp
---
