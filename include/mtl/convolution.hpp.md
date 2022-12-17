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
  bundledCode: "#line 2 \"include/mtl/convolution.hpp\"\n#include <vector>\n\ntemplate\
    \ <typename Transformer>\nclass Convolution {\n public:\n  using Polynomial =\
    \ std::vector<typename Transformer::value_type>;\n private:\n  Transformer tf_;\n\
    \ public:\n  explicit Convolution(size_t size) : tf_(size) {}\n\n  Polynomial\
    \ operator()(const Polynomial& g, const Polynomial& h) const {\n    auto fg =\
    \ tf_.fft(g);\n    auto fh = tf_.fft(h);\n    for (size_t i = 0; i < tf_.n();\
    \ i++) {\n      fg[i] *= fh[i];\n    }\n    tf_.ifft_inline(fg);\n    auto n_inv\
    \ = (typename Polynomial::value_type)(1) / tf_.n();\n    for (size_t i = 0; i\
    \ < tf_.n(); i++)\n      fg[i] *= n_inv;\n    return fg;\n  }\n\n};\n\n"
  code: "#pragma once\n#include <vector>\n\ntemplate <typename Transformer>\nclass\
    \ Convolution {\n public:\n  using Polynomial = std::vector<typename Transformer::value_type>;\n\
    \ private:\n  Transformer tf_;\n public:\n  explicit Convolution(size_t size)\
    \ : tf_(size) {}\n\n  Polynomial operator()(const Polynomial& g, const Polynomial&\
    \ h) const {\n    auto fg = tf_.fft(g);\n    auto fh = tf_.fft(h);\n    for (size_t\
    \ i = 0; i < tf_.n(); i++) {\n      fg[i] *= fh[i];\n    }\n    tf_.ifft_inline(fg);\n\
    \    auto n_inv = (typename Polynomial::value_type)(1) / tf_.n();\n    for (size_t\
    \ i = 0; i < tf_.n(); i++)\n      fg[i] *= n_inv;\n    return fg;\n  }\n\n};\n\
    \n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/convolution.hpp
  requiredBy: []
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/convolution.hpp
layout: document
redirect_from:
- /library/include/mtl/convolution.hpp
- /library/include/mtl/convolution.hpp.html
title: include/mtl/convolution.hpp
---
