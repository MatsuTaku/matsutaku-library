---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':question:'
    path: include/mtl/convolution.hpp
    title: include/mtl/convolution.hpp
  - icon: ':question:'
    path: include/mtl/fft.hpp
    title: include/mtl/fft.hpp
  - icon: ':question:'
    path: include/mtl/modular.hpp
    title: include/mtl/modular.hpp
  - icon: ':question:'
    path: include/mtl/ntt.hpp
    title: include/mtl/ntt.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/convolution_mod_1000000007
    links:
    - https://judge.yosupo.jp/problem/convolution_mod_1000000007
  bundledCode: "#line 1 \"test/yosupo/convolution_garner.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.yosupo.jp/problem/convolution_mod_1000000007\"\r\n#line 2 \"\
    include/mtl/fft.hpp\"\n#include <vector>\r\n#include <complex>\r\n\r\nclass Fft\
    \ {\r\n public:\r\n  using complex_t = std::complex<double>;\r\n  using value_type\
    \ = complex_t;\r\n private:\r\n  size_t n_;\r\n\r\n public:\r\n  explicit Fft(size_t\
    \ size) {\r\n    n_ = 1;\r\n    while (n_ < size)\r\n      n_ *= 2;\r\n  };\r\n\
    \r\n  size_t n() const { return n_; }\r\n\r\n  void fft_inline(std::vector<complex_t>&\
    \ f) const {\r\n    _fft(f);\r\n  }\r\n\r\n  std::vector<complex_t> fft(const\
    \ std::vector<complex_t>& f) const {\r\n    auto ff = f;\r\n    ff.resize(n_,\
    \ 0);\r\n    _fft(ff);\r\n    return ff;\r\n  }\r\n\r\n  void ifft_inline(std::vector<complex_t>&\
    \ f) const {\r\n    _ifft(f);\r\n  }\r\n\r\n  std::vector<complex_t> ifft(const\
    \ std::vector<complex_t>& f) const {\r\n    auto ff = f;\r\n    ff.resize(n_,\
    \ 0);\r\n    _ifft(ff);\r\n    return ff;\r\n  }\r\n\r\n private:\r\n  template\
    \ <bool Forward>\r\n  void _fft_impl(std::vector<complex_t>& f) const {\r\n  \
    \  // iterative bit reversal\r\n    for (size_t i = 0, j = 1; j < n_-1; j++) {\r\
    \n      for (size_t k = n_ >> 1; k > (i ^= k); k >>= 1);\r\n      if (i < j) std::swap(f[i],\
    \ f[j]);\r\n    }\r\n    // Cooley-Tukey FFT\r\n    for (size_t m = 1; m < n_;\
    \ m *= 2) {\r\n      double theta0 = Forward ? M_PI/m : -M_PI/m;\r\n      for\
    \ (size_t chunk = 0; chunk < n_; chunk += 2*m) {\r\n        for (size_t i = 0;\
    \ i < m; i++) {\r\n          complex_t w = {cos(theta0*i), -sin(theta0*i)};\r\n\
    \          auto p = chunk + i;\r\n          auto a = f[p + 0];\r\n          auto\
    \ b = f[p + m] * w;\r\n          f[p + 0] = a + b;\r\n          f[p + m] = a -\
    \ b;\r\n        }\r\n      }\r\n    }\r\n  }\r\n\r\n  void _fft(std::vector<complex_t>&\
    \ f) const {\r\n    _fft_impl<true>(f);\r\n  }\r\n  void _ifft(std::vector<complex_t>&\
    \ f) const {\r\n    _fft_impl<false>(f);\r\n    for (auto& x : f) {\r\n      x\
    \ /= n_;\r\n    }\r\n  }\r\n\r\n};\r\n\r\n#line 2 \"include/mtl/bit_manip.hpp\"\
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
    \ 1);\n  return x;\n}\n\n} // namespace bm\n#line 3 \"include/mtl/modular.hpp\"\
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
    template<class T>\r\nvoid _iterative_bit_reverse(std::vector<T>& f) {\r\n  int\
    \ log_n = math::ceil_pow2(f.size());\r\n  int n = 1 << log_n;\r\n  for (int i\
    \ = 0, j = 1; j < n-1; j++) {\r\n    for (int k = n >> 1; k > (i ^= k); k >>=\
    \ 1);\r\n    if (i < j) std::swap(f[i], f[j]);\r\n  }\r\n}\r\n\r\ntemplate <bool\
    \ Forward, bool BitReverse, int mod>\r\nvoid _fft_impl(std::vector<Modular<mod>>&\
    \ f) {\r\n  using mint = Modular<mod>;\r\n  int log_n = math::ceil_pow2(f.size());\r\
    \n  using info = ntt_info<mod>;\r\n  assert(info::log_n() >= log_n);\r\n  int\
    \ n = 1 << log_n;\r\n  f.resize(n, 0);\r\n  if constexpr (!Forward and BitReverse)\r\
    \n    _iterative_bit_reverse(f);\r\n  // Cooley-Tukey FFT\r\n  static constexpr\
    \ auto coeff = info::coeff(Forward);\r\n  if constexpr (Forward) {\r\n    for\
    \ (int log_m = log_n-1; log_m >= 0; log_m -= 2) {\r\n      int m = 1<<log_m;\r\
    \n      mint w0 = coeff[log_m];\r\n      if (log_m == 0) {\r\n        for (int\
    \ chunk = 0; chunk < n; chunk += 2*m) {\r\n          mint w = 1;\r\n         \
    \ for (int i = 0; i < m; i++) {\r\n            auto p = chunk + i;\r\n       \
    \     auto a = f[p + 0];\r\n            auto b = f[p + m];\r\n            f[p\
    \ + 0] = (a + b);\r\n            f[p + m] = (a - b) * w;\r\n            w *= w0;\r\
    \n          }\r\n        }\r\n      } else { // 4-base\r\n        auto w1 = coeff[log_m-1];\r\
    \n        auto wh = w0.pow(m/2).val();\r\n        for (int chunk = 0; chunk <\
    \ n; chunk += 2*m) {\r\n          mint w = 1, x = 1;\r\n          for (int i =\
    \ 0; i < m/2; i++) {\r\n            auto p = chunk + i;\r\n            auto ia\
    \ = p + 0*m/2;\r\n            auto ib = p + 1*m/2;\r\n            auto ic = p\
    \ + 2*m/2;\r\n            auto id = p + 3*m/2;\r\n            long long a = f[ia].val(),\
    \ b = f[ib].val(), c = f[ic].val(), d = f[id].val();\r\n            auto s = a\
    \ + c, t = b + d, u = (a - c + mod) * w.val() % mod, v = (b - d + mod) * w.val()\
    \ % mod * wh % mod;\r\n            f[ia] = (s + t);\r\n            f[ib] = (s\
    \ - t) * x.val();\r\n            f[ic] = (u + v);\r\n            f[id] = (u -\
    \ v) * x.val();\r\n            w *= w0;\r\n            x *= w1;\r\n          }\r\
    \n        }\r\n      }\r\n    }\r\n  } else {\r\n    int _log_m = 0;\r\n    if\
    \ (log_n % 2 == 1) {\r\n      for (int chunk = 0; chunk < n; chunk += 2) {\r\n\
    \        auto p = chunk;\r\n        auto a = f[p + 0];\r\n        auto b = f[p\
    \ + 1];\r\n        f[p + 0] = a + b;\r\n        f[p + 1] = a - b;\r\n      }\r\
    \n      _log_m = 1;\r\n    }\r\n    // 4-base\r\n    for (int log_m = _log_m;\
    \ log_m < log_n; log_m += 2) {\r\n      int m = 1<<(log_m+1);\r\n      mint w0\
    \ = coeff[log_m];\r\n      auto w1 = coeff[log_m+1];\r\n      auto wh = w1.pow(m/2).val();\r\
    \n      for (int chunk = 0; chunk < n; chunk += 2*m) {\r\n        mint w = 1,\
    \ x = 1;\r\n        for (int i = 0; i < m/2; i++) {\r\n          auto p = chunk\
    \ + i;\r\n          auto ia = p + 0*m/2;\r\n          auto ib = p + 1*m/2;\r\n\
    \          auto ic = p + 2*m/2;\r\n          auto id = p + 3*m/2;\r\n        \
    \  long long a = f[ia].val(), b = (long long)f[ib].val() * w.val() % mod, \r\n\
    \                    c = f[ic].val(), d = (long long)f[id].val() * w.val() % mod;\r\
    \n          auto s = a + b, t = a - b + mod, u = (c + d) * x.val() % mod, v =\
    \ (c - d + mod) * x.val() % mod * wh % mod;\r\n          f[ia] = s + u;\r\n  \
    \        f[ib] = t + v;\r\n          f[ic] = s - u; \r\n          f[id] = t -\
    \ v;\r\n          w *= w0;\r\n          x *= w1;\r\n        }\r\n      }\r\n \
    \   }\r\n  }\r\n  if constexpr (Forward and BitReverse)\r\n    _iterative_bit_reverse(f);\r\
    \n  if constexpr (!Forward) {\r\n    mint inv = mint(n).inv();\r\n    for (int\
    \ i = 0; i < n; i++) f[i] *= inv;\r\n  }\r\n}\r\ntemplate <int mod>\r\nvoid _fft(std::vector<Modular<mod>>&\
    \ f) { \r\n  _fft_impl<true, true>(f); \r\n}\r\ntemplate <int mod>\r\nvoid _ifft(std::vector<Modular<mod>>&\
    \ f) { \r\n  _fft_impl<false, true>(f); \r\n}\r\ntemplate <int mod>\r\nvoid _convolution_fft(std::vector<Modular<mod>>&\
    \ f) { \r\n  _fft_impl<true, false>(f); \r\n}\r\ntemplate <int mod>\r\nvoid _convolution_ifft(std::vector<Modular<mod>>&\
    \ f) { \r\n  _fft_impl<false, false>(f); \r\n}\r\n\r\n} // namespace _ntt\r\n\r\
    \ntemplate<class mint>\r\nvoid ntt_inline(std::vector<mint>& f) {\r\n  _ntt::_fft(f);\r\
    \n}\r\ntemplate<class mint>\r\nstd::vector<mint> ntt(std::vector<mint> f) {\r\n\
    \  _ntt::_fft(f);\r\n  return f;\r\n}\r\ntemplate<class mint>\r\nvoid intt_inline(std::vector<mint>&\
    \ f) {\r\n  _ntt::_ifft(f);\r\n}\r\ntemplate<class mint>\r\nstd::vector<mint>\
    \ intt(std::vector<mint> f) {\r\n  _ntt::_ifft(f);\r\n  return f;\r\n}\r\n\r\n\
    template<class mint>\r\nvoid ntt_convolution_inline(std::vector<mint>& f) {\r\n\
    \  _ntt::_convolution_fft(f);\r\n}\r\ntemplate<class mint>\r\nstd::vector<mint>\
    \ ntt_convolution(std::vector<mint> f) {\r\n  _ntt::_convolution_fft(f);\r\n \
    \ return f;\r\n}\r\ntemplate<class mint>\r\nvoid intt_convolution_inline(std::vector<mint>&\
    \ f) {\r\n  _ntt::_convolution_ifft(f);\r\n}\r\ntemplate<class mint>\r\nstd::vector<mint>\
    \ intt_convolution(std::vector<mint> f) {\r\n  _ntt::_convolution_ifft(f);\r\n\
    \  return f;\r\n}\r\n#line 4 \"include/mtl/convolution.hpp\"\n\nconst int CONVOLUTION_NAIVE_THRESHOLD\
    \ = 60;\n\ntemplate<class T>\nstd::vector<T> convolution_naive(const std::vector<T>&\
    \ f, const std::vector<T>& g) {\n  auto n = f.size(), m = g.size();\n  if (n ==\
    \ 0 or m == 0)\n    return {};\n  std::vector<T> h(n+m-1);\n  if (n < m) {\n \
    \   for (size_t j = 0; j < m; j++)\n      for (size_t i = 0; i < n; i++)\n   \
    \     h[i+j] += f[i] * g[j];\n  } else {\n    for (size_t i = 0; i < n; i++)\n\
    \      for (size_t j = 0; j < m; j++)\n        h[i+j] += f[i] * g[j];\n  }\n \
    \ return h;\n}\n\ntemplate<class T>\nstd::vector<T> convolution_fft(std::vector<T>\
    \ f, std::vector<T> g) {\n  auto n = f.size(), m = g.size();\n  size_t len = n\
    \ + m - 1;\n  size_t l = 1 << math::ceil_pow2(len);\n  if (n+m-3 < l/2) {\n  \
    \  auto fb = f.back(), gb = g.back();\n    f.pop_back(); g.pop_back();\n    std::vector<T>\
    \ h(n+m-1, 0);\n    h[n+m-2] = fb * gb;\n    for (size_t i = 0; i < n; i++)\n\
    \      h[i + m-1] += f[i] * gb;\n    for (size_t i = 0; i < m; i++)\n      h[i\
    \ + n-1] += g[i] * fb;\n    auto fg = convolution(std::move(f), std::move(g));\n\
    \    for (size_t i = 0; i < n+m-3; i++)\n      h[i] += fg[i];\n    return h;\n\
    \  }\n  Fft fft(len);\n  f.resize(fft.n(), 0);\n  g.resize(fft.n(), 0);\n  auto\
    \ same = f == g;\n  fft.fft_inline(f);\n  if (same) {\n    for (size_t i = 0;\
    \ i < fft.n(); i++)\n      f[i] *= f[i];\n  } else {\n    fft.fft_inline(g);\n\
    \    for (size_t i = 0; i < fft.n(); i++)\n      f[i] *= g[i];\n  }\n  fft.ifft_inline(f);\n\
    \  std::vector<T> res(len);\n  for (size_t i = 0; i < len; i++)\n    res[i] =\
    \ f[i].real();\n  return res;\n}\n\ntemplate<int Mod>\nstd::vector<Modular<Mod>>\
    \ convolution_ntt(std::vector<Modular<Mod>> f, std::vector<Modular<Mod>> g) {\n\
    \  auto n = f.size(), m = g.size();\n  size_t l = 1 << math::ceil_pow2(n + m -\
    \ 1);\n  if (n+m-3 < l/2) {\n    auto fb = f.back(), gb = g.back();\n    f.pop_back();\
    \ g.pop_back();\n    std::vector<Modular<Mod>> h(n+m-1, 0);\n    h[n+m-2] = fb\
    \ * gb;\n    for (size_t i = 0; i < n-1; i++)\n      h[i + m-1] += f[i] * gb;\n\
    \    for (size_t i = 0; i < m-1; i++)\n      h[i + n-1] += g[i] * fb;\n    auto\
    \ fg = convolution(std::move(f), std::move(g));\n    for (size_t i = 0; i < fg.size();\
    \ i++)\n      h[i] += fg[i];\n    return h;\n  }\n  f.resize(l, 0);\n  g.resize(l,\
    \ 0);\n  bool same = f == g;\n  ntt_convolution_inline(f);\n  if (same) {\n  \
    \  for (size_t i = 0; i < l; i++) \n      f[i] *= f[i];\n  } else {\n    ntt_convolution_inline(g);\n\
    \    for (size_t i = 0; i < l; i++) \n      f[i] *= g[i];\n  }\n  intt_convolution_inline(f);\n\
    \  return f;\n}\n\ntemplate<int Mod>\nstd::vector<Modular<Mod>> convolution_fft(std::vector<Modular<Mod>>\
    \ f, std::vector<Modular<Mod>> g) {\n  return convolution_ntt(std::move(f), std::move(g));\n\
    }\n\nstd::vector<int> convolution_fft(const std::vector<int>& a, const std::vector<int>&\
    \ b) {\n  auto n = a.size(), m = b.size();\n  std::vector<Modular998244353> f(n),\
    \ g(m);\n  for (size_t i = 0; i < n; i++)\n    f[i] = a[i];\n  for (size_t i =\
    \ 0; i < m; i++)\n    g[i] = b[i];\n  auto h = convolution_fft(std::move(f), std::move(g));\n\
    \  std::vector<int> res(n+m-1);\n  for (size_t i = 0; i < n+m-1; i++)\n    res[i]\
    \ = h[i].val();\n  return res;\n}\n\ntemplate<class T>\nstd::vector<T> convolution(const\
    \ std::vector<T>& f, const std::vector<T>& g) {\n  auto n = f.size(), m = g.size();\n\
    \  if (n == 0 or m == 0)\n    return {};\n  if (std::min(n, m) <= CONVOLUTION_NAIVE_THRESHOLD)\n\
    \    return convolution_naive(f, g);\n  return convolution_fft(f, g);\n}\n\ntemplate<class\
    \ T>\nstd::vector<T> convolution(std::vector<T>&& f, std::vector<T>&& g) {\n \
    \ auto n = f.size(), m = g.size();\n  if (n == 0 or m == 0)\n    return {};\n\
    \  if (std::min(n, m) <= CONVOLUTION_NAIVE_THRESHOLD)\n    return convolution_naive(f,\
    \ g);\n  return convolution_fft(std::move(f), std::move(g));\n}\n\ntemplate<class\
    \ mint>\nstd::vector<mint> convolution_garner(const std::vector<mint>& f, \n \
    \                                    const std::vector<mint>& g) {\n  auto n =\
    \ f.size();\n  auto m = g.size();\n  if (n == 0 or m == 0)\n    return {};\n \
    \ constexpr long long nttprimes[] = {754974721, 167772161, 469762049};\n  using\
    \ mint0 = Modular<754974721>;\n  using mint1 = Modular<167772161>;\n  using mint2\
    \ = Modular<469762049>;\n  std::vector<mint0> a0(n), b0(m);\n  std::vector<mint1>\
    \ a1(n), b1(m);\n  std::vector<mint2> a2(n), b2(m);\n  for(size_t i = 0; i < n;\
    \ i++) a0[i] = f[i].val(), a1[i] = f[i].val(), a2[i] = f[i].val();\n  for(size_t\
    \ i = 0; i < m; i++) b0[i] = g[i].val(), b1[i] = g[i].val(), b2[i] = g[i].val();\n\
    \  auto c0 = convolution_ntt(a0, b0);\n  auto c1 = convolution_ntt(a1, b1);\n\
    \  auto c2 = convolution_ntt(a2, b2);\n  constexpr long long m01 = (long long)\
    \ nttprimes[0] * nttprimes[1];\n  constexpr long long m0_inv_m1 = mint1(nttprimes[0]).inv().val();\n\
    \  constexpr long long m01_inv_m2 = mint2(m01).inv().val();\n  const int mod =\
    \ mint::mod();\n  auto garner = [&](mint0 x0, mint1 x1, mint2 x2) -> mint {\n\
    \    int r0 = x0.val(), r1 = x1.val(), r2 = x2.val();\n    int v1 = (m0_inv_m1\
    \ * (r1 + nttprimes[1] - r0)) % nttprimes[1];\n    auto v2 = (mint2(r2) - r0 -\
    \ mint2(nttprimes[0]) * v1) * mint2(m01_inv_m2);\n    return mint(r0 + 1LL * nttprimes[0]\
    \ * v1 + m01 % mod * v2.val());\n  };\n  std::vector<mint> c(c0.size());\n  for(size_t\
    \ i = 0; i < c.size(); i++) c[i] = garner(c0[i], c1[i], c2[i]);\n  return c;\n\
    }\n\ntemplate<int Mod>\nstd::vector<Modular<Mod>> convolution_large(const std::vector<Modular<Mod>>&\
    \ f, \n                                            const std::vector<Modular<Mod>>&\
    \ g) {\n  auto n = f.size();\n  auto m = g.size();\n  if (n == 0 or m == 0)\n\
    \    return {};\n  constexpr auto h = bm::ctz(Mod-1);\n  if (n+m-1 < 1u<<h)\n\
    \    return convolution(f ,g);\n  size_t l = h-1;\n  int cs = ((n-1)>>l)+1;\n\
    \  int ds = ((m-1)>>l)+1;\n  using result_type = std::vector<Modular<Mod>>;\n\
    \  std::vector c(cs, result_type(2<<l));\n  std::vector d(ds, result_type(2<<l));\n\
    \  size_t mask = (1<<l)-1;\n  for (size_t i = 0; i < n; i++)\n    c[i>>l][i&mask]\
    \ = f[i];\n  for (size_t i = 0; i < m; i++)\n    d[i>>l][i&mask] = g[i];\n  for\
    \ (auto& v: c) ntt_convolution_inline(v);\n  for (auto& v: d) ntt_convolution_inline(v);\n\
    \  result_type res(n+m-1);\n  result_type e(2<<l);\n  for (int i = 0; i < cs+ds-1;\
    \ i++) {\n    e.assign(2<<l, 0);\n    for (int j = std::max(i-ds+1, 0); j < std::min(cs,\
    \ i+1); j++) {\n      int k = i-j;\n      assert(k >= 0 and k < ds);\n      for\
    \ (int x = 0; x < 2<<l; x++)\n        e[x] += c[j][x] * d[k][x];\n    }\n    intt_convolution_inline(e);\n\
    \    auto len = std::min((size_t)2<<l, n+m-1-(i<<l));\n    for (size_t x = 0;\
    \ x < len; x++)\n      res[(i<<l) + x] += e[x];\n  }\n  return res;\n}\n\nsize_t\
    \ complexity_of_convolution(size_t fsize, size_t gsize) {\n  int lg = math::ceil_pow2(fsize\
    \ + gsize - 1);\n  return lg << lg;\n}\n#line 3 \"test/yosupo/convolution_garner.test.cpp\"\
    \n#include <bits/stdc++.h>\r\n\r\nusing namespace std;\r\nusing ll = long long;\r\
    \n\r\nint main() {\r\n  cin.tie(nullptr); ios::sync_with_stdio(false);\r\n\r\n\
    \  int N,M; cin>>N>>M;\r\n  vector<Modular1000000007> A(N), B(M);\r\n  for (auto&\
    \ a : A) cin>>a;\r\n  for (auto& b : B) cin>>b;\r\n  auto C = convolution_garner(A,\
    \ B);\r\n  C.resize(N+M-1);\r\n  for (int i = 0; i < N+M-1; i++)\r\n    cout <<\
    \ C[i] << \" \";\r\n  cout << endl;\r\n\r\n  return 0;\r\n}\r\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/convolution_mod_1000000007\"\
    \r\n#include \"../../include/mtl/convolution.hpp\"\r\n#include <bits/stdc++.h>\r\
    \n\r\nusing namespace std;\r\nusing ll = long long;\r\n\r\nint main() {\r\n  cin.tie(nullptr);\
    \ ios::sync_with_stdio(false);\r\n\r\n  int N,M; cin>>N>>M;\r\n  vector<Modular1000000007>\
    \ A(N), B(M);\r\n  for (auto& a : A) cin>>a;\r\n  for (auto& b : B) cin>>b;\r\n\
    \  auto C = convolution_garner(A, B);\r\n  C.resize(N+M-1);\r\n  for (int i =\
    \ 0; i < N+M-1; i++)\r\n    cout << C[i] << \" \";\r\n  cout << endl;\r\n\r\n\
    \  return 0;\r\n}\r\n"
  dependsOn:
  - include/mtl/convolution.hpp
  - include/mtl/fft.hpp
  - include/mtl/ntt.hpp
  - include/mtl/modular.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: true
  path: test/yosupo/convolution_garner.test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/yosupo/convolution_garner.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/convolution_garner.test.cpp
- /verify/test/yosupo/convolution_garner.test.cpp.html
title: test/yosupo/convolution_garner.test.cpp
---
