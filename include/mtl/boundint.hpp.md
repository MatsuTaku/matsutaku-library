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
  bundledCode: "#line 2 \"include/mtl/boundint.hpp\"\n#include <iostream>\n\ntemplate<typename\
    \ T, int ID>\nclass boundint {\n private:\n  static T sup_;\n  T v_;\n public:\n\
    \  static void set_sup(T sup) { sup_ = sup; }\n  static T sup() { return sup_;\
    \ }\n  boundint(T v=0) : v_(v) {\n    if (v_ < 0)\n      v_ = 0;\n    if (v_ >\
    \ sup_)\n      v_ = sup_;\n  }\n  T val() const { return v_; }\n  bool operator==(const\
    \ boundint& r) const { return v_ == r.v_; }\n  bool operator<(const boundint&\
    \ r) const { return v_ < r.v_; }\n  bool operator<=(const boundint& r) const {\
    \ return v_ <= r.v_; }\n  bool operator>(const boundint& r) const { return v_\
    \ > r.v_; }\n  bool operator>=(const boundint& r) const { return v_ >= r.v_; }\n\
    \  template<typename U> bool operator==(const U& r) const { return v_ == r; }\n\
    \  template<typename U> bool operator<(const U& r) const { return v_ < r; }\n\
    \  template<typename U> bool operator<=(const U& r) const { return v_ <= r; }\n\
    \  template<typename U> bool operator>(const U& r) const { return v_ > r; }\n\
    \  template<typename U> bool operator>=(const U& r) const { return v_ >= r; }\n\
    \  boundint operator=(const boundint& r) {\n    v_ = r.v_;\n    return *this;\n\
    \  }\n  boundint operator+=(const boundint& r) {\n    v_ = v_ <= sup_ - r.v_ ?\
    \ v_ + r.v_ : sup_;\n    return *this;\n  }\n  boundint operator-=(const boundint&\
    \ r) {\n    v_ = v_ >= r.v_ ? v_ - r.v_ : 0;\n    return *this;\n  }\n  boundint\
    \ operator*=(const boundint& r) {\n    v_ = v_ <= sup_ / r.v_ ? v_ * r.v_ : sup_;\n\
    \    return *this;\n  }\n  boundint operator/=(const boundint& r) {\n    v_ /=\
    \ r.v_;\n    return *this;\n  }\n  boundint operator+(const boundint& r) const\
    \ { return boundint(*this) += r; }\n  boundint operator-(const boundint& r) const\
    \ { return boundint(*this) -= r; }\n  boundint operator*(const boundint& r) const\
    \ { return boundint(*this) *= r; }\n  boundint operator/(const boundint& r) const\
    \ { return boundint(*this) /= r; }\n\n  friend std::istream& operator>>(std::istream&\
    \ is, boundint& x) {\n    return is >> x.v_;\n  }\n  friend std::ostream& operator<<(std::ostream&\
    \ os, const boundint& x) {\n    return os << x.val();\n  }\n};\ntemplate<typename\
    \ T, int ID>\nT boundint<T, ID>::sup_;\n"
  code: "#pragma once\n#include <iostream>\n\ntemplate<typename T, int ID>\nclass\
    \ boundint {\n private:\n  static T sup_;\n  T v_;\n public:\n  static void set_sup(T\
    \ sup) { sup_ = sup; }\n  static T sup() { return sup_; }\n  boundint(T v=0) :\
    \ v_(v) {\n    if (v_ < 0)\n      v_ = 0;\n    if (v_ > sup_)\n      v_ = sup_;\n\
    \  }\n  T val() const { return v_; }\n  bool operator==(const boundint& r) const\
    \ { return v_ == r.v_; }\n  bool operator<(const boundint& r) const { return v_\
    \ < r.v_; }\n  bool operator<=(const boundint& r) const { return v_ <= r.v_; }\n\
    \  bool operator>(const boundint& r) const { return v_ > r.v_; }\n  bool operator>=(const\
    \ boundint& r) const { return v_ >= r.v_; }\n  template<typename U> bool operator==(const\
    \ U& r) const { return v_ == r; }\n  template<typename U> bool operator<(const\
    \ U& r) const { return v_ < r; }\n  template<typename U> bool operator<=(const\
    \ U& r) const { return v_ <= r; }\n  template<typename U> bool operator>(const\
    \ U& r) const { return v_ > r; }\n  template<typename U> bool operator>=(const\
    \ U& r) const { return v_ >= r; }\n  boundint operator=(const boundint& r) {\n\
    \    v_ = r.v_;\n    return *this;\n  }\n  boundint operator+=(const boundint&\
    \ r) {\n    v_ = v_ <= sup_ - r.v_ ? v_ + r.v_ : sup_;\n    return *this;\n  }\n\
    \  boundint operator-=(const boundint& r) {\n    v_ = v_ >= r.v_ ? v_ - r.v_ :\
    \ 0;\n    return *this;\n  }\n  boundint operator*=(const boundint& r) {\n   \
    \ v_ = v_ <= sup_ / r.v_ ? v_ * r.v_ : sup_;\n    return *this;\n  }\n  boundint\
    \ operator/=(const boundint& r) {\n    v_ /= r.v_;\n    return *this;\n  }\n \
    \ boundint operator+(const boundint& r) const { return boundint(*this) += r; }\n\
    \  boundint operator-(const boundint& r) const { return boundint(*this) -= r;\
    \ }\n  boundint operator*(const boundint& r) const { return boundint(*this) *=\
    \ r; }\n  boundint operator/(const boundint& r) const { return boundint(*this)\
    \ /= r; }\n\n  friend std::istream& operator>>(std::istream& is, boundint& x)\
    \ {\n    return is >> x.v_;\n  }\n  friend std::ostream& operator<<(std::ostream&\
    \ os, const boundint& x) {\n    return os << x.val();\n  }\n};\ntemplate<typename\
    \ T, int ID>\nT boundint<T, ID>::sup_;\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/boundint.hpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/boundint.hpp
layout: document
redirect_from:
- /library/include/mtl/boundint.hpp
- /library/include/mtl/boundint.hpp.html
title: include/mtl/boundint.hpp
---
