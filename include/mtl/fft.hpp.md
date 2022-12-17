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
  requiredBy: []
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/fft.hpp
layout: document
redirect_from:
- /library/include/mtl/fft.hpp
- /library/include/mtl/fft.hpp.html
title: include/mtl/fft.hpp
---
