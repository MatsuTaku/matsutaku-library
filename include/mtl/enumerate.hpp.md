---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':warning:'
    path: test/abc116-c-multiple_sequences-dirichlet-optimized.cpp
    title: test/abc116-c-multiple_sequences-dirichlet-optimized.cpp
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/enumerate.hpp\"\n\n#include <vector>\n#include\
    \ <cassert>\n\ntemplate <typename MODULAR>\nclass Enumerate {\n public:\n  using\
    \ mint = MODULAR;\n private:\n  int max_n_ = 1;\n  std::vector<mint> fact_, ifact_;\n\
    \n  void _set_max_n(int n);\n\n public:\n  Enumerate() : fact_({1, 1}), ifact_({1,\
    \ 1}) {}\n  explicit Enumerate(int n) : fact_(std::max(2, n+1)), ifact_(std::max(2,\
    \ n+1)) {\n    fact_[0] = fact_[1] = ifact_[0] = ifact_[1] = 1;\n    _set_max_n(n);\n\
    \  }\n\n  mint cmb(int p, int q) {\n    if (p < q) return 0;\n    return fact(p)\
    \ * ifact(q) * ifact(p-q);\n  }\n\n  mint prm(int p, int q) {\n    if (p < q)\
    \ return 0;\n    return fact(p) * ifact(p-q);\n  }\n\n  mint fact(int p) {\n \
    \   if (p > max_n_)\n      _set_max_n(p);\n    return fact_[p];\n  }\n  mint ifact(int\
    \ p) {\n    if (p > max_n_)\n      _set_max_n(p);\n    return ifact_[p];\n  }\n\
    \n};\n\ntemplate<typename MODULAR>\nvoid Enumerate<MODULAR>::_set_max_n(int n)\
    \ {\n  if (n <= max_n_)\n    return;\n  int nxtn = std::max(max_n_*2, n);\n  fact_.resize(nxtn+1);\n\
    \  ifact_.resize(nxtn+1);\n  for (int i = max_n_+1; i <= nxtn; i++) {\n    fact_[i]\
    \ = fact_[i-1] * i;\n  }\n  ifact_[nxtn] = mint(1) / fact_[nxtn];\n  for (int\
    \ i = nxtn-1; i > max_n_; i--) {\n    ifact_[i] = ifact_[i+1] * (i+1);\n  }\n\
    \  max_n_ = nxtn;\n}\n\n"
  code: "#pragma once\n\n#include <vector>\n#include <cassert>\n\ntemplate <typename\
    \ MODULAR>\nclass Enumerate {\n public:\n  using mint = MODULAR;\n private:\n\
    \  int max_n_ = 1;\n  std::vector<mint> fact_, ifact_;\n\n  void _set_max_n(int\
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
    \  max_n_ = nxtn;\n}\n\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/enumerate.hpp
  requiredBy:
  - test/abc116-c-multiple_sequences-dirichlet-optimized.cpp
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/enumerate.hpp
layout: document
redirect_from:
- /library/include/mtl/enumerate.hpp
- /library/include/mtl/enumerate.hpp.html
title: include/mtl/enumerate.hpp
---
