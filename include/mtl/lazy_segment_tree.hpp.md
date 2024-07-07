---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp
    title: test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp
  - icon: ':x:'
    path: test/yosupo/range_affine_range_sum.test.cpp
    title: test/yosupo/range_affine_range_sum.test.cpp
  - icon: ':x:'
    path: test/yosupo/static_range_inversions_query-mo_rollback.test.cpp
    title: test/yosupo/static_range_inversions_query-mo_rollback.test.cpp
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
    \ 3 \"include/mtl/lazy_segment_tree.hpp\"\n// #include \"monoid.hpp\"\n#include\
    \ <cstddef>\n#include <utility>\n#include <vector>\n#include <stack>\n#line 9\
    \ \"include/mtl/lazy_segment_tree.hpp\"\n#if __cpp_concepts >= 202002L\n#include\
    \ <concepts>\n\ntemplate<typename M>\nconcept LazySegmentTreeMonoid = requires\
    \ (M m) {\n  {m * m} -> std::same_as<M>;\n};\ntemplate<typename A, typename M>\n\
    concept LazySegmentTreeOperatorMonoid = requires(A a, M m) {\n  {a *= a} -> std::same_as<A&>;\n\
    \  {a.act(m)} -> std::same_as<M>;\n};\n#endif\n\ntemplate <typename M, typename\
    \ A>\n#if __cpp_concepts >= 202002L\nrequires LazySegmentTreeMonoid<M> &&\n  \
    \       LazySegmentTreeOperatorMonoid<A,M>\n#endif\nclass LazySegmentTree {\n\
    \ private:\n  size_t size_;\n  std::vector<std::pair<M, A>> tree_;\n  std::vector<size_t>\
    \ ids_;\n\n public:\n  explicit LazySegmentTree(size_t size) :\n      size_(1ull<<(64-bm::clz(size-1))),\n\
    \      tree_(size_*2) {\n    ids_.reserve((64-bm::clz(size-1))*2);\n  }\n\n  template\
    \ <typename Iter>\n  explicit LazySegmentTree(Iter begin, Iter end)\n    : LazySegmentTree(std::distance(begin,\
    \ end)) {\n    static_assert(std::is_convertible<typename std::iterator_traits<Iter>::value_type,\
    \ M>::value, \"\");\n    for (auto it = begin; it != end; ++it) {\n      tree_[size_\
    \ + it - begin].first = *it;\n    }\n    for (size_t i = size_-1; i > 0; i--)\
    \ {\n      tree_[i].first = tree_[i*2].first * tree_[i*2+1].first;\n    }\n  }\n\
    \n  inline void range_update(size_t l, size_t r, const A& e) {\n    assert(l <=\
    \ r);\n    assert(r <= size_);\n    if (l == r) return;\n    _lazy_propagation(l,\
    \ r);\n\n    for (size_t _l=l+size_, _r=r+size_; _l<_r; _l>>=1, _r>>=1) {\n  \
    \    if (_l&1) \n        tree_[_l++].second *= e;\n      if (_r&1) \n        tree_[--_r].second\
    \ *= e;\n    }\n\n    for (auto id : ids_) {\n      _propagate(id*2);\n      _propagate(id*2+1);\n\
    \      tree_[id].first = tree_[id*2].first * tree_[id*2+1].first;\n    }\n  }\n\
    \  inline void update(size_t l, size_t r, const A& e) {\n    range_update(l, r,\
    \ e);\n  }\n  inline void update(size_t i, const A& e) {\n    range_update(i,\
    \ i+1, e);\n  }\n\n  template<typename T>\n  inline void set(size_t i, T&& e)\
    \ {\n    _lazy_propagation(i, i+1);\n    int u = i+size_;\n    tree_[u].first\
    \ = M(std::forward<T>(e));\n    u >>= 1;\n    while (u > 0) {\n      tree_[u].first\
    \ = tree_[u*2].first * tree_[u*2+1].first;\n      u >>= 1;\n    }\n  }\n\n  inline\
    \ M query(size_t l, size_t r) {\n    _lazy_propagation(l, r);\n\n    M lhs,rhs;\n\
    \    for (size_t _l=l+size_, _r=r+size_; _l<_r; _l>>=1, _r>>=1) {\n      if (_l&1)\
    \ {\n        _propagate(_l);\n        lhs = lhs * tree_[_l].first;\n        ++_l;\n\
    \      }\n      if (_r&1) {\n        --_r;\n        _propagate(_r);\n        rhs\
    \ = tree_[_r].first * rhs;\n      }\n    }\n    return lhs * rhs;\n  }\n  ///\
    \ Alias for query(l, r)\n  M prod(size_t l, size_t r) {\n    return query(l, r);\n\
    \  }\n\n  inline const M& get(size_t index) {\n    _lazy_propagation(index, index+1);\n\
    \    auto l = index+size_;\n    _propagate(l);\n    return tree_[l].first;\n \
    \ }\n\n private:\n  void _set_ids(size_t l, size_t r) {\n    ids_.clear();\n \
    \   auto _l=l+size_, _r=r+size_;\n    auto lth = _l/(_l&(-_l))/2;\n    auto rth\
    \ = _r/(_r&(-_r))/2;\n    for (; _l<_r; _l>>=1, _r>>=1) {\n      if (_r <= rth)\
    \ ids_.push_back(_r);\n      if (_l <= lth) ids_.push_back(_l);\n    }\n    for\
    \ (; _l>0; _l>>=1)\n      ids_.push_back(_l);\n  }\n\n  inline void _propagate(size_t\
    \ id) {\n    A& e = tree_[id].second;\n    tree_[id].first = e.act(tree_[id].first);\n\
    \    if (id < size_) {\n      tree_[id*2].second *= e;\n      tree_[id*2+1].second\
    \ *= e;\n    }\n    tree_[id].second = A();\n  }\n\n  void _lazy_propagation(size_t\
    \ l, size_t r) {\n    if (l == r) return;\n    _set_ids(l, r);\n    for (auto\
    \ it = ids_.rbegin(); it != ids_.rend(); ++it)\n      _propagate(*it);\n  }\n\n\
    \ public:\n  template<class F>\n  size_t max_right(size_t begin, size_t end, F\
    \ f) {\n    if (begin == end) return end;\n    M p;\n    std::stack<std::pair<size_t,\
    \ M>> rps;\n    auto l = size_ + begin;\n    auto r = size_ + end;\n    _lazy_propagation(begin,\
    \ end);\n    auto access = [&](size_t i) {\n      _propagate(i);\n      return\
    \ tree_[i].first;\n    };\n    while (l < r and f(p * access(l))) {\n      if\
    \ (l&1) p = p * tree_[l++].first;\n      if (r&1) {\n        rps.emplace(r, access(r-1));\n\
    \        r--;\n      }\n      l>>=1; r>>=1;\n    }\n    if (l >= r) {\n      while\
    \ (rps.size()) {\n        auto& [r, rp] = rps.top();\n        if (!f(p * rp))\
    \ {\n          l = r-1;\n          break;\n        }\n        p = p * rp;\n  \
    \      rps.pop();\n      }\n      if (rps.empty()) return end;\n    }\n    while\
    \ (l < size_) {\n      assert(!f(p * access(l)));\n      l <<= 1;\n      auto\
    \ pl = access(l);\n      if (f(p * pl)) {\n        p = p * pl;\n        l++;\n\
    \      }\n    }\n    return l - size_;\n  }\n  template<bool (*F)(M)>\n  size_t\
    \ max_right(size_t begin, size_t end) {\n    return max_right(begin, end, [](M\
    \ x) { return F(x); });\n  }\n\n  template<class F>\n  size_t min_left(size_t\
    \ begin, size_t end, F f) {\n    if (end == begin) return begin;\n    M p;\n \
    \   std::stack<std::pair<size_t, M>> lps;\n    auto l = size_ + begin;\n    auto\
    \ r = size_ + end;\n    _lazy_propagation(begin, end);\n    auto access = [&](size_t\
    \ i) {\n      _propagate(i);\n      return tree_[i].first;\n    };\n    while\
    \ (l < r and f(access(r-1) * p)) {\n      if (l&1) {\n        lps.emplace(l, access(l));\n\
    \        l++;\n      }\n      if (r&1) p = tree_[r-1].first * p;\n      l>>=1;\
    \ r>>=1;\n    }\n    if (l >= r) {\n      while (lps.size()) {\n        auto&\
    \ [l, lp] = lps.top();\n        if (!f(lp * p)) {\n          r = l+1;\n      \
    \    break;\n        }\n        p = lp * p;\n        lps.pop();\n      }\n   \
    \   if (lps.empty()) return begin;\n    }\n    while (r <= size_) {\n      assert(!f(access(r-1)\
    \ * p));\n      r <<= 1;\n      auto pr = access(r-1);\n      if (f(pr * p)) {\n\
    \        p = pr * p;\n        --r;\n      }\n    }\n    return r - size_;\n  }\n\
    \  template<bool (*F)(M)>\n  size_t min_left(size_t begin, size_t end) {\n   \
    \ return min_left(begin, [](M x) { return F(x); });\n  }\n\n};\n\n"
  code: "#pragma once\n#include \"bit_manip.hpp\"\n// #include \"monoid.hpp\"\n#include\
    \ <cstddef>\n#include <utility>\n#include <vector>\n#include <stack>\n#include\
    \ <cassert>\n#if __cpp_concepts >= 202002L\n#include <concepts>\n\ntemplate<typename\
    \ M>\nconcept LazySegmentTreeMonoid = requires (M m) {\n  {m * m} -> std::same_as<M>;\n\
    };\ntemplate<typename A, typename M>\nconcept LazySegmentTreeOperatorMonoid =\
    \ requires(A a, M m) {\n  {a *= a} -> std::same_as<A&>;\n  {a.act(m)} -> std::same_as<M>;\n\
    };\n#endif\n\ntemplate <typename M, typename A>\n#if __cpp_concepts >= 202002L\n\
    requires LazySegmentTreeMonoid<M> &&\n         LazySegmentTreeOperatorMonoid<A,M>\n\
    #endif\nclass LazySegmentTree {\n private:\n  size_t size_;\n  std::vector<std::pair<M,\
    \ A>> tree_;\n  std::vector<size_t> ids_;\n\n public:\n  explicit LazySegmentTree(size_t\
    \ size) :\n      size_(1ull<<(64-bm::clz(size-1))),\n      tree_(size_*2) {\n\
    \    ids_.reserve((64-bm::clz(size-1))*2);\n  }\n\n  template <typename Iter>\n\
    \  explicit LazySegmentTree(Iter begin, Iter end)\n    : LazySegmentTree(std::distance(begin,\
    \ end)) {\n    static_assert(std::is_convertible<typename std::iterator_traits<Iter>::value_type,\
    \ M>::value, \"\");\n    for (auto it = begin; it != end; ++it) {\n      tree_[size_\
    \ + it - begin].first = *it;\n    }\n    for (size_t i = size_-1; i > 0; i--)\
    \ {\n      tree_[i].first = tree_[i*2].first * tree_[i*2+1].first;\n    }\n  }\n\
    \n  inline void range_update(size_t l, size_t r, const A& e) {\n    assert(l <=\
    \ r);\n    assert(r <= size_);\n    if (l == r) return;\n    _lazy_propagation(l,\
    \ r);\n\n    for (size_t _l=l+size_, _r=r+size_; _l<_r; _l>>=1, _r>>=1) {\n  \
    \    if (_l&1) \n        tree_[_l++].second *= e;\n      if (_r&1) \n        tree_[--_r].second\
    \ *= e;\n    }\n\n    for (auto id : ids_) {\n      _propagate(id*2);\n      _propagate(id*2+1);\n\
    \      tree_[id].first = tree_[id*2].first * tree_[id*2+1].first;\n    }\n  }\n\
    \  inline void update(size_t l, size_t r, const A& e) {\n    range_update(l, r,\
    \ e);\n  }\n  inline void update(size_t i, const A& e) {\n    range_update(i,\
    \ i+1, e);\n  }\n\n  template<typename T>\n  inline void set(size_t i, T&& e)\
    \ {\n    _lazy_propagation(i, i+1);\n    int u = i+size_;\n    tree_[u].first\
    \ = M(std::forward<T>(e));\n    u >>= 1;\n    while (u > 0) {\n      tree_[u].first\
    \ = tree_[u*2].first * tree_[u*2+1].first;\n      u >>= 1;\n    }\n  }\n\n  inline\
    \ M query(size_t l, size_t r) {\n    _lazy_propagation(l, r);\n\n    M lhs,rhs;\n\
    \    for (size_t _l=l+size_, _r=r+size_; _l<_r; _l>>=1, _r>>=1) {\n      if (_l&1)\
    \ {\n        _propagate(_l);\n        lhs = lhs * tree_[_l].first;\n        ++_l;\n\
    \      }\n      if (_r&1) {\n        --_r;\n        _propagate(_r);\n        rhs\
    \ = tree_[_r].first * rhs;\n      }\n    }\n    return lhs * rhs;\n  }\n  ///\
    \ Alias for query(l, r)\n  M prod(size_t l, size_t r) {\n    return query(l, r);\n\
    \  }\n\n  inline const M& get(size_t index) {\n    _lazy_propagation(index, index+1);\n\
    \    auto l = index+size_;\n    _propagate(l);\n    return tree_[l].first;\n \
    \ }\n\n private:\n  void _set_ids(size_t l, size_t r) {\n    ids_.clear();\n \
    \   auto _l=l+size_, _r=r+size_;\n    auto lth = _l/(_l&(-_l))/2;\n    auto rth\
    \ = _r/(_r&(-_r))/2;\n    for (; _l<_r; _l>>=1, _r>>=1) {\n      if (_r <= rth)\
    \ ids_.push_back(_r);\n      if (_l <= lth) ids_.push_back(_l);\n    }\n    for\
    \ (; _l>0; _l>>=1)\n      ids_.push_back(_l);\n  }\n\n  inline void _propagate(size_t\
    \ id) {\n    A& e = tree_[id].second;\n    tree_[id].first = e.act(tree_[id].first);\n\
    \    if (id < size_) {\n      tree_[id*2].second *= e;\n      tree_[id*2+1].second\
    \ *= e;\n    }\n    tree_[id].second = A();\n  }\n\n  void _lazy_propagation(size_t\
    \ l, size_t r) {\n    if (l == r) return;\n    _set_ids(l, r);\n    for (auto\
    \ it = ids_.rbegin(); it != ids_.rend(); ++it)\n      _propagate(*it);\n  }\n\n\
    \ public:\n  template<class F>\n  size_t max_right(size_t begin, size_t end, F\
    \ f) {\n    if (begin == end) return end;\n    M p;\n    std::stack<std::pair<size_t,\
    \ M>> rps;\n    auto l = size_ + begin;\n    auto r = size_ + end;\n    _lazy_propagation(begin,\
    \ end);\n    auto access = [&](size_t i) {\n      _propagate(i);\n      return\
    \ tree_[i].first;\n    };\n    while (l < r and f(p * access(l))) {\n      if\
    \ (l&1) p = p * tree_[l++].first;\n      if (r&1) {\n        rps.emplace(r, access(r-1));\n\
    \        r--;\n      }\n      l>>=1; r>>=1;\n    }\n    if (l >= r) {\n      while\
    \ (rps.size()) {\n        auto& [r, rp] = rps.top();\n        if (!f(p * rp))\
    \ {\n          l = r-1;\n          break;\n        }\n        p = p * rp;\n  \
    \      rps.pop();\n      }\n      if (rps.empty()) return end;\n    }\n    while\
    \ (l < size_) {\n      assert(!f(p * access(l)));\n      l <<= 1;\n      auto\
    \ pl = access(l);\n      if (f(p * pl)) {\n        p = p * pl;\n        l++;\n\
    \      }\n    }\n    return l - size_;\n  }\n  template<bool (*F)(M)>\n  size_t\
    \ max_right(size_t begin, size_t end) {\n    return max_right(begin, end, [](M\
    \ x) { return F(x); });\n  }\n\n  template<class F>\n  size_t min_left(size_t\
    \ begin, size_t end, F f) {\n    if (end == begin) return begin;\n    M p;\n \
    \   std::stack<std::pair<size_t, M>> lps;\n    auto l = size_ + begin;\n    auto\
    \ r = size_ + end;\n    _lazy_propagation(begin, end);\n    auto access = [&](size_t\
    \ i) {\n      _propagate(i);\n      return tree_[i].first;\n    };\n    while\
    \ (l < r and f(access(r-1) * p)) {\n      if (l&1) {\n        lps.emplace(l, access(l));\n\
    \        l++;\n      }\n      if (r&1) p = tree_[r-1].first * p;\n      l>>=1;\
    \ r>>=1;\n    }\n    if (l >= r) {\n      while (lps.size()) {\n        auto&\
    \ [l, lp] = lps.top();\n        if (!f(lp * p)) {\n          r = l+1;\n      \
    \    break;\n        }\n        p = lp * p;\n        lps.pop();\n      }\n   \
    \   if (lps.empty()) return begin;\n    }\n    while (r <= size_) {\n      assert(!f(access(r-1)\
    \ * p));\n      r <<= 1;\n      auto pr = access(r-1);\n      if (f(pr * p)) {\n\
    \        p = pr * p;\n        --r;\n      }\n    }\n    return r - size_;\n  }\n\
    \  template<bool (*F)(M)>\n  size_t min_left(size_t begin, size_t end) {\n   \
    \ return min_left(begin, [](M x) { return F(x); });\n  }\n\n};\n\n"
  dependsOn:
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/lazy_segment_tree.hpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_SOME_WA
  verifiedWith:
  - test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp
  - test/yosupo/range_affine_range_sum.test.cpp
  - test/yosupo/static_range_inversions_query-mo_rollback.test.cpp
documentation_of: include/mtl/lazy_segment_tree.hpp
layout: document
redirect_from:
- /library/include/mtl/lazy_segment_tree.hpp
- /library/include/mtl/lazy_segment_tree.hpp.html
title: include/mtl/lazy_segment_tree.hpp
---
