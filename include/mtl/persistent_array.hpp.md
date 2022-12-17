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
  bundledCode: "#line 2 \"include/mtl/persistent_array.hpp\"\n#include <memory>\r\n\
    #include <utility>\r\n#include <array>\r\n\r\ntemplate<typename T, unsigned Degree=4>\r\
    \nclass PersistentArray {\r\n  static constexpr unsigned M = Degree;\r\n private:\r\
    \n  struct Node;\r\n  using node_ptr = std::shared_ptr<Node>;\r\n  struct Node\
    \ {\r\n    T v;\r\n    std::array<node_ptr, M> ch;\r\n    Node() = default;\r\n\
    \    explicit Node(const T& v) : v(v) {}\r\n    explicit Node(T&& v) : v(std::move(v))\
    \ {}\r\n  };\r\n  node_ptr root_;\r\n  T init_v_;\r\n  explicit PersistentArray(node_ptr\
    \ ptr, T init_v)\r\n    : root_(ptr), init_v_(init_v) {}\r\n\r\n public:\r\n \
    \ explicit PersistentArray(T init_v = T())\r\n    : root_(nullptr), init_v_(init_v)\
    \ {}\r\n\r\n  bool empty() const { return root_ == nullptr; }\r\n  T get(size_t\
    \ i) const {\r\n    auto u = root_;\r\n    while (u and i > 0) {\r\n      i--;\r\
    \n      u = u->ch[i % M];\r\n      i /= M;\r\n    }\r\n    return u ? u->v : init_v_;\r\
    \n  }\r\n\r\n private:\r\n  template<typename V>\r\n  node_ptr set_rec(node_ptr\
    \ u, size_t id, V&& v) const {\r\n    if (id == 0) {\r\n      if (u) {\r\n   \
    \     auto new_node = std::make_shared<Node>(*u);\r\n        new_node->v = std::forward<V>(v);\r\
    \n        return new_node;\r\n      } else {\r\n        return std::make_shared<Node>(std::forward<V>(v));\r\
    \n      }\r\n    } else {\r\n      auto new_node = u ? std::make_shared<Node>(*u)\
    \ : std::make_shared<Node>(init_v_);\r\n      --id;\r\n      new_node->ch[id %\
    \ M] = set_rec(new_node->ch[id % M], id / M, std::forward<V>(v));\r\n      return\
    \ new_node;\r\n    }\r\n  }\r\n public:\r\n  template<typename V>\r\n  [[nodiscard]]\
    \ PersistentArray set(size_t i, V&& v) const {\r\n    static_assert(std::is_convertible<V,\
    \ T>::value, \"\");\r\n    return PersistentArray(set_rec(root_, i, std::forward<V>(v)),\
    \ init_v_);\r\n  }\r\n  [[nodiscard]] PersistentArray set(size_t i, const T& v)\
    \ const {\r\n    return PersistentArray(set_rec(root_, i, v), init_v_);\r\n  }\r\
    \n  [[nodiscard]] PersistentArray set(size_t i, T&& v) const {\r\n    return PersistentArray(set_rec(root_,\
    \ i, std::move(v)), init_v_);\r\n  }\r\n};\r\n"
  code: "#pragma once\r\n#include <memory>\r\n#include <utility>\r\n#include <array>\r\
    \n\r\ntemplate<typename T, unsigned Degree=4>\r\nclass PersistentArray {\r\n \
    \ static constexpr unsigned M = Degree;\r\n private:\r\n  struct Node;\r\n  using\
    \ node_ptr = std::shared_ptr<Node>;\r\n  struct Node {\r\n    T v;\r\n    std::array<node_ptr,\
    \ M> ch;\r\n    Node() = default;\r\n    explicit Node(const T& v) : v(v) {}\r\
    \n    explicit Node(T&& v) : v(std::move(v)) {}\r\n  };\r\n  node_ptr root_;\r\
    \n  T init_v_;\r\n  explicit PersistentArray(node_ptr ptr, T init_v)\r\n    :\
    \ root_(ptr), init_v_(init_v) {}\r\n\r\n public:\r\n  explicit PersistentArray(T\
    \ init_v = T())\r\n    : root_(nullptr), init_v_(init_v) {}\r\n\r\n  bool empty()\
    \ const { return root_ == nullptr; }\r\n  T get(size_t i) const {\r\n    auto\
    \ u = root_;\r\n    while (u and i > 0) {\r\n      i--;\r\n      u = u->ch[i %\
    \ M];\r\n      i /= M;\r\n    }\r\n    return u ? u->v : init_v_;\r\n  }\r\n\r\
    \n private:\r\n  template<typename V>\r\n  node_ptr set_rec(node_ptr u, size_t\
    \ id, V&& v) const {\r\n    if (id == 0) {\r\n      if (u) {\r\n        auto new_node\
    \ = std::make_shared<Node>(*u);\r\n        new_node->v = std::forward<V>(v);\r\
    \n        return new_node;\r\n      } else {\r\n        return std::make_shared<Node>(std::forward<V>(v));\r\
    \n      }\r\n    } else {\r\n      auto new_node = u ? std::make_shared<Node>(*u)\
    \ : std::make_shared<Node>(init_v_);\r\n      --id;\r\n      new_node->ch[id %\
    \ M] = set_rec(new_node->ch[id % M], id / M, std::forward<V>(v));\r\n      return\
    \ new_node;\r\n    }\r\n  }\r\n public:\r\n  template<typename V>\r\n  [[nodiscard]]\
    \ PersistentArray set(size_t i, V&& v) const {\r\n    static_assert(std::is_convertible<V,\
    \ T>::value, \"\");\r\n    return PersistentArray(set_rec(root_, i, std::forward<V>(v)),\
    \ init_v_);\r\n  }\r\n  [[nodiscard]] PersistentArray set(size_t i, const T& v)\
    \ const {\r\n    return PersistentArray(set_rec(root_, i, v), init_v_);\r\n  }\r\
    \n  [[nodiscard]] PersistentArray set(size_t i, T&& v) const {\r\n    return PersistentArray(set_rec(root_,\
    \ i, std::move(v)), init_v_);\r\n  }\r\n};\r\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/persistent_array.hpp
  requiredBy: []
  timestamp: '2022-12-11 23:27:51+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/persistent_array.hpp
layout: document
redirect_from:
- /library/include/mtl/persistent_array.hpp
- /library/include/mtl/persistent_array.hpp.html
title: include/mtl/persistent_array.hpp
---
