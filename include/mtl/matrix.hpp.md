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
  bundledCode: "#line 2 \"include/mtl/matrix.hpp\"\n\r\n#include <cstddef>\r\n#include\
    \ <array>\r\n#include <cassert>\r\n\r\ntemplate <typename TYPE, unsigned ROW,\
    \ unsigned COLUMN>\r\nclass Matrix {\r\n public:\r\n  using value_type = TYPE;\r\
    \n  static constexpr unsigned R = ROW;\r\n  static constexpr unsigned C = COLUMN;\r\
    \n  using row_type = std::array<value_type, COLUMN>;\r\n  using matrix_type =\
    \ std::array<row_type, ROW>;\r\n\r\n private:\r\n  matrix_type mat_;\r\n\r\n public:\r\
    \n  Matrix() : mat_({}) {}\r\n  Matrix(std::initializer_list<row_type> list) {\r\
    \n    size_t i = 0;\r\n    for (auto &l : list) {\r\n      mat_[i] = l;\r\n  \
    \    i++;\r\n    }\r\n  }\r\n\r\n  static constexpr Matrix I() {\r\n    Matrix\
    \ ret;\r\n    auto d = std::min(R, C);\r\n    for (unsigned i = 0; i < d; i++)\
    \ {\r\n      ret[i][i] = 1;\r\n    }\r\n    return ret;\r\n  }\r\n\r\n  Matrix<value_type,\
    \ C, R> T() const {\r\n    Matrix<value_type, C, R> ret;\r\n    for (unsigned\
    \ i = 0; i < R; i++) {\r\n      for (unsigned j = 0; j < C; j++) {\r\n       \
    \ ret[j][i] = mat_[i][j];\r\n      }\r\n    }\r\n    return ret;\r\n  }\r\n\r\n\
    \  const row_type &operator[](size_t i) const { return mat_[i]; }\r\n  row_type\
    \ &operator[](size_t i) { return mat_[i]; }\r\n\r\n  bool operator==(const Matrix\
    \ &r) const {\r\n    for (unsigned i = 0; i < R; i++) {\r\n      for (unsigned\
    \ j = 0; j < C; j++) {\r\n        if (mat_[i][j] != r[i][j])\r\n          return\
    \ false;\r\n      }\r\n    }\r\n    return true;\r\n  }\r\n  bool operator!=(const\
    \ Matrix &r) const { return !(*this == r); }\r\n  Matrix operator+(const Matrix\
    \ &r) const {\r\n    Matrix ret;\r\n    for (unsigned i = 0; i < R; i++) {\r\n\
    \      for (unsigned j = 0; j < C; j++) {\r\n        ret[i][j] = mat_[i][j] +\
    \ r[i][j];\r\n      }\r\n    }\r\n    return ret;\r\n  }\r\n  Matrix &operator+=(const\
    \ Matrix &r) { return *this = *this + r; }\r\n  Matrix operator-() const {\r\n\
    \    Matrix ret;\r\n    for (unsigned i = 0; i < R; i++) {\r\n      for (unsigned\
    \ j = 0; j < C; j++) {\r\n        ret[i][j] = -mat_[i][j];\r\n      }\r\n    }\r\
    \n    return ret;\r\n  }\r\n  Matrix &operator-=(const Matrix &r) { return *this\
    \ = *this + -r; }\r\n  template <unsigned _COL>\r\n  Matrix<value_type, R, _COL>\r\
    \n  operator*(const Matrix<value_type, C, _COL> &r) const {\r\n    Matrix<value_type,\
    \ R, _COL> ret;\r\n    for (unsigned i = 0; i < R; i++) {\r\n      for (unsigned\
    \ j = 0; j < _COL; j++) {\r\n        for (unsigned k = 0; k < C; k++) {\r\n  \
    \        ret[i][j] += mat_[i][k] * r[k][j];\r\n        }\r\n      }\r\n    }\r\
    \n    return ret;\r\n  }\r\n  Matrix &operator*=(const Matrix &r) { return *this\
    \ = *this * r; }\r\n  Matrix pow(unsigned long long k) const {\r\n    assert(R\
    \ == C);\r\n    Matrix u = *this;\r\n    Matrix t = Matrix::I();\r\n    while\
    \ (k) {\r\n      if (k & 1) {\r\n        t *= u;\r\n      }\r\n      u *= u;\r\
    \n      k >>= 1;\r\n    }\r\n    return t;\r\n  }\r\n  friend Matrix pow(const\
    \ Matrix &mat, unsigned long long k) {\r\n    return mat.pow(k);\r\n  }\r\n};\r\
    \n"
  code: "#pragma once\r\n\r\n#include <cstddef>\r\n#include <array>\r\n#include <cassert>\r\
    \n\r\ntemplate <typename TYPE, unsigned ROW, unsigned COLUMN>\r\nclass Matrix\
    \ {\r\n public:\r\n  using value_type = TYPE;\r\n  static constexpr unsigned R\
    \ = ROW;\r\n  static constexpr unsigned C = COLUMN;\r\n  using row_type = std::array<value_type,\
    \ COLUMN>;\r\n  using matrix_type = std::array<row_type, ROW>;\r\n\r\n private:\r\
    \n  matrix_type mat_;\r\n\r\n public:\r\n  Matrix() : mat_({}) {}\r\n  Matrix(std::initializer_list<row_type>\
    \ list) {\r\n    size_t i = 0;\r\n    for (auto &l : list) {\r\n      mat_[i]\
    \ = l;\r\n      i++;\r\n    }\r\n  }\r\n\r\n  static constexpr Matrix I() {\r\n\
    \    Matrix ret;\r\n    auto d = std::min(R, C);\r\n    for (unsigned i = 0; i\
    \ < d; i++) {\r\n      ret[i][i] = 1;\r\n    }\r\n    return ret;\r\n  }\r\n\r\
    \n  Matrix<value_type, C, R> T() const {\r\n    Matrix<value_type, C, R> ret;\r\
    \n    for (unsigned i = 0; i < R; i++) {\r\n      for (unsigned j = 0; j < C;\
    \ j++) {\r\n        ret[j][i] = mat_[i][j];\r\n      }\r\n    }\r\n    return\
    \ ret;\r\n  }\r\n\r\n  const row_type &operator[](size_t i) const { return mat_[i];\
    \ }\r\n  row_type &operator[](size_t i) { return mat_[i]; }\r\n\r\n  bool operator==(const\
    \ Matrix &r) const {\r\n    for (unsigned i = 0; i < R; i++) {\r\n      for (unsigned\
    \ j = 0; j < C; j++) {\r\n        if (mat_[i][j] != r[i][j])\r\n          return\
    \ false;\r\n      }\r\n    }\r\n    return true;\r\n  }\r\n  bool operator!=(const\
    \ Matrix &r) const { return !(*this == r); }\r\n  Matrix operator+(const Matrix\
    \ &r) const {\r\n    Matrix ret;\r\n    for (unsigned i = 0; i < R; i++) {\r\n\
    \      for (unsigned j = 0; j < C; j++) {\r\n        ret[i][j] = mat_[i][j] +\
    \ r[i][j];\r\n      }\r\n    }\r\n    return ret;\r\n  }\r\n  Matrix &operator+=(const\
    \ Matrix &r) { return *this = *this + r; }\r\n  Matrix operator-() const {\r\n\
    \    Matrix ret;\r\n    for (unsigned i = 0; i < R; i++) {\r\n      for (unsigned\
    \ j = 0; j < C; j++) {\r\n        ret[i][j] = -mat_[i][j];\r\n      }\r\n    }\r\
    \n    return ret;\r\n  }\r\n  Matrix &operator-=(const Matrix &r) { return *this\
    \ = *this + -r; }\r\n  template <unsigned _COL>\r\n  Matrix<value_type, R, _COL>\r\
    \n  operator*(const Matrix<value_type, C, _COL> &r) const {\r\n    Matrix<value_type,\
    \ R, _COL> ret;\r\n    for (unsigned i = 0; i < R; i++) {\r\n      for (unsigned\
    \ j = 0; j < _COL; j++) {\r\n        for (unsigned k = 0; k < C; k++) {\r\n  \
    \        ret[i][j] += mat_[i][k] * r[k][j];\r\n        }\r\n      }\r\n    }\r\
    \n    return ret;\r\n  }\r\n  Matrix &operator*=(const Matrix &r) { return *this\
    \ = *this * r; }\r\n  Matrix pow(unsigned long long k) const {\r\n    assert(R\
    \ == C);\r\n    Matrix u = *this;\r\n    Matrix t = Matrix::I();\r\n    while\
    \ (k) {\r\n      if (k & 1) {\r\n        t *= u;\r\n      }\r\n      u *= u;\r\
    \n      k >>= 1;\r\n    }\r\n    return t;\r\n  }\r\n  friend Matrix pow(const\
    \ Matrix &mat, unsigned long long k) {\r\n    return mat.pow(k);\r\n  }\r\n};\r\
    \n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/matrix.hpp
  requiredBy: []
  timestamp: '2022-12-11 23:27:51+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/matrix.hpp
layout: document
redirect_from:
- /library/include/mtl/matrix.hpp
- /library/include/mtl/matrix.hpp.html
title: include/mtl/matrix.hpp
---
