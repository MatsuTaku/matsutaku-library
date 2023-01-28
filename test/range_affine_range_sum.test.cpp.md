---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/lazy_segment_tree.hpp
    title: include/mtl/lazy_segment_tree.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/modular.hpp
    title: include/mtl/modular.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/range_affine_range_sum
    links:
    - https://judge.yosupo.jp/problem/range_affine_range_sum
  bundledCode: "#line 1 \"test/range_affine_range_sum.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.yosupo.jp/problem/range_affine_range_sum\"\n#line 2 \"include/mtl/bit_manip.hpp\"\
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
    \ 1);\n  return x;\n}\n\n} // namespace bm\n#line 3 \"include/mtl/lazy_segment_tree.hpp\"\
    \n#include <cstddef>\n#include <vector>\n#line 6 \"include/mtl/lazy_segment_tree.hpp\"\
    \n#if __cplusplus >= 202002L\n#include <concepts>\n\ntemplate<typename M>\nconcept\
    \ LazySegmentTreeMonoid = requires (M m) {\n  {m * m} -> std::same_as<M>;\n};\n\
    template<typename A, typename M>\nconcept LazySegmentTreeOperatorMonoid = requires(A\
    \ a, M m) {\n  {a()} -> std::same_as<bool>;\n  {a *= a} -> std::convertible_to<A>;\n\
    \  {a.act(m, 1)} -> std::same_as<M>;\n};\n#endif\n\ntemplate <typename M, typename\
    \ A>\nclass LazySegmentTree {\n#if __cplusplus >= 202002L\n  static_assert(LazySegmentTreeMonoid<M>);\n\
    \  static_assert(LazySegmentTreeOperatorMonoid<A,M>);\n#endif\n private:\n  size_t\
    \ size_;\n  std::vector<std::pair<M,A>> tree_;\n  std::vector<std::pair<size_t,\
    \ size_t>> ids_;\n\n public:\n  explicit LazySegmentTree(size_t size) :\n    \
    \  size_(1ull<<(64-bm::clz(size-1))),\n      tree_(size_*2) {\n    ids_.reserve((64-bm::clz(size-1))*2);\n\
    \  }\n\n  template <typename Iter>\n  explicit LazySegmentTree(Iter begin, Iter\
    \ end)\n    : LazySegmentTree(std::distance(begin, end)) {\n    static_assert(std::is_convertible<typename\
    \ std::iterator_traits<Iter>::value_type, M>::value, \"\");\n    for (auto it\
    \ = begin; it != end; ++it) {\n      tree_[size_ + it - begin].first = *it;\n\
    \    }\n    for (size_t i = size_-1; i > 0; i--) {\n      tree_[i].first = tree_[i*2].first\
    \ * tree_[i*2+1].first;\n    }\n  }\n\n  void range_update(size_t l, size_t r,\
    \ const A& e) {\n    assert(l <= r and r <= size_);\n    if (l == r) return;\n\
    \    _set_ids(l, r);\n    for (int i = ids_.size()-1; i >= 0; --i) {\n      _propagate(ids_[i].first,\
    \ ids_[i].second);\n    }\n\n    for (size_t _l=l+size_, _r=r+size_, s=1; _l<_r;\
    \ _l>>=1, _r>>=1, s*=2) {\n      if (_l&1) {\n        tree_[_l].second *= e;\n\
    \        ++_l;\n      }\n      if (_r&1) {\n        --_r;\n        tree_[_r].second\
    \ *= e;\n      }\n    }\n\n    for (auto is : ids_) {\n      auto id = is.first;\n\
    \      auto sz = is.second;\n      _propagate(id*2, sz/2);\n      _propagate(id*2+1,\
    \ sz/2);\n      tree_[id].first = tree_[id*2].first * tree_[id*2+1].first;\n \
    \   }\n  }\n  inline void update(size_t l, size_t r, const A& e) {\n    range_update(l,\
    \ r, e);\n  }\n  inline void update(size_t i, const A& e) {\n    range_update(i,\
    \ i+1, e);\n  }\n\n  template<typename T>\n  inline void set(size_t i, T&& e)\
    \ {\n    _set_ids(i,i+1);\n    for (long long j = ids_.size()-1; j >= 0; --j)\n\
    \      _propagate(ids_[j].first, ids_[j].second);\n    int u = i+size_;\n    tree_[u].first\
    \ = M(std::forward(e));\n    u /= 2;\n    while (u > 0) {\n      tree_[u].first\
    \ = tree_[u*2].first * tree_[u*2+1].first;\n      u /= 2;\n    }\n  }\n\n  inline\
    \ M query(size_t l, size_t r) {\n    _set_ids(l, r);\n    for (int i = ids_.size()-1;\
    \ i >= 0; --i) {\n      _propagate(ids_[i].first, ids_[i].second);\n    }\n\n\
    \    M lhs,rhs;\n    for (size_t _l=l+size_, _r=r+size_, s=1; _l<_r; _l>>=1, _r>>=1,\
    \ s*=2) {\n      if (_l&1) {\n        _propagate(_l, s);\n        lhs = lhs *\
    \ tree_[_l].first;\n        ++_l;\n      }\n      if (_r&1) {\n        --_r;\n\
    \        _propagate(_r, s);\n        rhs = tree_[_r].first * rhs;\n      }\n \
    \   }\n    return lhs * rhs;\n  }\n\n  inline M get(size_t index) {\n    return\
    \ query(index, index+1);\n  }\n\n private:\n  inline void _set_ids(size_t l, size_t\
    \ r) {\n    ids_.clear();\n    auto _l=l+size_, _r=r+size_;\n    auto lth = _l/(_l&(-_l))/2;\n\
    \    auto rth = _r/(_r&(-_r))/2;\n    size_t s = 1;\n    for (; _l<_r; _l>>=1,\
    \ _r>>=1, s*=2) {\n      if (_r <= rth) ids_.emplace_back(_r, s);\n      if (_l\
    \ <= lth) ids_.emplace_back(_l, s);\n    }\n    for (; _l>0; _l>>=1, s*=2) {\n\
    \      ids_.emplace_back(_l, s);\n    }\n  }\n\n  inline void _propagate(size_t\
    \ id, size_t sz) {\n    A e = tree_[id].second;\n    if (!e()) return;\n    tree_[id].second\
    \ = A();\n    tree_[id].first = e.act(tree_[id].first, sz);\n    if (id < size_)\
    \ {\n      tree_[id*2].second *= e;\n      tree_[id*2+1].second *= e;\n    }\n\
    \  }\n\n};\n\n#line 2 \"include/mtl/modular.hpp\"\n#include <iostream>\n#line\
    \ 4 \"include/mtl/modular.hpp\"\n\ntemplate <int MOD>\nclass Modular {\n private:\n\
    \  unsigned int val_;\n\n public:\n  static constexpr unsigned int mod() { return\
    \ MOD; }\n  template<class T>\n  static constexpr unsigned int safe_mod(T v) {\n\
    \    auto x = (long long)(v%(long long)mod());\n    if (x < 0) x += mod();\n \
    \   return (unsigned int) x;\n  }\n\n  constexpr Modular() : val_(0) {}\n  template<class\
    \ T,\n      std::enable_if_t<\n          std::is_integral<T>::value && std::is_unsigned<T>::value\n\
    \      > * = nullptr>\n  constexpr Modular(T v) : val_(v%mod()) {}\n  template<class\
    \ T,\n      std::enable_if_t<\n          std::is_integral<T>::value && !std::is_unsigned<T>::value\n\
    \      > * = nullptr>\n  constexpr Modular(T v) : val_(safe_mod(v)) {}\n\n  constexpr\
    \ unsigned int val() const { return val_; }\n  constexpr Modular& operator+=(Modular\
    \ x) {\n    val_ += x.val();\n    if (val_ >= mod()) val_ -= mod();\n    return\
    \ *this;\n  }\n  constexpr Modular operator-() const { return {mod() - val_};\
    \ }\n  constexpr Modular& operator-=(Modular x) {\n    val_ += mod() - x.val();\n\
    \    if (val_ >= mod()) val_ -= mod();\n    return *this;\n  }\n  constexpr Modular&\
    \ operator*=(Modular x) {\n    auto v = (long long) val_ * x.val();\n    if (v\
    \ >= mod()) v %= mod();\n    val_ = v;\n    return *this;\n  }\n  constexpr Modular\
    \ pow(long long p) const {\n    assert(p >= 0);\n    Modular t = 1;\n    Modular\
    \ u = *this;\n    while (p) {\n      if (p & 1)\n        t *= u;\n      u *= u;\n\
    \      p >>= 1;\n    }\n    return t;\n  }\n  friend constexpr Modular pow(Modular\
    \ x, long long p) {\n    return x.pow(p);\n  }\n  constexpr Modular inv() const\
    \ { return pow(mod()-2); }\n  constexpr Modular& operator/=(Modular x) { return\
    \ *this *= x.inv(); }\n  constexpr Modular operator+(Modular x) const { return\
    \ Modular(*this) += x; }\n  constexpr Modular operator-(Modular x) const { return\
    \ Modular(*this) -= x; }\n  constexpr Modular operator*(Modular x) const { return\
    \ Modular(*this) *= x; }\n  constexpr Modular operator/(Modular x) const { return\
    \ Modular(*this) /= x; }\n  constexpr Modular& operator++() { return *this +=\
    \ 1; }\n  constexpr Modular operator++(int) { Modular c = *this; ++(*this); return\
    \ c; }\n  constexpr Modular& operator--() { return *this -= 1; }\n  constexpr\
    \ Modular operator--(int) { Modular c = *this; --(*this); return c; }\n\n  constexpr\
    \ bool operator==(Modular x) const { return val() == x.val(); }\n  constexpr bool\
    \ operator!=(Modular x) const { return val() != x.val(); }\n\n  friend std::ostream&\
    \ operator<<(std::ostream& os, const Modular& x) {\n    return os << x.val();\n\
    \  }\n  friend std::istream& operator>>(std::istream& is, Modular& x) {\n    return\
    \ is >> x.val_;\n  }\n\n};\n\nusing Modular998244353 = Modular<998244353>;\nusing\
    \ Modular1000000007 = Modular<(int)1e9+7>;\n\n#include <array>\n\nnamespace math\
    \ {\n\nconstexpr int mod_pow_constexpr(int x, int p, int m) {\n  int t = 1;\n\
    \  int u = x;\n  while (p) {\n    if (p & 1) {\n      t *= u;\n      t %= m;\n\
    \    }\n    u *= u;\n    u %= m;\n    p >>= 1;\n  }\n  return t;\n}\n\nconstexpr\
    \ int primitive_root_constexpr(int m) {\n  if (m == 2) return 1;\n  if (m == 998244353)\
    \ return 3;\n\n  std::array<int, 20> divs{2};\n  int cnt = 1;\n  int x = (m-1)\
    \ / 2;\n  for (int d = 3; d*d <= x; d += 2) {\n    if (x % d == 0) {\n      divs[cnt++]\
    \ = d;\n      while (x % d == 0)\n        x /= d;\n    }\n  }\n  if (x > 1) divs[cnt++]\
    \ = x;\n  for (int g = 2; ; g++) {\n    bool ok = true;\n    for (int i = 0; i\
    \ < cnt; i++) {\n      if (mod_pow_constexpr(g, (m-1) / divs[cnt], m) == 1) {\n\
    \        ok = false;\n        break;\n      }\n    }\n    if (ok) return g;\n\
    \  }\n}\n\ntemplate<int m>\nconstexpr int primitive_root = primitive_root_constexpr(m);\n\
    \n}\n#line 4 \"test/range_affine_range_sum.test.cpp\"\n#include <bits/stdc++.h>\n\
    \nusing namespace std;\nusing ll = long long;\nconstexpr ll MOD = 998244353;\n\
    using mint = Modular<MOD>;\n\nstruct SumMonoid {\n  mint a=0;\n  SumMonoid operator*(SumMonoid\
    \ r) const {\n    return {a+r.a};\n  }\n  SumMonoid& operator*=(SumMonoid r) {return\
    \ *this = *this * r;}\n};\n\nstruct FnMonoid {\n  mint b=1, c=0;\n  FnMonoid operator*(FnMonoid\
    \ r) const {\n    return {b*r.b, c*r.b+r.c};\n  }\n  FnMonoid& operator*=(FnMonoid\
    \ r) {return *this = *this * r;}\n  bool operator()() const {return !(b == 1 and\
    \ c == 0);}\n  SumMonoid act(SumMonoid a, size_t num) const {\n    return {b*a.a\
    \ + c*num};\n  }\n};\n\nint main() {\n  cin.tie(nullptr); ios::sync_with_stdio(false);\n\
    \n  int N,Q; cin>>N>>Q;\n  vector<SumMonoid> A(N); for (auto& a : A) cin>>a.a;\n\
    \  LazySegmentTree<SumMonoid, FnMonoid> rsq(A.begin(), A.end());\n\n  for (int\
    \ q = 0; q < Q; q++) {\n    int t; cin>>t;\n    if (t == 0) {\n      int l,r,b,c;\
    \ cin>>l>>r>>b>>c;\n      rsq.range_update(l,r, {b,c});\n    } else if (t == 1)\
    \ {\n      int l,r; cin>>l>>r;\n      auto ans = rsq.query(l,r);\n      cout <<\
    \ ans.a << endl;\n    }\n  }\n\n  return 0;\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/range_affine_range_sum\"\
    \n#include \"../include/mtl/lazy_segment_tree.hpp\"\n#include \"../include/mtl/modular.hpp\"\
    \n#include <bits/stdc++.h>\n\nusing namespace std;\nusing ll = long long;\nconstexpr\
    \ ll MOD = 998244353;\nusing mint = Modular<MOD>;\n\nstruct SumMonoid {\n  mint\
    \ a=0;\n  SumMonoid operator*(SumMonoid r) const {\n    return {a+r.a};\n  }\n\
    \  SumMonoid& operator*=(SumMonoid r) {return *this = *this * r;}\n};\n\nstruct\
    \ FnMonoid {\n  mint b=1, c=0;\n  FnMonoid operator*(FnMonoid r) const {\n   \
    \ return {b*r.b, c*r.b+r.c};\n  }\n  FnMonoid& operator*=(FnMonoid r) {return\
    \ *this = *this * r;}\n  bool operator()() const {return !(b == 1 and c == 0);}\n\
    \  SumMonoid act(SumMonoid a, size_t num) const {\n    return {b*a.a + c*num};\n\
    \  }\n};\n\nint main() {\n  cin.tie(nullptr); ios::sync_with_stdio(false);\n\n\
    \  int N,Q; cin>>N>>Q;\n  vector<SumMonoid> A(N); for (auto& a : A) cin>>a.a;\n\
    \  LazySegmentTree<SumMonoid, FnMonoid> rsq(A.begin(), A.end());\n\n  for (int\
    \ q = 0; q < Q; q++) {\n    int t; cin>>t;\n    if (t == 0) {\n      int l,r,b,c;\
    \ cin>>l>>r>>b>>c;\n      rsq.range_update(l,r, {b,c});\n    } else if (t == 1)\
    \ {\n      int l,r; cin>>l>>r;\n      auto ans = rsq.query(l,r);\n      cout <<\
    \ ans.a << endl;\n    }\n  }\n\n  return 0;\n}\n"
  dependsOn:
  - include/mtl/lazy_segment_tree.hpp
  - include/mtl/bit_manip.hpp
  - include/mtl/modular.hpp
  isVerificationFile: true
  path: test/range_affine_range_sum.test.cpp
  requiredBy: []
  timestamp: '2023-01-28 19:00:09+00:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/range_affine_range_sum.test.cpp
layout: document
redirect_from:
- /verify/test/range_affine_range_sum.test.cpp
- /verify/test/range_affine_range_sum.test.cpp.html
title: test/range_affine_range_sum.test.cpp
---
