---
data:
  _extendedDependsOn:
  - icon: ':x:'
    path: include/mtl/persistent_array.hpp
    title: include/mtl/persistent_array.hpp
  - icon: ':x:'
    path: include/mtl/persistent_unionfind.hpp
    title: include/mtl/persistent_unionfind.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/persistent_unionfind
    links:
    - https://judge.yosupo.jp/problem/persistent_unionfind
  bundledCode: "#line 1 \"test/yosupo/persistent_unionfind.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.yosupo.jp/problem/persistent_unionfind\"\n#include <iostream>\n\
    #line 2 \"include/mtl/persistent_array.hpp\"\n#include <memory>\r\n#include <utility>\r\
    \n#include <array>\r\n\r\ntemplate<typename T, unsigned Degree=4>\r\nclass PersistentArray\
    \ {\r\n  static constexpr unsigned M = Degree;\r\n private:\r\n  struct Node;\r\
    \n  using node_ptr = std::shared_ptr<Node>;\r\n  struct Node {\r\n    T v;\r\n\
    \    std::array<node_ptr, M> ch;\r\n    Node() = default;\r\n    explicit Node(const\
    \ T& v) : v(v), ch({}) {}\r\n    explicit Node(T&& v) : v(std::move(v)), ch({})\
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
    \ + size_of(rb)));\n        return ret;\n    }\n};\n#line 4 \"test/yosupo/persistent_unionfind.test.cpp\"\
    \n#include <bits/stdc++.h>\nusing namespace std;\n\nint main() {\n    int n,q;\
    \ cin>>n>>q;\n    vector<PersistentUnionfind> U(q+1);\n    for (int i = 0; i <\
    \ q; i++) {\n        int t; cin>>t;\n        if (t == 0) {\n            int k,u,v;\
    \ cin>>k>>u>>v;\n            U[i+1] = U[k+1].merge(u,v);\n        } else {\n \
    \           int k,u,v; cin>>k>>u>>v;\n            cout << U[k+1].same(u,v) <<\
    \ endl;\n        }\n    }\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/persistent_unionfind\"\n\
    #include <iostream>\n#include \"../../include/mtl/persistent_unionfind.hpp\"\n\
    #include <bits/stdc++.h>\nusing namespace std;\n\nint main() {\n    int n,q; cin>>n>>q;\n\
    \    vector<PersistentUnionfind> U(q+1);\n    for (int i = 0; i < q; i++) {\n\
    \        int t; cin>>t;\n        if (t == 0) {\n            int k,u,v; cin>>k>>u>>v;\n\
    \            U[i+1] = U[k+1].merge(u,v);\n        } else {\n            int k,u,v;\
    \ cin>>k>>u>>v;\n            cout << U[k+1].same(u,v) << endl;\n        }\n  \
    \  }\n}"
  dependsOn:
  - include/mtl/persistent_unionfind.hpp
  - include/mtl/persistent_array.hpp
  isVerificationFile: true
  path: test/yosupo/persistent_unionfind.test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: test/yosupo/persistent_unionfind.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/persistent_unionfind.test.cpp
- /verify/test/yosupo/persistent_unionfind.test.cpp.html
title: test/yosupo/persistent_unionfind.test.cpp
---
