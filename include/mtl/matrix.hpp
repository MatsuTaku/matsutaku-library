#pragma once
#include <cstddef>
#include <array>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <tuple>
#include <cassert>
#include <iostream>

template <typename TYPE, unsigned ROW, unsigned COLUMN>
class Matrix {
 public:
  using value_type = TYPE;
  static constexpr unsigned R = ROW;
  static constexpr unsigned C = COLUMN;
  using row_type = std::array<value_type, COLUMN>;
  using matrix_type = std::array<row_type, ROW>;

 private:
  matrix_type mat_;

 public:
  Matrix() : mat_({}) {}
  Matrix(std::initializer_list<row_type> list) {
    size_t i = 0;
    for (auto &l : list) {
      mat_[i] = l;
      i++;
    }
  }

  static constexpr Matrix I() {
    Matrix ret;
    auto d = std::min(R, C);
    for (unsigned i = 0; i < d; i++) {
      ret[i][i] = 1;
    }
    return ret;
  }

  Matrix<value_type, C, R> T() const {
    Matrix<value_type, C, R> ret;
    for (unsigned i = 0; i < R; i++) {
      for (unsigned j = 0; j < C; j++) {
        ret[j][i] = mat_[i][j];
      }
    }
    return ret;
  }

  const row_type &operator[](size_t i) const { return mat_[i]; }
  row_type &operator[](size_t i) { return mat_[i]; }

