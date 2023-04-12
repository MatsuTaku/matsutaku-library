---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    document_title: Integer Vector store ElementSize bits per element
    links: []
  bundledCode: "#line 2 \"include/mtl/succinct/int_vector.hpp\"\n#include <cstdint>\n\
    #include <vector>\n#include <cstddef>\n#include <cassert>\n\n/**\n * @brief Integer\
    \ Vector store ElementSize bits per element\n * @param ElementSize number of bits\
    \ per element\n*/\ntemplate<unsigned ElementSize>\nstruct IntVector {\n    using\
    \ value_type = uint64_t;\n    using element_type = uint64_t;\n    static constexpr\
    \ unsigned element_size = ElementSize;\n    static constexpr unsigned word_size\
    \ = 64;\n    std::vector<element_type> arr;\n    size_t size_;\n    IntVector(size_t\
    \ n = 0) : size_(n), arr(needs_size(n)) {}\n    size_t needs_size(size_t needs)\
    \ const {\n        return (needs * element_size + word_size - 1) / word_size;\n\
    \    }\n    size_t container_size() const {\n        return arr.size() * word_size\
    \ / element_size;\n    }\n    size_t size() const { return size_; }\n    bool\
    \ empty() const { return size() == 0; }\n    void resize(size_t new_size) {\n\
    \        size_ = new_size;\n        arr.resize(needs_size(new_size));\n    }\n\
    \    void clear() { resize(0); }\n    void set(size_t i, const value_type& v)\
    \ {\n        size_t j = i * element_size;\n        size_t k = j / word_size;\n\
    \        size_t l = j % word_size;\n        arr[k] = (arr[k] & ~(value_type(-1)\
    \ << l)) | (v << l);\n        if (l + element_size > word_size) {\n          \
    \  arr[k+1] = (arr[k+1] & (value_type(-1) << (l + element_size - word_size)))\
    \ \n                | (v >> (word_size - l));\n        }\n    }\n    void push_back(const\
    \ value_type& v) {\n        auto i = size();\n        resize(size()+1);\n    \
    \    set(i, v);\n    }\n    value_type get(size_t i) const {\n        size_t j\
    \ = i * element_size;\n        size_t k = j / word_size;\n        size_t l = j\
    \ % word_size;\n        value_type res = arr[k] >> l;\n        if (l + element_size\
    \ > word_size) {\n            res |= (arr[k+1] & ~(value_type(-1) << (l + element_size\
    \ - word_size))) << (word_size - l);\n        }\n        return res;\n    }\n\
    \    value_type front() const { return get(0); }\n    value_type back() const\
    \ { return get(size()-1); }\n};\n"
  code: "#pragma once\n#include <cstdint>\n#include <vector>\n#include <cstddef>\n\
    #include <cassert>\n\n/**\n * @brief Integer Vector store ElementSize bits per\
    \ element\n * @param ElementSize number of bits per element\n*/\ntemplate<unsigned\
    \ ElementSize>\nstruct IntVector {\n    using value_type = uint64_t;\n    using\
    \ element_type = uint64_t;\n    static constexpr unsigned element_size = ElementSize;\n\
    \    static constexpr unsigned word_size = 64;\n    std::vector<element_type>\
    \ arr;\n    size_t size_;\n    IntVector(size_t n = 0) : size_(n), arr(needs_size(n))\
    \ {}\n    size_t needs_size(size_t needs) const {\n        return (needs * element_size\
    \ + word_size - 1) / word_size;\n    }\n    size_t container_size() const {\n\
    \        return arr.size() * word_size / element_size;\n    }\n    size_t size()\
    \ const { return size_; }\n    bool empty() const { return size() == 0; }\n  \
    \  void resize(size_t new_size) {\n        size_ = new_size;\n        arr.resize(needs_size(new_size));\n\
    \    }\n    void clear() { resize(0); }\n    void set(size_t i, const value_type&\
    \ v) {\n        size_t j = i * element_size;\n        size_t k = j / word_size;\n\
    \        size_t l = j % word_size;\n        arr[k] = (arr[k] & ~(value_type(-1)\
    \ << l)) | (v << l);\n        if (l + element_size > word_size) {\n          \
    \  arr[k+1] = (arr[k+1] & (value_type(-1) << (l + element_size - word_size)))\
    \ \n                | (v >> (word_size - l));\n        }\n    }\n    void push_back(const\
    \ value_type& v) {\n        auto i = size();\n        resize(size()+1);\n    \
    \    set(i, v);\n    }\n    value_type get(size_t i) const {\n        size_t j\
    \ = i * element_size;\n        size_t k = j / word_size;\n        size_t l = j\
    \ % word_size;\n        value_type res = arr[k] >> l;\n        if (l + element_size\
    \ > word_size) {\n            res |= (arr[k+1] & ~(value_type(-1) << (l + element_size\
    \ - word_size))) << (word_size - l);\n        }\n        return res;\n    }\n\
    \    value_type front() const { return get(0); }\n    value_type back() const\
    \ { return get(size()-1); }\n};"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/succinct/int_vector.hpp
  requiredBy: []
  timestamp: '2023-04-10 16:54:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/succinct/int_vector.hpp
layout: document
redirect_from:
- /library/include/mtl/succinct/int_vector.hpp
- /library/include/mtl/succinct/int_vector.hpp.html
title: Integer Vector store ElementSize bits per element
---
