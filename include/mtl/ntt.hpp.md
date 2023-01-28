---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/modular.hpp
    title: include/mtl/modular.hpp
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: include/mtl/fps.hpp
    title: include/mtl/fps.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/convolution.test.cpp
    title: test/convolution.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/modular.hpp\"\n#include <iostream>\n#include\
    \ <cassert>\n\ntemplate <int MOD>\nclass Modular {\n private:\n  unsigned int\
    \ val_;\n\n public:\n  static constexpr unsigned int mod() { return MOD; }\n \
    \ template<class T>\n  static constexpr unsigned int safe_mod(T v) {\n    auto\
    \ x = (long long)(v%(long long)mod());\n    if (x < 0) x += mod();\n    return\
    \ (unsigned int) x;\n  }\n\n  constexpr Modular() : val_(0) {}\n  template<class\
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
    \ int primitive_root_constexpr(int m) {\n  assert(m % 2 == 1);\n  if (m == 998244353)\
    \ return 3;\n\n  std::array<int, 20> divs{2};\n  int cnt = 1;\n  int x = (m-1)\
    \ / 2;\n  for (int d = 3; d*d <= x; d += 2) {\n    if (x % d == 0) {\n      divs[cnt++]\
    \ = d;\n      while (x % d == 0)\n        x /= d;\n    }\n  }\n  if (x > 1) divs[cnt++]\
    \ = x;\n  for (int g = 2; ; g++) {\n    bool ok = true;\n    for (int i = 0; i\
    \ < cnt; i++) {\n      if (mod_pow_constexpr(g, (m-1) / divs[cnt], m) == 1) {\n\
    \        ok = false;\n        break;\n      }\n    }\n    if (ok) return g;\n\
    \  }\n}\n\ntemplate<int m>\nconstexpr int primitive_root = primitive_root_constexpr(m);\n\
    \n}\n#line 2 \"include/mtl/bit_manip.hpp\"\n#include <cstdint>\n#line 4 \"include/mtl/bit_manip.hpp\"\
    \n\nnamespace bm {\n\ninline constexpr uint64_t popcnt_e8(uint64_t x) {\n  x =\
    \ (x & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);\n  x = (x & 0x3333333333333333)\
    \ + ((x>>2) & 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) &\
    \ 0x0F0F0F0F0F0F0F0F);\n  return x;\n}\n// Count 1s\ninline constexpr unsigned\
    \ popcnt(uint64_t x) {\n  return (popcnt_e8(x) * 0x0101010101010101) >> 56;\n\
    }\n// Count trailing 0s. ...01101000 -> 3\ninline constexpr unsigned ctz(uint64_t\
    \ x) {\n  return popcnt((x & (-x)) - 1);\n}\ninline constexpr unsigned ctz8(uint8_t\
    \ x) {\n  return x == 0 ? 8 : popcnt_e8((x & (-x)) - 1);\n}\n// [00..0](8bit)\
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
    \ 4 \"include/mtl/ntt.hpp\"\n#include <vector>\r\n#line 6 \"include/mtl/ntt.hpp\"\
    \n\r\nnamespace math {\r\n\r\nconstexpr int ceil_pow2(unsigned long long x) {\r\
    \n  return x == 0 ? 0 : 64-bm::clz(x-1);\r\n}\r\n\r\n}\r\n\r\nnamespace _ntt {\r\
    \n\r\ntemplate<int mod>\r\nstruct ntt_info {\r\n  using mint = Modular<mod>;\r\
    \n  static constexpr int primitive_root() {\r\n    return math::primitive_root<mod>;\r\
    \n  }\r\n  static constexpr int log_n() {\r\n    return bm::ctz(mod-1);\r\n  }\r\
    \n  static constexpr std::array<Modular<mod>, log_n()> coeff(bool forward) {\r\
    \n    mint r = mint(primitive_root()).pow((mod-1) >> log_n());\r\n    std::array<mint,\
    \ log_n()> coeff{};\r\n    mint iw = forward ? r.inv() : r;\r\n    for (int i\
    \ = log_n()-1; i >= 0; i--) {\r\n      coeff[i] = iw;\r\n      iw *= iw;\r\n \
    \   }\r\n    return coeff;\r\n  }\r\n};\r\n\r\ntemplate <bool Forward, int mod>\r\
    \nvoid _fft_impl(std::vector<Modular<mod>>& f) {\r\n  using mint = Modular<mod>;\r\
    \n  int log_n = math::ceil_pow2(f.size());\r\n  using info = ntt_info<mod>;\r\n\
    \  assert(info::log_n() >= log_n);\r\n  int n = 1 << log_n;\r\n  f.resize(n, 0);\r\
    \n  // iterative bit reversal\r\n  for (int i = 0, j = 1; j < n-1; j++) {\r\n\
    \    for (int k = n >> 1; k > (i ^= k); k >>= 1);\r\n    if (i < j) std::swap(f[i],\
    \ f[j]);\r\n  }\r\n  // Cooley-Tukey FFT\r\n  static constexpr auto coeff = info::coeff(Forward);\r\
    \n  for (int log_m = 0; log_m < log_n; log_m++) {\r\n    int m = 1<<log_m;\r\n\
    \    mint w0 = coeff[log_m];\r\n    for (int chunk = 0; chunk < n; chunk += 2*m)\
    \ {\r\n      mint w = 1;\r\n      for (int i = 0; i < m; i++) {\r\n        auto\
    \ p = chunk + i;\r\n        auto a = f[p + 0];\r\n        auto b = f[p + m] *\
    \ w;\r\n        f[p + 0] = a + b;\r\n        f[p + m] = a - b;\r\n        w *=\
    \ w0;\r\n      }\r\n    }\r\n  }\r\n}\r\ntemplate<class mint>\r\nvoid _fft(std::vector<mint>&\
    \ f) { _fft_impl<true>(f); }\r\ntemplate<class mint>\r\nvoid _ifft(std::vector<mint>&\
    \ f) { _fft_impl<false>(f); }\r\n\r\n}\r\n\r\ntemplate<class mint>\r\nvoid ntt_inline(std::vector<mint>&\
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
    \ i++)\r\n    f[i] *= n_inv;\r\n  return f;\r\n}\r\n"
  code: "#pragma once\r\n#include \"modular.hpp\"\r\n#include \"bit_manip.hpp\"\r\n\
    #include <vector>\r\n#include <cassert>\r\n\r\nnamespace math {\r\n\r\nconstexpr\
    \ int ceil_pow2(unsigned long long x) {\r\n  return x == 0 ? 0 : 64-bm::clz(x-1);\r\
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
    \ i++)\r\n    f[i] *= n_inv;\r\n  return f;\r\n}\r\n"
  dependsOn:
  - include/mtl/modular.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/ntt.hpp
  requiredBy:
  - include/mtl/fps.hpp
  timestamp: '2023-01-28 19:00:09+00:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/convolution.test.cpp
documentation_of: include/mtl/ntt.hpp
layout: document
redirect_from:
- /library/include/mtl/ntt.hpp
- /library/include/mtl/ntt.hpp.html
title: include/mtl/ntt.hpp
---
