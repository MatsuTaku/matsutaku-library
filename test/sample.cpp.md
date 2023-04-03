---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 1 \"test/sample.cpp\"\n// -----------------------------------\r\
    \n// Author     : MatsuTaku\r\n// Affiliation: Tokushima University\r\n// Country\
    \    : Japan\r\n// Date       : 03/14/2020\r\n// -----------------------------------\r\
    \n\r\n#include <bits/stdc++.h>\r\n\r\nusing namespace std;\r\nusing ll = long\
    \ long;\r\n\r\ntemplate <typename T, T MOD>\r\nclass modint {\r\n private:\r\n\
    \  T val_;\r\n\r\n public:\r\n  constexpr modint() : val_(0) {}\r\n  constexpr\
    \ modint(T v) : val_(v%MOD) {}\r\n  constexpr modint& operator=(T v) { val_ =\
    \ v%MOD; return *this; }\r\n\r\n  constexpr T val() const { return val_; }\r\n\
    \  constexpr modint operator+(modint x) const { return (val() + x.val())%MOD;\
    \ }\r\n  constexpr modint operator+=(modint x) { return *this = *this + x; }\r\
    \n  constexpr modint operator++() { return *this += 1; }\r\n  constexpr modint\
    \ operator++(int) { modint c = *this; ++(*this); return c; }\r\n  constexpr modint\
    \ operator-(modint x) const { return (val() + MOD - x.val())%MOD; }\r\n  constexpr\
    \ modint operator-=(modint x) { return *this = *this - x; }\r\n  constexpr modint\
    \ operator--() { return *this -= 1; }\r\n  constexpr modint operator--(int) {\
    \ modint c = *this; --(*this); return c; }\r\n  constexpr modint operator*(modint\
    \ x) const { return (val() * x.val())%MOD; }\r\n  constexpr modint operator*=(modint\
    \ x) { return *this = *this * x; }\r\n  friend constexpr modint pow(modint x,\
    \ T p) {\r\n    modint t = 1;\r\n    modint u = x;\r\n    while (p) {\r\n    \
    \  if (p & 1)\r\n        t *= u;\r\n      u *= u;\r\n      p >>= 1;\r\n    }\r\
    \n    return t;\r\n  }\r\n  constexpr modint operator/(modint x) const { return\
    \ *this * pow(modint{x}, MOD-2); }\r\n  constexpr modint operator/=(modint x)\
    \ { return *this = *this / x; }\r\n\r\n  constexpr bool operator==(modint x) const\
    \ { return val() == x.val(); }\r\n  constexpr bool operator!=(modint x) const\
    \ { return val() != x.val(); }\r\n  constexpr bool operator<(modint x) const {\
    \ return val() < x.val(); };\r\n  constexpr bool operator<=(modint x) const {\
    \ return val() <= x.val(); };\r\n  constexpr bool operator>(modint x) const {\
    \ return val() > x.val(); };\r\n  constexpr bool operator>=(modint x) const {\
    \ return val() >= x.val(); };\r\n\r\n  friend std::ostream& operator<<(std::ostream&\
    \ os, modint x) {\r\n    return os << x.val();\r\n  }\r\n  friend std::istream&\
    \ operator>>(std::istream& is, modint& x) {\r\n    return is >> x.val_;\r\n  }\r\
    \n\r\n};\r\n\r\nclass NTT {\r\n public:\r\n  using int_type = unsigned long long;\r\
    \n  static constexpr int_type P = 998244353;\r\n  using mint = modint<int_type,\
    \ P>;\r\n private:\r\n  size_t n_;\r\n  mint r_;\r\n  std::vector<mint> coeff_;\r\
    \n  std::vector<mint> icoeff_;\r\n  size_t log_n_;\r\n  size_t clz_n_;\r\n\r\n\
    \  int_type bit_reverse(int_type x) const {\r\n    x = ((x & 0x00000000FFFFFFFF)\
    \ << 32) | ((x & 0xFFFFFFFF00000000) >> 32);\r\n    x = ((x & 0x0000FFFF0000FFFF)\
    \ << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);\r\n    x = ((x & 0x00FF00FF00FF00FF)\
    \ << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);\r\n    x = ((x & 0x0F0F0F0F0F0F0F0F)\
    \ << 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);\r\n    x = ((x & 0x3333333333333333)\
    \ << 2) | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);\r\n    x = ((x & 0x5555555555555555)\
    \ << 1) | ((x & 0xAAAAAAAAAAAAAAAA) >> 1);\r\n    return x >> (clz_n_+1);\r\n\
    \  }\r\n\r\n public:\r\n  explicit NTT(size_t size) {\r\n    n_ = 1;\r\n    log_n_\
    \ = 0;\r\n    clz_n_ = 63;\r\n    while (n_ < size) {\r\n      n_*=2;\r\n    \
    \  log_n_++;\r\n      clz_n_--;\r\n    }\r\n    assert((P-1)%n_ == 0);\r\n   \
    \ r_ = pow(mint{3}, (P-1)/n_);\r\n    coeff_.resize(log_n_);\r\n    icoeff_.resize(log_n_);\r\
    \n    mint iw = mint{1}/r_;\r\n    mint w = r_;\r\n    for (int i = log_n_-1;\
    \ i >= 0; i--) {\r\n      coeff_[i] = iw;\r\n      iw *= iw;\r\n      icoeff_[i]\
    \ = w;\r\n      w *= w;\r\n    }\r\n  };\r\n\r\n  size_t n() const { return n_;\
    \ }\r\n\r\n  void fft_inline(vector<mint>& f) const {\r\n    _fft(f);\r\n  }\r\
    \n\r\n  vector<mint> fft(const vector<mint>& f) const {\r\n    auto ff = f;\r\n\
    \    ff.resize(n_, 0);\r\n    _fft(ff);\r\n    return ff;\r\n  }\r\n\r\n  void\
    \ ifft_inline(vector<mint>& f) const {\r\n    _ifft(f);\r\n  }\r\n\r\n  vector<mint>\
    \ ifft(const vector<mint>& f) const {\r\n    auto ff = f;\r\n    ff.resize(n_,\
    \ 0);\r\n    _ifft(ff);\r\n    return ff;\r\n  }\r\n\r\n private:\r\n  template\
    \ <bool Forward>\r\n  void _fft_impl(vector<mint>& f) const {\r\n    // iterative\
    \ bit reversal\r\n    for (int_type i = 0; i < n_; i++) {\r\n      auto j = bit_reverse(i);\r\
    \n      if (i >= j)\r\n        continue;\r\n      swap(f[i], f[j]);\r\n    }\r\
    \n    // Cooley-Tukey FFT\r\n    for (size_t log_m = 0; log_m < log_n_; log_m++)\
    \ {\r\n      size_t m = 1ull<<log_m;\r\n      for (size_t chunk = 0; chunk < n_;\
    \ chunk += 2*m) {\r\n        mint w = 1;\r\n        for (size_t i = 0; i < m;\
    \ i++) {\r\n          auto p = chunk + i;\r\n          auto a = f[p];\r\n    \
    \      auto b = f[p + m] * w;\r\n          f[p + 0] = a + b;\r\n          f[p\
    \ + m] = a - b;\r\n          if (Forward) {\r\n            w *= coeff_[log_m];\r\
    \n          } else {\r\n            w *= icoeff_[log_m];\r\n          }\r\n  \
    \      }\r\n      }\r\n    }\r\n\r\n  }\r\n\r\n  void _fft(vector<mint>& f) const\
    \ {\r\n    _fft_impl<true>(f);\r\n  }\r\n  void _ifft(vector<mint>& f) const {\r\
    \n    _fft_impl<false>(f);\r\n    mint div_n = mint{1}/n_;\r\n    for (auto& x\
    \ : f)\r\n      x *= div_n;\r\n  }\r\n\r\n};\r\n\r\nconstexpr NTT::int_type NTT::P;\r\
    \n\r\nclass convolution {\r\n public:\r\n  using porinomial = std::vector<NTT::mint>;\r\
    \n private:\r\n  NTT ntt_;\r\n public:\r\n  explicit convolution(size_t size)\
    \ : ntt_(size) {}\r\n\r\n  porinomial operator()(const porinomial& g, const porinomial&\
    \ h) const {\r\n    auto fg = ntt_.fft(g);\r\n    auto fh = ntt_.fft(h);\r\n \
    \   for (size_t i = 0; i < ntt_.n(); i++) {\r\n      fg[i] *= fh[i];\r\n    }\r\
    \n    ntt_.ifft_inline(fg);\r\n    return fg;\r\n  }\r\n\r\n};\r\n\r\nint main()\
    \ {\r\n  cin.tie(nullptr); ios::sync_with_stdio(false);\r\n\r\n\r\n\r\n  return\
    \ 0;\r\n}\r\n"
  code: "// -----------------------------------\r\n// Author     : MatsuTaku\r\n//\
    \ Affiliation: Tokushima University\r\n// Country    : Japan\r\n// Date      \
    \ : 03/14/2020\r\n// -----------------------------------\r\n\r\n#include <bits/stdc++.h>\r\
    \n\r\nusing namespace std;\r\nusing ll = long long;\r\n\r\ntemplate <typename\
    \ T, T MOD>\r\nclass modint {\r\n private:\r\n  T val_;\r\n\r\n public:\r\n  constexpr\
    \ modint() : val_(0) {}\r\n  constexpr modint(T v) : val_(v%MOD) {}\r\n  constexpr\
    \ modint& operator=(T v) { val_ = v%MOD; return *this; }\r\n\r\n  constexpr T\
    \ val() const { return val_; }\r\n  constexpr modint operator+(modint x) const\
    \ { return (val() + x.val())%MOD; }\r\n  constexpr modint operator+=(modint x)\
    \ { return *this = *this + x; }\r\n  constexpr modint operator++() { return *this\
    \ += 1; }\r\n  constexpr modint operator++(int) { modint c = *this; ++(*this);\
    \ return c; }\r\n  constexpr modint operator-(modint x) const { return (val()\
    \ + MOD - x.val())%MOD; }\r\n  constexpr modint operator-=(modint x) { return\
    \ *this = *this - x; }\r\n  constexpr modint operator--() { return *this -= 1;\
    \ }\r\n  constexpr modint operator--(int) { modint c = *this; --(*this); return\
    \ c; }\r\n  constexpr modint operator*(modint x) const { return (val() * x.val())%MOD;\
    \ }\r\n  constexpr modint operator*=(modint x) { return *this = *this * x; }\r\
    \n  friend constexpr modint pow(modint x, T p) {\r\n    modint t = 1;\r\n    modint\
    \ u = x;\r\n    while (p) {\r\n      if (p & 1)\r\n        t *= u;\r\n      u\
    \ *= u;\r\n      p >>= 1;\r\n    }\r\n    return t;\r\n  }\r\n  constexpr modint\
    \ operator/(modint x) const { return *this * pow(modint{x}, MOD-2); }\r\n  constexpr\
    \ modint operator/=(modint x) { return *this = *this / x; }\r\n\r\n  constexpr\
    \ bool operator==(modint x) const { return val() == x.val(); }\r\n  constexpr\
    \ bool operator!=(modint x) const { return val() != x.val(); }\r\n  constexpr\
    \ bool operator<(modint x) const { return val() < x.val(); };\r\n  constexpr bool\
    \ operator<=(modint x) const { return val() <= x.val(); };\r\n  constexpr bool\
    \ operator>(modint x) const { return val() > x.val(); };\r\n  constexpr bool operator>=(modint\
    \ x) const { return val() >= x.val(); };\r\n\r\n  friend std::ostream& operator<<(std::ostream&\
    \ os, modint x) {\r\n    return os << x.val();\r\n  }\r\n  friend std::istream&\
    \ operator>>(std::istream& is, modint& x) {\r\n    return is >> x.val_;\r\n  }\r\
    \n\r\n};\r\n\r\nclass NTT {\r\n public:\r\n  using int_type = unsigned long long;\r\
    \n  static constexpr int_type P = 998244353;\r\n  using mint = modint<int_type,\
    \ P>;\r\n private:\r\n  size_t n_;\r\n  mint r_;\r\n  std::vector<mint> coeff_;\r\
    \n  std::vector<mint> icoeff_;\r\n  size_t log_n_;\r\n  size_t clz_n_;\r\n\r\n\
    \  int_type bit_reverse(int_type x) const {\r\n    x = ((x & 0x00000000FFFFFFFF)\
    \ << 32) | ((x & 0xFFFFFFFF00000000) >> 32);\r\n    x = ((x & 0x0000FFFF0000FFFF)\
    \ << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);\r\n    x = ((x & 0x00FF00FF00FF00FF)\
    \ << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);\r\n    x = ((x & 0x0F0F0F0F0F0F0F0F)\
    \ << 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);\r\n    x = ((x & 0x3333333333333333)\
    \ << 2) | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);\r\n    x = ((x & 0x5555555555555555)\
    \ << 1) | ((x & 0xAAAAAAAAAAAAAAAA) >> 1);\r\n    return x >> (clz_n_+1);\r\n\
    \  }\r\n\r\n public:\r\n  explicit NTT(size_t size) {\r\n    n_ = 1;\r\n    log_n_\
    \ = 0;\r\n    clz_n_ = 63;\r\n    while (n_ < size) {\r\n      n_*=2;\r\n    \
    \  log_n_++;\r\n      clz_n_--;\r\n    }\r\n    assert((P-1)%n_ == 0);\r\n   \
    \ r_ = pow(mint{3}, (P-1)/n_);\r\n    coeff_.resize(log_n_);\r\n    icoeff_.resize(log_n_);\r\
    \n    mint iw = mint{1}/r_;\r\n    mint w = r_;\r\n    for (int i = log_n_-1;\
    \ i >= 0; i--) {\r\n      coeff_[i] = iw;\r\n      iw *= iw;\r\n      icoeff_[i]\
    \ = w;\r\n      w *= w;\r\n    }\r\n  };\r\n\r\n  size_t n() const { return n_;\
    \ }\r\n\r\n  void fft_inline(vector<mint>& f) const {\r\n    _fft(f);\r\n  }\r\
    \n\r\n  vector<mint> fft(const vector<mint>& f) const {\r\n    auto ff = f;\r\n\
    \    ff.resize(n_, 0);\r\n    _fft(ff);\r\n    return ff;\r\n  }\r\n\r\n  void\
    \ ifft_inline(vector<mint>& f) const {\r\n    _ifft(f);\r\n  }\r\n\r\n  vector<mint>\
    \ ifft(const vector<mint>& f) const {\r\n    auto ff = f;\r\n    ff.resize(n_,\
    \ 0);\r\n    _ifft(ff);\r\n    return ff;\r\n  }\r\n\r\n private:\r\n  template\
    \ <bool Forward>\r\n  void _fft_impl(vector<mint>& f) const {\r\n    // iterative\
    \ bit reversal\r\n    for (int_type i = 0; i < n_; i++) {\r\n      auto j = bit_reverse(i);\r\
    \n      if (i >= j)\r\n        continue;\r\n      swap(f[i], f[j]);\r\n    }\r\
    \n    // Cooley-Tukey FFT\r\n    for (size_t log_m = 0; log_m < log_n_; log_m++)\
    \ {\r\n      size_t m = 1ull<<log_m;\r\n      for (size_t chunk = 0; chunk < n_;\
    \ chunk += 2*m) {\r\n        mint w = 1;\r\n        for (size_t i = 0; i < m;\
    \ i++) {\r\n          auto p = chunk + i;\r\n          auto a = f[p];\r\n    \
    \      auto b = f[p + m] * w;\r\n          f[p + 0] = a + b;\r\n          f[p\
    \ + m] = a - b;\r\n          if (Forward) {\r\n            w *= coeff_[log_m];\r\
    \n          } else {\r\n            w *= icoeff_[log_m];\r\n          }\r\n  \
    \      }\r\n      }\r\n    }\r\n\r\n  }\r\n\r\n  void _fft(vector<mint>& f) const\
    \ {\r\n    _fft_impl<true>(f);\r\n  }\r\n  void _ifft(vector<mint>& f) const {\r\
    \n    _fft_impl<false>(f);\r\n    mint div_n = mint{1}/n_;\r\n    for (auto& x\
    \ : f)\r\n      x *= div_n;\r\n  }\r\n\r\n};\r\n\r\nconstexpr NTT::int_type NTT::P;\r\
    \n\r\nclass convolution {\r\n public:\r\n  using porinomial = std::vector<NTT::mint>;\r\
    \n private:\r\n  NTT ntt_;\r\n public:\r\n  explicit convolution(size_t size)\
    \ : ntt_(size) {}\r\n\r\n  porinomial operator()(const porinomial& g, const porinomial&\
    \ h) const {\r\n    auto fg = ntt_.fft(g);\r\n    auto fh = ntt_.fft(h);\r\n \
    \   for (size_t i = 0; i < ntt_.n(); i++) {\r\n      fg[i] *= fh[i];\r\n    }\r\
    \n    ntt_.ifft_inline(fg);\r\n    return fg;\r\n  }\r\n\r\n};\r\n\r\nint main()\
    \ {\r\n  cin.tie(nullptr); ios::sync_with_stdio(false);\r\n\r\n\r\n\r\n  return\
    \ 0;\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/sample.cpp
  requiredBy: []
  timestamp: '2023-04-03 03:00:14+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/sample.cpp
layout: document
redirect_from:
- /library/test/sample.cpp
- /library/test/sample.cpp.html
title: test/sample.cpp
---
