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
  bundledCode: "#line 2 \"include/mtl/meldable_heap.hpp\"\n#include <memory>\r\n#include\
    \ <cstddef>\r\n\r\ntemplate<typename T, typename Cond = less<>>\r\nclass MeldableHeap\
    \ {\r\n public:\r\n  struct Node;\r\n  using NodePtr = shared_ptr<Node>;\r\n \
    \ struct Node {\r\n    T x;\r\n    NodePtr left, right, parent;\r\n  };\r\n\r\n\
    \ private:\r\n  NodePtr _r;\r\n  size_t _sz;\r\n\r\n  friend NodePtr _meld(NodePtr\
    \ l, NodePtr r) {\r\n    if (l == nullptr) return r;\r\n    if (r == nullptr)\
    \ return l;\r\n    if (Cond()(l->x, r->x))\r\n      return _meld(r, l);\r\n  \
    \  if (rand()%2 == 0) {\r\n      l->left = _meld(l->left, r);\r\n      l->left->parent\
    \ = l;\r\n    } else {\r\n      l->right = _meld(l->right, r);\r\n      l->right->parent\
    \ = l;\r\n    }\r\n    return l;\r\n  }\r\n\r\n public:\r\n  MeldableHeap() :\
    \ _r(nullptr), _sz(0) {}\r\n  template<typename It, typename Traits = std::iterator_traits<It>>\r\
    \n  MeldableHeap(It begin, It end) : MeldableHeap() {\r\n    for (It it = begin;\
    \ it != end; ++it) {\r\n      push(*it);\r\n    }\r\n  }\r\n\r\n  MeldableHeap&\
    \ meld(MeldableHeap& r) {\r\n    _r = _meld(_r, r._r);\r\n    _sz += r.size();\r\
    \n    return *this;\r\n  }\r\n\r\n  friend MeldableHeap& meld(MeldableHeap& l,\
    \ MeldableHeap& r) {\r\n    l.meld(r);\r\n    return l;\r\n  }\r\n\r\n  void push(T\
    \ x) {\r\n    NodePtr u(new Node);\r\n    u->x = x;\r\n    _r = _meld(_r, u);\r\
    \n    _r->parent = nullptr;\r\n    _sz++;\r\n  }\r\n\r\n  T top() const { return\
    \ _r->x; }\r\n  T pop() {\r\n    assert(_r);\r\n    T x = _r->x;\r\n    _r = _meld(_r->left,\
    \ _r->right);\r\n    if (_r) _r->parent = nullptr;\r\n    _sz--;\r\n    return\
    \ x;\r\n  }\r\n\r\n  size_t size() const { return _sz; }\r\n  bool empty() const\
    \ { return size() == 0; }\r\n  void clear() {\r\n    _r = nullptr;\r\n    _sz\
    \ = 0;\r\n  }\r\n\r\n};\n"
  code: "#pragma once\r\n#include <memory>\r\n#include <cstddef>\r\n\r\ntemplate<typename\
    \ T, typename Cond = less<>>\r\nclass MeldableHeap {\r\n public:\r\n  struct Node;\r\
    \n  using NodePtr = shared_ptr<Node>;\r\n  struct Node {\r\n    T x;\r\n    NodePtr\
    \ left, right, parent;\r\n  };\r\n\r\n private:\r\n  NodePtr _r;\r\n  size_t _sz;\r\
    \n\r\n  friend NodePtr _meld(NodePtr l, NodePtr r) {\r\n    if (l == nullptr)\
    \ return r;\r\n    if (r == nullptr) return l;\r\n    if (Cond()(l->x, r->x))\r\
    \n      return _meld(r, l);\r\n    if (rand()%2 == 0) {\r\n      l->left = _meld(l->left,\
    \ r);\r\n      l->left->parent = l;\r\n    } else {\r\n      l->right = _meld(l->right,\
    \ r);\r\n      l->right->parent = l;\r\n    }\r\n    return l;\r\n  }\r\n\r\n\
    \ public:\r\n  MeldableHeap() : _r(nullptr), _sz(0) {}\r\n  template<typename\
    \ It, typename Traits = std::iterator_traits<It>>\r\n  MeldableHeap(It begin,\
    \ It end) : MeldableHeap() {\r\n    for (It it = begin; it != end; ++it) {\r\n\
    \      push(*it);\r\n    }\r\n  }\r\n\r\n  MeldableHeap& meld(MeldableHeap& r)\
    \ {\r\n    _r = _meld(_r, r._r);\r\n    _sz += r.size();\r\n    return *this;\r\
    \n  }\r\n\r\n  friend MeldableHeap& meld(MeldableHeap& l, MeldableHeap& r) {\r\
    \n    l.meld(r);\r\n    return l;\r\n  }\r\n\r\n  void push(T x) {\r\n    NodePtr\
    \ u(new Node);\r\n    u->x = x;\r\n    _r = _meld(_r, u);\r\n    _r->parent =\
    \ nullptr;\r\n    _sz++;\r\n  }\r\n\r\n  T top() const { return _r->x; }\r\n \
    \ T pop() {\r\n    assert(_r);\r\n    T x = _r->x;\r\n    _r = _meld(_r->left,\
    \ _r->right);\r\n    if (_r) _r->parent = nullptr;\r\n    _sz--;\r\n    return\
    \ x;\r\n  }\r\n\r\n  size_t size() const { return _sz; }\r\n  bool empty() const\
    \ { return size() == 0; }\r\n  void clear() {\r\n    _r = nullptr;\r\n    _sz\
    \ = 0;\r\n  }\r\n\r\n};"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/meldable_heap.hpp
  requiredBy: []
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/meldable_heap.hpp
layout: document
redirect_from:
- /library/include/mtl/meldable_heap.hpp
- /library/include/mtl/meldable_heap.hpp.html
title: include/mtl/meldable_heap.hpp
---
