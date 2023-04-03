---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/dual_segment_tree.hpp
    title: include/mtl/dual_segment_tree.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_D
    links:
    - https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_D
  bundledCode: "#line 1 \"test/aoj-range_update_query.test.cpp\"\n#define PROBLEM\
    \ \"https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_D\"\n#line 2 \"include/mtl/bit_manip.hpp\"\
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
    \ 1);\n  return x;\n}\n\n} // namespace bm\n#line 3 \"include/mtl/dual_segment_tree.hpp\"\
    \n#include <cstddef>\n#include <vector>\n#include <algorithm>\n#line 7 \"include/mtl/dual_segment_tree.hpp\"\
    \n#if _cplusplus >= 202002L\n#include <concept>\n\ntemplate<typename M>\nconcept\
    \ DualSegmentTreeMonoid = requires (M m) {\n  {m *= m} -> std::same_as<M>;\n};\n\
    #endif\n\ntemplate <typename M>\nclass DualSegmentTree {\n#if _cplusplus >= 202002L\n\
    \  static_assert(DualSegmentTreeMonoid<M>);\n#endif\n private:\n  size_t size_;\n\
    \  std::vector<M> tree_;\n  std::vector<std::pair<size_t, size_t>> ids_;\n\n \
    \ int log(size_t x) const {\n    return 64 - bm::clz(x-1);\n  }\n\n public:\n\
    \  explicit DualSegmentTree(size_t size) :\n      size_(1ull<<log(size)),\n  \
    \    tree_(size_*2) {\n    ids_.reserve(log(size)*2);\n  }\n\n  template <typename\
    \ Iter>\n  explicit DualSegmentTree(Iter begin, Iter end)\n    : DualSegmentTree(std::distance(begin,\
    \ end)) {\n    static_assert(std::is_convertible<typename std::iterator_traits<Iter>::value_type,\
    \ M>::value, \"\");\n    std::transform(begin, end, tree_.begin()+size_, [](const\
    \ auto& v) {return v;});\n  }\n\n  void update(size_t l, size_t r, const M& e)\
    \ {\n    assert(l <= r and r <= size_);\n    if (l == r) return;\n    _set_ids(l,\
    \ r);\n    for (int i = ids_.size()-1; i >= 0; --i) \n      _propagate(ids_[i].first,\
    \ ids_[i].second);\n\n    for (size_t _l=l+size_, _r=r+size_, s=1; _l<_r; _l>>=1,\
    \ _r>>=1, s*=2) {\n      if (_l&1) {\n        tree_[_l] *= e;\n        ++_l;\n\
    \      }\n      if (_r&1) {\n        --_r;\n        tree_[_r] *= e;\n      }\n\
    \    }\n  }\n  void update(size_t i, const M& e) {\n    update(i, i+1, e);\n \
    \ }\n\n  M get(size_t index) {\n    assert(index < size_);\n    _set_ids(index,\
    \ index+1);\n    for (int i = ids_.size()-1; i >= 0; --i) \n      _propagate(ids_[i].first,\
    \ ids_[i].second);\n    return tree_[size_ + index];\n  }\n\n private:\n  void\
    \ _set_ids(size_t l, size_t r) {\n    ids_.clear();\n    auto _l=l+size_, _r=r+size_;\n\
    \    auto lth = _l/(_l&(-_l))/2;\n    auto rth = _r/(_r&(-_r))/2;\n    size_t\
    \ s = 1;\n    for (; _l<_r; _l>>=1, _r>>=1, s*=2) {\n      if (_r <= rth) ids_.emplace_back(_r,\
    \ s);\n      if (_l <= lth) ids_.emplace_back(_l, s);\n    }\n    for (; _l>0;\
    \ _l>>=1, s*=2) {\n      ids_.emplace_back(_l, s);\n    }\n  }\n\n  void _propagate(size_t\
    \ id, size_t sz) {\n    if (id >= size_) return;\n    M e = tree_[id];\n    tree_[id]\
    \ = M();\n    tree_[id*2] *= e;\n    tree_[id*2+1] *= e;\n  }\n\n};\n\n#line 3\
    \ \"test/aoj-range_update_query.test.cpp\"\n#include <bits/stdc++.h>\nusing namespace\
    \ std;\n\nstruct OR {\n    int a,b;\n    OR() : a(1), b(0) {}\n    OR(int b) :\
    \ a(0), b(b) {}\n    OR& operator=(int x) {\n        a = 0;\n        b = x;\n\
    \        return *this;\n    }\n    OR& operator*=(const OR& r) {\n        a *=\
    \ r.a;\n        b = b * r.a + r.b;\n        return *this;\n    }\n};\n\nint main()\
    \ {\n    int n,q; cin>>n>>q;\n    vector init(n, (1u<<31)-1);\n    DualSegmentTree<OR>\
    \ dst(init.begin(), init.end());\n    for (int i = 0; i < q; i++) {\n        int\
    \ t; cin>>t;\n        if (t == 0) {\n            int s,t,x; cin>>s>>t>>x;\n  \
    \          ++t;\n            dst.update(s, t, x);\n        } else {\n        \
    \    int i; cin>>i;\n            cout << dst.get(i).b << endl;\n        }\n  \
    \  }\n}\n"
  code: "#define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_D\"\n#include\
    \ \"../include/mtl/dual_segment_tree.hpp\"\n#include <bits/stdc++.h>\nusing namespace\
    \ std;\n\nstruct OR {\n    int a,b;\n    OR() : a(1), b(0) {}\n    OR(int b) :\
    \ a(0), b(b) {}\n    OR& operator=(int x) {\n        a = 0;\n        b = x;\n\
    \        return *this;\n    }\n    OR& operator*=(const OR& r) {\n        a *=\
    \ r.a;\n        b = b * r.a + r.b;\n        return *this;\n    }\n};\n\nint main()\
    \ {\n    int n,q; cin>>n>>q;\n    vector init(n, (1u<<31)-1);\n    DualSegmentTree<OR>\
    \ dst(init.begin(), init.end());\n    for (int i = 0; i < q; i++) {\n        int\
    \ t; cin>>t;\n        if (t == 0) {\n            int s,t,x; cin>>s>>t>>x;\n  \
    \          ++t;\n            dst.update(s, t, x);\n        } else {\n        \
    \    int i; cin>>i;\n            cout << dst.get(i).b << endl;\n        }\n  \
    \  }\n}"
  dependsOn:
  - include/mtl/dual_segment_tree.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: true
  path: test/aoj-range_update_query.test.cpp
  requiredBy: []
  timestamp: '2023-04-04 01:36:55+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/aoj-range_update_query.test.cpp
layout: document
redirect_from:
- /verify/test/aoj-range_update_query.test.cpp
- /verify/test/aoj-range_update_query.test.cpp.html
title: test/aoj-range_update_query.test.cpp
---