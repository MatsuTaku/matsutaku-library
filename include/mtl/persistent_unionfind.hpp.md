---
data:
  _extendedDependsOn:
  - icon: ':x:'
    path: include/mtl/persistent_array.hpp
    title: include/mtl/persistent_array.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':x:'
    path: test/yosupo/persistent_unionfind.test.cpp
    title: test/yosupo/persistent_unionfind.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':x:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/persistent_array.hpp\"\n#include <memory>\r\n\
    #include <utility>\r\n#include <array>\r\n\r\ntemplate<typename T, unsigned Degree=4>\r\
    \nclass PersistentArray {\r\n  static constexpr unsigned M = Degree;\r\n private:\r\
    \n  struct Node;\r\n  using node_ptr = std::shared_ptr<Node>;\r\n  struct Node\
    \ {\r\n    T v;\r\n    std::array<node_ptr, M> ch;\r\n    Node() = default;\r\n\
    \    explicit Node(const T& v) : v(v), ch({}) {}\r\n    explicit Node(T&& v) :\
    \ v(std::move(v)), ch({}) {}\r\n  };\r\n  node_ptr root_;\r\n  T init_v_;\r\n\
    \  explicit PersistentArray(node_ptr ptr, T init_v)\r\n    : root_(ptr), init_v_(init_v)\
    \ {}\r\n\r\n public:\r\n  explicit PersistentArray(T init_v = T())\r\n    : root_(nullptr),\
    \ init_v_(init_v) {}\r\n\r\n  bool empty() const { return root_ == nullptr; }\r\
    \n  T get(size_t i) const {\r\n    auto u = root_;\r\n    while (u and i > 0)\
    \ {\r\n      i--;\r\n      u = u->ch[i % M];\r\n      i /= M;\r\n    }\r\n   \
    \ return u ? u->v : init_v_;\r\n  }\r\n\r\n private:\r\n  template<typename V>\r\
    \n  node_ptr set_rec(node_ptr u, size_t id, V&& v) const {\r\n    if (id == 0)\
    \ {\r\n      if (u) {\r\n        auto new_node = std::make_shared<Node>(*u);\r\
    \n        new_node->v = std::forward<V>(v);\r\n        return new_node;\r\n  \
    \    } else {\r\n        return std::make_shared<Node>(std::forward<V>(v));\r\n\
    \      }\r\n    } else {\r\n      auto new_node = u ? std::make_shared<Node>(*u)\
    \ : std::make_shared<Node>(init_v_);\r\n      --id;\r\n      new_node->ch[id %\
    \ M] = set_rec(new_node->ch[id % M], id / M, std::forward<V>(v));\r\n      return\
    \ new_node;\r\n    }\r\n  }\r\n public:\r\n  template<typename V>\r\n  [[nodiscard]]\
    \ PersistentArray set(size_t i, V&& v) const {\r\n    static_assert(std::is_convertible<V,\
    \ T>::value, \"\");\r\n    return PersistentArray(set_rec(root_, i, std::forward<V>(v)),\
    \ init_v_);\r\n  }\r\n  [[nodiscard]] PersistentArray set(size_t i, const T& v)\
    \ const {\r\n    return PersistentArray(set_rec(root_, i, v), init_v_);\r\n  }\r\
    \n  [[nodiscard]] PersistentArray set(size_t i, T&& v) const {\r\n    return PersistentArray(set_rec(root_,\
    \ i, std::move(v)), init_v_);\r\n  }\r\n};\r\n#line 3 \"include/mtl/persistent_unionfind.hpp\"\
    \n#include <cassert>\n\nclass PersistentUnionfind {\nprivate:\n    PersistentArray<long\
    \ long> par_;\npublic:\n    PersistentUnionfind() : par_(-1) {}\n    size_t leader(size_t\
    \ u) {\n        auto pu = par_.get(u);\n        if (pu < 0)\n            return\
    \ u;\n        auto ret = leader(pu);\n        par_ = par_.set(u, ret);\n     \
    \   return ret;\n    }\n    size_t size_of(size_t u) {\n        return -par_.get(leader(u));\n\
    \    }\n    bool same(size_t a, size_t b) {\n        return leader(a) == leader(b);\n\
    \    }\n    [[nodiscard]] PersistentUnionfind merge(size_t a, size_t b) {\n  \
    \      auto ra = leader(a);\n        auto rb = leader(b);\n        if (ra == rb)\n\
    \            return *this;\n        if (size_of(ra) < size_of(rb))\n         \
    \   std::swap(ra, rb);\n        PersistentUnionfind ret = *this;\n        ret.par_\
    \ = ret.par_.set(rb, ra);\n        ret.par_ = ret.par_.set(ra, -(long long)(size_of(ra)\
    \ + size_of(rb)));\n        return ret;\n    }\n};\n"
  code: "#pragma once\n#include \"persistent_array.hpp\"\n#include <cassert>\n\nclass\
    \ PersistentUnionfind {\nprivate:\n    PersistentArray<long long> par_;\npublic:\n\
    \    PersistentUnionfind() : par_(-1) {}\n    size_t leader(size_t u) {\n    \
    \    auto pu = par_.get(u);\n        if (pu < 0)\n            return u;\n    \
    \    auto ret = leader(pu);\n        par_ = par_.set(u, ret);\n        return\
    \ ret;\n    }\n    size_t size_of(size_t u) {\n        return -par_.get(leader(u));\n\
    \    }\n    bool same(size_t a, size_t b) {\n        return leader(a) == leader(b);\n\
    \    }\n    [[nodiscard]] PersistentUnionfind merge(size_t a, size_t b) {\n  \
    \      auto ra = leader(a);\n        auto rb = leader(b);\n        if (ra == rb)\n\
    \            return *this;\n        if (size_of(ra) < size_of(rb))\n         \
    \   std::swap(ra, rb);\n        PersistentUnionfind ret = *this;\n        ret.par_\
    \ = ret.par_.set(rb, ra);\n        ret.par_ = ret.par_.set(ra, -(long long)(size_of(ra)\
    \ + size_of(rb)));\n        return ret;\n    }\n};"
  dependsOn:
  - include/mtl/persistent_array.hpp
  isVerificationFile: false
  path: include/mtl/persistent_unionfind.hpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_ALL_WA
  verifiedWith:
  - test/yosupo/persistent_unionfind.test.cpp
documentation_of: include/mtl/persistent_unionfind.hpp
layout: document
redirect_from:
- /library/include/mtl/persistent_unionfind.hpp
- /library/include/mtl/persistent_unionfind.hpp.html
title: include/mtl/persistent_unionfind.hpp
---
