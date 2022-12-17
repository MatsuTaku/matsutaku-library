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
    #define CPL_CPP20\n#include <compare>\n#endif\n#include <numeric>\n#include <cassert>\n\
    \ntemplate<typename I>\nstruct FractionBase {\n  I u,v;\n  FractionBase(I _u,\
    \ I _v) : u(_u), v(_v) {\n    assert(v != 0);\n    if (u == 0) {\n      v = 1;\n\
    \    } else if (v < 0) {\n      u = -u;\n      v = -v;\n    }\n  }\n  FractionBase(I\
    \ _u = 0) : FractionBase(_u, 1) {}\n  FractionBase& reduct() {\n    int g = std::gcd(u,v);\n\
    \    u /= g;\n    v /= g;\n    return *this;\n  }\n  template<typename T>\n  inline\
    \ T get() const {\n    return (T) u / (T) v;\n  }\n  \n};\n\nstruct FractionOrdering\
    \ : public FractionBase<int> {\n  FractionOrdering(int _u, int _v) : FractionBase<int>(_u,_v)\
    \ {}\n  FractionOrdering(int u = 0) : FractionOrdering(u, 1) {}\n\n  inline bool\
    \ operator==(const FractionOrdering& rhs) const {\n    return (long long int)\
    \ u * rhs.v == (long long int) v * rhs.u;\n  }\n#ifdef CPL_CPP20\n  auto operator<=>(const\
    \ FractionOrdering&) const {\n    auto lv = (long long int) u * rhs.v;\n    auto\
    \ rv = (long long int) v * rhs.u;\n    if (lv != rv) {\n      return lv <=> rv;\n\
    \    } else {\n      return std::string_ordering::equal;\n    }\n  }\n#else\n\
    \  inline bool operator!=(const FractionOrdering& rhs) const {\n    return (long\
    \ long int) u * rhs.v != (long long int) v * rhs.u;\n  }\n  inline bool operator<(const\
    \ FractionOrdering& rhs) const {\n    return (long long int) u * rhs.v < (long\
    \ long int) v * rhs.u;\n  }\n  inline bool operator<=(const FractionOrdering&\
    \ rhs) const {\n    return (long long int) u * rhs.v <= (long long int) v * rhs.u;\n\
    \  }\n  inline bool operator>(const FractionOrdering& rhs) const {\n    return\
    \ (long long int) u * rhs.v > (long long int) v * rhs.u;\n  }\n  inline bool operator>=(const\
    \ FractionOrdering& rhs) const {\n    return (long long int) u * rhs.v >= (long\
    \ long int) v * rhs.u;\n  }\n#endif\n  inline bool operator==(int rhs) const {\n\
    \    return u == (long long int) v * rhs;\n  }\n  inline bool operator!=(int rhs)\
    \ const {\n    return u != (long long int) v * rhs;\n  }\n  inline bool operator<(int\
    \ rhs) const {\n    return u < (long long int) v * rhs;\n  }\n  inline bool operator<=(int\
    \ rhs) const {\n    return u <= (long long int) v * rhs;\n  }\n  inline bool operator>(int\
    \ rhs) const {\n    return u > (long long int) v * rhs;\n  }\n  inline bool operator>=(int\
    \ rhs) const {\n    return u >= (long long int) v * rhs;\n  }\n  inline friend\
    \ bool operator==(int lhs, const FractionOrdering& rhs) {\n    return rhs == lhs;\n\
    \  }\n  inline friend bool operator!=(int lhs, const FractionOrdering& rhs) {\n\
    \    return rhs != lhs;\n  }\n  inline friend bool operator<(int lhs, const FractionOrdering&\
    \ rhs) {\n    return rhs > lhs;\n  }\n  inline friend bool operator<=(int lhs,\
    \ const FractionOrdering& rhs) {\n    return rhs >= lhs;\n  }\n  inline friend\
    \ bool operator>(int lhs, const FractionOrdering& rhs) {\n    return rhs < lhs;\n\
    \  }\n  inline friend bool operator>=(int lhs, const FractionOrdering& rhs) {\n\
    \    return rhs <= lhs;\n  }\n\n};\n\nstruct Fraction : public FractionBase<long\
    \ long> {\n  Fraction(long long u, long long v) : FractionBase<long long>(u,v)\
    \ {}\n  Fraction(long long u = 0) : Fraction(u, 1) {}\n  Fraction(const FractionOrdering&\
    \ fo) : Fraction(fo.u,fo.v) {}\n\n  Fraction& reduct() {\n    FractionBase<long\
    \ long>::reduct();\n    return *this;\n  }\n  Fraction& operator+=(const Fraction&\
    \ r) {\n    auto g = std::gcd(v, r.v);\n    auto rvbg = r.v / g;\n    auto nv\
    \ = v * rvbg;\n    auto nu = u * rvbg + r.u * (v / g);\n    u = nu;\n    v = nv;\n\
    \    return *this;\n  }\n  Fraction operator+(const Fraction& r) const {\n   \
    \ return Fraction(*this) += r;\n  }\n  Fraction& operator-=(const Fraction& r)\
    \ {\n    return operator+=(-r);\n  }\n  Fraction operator-(const Fraction& r)\
    \ const {\n    return Fraction(*this) -= r;\n  }\n  Fraction& operator*=(const\
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
    \ x, const Fraction& r) {\n    return x * r.inv();\n  }\n\n};\n\n"
  code: "#pragma once\n#if __cplusplus >= 202002L\n#define CPL_CPP20\n#include <compare>\n\
    #endif\n#include <numeric>\n#include <cassert>\n\ntemplate<typename I>\nstruct\
    \ FractionBase {\n  I u,v;\n  FractionBase(I _u, I _v) : u(_u), v(_v) {\n    assert(v\
    \ != 0);\n    if (u == 0) {\n      v = 1;\n    } else if (v < 0) {\n      u =\
    \ -u;\n      v = -v;\n    }\n  }\n  FractionBase(I _u = 0) : FractionBase(_u,\
    \ 1) {}\n  FractionBase& reduct() {\n    int g = std::gcd(u,v);\n    u /= g;\n\
    \    v /= g;\n    return *this;\n  }\n  template<typename T>\n  inline T get()\
    \ const {\n    return (T) u / (T) v;\n  }\n  \n};\n\nstruct FractionOrdering :\
    \ public FractionBase<int> {\n  FractionOrdering(int _u, int _v) : FractionBase<int>(_u,_v)\
    \ {}\n  FractionOrdering(int u = 0) : FractionOrdering(u, 1) {}\n\n  inline bool\
    \ operator==(const FractionOrdering& rhs) const {\n    return (long long int)\
    \ u * rhs.v == (long long int) v * rhs.u;\n  }\n#ifdef CPL_CPP20\n  auto operator<=>(const\
    \ FractionOrdering&) const {\n    auto lv = (long long int) u * rhs.v;\n    auto\
    \ rv = (long long int) v * rhs.u;\n    if (lv != rv) {\n      return lv <=> rv;\n\
    \    } else {\n      return std::string_ordering::equal;\n    }\n  }\n#else\n\
    \  inline bool operator!=(const FractionOrdering& rhs) const {\n    return (long\
    \ long int) u * rhs.v != (long long int) v * rhs.u;\n  }\n  inline bool operator<(const\
    \ FractionOrdering& rhs) const {\n    return (long long int) u * rhs.v < (long\
    \ long int) v * rhs.u;\n  }\n  inline bool operator<=(const FractionOrdering&\
    \ rhs) const {\n    return (long long int) u * rhs.v <= (long long int) v * rhs.u;\n\
    \  }\n  inline bool operator>(const FractionOrdering& rhs) const {\n    return\
    \ (long long int) u * rhs.v > (long long int) v * rhs.u;\n  }\n  inline bool operator>=(const\
    \ FractionOrdering& rhs) const {\n    return (long long int) u * rhs.v >= (long\
    \ long int) v * rhs.u;\n  }\n#endif\n  inline bool operator==(int rhs) const {\n\
    \    return u == (long long int) v * rhs;\n  }\n  inline bool operator!=(int rhs)\
    \ const {\n    return u != (long long int) v * rhs;\n  }\n  inline bool operator<(int\
    \ rhs) const {\n    return u < (long long int) v * rhs;\n  }\n  inline bool operator<=(int\
    \ rhs) const {\n    return u <= (long long int) v * rhs;\n  }\n  inline bool operator>(int\
    \ rhs) const {\n    return u > (long long int) v * rhs;\n  }\n  inline bool operator>=(int\
    \ rhs) const {\n    return u >= (long long int) v * rhs;\n  }\n  inline friend\
    \ bool operator==(int lhs, const FractionOrdering& rhs) {\n    return rhs == lhs;\n\
    \  }\n  inline friend bool operator!=(int lhs, const FractionOrdering& rhs) {\n\
    \    return rhs != lhs;\n  }\n  inline friend bool operator<(int lhs, const FractionOrdering&\
    \ rhs) {\n    return rhs > lhs;\n  }\n  inline friend bool operator<=(int lhs,\
    \ const FractionOrdering& rhs) {\n    return rhs >= lhs;\n  }\n  inline friend\
    \ bool operator>(int lhs, const FractionOrdering& rhs) {\n    return rhs < lhs;\n\
    \  }\n  inline friend bool operator>=(int lhs, const FractionOrdering& rhs) {\n\
    \    return rhs <= lhs;\n  }\n\n};\n\nstruct Fraction : public FractionBase<long\
    \ long> {\n  Fraction(long long u, long long v) : FractionBase<long long>(u,v)\
    \ {}\n  Fraction(long long u = 0) : Fraction(u, 1) {}\n  Fraction(const FractionOrdering&\
    \ fo) : Fraction(fo.u,fo.v) {}\n\n  Fraction& reduct() {\n    FractionBase<long\
    \ long>::reduct();\n    return *this;\n  }\n  Fraction& operator+=(const Fraction&\
    \ r) {\n    auto g = std::gcd(v, r.v);\n    auto rvbg = r.v / g;\n    auto nv\
    \ = v * rvbg;\n    auto nu = u * rvbg + r.u * (v / g);\n    u = nu;\n    v = nv;\n\
    \    return *this;\n  }\n  Fraction operator+(const Fraction& r) const {\n   \
    \ return Fraction(*this) += r;\n  }\n  Fraction& operator-=(const Fraction& r)\
    \ {\n    return operator+=(-r);\n  }\n  Fraction operator-(const Fraction& r)\
    \ const {\n    return Fraction(*this) -= r;\n  }\n  Fraction& operator*=(const\
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
    \ x, const Fraction& r) {\n    return x * r.inv();\n  }\n\n};\n\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/fraction.hpp
  requiredBy: []
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/fraction.hpp
layout: document
redirect_from:
- /library/include/mtl/fraction.hpp
- /library/include/mtl/fraction.hpp.html
title: include/mtl/fraction.hpp
---
