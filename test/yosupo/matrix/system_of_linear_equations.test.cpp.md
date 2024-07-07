---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':x:'
    path: include/mtl/matrix.hpp
    title: include/mtl/matrix.hpp
  - icon: ':question:'
    path: include/mtl/modular.hpp
    title: include/mtl/modular.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/system_of_linear_equations
    links:
    - https://judge.yosupo.jp/problem/system_of_linear_equations
  bundledCode: "#line 1 \"test/yosupo/matrix/system_of_linear_equations.test.cpp\"\
    \n#define PROBLEM \"https://judge.yosupo.jp/problem/system_of_linear_equations\"\
    \n#line 2 \"include/mtl/matrix.hpp\"\n#include <cstddef>\r\n#include <array>\r\
    \n#include <vector>\r\n#include <numeric>\r\n#include <algorithm>\r\n#include\
    \ <cmath>\r\n#include <tuple>\r\n#include <cassert>\r\n#include <iostream>\r\n\
    \r\ntemplate <typename TYPE, unsigned ROW, unsigned COLUMN>\r\nclass Matrix {\r\
    \n public:\r\n  using value_type = TYPE;\r\n  static constexpr unsigned R = ROW;\r\
    \n  static constexpr unsigned C = COLUMN;\r\n  using row_type = std::array<value_type,\
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
    \n    return std::make_pair(ret, true);\r\n  }\r\n\r\n};\r\n#line 2 \"include/mtl/bit_manip.hpp\"\
    \n#include <cstdint>\n#line 4 \"include/mtl/bit_manip.hpp\"\n\nnamespace bm {\n\
    \ninline constexpr uint64_t popcnt_e8(uint64_t x) {\n  x = (x & 0x5555555555555555)\
    \ + ((x>>1) & 0x5555555555555555);\n  x = (x & 0x3333333333333333) + ((x>>2) &\
    \ 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n\
    \  return x;\n}\n// Count 1s\ninline constexpr unsigned popcnt(uint64_t x) {\n\
    \  return (popcnt_e8(x) * 0x0101010101010101) >> 56;\n}\n// Count trailing 0s.\
    \ s.t. *11011000 -> 3\ninline constexpr unsigned ctz(uint64_t x) {\n  return popcnt((x\
    \ & (-x)) - 1);\n}\ninline constexpr unsigned ctz8(uint8_t x) {\n  return x ==\
    \ 0 ? 8 : popcnt_e8((x & (-x)) - 1);\n}\n// [00..0](8bit) -> 0, [**..*](not only\
    \ 0) -> 1\ninline constexpr uint8_t summary(uint64_t x) {\n  constexpr uint64_t\
    \ hmask = 0x8080808080808080ull;\n  constexpr uint64_t lmask = 0x7F7F7F7F7F7F7F7Full;\n\
    \  auto a = x & hmask;\n  auto b = x & lmask;\n  b = hmask - b;\n  b = ~b;\n \
    \ auto c = (a | b) & hmask;\n  c *= 0x0002040810204081ull;\n  return uint8_t(c\
    \ >> 56);\n}\n// Extract target area of bits\ninline constexpr uint64_t bextr(uint64_t\
    \ x, unsigned start, unsigned len) {\n  uint64_t mask = len < 64 ? (1ull<<len)-1\
    \ : 0xFFFFFFFFFFFFFFFFull;\n  return (x >> start) & mask;\n}\n// 00101101 -> 00111111\
    \ -count_1s-> 6\ninline constexpr unsigned log2p1(uint8_t x) {\n  if (x & 0x80)\n\
    \    return 8;\n  uint64_t p = uint64_t(x) * 0x0101010101010101ull;\n  p -= 0x8040201008040201ull;\n\
    \  p = ~p & 0x8080808080808080ull;\n  p = (p >> 7) * 0x0101010101010101ull;\n\
    \  p >>= 56;\n  return p;\n}\n// 00101100 -mask_mssb-> 00100000 -to_index-> 5\n\
    inline constexpr unsigned mssb8(uint8_t x) {\n  assert(x != 0);\n  return log2p1(x)\
    \ - 1;\n}\n// 00101100 -mask_lssb-> 00000100 -to_index-> 2\ninline constexpr unsigned\
    \ lssb8(uint8_t x) {\n  assert(x != 0);\n  return popcnt_e8((x & -x) - 1);\n}\n\
    // Count leading 0s. 00001011... -> 4\ninline constexpr unsigned clz(uint64_t\
    \ x) {\n  if (x == 0)\n    return 64;\n  auto i = mssb8(summary(x));\n  auto j\
    \ = mssb8(bextr(x, 8 * i, 8));\n  return 63 - (8 * i + j);\n}\ninline constexpr\
    \ unsigned clz8(uint8_t x) {\n  return x == 0 ? 8 : 7 - mssb8(x);\n}\ninline constexpr\
    \ uint64_t bit_reverse(uint64_t x) {\n  x = ((x & 0x00000000FFFFFFFF) << 32) |\
    \ ((x & 0xFFFFFFFF00000000) >> 32);\n  x = ((x & 0x0000FFFF0000FFFF) << 16) |\
    \ ((x & 0xFFFF0000FFFF0000) >> 16);\n  x = ((x & 0x00FF00FF00FF00FF) << 8) | ((x\
    \ & 0xFF00FF00FF00FF00) >> 8);\n  x = ((x & 0x0F0F0F0F0F0F0F0F) << 4) | ((x &\
    \ 0xF0F0F0F0F0F0F0F0) >> 4);\n  x = ((x & 0x3333333333333333) << 2) | ((x & 0xCCCCCCCCCCCCCCCC)\
    \ >> 2);\n  x = ((x & 0x5555555555555555) << 1) | ((x & 0xAAAAAAAAAAAAAAAA) >>\
    \ 1);\n  return x;\n}\n\n} // namespace bm\n#line 5 \"include/mtl/modular.hpp\"\
    \n\ntemplate <int MOD>\nclass Modular {\n private:\n  unsigned int val_;\n\n public:\n\
    \  static constexpr unsigned int mod() { return MOD; }\n  template<class T>\n\
    \  static constexpr unsigned int safe_mod(T v) {\n    auto x = (long long)(v%(long\
    \ long)mod());\n    if (x < 0) x += mod();\n    return (unsigned int) x;\n  }\n\
    \n  constexpr Modular() : val_(0) {}\n  template<class T,\n      std::enable_if_t<\n\
    \          std::is_integral<T>::value && std::is_unsigned<T>::value\n      > *\
    \ = nullptr>\n  constexpr Modular(T v) : val_(v%mod()) {}\n  template<class T,\n\
    \      std::enable_if_t<\n          std::is_integral<T>::value && !std::is_unsigned<T>::value\n\
    \      > * = nullptr>\n  constexpr Modular(T v) : val_(safe_mod(v)) {}\n\n  constexpr\
    \ unsigned int val() const { return val_; }\n  constexpr Modular& operator+=(Modular\
    \ x) {\n    val_ += x.val();\n    if (val_ >= mod()) val_ -= mod();\n    return\
    \ *this;\n  }\n  constexpr Modular operator-() const { return {mod() - val_};\
    \ }\n  constexpr Modular& operator-=(Modular x) {\n    val_ += mod() - x.val();\n\
    \    if (val_ >= mod()) val_ -= mod();\n    return *this;\n  }\n  constexpr Modular&\
    \ operator*=(Modular x) {\n    auto v = (long long) val_ * x.val();\n    if (v\
    \ >= mod()) v %= mod();\n    val_ = v;\n    return *this;\n  }\n  constexpr Modular\
    \ pow(long long p) const {\n    assert(p >= 0);\n    Modular t = 1;\n    Modular\
    \ u = *this;\n    while (p) {\n      if (p & 1)\n        t *= u;\n      u *= u;\n\
    \      p >>= 1;\n    }\n    return t;\n  }\n  friend constexpr Modular pow(Modular\
    \ x, long long p) {\n    return x.pow(p);\n  }\n  constexpr Modular inv() const\
    \ { return pow(mod()-2); }\n  constexpr Modular& operator/=(Modular x) { return\
    \ *this *= x.inv(); }\n  constexpr Modular operator+(Modular x) const { return\
    \ Modular(*this) += x; }\n  constexpr Modular operator-(Modular x) const { return\
    \ Modular(*this) -= x; }\n  constexpr Modular operator*(Modular x) const { return\
    \ Modular(*this) *= x; }\n  constexpr Modular operator/(Modular x) const { return\
    \ Modular(*this) /= x; }\n  constexpr Modular& operator++() { return *this +=\
    \ 1; }\n  constexpr Modular operator++(int) { Modular c = *this; ++(*this); return\
    \ c; }\n  constexpr Modular& operator--() { return *this -= 1; }\n  constexpr\
    \ Modular operator--(int) { Modular c = *this; --(*this); return c; }\n\n  constexpr\
    \ bool operator==(Modular x) const { return val() == x.val(); }\n  constexpr bool\
    \ operator!=(Modular x) const { return val() != x.val(); }\n\n  constexpr bool\
    \ is_square() const {\n    return pow((mod()-1)/2) == 1;\n  }\n  /**\n   * Return\
    \ x s.t. x * x = a mod p\n   * reference: https://zenn.dev/peria/articles/c6afc72b6b003c\n\
    \  */\n  constexpr Modular sqrt() const {\n    if (!is_square()) \n      throw\
    \ std::runtime_error(\"not square\");\n    auto mod_eight = mod() % 8;\n    if\
    \ (mod_eight == 3 || mod_eight == 7) {\n      return pow((mod()+1)/4);\n    }\
    \ else if (mod_eight == 5) {\n      auto x = pow((mod()+3)/8);\n      if (x *\
    \ x != *this)\n        x *= Modular(2).pow((mod()-1)/4);\n      return x;\n  \
    \  } else {\n      Modular d = 2;\n      while (d.is_square())\n        d += 1;\n\
    \      auto t = mod()-1;\n      int s = bm::ctz(t);\n      t >>= s;\n      auto\
    \ a = pow(t);\n      auto D = d.pow(t);\n      int m = 0;\n      Modular dt =\
    \ 1;\n      Modular du = D;\n      for (int i = 0; i < s; i++) {\n        if ((a*dt).pow(1u<<(s-1-i))\
    \ == -1) {\n          m |= 1u << i;\n          dt *= du;\n        }\n        du\
    \ *= du;\n      }\n      return pow((t+1)/2) * D.pow(m/2);\n    }\n  }\n\n  friend\
    \ std::ostream& operator<<(std::ostream& os, const Modular& x) {\n    return os\
    \ << x.val();\n  }\n  friend std::istream& operator>>(std::istream& is, Modular&\
    \ x) {\n    return is >> x.val_;\n  }\n\n};\n\nusing Modular998244353 = Modular<998244353>;\n\
    using Modular1000000007 = Modular<(int)1e9+7>;\n\ntemplate<int Id=0>\nclass DynamicModular\
    \ {\n private:\n  static unsigned int mod_;\n  unsigned int val_;\n\n public:\n\
    \  static unsigned int mod() { return mod_; }\n  static void set_mod(unsigned\
    \ int m) { mod_ = m; }\n  template<class T>\n  static constexpr unsigned int safe_mod(T\
    \ v) {\n    auto x = (long long)(v%(long long)mod());\n    if (x < 0) x += mod();\n\
    \    return (unsigned int) x;\n  }\n\n  constexpr DynamicModular() : val_(0) {}\n\
    \  template<class T,\n      std::enable_if_t<\n          std::is_integral<T>::value\
    \ && std::is_unsigned<T>::value\n      > * = nullptr>\n  constexpr DynamicModular(T\
    \ v) : val_(v%mod()) {}\n  template<class T,\n      std::enable_if_t<\n      \
    \    std::is_integral<T>::value && !std::is_unsigned<T>::value\n      > * = nullptr>\n\
    \  constexpr DynamicModular(T v) : val_(safe_mod(v)) {}\n\n  constexpr unsigned\
    \ int val() const { return val_; }\n  constexpr DynamicModular& operator+=(DynamicModular\
    \ x) {\n    val_ += x.val();\n    if (val_ >= mod()) val_ -= mod();\n    return\
    \ *this;\n  }\n  constexpr DynamicModular operator-() const { return {mod() -\
    \ val_}; }\n  constexpr DynamicModular& operator-=(DynamicModular x) {\n    val_\
    \ += mod() - x.val();\n    if (val_ >= mod()) val_ -= mod();\n    return *this;\n\
    \  }\n  constexpr DynamicModular& operator*=(DynamicModular x) {\n    auto v =\
    \ (long long) val_ * x.val();\n    if (v >= mod()) v %= mod();\n    val_ = v;\n\
    \    return *this;\n  }\n  constexpr DynamicModular pow(long long p) const {\n\
    \    assert(p >= 0);\n    DynamicModular t = 1;\n    DynamicModular u = *this;\n\
    \    while (p) {\n      if (p & 1)\n        t *= u;\n      u *= u;\n      p >>=\
    \ 1;\n    }\n    return t;\n  }\n  friend constexpr DynamicModular pow(DynamicModular\
    \ x, long long p) {\n    return x.pow(p);\n  }\n  // TODO: implement when mod\
    \ is not prime\n  constexpr DynamicModular inv() const { return pow(mod()-2);\
    \ }\n  constexpr DynamicModular& operator/=(DynamicModular x) { return *this *=\
    \ x.inv(); }\n  constexpr DynamicModular operator+(DynamicModular x) const { return\
    \ DynamicModular(*this) += x; }\n  constexpr DynamicModular operator-(DynamicModular\
    \ x) const { return DynamicModular(*this) -= x; }\n  constexpr DynamicModular\
    \ operator*(DynamicModular x) const { return DynamicModular(*this) *= x; }\n \
    \ constexpr DynamicModular operator/(DynamicModular x) const { return DynamicModular(*this)\
    \ /= x; }\n  constexpr DynamicModular& operator++() { return *this += 1; }\n \
    \ constexpr DynamicModular operator++(int) { DynamicModular c = *this; ++(*this);\
    \ return c; }\n  constexpr DynamicModular& operator--() { return *this -= 1; }\n\
    \  constexpr DynamicModular operator--(int) { DynamicModular c = *this; --(*this);\
    \ return c; }\n\n  constexpr bool operator==(DynamicModular x) const { return\
    \ val() == x.val(); }\n  constexpr bool operator!=(DynamicModular x) const { return\
    \ val() != x.val(); }\n\n  constexpr bool is_square() const {\n    return val()\
    \ == 0 or pow((mod()-1)/2) == 1;\n  }\n  /**\n   * Return x s.t. x * x = a mod\
    \ p\n   * reference: https://zenn.dev/peria/articles/c6afc72b6b003c\n  */\n  constexpr\
    \ DynamicModular sqrt() const {\n    // assert mod is prime\n    if (!is_square())\
    \ \n      throw std::runtime_error(\"not square\");\n    if (val() < 2)\n    \
    \  return val();\n    auto mod_eight = mod() % 8;\n    if (mod_eight == 3 || mod_eight\
    \ == 7) {\n      return pow((mod()+1)/4);\n    } else if (mod_eight == 5) {\n\
    \      auto x = pow((mod()+3)/8);\n      if (x * x != *this)\n        x *= DynamicModular(2).pow((mod()-1)/4);\n\
    \      return x;\n    } else {\n      DynamicModular d = 2;\n      while (d.is_square())\n\
    \        ++d;\n      auto t = mod()-1;\n      int s = bm::ctz(t);\n      t >>=\
    \ s;\n      auto a = pow(t);\n      auto D = d.pow(t);\n      int m = 0;\n   \
    \   DynamicModular dt = 1;\n      DynamicModular du = D;\n      for (int i = 0;\
    \ i < s; i++) {\n        if ((a*dt).pow(1u<<(s-1-i)) == -1) {\n          m |=\
    \ 1u << i;\n          dt *= du;\n        }\n        du *= du;\n      }\n     \
    \ return pow((t+1)/2) * D.pow(m/2);\n    }\n  }\n\n  friend std::ostream& operator<<(std::ostream&\
    \ os, const DynamicModular& x) {\n    return os << x.val();\n  }\n  friend std::istream&\
    \ operator>>(std::istream& is, DynamicModular& x) {\n    return is >> x.val_;\n\
    \  }\n\n};\ntemplate<int Id>\nunsigned int DynamicModular<Id>::mod_;\n\n#line\
    \ 264 \"include/mtl/modular.hpp\"\n\ntemplate<class ModInt>\nstruct ModularUtil\
    \ {\n  static constexpr int mod = ModInt::mod();\n  static struct inv_table {\n\
    \    std::vector<ModInt> tb{0,1};\n    inv_table() : tb({0,1}) {}\n  } inv_;\n\
    \  void set_inv(int n) {\n    int m = inv_.tb.size();\n    if (m > n) return;\n\
    \    inv_.tb.resize(n+1);\n    for (int i = m; i < n+1; i++)\n      inv_.tb[i]\
    \ = -inv_.tb[mod % i] * (mod / i);\n  }\n  ModInt& inv(int i) {\n    set_inv(i);\n\
    \    return inv_.tb[i];\n  }\n};\ntemplate<class ModInt>\ntypename ModularUtil<ModInt>::inv_table\
    \ ModularUtil<ModInt>::inv_;\n\n#line 288 \"include/mtl/modular.hpp\"\n\nnamespace\
    \ math {\n\nconstexpr int mod_pow_constexpr(int x, int p, int m) {\n  long long\
    \ t = 1;\n  long long u = x;\n  while (p) {\n    if (p & 1) {\n      t *= u;\n\
    \      t %= m;\n    }\n    u *= u;\n    u %= m;\n    p >>= 1;\n  }\n  return (int)\
    \ t;\n}\n\nconstexpr int primitive_root_constexpr(int m) {\n  if (m == 2) return\
    \ 1;\n  if (m == 167772161) return 3;\n  if (m == 469762049) return 3;\n  if (m\
    \ == 754974721) return 11;\n  if (m == 880803841) return 26;\n  if (m == 998244353)\
    \ return 3;\n\n  std::array<int, 20> divs{};\n  int cnt = 0;\n  int x = m-1;\n\
    \  if (x % 2 == 0) {\n    divs[cnt++] = 2;\n    x >>= bm::ctz(x);\n  }\n  for\
    \ (int d = 3; d*d <= x; d += 2) {\n    if (x % d == 0) {\n      divs[cnt++] =\
    \ d;\n      while (x % d == 0)\n        x /= d;\n    }\n  }\n  if (x > 1) divs[cnt++]\
    \ = x;\n  for (int g = 2; g < m; g++) {\n    bool ok = true;\n    for (int i =\
    \ 0; i < cnt; i++) {\n      if (mod_pow_constexpr(g, (m-1) / divs[i], m) == 1)\
    \ {\n        ok = false;\n        break;\n      }\n    }\n    if (ok) return g;\n\
    \  }\n  return -1;\n}\n\ntemplate<int m>\nconstexpr int primitive_root = primitive_root_constexpr(m);\n\
    \n}\n#line 4 \"test/yosupo/matrix/system_of_linear_equations.test.cpp\"\n#include\
    \ <bits/stdc++.h>\nusing namespace std;\nusing mint = Modular998244353;\n\nint\
    \ main() {\n    int n,m; cin>>n>>m;\n    Matrix<mint,500,500> A;\n    for (int\
    \ i = 0; i < n; i++) for (int j = 0; j < m; j++) {\n        cin>>A[i][j];\n  \
    \  }\n    Matrix<mint,500,1> b;\n    for (int i = 0; i < n; i++) {\n        cin>>b[i][0];\n\
    \    }\n    auto [rank,ans_basis,suc] = A.solve(b, n, m);\n    if (!suc) {\n \
    \       cout << -1 << endl;\n        return 0;\n    }\n    cout << rank << endl;\n\
    \    for (int i = 0; i < m; i++) {\n        cout << ans_basis[0][i] << ' ';\n\
    \    }\n    cout << endl;\n    for (int i = 0; i < (int)rank; i++) {\n       \
    \ for (int j = 0; j < m; j++) {\n            cout << ans_basis[i+1][j] << ' ';\n\
    \        }\n        cout << endl;\n    }\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/system_of_linear_equations\"\
    \n#include \"../../../include/mtl/matrix.hpp\"\n#include \"../../../include/mtl/modular.hpp\"\
    \n#include <bits/stdc++.h>\nusing namespace std;\nusing mint = Modular998244353;\n\
    \nint main() {\n    int n,m; cin>>n>>m;\n    Matrix<mint,500,500> A;\n    for\
    \ (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {\n        cin>>A[i][j];\n\
    \    }\n    Matrix<mint,500,1> b;\n    for (int i = 0; i < n; i++) {\n       \
    \ cin>>b[i][0];\n    }\n    auto [rank,ans_basis,suc] = A.solve(b, n, m);\n  \
    \  if (!suc) {\n        cout << -1 << endl;\n        return 0;\n    }\n    cout\
    \ << rank << endl;\n    for (int i = 0; i < m; i++) {\n        cout << ans_basis[0][i]\
    \ << ' ';\n    }\n    cout << endl;\n    for (int i = 0; i < (int)rank; i++) {\n\
    \        for (int j = 0; j < m; j++) {\n            cout << ans_basis[i+1][j]\
    \ << ' ';\n        }\n        cout << endl;\n    }\n}"
  dependsOn:
  - include/mtl/matrix.hpp
  - include/mtl/modular.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: true
  path: test/yosupo/matrix/system_of_linear_equations.test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: test/yosupo/matrix/system_of_linear_equations.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/matrix/system_of_linear_equations.test.cpp
- /verify/test/yosupo/matrix/system_of_linear_equations.test.cpp.html
title: test/yosupo/matrix/system_of_linear_equations.test.cpp
---
