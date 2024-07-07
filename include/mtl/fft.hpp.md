---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':question:'
    path: include/mtl/convolution.hpp
    title: include/mtl/convolution.hpp
  - icon: ':x:'
    path: include/mtl/fps.hpp
    title: include/mtl/fps.hpp
  - icon: ':x:'
    path: include/mtl/fps/exp.hpp
    title: include/mtl/fps/exp.hpp
  - icon: ':x:'
    path: include/mtl/fps/pow.hpp
    title: include/mtl/fps/pow.hpp
  - icon: ':x:'
    path: include/mtl/sparse_fps.hpp
    title: include/mtl/sparse_fps.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/yosupo/convolution.test.cpp
    title: test/yosupo/convolution.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/convolution_garner.test.cpp
    title: test/yosupo/convolution_garner.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/convolution_large.test.cpp
    title: test/yosupo/convolution_large.test.cpp
  - icon: ':x:'
    path: test/yosupo/polynomial/exp_of_formal_power_series.test.cpp
    title: test/yosupo/polynomial/exp_of_formal_power_series.test.cpp
  - icon: ':x:'
    path: test/yosupo/polynomial/exp_of_formal_power_series_sparse.test.cpp
    title: test/yosupo/polynomial/exp_of_formal_power_series_sparse.test.cpp
  - icon: ':x:'
    path: test/yosupo/polynomial/inv_of_formal_power_series.test.cpp
    title: test/yosupo/polynomial/inv_of_formal_power_series.test.cpp
  - icon: ':x:'
    path: test/yosupo/polynomial/inv_of_formal_power_series_sparse.test.cpp
    title: test/yosupo/polynomial/inv_of_formal_power_series_sparse.test.cpp
  - icon: ':x:'
    path: test/yosupo/polynomial/log_of_formal_power_series.test.cpp
    title: test/yosupo/polynomial/log_of_formal_power_series.test.cpp
  - icon: ':x:'
    path: test/yosupo/polynomial/log_of_formal_power_series_sparse.test.cpp
    title: test/yosupo/polynomial/log_of_formal_power_series_sparse.test.cpp
  - icon: ':x:'
    path: test/yosupo/polynomial/pow_of_formal_power_series.test.cpp
    title: test/yosupo/polynomial/pow_of_formal_power_series.test.cpp
  - icon: ':x:'
    path: test/yosupo/polynomial/pow_of_formal_power_series_sparse.test.cpp
    title: test/yosupo/polynomial/pow_of_formal_power_series_sparse.test.cpp
  - icon: ':x:'
    path: test/yosupo/polynomial/sqrt_of_formal_power_series.test.cpp
    title: test/yosupo/polynomial/sqrt_of_formal_power_series.test.cpp
  - icon: ':x:'
    path: test/yosupo/polynomial/sqrt_of_formal_power_series_sparse.test.cpp
    title: test/yosupo/polynomial/sqrt_of_formal_power_series_sparse.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':question:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/fft.hpp\"\n#include <vector>\r\n#include <complex>\r\
    \n\r\nclass Fft {\r\n public:\r\n  using complex_t = std::complex<double>;\r\n\
    \  using value_type = complex_t;\r\n private:\r\n  size_t n_;\r\n\r\n public:\r\
    \n  explicit Fft(size_t size) {\r\n    n_ = 1;\r\n    while (n_ < size)\r\n  \
    \    n_ *= 2;\r\n  };\r\n\r\n  size_t n() const { return n_; }\r\n\r\n  void fft_inline(std::vector<complex_t>&\
    \ f) const {\r\n    _fft(f);\r\n  }\r\n\r\n  std::vector<complex_t> fft(const\
    \ std::vector<complex_t>& f) const {\r\n    auto ff = f;\r\n    ff.resize(n_,\
    \ 0);\r\n    _fft(ff);\r\n    return ff;\r\n  }\r\n\r\n  void ifft_inline(std::vector<complex_t>&\
    \ f) const {\r\n    _ifft(f);\r\n  }\r\n\r\n  std::vector<complex_t> ifft(const\
    \ std::vector<complex_t>& f) const {\r\n    auto ff = f;\r\n    ff.resize(n_,\
    \ 0);\r\n    _ifft(ff);\r\n    return ff;\r\n  }\r\n\r\n private:\r\n  template\
    \ <bool Forward>\r\n  void _fft_impl(std::vector<complex_t>& f) const {\r\n  \
    \  // iterative bit reversal\r\n    for (size_t i = 0, j = 1; j < n_-1; j++) {\r\
    \n      for (size_t k = n_ >> 1; k > (i ^= k); k >>= 1);\r\n      if (i < j) std::swap(f[i],\
    \ f[j]);\r\n    }\r\n    // Cooley-Tukey FFT\r\n    for (size_t m = 1; m < n_;\
    \ m *= 2) {\r\n      double theta0 = Forward ? M_PI/m : -M_PI/m;\r\n      for\
    \ (size_t chunk = 0; chunk < n_; chunk += 2*m) {\r\n        for (size_t i = 0;\
    \ i < m; i++) {\r\n          complex_t w = {cos(theta0*i), -sin(theta0*i)};\r\n\
    \          auto p = chunk + i;\r\n          auto a = f[p + 0];\r\n          auto\
    \ b = f[p + m] * w;\r\n          f[p + 0] = a + b;\r\n          f[p + m] = a -\
    \ b;\r\n        }\r\n      }\r\n    }\r\n  }\r\n\r\n  void _fft(std::vector<complex_t>&\
    \ f) const {\r\n    _fft_impl<true>(f);\r\n  }\r\n  void _ifft(std::vector<complex_t>&\
    \ f) const {\r\n    _fft_impl<false>(f);\r\n    for (auto& x : f) {\r\n      x\
    \ /= n_;\r\n    }\r\n  }\r\n\r\n};\r\n\r\n"
  code: "#pragma once\r\n#include <vector>\r\n#include <complex>\r\n\r\nclass Fft\
    \ {\r\n public:\r\n  using complex_t = std::complex<double>;\r\n  using value_type\
    \ = complex_t;\r\n private:\r\n  size_t n_;\r\n\r\n public:\r\n  explicit Fft(size_t\
    \ size) {\r\n    n_ = 1;\r\n    while (n_ < size)\r\n      n_ *= 2;\r\n  };\r\n\
    \r\n  size_t n() const { return n_; }\r\n\r\n  void fft_inline(std::vector<complex_t>&\
    \ f) const {\r\n    _fft(f);\r\n  }\r\n\r\n  std::vector<complex_t> fft(const\
    \ std::vector<complex_t>& f) const {\r\n    auto ff = f;\r\n    ff.resize(n_,\
    \ 0);\r\n    _fft(ff);\r\n    return ff;\r\n  }\r\n\r\n  void ifft_inline(std::vector<complex_t>&\
    \ f) const {\r\n    _ifft(f);\r\n  }\r\n\r\n  std::vector<complex_t> ifft(const\
    \ std::vector<complex_t>& f) const {\r\n    auto ff = f;\r\n    ff.resize(n_,\
    \ 0);\r\n    _ifft(ff);\r\n    return ff;\r\n  }\r\n\r\n private:\r\n  template\
    \ <bool Forward>\r\n  void _fft_impl(std::vector<complex_t>& f) const {\r\n  \
    \  // iterative bit reversal\r\n    for (size_t i = 0, j = 1; j < n_-1; j++) {\r\
    \n      for (size_t k = n_ >> 1; k > (i ^= k); k >>= 1);\r\n      if (i < j) std::swap(f[i],\
    \ f[j]);\r\n    }\r\n    // Cooley-Tukey FFT\r\n    for (size_t m = 1; m < n_;\
    \ m *= 2) {\r\n      double theta0 = Forward ? M_PI/m : -M_PI/m;\r\n      for\
    \ (size_t chunk = 0; chunk < n_; chunk += 2*m) {\r\n        for (size_t i = 0;\
    \ i < m; i++) {\r\n          complex_t w = {cos(theta0*i), -sin(theta0*i)};\r\n\
    \          auto p = chunk + i;\r\n          auto a = f[p + 0];\r\n          auto\
    \ b = f[p + m] * w;\r\n          f[p + 0] = a + b;\r\n          f[p + m] = a -\
    \ b;\r\n        }\r\n      }\r\n    }\r\n  }\r\n\r\n  void _fft(std::vector<complex_t>&\
    \ f) const {\r\n    _fft_impl<true>(f);\r\n  }\r\n  void _ifft(std::vector<complex_t>&\
    \ f) const {\r\n    _fft_impl<false>(f);\r\n    for (auto& x : f) {\r\n      x\
    \ /= n_;\r\n    }\r\n  }\r\n\r\n};\r\n\r\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/fft.hpp
  requiredBy:
  - include/mtl/fps.hpp
  - include/mtl/fps/pow.hpp
  - include/mtl/fps/exp.hpp
  - include/mtl/convolution.hpp
  - include/mtl/sparse_fps.hpp
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_SOME_WA
  verifiedWith:
  - test/yosupo/convolution.test.cpp
  - test/yosupo/polynomial/log_of_formal_power_series_sparse.test.cpp
  - test/yosupo/polynomial/inv_of_formal_power_series.test.cpp
  - test/yosupo/polynomial/pow_of_formal_power_series_sparse.test.cpp
  - test/yosupo/polynomial/exp_of_formal_power_series.test.cpp
  - test/yosupo/polynomial/sqrt_of_formal_power_series.test.cpp
  - test/yosupo/polynomial/inv_of_formal_power_series_sparse.test.cpp
  - test/yosupo/polynomial/pow_of_formal_power_series.test.cpp
  - test/yosupo/polynomial/exp_of_formal_power_series_sparse.test.cpp
  - test/yosupo/polynomial/sqrt_of_formal_power_series_sparse.test.cpp
  - test/yosupo/polynomial/log_of_formal_power_series.test.cpp
  - test/yosupo/convolution_large.test.cpp
  - test/yosupo/convolution_garner.test.cpp
documentation_of: include/mtl/fft.hpp
layout: document
redirect_from:
- /library/include/mtl/fft.hpp
- /library/include/mtl/fft.hpp.html
title: include/mtl/fft.hpp
---
