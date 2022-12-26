---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/arbitrary_mod_binomial.hpp\"\n#include <vector>\r\
    \n#include <array>\r\n#include <type_traits>\r\n\r\nnamespace math {\r\n\r\ntemplate<class\
    \ T>\r\nconstexpr bool is_coprime(T a, T b) {\r\n  using type = typename std::remove_const<typename\
    \ std::remove_reference<T>::type>::type;\r\n  type c = a;\r\n  type d = b;\r\n\
    \  while (d) {\r\n    std::swap(c,d);\r\n    d %= c;\r\n  }\r\n  return c == 1;\r\
    \n}\r\n\r\n} // namespace\r\n\r\n/* \\binom(N, m) mod M\r\n * @param N, M\r\n\
    \ * @complexity\r\n *   - preprocess: O(N \\log M / \\log\\log M)\r\n *   - query:\
    \      O(1)\r\n * @example\r\n *   int binom_N_m_mod_M = ArbitraryModBinomial(N,M)(m);\r\
    \n */\r\nstruct ArbitraryModBinomial {\r\n  static constexpr int MAX_PRIME_FACTOR_NUM\
    \ = 9;\r\n  using buffer_type = std::array<int, MAX_PRIME_FACTOR_NUM+1>;\r\n \
    \ bool enabled_;\r\n  int n_, mod_;\r\n  constexpr bool enabled() const { return\
    \ enabled_; }\r\n  constexpr int mod() { return mod_; }\r\n  std::vector<int>\
    \ st_;\r\n  constexpr ArbitraryModBinomial(int n, int mod)\r\n    : enabled_(math::is_coprime(n,\
    \ mod)), n_(n), mod_(mod), st_((n+1)/2) {\r\n    if (!enabled) return;\r\n   \
    \ std::vector<bool> isn_p(n+1);\r\n    buffer_type P{};\r\n    std::vector<int>\
    \ d(n+1), inv(n+1);\r\n    int k = 0;\r\n    for (int i = 2; i <= n; i++) {\r\n\
    \      if (isn_p[i]) continue;\r\n      for (int j = i+i; j <= n; j += i)\r\n\
    \        isn_p[j] = 1;\r\n      if (mod() % i) continue;\r\n      P[++k] = prime;\r\
    \n      for (int v = prime; v <= n; v += prime)\r\n        d[v] = k;\r\n    }\r\
    \n    using mint = Modular<mod()>;\r\n    int mid = (n+1)/2;\r\n    std::vector<int>\
    \ inv(mid+1);\r\n    inv[mid] = mint(mid).inv().val();\r\n    for (int i = mid-1;\
    \ i > 0; i--)\r\n      inv[i] = (long long) inv[i+1] * (i+1) % mod();\r\n    int\
    \ h = k+1;\r\n    int w = n/(P[1]-1) + 1;\r\n    std::vector<int> ptb(h * w);\r\
    \n    for (int i = 1; i <= MAX_PRIME_FACTOR_NUM; i++) {\r\n      int t = n / (P[i]-1)\
    \ + 1;\r\n      int v = 1;\r\n      ptb[i*h] = v;\r\n      for(int j = 1; j <\
    \ t; j++) {\r\n        v = v * P[i] % mod();\r\n        ptb[i*h+j] = v;\r\n  \
    \    }\r\n    }\r\n    int s = 1, ns = 1;\r\n    buffer_type t{}, nt{};\r\n  \
    \  S[0] = T[0] = 1;\r\n    /* important equation\r\n     * \\binom(n,k) = (n-k+1)\
    \ / k * \\binom(n,k-1)\r\n     */\r\n    auto make_st = [&](int k, buffer_type&\
    \ q) {\r\n      std::fill(q.begin(), q.end(), 0);\r\n      if (k == 0)\r\n   \
    \     return 1;\r\n      int x = k;\r\n      while (d[x]) {\r\n        q[d[x]]++;\r\
    \n        x /= P[d[x]];\r\n      }\r\n      return x;\r\n    };\r\n    for (int\
    \ k = 1; k <= mid; k++) {\r\n      s = (long long) s * ns % mod();\r\n      for\
    \ (int i = 1; i <= MAX_PRIME_FACTOR_NUM; i++)\r\n        t[i] += nt[i];\r\n  \
    \    ns = make_st(k, nt);\r\n      s = (long long) s * inv[ns] % mod();\r\n  \
    \    for (int i = 1; i <= MAX_PRIME_FACTOR_NUM; i++)\r\n        t[i] -= nt[i];\r\
    \n      st_[i] = s;\r\n      for (int i = 0; i <= MAX_PRIME_FACTOR_NUM; i++)\r\
    \n        st_[i] = (long long) st_[i] * ptb[i][t[i]] % mod();\r\n    }\r\n  }\r\
    \n  constexpr inline int operator(int m) const {\r\n    assert(enabled());\r\n\
    \    if (m*2 > n_)\r\n      return operator()(n_-m);\r\n    return st_[m];\r\n\
    \  }\r\n};\r\n"
  code: "#pragma once\r\n#include <vector>\r\n#include <array>\r\n#include <type_traits>\r\
    \n\r\nnamespace math {\r\n\r\ntemplate<class T>\r\nconstexpr bool is_coprime(T\
    \ a, T b) {\r\n  using type = typename std::remove_const<typename std::remove_reference<T>::type>::type;\r\
    \n  type c = a;\r\n  type d = b;\r\n  while (d) {\r\n    std::swap(c,d);\r\n \
    \   d %= c;\r\n  }\r\n  return c == 1;\r\n}\r\n\r\n} // namespace\r\n\r\n/* \\\
    binom(N, m) mod M\r\n * @param N, M\r\n * @complexity\r\n *   - preprocess: O(N\
    \ \\log M / \\log\\log M)\r\n *   - query:      O(1)\r\n * @example\r\n *   int\
    \ binom_N_m_mod_M = ArbitraryModBinomial(N,M)(m);\r\n */\r\nstruct ArbitraryModBinomial\
    \ {\r\n  static constexpr int MAX_PRIME_FACTOR_NUM = 9;\r\n  using buffer_type\
    \ = std::array<int, MAX_PRIME_FACTOR_NUM+1>;\r\n  bool enabled_;\r\n  int n_,\
    \ mod_;\r\n  constexpr bool enabled() const { return enabled_; }\r\n  constexpr\
    \ int mod() { return mod_; }\r\n  std::vector<int> st_;\r\n  constexpr ArbitraryModBinomial(int\
    \ n, int mod)\r\n    : enabled_(math::is_coprime(n, mod)), n_(n), mod_(mod), st_((n+1)/2)\
    \ {\r\n    if (!enabled) return;\r\n    std::vector<bool> isn_p(n+1);\r\n    buffer_type\
    \ P{};\r\n    std::vector<int> d(n+1), inv(n+1);\r\n    int k = 0;\r\n    for\
    \ (int i = 2; i <= n; i++) {\r\n      if (isn_p[i]) continue;\r\n      for (int\
    \ j = i+i; j <= n; j += i)\r\n        isn_p[j] = 1;\r\n      if (mod() % i) continue;\r\
    \n      P[++k] = prime;\r\n      for (int v = prime; v <= n; v += prime)\r\n \
    \       d[v] = k;\r\n    }\r\n    using mint = Modular<mod()>;\r\n    int mid\
    \ = (n+1)/2;\r\n    std::vector<int> inv(mid+1);\r\n    inv[mid] = mint(mid).inv().val();\r\
    \n    for (int i = mid-1; i > 0; i--)\r\n      inv[i] = (long long) inv[i+1] *\
    \ (i+1) % mod();\r\n    int h = k+1;\r\n    int w = n/(P[1]-1) + 1;\r\n    std::vector<int>\
    \ ptb(h * w);\r\n    for (int i = 1; i <= MAX_PRIME_FACTOR_NUM; i++) {\r\n   \
    \   int t = n / (P[i]-1) + 1;\r\n      int v = 1;\r\n      ptb[i*h] = v;\r\n \
    \     for(int j = 1; j < t; j++) {\r\n        v = v * P[i] % mod();\r\n      \
    \  ptb[i*h+j] = v;\r\n      }\r\n    }\r\n    int s = 1, ns = 1;\r\n    buffer_type\
    \ t{}, nt{};\r\n    S[0] = T[0] = 1;\r\n    /* important equation\r\n     * \\\
    binom(n,k) = (n-k+1) / k * \\binom(n,k-1)\r\n     */\r\n    auto make_st = [&](int\
    \ k, buffer_type& q) {\r\n      std::fill(q.begin(), q.end(), 0);\r\n      if\
    \ (k == 0)\r\n        return 1;\r\n      int x = k;\r\n      while (d[x]) {\r\n\
    \        q[d[x]]++;\r\n        x /= P[d[x]];\r\n      }\r\n      return x;\r\n\
    \    };\r\n    for (int k = 1; k <= mid; k++) {\r\n      s = (long long) s * ns\
    \ % mod();\r\n      for (int i = 1; i <= MAX_PRIME_FACTOR_NUM; i++)\r\n      \
    \  t[i] += nt[i];\r\n      ns = make_st(k, nt);\r\n      s = (long long) s * inv[ns]\
    \ % mod();\r\n      for (int i = 1; i <= MAX_PRIME_FACTOR_NUM; i++)\r\n      \
    \  t[i] -= nt[i];\r\n      st_[i] = s;\r\n      for (int i = 0; i <= MAX_PRIME_FACTOR_NUM;\
    \ i++)\r\n        st_[i] = (long long) st_[i] * ptb[i][t[i]] % mod();\r\n    }\r\
    \n  }\r\n  constexpr inline int operator(int m) const {\r\n    assert(enabled());\r\
    \n    if (m*2 > n_)\r\n      return operator()(n_-m);\r\n    return st_[m];\r\n\
    \  }\r\n};\r\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/arbitrary_mod_binomial.hpp
  requiredBy: []
  timestamp: '2022-12-23 17:56:06+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/arbitrary_mod_binomial.hpp
layout: document
redirect_from:
- /library/include/mtl/arbitrary_mod_binomial.hpp
- /library/include/mtl/arbitrary_mod_binomial.hpp.html
title: include/mtl/arbitrary_mod_binomial.hpp
---
