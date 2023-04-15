---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: include/mtl/ordinal_range_search.hpp
    title: Ordinal Range Search
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/binary_set.hpp
    title: include/mtl/succinct/binary_set.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/rrr.hpp
    title: Succinct bit vector in memory of B(n, u) + O(u log log n / log n) bits
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/yosupo/rectangle_sum.test.cpp
    title: test/yosupo/rectangle_sum.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/static_rectangle_add_rectangle_sum.test.cpp
    title: test/yosupo/static_rectangle_add_rectangle_sum.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
    title: test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    document_title: 'TY: Store increasing sequence of integers.'
    links: []
  bundledCode: "#line 2 \"include/mtl/succinct/ty.hpp\"\n#include <vector>\n#include\
    \ <cstdint>\n#include <limits>\n#include <cstddef>\n#include <cassert>\n\n/**\n\
    \ * @brief TY: Store increasing sequence of integers.\n *            Memory needs\
    \ for store nth integers O(n log d) bits \n *            which d is max diff of\
    \ consecutive elements.\n*/\ntemplate<class T, class DiffType = uint16_t>\nstruct\
    \ TY {\n    using value_type = T;\n    static constexpr auto block_size = sizeof(value_type)\
    \ * 8;\n    using diff_value_type = DiffType;\n    static constexpr unsigned max_diff\
    \ = std::numeric_limits<diff_value_type>::max();\n    std::vector<value_type>\
    \ head;\n    std::vector<diff_value_type> diff;\n\n    TY() = default;\n    size_t\
    \ size() const {\n        return head.size() + diff.size();\n    }\n    void reserve(size_t\
    \ n) {\n        head.reserve((n + block_size - 1) / block_size);\n        diff.reserve(n\
    \ / block_size * (block_size - 1) + n % block_size);\n    }\n    bool empty()\
    \ const { return size() == 0; }\n    value_type raw_element(const value_type&\
    \ v) {\n        return v;\n    }\n    value_type diff_element(const value_type&\
    \ v) {\n        return v;\n    }\n    void push_back(const value_type& v) {\n\
    \        assert(head.empty() or size() % block_size == 0 or v - head.back() <=\
    \ (value_type)max_diff);\n        if (size() % block_size == 0) {\n          \
    \  head.push_back(raw_element(v));\n        } else {\n            diff.push_back(diff_element(v\
    \ - head.back()));\n        }\n    }\n    value_type get(size_t i) const {\n \
    \       return (i % block_size == 0) ? \n            head[i / block_size] : \n\
    \            head[i / block_size] + diff[i / block_size * (block_size-1) + i %\
    \ block_size - 1];\n    }\n    value_type operator[](size_t i) const { return\
    \ get(i); }\n    value_type front() const { return get(0); }\n    value_type back()\
    \ const { return get(size()-1); }\n};\n"
  code: "#pragma once\n#include <vector>\n#include <cstdint>\n#include <limits>\n\
    #include <cstddef>\n#include <cassert>\n\n/**\n * @brief TY: Store increasing\
    \ sequence of integers.\n *            Memory needs for store nth integers O(n\
    \ log d) bits \n *            which d is max diff of consecutive elements.\n*/\n\
    template<class T, class DiffType = uint16_t>\nstruct TY {\n    using value_type\
    \ = T;\n    static constexpr auto block_size = sizeof(value_type) * 8;\n    using\
    \ diff_value_type = DiffType;\n    static constexpr unsigned max_diff = std::numeric_limits<diff_value_type>::max();\n\
    \    std::vector<value_type> head;\n    std::vector<diff_value_type> diff;\n\n\
    \    TY() = default;\n    size_t size() const {\n        return head.size() +\
    \ diff.size();\n    }\n    void reserve(size_t n) {\n        head.reserve((n +\
    \ block_size - 1) / block_size);\n        diff.reserve(n / block_size * (block_size\
    \ - 1) + n % block_size);\n    }\n    bool empty() const { return size() == 0;\
    \ }\n    value_type raw_element(const value_type& v) {\n        return v;\n  \
    \  }\n    value_type diff_element(const value_type& v) {\n        return v;\n\
    \    }\n    void push_back(const value_type& v) {\n        assert(head.empty()\
    \ or size() % block_size == 0 or v - head.back() <= (value_type)max_diff);\n \
    \       if (size() % block_size == 0) {\n            head.push_back(raw_element(v));\n\
    \        } else {\n            diff.push_back(diff_element(v - head.back()));\n\
    \        }\n    }\n    value_type get(size_t i) const {\n        return (i % block_size\
    \ == 0) ? \n            head[i / block_size] : \n            head[i / block_size]\
    \ + diff[i / block_size * (block_size-1) + i % block_size - 1];\n    }\n    value_type\
    \ operator[](size_t i) const { return get(i); }\n    value_type front() const\
    \ { return get(0); }\n    value_type back() const { return get(size()-1); }\n\
    };"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/succinct/ty.hpp
  requiredBy:
  - include/mtl/succinct/binary_set.hpp
  - include/mtl/succinct/rrr.hpp
  - include/mtl/ordinal_range_search.hpp
  timestamp: '2023-04-12 01:55:29+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/yosupo/static_rectangle_add_rectangle_sum.test.cpp
  - test/yosupo/rectangle_sum.test.cpp
  - test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
documentation_of: include/mtl/succinct/ty.hpp
layout: document
redirect_from:
- /library/include/mtl/succinct/ty.hpp
- /library/include/mtl/succinct/ty.hpp.html
title: 'TY: Store increasing sequence of integers.'
---
