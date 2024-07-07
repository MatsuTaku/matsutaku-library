---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':x:'
    path: test/yosupo/matrix/determinant_of_matrix.test.cpp
    title: test/yosupo/matrix/determinant_of_matrix.test.cpp
  - icon: ':x:'
    path: test/yosupo/matrix/inverse_matrix.test.cpp
    title: test/yosupo/matrix/inverse_matrix.test.cpp
  - icon: ':x:'
    path: test/yosupo/matrix/matrix_product.test.cpp
    title: test/yosupo/matrix/matrix_product.test.cpp
  - icon: ':x:'
    path: test/yosupo/matrix/system_of_linear_equations.test.cpp
    title: test/yosupo/matrix/system_of_linear_equations.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':x:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/matrix.hpp\"\n#include <cstddef>\r\n#include\
    \ <array>\r\n#include <vector>\r\n#include <numeric>\r\n#include <algorithm>\r\
    \n#include <cmath>\r\n#include <tuple>\r\n#include <cassert>\r\n#include <iostream>\r\
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
    \ Matrix &r) const { return !(*this == r); }\r\n  Matrix &operator+=(const Matrix\
    \ &r) {\r\n    for (unsigned i = 0; i < R; i++) {\r\n      for (unsigned j = 0;\
    \ j < C; j++) {\r\n        mat_[i][j] += r[i][j];\r\n      }\r\n    }\r\n    return\
    \ *this;\r\n  }\r\n  Matrix operator+(const Matrix &r) const {\r\n    return Matrix(*this)\
    \ += r;\r\n  }\r\n  Matrix operator-() const {\r\n    Matrix ret;\r\n    for (unsigned\
    \ i = 0; i < R; i++) {\r\n      for (unsigned j = 0; j < C; j++) {\r\n       \
    \ ret[i][j] = -mat_[i][j];\r\n      }\r\n    }\r\n    return ret;\r\n  }\r\n \
    \ Matrix &operator-=(const Matrix &r) {\r\n    for (unsigned i = 0; i < R; i++)\
    \ {\r\n      for (unsigned j = 0; j < C; j++) {\r\n        mat_[i][j] = -r[i][j];\r\
    \n      }\r\n    }\r\n    return *this;\r\n  }\r\n  Matrix operator-(const Matrix\
    \ &r) const {\r\n    return Matrix(*this) -= r;\r\n  }\r\n  friend Matrix operator*(const\
    \ value_type &l, const Matrix &r) {\r\n    Matrix ret;\r\n    for (unsigned i\
    \ = 0; i < R; i++) {\r\n      for (unsigned j = 0; j < C; j++) {\r\n        ret[i][j]\
    \ = l * r[i][j];\r\n      }\r\n    }\r\n    return ret;\r\n  }\r\n  template <unsigned\
    \ _COL>\r\n  Matrix<value_type, R, _COL>\r\n  operator*(const Matrix<value_type,\
    \ C, _COL> &r) const {\r\n    Matrix<value_type, R, _COL> ret;\r\n    for (unsigned\
    \ i = 0; i < R; i++) {\r\n      for (unsigned k = 0; k < C; k++) {\r\n       \
    \ auto mik = mat_[i][k];\r\n        for (unsigned j = 0; j < _COL; j++) {\r\n\
    \          ret[i][j] += mik * r[k][j];\r\n        }\r\n      }\r\n    }\r\n  \
    \  return ret;\r\n  }\r\n  Matrix &operator*=(const Matrix &r) { return *this\
    \ = *this * r; }\r\n  Matrix pow(unsigned long long k) const {\r\n    assert(R\
    \ == C);\r\n    Matrix u = *this;\r\n    Matrix t = Matrix::I();\r\n    while\
    \ (k) {\r\n      if (k & 1) {\r\n        t *= u;\r\n      }\r\n      u *= u;\r\
    \n      k >>= 1;\r\n    }\r\n    return t;\r\n  }\r\n  friend Matrix pow(const\
    \ Matrix &mat, unsigned long long k) {\r\n    return mat.pow(k);\r\n  }\r\n  /**\r\
    \n   * Return Determinant of matrix\r\n   * using Gaussian elimination\r\n  */\r\
    \n  value_type det(unsigned n = R) const {\r\n    value_type ret = 1;\r\n    auto\
    \ x = *this;\r\n    for (unsigned i = 0; i < n; i++) {\r\n      unsigned index\
    \ = i;\r\n      while (index < n and x[index][i] == value_type(0)) {\r\n     \
    \   ++index;\r\n      }\r\n      if (index == n) {\r\n        return 0;\r\n  \
    \    }\r\n      if (index != i) {\r\n        std::swap(x[index], x[i]);\r\n  \
    \      ret = -ret;\r\n      }\r\n      auto ixii = value_type(1) / x[i][i];\r\n\
    \      for (unsigned j = i + 1; j < n; j++) {\r\n        value_type factor = x[j][i]\
    \ * ixii;\r\n        for (unsigned k = i + 1; k < n; k++) {\r\n          x[j][k]\
    \ -= factor * x[i][k];\r\n        }\r\n      }\r\n    }\r\n    for (unsigned i\
    \ = 0; i < n; i++) {\r\n      ret *= x[i][i];\r\n    }\r\n    return ret;\r\n\
    \  }\r\n  // Return rank of left matrix\r\n  unsigned gaussian_elimination(unsigned\
    \ row=R, unsigned l_column=C, unsigned r_column=0, \r\n                      \
    \          std::vector<unsigned>* principal = nullptr) {\r\n    auto w = l_column+r_column;\r\
    \n    unsigned rank = 0;\r\n    if (principal) principal->resize(row+1, l_column);\r\
    \n    for (unsigned i = 0; i < l_column and rank < row; i++) {\r\n      unsigned\
    \ index = row;\r\n      value_type zero = 0;\r\n      for (unsigned j = rank;\
    \ j < row; j++) \r\n        if (mat_[j][i] != zero) {\r\n        index = j;\r\n\
    \        break;\r\n      }\r\n      if (index == row) \r\n        continue;\r\n\
    \      if (index != rank) \r\n        std::swap(mat_[index], mat_[rank]);\r\n\
    \      if (principal) (*principal)[rank] = i;\r\n      auto ixii = value_type(1)\
    \ / mat_[rank][i];\r\n      mat_[rank][i] = 1;\r\n      for (unsigned j = i +\
    \ 1; j < w; j++) \r\n        mat_[rank][j] *= ixii;\r\n      for (unsigned j =\
    \ 0; j < row; j++) if (j != rank) {\r\n        value_type factor = mat_[j][i];\r\
    \n        mat_[j][i] = zero;\r\n        for (unsigned k = i + 1; k < w; k++) {\r\
    \n          mat_[j][k] -= factor * mat_[rank][k];\r\n        }\r\n      }\r\n\
    \      ++rank;\r\n    }\r\n    return rank;\r\n  }\r\n  using system_type = Matrix<value_type,\
    \ R, C+1>;\r\n  using system_result_matrix = Matrix<value_type, R+1, C>;\r\n \
    \ std::tuple<unsigned, system_result_matrix, bool> \r\n  solve(Matrix<value_type,\
    \ R, 1>& b, unsigned row=R, unsigned column=C) const {\r\n    // x = [A|b]\r\n\
    \    system_type x;\r\n    for (unsigned i = 0; i < row; i++) {\r\n      for (unsigned\
    \ j = 0; j < column; j++) {\r\n        x[i][j] = mat_[i][j];\r\n      }\r\n  \
    \    x[i][column] = b[i][0];\r\n    }\r\n    // x = [I|ans] ideally\r\n    std::vector<unsigned>\
    \ col;\r\n    auto rank = x.gaussian_elimination(row, column, 1, &col);\r\n  \
    \  for (unsigned i = rank; i < row; i++) if (x[i][column] != value_type(0)) {\r\
    \n      return std::make_tuple(column-rank, system_result_matrix{}, false);\r\n\
    \    }\r\n    // { x(ans) } \r\n    // { basis  }\r\n    system_result_matrix\
    \ res;\r\n    for (unsigned i = 0; i < rank; i++)\r\n      res[0][col[i]] = x[i][column];\r\
    \n    unsigned t = 0;\r\n    col[rank] = column;\r\n    for (; t < col[0]; t++)\r\
    \n      res[t+1][t] = 1;\r\n    for (unsigned i = 0; i < rank; i++) {\r\n    \
    \  for (unsigned c = col[i]+1; c < col[i+1]; c++) {\r\n        for (unsigned j\
    \ = 0; j <= i; j++) {\r\n          res[t+1][col[j]] = -x[j][c];\r\n        }\r\
    \n        res[t+1][c] = 1;\r\n        t++;\r\n      }\r\n    }\r\n    assert(t\
    \ == column-rank);\r\n    return std::make_tuple(column-rank, res, true);\r\n\
    \  }\r\n  std::pair<Matrix, bool> inv(unsigned n=R) const {\r\n    // x = [A|I]\r\
    \n    Matrix<value_type, R, C*2> x;\r\n    for (unsigned i = 0; i < n; i++) {\r\
    \n      for (unsigned j = 0; j < n; j++) {\r\n        x[i][j] = mat_[i][j];\r\n\
    \      }\r\n      x[i][n+i] = 1;\r\n    }\r\n    // x = [I|A^-1]\r\n    if (x.gaussian_elimination(n,\
    \ n, n) != n) {\r\n      return std::make_pair(Matrix{}, false);\r\n    }\r\n\
    \    Matrix ret;\r\n    for (unsigned i = 0; i < n; i++) {\r\n      for (unsigned\
    \ j = 0; j < n; j++) {\r\n        ret[i][j] = x[i][n+j];\r\n      }\r\n    }\r\
    \n    return std::make_pair(ret, true);\r\n  }\r\n\r\n};\r\n"
  code: "#pragma once\r\n#include <cstddef>\r\n#include <array>\r\n#include <vector>\r\
    \n#include <numeric>\r\n#include <algorithm>\r\n#include <cmath>\r\n#include <tuple>\r\
    \n#include <cassert>\r\n#include <iostream>\r\n\r\ntemplate <typename TYPE, unsigned\
    \ ROW, unsigned COLUMN>\r\nclass Matrix {\r\n public:\r\n  using value_type =\
    \ TYPE;\r\n  static constexpr unsigned R = ROW;\r\n  static constexpr unsigned\
    \ C = COLUMN;\r\n  using row_type = std::array<value_type, COLUMN>;\r\n  using\
    \ matrix_type = std::array<row_type, ROW>;\r\n\r\n private:\r\n  matrix_type mat_;\r\
    \n\r\n public:\r\n  Matrix() : mat_({}) {}\r\n  Matrix(std::initializer_list<row_type>\
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
    \ Matrix &r) const { return !(*this == r); }\r\n  Matrix &operator+=(const Matrix\
    \ &r) {\r\n    for (unsigned i = 0; i < R; i++) {\r\n      for (unsigned j = 0;\
    \ j < C; j++) {\r\n        mat_[i][j] += r[i][j];\r\n      }\r\n    }\r\n    return\
    \ *this;\r\n  }\r\n  Matrix operator+(const Matrix &r) const {\r\n    return Matrix(*this)\
    \ += r;\r\n  }\r\n  Matrix operator-() const {\r\n    Matrix ret;\r\n    for (unsigned\
    \ i = 0; i < R; i++) {\r\n      for (unsigned j = 0; j < C; j++) {\r\n       \
    \ ret[i][j] = -mat_[i][j];\r\n      }\r\n    }\r\n    return ret;\r\n  }\r\n \
    \ Matrix &operator-=(const Matrix &r) {\r\n    for (unsigned i = 0; i < R; i++)\
    \ {\r\n      for (unsigned j = 0; j < C; j++) {\r\n        mat_[i][j] = -r[i][j];\r\
    \n      }\r\n    }\r\n    return *this;\r\n  }\r\n  Matrix operator-(const Matrix\
    \ &r) const {\r\n    return Matrix(*this) -= r;\r\n  }\r\n  friend Matrix operator*(const\
    \ value_type &l, const Matrix &r) {\r\n    Matrix ret;\r\n    for (unsigned i\
    \ = 0; i < R; i++) {\r\n      for (unsigned j = 0; j < C; j++) {\r\n        ret[i][j]\
    \ = l * r[i][j];\r\n      }\r\n    }\r\n    return ret;\r\n  }\r\n  template <unsigned\
    \ _COL>\r\n  Matrix<value_type, R, _COL>\r\n  operator*(const Matrix<value_type,\
    \ C, _COL> &r) const {\r\n    Matrix<value_type, R, _COL> ret;\r\n    for (unsigned\
    \ i = 0; i < R; i++) {\r\n      for (unsigned k = 0; k < C; k++) {\r\n       \
    \ auto mik = mat_[i][k];\r\n        for (unsigned j = 0; j < _COL; j++) {\r\n\
    \          ret[i][j] += mik * r[k][j];\r\n        }\r\n      }\r\n    }\r\n  \
    \  return ret;\r\n  }\r\n  Matrix &operator*=(const Matrix &r) { return *this\
    \ = *this * r; }\r\n  Matrix pow(unsigned long long k) const {\r\n    assert(R\
    \ == C);\r\n    Matrix u = *this;\r\n    Matrix t = Matrix::I();\r\n    while\
    \ (k) {\r\n      if (k & 1) {\r\n        t *= u;\r\n      }\r\n      u *= u;\r\
    \n      k >>= 1;\r\n    }\r\n    return t;\r\n  }\r\n  friend Matrix pow(const\
    \ Matrix &mat, unsigned long long k) {\r\n    return mat.pow(k);\r\n  }\r\n  /**\r\
    \n   * Return Determinant of matrix\r\n   * using Gaussian elimination\r\n  */\r\
    \n  value_type det(unsigned n = R) const {\r\n    value_type ret = 1;\r\n    auto\
    \ x = *this;\r\n    for (unsigned i = 0; i < n; i++) {\r\n      unsigned index\
    \ = i;\r\n      while (index < n and x[index][i] == value_type(0)) {\r\n     \
    \   ++index;\r\n      }\r\n      if (index == n) {\r\n        return 0;\r\n  \
    \    }\r\n      if (index != i) {\r\n        std::swap(x[index], x[i]);\r\n  \
    \      ret = -ret;\r\n      }\r\n      auto ixii = value_type(1) / x[i][i];\r\n\
    \      for (unsigned j = i + 1; j < n; j++) {\r\n        value_type factor = x[j][i]\
    \ * ixii;\r\n        for (unsigned k = i + 1; k < n; k++) {\r\n          x[j][k]\
    \ -= factor * x[i][k];\r\n        }\r\n      }\r\n    }\r\n    for (unsigned i\
    \ = 0; i < n; i++) {\r\n      ret *= x[i][i];\r\n    }\r\n    return ret;\r\n\
    \  }\r\n  // Return rank of left matrix\r\n  unsigned gaussian_elimination(unsigned\
    \ row=R, unsigned l_column=C, unsigned r_column=0, \r\n                      \
    \          std::vector<unsigned>* principal = nullptr) {\r\n    auto w = l_column+r_column;\r\
    \n    unsigned rank = 0;\r\n    if (principal) principal->resize(row+1, l_column);\r\
    \n    for (unsigned i = 0; i < l_column and rank < row; i++) {\r\n      unsigned\
    \ index = row;\r\n      value_type zero = 0;\r\n      for (unsigned j = rank;\
    \ j < row; j++) \r\n        if (mat_[j][i] != zero) {\r\n        index = j;\r\n\
    \        break;\r\n      }\r\n      if (index == row) \r\n        continue;\r\n\
    \      if (index != rank) \r\n        std::swap(mat_[index], mat_[rank]);\r\n\
    \      if (principal) (*principal)[rank] = i;\r\n      auto ixii = value_type(1)\
    \ / mat_[rank][i];\r\n      mat_[rank][i] = 1;\r\n      for (unsigned j = i +\
    \ 1; j < w; j++) \r\n        mat_[rank][j] *= ixii;\r\n      for (unsigned j =\
    \ 0; j < row; j++) if (j != rank) {\r\n        value_type factor = mat_[j][i];\r\
    \n        mat_[j][i] = zero;\r\n        for (unsigned k = i + 1; k < w; k++) {\r\
    \n          mat_[j][k] -= factor * mat_[rank][k];\r\n        }\r\n      }\r\n\
    \      ++rank;\r\n    }\r\n    return rank;\r\n  }\r\n  using system_type = Matrix<value_type,\
    \ R, C+1>;\r\n  using system_result_matrix = Matrix<value_type, R+1, C>;\r\n \
    \ std::tuple<unsigned, system_result_matrix, bool> \r\n  solve(Matrix<value_type,\
    \ R, 1>& b, unsigned row=R, unsigned column=C) const {\r\n    // x = [A|b]\r\n\
    \    system_type x;\r\n    for (unsigned i = 0; i < row; i++) {\r\n      for (unsigned\
    \ j = 0; j < column; j++) {\r\n        x[i][j] = mat_[i][j];\r\n      }\r\n  \
    \    x[i][column] = b[i][0];\r\n    }\r\n    // x = [I|ans] ideally\r\n    std::vector<unsigned>\
    \ col;\r\n    auto rank = x.gaussian_elimination(row, column, 1, &col);\r\n  \
    \  for (unsigned i = rank; i < row; i++) if (x[i][column] != value_type(0)) {\r\
    \n      return std::make_tuple(column-rank, system_result_matrix{}, false);\r\n\
    \    }\r\n    // { x(ans) } \r\n    // { basis  }\r\n    system_result_matrix\
    \ res;\r\n    for (unsigned i = 0; i < rank; i++)\r\n      res[0][col[i]] = x[i][column];\r\
    \n    unsigned t = 0;\r\n    col[rank] = column;\r\n    for (; t < col[0]; t++)\r\
    \n      res[t+1][t] = 1;\r\n    for (unsigned i = 0; i < rank; i++) {\r\n    \
    \  for (unsigned c = col[i]+1; c < col[i+1]; c++) {\r\n        for (unsigned j\
    \ = 0; j <= i; j++) {\r\n          res[t+1][col[j]] = -x[j][c];\r\n        }\r\
    \n        res[t+1][c] = 1;\r\n        t++;\r\n      }\r\n    }\r\n    assert(t\
    \ == column-rank);\r\n    return std::make_tuple(column-rank, res, true);\r\n\
    \  }\r\n  std::pair<Matrix, bool> inv(unsigned n=R) const {\r\n    // x = [A|I]\r\
    \n    Matrix<value_type, R, C*2> x;\r\n    for (unsigned i = 0; i < n; i++) {\r\
    \n      for (unsigned j = 0; j < n; j++) {\r\n        x[i][j] = mat_[i][j];\r\n\
    \      }\r\n      x[i][n+i] = 1;\r\n    }\r\n    // x = [I|A^-1]\r\n    if (x.gaussian_elimination(n,\
    \ n, n) != n) {\r\n      return std::make_pair(Matrix{}, false);\r\n    }\r\n\
    \    Matrix ret;\r\n    for (unsigned i = 0; i < n; i++) {\r\n      for (unsigned\
    \ j = 0; j < n; j++) {\r\n        ret[i][j] = x[i][n+j];\r\n      }\r\n    }\r\
    \n    return std::make_pair(ret, true);\r\n  }\r\n\r\n};\r\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/matrix.hpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_ALL_WA
  verifiedWith:
  - test/yosupo/matrix/inverse_matrix.test.cpp
  - test/yosupo/matrix/matrix_product.test.cpp
  - test/yosupo/matrix/system_of_linear_equations.test.cpp
  - test/yosupo/matrix/determinant_of_matrix.test.cpp
documentation_of: include/mtl/matrix.hpp
layout: document
redirect_from:
- /library/include/mtl/matrix.hpp
- /library/include/mtl/matrix.hpp.html
title: include/mtl/matrix.hpp
---
