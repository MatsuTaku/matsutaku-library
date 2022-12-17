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
  bundledCode: "#line 1 \"test/sample.cpp\"\n// -----------------------------------\n\
    // Author     : MatsuTaku\n// Affiliation: Tokushima University\n// Country  \
    \  : Japan\n// Date       : 03/14/2020\n// -----------------------------------\n\
    \n#include <bits/stdc++.h>\n\nusing namespace std;\nusing ll = long long;\n\n\
    template <typename T, T MOD>\nclass modint {\n private:\n  T val_;\n\n public:\n\
    \  constexpr modint() : val_(0) {}\n  constexpr modint(T v) : val_(v%MOD) {}\n\
    \  constexpr modint& operator=(T v) { val_ = v%MOD; return *this; }\n\n  constexpr\
    \ T val() const { return val_; }\n  constexpr modint operator+(modint x) const\
    \ { return (val() + x.val())%MOD; }\n  constexpr modint operator+=(modint x) {\
    \ return *this = *this + x; }\n  constexpr modint operator++() { return *this\
    \ += 1; }\n  constexpr modint operator++(int) { modint c = *this; ++(*this); return\
    \ c; }\n  constexpr modint operator-(modint x) const { return (val() + MOD - x.val())%MOD;\
    \ }\n  constexpr modint operator-=(modint x) { return *this = *this - x; }\n \
    \ constexpr modint operator--() { return *this -= 1; }\n  constexpr modint operator--(int)\
    \ { modint c = *this; --(*this); return c; }\n  constexpr modint operator*(modint\
    \ x) const { return (val() * x.val())%MOD; }\n  constexpr modint operator*=(modint\
    \ x) { return *this = *this * x; }\n  friend constexpr modint pow(modint x, T\
    \ p) {\n    modint t = 1;\n    modint u = x;\n    while (p) {\n      if (p & 1)\n\
    \        t *= u;\n      u *= u;\n      p >>= 1;\n    }\n    return t;\n  }\n \
    \ constexpr modint operator/(modint x) const { return *this * pow(modint{x}, MOD-2);\
    \ }\n  constexpr modint operator/=(modint x) { return *this = *this / x; }\n\n\
    \  constexpr bool operator==(modint x) const { return val() == x.val(); }\n  constexpr\
    \ bool operator!=(modint x) const { return val() != x.val(); }\n  constexpr bool\
    \ operator<(modint x) const { return val() < x.val(); };\n  constexpr bool operator<=(modint\
    \ x) const { return val() <= x.val(); };\n  constexpr bool operator>(modint x)\
    \ const { return val() > x.val(); };\n  constexpr bool operator>=(modint x) const\
    \ { return val() >= x.val(); };\n\n  friend std::ostream& operator<<(std::ostream&\
    \ os, modint x) {\n    return os << x.val();\n  }\n  friend std::istream& operator>>(std::istream&\
    \ is, modint& x) {\n    return is >> x.val_;\n  }\n\n};\n\nclass NTT {\n public:\n\
    \  using int_type = unsigned long long;\n  static constexpr int_type P = 998244353;\n\
    \  using mint = modint<int_type, P>;\n private:\n  size_t n_;\n  mint r_;\n  std::vector<mint>\
    \ coeff_;\n  std::vector<mint> icoeff_;\n  size_t log_n_;\n  size_t clz_n_;\n\n\
    \  int_type bit_reverse(int_type x) const {\n    x = ((x & 0x00000000FFFFFFFF)\
    \ << 32) | ((x & 0xFFFFFFFF00000000) >> 32);\n    x = ((x & 0x0000FFFF0000FFFF)\
    \ << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);\n    x = ((x & 0x00FF00FF00FF00FF)\
    \ << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);\n    x = ((x & 0x0F0F0F0F0F0F0F0F)\
    \ << 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);\n    x = ((x & 0x3333333333333333)\
    \ << 2) | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);\n    x = ((x & 0x5555555555555555)\
    \ << 1) | ((x & 0xAAAAAAAAAAAAAAAA) >> 1);\n    return x >> (clz_n_+1);\n  }\n\
    \n public:\n  explicit NTT(size_t size) {\n    n_ = 1;\n    log_n_ = 0;\n    clz_n_\
    \ = 63;\n    while (n_ < size) {\n      n_*=2;\n      log_n_++;\n      clz_n_--;\n\
    \    }\n    assert((P-1)%n_ == 0);\n    r_ = pow(mint{3}, (P-1)/n_);\n    coeff_.resize(log_n_);\n\
    \    icoeff_.resize(log_n_);\n    mint iw = mint{1}/r_;\n    mint w = r_;\n  \
    \  for (int i = log_n_-1; i >= 0; i--) {\n      coeff_[i] = iw;\n      iw *= iw;\n\
    \      icoeff_[i] = w;\n      w *= w;\n    }\n  };\n\n  size_t n() const { return\
    \ n_; }\n\n  void fft_inline(vector<mint>& f) const {\n    _fft(f);\n  }\n\n \
    \ vector<mint> fft(const vector<mint>& f) const {\n    auto ff = f;\n    ff.resize(n_,\
    \ 0);\n    _fft(ff);\n    return ff;\n  }\n\n  void ifft_inline(vector<mint>&\
    \ f) const {\n    _ifft(f);\n  }\n\n  vector<mint> ifft(const vector<mint>& f)\
    \ const {\n    auto ff = f;\n    ff.resize(n_, 0);\n    _ifft(ff);\n    return\
    \ ff;\n  }\n\n private:\n  template <bool Forward>\n  void _fft_impl(vector<mint>&\
    \ f) const {\n    // iterative bit reversal\n    for (int_type i = 0; i < n_;\
    \ i++) {\n      auto j = bit_reverse(i);\n      if (i >= j)\n        continue;\n\
    \      swap(f[i], f[j]);\n    }\n    // Cooley-Tukey FFT\n    for (size_t log_m\
    \ = 0; log_m < log_n_; log_m++) {\n      size_t m = 1ull<<log_m;\n      for (size_t\
    \ chunk = 0; chunk < n_; chunk += 2*m) {\n        mint w = 1;\n        for (size_t\
    \ i = 0; i < m; i++) {\n          auto p = chunk + i;\n          auto a = f[p];\n\
    \          auto b = f[p + m] * w;\n          f[p + 0] = a + b;\n          f[p\
    \ + m] = a - b;\n          if (Forward) {\n            w *= coeff_[log_m];\n \
    \         } else {\n            w *= icoeff_[log_m];\n          }\n        }\n\
    \      }\n    }\n\n  }\n\n  void _fft(vector<mint>& f) const {\n    _fft_impl<true>(f);\n\
    \  }\n  void _ifft(vector<mint>& f) const {\n    _fft_impl<false>(f);\n    mint\
    \ div_n = mint{1}/n_;\n    for (auto& x : f)\n      x *= div_n;\n  }\n\n};\n\n\
    constexpr NTT::int_type NTT::P;\n\nclass convolution {\n public:\n  using porinomial\
    \ = std::vector<NTT::mint>;\n private:\n  NTT ntt_;\n public:\n  explicit convolution(size_t\
    \ size) : ntt_(size) {}\n\n  porinomial operator()(const porinomial& g, const\
    \ porinomial& h) const {\n    auto fg = ntt_.fft(g);\n    auto fh = ntt_.fft(h);\n\
    \    for (size_t i = 0; i < ntt_.n(); i++) {\n      fg[i] *= fh[i];\n    }\n \
    \   ntt_.ifft_inline(fg);\n    return fg;\n  }\n\n};\n\nint main() {\n  cin.tie(nullptr);\
    \ ios::sync_with_stdio(false);\n\n\n\n  return 0;\n}\n"
  code: "// -----------------------------------\n// Author     : MatsuTaku\n// Affiliation:\
    \ Tokushima University\n// Country    : Japan\n// Date       : 03/14/2020\n//\
    \ -----------------------------------\n\n#include <bits/stdc++.h>\n\nusing namespace\
    \ std;\nusing ll = long long;\n\ntemplate <typename T, T MOD>\nclass modint {\n\
    \ private:\n  T val_;\n\n public:\n  constexpr modint() : val_(0) {}\n  constexpr\
    \ modint(T v) : val_(v%MOD) {}\n  constexpr modint& operator=(T v) { val_ = v%MOD;\
    \ return *this; }\n\n  constexpr T val() const { return val_; }\n  constexpr modint\
    \ operator+(modint x) const { return (val() + x.val())%MOD; }\n  constexpr modint\
    \ operator+=(modint x) { return *this = *this + x; }\n  constexpr modint operator++()\
    \ { return *this += 1; }\n  constexpr modint operator++(int) { modint c = *this;\
    \ ++(*this); return c; }\n  constexpr modint operator-(modint x) const { return\
    \ (val() + MOD - x.val())%MOD; }\n  constexpr modint operator-=(modint x) { return\
    \ *this = *this - x; }\n  constexpr modint operator--() { return *this -= 1; }\n\
    \  constexpr modint operator--(int) { modint c = *this; --(*this); return c; }\n\
    \  constexpr modint operator*(modint x) const { return (val() * x.val())%MOD;\
    \ }\n  constexpr modint operator*=(modint x) { return *this = *this * x; }\n \
    \ friend constexpr modint pow(modint x, T p) {\n    modint t = 1;\n    modint\
    \ u = x;\n    while (p) {\n      if (p & 1)\n        t *= u;\n      u *= u;\n\
    \      p >>= 1;\n    }\n    return t;\n  }\n  constexpr modint operator/(modint\
    \ x) const { return *this * pow(modint{x}, MOD-2); }\n  constexpr modint operator/=(modint\
    \ x) { return *this = *this / x; }\n\n  constexpr bool operator==(modint x) const\
    \ { return val() == x.val(); }\n  constexpr bool operator!=(modint x) const {\
    \ return val() != x.val(); }\n  constexpr bool operator<(modint x) const { return\
    \ val() < x.val(); };\n  constexpr bool operator<=(modint x) const { return val()\
    \ <= x.val(); };\n  constexpr bool operator>(modint x) const { return val() >\
    \ x.val(); };\n  constexpr bool operator>=(modint x) const { return val() >= x.val();\
    \ };\n\n  friend std::ostream& operator<<(std::ostream& os, modint x) {\n    return\
    \ os << x.val();\n  }\n  friend std::istream& operator>>(std::istream& is, modint&\
    \ x) {\n    return is >> x.val_;\n  }\n\n};\n\nclass NTT {\n public:\n  using\
    \ int_type = unsigned long long;\n  static constexpr int_type P = 998244353;\n\
    \  using mint = modint<int_type, P>;\n private:\n  size_t n_;\n  mint r_;\n  std::vector<mint>\
    \ coeff_;\n  std::vector<mint> icoeff_;\n  size_t log_n_;\n  size_t clz_n_;\n\n\
    \  int_type bit_reverse(int_type x) const {\n    x = ((x & 0x00000000FFFFFFFF)\
    \ << 32) | ((x & 0xFFFFFFFF00000000) >> 32);\n    x = ((x & 0x0000FFFF0000FFFF)\
    \ << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);\n    x = ((x & 0x00FF00FF00FF00FF)\
    \ << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);\n    x = ((x & 0x0F0F0F0F0F0F0F0F)\
    \ << 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);\n    x = ((x & 0x3333333333333333)\
    \ << 2) | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);\n    x = ((x & 0x5555555555555555)\
    \ << 1) | ((x & 0xAAAAAAAAAAAAAAAA) >> 1);\n    return x >> (clz_n_+1);\n  }\n\
    \n public:\n  explicit NTT(size_t size) {\n    n_ = 1;\n    log_n_ = 0;\n    clz_n_\
    \ = 63;\n    while (n_ < size) {\n      n_*=2;\n      log_n_++;\n      clz_n_--;\n\
    \    }\n    assert((P-1)%n_ == 0);\n    r_ = pow(mint{3}, (P-1)/n_);\n    coeff_.resize(log_n_);\n\
    \    icoeff_.resize(log_n_);\n    mint iw = mint{1}/r_;\n    mint w = r_;\n  \
    \  for (int i = log_n_-1; i >= 0; i--) {\n      coeff_[i] = iw;\n      iw *= iw;\n\
    \      icoeff_[i] = w;\n      w *= w;\n    }\n  };\n\n  size_t n() const { return\
    \ n_; }\n\n  void fft_inline(vector<mint>& f) const {\n    _fft(f);\n  }\n\n \
    \ vector<mint> fft(const vector<mint>& f) const {\n    auto ff = f;\n    ff.resize(n_,\
    \ 0);\n    _fft(ff);\n    return ff;\n  }\n\n  void ifft_inline(vector<mint>&\
    \ f) const {\n    _ifft(f);\n  }\n\n  vector<mint> ifft(const vector<mint>& f)\
    \ const {\n    auto ff = f;\n    ff.resize(n_, 0);\n    _ifft(ff);\n    return\
    \ ff;\n  }\n\n private:\n  template <bool Forward>\n  void _fft_impl(vector<mint>&\
    \ f) const {\n    // iterative bit reversal\n    for (int_type i = 0; i < n_;\
    \ i++) {\n      auto j = bit_reverse(i);\n      if (i >= j)\n        continue;\n\
    \      swap(f[i], f[j]);\n    }\n    // Cooley-Tukey FFT\n    for (size_t log_m\
    \ = 0; log_m < log_n_; log_m++) {\n      size_t m = 1ull<<log_m;\n      for (size_t\
    \ chunk = 0; chunk < n_; chunk += 2*m) {\n        mint w = 1;\n        for (size_t\
    \ i = 0; i < m; i++) {\n          auto p = chunk + i;\n          auto a = f[p];\n\
    \          auto b = f[p + m] * w;\n          f[p + 0] = a + b;\n          f[p\
    \ + m] = a - b;\n          if (Forward) {\n            w *= coeff_[log_m];\n \
    \         } else {\n            w *= icoeff_[log_m];\n          }\n        }\n\
    \      }\n    }\n\n  }\n\n  void _fft(vector<mint>& f) const {\n    _fft_impl<true>(f);\n\
    \  }\n  void _ifft(vector<mint>& f) const {\n    _fft_impl<false>(f);\n    mint\
    \ div_n = mint{1}/n_;\n    for (auto& x : f)\n      x *= div_n;\n  }\n\n};\n\n\
    constexpr NTT::int_type NTT::P;\n\nclass convolution {\n public:\n  using porinomial\
    \ = std::vector<NTT::mint>;\n private:\n  NTT ntt_;\n public:\n  explicit convolution(size_t\
    \ size) : ntt_(size) {}\n\n  porinomial operator()(const porinomial& g, const\
    \ porinomial& h) const {\n    auto fg = ntt_.fft(g);\n    auto fh = ntt_.fft(h);\n\
    \    for (size_t i = 0; i < ntt_.n(); i++) {\n      fg[i] *= fh[i];\n    }\n \
    \   ntt_.ifft_inline(fg);\n    return fg;\n  }\n\n};\n\nint main() {\n  cin.tie(nullptr);\
    \ ios::sync_with_stdio(false);\n\n\n\n  return 0;\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: test/sample.cpp
  requiredBy: []
  timestamp: '2022-11-27 14:51:20+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/sample.cpp
layout: document
redirect_from:
- /library/test/sample.cpp
- /library/test/sample.cpp.html
title: test/sample.cpp
---