  bool operator==(const Matrix &r) const {
    for (unsigned i = 0; i < R; i++) {
      for (unsigned j = 0; j < C; j++) {
        if (mat_[i][j] != r[i][j])
          return false;
      }
    }
    return true;
  }
  bool operator!=(const Matrix &r) const { return !(*this == r); }
  Matrix &operator+=(const Matrix &r) {
    for (unsigned i = 0; i < R; i++) {
      for (unsigned j = 0; j < C; j++) {
        mat_[i][j] += r[i][j];
      }
    }
    return *this;
  }
  Matrix operator+(const Matrix &r) const {
    return Matrix(*this) += r;
  }
  Matrix operator-() const {
    Matrix ret;
    for (unsigned i = 0; i < R; i++) {
      for (unsigned j = 0; j < C; j++) {
        ret[i][j] = -mat_[i][j];
      }
    }
    return ret;
  }
  Matrix &operator-=(const Matrix &r) {
    for (unsigned i = 0; i < R; i++) {
      for (unsigned j = 0; j < C; j++) {
        mat_[i][j] = -r[i][j];
      }
    }
    return *this;
  }
  Matrix operator-(const Matrix &r) const {
    return Matrix(*this) -= r;
  }
  friend Matrix operator*(const value_type &l, const Matrix &r) {
    Matrix ret;
    for (unsigned i = 0; i < R; i++) {
      for (unsigned j = 0; j < C; j++) {
        ret[i][j] = l * r[i][j];
      }
    }
    return ret;
  }
  template <unsigned _COL>
  Matrix<value_type, R, _COL>
  operator*(const Matrix<value_type, C, _COL> &r) const {
    Matrix<value_type, R, _COL> ret;
    for (unsigned i = 0; i < R; i++) {
      for (unsigned k = 0; k < C; k++) {
        auto mik = mat_[i][k];
        for (unsigned j = 0; j < _COL; j++) {
          ret[i][j] += mik * r[k][j];
        }
      }
    }
    return ret;
  }
  Matrix &operator*=(const Matrix &r) { return *this = *this * r; }
  Matrix pow(unsigned long long k) const {
    assert(R == C);
    Matrix u = *this;
    Matrix t = Matrix::I();
    while (k) {
      if (k & 1) {
        t *= u;
      }
      u *= u;
      k >>= 1;
    }
    return t;
  }
  friend Matrix pow(const Matrix &mat, unsigned long long k) {
    return mat.pow(k);
  }
  /**
   * Return Determinant of matrix
   * using Gaussian elimination
  */
  value_type det(unsigned n = R) const {
    value_type ret = 1;
    auto x = *this;
    for (unsigned i = 0; i < n; i++) {
      unsigned index = i;
      while (index < n and x[index][i] == value_type(0)) {
        ++index;
      }
      if (index == n) {
        return 0;
      }
      if (index != i) {
        std::swap(x[index], x[i]);
        ret = -ret;
      }
      auto ixii = value_type(1) / x[i][i];
      for (unsigned j = i + 1; j < n; j++) {
        value_type factor = x[j][i] * ixii;
        for (unsigned k = i + 1; k < n; k++) {
          x[j][k] -= factor * x[i][k];
        }
      }
    }
    for (unsigned i = 0; i < n; i++) {
      ret *= x[i][i];
    }
    return ret;
  }
  // Return rank of left matrix
  unsigned gaussian_elimination(unsigned row=R, unsigned l_column=C, unsigned r_column=0, 
                                std::vector<unsigned>* principal = nullptr) {
    auto w = l_column+r_column;
    unsigned rank = 0;
    if (principal) principal->resize(row+1, l_column);
    for (unsigned i = 0; i < l_column and rank < row; i++) {
      unsigned index = row;
      value_type zero = 0;
      for (unsigned j = rank; j < row; j++) 
        if (mat_[j][i] != zero) {
        index = j;
        break;
      }
      if (index == row) 
        continue;
      if (index != rank) 
        std::swap(mat_[index], mat_[rank]);
      if (principal) (*principal)[rank] = i;
      auto ixii = value_type(1) / mat_[rank][i];
      mat_[rank][i] = 1;
      for (unsigned j = i + 1; j < w; j++) 
        mat_[rank][j] *= ixii;
      for (unsigned j = 0; j < row; j++) if (j != rank) {
        value_type factor = mat_[j][i];
        mat_[j][i] = zero;
        for (unsigned k = i + 1; k < w; k++) {
          mat_[j][k] -= factor * mat_[rank][k];
        }
      }
      ++rank;
    }
    return rank;
  }
  using system_type = Matrix<value_type, R, C+1>;
  using system_result_matrix = Matrix<value_type, R+1, C>;
  std::tuple<unsigned, system_result_matrix, bool> 
  solve(Matrix<value_type, R, 1>& b, unsigned row=R, unsigned column=C) const {
    // x = [A|b]
    system_type x;
    for (unsigned i = 0; i < row; i++) {
      for (unsigned j = 0; j < column; j++) {
        x[i][j] = mat_[i][j];
      }
      x[i][column] = b[i][0];
    }
    // x = [I|ans] ideally
    std::vector<unsigned> col;
    auto rank = x.gaussian_elimination(row, column, 1, &col);
    for (unsigned i = rank; i < row; i++) if (x[i][column] != value_type(0)) {
      return std::make_tuple(column-rank, system_result_matrix{}, false);
    }
    // { x(ans) } 
    // { basis  }
    system_result_matrix res;
    for (unsigned i = 0; i < rank; i++)
      res[0][col[i]] = x[i][column];
    unsigned t = 0;
    col[rank] = column;
    for (; t < col[0]; t++)
      res[t+1][t] = 1;
    for (unsigned i = 0; i < rank; i++) {
      for (unsigned c = col[i]+1; c < col[i+1]; c++) {
        for (unsigned j = 0; j <= i; j++) {
          res[t+1][col[j]] = -x[j][c];
        }
        res[t+1][c] = 1;
        t++;
      }
    }
    assert(t == column-rank);
    return std::make_tuple(column-rank, res, true);
  }
  std::pair<Matrix, bool> inv(unsigned n=R) const {
    // x = [A|I]
    Matrix<value_type, R, C*2> x;
    for (unsigned i = 0; i < n; i++) {
      for (unsigned j = 0; j < n; j++) {
        x[i][j] = mat_[i][j];
      }
      x[i][n+i] = 1;
    }
    // x = [I|A^-1]
    if (x.gaussian_elimination(n, n, n) != n) {
      return std::make_pair(Matrix{}, false);
    }
    Matrix ret;
    for (unsigned i = 0; i < n; i++) {
      for (unsigned j = 0; j < n; j++) {
        ret[i][j] = x[i][n+j];
      }
    }
    return std::make_pair(ret, true);
  }

};
