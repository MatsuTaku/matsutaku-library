---
data:
  _extendedDependsOn:
  - icon: ':warning:'
    path: include/mtl/dirichlet.hpp
    title: include/mtl/dirichlet.hpp
  - icon: ':question:'
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
  bundledCode: "#line 1 \"test/abc116-c-multiple_sequences-dirichlet.cpp\"\n#define\
    \ PROBLEM \"https://atcoder.jp/contests/arc116/tasks/arc116_c\"\r\n#line 2 \"\
    include/mtl/dirichlet.hpp\"\n#include <vector>\n#include <cmath>\n#include <cassert>\n\
    \nstd::vector<int> EratosthenesSieve(const int n) {\n  std::vector<int> p(n+1);\n\
    \  if (n == 0)\n    return p;\n  p[1] = 1;\n  for (int i = 2; i <= n; i++) {\n\
    \    if (p[i] == 0) {\n      p[i] = i;\n      for (int j = i*2; j <= n; j += i)\
    \ {\n        if (p[j] == 0)\n          p[j] = i;\n      }\n    }\n  }\n  return\
    \ p;\n}\n\nstd::vector<int> Primes(const int n) {\n  std::vector<int> ps;\n  auto\
    \ era = EratosthenesSieve(n);\n  for (int i = 2; i <= n; i++) {\n    if (era[i]\
    \ == i) {\n      ps.push_back(i);\n    }\n  }\n  return ps;\n}\n\n/* PseudoCode:\n\
    \ *   D_c(s) = sum c(n) n^{-s} = sum_n sum_{ij=n} a(i)b(j) (ij)^{-s}\n * complexity:\
    \ O(n log n)\n */\ntemplate<typename T>\nstd::vector<T> DirichletConvolution(const\
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
    \ 2 \"include/mtl/modular.hpp\"\n#include <iostream>\n\ntemplate <long long MOD>\n\
    class Modular {\n private:\n  long long val_;\n\n public:\n  Modular() : val_(0)\
    \ {}\n  Modular(long long v) : val_(v%MOD) {\n    if (val_ < 0) val_ += MOD;\n\
    \  }\n\n  long long val() const { return val_; }\n  Modular& operator+=(Modular\
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
    \ Modular<(int)1e9+7>;\n#line 4 \"test/abc116-c-multiple_sequences-dirichlet.cpp\"\
    \n#include <bits/stdc++.h>\r\nusing namespace std;\r\nusing mint = Modular998244353;\r\
    \n\r\nint main() {\r\n  int n,m; cin>>n>>m;\r\n  auto z = Zeta<mint>(m);\r\n \
    \ auto t = Identity<mint>(m);\r\n  while (n) {\r\n    if (n&1) {\r\n      t =\
    \ DirichletConvolveOptimal(m, z, t);\r\n    }\r\n    z = DirichletConvolveOptimal(m,\
    \ z, z);\r\n    n >>= 1;\r\n  }\r\n  cout << t.second[1] << endl;\r\n}\n"
  code: "#define PROBLEM \"https://atcoder.jp/contests/arc116/tasks/arc116_c\"\r\n\
    #include \"../include/mtl/dirichlet.hpp\"\r\n#include \"../include/mtl/modular.hpp\"\
    \r\n#include <bits/stdc++.h>\r\nusing namespace std;\r\nusing mint = Modular998244353;\r\
    \n\r\nint main() {\r\n  int n,m; cin>>n>>m;\r\n  auto z = Zeta<mint>(m);\r\n \
    \ auto t = Identity<mint>(m);\r\n  while (n) {\r\n    if (n&1) {\r\n      t =\
    \ DirichletConvolveOptimal(m, z, t);\r\n    }\r\n    z = DirichletConvolveOptimal(m,\
    \ z, z);\r\n    n >>= 1;\r\n  }\r\n  cout << t.second[1] << endl;\r\n}"
  dependsOn:
  - include/mtl/dirichlet.hpp
  - include/mtl/modular.hpp
  isVerificationFile: false
  path: test/abc116-c-multiple_sequences-dirichlet.cpp
  requiredBy: []
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/abc116-c-multiple_sequences-dirichlet.cpp
layout: document
redirect_from:
- /library/test/abc116-c-multiple_sequences-dirichlet.cpp
- /library/test/abc116-c-multiple_sequences-dirichlet.cpp.html
title: test/abc116-c-multiple_sequences-dirichlet.cpp
---
