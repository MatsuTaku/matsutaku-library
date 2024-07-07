---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':question:'
    path: include/mtl/lazy_segment_tree.hpp
    title: include/mtl/lazy_segment_tree.hpp
  - icon: ':question:'
    path: include/mtl/modular.hpp
    title: include/mtl/modular.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/range_affine_range_sum
    links:
    - https://judge.yosupo.jp/problem/range_affine_range_sum
  bundledCode: "#line 1 \"test/yosupo/range_affine_range_sum.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.yosupo.jp/problem/range_affine_range_sum\"\r\n#line 2 \"include/mtl/bit_manip.hpp\"\
    \n#include <cstdint>\n#include <cassert>\n\nnamespace bm {\n\ninline constexpr\
    \ uint64_t popcnt_e8(uint64_t x) {\n  x = (x & 0x5555555555555555) + ((x>>1) &\
    \ 0x5555555555555555);\n  x = (x & 0x3333333333333333) + ((x>>2) & 0x3333333333333333);\n\
    \  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n  return x;\n\
    }\n// Count 1s\ninline constexpr unsigned popcnt(uint64_t x) {\n  return (popcnt_e8(x)\
    \ * 0x0101010101010101) >> 56;\n}\n// Count trailing 0s. s.t. *11011000 -> 3\n\
    inline constexpr unsigned ctz(uint64_t x) {\n  return popcnt((x & (-x)) - 1);\n\
    }\ninline constexpr unsigned ctz8(uint8_t x) {\n  return x == 0 ? 8 : popcnt_e8((x\
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
    \n// #include \"monoid.hpp\"\n#include <cstddef>\n#include <utility>\n#include\
    \ <vector>\n#include <stack>\n#line 9 \"include/mtl/lazy_segment_tree.hpp\"\n\
    #if __cpp_concepts >= 202002L\n#include <concepts>\n\ntemplate<typename M>\nconcept\
    \ LazySegmentTreeMonoid = requires (M m) {\n  {m * m} -> std::same_as<M>;\n};\n\
    template<typename A, typename M>\nconcept LazySegmentTreeOperatorMonoid = requires(A\
    \ a, M m) {\n  {a *= a} -> std::same_as<A&>;\n  {a.act(m)} -> std::same_as<M>;\n\
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
    \ return min_left(begin, [](M x) { return F(x); });\n  }\n\n};\n\n#line 3 \"include/mtl/modular.hpp\"\
    \n#include <iostream>\n#line 5 \"include/mtl/modular.hpp\"\n\ntemplate <int MOD>\n\
    class Modular {\n private:\n  unsigned int val_;\n\n public:\n  static constexpr\
    \ unsigned int mod() { return MOD; }\n  template<class T>\n  static constexpr\
    \ unsigned int safe_mod(T v) {\n    auto x = (long long)(v%(long long)mod());\n\
    \    if (x < 0) x += mod();\n    return (unsigned int) x;\n  }\n\n  constexpr\
    \ Modular() : val_(0) {}\n  template<class T,\n      std::enable_if_t<\n     \
    \     std::is_integral<T>::value && std::is_unsigned<T>::value\n      > * = nullptr>\n\
    \  constexpr Modular(T v) : val_(v%mod()) {}\n  template<class T,\n      std::enable_if_t<\n\
    \          std::is_integral<T>::value && !std::is_unsigned<T>::value\n      >\
    \ * = nullptr>\n  constexpr Modular(T v) : val_(safe_mod(v)) {}\n\n  constexpr\
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
    \ operator!=(Modular x) const { return val() != x.val(); }\n\n  constexpr bool\
    \ is_square() const {\n    return pow((mod()-1)/2) == 1;\n  }\n  /**\n   * Return\
    \ x s.t. x * x = a mod p\n   * reference: https://zenn.dev/peria/articles/c6afc72b6b003c\n\
    \  */\n  constexpr Modular sqrt() const {\n    if (!is_square()) \n      throw\
    \ std::runtime_error(\"not square\");\n    auto mod_eight = mod() % 8;\n    if\
    \ (mod_eight == 3 || mod_eight == 7) {\n      return pow((mod()+1)/4);\n    }\
    \ else if (mod_eight == 5) {\n      auto x = pow((mod()+3)/8);\n      if (x *\
    \ x != *this)\n        x *= Modular(2).pow((mod()-1)/4);\n      return x;\n  \
    \  } else {\n      Modular d = 2;\n      while (d.is_square())\n        d += 1;\n\
    \      auto t = mod()-1;\n      int s = bm::ctz(t);\n      t >>= s;\n      auto\
    \ a = pow(t);\n      auto D = d.pow(t);\n      int m = 0;\n      Modular dt =\
    \ 1;\n      Modular du = D;\n      for (int i = 0; i < s; i++) {\n        if ((a*dt).pow(1u<<(s-1-i))\
    \ == -1) {\n          m |= 1u << i;\n          dt *= du;\n        }\n        du\
    \ *= du;\n      }\n      return pow((t+1)/2) * D.pow(m/2);\n    }\n  }\n\n  friend\
    \ std::ostream& operator<<(std::ostream& os, const Modular& x) {\n    return os\
    \ << x.val();\n  }\n  friend std::istream& operator>>(std::istream& is, Modular&\
    \ x) {\n    return is >> x.val_;\n  }\n\n};\n\nusing Modular998244353 = Modular<998244353>;\n\
    using Modular1000000007 = Modular<(int)1e9+7>;\n\ntemplate<int Id=0>\nclass DynamicModular\
    \ {\n private:\n  static unsigned int mod_;\n  unsigned int val_;\n\n public:\n\
    \  static unsigned int mod() { return mod_; }\n  static void set_mod(unsigned\
    \ int m) { mod_ = m; }\n  template<class T>\n  static constexpr unsigned int safe_mod(T\
    \ v) {\n    auto x = (long long)(v%(long long)mod());\n    if (x < 0) x += mod();\n\
    \    return (unsigned int) x;\n  }\n\n  constexpr DynamicModular() : val_(0) {}\n\
    \  template<class T,\n      std::enable_if_t<\n          std::is_integral<T>::value\
    \ && std::is_unsigned<T>::value\n      > * = nullptr>\n  constexpr DynamicModular(T\
    \ v) : val_(v%mod()) {}\n  template<class T,\n      std::enable_if_t<\n      \
    \    std::is_integral<T>::value && !std::is_unsigned<T>::value\n      > * = nullptr>\n\
    \  constexpr DynamicModular(T v) : val_(safe_mod(v)) {}\n\n  constexpr unsigned\
    \ int val() const { return val_; }\n  constexpr DynamicModular& operator+=(DynamicModular\
    \ x) {\n    val_ += x.val();\n    if (val_ >= mod()) val_ -= mod();\n    return\
    \ *this;\n  }\n  constexpr DynamicModular operator-() const { return {mod() -\
    \ val_}; }\n  constexpr DynamicModular& operator-=(DynamicModular x) {\n    val_\
    \ += mod() - x.val();\n    if (val_ >= mod()) val_ -= mod();\n    return *this;\n\
    \  }\n  constexpr DynamicModular& operator*=(DynamicModular x) {\n    auto v =\
    \ (long long) val_ * x.val();\n    if (v >= mod()) v %= mod();\n    val_ = v;\n\
    \    return *this;\n  }\n  constexpr DynamicModular pow(long long p) const {\n\
    \    assert(p >= 0);\n    DynamicModular t = 1;\n    DynamicModular u = *this;\n\
    \    while (p) {\n      if (p & 1)\n        t *= u;\n      u *= u;\n      p >>=\
    \ 1;\n    }\n    return t;\n  }\n  friend constexpr DynamicModular pow(DynamicModular\
    \ x, long long p) {\n    return x.pow(p);\n  }\n  // TODO: implement when mod\
    \ is not prime\n  constexpr DynamicModular inv() const { return pow(mod()-2);\
    \ }\n  constexpr DynamicModular& operator/=(DynamicModular x) { return *this *=\
    \ x.inv(); }\n  constexpr DynamicModular operator+(DynamicModular x) const { return\
    \ DynamicModular(*this) += x; }\n  constexpr DynamicModular operator-(DynamicModular\
    \ x) const { return DynamicModular(*this) -= x; }\n  constexpr DynamicModular\
    \ operator*(DynamicModular x) const { return DynamicModular(*this) *= x; }\n \
    \ constexpr DynamicModular operator/(DynamicModular x) const { return DynamicModular(*this)\
    \ /= x; }\n  constexpr DynamicModular& operator++() { return *this += 1; }\n \
    \ constexpr DynamicModular operator++(int) { DynamicModular c = *this; ++(*this);\
    \ return c; }\n  constexpr DynamicModular& operator--() { return *this -= 1; }\n\
    \  constexpr DynamicModular operator--(int) { DynamicModular c = *this; --(*this);\
    \ return c; }\n\n  constexpr bool operator==(DynamicModular x) const { return\
    \ val() == x.val(); }\n  constexpr bool operator!=(DynamicModular x) const { return\
    \ val() != x.val(); }\n\n  constexpr bool is_square() const {\n    return val()\
    \ == 0 or pow((mod()-1)/2) == 1;\n  }\n  /**\n   * Return x s.t. x * x = a mod\
    \ p\n   * reference: https://zenn.dev/peria/articles/c6afc72b6b003c\n  */\n  constexpr\
    \ DynamicModular sqrt() const {\n    // assert mod is prime\n    if (!is_square())\
    \ \n      throw std::runtime_error(\"not square\");\n    if (val() < 2)\n    \
    \  return val();\n    auto mod_eight = mod() % 8;\n    if (mod_eight == 3 || mod_eight\
    \ == 7) {\n      return pow((mod()+1)/4);\n    } else if (mod_eight == 5) {\n\
    \      auto x = pow((mod()+3)/8);\n      if (x * x != *this)\n        x *= DynamicModular(2).pow((mod()-1)/4);\n\
    \      return x;\n    } else {\n      DynamicModular d = 2;\n      while (d.is_square())\n\
    \        ++d;\n      auto t = mod()-1;\n      int s = bm::ctz(t);\n      t >>=\
    \ s;\n      auto a = pow(t);\n      auto D = d.pow(t);\n      int m = 0;\n   \
    \   DynamicModular dt = 1;\n      DynamicModular du = D;\n      for (int i = 0;\
    \ i < s; i++) {\n        if ((a*dt).pow(1u<<(s-1-i)) == -1) {\n          m |=\
    \ 1u << i;\n          dt *= du;\n        }\n        du *= du;\n      }\n     \
    \ return pow((t+1)/2) * D.pow(m/2);\n    }\n  }\n\n  friend std::ostream& operator<<(std::ostream&\
    \ os, const DynamicModular& x) {\n    return os << x.val();\n  }\n  friend std::istream&\
    \ operator>>(std::istream& is, DynamicModular& x) {\n    return is >> x.val_;\n\
    \  }\n\n};\ntemplate<int Id>\nunsigned int DynamicModular<Id>::mod_;\n\n#line\
    \ 264 \"include/mtl/modular.hpp\"\n\ntemplate<class ModInt>\nstruct ModularUtil\
    \ {\n  static constexpr int mod = ModInt::mod();\n  static struct inv_table {\n\
    \    std::vector<ModInt> tb{0,1};\n    inv_table() : tb({0,1}) {}\n  } inv_;\n\
    \  void set_inv(int n) {\n    int m = inv_.tb.size();\n    if (m > n) return;\n\
    \    inv_.tb.resize(n+1);\n    for (int i = m; i < n+1; i++)\n      inv_.tb[i]\
    \ = -inv_.tb[mod % i] * (mod / i);\n  }\n  ModInt& inv(int i) {\n    set_inv(i);\n\
    \    return inv_.tb[i];\n  }\n};\ntemplate<class ModInt>\ntypename ModularUtil<ModInt>::inv_table\
    \ ModularUtil<ModInt>::inv_;\n\n#include <array>\n\nnamespace math {\n\nconstexpr\
    \ int mod_pow_constexpr(int x, int p, int m) {\n  long long t = 1;\n  long long\
    \ u = x;\n  while (p) {\n    if (p & 1) {\n      t *= u;\n      t %= m;\n    }\n\
    \    u *= u;\n    u %= m;\n    p >>= 1;\n  }\n  return (int) t;\n}\n\nconstexpr\
    \ int primitive_root_constexpr(int m) {\n  if (m == 2) return 1;\n  if (m == 167772161)\
    \ return 3;\n  if (m == 469762049) return 3;\n  if (m == 754974721) return 11;\n\
    \  if (m == 880803841) return 26;\n  if (m == 998244353) return 3;\n\n  std::array<int,\
    \ 20> divs{};\n  int cnt = 0;\n  int x = m-1;\n  if (x % 2 == 0) {\n    divs[cnt++]\
    \ = 2;\n    x >>= bm::ctz(x);\n  }\n  for (int d = 3; d*d <= x; d += 2) {\n  \
    \  if (x % d == 0) {\n      divs[cnt++] = d;\n      while (x % d == 0)\n     \
    \   x /= d;\n    }\n  }\n  if (x > 1) divs[cnt++] = x;\n  for (int g = 2; g <\
    \ m; g++) {\n    bool ok = true;\n    for (int i = 0; i < cnt; i++) {\n      if\
    \ (mod_pow_constexpr(g, (m-1) / divs[i], m) == 1) {\n        ok = false;\n   \
    \     break;\n      }\n    }\n    if (ok) return g;\n  }\n  return -1;\n}\n\n\
    template<int m>\nconstexpr int primitive_root = primitive_root_constexpr(m);\n\
    \n}\n#line 4 \"test/yosupo/range_affine_range_sum.test.cpp\"\n#include <bits/stdc++.h>\r\
    \n\r\nusing namespace std;\r\nusing ll = long long;\r\nconstexpr ll MOD = 998244353;\r\
    \nusing mint = Modular<MOD>;\r\n\r\nstruct Sum {\r\n  mint a=0;\r\n  int sz=0;\r\
    \n  Sum operator*(const Sum& r) const {\r\n    return {a+r.a, sz+r.sz};\r\n  }\r\
    \n};\r\n\r\nstruct Affine {\r\n  mint b=1, c=0;\r\n  Affine& operator*=(const\
    \ Affine& r) {\r\n    b *= r.b;\r\n    c = c*r.b + r.c;\r\n    return *this;\r\
    \n  }\r\n  Sum act(const Sum& a) const {\r\n    return {b*a.a + c*a.sz, a.sz};\r\
    \n  }\r\n};\r\n\r\nint main() {\r\n  cin.tie(nullptr); ios::sync_with_stdio(false);\r\
    \n\r\n  int N,Q; cin>>N>>Q;\r\n  vector<Sum> A(N); \r\n  for (auto& a : A) {\r\
    \n    cin>>a.a;\r\n    a.sz = 1;\r\n  }\r\n  LazySegmentTree<Sum, Affine> rsq(A.begin(),\
    \ A.end());\r\n\r\n  for (int q = 0; q < Q; q++) {\r\n    int t; cin>>t;\r\n \
    \   if (t == 0) {\r\n      int l,r,b,c; cin>>l>>r>>b>>c;\r\n      rsq.update(l,r,\
    \ {b,c});\r\n    } else if (t == 1) {\r\n      int l,r; cin>>l>>r;\r\n      auto\
    \ ans = rsq.query(l,r);\r\n      cout << ans.a << endl;\r\n    }\r\n  }\r\n\r\n\
    \  return 0;\r\n}\r\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/range_affine_range_sum\"\
    \r\n#include \"../../include/mtl/lazy_segment_tree.hpp\"\r\n#include \"../../include/mtl/modular.hpp\"\
    \r\n#include <bits/stdc++.h>\r\n\r\nusing namespace std;\r\nusing ll = long long;\r\
    \nconstexpr ll MOD = 998244353;\r\nusing mint = Modular<MOD>;\r\n\r\nstruct Sum\
    \ {\r\n  mint a=0;\r\n  int sz=0;\r\n  Sum operator*(const Sum& r) const {\r\n\
    \    return {a+r.a, sz+r.sz};\r\n  }\r\n};\r\n\r\nstruct Affine {\r\n  mint b=1,\
    \ c=0;\r\n  Affine& operator*=(const Affine& r) {\r\n    b *= r.b;\r\n    c =\
    \ c*r.b + r.c;\r\n    return *this;\r\n  }\r\n  Sum act(const Sum& a) const {\r\
    \n    return {b*a.a + c*a.sz, a.sz};\r\n  }\r\n};\r\n\r\nint main() {\r\n  cin.tie(nullptr);\
    \ ios::sync_with_stdio(false);\r\n\r\n  int N,Q; cin>>N>>Q;\r\n  vector<Sum> A(N);\
    \ \r\n  for (auto& a : A) {\r\n    cin>>a.a;\r\n    a.sz = 1;\r\n  }\r\n  LazySegmentTree<Sum,\
    \ Affine> rsq(A.begin(), A.end());\r\n\r\n  for (int q = 0; q < Q; q++) {\r\n\
    \    int t; cin>>t;\r\n    if (t == 0) {\r\n      int l,r,b,c; cin>>l>>r>>b>>c;\r\
    \n      rsq.update(l,r, {b,c});\r\n    } else if (t == 1) {\r\n      int l,r;\
    \ cin>>l>>r;\r\n      auto ans = rsq.query(l,r);\r\n      cout << ans.a << endl;\r\
    \n    }\r\n  }\r\n\r\n  return 0;\r\n}\r\n"
  dependsOn:
  - include/mtl/lazy_segment_tree.hpp
  - include/mtl/bit_manip.hpp
  - include/mtl/modular.hpp
  isVerificationFile: true
  path: test/yosupo/range_affine_range_sum.test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: test/yosupo/range_affine_range_sum.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/range_affine_range_sum.test.cpp
- /verify/test/yosupo/range_affine_range_sum.test.cpp.html
title: test/yosupo/range_affine_range_sum.test.cpp
---
