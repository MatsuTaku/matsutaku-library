---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/fps.hpp
    title: include/mtl/fps.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/modular.hpp
    title: include/mtl/modular.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/ntt.hpp
    title: include/mtl/ntt.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/convolution_mod
    links:
    - https://judge.yosupo.jp/problem/convolution_mod
  bundledCode: "#line 1 \"test/convolution.test.cpp\"\n#define PROBLEM \"https://judge.yosupo.jp/problem/convolution_mod\"\
    \n\n#line 2 \"include/mtl/modular.hpp\"\n#include <iostream>\n\ntemplate <long\
    \ long MOD>\nclass Modular {\n private:\n  long long val_;\n\n public:\n  Modular()\
    \ : val_(0) {}\n  Modular(long long v) : val_(v%MOD) {\n    if (val_ < 0) val_\
    \ += MOD;\n  }\n\n  long long val() const { return val_; }\n  Modular& operator+=(Modular\
    \ x) {\n    val_ += x.val();\n    if (val_ >= MOD) val_ %= MOD;\n    return *this;\n\
    \  }\n  Modular operator-() const { return {MOD - val_}; }\n  Modular& operator-=(Modular\
    \ x) {\n    val_ -= x.val();\n    if (val_ < 0) val_ += MOD;\n    return *this;\n\
    \  }\n  Modular& operator*=(Modular x) {\n    val_ *= x.val();\n    if (val_ >=\
    \ MOD) val_ %= MOD;\n    return *this;\n  }\n  Modular pow(long long p) const\
    \ {\n    Modular t = 1;\n    Modular u = *this;\n    while (p) {\n      if (p\
    \ & 1)\n        t *= u;\n      u *= u;\n      p >>= 1;\n    }\n    return t;\n\
    \  }\n  friend Modular pow(Modular x, long long p) {\n    return x.pow(p);\n \
    \ }\n  Modular inv() const { return pow(MOD-2); }\n  Modular& operator/=(Modular\
    \ x) { return *this *= x.inv(); }\n  Modular operator+(Modular x) const { return\
    \ Modular(*this) += x; }\n  Modular operator-(Modular x) const { return Modular(*this)\
    \ -= x; }\n  Modular operator*(Modular x) const { return Modular(*this) *= x;\
    \ }\n  Modular operator/(Modular x) const { return Modular(*this) /= x; }\n  Modular&\
    \ operator++() { return *this += 1; }\n  Modular operator++(int) { Modular c =\
    \ *this; ++(*this); return c; }\n  Modular& operator--() { return *this -= 1;\
    \ }\n  Modular operator--(int) { Modular c = *this; --(*this); return c; }\n\n\
    \  bool operator==(Modular x) const { return val() == x.val(); }\n  bool operator!=(Modular\
    \ x) const { return val() != x.val(); }\n  bool operator<(Modular x) const { return\
    \ val() < x.val(); };\n  bool operator<=(Modular x) const { return val() <= x.val();\
    \ };\n  bool operator>(Modular x) const { return val() > x.val(); };\n  bool operator>=(Modular\
    \ x) const { return val() >= x.val(); };\n\n  friend std::ostream& operator<<(std::ostream&\
    \ os, const Modular& x) {\n    return os << x.val();\n  }\n  friend std::istream&\
    \ operator>>(std::istream& is, Modular& x) {\n    return is >> x.val_;\n  }\n\n\
    };\n\nusing Modular998244353 = Modular<998244353>;\nusing Modular1000000007 =\
    \ Modular<(int)1e9+7>;\n#line 2 \"include/mtl/bit_manip.hpp\"\n#include <cstdint>\n\
    #include <cassert>\n\nnamespace bm {\n\ninline constexpr uint64_t popcnt_e8(uint64_t\
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
    \ 4 \"include/mtl/ntt.hpp\"\n#include <vector>\r\n#line 6 \"include/mtl/ntt.hpp\"\
    \n\r\nnamespace _ntt {\r\n\r\nstatic constexpr int P = 998244353;\r\nstatic constexpr\
    \ int MAX_DIV = 23;\r\nusing mint = Modular<P>;\r\nstatic const mint PRIMITIVE_ROOT\
    \ = 3;\r\n\r\nbool _ntt_initialized = false;\r\nstd::vector<mint> _coeff;\r\n\
    std::vector<mint> _icoeff;\r\n\r\nvoid _ntt_initialize() {\r\n  if (_ntt_initialized)\r\
    \n    return;\r\n  _ntt_initialized = true;\r\n  int log_n_ = MAX_DIV;\r\n  mint\
    \ r_ = pow(PRIMITIVE_ROOT, (P-1) >> MAX_DIV);\r\n  _coeff.resize(log_n_);\r\n\
    \  _icoeff.resize(log_n_);\r\n  mint w = r_;\r\n  mint iw = w.inv();\r\n  for\
    \ (int i = log_n_-1; i >= 0; i--) {\r\n    _coeff[i] = iw;\r\n    iw *= iw;\r\n\
    \    _icoeff[i] = w;\r\n    w *= w;\r\n  }\r\n}\r\n\r\ntemplate <bool Forward>\r\
    \nvoid _fft_impl(std::vector<mint>& f) {\r\n  _ntt_initialize();\r\n  int log_n\
    \ = 64-bm::clz(f.size()-1);\r\n  int n = 1 << log_n;\r\n  f.resize(n, 0);\r\n\
    \  // iterative bit reversal\r\n  for (int i = 0, j = 1; j < n-1; j++) {\r\n \
    \   for (int k = n >> 1; k > (i ^= k); k >>= 1);\r\n    if (i < j) std::swap(f[i],\
    \ f[j]);\r\n  }\r\n  // Cooley-Tukey FFT\r\n  for (int log_m = 0; log_m < log_n;\
    \ log_m++) {\r\n    int m = 1<<log_m;\r\n    mint w0 = Forward ? _coeff[log_m]\
    \ : _icoeff[log_m];\r\n    for (int chunk = 0; chunk < n; chunk += 2*m) {\r\n\
    \      mint w = 1;\r\n      for (int i = 0; i < m; i++) {\r\n        auto p =\
    \ chunk + i;\r\n        auto a = f[p + 0];\r\n        auto b = f[p + m] * w;\r\
    \n        f[p + 0] = a + b;\r\n        f[p + m] = a - b;\r\n        w *= w0;\r\
    \n      }\r\n    }\r\n  }\r\n}\r\nvoid _fft(std::vector<mint>& f) { _fft_impl<true>(f);\
    \ }\r\nvoid _ifft(std::vector<mint>& f) { _fft_impl<false>(f); }\r\n\r\n}\r\n\r\
    \nstd::vector<_ntt::mint>& ntt_inline(std::vector<_ntt::mint>& f) {\r\n  _ntt::_fft(f);\r\
    \n  return f;\r\n}\r\n\r\nstd::vector<_ntt::mint> ntt(std::vector<_ntt::mint>\
    \ f) {\r\n  _ntt::_fft(f);\r\n  return f;\r\n}\r\n\r\nstd::vector<_ntt::mint>&\
    \ intt_inline(std::vector<_ntt::mint>& f) {\r\n  _ntt::_ifft(f);\r\n  return f;\r\
    \n}\r\n\r\nstd::vector<_ntt::mint> intt(std::vector<_ntt::mint> f) {\r\n  _ntt::_ifft(f);\r\
    \n  return f;\r\n}\r\n\r\nstd::vector<_ntt::mint> convolution(std::vector<_ntt::mint>\
    \ f, std::vector<_ntt::mint> g) {\r\n  int l = f.size() + g.size() - 1;\r\n  f.resize(l);\r\
    \n  ntt_inline(f);\r\n  g.resize(l);\r\n  ntt_inline(g);\r\n  int n = f.size();\r\
    \n  for (int i = 0; i < n; i++)\r\n    f[i] *= g[i];\r\n  intt_inline(f);\r\n\
    \  auto n_inv = _ntt::mint(n).inv();\r\n  for (int i = 0; i < n; i++)\r\n    f[i]\
    \ *= n_inv;\r\n  return f;\r\n}\r\n#line 4 \"include/mtl/fps.hpp\"\n#include <initializer_list>\n\
    \nclass Fps : public std::vector<_ntt::mint> {\n public:\n  using mint = _ntt::mint;\n\
    \  using f = std::vector<mint>;\n  explicit Fps(size_t n=0, mint v=0) : f(n, v)\
    \ {}\n  template<typename It> Fps(It begin, It end) : f(begin, end) {}\n  Fps(std::initializer_list<mint>\
    \ l) : f(l) {}\n  explicit Fps(const std::vector<mint>& l) : f(l) {}\n\n  Fps&\
    \ normalize() {\n    int n = (int) size();\n    while (n-1 >= 0 and (*this)[n-1]\
    \ == 0) n--;\n    resize(n);\n    return *this;\n  }\n  Fps& operator+=(mint x)\
    \ {\n    if (empty())\n      resize(1);\n    (*this)[0] += x;\n    return *this;\n\
    \  }\n  Fps& operator-=(mint x) { return *this += -x; }\n  Fps& operator*=(mint\
    \ x) {\n    for (int i = 0; i < (int) size(); i++)\n      (*this)[i] *= x;\n \
    \   return *this;\n  }\n  Fps& operator/=(mint x) { return *this *= x.inv(); }\n\
    \  Fps operator+(mint x) const { return Fps(*this) += x; }\n  Fps operator-(mint\
    \ x) const { return Fps(*this) -= x; }\n  Fps operator*(mint x) const { return\
    \ Fps(*this) *= x; }\n  Fps operator/(mint x) const { return Fps(*this) /= x;\
    \ }\n  Fps operator-() const {\n    auto g = *this;\n    for (auto& v : g) v =\
    \ -v;\n    return g;\n  }\n  Fps& operator+=(const Fps& r) {\n    if (r.size()\
    \ > size())\n      resize(r.size());\n    for (int i = 0; i < (int) r.size();\
    \ i++)\n      (*this)[i] += r[i];\n    return *this;\n  }\n  Fps& operator-=(const\
    \ Fps& r) {\n    if (r.size() > size())\n      resize(r.size());\n    for (int\
    \ i = 0; i < (int) r.size(); i++)\n      (*this)[i] -= r[i];\n    return *this;\n\
    \  }\n  Fps& dot(const Fps& r) {\n    if (size() > r.size())\n      resize(r.size());\n\
    \    for (int i = 0; i < (int) size(); i++)\n      (*this)[i] *= r[i];\n    return\
    \ *this;\n  }\n  Fps& operator*=(const Fps& r) {\n    int n = (int) (size() +\
    \ r.size()) - 1;\n    resize(n);\n    ntt_inline(*this);\n    Fps Fr = r;\n  \
    \  Fr.resize(n);\n    ntt_inline(Fr);\n    dot(Fr);\n    intt_inline(*this);\n\
    \    return *this /= (int) size();\n  }\n  Fps& mod(int k) {\n    resize(k);\n\
    \    return *this;\n  }\n  Fps inv(int n = -1) const {\n    if (n == -1)\n   \
    \   n = (int) size();\n    assert(!empty() and (*this)[0] != 0);\n    Fps g,fm;\n\
    \    g.reserve(n); fm.reserve(n);\n    g.push_back((*this)[0].inv());\n    fm.push_back((*this)[0]);\n\
    \    for (int m = 1; m < n; m <<= 1) {\n      int nm = std::min(m*2, n);\n   \
    \   for (int i = m; i < nm; i++)\n        fm.push_back(i < (int) size() ? (*this)[i]\
    \ : 0);\n      auto fgg = g * g * fm;\n      for (int i = m; i < nm; i++)\n  \
    \      g.push_back(-fgg[i]);\n    }\n    return g;\n  }\n  Fps& operator/=(const\
    \ Fps& r) {\n    return *this *= r.inv();\n  }\n  Fps& operator%=(const Fps& r)\
    \ {\n    normalize();\n    Fps q = *this / r;\n    return *this -= q * r;\n  }\n\
    \  Fps operator+(const Fps& r) const { return Fps(*this) += r; }\n  Fps operator-(const\
    \ Fps& r) const { return Fps(*this) -= r; }\n  Fps operator*(const Fps& r) const\
    \ { return Fps(*this) *= r; }\n  Fps operator/(const Fps& r) const { return Fps(*this)\
    \ /= r; }\n  Fps operator%(const Fps& r) const { return Fps(*this) %= r; }\n};\n\
    #line 4 \"test/convolution.test.cpp\"\n#include <bits/stdc++.h>\n\nusing namespace\
    \ std;\nusing ll = long long;\n\nint main() {\n  cin.tie(nullptr); ios::sync_with_stdio(false);\n\
    \n  int N,M; cin>>N>>M;\n  Fps A(N), B(M);\n  for (auto& a : A) cin>>a;\n  for\
    \ (auto& b : B) cin>>b;\n  auto C = A * B;\n  for (int i = 0; i < N+M-1; i++)\n\
    \    cout << C[i] << \" \";\n  cout << endl;\n\n  return 0;\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/convolution_mod\"\n\n#include\
    \ \"../include/mtl/fps.hpp\"\n#include <bits/stdc++.h>\n\nusing namespace std;\n\
    using ll = long long;\n\nint main() {\n  cin.tie(nullptr); ios::sync_with_stdio(false);\n\
    \n  int N,M; cin>>N>>M;\n  Fps A(N), B(M);\n  for (auto& a : A) cin>>a;\n  for\
    \ (auto& b : B) cin>>b;\n  auto C = A * B;\n  for (int i = 0; i < N+M-1; i++)\n\
    \    cout << C[i] << \" \";\n  cout << endl;\n\n  return 0;\n}\n"
  dependsOn:
  - include/mtl/fps.hpp
  - include/mtl/ntt.hpp
  - include/mtl/modular.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: true
  path: test/convolution.test.cpp
  requiredBy: []
  timestamp: '2022-12-18 04:26:00+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/convolution.test.cpp
layout: document
redirect_from:
- /verify/test/convolution.test.cpp
- /verify/test/convolution.test.cpp.html
title: test/convolution.test.cpp
---