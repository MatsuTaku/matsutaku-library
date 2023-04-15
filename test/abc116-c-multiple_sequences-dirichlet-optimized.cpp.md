---
data:
  _extendedDependsOn:
  - icon: ':warning:'
    path: include/mtl/dirichlet.hpp
    title: include/mtl/dirichlet.hpp
  - icon: ':warning:'
    path: include/mtl/enumerate.hpp
    title: include/mtl/enumerate.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/modular.hpp
    title: include/mtl/modular.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links:
    - https://atcoder.jp/contests/arc116/tasks/arc116_c
  bundledCode: "#line 1 \"test/abc116-c-multiple_sequences-dirichlet-optimized.cpp\"\
    \n#define PROBLEM \"https://atcoder.jp/contests/arc116/tasks/arc116_c\"\r\n#line\
    \ 2 \"include/mtl/dirichlet.hpp\"\n#include <vector>\n#include <cmath>\n#include\
    \ <cassert>\n\nstd::vector<int> EratosthenesSieve(const int n) {\n  std::vector<int>\
    \ p(n+1);\n  if (n == 0)\n    return p;\n  p[1] = 1;\n  for (int i = 2; i <= n;\
    \ i++) {\n    if (p[i] == 0) {\n      p[i] = i;\n      for (int j = i*2; j <=\
    \ n; j += i) {\n        if (p[j] == 0)\n          p[j] = i;\n      }\n    }\n\
    \  }\n  return p;\n}\n\nstd::vector<int> Primes(const int n) {\n  std::vector<int>\
    \ ps;\n  auto era = EratosthenesSieve(n);\n  for (int i = 2; i <= n; i++) {\n\
    \    if (era[i] == i) {\n      ps.push_back(i);\n    }\n  }\n  return ps;\n}\n\
    \n/* PseudoCode:\n *   D_c(s) = sum c(n) n^{-s} = sum_n sum_{ij=n} a(i)b(j) (ij)^{-s}\n\
    \ * complexity: O(n log n)\n */\ntemplate<typename T>\nstd::vector<T> DirichletConvolution(const\
    \ std::vector<T>& a, const std::vector<T>& b) {\n  int n = (a.size()-1);\n  std::vector<T>\
    \ c(n+1);\n  for (int i = 1; i <= n; i++) {\n    int m = n / i;\n    for (int\
    \ j = 1; j <= m; j++)\n      c[i * j] += a[i] * b[j];\n  }\n  return c;\n}\n\n\
    /* PseudoCode:\n *   for p in primes:\n *     D_{a,p}(s) = sum_k a(p^k) p^{-ks}\
    \ (p-part of D_a)\n *     D_b(s) <- D_b(s) D_{a,p}(s)\n * requirements:\n *  \
    \ - Sequence a should be multinomial.\n *     D_a(s) = prod_p sum_k a(p^k) p^{-ks}\n\
    \ * complexity: O(n log log n)\n */\ntemplate<typename T>\nstd::vector<T> DirichletMultinomialConvolution(const\
    \ std::vector<T>& a, const std::vector<T>& b) {\n  int n = (a.size()-1);\n  auto\
    \ c = b;\n  c.resize(n+1);\n  for (int p : Primes(n)) {\n    int m = n/p;\n  \
    \  for (int i = m; i >= 1; i--) {\n      int u = p * i;\n      int q = p, j =\
    \ i;\n      while (true) {\n        c[u] += a[q] * c[j];\n        if (j % p !=\
    \ 0)\n          break;\n        q *= p;\n        j /= p;\n      }\n    }\n  }\n\
    \  return c;\n}\n\ntemplate<typename T>\nstd::pair<std::vector<T>, std::vector<T>>\
    \ Identity(int n) {\n  int k = std::pow(n, (double) 2 / 3);\n  int l = (n + k\
    \ - 1) / k;\n  std::vector<T> a(k+1, 0), A(l+1, 1);\n  a[1] = 1;\n  A[0] = 0;\n\
    \  return make_pair(a, A);\n}\n\ntemplate<typename T>\nstd::pair<std::vector<T>,\
    \ std::vector<T>> Zeta(int n) {\n  int k = std::pow(n, (double) 2 / 3);\n  int\
    \ l = (n + k - 1) / k;\n  std::vector<T> a(k+1, 1), A(l+1);\n  a[0] = 0;\n  for\
    \ (int i = 1; i <= l; i++)\n    A[i] = n / i;\n  return make_pair(a, A);\n}\n\n\
    template<typename T>\nstd::pair<std::vector<T>, std::vector<T>> DirichletConvolveOptimal(int\
    \ N, const std::pair<std::vector<T>, std::vector<T>>& _a, const std::pair<std::vector<T>,\
    \ std::vector<T>>& _b) {\n  const auto &a = _a.first, &A = _a.second, &b = _b.first,\
    \ &B = _b.second;\n  int k = a.size()-1, l = A.size()-1;\n  assert(k * l >= N);\n\
    \  auto Alow = a;\n  auto Blow = b;\n  for (int i = 1; i <= k; i++)\n    Alow[i]\
    \ += Alow[i-1];\n  auto getA = [&](int i) {\n    return i <= k ? Alow[i] : A[N\
    \ / i];\n  };\n  for (int i = 1; i <= k; i++)\n    Blow[i] += Blow[i-1];\n  auto\
    \ getB = [&](int i) {\n    return i <= k ? Blow[i] : B[N / i];\n  };\n\n  auto\
    \ c = DirichletConvolution(a, b);\n\n  std::vector<T> C(l+1);\n  for (int j =\
    \ 1; j <= l; j++) {\n    int n = N / j;\n    int m = sqrt(n);\n    for (int i\
    \ = 1; i <= m; i++) {\n      C[j] += a[i] * getB(n / i);\n      C[j] += (getA(n\
    \ / i) - getA(m)) * b[i];\n    }\n  }\n  return std::make_pair(c, C);\n}\n\ntemplate<typename\
    \ T>\nstd::pair<std::vector<T>, std::vector<T>> DirichletConvolveZeta(int N, const\
    \ std::vector<int>& primes, const std::pair<std::vector<T>, std::vector<T>>& _a)\
    \ {\n  const auto &a = _a.first, &A = _a.second;\n  int k = a.size()-1, l = A.size()-1;\n\
    \  auto Alow = a;\n  for (int i = 1; i <= k; i++)\n    Alow[i] += Alow[i-1];\n\
    \  auto getA = [&](int i) {\n    return i <= k ? Alow[i] : A[N / i];\n  };\n\n\
    \  auto c = a;\n  for (int p : primes) {\n    int m = k / p;\n    for (int i =\
    \ 1; i <= m; i++) {\n      c[p * i] += c[i];\n    }\n  }\n\n  std::vector<T> C(l+1);\n\
    \  for (int j = 1; j <= l; j++) {\n    int n = N / j;\n    int m = std::sqrt(n);\n\
    \    for (int i = 1; i <= m; i++) {\n      C[j] += a[i] * (n / i);\n      C[j]\
    \ += getA(n / i) - getA(m);\n    }\n  }\n  return std::make_pair(c, C);\n}\n#line\
    \ 2 \"include/mtl/modular.hpp\"\n#include <iostream>\n#line 4 \"include/mtl/modular.hpp\"\
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
    \n}\n#line 2 \"include/mtl/enumerate.hpp\"\n\n#line 5 \"include/mtl/enumerate.hpp\"\
    \n\ntemplate <typename MODULAR>\nclass Enumerate {\n public:\n  using mint = MODULAR;\n\
    \ private:\n  int max_n_ = 1;\n  std::vector<mint> fact_, ifact_;\n\n  void _set_max_n(int\
    \ n);\n\n public:\n  Enumerate() : fact_({1, 1}), ifact_({1, 1}) {}\n  explicit\
    \ Enumerate(int n) : fact_(std::max(2, n+1)), ifact_(std::max(2, n+1)) {\n   \
    \ fact_[0] = fact_[1] = ifact_[0] = ifact_[1] = 1;\n    _set_max_n(n);\n  }\n\n\
    \  mint cmb(int p, int q) {\n    if (p < q) return 0;\n    return fact(p) * ifact(q)\
    \ * ifact(p-q);\n  }\n\n  mint prm(int p, int q) {\n    if (p < q) return 0;\n\
    \    return fact(p) * ifact(p-q);\n  }\n\n  mint fact(int p) {\n    if (p > max_n_)\n\
    \      _set_max_n(p);\n    return fact_[p];\n  }\n  mint ifact(int p) {\n    if\
    \ (p > max_n_)\n      _set_max_n(p);\n    return ifact_[p];\n  }\n\n};\n\ntemplate<typename\
    \ MODULAR>\nvoid Enumerate<MODULAR>::_set_max_n(int n) {\n  if (n <= max_n_)\n\
    \    return;\n  int nxtn = std::max(max_n_*2, n);\n  fact_.resize(nxtn+1);\n \
    \ ifact_.resize(nxtn+1);\n  for (int i = max_n_+1; i <= nxtn; i++) {\n    fact_[i]\
    \ = fact_[i-1] * i;\n  }\n  ifact_[nxtn] = mint(1) / fact_[nxtn];\n  for (int\
    \ i = nxtn-1; i > max_n_; i--) {\n    ifact_[i] = ifact_[i+1] * (i+1);\n  }\n\
    \  max_n_ = nxtn;\n}\n\n#line 5 \"test/abc116-c-multiple_sequences-dirichlet-optimized.cpp\"\
    \n#include <bits/stdc++.h>\r\nusing namespace std;\r\nusing mint = Modular998244353;\r\
    \n\r\nint main() {\r\n  int n,m; cin>>n>>m;\r\n  constexpr int max_logn = 30;\r\
    \n  auto primes = Primes(m);\r\n  // Sum of zeta^k for m\r\n  auto f = Identity<mint>(m);\r\
    \n  vector<mint> A(max_logn);\r\n  for (int k = 0; k < max_logn; k++) {\r\n  \
    \  A[k] = f.second[1];\r\n    f = DirichletConvolveZeta(m, primes, f);\r\n  }\r\
    \n  // Sum of (zeta-1)^k for m\r\n  vector<mint> B(max_logn);\r\n  Enumerate<mint>\
    \ enm;\r\n  for (int k = 0; k < max_logn; k++) {\r\n    for (int i = 0; i <= k;\
    \ i++) {\r\n      auto coef = enm.cmb(k, i);\r\n      if ((k-i)%2) coef = -coef;\r\
    \n      B[k] += A[i] * coef;\r\n    }\r\n  }\r\n  // ans = sum_k B[k] binom(n,\
    \ k)\r\n  mint binom = 1;\r\n  mint ans = 0;\r\n  for (int k = 0; k < max_logn;\
    \ k++) {\r\n    ans += binom * B[k];\r\n    binom *= mint(n-k) * mint(k+1).inv();\r\
    \n  }\r\n  cout << ans << endl;\r\n}\n"
  code: "#define PROBLEM \"https://atcoder.jp/contests/arc116/tasks/arc116_c\"\r\n\
    #include \"../include/mtl/dirichlet.hpp\"\r\n#include \"../include/mtl/modular.hpp\"\
    \r\n#include \"../include/mtl/enumerate.hpp\"\r\n#include <bits/stdc++.h>\r\n\
    using namespace std;\r\nusing mint = Modular998244353;\r\n\r\nint main() {\r\n\
    \  int n,m; cin>>n>>m;\r\n  constexpr int max_logn = 30;\r\n  auto primes = Primes(m);\r\
    \n  // Sum of zeta^k for m\r\n  auto f = Identity<mint>(m);\r\n  vector<mint>\
    \ A(max_logn);\r\n  for (int k = 0; k < max_logn; k++) {\r\n    A[k] = f.second[1];\r\
    \n    f = DirichletConvolveZeta(m, primes, f);\r\n  }\r\n  // Sum of (zeta-1)^k\
    \ for m\r\n  vector<mint> B(max_logn);\r\n  Enumerate<mint> enm;\r\n  for (int\
    \ k = 0; k < max_logn; k++) {\r\n    for (int i = 0; i <= k; i++) {\r\n      auto\
    \ coef = enm.cmb(k, i);\r\n      if ((k-i)%2) coef = -coef;\r\n      B[k] += A[i]\
    \ * coef;\r\n    }\r\n  }\r\n  // ans = sum_k B[k] binom(n, k)\r\n  mint binom\
    \ = 1;\r\n  mint ans = 0;\r\n  for (int k = 0; k < max_logn; k++) {\r\n    ans\
    \ += binom * B[k];\r\n    binom *= mint(n-k) * mint(k+1).inv();\r\n  }\r\n  cout\
    \ << ans << endl;\r\n}"
  dependsOn:
  - include/mtl/dirichlet.hpp
  - include/mtl/modular.hpp
  - include/mtl/enumerate.hpp
  isVerificationFile: false
  path: test/abc116-c-multiple_sequences-dirichlet-optimized.cpp
  requiredBy: []
  timestamp: '2023-01-28 19:00:09+00:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/abc116-c-multiple_sequences-dirichlet-optimized.cpp
layout: document
redirect_from:
- /library/test/abc116-c-multiple_sequences-dirichlet-optimized.cpp
- /library/test/abc116-c-multiple_sequences-dirichlet-optimized.cpp.html
title: test/abc116-c-multiple_sequences-dirichlet-optimized.cpp
---
