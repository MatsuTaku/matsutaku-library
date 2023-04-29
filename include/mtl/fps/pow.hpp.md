---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':question:'
    path: include/mtl/fps.hpp
    title: include/mtl/fps.hpp
  - icon: ':question:'
    path: include/mtl/fps/exp.hpp
    title: include/mtl/fps/exp.hpp
  - icon: ':question:'
    path: include/mtl/modular.hpp
    title: include/mtl/modular.hpp
  - icon: ':question:'
    path: include/mtl/ntt.hpp
    title: include/mtl/ntt.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/yosupo/polynomial/pow_of_formal_power_series.test.cpp
    title: test/yosupo/polynomial/pow_of_formal_power_series.test.cpp
  - icon: ':x:'
    path: test/yosupo/polynomial/pow_of_formal_power_series_sparse.test.cpp
    title: test/yosupo/polynomial/pow_of_formal_power_series_sparse.test.cpp
  - icon: ':x:'
    path: test/yosupo/polynomial/sqrt_of_formal_power_series.test.cpp
    title: test/yosupo/polynomial/sqrt_of_formal_power_series.test.cpp
  - icon: ':x:'
    path: test/yosupo/polynomial/sqrt_of_formal_power_series_sparse.test.cpp
    title: test/yosupo/polynomial/sqrt_of_formal_power_series_sparse.test.cpp
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
    \ 3 \"include/mtl/modular.hpp\"\n#include <iostream>\n#line 5 \"include/mtl/modular.hpp\"\
    \n\ntemplate <int MOD>\nclass Modular {\n private:\n  unsigned int val_;\n\n public:\n\
    \  static constexpr unsigned int mod() { return MOD; }\n  template<class T>\n\
    \  static constexpr unsigned int safe_mod(T v) {\n    auto x = (long long)(v%(long\
    \ long)mod());\n    if (x < 0) x += mod();\n    return (unsigned int) x;\n  }\n\
    \n  constexpr Modular() : val_(0) {}\n  template<class T,\n      std::enable_if_t<\n\
    \          std::is_integral<T>::value && std::is_unsigned<T>::value\n      > *\
    \ = nullptr>\n  constexpr Modular(T v) : val_(v%mod()) {}\n  template<class T,\n\
    \      std::enable_if_t<\n          std::is_integral<T>::value && !std::is_unsigned<T>::value\n\
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
    \  }\n\n};\ntemplate<int Id>\nunsigned int DynamicModular<Id>::mod_;\n\n#include\
    \ <vector>\n\ntemplate<class ModInt>\nstruct ModularUtil {\n  static constexpr\
    \ int mod = ModInt::mod();\n  static struct inv_table {\n    std::vector<ModInt>\
    \ tb{0,1};\n    inv_table() : tb({0,1}) {}\n  } inv_;\n  void set_inv(int n) {\n\
    \    int m = inv_.tb.size();\n    if (m > n) return;\n    inv_.tb.resize(n+1);\n\
    \    for (int i = m; i < n+1; i++)\n      inv_.tb[i] = -inv_.tb[mod % i] * (mod\
    \ / i);\n  }\n  ModInt& inv(int i) {\n    set_inv(i);\n    return inv_.tb[i];\n\
    \  }\n};\ntemplate<class ModInt>\ntypename ModularUtil<ModInt>::inv_table ModularUtil<ModInt>::inv_;\n\
    \n#include <array>\n\nnamespace math {\n\nconstexpr int mod_pow_constexpr(int\
    \ x, int p, int m) {\n  int t = 1;\n  int u = x;\n  while (p) {\n    if (p & 1)\
    \ {\n      t *= u;\n      t %= m;\n    }\n    u *= u;\n    u %= m;\n    p >>=\
    \ 1;\n  }\n  return t;\n}\n\nconstexpr int primitive_root_constexpr(int m) {\n\
    \  if (m == 2) return 1;\n  if (m == 998244353) return 3;\n\n  std::array<int,\
    \ 20> divs{2};\n  int cnt = 1;\n  int x = (m-1) / 2;\n  for (int d = 3; d*d <=\
    \ x; d += 2) {\n    if (x % d == 0) {\n      divs[cnt++] = d;\n      while (x\
    \ % d == 0)\n        x /= d;\n    }\n  }\n  if (x > 1) divs[cnt++] = x;\n  for\
    \ (int g = 2; ; g++) {\n    bool ok = true;\n    for (int i = 0; i < cnt; i++)\
    \ {\n      if (mod_pow_constexpr(g, (m-1) / divs[cnt], m) == 1) {\n        ok\
    \ = false;\n        break;\n      }\n    }\n    if (ok) return g;\n  }\n}\n\n\
    template<int m>\nconstexpr int primitive_root = primitive_root_constexpr(m);\n\
    \n}\n#line 6 \"include/mtl/ntt.hpp\"\n\r\nnamespace math {\r\n\r\nconstexpr int\
    \ ceil_pow2(unsigned long long x) {\r\n  return x == 0 ? 0 : 64-bm::clz(x-1);\r\
    \n}\r\n\r\n}\r\n\r\nnamespace _ntt {\r\n\r\ntemplate<int mod>\r\nstruct ntt_info\
    \ {\r\n  using mint = Modular<mod>;\r\n  static constexpr int primitive_root()\
    \ {\r\n    return math::primitive_root<mod>;\r\n  }\r\n  static constexpr int\
    \ log_n() {\r\n    return bm::ctz(mod-1);\r\n  }\r\n  static constexpr std::array<Modular<mod>,\
    \ log_n()> coeff(bool forward) {\r\n    mint r = mint(primitive_root()).pow((mod-1)\
    \ >> log_n());\r\n    std::array<mint, log_n()> coeff{};\r\n    mint iw = forward\
    \ ? r.inv() : r;\r\n    for (int i = log_n()-1; i >= 0; i--) {\r\n      coeff[i]\
    \ = iw;\r\n      iw *= iw;\r\n    }\r\n    return coeff;\r\n  }\r\n};\r\n\r\n\
    template <bool Forward, int mod>\r\nvoid _fft_impl(std::vector<Modular<mod>>&\
    \ f) {\r\n  using mint = Modular<mod>;\r\n  int log_n = math::ceil_pow2(f.size());\r\
    \n  using info = ntt_info<mod>;\r\n  assert(info::log_n() >= log_n);\r\n  int\
    \ n = 1 << log_n;\r\n  f.resize(n, 0);\r\n  // iterative bit reversal\r\n  for\
    \ (int i = 0, j = 1; j < n-1; j++) {\r\n    for (int k = n >> 1; k > (i ^= k);\
    \ k >>= 1);\r\n    if (i < j) std::swap(f[i], f[j]);\r\n  }\r\n  // Cooley-Tukey\
    \ FFT\r\n  static constexpr auto coeff = info::coeff(Forward);\r\n  for (int log_m\
    \ = 0; log_m < log_n; log_m++) {\r\n    int m = 1<<log_m;\r\n    mint w0 = coeff[log_m];\r\
    \n    for (int chunk = 0; chunk < n; chunk += 2*m) {\r\n      mint w = 1;\r\n\
    \      for (int i = 0; i < m; i++) {\r\n        auto p = chunk + i;\r\n      \
    \  auto a = f[p + 0];\r\n        auto b = f[p + m] * w;\r\n        f[p + 0] =\
    \ a + b;\r\n        f[p + m] = a - b;\r\n        w *= w0;\r\n      }\r\n    }\r\
    \n  }\r\n}\r\ntemplate<class mint>\r\nvoid _fft(std::vector<mint>& f) { _fft_impl<true>(f);\
    \ }\r\ntemplate<class mint>\r\nvoid _ifft(std::vector<mint>& f) { _fft_impl<false>(f);\
    \ }\r\n\r\n}\r\n\r\ntemplate<class mint>\r\nvoid ntt_inline(std::vector<mint>&\
    \ f) {\r\n  _ntt::_fft(f);\r\n}\r\n\r\ntemplate<class mint>\r\nstd::vector<mint>\
    \ ntt(std::vector<mint> f) {\r\n  _ntt::_fft(f);\r\n  return f;\r\n}\r\n\r\ntemplate<class\
    \ mint>\r\nvoid intt_inline(std::vector<mint>& f) {\r\n  _ntt::_ifft(f);\r\n}\r\
    \n\r\ntemplate<class mint>\r\nstd::vector<mint> intt(std::vector<mint> f) {\r\n\
    \  _ntt::_ifft(f);\r\n  return f;\r\n}\r\n\r\ntemplate<class mint>\r\nstd::vector<mint>\
    \ convolution(std::vector<mint> f, std::vector<mint> g) {\r\n  if (f.empty() or\
    \ g.empty())\r\n    return {};\r\n  int l = 1 << math::ceil_pow2(f.size() + g.size()\
    \ - 1);\r\n  f.resize(l);\r\n  ntt_inline(f);\r\n  g.resize(l);\r\n  ntt_inline(g);\r\
    \n  int n = f.size();\r\n  for (int i = 0; i < n; i++)\r\n    f[i] *= g[i];\r\n\
    \  intt_inline(f);\r\n  auto n_inv = mint(n).inv();\r\n  for (int i = 0; i < n;\
    \ i++)\r\n    f[i] *= n_inv;\r\n  return f;\r\n}\r\n\r\nsize_t complexity_of_convolution(size_t\
    \ fsize, size_t gsize) {\r\n  int lg = math::ceil_pow2(fsize + gsize - 1);\r\n\
    \  return lg << lg;\r\n}\r\n#line 4 \"include/mtl/fps.hpp\"\n#include <initializer_list>\n\
    #line 6 \"include/mtl/fps.hpp\"\n#include <algorithm>\n\ntemplate<int Mod = 998244353>\n\
    class Fps : public std::vector<Modular<Mod>> {\n  using _base = std::vector<Modular<Mod>>;\n\
    \ public:\n  using mint = Modular<Mod>;\n  using f = std::vector<mint>;\n  explicit\
    \ Fps(size_t n=0, mint v=0) : f(n, v) {}\n  template<typename It> Fps(It begin,\
    \ It end) : f(begin, end) {}\n  Fps(std::initializer_list<mint> l) : f(l) {}\n\
    \  explicit Fps(const std::vector<mint>& l) : f(l) {}\n  explicit Fps(std::vector<mint>&&\
    \ l) : f(std::move(l)) {}\n\n  Fps& normalize() {\n    int n = (int) _base::size();\n\
    \    while (n-1 >= 0 and (*this)[n-1] == 0) n--;\n    _base::resize(n);\n    return\
    \ *this;\n  }\n  Fps& inline_pre(size_t m) {\n    _base::resize(m, 0);\n    return\
    \ *this;\n  }\n  /// f mod x^m\n  Fps pre(size_t m) const {\n    if (m <= _base::size())\n\
    \      return Fps(_base::begin(), _base::begin()+m);\n    else \n      return\
    \ Fps(*this).inline_pre(m);\n  }\n  bool operator==(const Fps& rhs) const {\n\
    \    for (size_t i = 0, n = std::max(_base::size(), rhs.size()); i < n; i++) {\n\
    \      auto l = i < _base::size() ? (*this)[i] : 0;\n      auto r = i < rhs.size()\
    \ ? rhs[i] : 0;\n      if (l != r) return false;\n    }\n    return true;\n  }\n\
    \  bool operator!=(const Fps& rhs) const {\n    return !(*this == rhs);\n  }\n\
    \  Fps& operator+=(mint x) {\n    if (_base::empty())\n      _base::resize(1);\n\
    \    (*this)[0] += x;\n    return *this;\n  }\n  Fps& operator-=(mint x) { return\
    \ *this += -x; }\n  Fps& operator*=(mint x) {\n    for (auto& v : *this) v *=\
    \ x;\n    return *this;\n  }\n  Fps& operator/=(mint x) { return *this *= x.inv();\
    \ }\n  Fps operator+(mint x) const { return Fps(*this) += x; }\n  Fps operator-(mint\
    \ x) const { return Fps(*this) -= x; }\n  Fps operator*(mint x) const { return\
    \ Fps(*this) *= x; }\n  Fps operator/(mint x) const { return Fps(*this) /= x;\
    \ }\n  Fps operator-() const {\n    auto g = *this;\n    for (auto& v : g) v =\
    \ -v;\n    return g;\n  }\n  friend Fps operator+(mint x, const Fps& y) { return\
    \ y+x; }\n  friend Fps operator-(mint x, const Fps& y) { return -y+x; }\n  friend\
    \ Fps operator*(mint x, const Fps& y) { return y*x; }\n  friend Fps operator/(mint\
    \ x, const Fps& y) { return y.inv()*x; }\n  Fps& operator+=(const Fps& r) {\n\
    \    if (r.size() > _base::size())\n      _base::resize(r.size());\n    for (size_t\
    \ i = 0; i < r.size(); i++)\n      (*this)[i] += r[i];\n    return *this;\n  }\n\
    \  Fps& operator-=(const Fps& r) {\n    if (r.size() > _base::size())\n      _base::resize(r.size());\n\
    \    for (size_t i = 0; i < r.size(); i++)\n      (*this)[i] -= r[i];\n    return\
    \ *this;\n  }\n  Fps& dot(const Fps& r) {\n    if (_base::size() > r.size())\n\
    \      _base::resize(r.size());\n    for (size_t i = 0; i < _base::size(); i++)\n\
    \      (*this)[i] *= r[i];\n    return *this;\n  }\n  size_t count_terms(int n\
    \ = -1) const {\n    if (n == -1) n = (int)_base::size();\n    n = std::min(n,\
    \ (int)_base::size());\n    return std::count_if(_base::begin(), _base::begin()+n,\
    \ [](mint x) { return x != 0; });\n  }\n  std::vector<std::pair<size_t, mint>>\
    \ term_ties() const {\n    return term_ties(0, _base::size());\n  }\n  std::vector<std::pair<size_t,\
    \ mint>> term_ties(size_t n) const {\n    return term_ties(0, n);\n  }\n  std::vector<std::pair<size_t,\
    \ mint>> term_ties(size_t front, size_t back) const {\n    std::vector<std::pair<size_t,\
    \ mint>> ret;\n    for (size_t i = front; i < back; i++)\n      if ((*this)[i]\
    \ != 0)\n        ret.emplace_back(i, (*this)[i]);\n    return ret;\n  }\n private:\n\
    \  template<class F>\n  Fps& _mul_set_dense(F&& r) {\n    return *this = Fps(convolution(std::move(*this),\
    \ std::forward<F>(r)));\n  }\n  /** \n   * Complexity: O(NK) \n   *          \
    \   where N is count of non-zero terms of self and K is count of non-zero terms\
    \ of r\n  */\n  Fps& _mul_set_sparse(const Fps& r) {\n    auto ri = r.term_ties();\n\
    \    if (ri.empty()) return *this = Fps();\n    Fps ret(_base::size() + ri.back().first);\n\
    \    for (size_t i = 0; i < _base::size(); i++) if ((*this)[i] != 0) for (auto\
    \ j:ri) {\n      ret[i + j.first] += (*this)[i] * j.second;\n    }\n    return\
    \ *this = ret;\n  }\n  template<class F>\n  Fps& _mul_set(F&& r) {\n    return\n\
    \      r.count_terms() < 100 ?\n      _mul_set_sparse(std::forward<F>(r)) :\n\
    \      _mul_set_dense(std::forward<F>(r));\n  }\n public:\n  Fps& operator*=(const\
    \ Fps& r) {\n    return _mul_set(r);\n  }\n  Fps& operator*=(Fps&& r) {\n    return\
    \ _mul_set(std::move(r));\n  }\n  Fps inv_dense(int n = -1) const {\n    assert(!_base::empty()\
    \ and (*this)[0] != 0);\n    if (n == -1) n = (int) _base::size();\n    if (n\
    \ == 0) return Fps();\n    // Newton descent\n    // find g, s.t. F(g) = a\n \
    \   //   g_{n+1} = g_n - (F(g_n) - a) / F'(g_n)\n    // find g, s.t. g^{-1} =\
    \ f\n    //   g_{n+1} = g_n - (g_n^{-1} - f) / -g_n^{-2}\n    //           = 2g_n\
    \ - g_n^2 f\n    //   g_0 = f_0^{-1}\n    Fps g,fm,fgg;\n    g.reserve(n); fm.reserve(n);\
    \ fgg.reserve(n*2-1);\n    g.push_back((*this)[0].inv());\n    fm.push_back((*this)[0]);\n\
    \    for (int m = 1; m < n; m <<= 1) {\n      int nm = std::min(m*2, n);\n   \
    \   fm.resize(nm);\n      for (int i = m; i < std::min(nm, (int)_base::size());\
    \ i++)\n        fm[i] = (*this)[i];\n      fgg = g;\n      fgg *= g;\n      fgg\
    \ *= fm;\n      fgg.inline_pre(nm);\n      g.resize(nm);\n      for (int i = m;\
    \ i < nm; i++)\n        g[i] = -fgg[i];\n    }\n    // assert((pre(n)*g).pre(n)\
    \ == Fps{1});\n    return g;\n  }\n  Fps inv_sparse(int n = -1) const {\n    assert(!_base::empty()\
    \ and (*this)[0] != 0);\n    if (n == -1) n = (int) _base::size();\n    if (n\
    \ == 0) return Fps();\n    // fg = 1 => (n>0) sum_i f_i g_{n-i} = 0\n    // f_0\
    \ g_n = - sum_{i=1}^n f_i g_{n-i}\n    auto tf = term_ties(n);\n    Fps g(n);\n\
    \    auto ifz = (*this)[0].inv();\n    g[0] = ifz;\n    for (size_t i = 1; i <\
    \ (size_t) n; i++) {\n      mint s = 0;\n      for (size_t j = 1; j < tf.size();\
    \ j++) {\n        if (tf[j].first > i) break;\n        s += tf[j].second * g[i-tf[j].first];\n\
    \      }\n      g[i] = -s * ifz;\n    }\n    // assert((pre(n)*g).pre(n) == Fps{1});\n\
    \    return g;\n  }\n  Fps inv(int n = -1) const {\n    if (n == -1) n = (int)\
    \ _base::size();\n    return count_terms(n) < 100 ? inv_sparse(n) : inv_dense(n);\n\
    \  }\n  Fps& operator/=(const Fps& r) {\n    return *this *= r.inv();\n  }\n \
    \ // f x^m = sum f_i x^{i+m}\n  Fps operator<<(size_t m) const {\n    Fps ret(m\
    \ + _base::size());\n    std::copy(_base::begin(), _base::end(), ret.begin()+m);\n\
    \    return ret;\n  }\n  // sum f_{i+m} x^i\n  Fps operator>>(size_t m) const\
    \ {\n    if (m >= _base::size()) return Fps();\n    return Fps(_base::begin()+m,\
    \ _base::end());\n  }\n  Fps& operator<<=(size_t m) {\n    size_t s = _base::size();\n\
    \    _base::resize(m + s);\n    for (size_t i = 0; i < s; i++)\n      (*this)[m+s-1-i]\
    \ = (*this)[s-1-i];\n    std::fill(_base::begin(), _base::begin()+m, 0);\n   \
    \ return *this;\n  }\n  Fps& operator>>=(size_t m) {\n    if (m >= _base::size())\
    \ return *this = Fps();\n    _base::erase(_base::begin(), _base::begin()+m);\n\
    \    return *this;\n  }\n  Fps& inline_diff() {\n    if (_base::empty()) return\
    \ *this;\n    for (size_t i = 1; i < _base::size(); i++)\n      (*this)[i-1] =\
    \ (*this)[i] * i;\n    _base::pop_back();\n    return *this;\n  }\n  Fps diff()\
    \ const {\n    return Fps(*this).inline_diff();\n  }\n  Fps& inline_inte() {\n\
    \    if (_base::empty()) return *this;\n    _base::push_back(0);\n    ModularUtil<mint>\
    \ mu;\n    mu.set_inv(_base::size());\n    for (int i = (int) _base::size()-1;\
    \ i > 0; i--)\n      (*this)[i] = (*this)[i-1] * mu.inv(i);\n    (*this)[0] =\
    \ 0;\n    return *this;\n  }\n  Fps inte() const {\n    return Fps(*this).inline_inte();\n\
    \  }\n  Fps log_dense(int n = -1) const {\n    assert(!_base::empty() and _base::operator[](0)\
    \ == 1);\n    if (n == -1) n = (int) _base::size();\n    // integral(f' / f)\n\
    \    return (diff() * inv_dense(n-1)).inline_pre(n-1).inline_inte().inline_pre(n);\n\
    \  }\n  Fps log_sparse(int n = -1) const {\n    assert(!_base::empty() and _base::operator[](0)\
    \ == 1);\n    if (n == -1) n = (int) _base::size();\n    // integral(f' / f)\n\
    \    return (diff() * inv_sparse(n-1)).inline_pre(n-1).inline_inte().inline_pre(n);\n\
    \  }\n  /**\n   * define log (1-f) = -sum_n f^n / n\n   * satisfy log(f)' = f'/f,\
    \ log(fg) = log f + log g\n  */\n  Fps log(int n = -1) const {\n    assert(!_base::empty()\
    \ and _base::operator[](0) == 1);\n    if (n == -1) n = (int) _base::size();\n\
    \    // integral(f' / f)\n    return (diff() * inv(n-1)).inline_pre(n-1).inline_inte().inline_pre(n);\n\
    \  }\n  Fps exp_dense(int n = -1) const;\n  Fps exp_sparse(int n = -1) const;\n\
    \  /**\n   * define exp f = sum_n f^n / n!\n   * satisfy (exp f)' = (exp f)f',\
    \ exp(f + g) = exp(f)exp(g)\n  */\n  Fps exp(int n = -1) const {\n    assert(_base::empty()\
    \ or _base::operator[](0) == 0);\n    return count_terms() < 100 ? exp_sparse(n)\
    \ : exp_dense(n);\n  }\n private:\n  Fps _pow_1_dense(long long n) const;\n  Fps\
    \ _pow_1_sparse(long long n) const;\n  Fps _pow_1(long long n) const {\n    assert(!_base::empty()\
    \ and _base::operator[](0) == 1);\n    return count_terms() < 100 ? _pow_1_sparse(n)\
    \ : _pow_1_dense(n);\n  }\n public:\n  /**\n   * f^n = exp(n log f)\n   * f^{ab}\
    \ = (f^a)^b, f^{a+b} = f^a f^b\n  */\n  Fps pow(long long n) const;\n private:\n\
    \  /**\n   * define f^{1/2}' = 1/2f^{-1/2} if [x^0]f = 1\n   * F = f^{1/2}\n \
    \  * fF' = 1/2 F f'\n   */\n  Fps _sqrt_1() const {\n    assert(!_base::empty()\
    \ and (*this)[0] == 1);\n    return _pow_1(mint(2).inv().val());\n  }\n public:\n\
    \  Fps sqrt() const {\n    // f = sum_i a_i x^i = c_k x^k sum_i a_i/c_k x^{i-k}\
    \ = c_k x^k g\n    // f^{1/2} = c_k^{1/2} x^{k/2} g^{1/2}\n    // s.t. c_k is\
    \ square and k is even\n    size_t k = 0;\n    while (k < _base::size() and (*this)[k]\
    \ == 0) k++;\n    if (k == _base::size()) return Fps();\n    if (k%2==1) \n  \
    \    throw std::runtime_error(\"minimum degree must be even\");\n    auto ck =\
    \ (*this)[k];\n    if (!ck.is_square())\n      throw std::runtime_error(\"not\
    \ square\");\n    auto g = *this >> k;\n    g /= ck;\n    g = g._sqrt_1();\n \
    \   g *= ck.sqrt();\n    g <<= k/2;\n    return g;\n  }\n  bool is_square() const\
    \ {\n    size_t k = 0;\n    while (k < _base::size() and (*this)[k] == 0) k++;\n\
    \    if (k == _base::size()) return true;\n    if (k%2==1) \n      return false;\n\
    \    if (!(*this)[k].is_square()) \n      return false;\n    return true;\n  }\n\
    \ private:\n  template<class F>\n  Fps& _mod_set(F&& r) {\n    normalize();\n\
    \    Fps q = *this / r;\n    return *this -= q * std::forward<F>(r);\n  }\n public:\n\
    \  Fps& operator%=(const Fps& r) {\n    return _mod_set(r);\n  }\n  Fps& operator%=(Fps&&\
    \ r) {\n    return _mod_set(std::move(r));\n  }\n  Fps operator+(const Fps& r)\
    \ const { return Fps(*this) += r; }\n  Fps operator+(Fps&& r) const { return Fps(*this)\
    \ += std::move(r); }\n  Fps operator-(const Fps& r) const { return Fps(*this)\
    \ -= r; }\n  Fps operator-(Fps&& r) const { return Fps(*this) -= std::move(r);\
    \ }\n  Fps operator*(const Fps& r) const { return Fps(*this) *= r; }\n  Fps operator*(Fps&&\
    \ r) const { return Fps(*this) *= std::move(r); }\n  Fps operator/(const Fps&\
    \ r) const { return Fps(*this) /= r; }\n  Fps operator/(Fps&& r) const { return\
    \ Fps(*this) /= std::move(r); }\n  Fps operator%(const Fps& r) const { return\
    \ Fps(*this) %= r; }\n  Fps operator%(Fps&& r) const { return Fps(*this) %= std::move(r);\
    \ }\n};\n#line 3 \"include/mtl/fps/exp.hpp\"\n\n/**\n * We adopt version2 of the\
    \ exponential computation algorithm in [1].\n * reference:\n *  [1] Alin Bostan\
    \ and Eric Schost, A simple and fast algorithm for computing exponentials of power\
    \ series, Information Processing Letters 109, 13 (2009) 754-756\n *      https://doi.org/10.1016/j.ipl.2009.03.012\n\
    */\ntemplate<int M>\nFps<M> Fps<M>::exp_dense(int n) const {\n    // Newton descent\n\
    \    // find g, s.t. F(g) = a\n    //   g_{n+1} = g_n - (F(g_n) - a) / F'(g_n)\n\
    \    // find g, s.t. g = exp f => log g = f\n    //   g_{n+1} = g_n - (log g_n\
    \ - f) / g_n^{-1}\n    //           = g_n + g_n (f - log g_n)\n    //        \
    \   = g_n + g_n (f - inte(h' + (g_n' - g_nh')g_n^{-1}))\n    //   g_0 = 1\n  \
    \  assert(this->empty() or this->operator[](0) == 0);\n    if (n == -1) n = (int)\
    \ this->size();\n    if (n == 0) return Fps();\n    Fps<M> f, g, t, q;\n    //\
    \ f = (exp h)_0 = {1}\n    // g = (exp h)^{-1}_0 = {1}\n    // q = h' = {}\n \
    \   f.reserve(n*2-1);\n    f.push_back(1);\n    g.reserve(n-1);\n    g.push_back(1);\
    \ \n    t.reserve(n*2-1);\n    q.reserve(n-1); \n    auto term = [&](size_t i)\
    \ {\n      return i < this->size() ? this->operator[](i) : 0;\n    };\n    ModularUtil<mint>\
    \ mu;\n    mu.set_inv(n);\n    for (size_t i = 1; i < (size_t)n; i *= 2) {\n \
    \     /*\n      Exact computation\n      f = f + f*(pre(i*2) - f.log(i*2));\n\
    \      f.inline_pre(ni);\n      */\n      if (i > 1) {\n        // t = (f*g*g).pre(i)\n\
    \        t = g;\n        t *= g;\n        t *= f;\n        t.inline_pre(i);\n\
    \        // g = (2*g-f*g*g).pre(i)\n        g.inline_pre(i);\n        for (size_t\
    \ j = i/2; j < i; j++)\n          g[j] = -t[j];\n        // q = pre(i).diff()\n\
    \        q.resize(i-1);\n        for (size_t j = i/2; j < i; j++)\n          q[j-1]\
    \ = term(j) * j;\n      }\n      auto ni = i*2;\n      auto l = std::min(ni, (size_t)\
    \ n);\n      // t = fq % x^{2i-1}\n      t = f;\n      t *= q;\n      t.inline_pre(l-1);\n\
    \      // t = fqg div x^{i-1}\n      t >>= i-1;\n      t *= g;\n      t.inline_pre(l-i+1);\n\
    \      // t = (h + (t x^{i-1}).inte()) div x^m\n      for (size_t j = 0; j < l-i+1;\
    \ j++)\n        t[j] = t[j] * mu.inv(i+j) + term(i+j);\n      t *= f;\n      t.inline_pre(l-i);\n\
    \      f.inline_pre(l);\n      for (size_t j = i; j < l; j++)\n        f[j] =\
    \ t[j-i];\n    }\n    return f.inline_pre(n);\n}\n\ntemplate<int M>\nFps<M> Fps<M>::exp_sparse(int\
    \ n) const {\n    assert(this->empty() or this->operator[](0) == 0);\n    if (n\
    \ == -1) n = (int) this->size();\n    if (n == 0) return Fps();\n    if (this->empty())\
    \ return Fps(n);\n    // F = exp f\n    // F' = Ff'\n    // F'_{n-1} = sum_{i=1}^{n-1}\
    \ F_i f'_{n-i-1}, F_0 = 0;\n    auto td = pre(std::min(n, (int)this->size())).diff().term_ties();\n\
    \    Fps<M> ret(n);\n    ret[0] = 1;\n    ModularUtil<mint> mu;\n    mu.set_inv(n-1);\n\
    \    for (int i = 1; i < n; i++) {\n      Fps<M>::mint pf = 0;\n      for (auto&\
    \ t:td) {\n        if ((int)t.first > i-1) break;\n        pf += ret[i-1-t.first]\
    \ * t.second;\n      }\n      ret[i] = pf * mu.inv(i);\n    }\n    return ret;\n\
    }\n#line 3 \"include/mtl/fps/pow.hpp\"\n\ntemplate<int M>\nFps<M> Fps<M>::_pow_1_dense(long\
    \ long n) const {\n    return (log_dense() * n).exp_dense();\n}\n\ntemplate<int\
    \ M>\nFps<M> Fps<M>::_pow_1_sparse(long long n) const {\n    // F = f^n\n    //\
    \ fF' = NFf'\n    // sum_i f_i F'_{n-i} = N sum_i F_{n-i} f'_i\n    // F'_n =\
    \ -sum_{i>0} f_i F'_{n-i} + N sum_i F_{n-i} f'_i\n    // F'_n = -sum_{i>0} f_i\
    \ F_{n-i+1} (n-i+1) + N sum_{i>0} F_{n-i+1} if_i\n    auto tf = term_ties(1, _base::size());\n\
    \    if (tf.empty()) return Fps{1};\n    Fps ret(_base::size());\n    ret[0] =\
    \ 1;\n    ModularUtil<mint> mu;\n    mu.set_inv(_base::size()-1);\n    mint N\
    \ = n;\n    for (size_t i = 1; i < _base::size(); i++) {\n      mint f = 0;\n\
    \      for (auto& t:tf) {\n        if (i < t.first) break;\n        auto j = i\
    \ - t.first;\n        auto c = t.second * ret[j];\n        f += c * (N * t.first\
    \ - j);\n      }\n      ret[i] = f * mu.inv(i);\n    }\n    return ret;\n}\n\n\
    template<int M>\nFps<M> Fps<M>::pow(long long n) const {\n    // f = sum c_i x^i\
    \ = c_k x^k sum c_{k+i}/c_k x^i = c_k x^k g\n    // [x^0]g = 1\n    // f^n = c_k^n\
    \ x^{kn} g^n\n    if (n == 0) \n      return Fps{1};\n    size_t k = 0;\n    for\
    \ (; k < _base::size(); k++)\n      if ((*this)[k] != 0) break;\n    if (k ==\
    \ _base::size() or\n        (k and n >= (long long)((_base::size()-1)/k+1))) \n\
    \      return Fps();\n    if ((*this)[0] == 1)\n      return _pow_1(n);\n    auto\
    \ c = (*this)[k];\n    auto ic = c.inv();\n    Fps g(_base::size()-k);\n    for\
    \ (size_t i = k; i < _base::size(); i++)\n      g[i-k] = (*this)[i] * ic;\n  \
    \  g = g._pow_1(n);\n    auto d = k * n;\n    Fps res(d+g.size());\n    auto cpn\
    \ = c.pow(n);\n    for (size_t i = 0; i < g.size(); i++)\n      res[d+i] = g[i]\
    \ * cpn;\n    return res;\n}\n"
  code: "#pragma once\n#include \"../fps/exp.hpp\"\n\ntemplate<int M>\nFps<M> Fps<M>::_pow_1_dense(long\
    \ long n) const {\n    return (log_dense() * n).exp_dense();\n}\n\ntemplate<int\
    \ M>\nFps<M> Fps<M>::_pow_1_sparse(long long n) const {\n    // F = f^n\n    //\
    \ fF' = NFf'\n    // sum_i f_i F'_{n-i} = N sum_i F_{n-i} f'_i\n    // F'_n =\
    \ -sum_{i>0} f_i F'_{n-i} + N sum_i F_{n-i} f'_i\n    // F'_n = -sum_{i>0} f_i\
    \ F_{n-i+1} (n-i+1) + N sum_{i>0} F_{n-i+1} if_i\n    auto tf = term_ties(1, _base::size());\n\
    \    if (tf.empty()) return Fps{1};\n    Fps ret(_base::size());\n    ret[0] =\
    \ 1;\n    ModularUtil<mint> mu;\n    mu.set_inv(_base::size()-1);\n    mint N\
    \ = n;\n    for (size_t i = 1; i < _base::size(); i++) {\n      mint f = 0;\n\
    \      for (auto& t:tf) {\n        if (i < t.first) break;\n        auto j = i\
    \ - t.first;\n        auto c = t.second * ret[j];\n        f += c * (N * t.first\
    \ - j);\n      }\n      ret[i] = f * mu.inv(i);\n    }\n    return ret;\n}\n\n\
    template<int M>\nFps<M> Fps<M>::pow(long long n) const {\n    // f = sum c_i x^i\
    \ = c_k x^k sum c_{k+i}/c_k x^i = c_k x^k g\n    // [x^0]g = 1\n    // f^n = c_k^n\
    \ x^{kn} g^n\n    if (n == 0) \n      return Fps{1};\n    size_t k = 0;\n    for\
    \ (; k < _base::size(); k++)\n      if ((*this)[k] != 0) break;\n    if (k ==\
    \ _base::size() or\n        (k and n >= (long long)((_base::size()-1)/k+1))) \n\
    \      return Fps();\n    if ((*this)[0] == 1)\n      return _pow_1(n);\n    auto\
    \ c = (*this)[k];\n    auto ic = c.inv();\n    Fps g(_base::size()-k);\n    for\
    \ (size_t i = k; i < _base::size(); i++)\n      g[i-k] = (*this)[i] * ic;\n  \
    \  g = g._pow_1(n);\n    auto d = k * n;\n    Fps res(d+g.size());\n    auto cpn\
    \ = c.pow(n);\n    for (size_t i = 0; i < g.size(); i++)\n      res[d+i] = g[i]\
    \ * cpn;\n    return res;\n}"
  dependsOn:
  - include/mtl/fps/exp.hpp
  - include/mtl/fps.hpp
  - include/mtl/ntt.hpp
  - include/mtl/modular.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/fps/pow.hpp
  requiredBy: []
  timestamp: '2023-04-29 13:51:09+09:00'
  verificationStatus: LIBRARY_SOME_WA
  verifiedWith:
  - test/yosupo/polynomial/sqrt_of_formal_power_series.test.cpp
  - test/yosupo/polynomial/sqrt_of_formal_power_series_sparse.test.cpp
  - test/yosupo/polynomial/pow_of_formal_power_series.test.cpp
  - test/yosupo/polynomial/pow_of_formal_power_series_sparse.test.cpp
documentation_of: include/mtl/fps/pow.hpp
layout: document
redirect_from:
- /library/include/mtl/fps/pow.hpp
- /library/include/mtl/fps/pow.hpp.html
title: include/mtl/fps/pow.hpp
---
