---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':warning:'
    path: include/mtl/dijkstra.hpp
    title: include/mtl/dijkstra.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/min_cost_flow.hpp
    title: include/mtl/min_cost_flow.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/dijkstra_fibonacci.test.cpp
    title: test/dijkstra_fibonacci.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/min_cost_flow.test.cpp
    title: test/min_cost_flow.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/fibonacci_heap.hpp\"\n#include <memory>\n#include\
    \ <cassert>\n#include <vector>\n#include <list>\n#include <iostream>\n\ntemplate<typename\
    \ T, typename Cond = std::less<>>\nclass FibonacciHeap {\n  Cond cond_;\n\n public:\n\
    \  struct Node;\n  using node_ptr = Node*;\n  using const_node_ptr = const Node*;\n\
    \  struct Node {\n    node_ptr next = nullptr;\n    node_ptr prev = nullptr;\n\
    \    node_ptr child = nullptr;\n    node_ptr parent = nullptr;\n    int deg =\
    \ 0;\n    bool marked = false;\n    bool enabled = false;\n    std::pair<int,\
    \ T> value;\n\n    Node() = default;\n    void init(int k, T v) {\n      next\
    \ = prev = this;\n      child = parent = nullptr;\n      deg = 0;\n      marked\
    \ = false;\n      enabled = true;\n      value = {k, v};\n    }\n    T priority()\
    \ const { return value.second; }\n    void free() {\n      enabled = false;\n\
    \    }\n  };\n\n private:\n  node_ptr root_ = nullptr;\n  size_t sz_ = 0;\n  std::vector<node_ptr>\
    \ map_;\n  std::vector<Node> container_;\n  std::vector<node_ptr> deg_table_;\n\
    \n public:\n  explicit FibonacciHeap(size_t size) : map_(size) {\n    container_.reserve(size);\n\
    \    std::array<size_t,2> tb{1,1};\n    int k = 2;\n    while (tb[1] < size) {\n\
    \      auto x = tb[0]+tb[1];\n      tb[0] = tb[1];\n      tb[1] = x;\n      ++k;\n\
    \    }\n    deg_table_.resize(k);\n  }\n\n  inline std::pair<int, T> top() const\
    \ {\n    assert(root_ and root_->enabled);\n    return root_->value;\n  }\n\n\
    \  inline std::pair<int, T> get(int key) const {\n    assert(map_[key] and map_[key]->enabled);\n\
    \    return map_[key]->value;\n  }\n\n  // Time complexity: O(1)\n  void push(int\
    \ key, T value) {\n    if (map_[key] and map_[key]->enabled) {\n      update(key,\
    \ value);\n      return;\n    }\n    if (!map_[key]) {\n      container_.emplace_back();\n\
    \      map_[key] = &container_.back();\n    }\n    node_ptr node = map_[key];\n\
    \    node->init(key, value);\n    _PushLink(node, &deg_table_[0]);\n    if (!root_\
    \ or _CompareNodePriority(root_, node))\n      root_ = node;\n    ++sz_;\n  }\n\
    \n  // Time complexity: O(log n)\n  void update(int key, T updated_value) {\n\
    \    if (!map_[key] or !map_[key]->enabled) {\n      push(key, updated_value);\n\
    \      return;\n    }\n    auto node = map_[key];\n    if (!cond_(node->priority(),\
    \ updated_value)) {\n      return;\n    }\n    node->value.second = updated_value;\n\
    \    if (node->parent and _CompareNodePriority(node->parent, node)) {\n      _Consolidate(node);\n\
    \    }\n    assert(root_);\n    if (_CompareNodePriority(root_, node)) {\n   \
    \   root_ = node;\n    }\n  }\n\n  // Time complexity: O(log n)\n  void pop()\
    \ {\n    assert(root_);\n    auto r = root_;\n    root_ = nullptr;\n    _PopLink(r,\
    \ &deg_table_[r->deg]);\n    _ExpandChildren(r);\n    _ExtractTop();\n    r->free();\n\
    \    --sz_;\n  }\n\n  // Time complexity: O(log n)\n  void erase(int key) {\n\
    \    assert(map_[key] and map_[key]->enabled);\n    auto node = map_[key];\n \
    \   bool needs_extraction = root_ == node;\n    if (node->parent) {\n      _Consolidate(node);\n\
    \    }\n    assert(!node->parent);\n    _PopLink(node, &deg_table_[node->deg]);\n\
    \    _ExpandChildren(node);\n    if (needs_extraction)\n      _ExtractTop();\n\
    \    node->free();\n    --sz_;\n  }\n\n  size_t size() const { return sz_; }\n\
    \  bool empty() const { return size() == 0; }\n  void clear() {\n    root_ = nullptr;\n\
    \    sz_ = 0;\n    map_.fill(nullptr);\n    container_.clear();\n  }\n\n  void\
    \ print_for_debug() const {\n      for (int k = 0; k < (int)deg_table_.size();\
    \ k++) {\n        std::cout<<\"d=\"<<k<<std::endl;\n        if (deg_table_[k])\
    \ {\n          std::cout<<\"forward\"<<std::endl;\n          auto r = deg_table_[k];\n\
    \          do {\n            std::cout << r->priority() << ' ' << std::flush;\n\
    \            r = r->next;\n          } while (r != deg_table_[k]);\n         \
    \ std::cout<<std::endl;\n          std::cout<<\"deg\"<<std::endl;\n          r\
    \ = deg_table_[k];\n          do {\n            std::cout<<r->deg<<' '<<std::flush;\n\
    \            r = r->prev;\n          } while (r != deg_table_[k]);\n         \
    \ std::cout<<std::endl;\n        } else {\n          std::cout<<\"empty\"<<std::endl;\n\
    \        }\n      }\n  }\n\n private:\n  bool _CompareNodePriority(const_node_ptr\
    \ l, const_node_ptr r) const {\n    return cond_(l->priority(), r->priority());\n\
    \  }\n\n  void _PushLink(node_ptr node, node_ptr* link_head) {\n    if (!*link_head)\
    \ {\n      *link_head = node;\n      node->next = node;\n      node->prev = node;\n\
    \    } else {\n      auto b = (*link_head)->prev;\n      node->next = *link_head;\n\
    \      node->prev = b;\n      b->next = node;\n      (*link_head)->prev = node;\n\
    \    }\n  }\n\n  void _PopLink(node_ptr node, node_ptr* link_head) {\n    assert(*link_head);\n\
    \    if (node->next == node) {\n      assert(node->prev == node);\n      assert(*link_head\
    \ == node);\n      *link_head = nullptr;\n    } else {\n      auto n = node->next;\n\
    \      auto p = node->prev;\n      p->next = n;\n      n->prev = p;\n      if\
    \ (*link_head == node)\n        *link_head = n;\n      node->next = node;\n  \
    \    node->prev = node;\n    }\n  }\n\n  node_ptr _Meld(node_ptr l, node_ptr r)\
    \ {\n    assert(l->deg == r->deg);\n    if (_CompareNodePriority(l, r)) {\n  \
    \    std::swap(l, r);\n    }\n    _PushLink(r, &l->child);\n    r->parent = l;\n\
    \    l->deg++;\n    return l;\n  }\n\n  void _ExpandChildren(node_ptr node) {\n\
    \    while (node->child) {\n      auto c = node->child;\n      _PopLink(c, &node->child);\n\
    \      --node->deg;\n      c->parent = nullptr;\n      c->marked = false;\n  \
    \    _PushLink(c, &deg_table_[c->deg]);\n    }\n    assert(node->deg == 0);\n\
    \  }\n\n  void _ExtractTop() {\n    int k = 0;\n    while (k < (int)deg_table_.size())\
    \ {\n      while (k < (int)deg_table_.size() and !deg_table_[k]) ++k;\n      if\
    \ (k == (int)deg_table_.size())\n        break;\n      if (deg_table_[k]->next\
    \ != deg_table_[k]) { // Multiple trees at current degree\n        auto node =\
    \ deg_table_[k];\n        _PopLink(node, &deg_table_[k]);\n        assert(node->deg\
    \ == k);\n        while (deg_table_[node->deg]) {\n          auto d = node->deg;\n\
    \          auto u = deg_table_[d];\n          _PopLink(u, &deg_table_[d]);\n \
    \         node = _Meld(node, u);\n          assert(node->deg == d+1);\n      \
    \  }\n        _PushLink(node, &deg_table_[node->deg]);\n      } else { // Single\
    \ tree at current degree\n        if (!root_ or _CompareNodePriority(root_, deg_table_[k]))\n\
    \          root_ = deg_table_[k];\n        ++k;\n      }\n    }\n  }\n\n  void\
    \ _Consolidate(node_ptr node) {\n    assert(node->parent);\n    auto p = node->parent;\n\
    \    if (p->marked) {\n      assert(p->parent); // Parent is not one of the roots.\n\
    \      _Consolidate(p);\n    }\n    assert(!p->marked);\n    if (p->parent) {\n\
    \      p->marked = true;\n    } else {\n      _PopLink(p, &deg_table_[p->deg]);\n\
    \      _PushLink(p, &deg_table_[p->deg-1]);\n    }\n    _PopLink(node, &p->child);\n\
    \    p->deg--;\n    node->parent = nullptr;\n    node->marked = false;\n    _PushLink(node,\
    \ &deg_table_[node->deg]);\n  }\n\n};\n"
  code: "#pragma once\n#include <memory>\n#include <cassert>\n#include <vector>\n\
    #include <list>\n#include <iostream>\n\ntemplate<typename T, typename Cond = std::less<>>\n\
    class FibonacciHeap {\n  Cond cond_;\n\n public:\n  struct Node;\n  using node_ptr\
    \ = Node*;\n  using const_node_ptr = const Node*;\n  struct Node {\n    node_ptr\
    \ next = nullptr;\n    node_ptr prev = nullptr;\n    node_ptr child = nullptr;\n\
    \    node_ptr parent = nullptr;\n    int deg = 0;\n    bool marked = false;\n\
    \    bool enabled = false;\n    std::pair<int, T> value;\n\n    Node() = default;\n\
    \    void init(int k, T v) {\n      next = prev = this;\n      child = parent\
    \ = nullptr;\n      deg = 0;\n      marked = false;\n      enabled = true;\n \
    \     value = {k, v};\n    }\n    T priority() const { return value.second; }\n\
    \    void free() {\n      enabled = false;\n    }\n  };\n\n private:\n  node_ptr\
    \ root_ = nullptr;\n  size_t sz_ = 0;\n  std::vector<node_ptr> map_;\n  std::vector<Node>\
    \ container_;\n  std::vector<node_ptr> deg_table_;\n\n public:\n  explicit FibonacciHeap(size_t\
    \ size) : map_(size) {\n    container_.reserve(size);\n    std::array<size_t,2>\
    \ tb{1,1};\n    int k = 2;\n    while (tb[1] < size) {\n      auto x = tb[0]+tb[1];\n\
    \      tb[0] = tb[1];\n      tb[1] = x;\n      ++k;\n    }\n    deg_table_.resize(k);\n\
    \  }\n\n  inline std::pair<int, T> top() const {\n    assert(root_ and root_->enabled);\n\
    \    return root_->value;\n  }\n\n  inline std::pair<int, T> get(int key) const\
    \ {\n    assert(map_[key] and map_[key]->enabled);\n    return map_[key]->value;\n\
    \  }\n\n  // Time complexity: O(1)\n  void push(int key, T value) {\n    if (map_[key]\
    \ and map_[key]->enabled) {\n      update(key, value);\n      return;\n    }\n\
    \    if (!map_[key]) {\n      container_.emplace_back();\n      map_[key] = &container_.back();\n\
    \    }\n    node_ptr node = map_[key];\n    node->init(key, value);\n    _PushLink(node,\
    \ &deg_table_[0]);\n    if (!root_ or _CompareNodePriority(root_, node))\n   \
    \   root_ = node;\n    ++sz_;\n  }\n\n  // Time complexity: O(log n)\n  void update(int\
    \ key, T updated_value) {\n    if (!map_[key] or !map_[key]->enabled) {\n    \
    \  push(key, updated_value);\n      return;\n    }\n    auto node = map_[key];\n\
    \    if (!cond_(node->priority(), updated_value)) {\n      return;\n    }\n  \
    \  node->value.second = updated_value;\n    if (node->parent and _CompareNodePriority(node->parent,\
    \ node)) {\n      _Consolidate(node);\n    }\n    assert(root_);\n    if (_CompareNodePriority(root_,\
    \ node)) {\n      root_ = node;\n    }\n  }\n\n  // Time complexity: O(log n)\n\
    \  void pop() {\n    assert(root_);\n    auto r = root_;\n    root_ = nullptr;\n\
    \    _PopLink(r, &deg_table_[r->deg]);\n    _ExpandChildren(r);\n    _ExtractTop();\n\
    \    r->free();\n    --sz_;\n  }\n\n  // Time complexity: O(log n)\n  void erase(int\
    \ key) {\n    assert(map_[key] and map_[key]->enabled);\n    auto node = map_[key];\n\
    \    bool needs_extraction = root_ == node;\n    if (node->parent) {\n      _Consolidate(node);\n\
    \    }\n    assert(!node->parent);\n    _PopLink(node, &deg_table_[node->deg]);\n\
    \    _ExpandChildren(node);\n    if (needs_extraction)\n      _ExtractTop();\n\
    \    node->free();\n    --sz_;\n  }\n\n  size_t size() const { return sz_; }\n\
    \  bool empty() const { return size() == 0; }\n  void clear() {\n    root_ = nullptr;\n\
    \    sz_ = 0;\n    map_.fill(nullptr);\n    container_.clear();\n  }\n\n  void\
    \ print_for_debug() const {\n      for (int k = 0; k < (int)deg_table_.size();\
    \ k++) {\n        std::cout<<\"d=\"<<k<<std::endl;\n        if (deg_table_[k])\
    \ {\n          std::cout<<\"forward\"<<std::endl;\n          auto r = deg_table_[k];\n\
    \          do {\n            std::cout << r->priority() << ' ' << std::flush;\n\
    \            r = r->next;\n          } while (r != deg_table_[k]);\n         \
    \ std::cout<<std::endl;\n          std::cout<<\"deg\"<<std::endl;\n          r\
    \ = deg_table_[k];\n          do {\n            std::cout<<r->deg<<' '<<std::flush;\n\
    \            r = r->prev;\n          } while (r != deg_table_[k]);\n         \
    \ std::cout<<std::endl;\n        } else {\n          std::cout<<\"empty\"<<std::endl;\n\
    \        }\n      }\n  }\n\n private:\n  bool _CompareNodePriority(const_node_ptr\
    \ l, const_node_ptr r) const {\n    return cond_(l->priority(), r->priority());\n\
    \  }\n\n  void _PushLink(node_ptr node, node_ptr* link_head) {\n    if (!*link_head)\
    \ {\n      *link_head = node;\n      node->next = node;\n      node->prev = node;\n\
    \    } else {\n      auto b = (*link_head)->prev;\n      node->next = *link_head;\n\
    \      node->prev = b;\n      b->next = node;\n      (*link_head)->prev = node;\n\
    \    }\n  }\n\n  void _PopLink(node_ptr node, node_ptr* link_head) {\n    assert(*link_head);\n\
    \    if (node->next == node) {\n      assert(node->prev == node);\n      assert(*link_head\
    \ == node);\n      *link_head = nullptr;\n    } else {\n      auto n = node->next;\n\
    \      auto p = node->prev;\n      p->next = n;\n      n->prev = p;\n      if\
    \ (*link_head == node)\n        *link_head = n;\n      node->next = node;\n  \
    \    node->prev = node;\n    }\n  }\n\n  node_ptr _Meld(node_ptr l, node_ptr r)\
    \ {\n    assert(l->deg == r->deg);\n    if (_CompareNodePriority(l, r)) {\n  \
    \    std::swap(l, r);\n    }\n    _PushLink(r, &l->child);\n    r->parent = l;\n\
    \    l->deg++;\n    return l;\n  }\n\n  void _ExpandChildren(node_ptr node) {\n\
    \    while (node->child) {\n      auto c = node->child;\n      _PopLink(c, &node->child);\n\
    \      --node->deg;\n      c->parent = nullptr;\n      c->marked = false;\n  \
    \    _PushLink(c, &deg_table_[c->deg]);\n    }\n    assert(node->deg == 0);\n\
    \  }\n\n  void _ExtractTop() {\n    int k = 0;\n    while (k < (int)deg_table_.size())\
    \ {\n      while (k < (int)deg_table_.size() and !deg_table_[k]) ++k;\n      if\
    \ (k == (int)deg_table_.size())\n        break;\n      if (deg_table_[k]->next\
    \ != deg_table_[k]) { // Multiple trees at current degree\n        auto node =\
    \ deg_table_[k];\n        _PopLink(node, &deg_table_[k]);\n        assert(node->deg\
    \ == k);\n        while (deg_table_[node->deg]) {\n          auto d = node->deg;\n\
    \          auto u = deg_table_[d];\n          _PopLink(u, &deg_table_[d]);\n \
    \         node = _Meld(node, u);\n          assert(node->deg == d+1);\n      \
    \  }\n        _PushLink(node, &deg_table_[node->deg]);\n      } else { // Single\
    \ tree at current degree\n        if (!root_ or _CompareNodePriority(root_, deg_table_[k]))\n\
    \          root_ = deg_table_[k];\n        ++k;\n      }\n    }\n  }\n\n  void\
    \ _Consolidate(node_ptr node) {\n    assert(node->parent);\n    auto p = node->parent;\n\
    \    if (p->marked) {\n      assert(p->parent); // Parent is not one of the roots.\n\
    \      _Consolidate(p);\n    }\n    assert(!p->marked);\n    if (p->parent) {\n\
    \      p->marked = true;\n    } else {\n      _PopLink(p, &deg_table_[p->deg]);\n\
    \      _PushLink(p, &deg_table_[p->deg-1]);\n    }\n    _PopLink(node, &p->child);\n\
    \    p->deg--;\n    node->parent = nullptr;\n    node->marked = false;\n    _PushLink(node,\
    \ &deg_table_[node->deg]);\n  }\n\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/fibonacci_heap.hpp
  requiredBy:
  - include/mtl/min_cost_flow.hpp
  - include/mtl/dijkstra.hpp
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/dijkstra_fibonacci.test.cpp
  - test/min_cost_flow.test.cpp
documentation_of: include/mtl/fibonacci_heap.hpp
layout: document
redirect_from:
- /library/include/mtl/fibonacci_heap.hpp
- /library/include/mtl/fibonacci_heap.hpp.html
title: include/mtl/fibonacci_heap.hpp
---
