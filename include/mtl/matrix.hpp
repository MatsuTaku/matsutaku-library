#pragma once

#include <cstddef>
#include <array>
#include <cassert>

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
  Matrix operator+(const Matrix &r) const {
    Matrix ret;
    for (unsigned i = 0; i < R; i++) {
      for (unsigned j = 0; j < C; j++) {
        ret[i][j] = mat_[i][j] + r[i][j];
      }
    }
    return ret;
  }
  Matrix &operator+=(const Matrix &r) { return *this = *this + r; }
  Matrix operator-() const {
    Matrix ret;
    for (unsigned i = 0; i < R; i++) {
      for (unsigned j = 0; j < C; j++) {
        ret[i][j] = -mat_[i][j];
      }
    }
    return ret;
  }
  Matrix &operator-=(const Matrix &r) { return *this = *this + -r; }
  template <unsigned _COL>
  Matrix<value_type, R, _COL>
  operator*(const Matrix<value_type, C, _COL> &r) const {
    Matrix<value_type, R, _COL> ret;
    for (unsigned i = 0; i < R; i++) {
      for (unsigned j = 0; j < _COL; j++) {
        for (unsigned k = 0; k < C; k++) {
          ret[i][j] += mat_[i][k] * r[k][j];
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
};
