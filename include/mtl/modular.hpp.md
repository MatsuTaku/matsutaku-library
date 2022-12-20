---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: include/mtl/fps.hpp
    title: include/mtl/fps.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/ntt.hpp
    title: include/mtl/ntt.hpp
  - icon: ':warning:'
    path: test/abc116-c-multiple_sequences-dirichlet-optimized.cpp
    title: test/abc116-c-multiple_sequences-dirichlet-optimized.cpp
  - icon: ':warning:'
    path: test/abc116-c-multiple_sequences-dirichlet.cpp
    title: test/abc116-c-multiple_sequences-dirichlet.cpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/convolution.test.cpp
    title: test/convolution.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/point_set_range_composite.test.cpp
    title: test/point_set_range_composite.test.cpp
  - icon: ':x:'
    path: test/range_affine_range_sum.test.cpp
    title: test/range_affine_range_sum.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':question:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/modular.hpp\"\n#include <iostream>\n\ntemplate\
    \ <long long MOD>\nclass Modular {\n private:\n  long long val_;\n\n public:\n\
    \  Modular() : val_(0) {}\n  Modular(long long v) : val_(v%MOD) {\n    if (val_\
    \ < 0) val_ += MOD;\n  }\n\n  long long val() const { return val_; }\n  Modular&\
    \ operator+=(Modular x) {\n    val_ += x.val();\n    if (val_ >= MOD) val_ %=\
    \ MOD;\n    return *this;\n  }\n  Modular operator-() const { return {MOD - val_};\
    \ }\n  Modular& operator-=(Modular x) {\n    val_ -= x.val();\n    if (val_ <\
    \ 0) val_ += MOD;\n    return *this;\n  }\n  Modular& operator*=(Modular x) {\n\
    \    val_ *= x.val();\n    if (val_ >= MOD) val_ %= MOD;\n    return *this;\n\
    \  }\n  Modular pow(long long p) const {\n    Modular t = 1;\n    Modular u =\
    \ *this;\n    while (p) {\n      if (p & 1)\n        t *= u;\n      u *= u;\n\
    \      p >>= 1;\n    }\n    return t;\n  }\n  friend Modular pow(Modular x, long\
    \ long p) {\n    return x.pow(p);\n  }\n  Modular inv() const { return pow(MOD-2);\
    \ }\n  Modular& operator/=(Modular x) { return *this *= x.inv(); }\n  Modular\
    \ operator+(Modular x) const { return Modular(*this) += x; }\n  Modular operator-(Modular\
    \ x) const { return Modular(*this) -= x; }\n  Modular operator*(Modular x) const\
    \ { return Modular(*this) *= x; }\n  Modular operator/(Modular x) const { return\
    \ Modular(*this) /= x; }\n  Modular& operator++() { return *this += 1; }\n  Modular\
    \ operator++(int) { Modular c = *this; ++(*this); return c; }\n  Modular& operator--()\
    \ { return *this -= 1; }\n  Modular operator--(int) { Modular c = *this; --(*this);\
    \ return c; }\n\n  bool operator==(Modular x) const { return val() == x.val();\
    \ }\n  bool operator!=(Modular x) const { return val() != x.val(); }\n  bool operator<(Modular\
    \ x) const { return val() < x.val(); };\n  bool operator<=(Modular x) const {\
    \ return val() <= x.val(); };\n  bool operator>(Modular x) const { return val()\
    \ > x.val(); };\n  bool operator>=(Modular x) const { return val() >= x.val();\
    \ };\n\n  friend std::ostream& operator<<(std::ostream& os, const Modular& x)\
    \ {\n    return os << x.val();\n  }\n  friend std::istream& operator>>(std::istream&\
    \ is, Modular& x) {\n    return is >> x.val_;\n  }\n\n};\n\nusing Modular998244353\
    \ = Modular<998244353>;\nusing Modular1000000007 = Modular<(int)1e9+7>;\n"
  code: "#pragma once\n#include <iostream>\n\ntemplate <long long MOD>\nclass Modular\
    \ {\n private:\n  long long val_;\n\n public:\n  Modular() : val_(0) {}\n  Modular(long\
    \ long v) : val_(v%MOD) {\n    if (val_ < 0) val_ += MOD;\n  }\n\n  long long\
    \ val() const { return val_; }\n  Modular& operator+=(Modular x) {\n    val_ +=\
    \ x.val();\n    if (val_ >= MOD) val_ %= MOD;\n    return *this;\n  }\n  Modular\
    \ operator-() const { return {MOD - val_}; }\n  Modular& operator-=(Modular x)\
    \ {\n    val_ -= x.val();\n    if (val_ < 0) val_ += MOD;\n    return *this;\n\
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
    \ Modular<(int)1e9+7>;\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/modular.hpp
  requiredBy:
  - test/abc116-c-multiple_sequences-dirichlet.cpp
  - test/abc116-c-multiple_sequences-dirichlet-optimized.cpp
  - include/mtl/fps.hpp
  - include/mtl/ntt.hpp
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_SOME_WA
  verifiedWith:
  - test/range_affine_range_sum.test.cpp
  - test/point_set_range_composite.test.cpp
  - test/convolution.test.cpp
documentation_of: include/mtl/modular.hpp
layout: document
redirect_from:
- /library/include/mtl/modular.hpp
- /library/include/mtl/modular.hpp.html
title: include/mtl/modular.hpp
---
