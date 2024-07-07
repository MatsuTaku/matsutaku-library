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
  bundledCode: "#line 2 \"include/mtl/fraction.hpp\"\n#if __cplusplus >= 202002L\n\
    #ifndef MTL_CPP20\n#define MTL_CPP20\n#endif\n#include <compare>\n#endif\n#include\
    \ <numeric>\n#include <cmath>\n#include <cassert>\n\ntemplate<typename I>\nstruct\
    \ FractionBase {\n  I u,v;\n  FractionBase(I _u, I _v) : u(_u), v(_v) {\n    assert(v\
    \ != 0);\n    if (u == 0) {\n      v = 1;\n    } else if (v < 0) {\n      u =\
    \ -u;\n      v = -v;\n    }\n  }\n  FractionBase(I _u = 0) : FractionBase(_u,\
    \ 1) {}\n  FractionBase reducted() const {\n    int g = std::gcd(u,v);\n    return\
    \ {u/g, v/g};\n  }\n  FractionBase& reduct() {\n    return *this = reducted();\n\
    \  }\n  template<typename T>\n  inline T get() const {\n    return (T) u / (T)\
    \ v;\n  }\n  \n};\n\nstruct FractionOrdering : public FractionBase<long long int>\
    \ {\n  FractionOrdering(long long int _u, long long int _v) : \n    FractionBase<long\
    \ long int>((_v >= 0 ? _u : -_u), std::abs(_v)) {}\n  FractionOrdering(long long\
    \ int u = 0) : FractionOrdering(u, 1) {}\n\n  inline bool operator==(const FractionOrdering&\
    \ rhs) const {\n    return u * rhs.v == v * rhs.u;\n  }\n#ifdef MTL_CPP20\n  auto\
    \ operator<=>(const FractionOrdering& rhs) const -> std::strong_ordering {\n \
    \   auto lv = u * rhs.v;\n    auto rv = v * rhs.u;\n    return lv <=> rv;\n  }\n\
    \  auto operator<=>(long long int rhs) const -> std::strong_ordering {\n    return\
    \ u <=> v * rhs;\n  }\n  friend auto operator==(long long int lhs, const FractionOrdering&\
    \ rhs) -> std::strong_ordering {\n    return lhs * rhs.v <=> rhs.u;\n  }\n#else\n\
    \  inline bool operator!=(const FractionOrdering& rhs) const {\n    return u *\
    \ rhs.v != v * rhs.u;\n  }\n  inline bool operator<(const FractionOrdering& rhs)\
    \ const {\n    return u * rhs.v < v * rhs.u;\n  }\n  inline bool operator<=(const\
    \ FractionOrdering& rhs) const {\n    return u * rhs.v <= v * rhs.u;\n  }\n  inline\
    \ bool operator>(const FractionOrdering& rhs) const {\n    return u * rhs.v >\
    \ v * rhs.u;\n  }\n  inline bool operator>=(const FractionOrdering& rhs) const\
    \ {\n    return u * rhs.v >= v * rhs.u;\n  }\n  inline bool operator==(long long\
    \ int rhs) const {\n    return u == v * rhs;\n  }\n  inline bool operator!=(long\
    \ long int rhs) const {\n    return u != v * rhs;\n  }\n  inline bool operator<(long\
    \ long int rhs) const {\n    return u < v * rhs;\n  }\n  inline bool operator<=(long\
    \ long int rhs) const {\n    return u <= v * rhs;\n  }\n  inline bool operator>(long\
    \ long int rhs) const {\n    return u > v * rhs;\n  }\n  inline bool operator>=(long\
    \ long int rhs) const {\n    return u >= v * rhs;\n  }\n  inline friend bool operator==(long\
    \ long int lhs, const FractionOrdering& rhs) {\n    return rhs == lhs;\n  }\n\
    \  inline friend bool operator!=(long long int lhs, const FractionOrdering& rhs)\
    \ {\n    return rhs != lhs;\n  }\n  inline friend bool operator<(long long int\
    \ lhs, const FractionOrdering& rhs) {\n    return rhs > lhs;\n  }\n  inline friend\
    \ bool operator<=(long long int lhs, const FractionOrdering& rhs) {\n    return\
    \ rhs >= lhs;\n  }\n  inline friend bool operator>(long long int lhs, const FractionOrdering&\
    \ rhs) {\n    return rhs < lhs;\n  }\n  inline friend bool operator>=(long long\
    \ int lhs, const FractionOrdering& rhs) {\n    return rhs <= lhs;\n  }\n#endif\n\
    \n};\n\nstruct Fraction : public FractionBase<long long> {\n  Fraction(long long\
    \ u, long long v) : FractionBase<long long>(u,v) {}\n  Fraction(long long u =\
    \ 0) : Fraction(u, 1) {}\n  Fraction(const FractionOrdering& fo) : Fraction(fo.u,fo.v)\
    \ {}\n\n  Fraction& reduct() {\n    FractionBase::reduct();\n    return *this;\n\
    \  }\n\n  Fraction& operator+=(const Fraction& r) {\n    auto g = std::gcd(v,\
    \ r.v);\n    auto rvbg = r.v / g;\n    auto nv = v * rvbg;\n    auto nu = u *\
    \ rvbg + r.u * (v / g);\n    u = nu;\n    v = nv;\n    return *this;\n  }\n  Fraction\
    \ operator+(const Fraction& r) const {\n    return Fraction(*this) += r;\n  }\n\
    \  Fraction& operator-=(const Fraction& r) {\n    return operator+=(-r);\n  }\n\
    \  Fraction operator-(const Fraction& r) const {\n    return Fraction(*this) -=\
    \ r;\n  }\n  Fraction& operator*=(const Fraction& r) {\n    auto nv = v * r.v;\n\
    \    auto nu = u * r.u;\n    auto g = std::gcd(nu, nv);\n    u = nu / g;\n   \
    \ v = nv / g;\n    return *this;\n  }\n  Fraction operator*(const Fraction& r)\
    \ const {\n    return Fraction(*this) *= r;\n  }\n  Fraction inv() const {\n \
    \   assert(u != 0);\n    return Fraction(v,u);\n  }\n  Fraction& operator/=(const\
    \ Fraction& r) {\n    return *this *= r.inv();\n  }\n  Fraction operator/(const\
    \ Fraction& r) const {\n    return Fraction(*this) /= r;\n  }\n  Fraction& operator+=(long\
    \ long x) {\n    u += x * v;\n    return reduct();\n  }\n  Fraction operator+(long\
    \ long x) const {\n    return Fraction(*this) += x;\n  }\n  Fraction& operator-=(long\
    \ long x) {\n    u -= x * v;\n    return reduct();\n  }\n  Fraction operator-(long\
    \ long x) const {\n    return Fraction(*this) -= x;\n  }\n  Fraction operator-()\
    \ const {\n    return Fraction(-u, v);\n  }\n  Fraction& operator*=(long long\
    \ x) {\n    u *= x;\n    return reduct();\n  }\n  Fraction operator*(long long\
    \ x) const {\n    return Fraction(*this) *= x;\n  }\n  Fraction& operator/=(long\
    \ long x) {\n    v *= x;\n    return reduct();\n  }\n  Fraction operator/(long\
    \ long x) const {\n    return Fraction(*this) /= x;\n  }\n  friend Fraction operator+(long\
    \ long x, const Fraction& r) {\n    return r + x;\n  }\n  friend Fraction operator-(long\
    \ long x, const Fraction& r) {\n    return -r + x;\n  }\n  friend Fraction operator*(long\
    \ long x, const Fraction& r) {\n    return r * x;\n  }\n  friend Fraction operator/(long\
    \ long x, const Fraction& r) {\n    return x * r.inv();\n  }\n\n  FractionOrdering\
    \ to_cmp() const {\n    return FractionOrdering(u,v);\n  }\n\n  bool operator==(const\
    \ Fraction& rhs) const {\n    return u * rhs.v == v * rhs.u;\n  }\n\n#ifdef MTL_CPP20\n\
    \  auto operator<=>(const Fraction& rhs) const -> decltype(to_cmp() <=> rhs.to_cmp())\
    \ {\n    return to_cmp() <=> rhs.to_cmp();\n  }\n#endif\n\n};\n\n"
  code: "#pragma once\n#if __cplusplus >= 202002L\n#ifndef MTL_CPP20\n#define MTL_CPP20\n\
    #endif\n#include <compare>\n#endif\n#include <numeric>\n#include <cmath>\n#include\
    \ <cassert>\n\ntemplate<typename I>\nstruct FractionBase {\n  I u,v;\n  FractionBase(I\
    \ _u, I _v) : u(_u), v(_v) {\n    assert(v != 0);\n    if (u == 0) {\n      v\
    \ = 1;\n    } else if (v < 0) {\n      u = -u;\n      v = -v;\n    }\n  }\n  FractionBase(I\
    \ _u = 0) : FractionBase(_u, 1) {}\n  FractionBase reducted() const {\n    int\
    \ g = std::gcd(u,v);\n    return {u/g, v/g};\n  }\n  FractionBase& reduct() {\n\
    \    return *this = reducted();\n  }\n  template<typename T>\n  inline T get()\
    \ const {\n    return (T) u / (T) v;\n  }\n  \n};\n\nstruct FractionOrdering :\
    \ public FractionBase<long long int> {\n  FractionOrdering(long long int _u, long\
    \ long int _v) : \n    FractionBase<long long int>((_v >= 0 ? _u : -_u), std::abs(_v))\
    \ {}\n  FractionOrdering(long long int u = 0) : FractionOrdering(u, 1) {}\n\n\
    \  inline bool operator==(const FractionOrdering& rhs) const {\n    return u *\
    \ rhs.v == v * rhs.u;\n  }\n#ifdef MTL_CPP20\n  auto operator<=>(const FractionOrdering&\
    \ rhs) const -> std::strong_ordering {\n    auto lv = u * rhs.v;\n    auto rv\
    \ = v * rhs.u;\n    return lv <=> rv;\n  }\n  auto operator<=>(long long int rhs)\
    \ const -> std::strong_ordering {\n    return u <=> v * rhs;\n  }\n  friend auto\
    \ operator==(long long int lhs, const FractionOrdering& rhs) -> std::strong_ordering\
    \ {\n    return lhs * rhs.v <=> rhs.u;\n  }\n#else\n  inline bool operator!=(const\
    \ FractionOrdering& rhs) const {\n    return u * rhs.v != v * rhs.u;\n  }\n  inline\
    \ bool operator<(const FractionOrdering& rhs) const {\n    return u * rhs.v <\
    \ v * rhs.u;\n  }\n  inline bool operator<=(const FractionOrdering& rhs) const\
    \ {\n    return u * rhs.v <= v * rhs.u;\n  }\n  inline bool operator>(const FractionOrdering&\
    \ rhs) const {\n    return u * rhs.v > v * rhs.u;\n  }\n  inline bool operator>=(const\
    \ FractionOrdering& rhs) const {\n    return u * rhs.v >= v * rhs.u;\n  }\n  inline\
    \ bool operator==(long long int rhs) const {\n    return u == v * rhs;\n  }\n\
    \  inline bool operator!=(long long int rhs) const {\n    return u != v * rhs;\n\
    \  }\n  inline bool operator<(long long int rhs) const {\n    return u < v * rhs;\n\
    \  }\n  inline bool operator<=(long long int rhs) const {\n    return u <= v *\
    \ rhs;\n  }\n  inline bool operator>(long long int rhs) const {\n    return u\
    \ > v * rhs;\n  }\n  inline bool operator>=(long long int rhs) const {\n    return\
    \ u >= v * rhs;\n  }\n  inline friend bool operator==(long long int lhs, const\
    \ FractionOrdering& rhs) {\n    return rhs == lhs;\n  }\n  inline friend bool\
    \ operator!=(long long int lhs, const FractionOrdering& rhs) {\n    return rhs\
    \ != lhs;\n  }\n  inline friend bool operator<(long long int lhs, const FractionOrdering&\
    \ rhs) {\n    return rhs > lhs;\n  }\n  inline friend bool operator<=(long long\
    \ int lhs, const FractionOrdering& rhs) {\n    return rhs >= lhs;\n  }\n  inline\
    \ friend bool operator>(long long int lhs, const FractionOrdering& rhs) {\n  \
    \  return rhs < lhs;\n  }\n  inline friend bool operator>=(long long int lhs,\
    \ const FractionOrdering& rhs) {\n    return rhs <= lhs;\n  }\n#endif\n\n};\n\n\
    struct Fraction : public FractionBase<long long> {\n  Fraction(long long u, long\
    \ long v) : FractionBase<long long>(u,v) {}\n  Fraction(long long u = 0) : Fraction(u,\
    \ 1) {}\n  Fraction(const FractionOrdering& fo) : Fraction(fo.u,fo.v) {}\n\n \
    \ Fraction& reduct() {\n    FractionBase::reduct();\n    return *this;\n  }\n\n\
    \  Fraction& operator+=(const Fraction& r) {\n    auto g = std::gcd(v, r.v);\n\
    \    auto rvbg = r.v / g;\n    auto nv = v * rvbg;\n    auto nu = u * rvbg + r.u\
    \ * (v / g);\n    u = nu;\n    v = nv;\n    return *this;\n  }\n  Fraction operator+(const\
    \ Fraction& r) const {\n    return Fraction(*this) += r;\n  }\n  Fraction& operator-=(const\
    \ Fraction& r) {\n    return operator+=(-r);\n  }\n  Fraction operator-(const\
    \ Fraction& r) const {\n    return Fraction(*this) -= r;\n  }\n  Fraction& operator*=(const\
    \ Fraction& r) {\n    auto nv = v * r.v;\n    auto nu = u * r.u;\n    auto g =\
    \ std::gcd(nu, nv);\n    u = nu / g;\n    v = nv / g;\n    return *this;\n  }\n\
    \  Fraction operator*(const Fraction& r) const {\n    return Fraction(*this) *=\
    \ r;\n  }\n  Fraction inv() const {\n    assert(u != 0);\n    return Fraction(v,u);\n\
    \  }\n  Fraction& operator/=(const Fraction& r) {\n    return *this *= r.inv();\n\
    \  }\n  Fraction operator/(const Fraction& r) const {\n    return Fraction(*this)\
    \ /= r;\n  }\n  Fraction& operator+=(long long x) {\n    u += x * v;\n    return\
    \ reduct();\n  }\n  Fraction operator+(long long x) const {\n    return Fraction(*this)\
    \ += x;\n  }\n  Fraction& operator-=(long long x) {\n    u -= x * v;\n    return\
    \ reduct();\n  }\n  Fraction operator-(long long x) const {\n    return Fraction(*this)\
    \ -= x;\n  }\n  Fraction operator-() const {\n    return Fraction(-u, v);\n  }\n\
    \  Fraction& operator*=(long long x) {\n    u *= x;\n    return reduct();\n  }\n\
    \  Fraction operator*(long long x) const {\n    return Fraction(*this) *= x;\n\
    \  }\n  Fraction& operator/=(long long x) {\n    v *= x;\n    return reduct();\n\
    \  }\n  Fraction operator/(long long x) const {\n    return Fraction(*this) /=\
    \ x;\n  }\n  friend Fraction operator+(long long x, const Fraction& r) {\n   \
    \ return r + x;\n  }\n  friend Fraction operator-(long long x, const Fraction&\
    \ r) {\n    return -r + x;\n  }\n  friend Fraction operator*(long long x, const\
    \ Fraction& r) {\n    return r * x;\n  }\n  friend Fraction operator/(long long\
    \ x, const Fraction& r) {\n    return x * r.inv();\n  }\n\n  FractionOrdering\
    \ to_cmp() const {\n    return FractionOrdering(u,v);\n  }\n\n  bool operator==(const\
    \ Fraction& rhs) const {\n    return u * rhs.v == v * rhs.u;\n  }\n\n#ifdef MTL_CPP20\n\
    \  auto operator<=>(const Fraction& rhs) const -> decltype(to_cmp() <=> rhs.to_cmp())\
    \ {\n    return to_cmp() <=> rhs.to_cmp();\n  }\n#endif\n\n};\n\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/fraction.hpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/fraction.hpp
layout: document
redirect_from:
- /library/include/mtl/fraction.hpp
- /library/include/mtl/fraction.hpp.html
title: include/mtl/fraction.hpp
---
