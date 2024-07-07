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
  bundledCode: "#line 2 \"include/mtl/persistent_stack.hpp\"\n#include <algorithm>\r\
    \n#include <cstddef>\r\n#include <memory>\r\n\r\ntemplate<typename T>\r\nclass\
    \ PersistentStack {\r\n public:\r\n  using value_type = T;\r\n  using const_reference\
    \ = const value_type&;\r\n  using reference = const_reference;\r\n  struct Node;\r\
    \n  using node_ptr = std::shared_ptr<Node>;\r\n  struct Node {\r\n    value_type\
    \ v;\r\n    node_ptr next;\r\n    Node() = default;\r\n    Node(const value_type&\
    \ v, node_ptr next) : v(v), next(next) {}\r\n    Node(value_type&& v, node_ptr\
    \ next) : v(std::move(v)), next(next) {}\r\n  };\r\n private:\r\n  node_ptr top_;\r\
    \n  size_t size_;\r\n  PersistentStack(node_ptr ptr, size_t sz) : top_(ptr), size_(sz)\
    \ {}\r\n public:\r\n  PersistentStack() : top_(nullptr), size_(0) {}\r\n  size_t\
    \ size() const { return size_; }\r\n  bool empty() const { return size() == 0;\
    \ }\r\n  const_reference top() const { return top_->v; }\r\n private:\r\n  template<typename\
    \ V>\r\n  PersistentStack _push(V&& v) const {\r\n    auto new_node = std::make_shared<Node>(std::forward<V>(v),\
    \ top_);\r\n    return PersistentStack(new_node, size()+1);\r\n  }\r\n public:\r\
    \n  template<class... Args>\r\n  [[nodiscard]] PersistentStack emplace(Args&&...\
    \ args) const {\r\n    return _push(value_type(std::forward<Args>(args)...));\r\
    \n  }\r\n  template<typename V>\r\n  [[nodiscard]] PersistentStack push(V&& v)\
    \ const {\r\n    static_assert(std::is_convertible<V,value_type>::value, \"\"\
    );\r\n    return _push(std::forward<V>(v));\r\n  }\r\n  [[nodiscard]] PersistentStack\
    \ push(const value_type& v) const {\r\n    return _push(v);\r\n  }\r\n  [[nodiscard]]\
    \ PersistentStack push(value_type&& v) const {\r\n    return _push(std::move(v));\r\
    \n  }\r\n  [[nodiscard]] PersistentStack pop() const {\r\n    return PersistentStack(top_->next,\
    \ size()-1);\r\n  }\r\n  [[nodiscard]] PersistentStack concat(const PersistentStack&\
    \ other) const {\r\n    if (empty()) \r\n      return other;\r\n    else \r\n\
    \      return pop().concat(other).push(top());\r\n  }\r\n  [[nodiscard]] PersistentStack\
    \ reverse() const {\r\n    PersistentStack ret;\r\n    for (auto t = *this; !t.empty();\
    \ t = t.pop())\r\n      ret = ret.push(t.top());\r\n    return ret;\r\n  }\r\n\
    };\n"
  code: "#pragma once\r\n#include <algorithm>\r\n#include <cstddef>\r\n#include <memory>\r\
    \n\r\ntemplate<typename T>\r\nclass PersistentStack {\r\n public:\r\n  using value_type\
    \ = T;\r\n  using const_reference = const value_type&;\r\n  using reference =\
    \ const_reference;\r\n  struct Node;\r\n  using node_ptr = std::shared_ptr<Node>;\r\
    \n  struct Node {\r\n    value_type v;\r\n    node_ptr next;\r\n    Node() = default;\r\
    \n    Node(const value_type& v, node_ptr next) : v(v), next(next) {}\r\n    Node(value_type&&\
    \ v, node_ptr next) : v(std::move(v)), next(next) {}\r\n  };\r\n private:\r\n\
    \  node_ptr top_;\r\n  size_t size_;\r\n  PersistentStack(node_ptr ptr, size_t\
    \ sz) : top_(ptr), size_(sz) {}\r\n public:\r\n  PersistentStack() : top_(nullptr),\
    \ size_(0) {}\r\n  size_t size() const { return size_; }\r\n  bool empty() const\
    \ { return size() == 0; }\r\n  const_reference top() const { return top_->v; }\r\
    \n private:\r\n  template<typename V>\r\n  PersistentStack _push(V&& v) const\
    \ {\r\n    auto new_node = std::make_shared<Node>(std::forward<V>(v), top_);\r\
    \n    return PersistentStack(new_node, size()+1);\r\n  }\r\n public:\r\n  template<class...\
    \ Args>\r\n  [[nodiscard]] PersistentStack emplace(Args&&... args) const {\r\n\
    \    return _push(value_type(std::forward<Args>(args)...));\r\n  }\r\n  template<typename\
    \ V>\r\n  [[nodiscard]] PersistentStack push(V&& v) const {\r\n    static_assert(std::is_convertible<V,value_type>::value,\
    \ \"\");\r\n    return _push(std::forward<V>(v));\r\n  }\r\n  [[nodiscard]] PersistentStack\
    \ push(const value_type& v) const {\r\n    return _push(v);\r\n  }\r\n  [[nodiscard]]\
    \ PersistentStack push(value_type&& v) const {\r\n    return _push(std::move(v));\r\
    \n  }\r\n  [[nodiscard]] PersistentStack pop() const {\r\n    return PersistentStack(top_->next,\
    \ size()-1);\r\n  }\r\n  [[nodiscard]] PersistentStack concat(const PersistentStack&\
    \ other) const {\r\n    if (empty()) \r\n      return other;\r\n    else \r\n\
    \      return pop().concat(other).push(top());\r\n  }\r\n  [[nodiscard]] PersistentStack\
    \ reverse() const {\r\n    PersistentStack ret;\r\n    for (auto t = *this; !t.empty();\
    \ t = t.pop())\r\n      ret = ret.push(t.top());\r\n    return ret;\r\n  }\r\n\
    };"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/persistent_stack.hpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/persistent_stack.hpp
layout: document
redirect_from:
- /library/include/mtl/persistent_stack.hpp
- /library/include/mtl/persistent_stack.hpp.html
title: include/mtl/persistent_stack.hpp
---
