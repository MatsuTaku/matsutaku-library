---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/hld.hpp
    title: include/mtl/hld.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/segment_hld.hpp
    title: include/mtl/segment_hld.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://onlinejudge.u-aizu.ac.jp/problems/2450
    links:
    - https://onlinejudge.u-aizu.ac.jp/problems/2450
  bundledCode: "#line 1 \"test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp\"\
    \n#define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/2450\"\r\n#line\
    \ 2 \"include/mtl/hld.hpp\"\n#include <cstddef>\n#include <vector>\n\nstruct Hld\
    \ {\n  int n;\n  std::vector<std::vector<int>> edge;\n  std::vector<int> size,\
    \ in, out, head, rev, par, depth;\n private:\n  void dfs_sz(int v, int p, int\
    \ d) {\n    par[v] = p;\n    size[v] = 1;\n    if (!edge[v].empty() and edge[v][0]\
    \ == p)\n      std::swap(edge[v][0], edge[v].back());\n    for (auto& t:edge[v])\
    \ {\n      if (t == p) continue;\n      dfs_sz(t, v, d+1);\n      size[v] += size[t];\n\
    \      if (size[edge[v][0]] < size[t])\n        std::swap(edge[v][0], t);\n  \
    \  }\n  }\n  void dfs_hld(int v, int p, int& times) {\n    in[v] = times++;\n\
    \    rev[in[v]] = v;\n    if (edge[v][0] != p) {\n      int t = edge[v][0];\n\
    \      head[t] = head[v];\n      depth[t] = depth[v];\n      dfs_hld(t, v, times);\n\
    \    }\n    for (size_t i = 1; i < edge[v].size(); i++) {\n      int t = edge[v][i];\n\
    \      if (t == p) continue;\n      head[t] = t;\n      depth[t] = depth[v] +\
    \ 1;\n      dfs_hld(t, v, times);\n    }\n    out[v] = times;\n  }\n\n public:\n\
    \  Hld(int n) : n(n), edge(n), size(n), in(n), out(n), head(n), rev(n), par(n),\
    \ depth(n) {}\n\n  inline void add_edge(int a, int b) {\n    edge[a].push_back(b);\n\
    \    edge[b].push_back(a);\n  }\n\n  void build(int root = 0) {\n    dfs_sz(root,\
    \ -1, 0);\n    int t = 0;\n    head[root] = root;\n    depth[root] = 0;\n    dfs_hld(root,\
    \ -1, t);\n  }\n\n  inline int lca(int a, int b) const {\n    if (depth[a] > depth[b])\
    \ std::swap(a, b);\n    while (depth[a] < depth[b]) {\n      b = par[head[b]];\n\
    \    }\n    while (head[a] != head[b]) {\n      a = par[head[a]];\n      b = par[head[b]];\n\
    \    }\n    return in[a] < in[b] ? a : b;\n  }\n\n  template<class T, typename\
    \ Query,\n      bool INCLUDE_LCA = true>\n  T query(int u, int v, Query Q) const\
    \ {\n    if (depth[u] > depth[v]) std::swap(u,v);\n    T um, vm;\n    auto up\
    \ = [&](int& v, T& ret) {\n      ret = Q(in[head[v]], in[v]+1) * ret;\n      v\
    \ = par[head[v]];\n    };\n    while (depth[u] < depth[v]) {\n      up(v, vm);\n\
    \    }\n    while (head[u] != head[v]) {\n      up(u, um);\n      up(v, vm);\n\
    \    }\n    if (in[u] > in[v]) {\n      std::swap(u,v);\n      std::swap(um,vm);\n\
    \    }\n    int l = INCLUDE_LCA ? in[u] : in[u]+1;\n    return ~um * Q(l, in[v]+1)\
    \ * vm;\n  }\n\n  template<typename T, typename Set>\n  void set(int i, T&& val,\
    \ Set S) const {\n    S(in[i], std::forward<T>(val));\n  }\n\n  template<typename\
    \ T, typename Upd,\n      bool INCLUDE_LCA = true>\n  void update(int u, int v,\
    \ const T& val, Upd U) const {\n    if (depth[u] > depth[v]) std::swap(u,v);\n\
    \    auto up = [&](int& v) {\n      U(in[head[v]], in[v]+1, val);\n      v = par[head[v]];\n\
    \    };\n    while (depth[u] < depth[v]) {\n      up(v);\n    }\n    while (head[u]\
    \ != head[v]) {\n      up(u);\n      up(v);\n    }\n    if (in[u] > in[v]) std::swap(u,v);\n\
    \    int l = INCLUDE_LCA ? in[u] : in[u]+1;\n    U(l, in[v]+1, val);\n  }\n};\n\
    #line 4 \"include/mtl/segment_hld.hpp\"\n#include <cassert>\r\n#if __cplusplus\
    \ >= 202002L\r\n#include <concepts>\r\n\r\ntemplate<typename M>\r\nconcept SegmentHldMonoid\
    \ = requires (M m) {\r\n  {m * m} -> std::same_as<M>;\r\n  {~m} -> std::same_as<M>;\r\
    \n};\r\ntemplate<typename A, typename M>\r\nconcept SegmentHldOperatorMonoid =\
    \ requires (A a, M m) {\r\n  {a()} -> std::same_as<bool>;\r\n  {a *= a} -> std::convertible_to<A>;\r\
    \n  {a.act(m, 1)} -> std::same_as<M>;\r\n};\r\n#endif\r\n\r\ntemplate<typename\
    \ Node>\r\nclass SegmentHldBase {\r\n protected:\r\n  int n_;\r\n  std::vector<Node>\
    \ tree_;\r\n  std::vector<int> target_;\r\n public:\r\n  explicit SegmentHldBase(const\
    \ Hld& tree) : n_(tree.n), target_(n_) {\r\n    std::vector<long long> cw(n_+1);\r\
    \n    for (int i = 0; i < n_; i++) {\r\n      auto w = tree.size[i];\r\n     \
    \ if (!tree.edge[i].empty() and tree.edge[i][0] != tree.par[i])\r\n        w -=\
    \ tree.size[tree.edge[i][0]];\r\n      cw[i+1] = cw[i] + w;\r\n    }\r\n    tree_.reserve(n_*2);\r\
    \n    tree_.resize(1);\r\n    tree_[0].l = 0;\r\n    tree_[0].r = n_;\r\n    for\
    \ (int i = 0; i < (int)tree_.size(); i++) {\r\n      if (tree_[i].size() == 1)\
    \ {\r\n        target_[tree_[i].l] = i;\r\n        continue;\r\n      }\r\n  \
    \    auto l = tree_[i].l;\r\n      auto r = tree_[i].r;\r\n      auto mid = upper_bound(cw.begin()+l,\
    \ cw.begin()+r, (cw[r]+cw[l]+1)/2);\r\n      assert(cw.begin()+l != mid);\r\n\
    \      if (*std::prev(mid)-cw[l] > cw[r]-*mid)\r\n        --mid;\r\n      int\
    \ m = mid-cw.begin();\r\n      if (l < m) {\r\n        tree_[i].lc = tree_.size();\r\
    \n        tree_.emplace_back();\r\n        tree_.back().l = l;\r\n        tree_.back().r\
    \ = m;\r\n        tree_.back().p = i;\r\n      }\r\n      if (m < r) {\r\n   \
    \     tree_[i].rc = tree_.size();\r\n        tree_.emplace_back();\r\n       \
    \ tree_.back().l = m;\r\n        tree_.back().r = r;\r\n        tree_.back().p\
    \ = i;\r\n      }\r\n    }\r\n  }\r\n  template<typename InputIt>\r\n  explicit\
    \ SegmentHldBase(const Hld& tree, InputIt begin, InputIt end) : SegmentHldBase(tree)\
    \ {\r\n      int i = 0;\r\n      for (auto it = begin; it != end; ++it, ++i) {\r\
    \n        tree_[target_[i]].m = *it;\r\n      }\r\n      for (int i = (int)tree_.size()-1;\
    \ i >= 0; i--) {\r\n        if (tree_[i].size() == 1) continue;\r\n        tree_[i].m\
    \ = tree_[tree_[i].lc].m * tree_[tree_[i].rc].m;\r\n      }\r\n    }\r\n};\r\n\
    \r\ntemplate<typename M>\r\nstruct SegmentHldNode {\r\n  using monoid_type = M;\r\
    \n  int l,r,p=-1,lc=-1,rc=-1;\r\n  M m;\r\n  int size() const {\r\n    return\
    \ r-l;\r\n  }\r\n};\r\ntemplate<typename M>\r\nclass SegmentHld : private SegmentHldBase<SegmentHldNode<M>>\
    \ {\r\n#if __cplusplus >= 202002L\r\n  static_assert(SegmentHldMonoid<M>);\r\n\
    #endif\r\n private:\r\n  using Node = SegmentHldNode<M>;\r\n  using Base = SegmentHldBase<Node>;\r\
    \n  using Base::n_;\r\n  using Base::tree_;\r\n  using Base::target_;\r\n public:\r\
    \n  explicit SegmentHld(const Hld& tree) : Base(tree) {}\r\n  template<typename\
    \ InputIt>\r\n  explicit SegmentHld(const Hld& tree, InputIt begin, InputIt end)\
    \ : Base(tree, begin, end) {}\r\n  template<typename T>\r\n  void set(int index,\
    \ T&& v) {\r\n    int i = target_[index];\r\n    tree_[i].m = std::forward<T>(v);\r\
    \n    i = tree_[i].p;\r\n    while (i != -1) {\r\n      tree_[i].m = tree_[tree_[i].lc].m\
    \ * tree_[tree_[i].rc].m;\r\n      i = tree_[i].p;\r\n    }\r\n  }\r\n  M query(int\
    \ l, int r) const {\r\n    return _query(l,r,0);\r\n  }\r\n private:\r\n  M _query(int\
    \ l, int r, int u) const {\r\n    if (u == -1)\r\n      return M();\r\n    auto\
    \ _l = tree_[u].l, _r = tree_[u].r;\r\n    if (_r <= l or r <= _l)\r\n      return\
    \ M();\r\n    if (l <= _l and _r <= r)\r\n      return tree_[u].m;\r\n    return\
    \ _query(l, r, tree_[u].lc) * _query(l, r, tree_[u].rc);\r\n  }\r\n};\r\n\r\n\r\
    \ntemplate<typename M, typename A>\r\nstruct LazySegmentHldNode : SegmentHldNode<M>\
    \ {\r\n  using operator_monoid_type = A;\r\n  A a;\r\n};\r\ntemplate<typename\
    \ M, typename A>\r\nclass LazySegmentHld : private SegmentHldBase<LazySegmentHldNode<M,A>>\
    \ {\r\n#if __cplusplus >= 202002L\r\n  static_assert(SegmentHldMonoid<M>);\r\n\
    \  static_assert(SegmentHldOperatorMonoid<A, M>);\r\n#endif\r\n private:\r\n \
    \ using Node = LazySegmentHldNode<M,A>;\r\n  using Base = SegmentHldBase<Node>;\r\
    \n  using Base::n_;\r\n  using Base::tree_;\r\n  using Base::target_;\r\n public:\r\
    \n  explicit LazySegmentHld(const Hld& tree) : Base(tree) {}\r\n  template<typename\
    \ InputIt>\r\n  explicit LazySegmentHld(const Hld& tree, InputIt begin, InputIt\
    \ end) : Base(tree, begin, end) {}\r\n private:\r\n  inline void _propagate(int\
    \ u) {\r\n    auto& a = tree_[u].a;\r\n    if (!a()) return;\r\n    tree_[u].m\
    \ = a.act(tree_[u].m, tree_[u].size());\r\n    if (tree_[u].size() > 1) {\r\n\
    \      tree_[tree_[u].lc].a *= a;\r\n      tree_[tree_[u].rc].a *= a;\r\n    }\r\
    \n    tree_[u].a = A();\r\n  }\r\n public:\r\n  template<typename T>\r\n  void\
    \ set(int index, T&& v) {\r\n    std::vector<int> ids;\r\n    int u = target_[index];\r\
    \n    ids.push_back(u);\r\n    u = tree_[u].p;\r\n    while (u != -1) {\r\n  \
    \    ids.push_back(u);\r\n      u = tree_[u].p;\r\n    }\r\n    for (int i = (int)ids.size()-1;\
    \ i >= 0; i--) {\r\n      _propagate(ids[i]);\r\n    }\r\n    tree_[ids[0]].m\
    \ = std::forward<T>(v);\r\n    for (int i = 1; i < ids.size(); i++) {\r\n    \
    \  u = ids[i];\r\n      auto lc = tree_[u].lc, rc = tree_[u].rc;\r\n      auto\
    \ ac = lc ^ rc ^ ids[i-1];\r\n      _propagate(ac);\r\n      tree_[u].m = tree_[lc].m\
    \ * tree_[rc].m;\r\n    }\r\n  }\r\n  M query(int l, int r) {\r\n    return _query(l,r,0);\r\
    \n  }\r\n private:\r\n  M _query(int l, int r, int u) {\r\n    if (u == -1)\r\n\
    \      return M();\r\n    auto _l = tree_[u].l, _r = tree_[u].r;\r\n    if (_r\
    \ <= l or r <= _l)\r\n      return M();\r\n    _propagate(u);\r\n    if (l <=\
    \ _l and _r <= r) {\r\n      return tree_[u].m;\r\n    } else {\r\n      return\
    \ _query(l, r, tree_[u].lc) * _query(l, r, tree_[u].rc);\r\n    }\r\n  }\r\n public:\r\
    \n  template<typename T>\r\n  void update(int l, int r, const T& v) {\r\n    _update(l,\
    \ r, v, 0);\r\n  }\r\n private:\r\n  template<typename T>\r\n  void _update(int\
    \ l, int r, const T& v, int u) {\r\n    if (u == -1)\r\n      return;\r\n    auto\
    \ _l = tree_[u].l, _r = tree_[u].r;\r\n    if (_r <= l or r <= _l) {\r\n     \
    \ _propagate(u);\r\n    } else if (l <= _l and _r <= r) {\r\n      tree_[u].a\
    \ *= v;\r\n      _propagate(u);\r\n    } else {\r\n      _propagate(u);\r\n  \
    \    if (tree_[u].size() > 1) {\r\n        auto lc = tree_[u].lc, rc = tree_[u].rc;\r\
    \n        _update(l, r, v, lc);\r\n        _update(l, r, v, rc);\r\n        tree_[u].m\
    \ = tree_[lc].m * tree_[rc].m;\r\n      }\r\n    }\r\n  }\r\n};\n#line 4 \"test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp\"\
    \n#include <bits/stdc++.h>\r\n\r\nusing namespace std;\r\n\r\nconstexpr int MINF\
    \ = -1e9;\r\nstruct M {\r\n  int l,r,sum,v;\r\n  M() : v(MINF) {}\r\n  M(int w)\
    \ : l(w),r(w),sum(w),v(w) {}\r\n  friend M operator*(const M& lhs, const M& rhs)\
    \ {\r\n    if (lhs.v == MINF) return rhs;\r\n    if (rhs.v == MINF) return lhs;\r\
    \n    M ret;\r\n    ret.l = max(lhs.l, lhs.sum + rhs.l);\r\n    ret.r = max(rhs.r,\
    \ lhs.r + rhs.sum);\r\n    ret.v = max({lhs.v, rhs.v, lhs.r + rhs.l});\r\n   \
    \ ret.sum = lhs.sum + rhs.sum;\r\n    return ret;\r\n  }\r\n  M operator~() {\r\
    \n    M ret = *this;\r\n    swap(ret.l, ret.r);\r\n    return ret;\r\n  }\r\n\
    };\r\nM rev(M m) {\r\n  M ret = m;\r\n  swap(ret.l, ret.r);\r\n  return ret;\r\
    \n}\r\nstruct A {\r\n  int v;\r\n  bool f;\r\n  A() : f(false) {}\r\n  A(int v)\
    \ : v(v), f(true) {}\r\n  bool operator()() const { return f; }\r\n  A& operator*=(const\
    \ A& r) {\r\n    if (r.f) *this = r;\r\n    return *this;\r\n  }\r\n  M act(const\
    \ M& m,int sz) const {\r\n    assert(f);\r\n    M ret;\r\n    ret.sum = v*sz;\r\
    \n    if (v >= 0) {\r\n      ret.l = ret.r = ret.v = ret.sum;\r\n    } else {\r\
    \n      ret.l = ret.r = ret.v = v;\r\n    }\r\n    return ret;\r\n  }\r\n};\r\n\
    \r\nint main() {\r\n  int n,q; cin>>n>>q;\r\n  vector<int> W(n);\r\n  for (auto&\
    \ w:W) cin>>w;\r\n  Hld T(n);\r\n  for (int i = 0; i < n-1; i++) {\r\n    int\
    \ s,e; cin>>s>>e; s--; e--;\r\n    T.add_edge(s,e);\r\n  }\r\n  T.build();\r\n\
    \  vector<int> X(n);\r\n  for (int i = 0; i < n; i++) X[T.in[i]] = W[i];\r\n \
    \ LazySegmentHld<M,A> RQ(T, X.begin(), X.end());\r\n  auto range_update = [&](int\
    \ l, int r, int v) {\r\n    RQ.update(l,r,v);\r\n  };\r\n  auto query = [&](int\
    \ l, int r) {\r\n    return RQ.query(l,r);\r\n  };\r\n  for (int i = 0; i < q;\
    \ i++) {\r\n    int t; cin>>t;\r\n    if (t == 1) {\r\n      int a,b,c; cin>>a>>b>>c;\
    \ a--; b--;\r\n      T.update(a,b,c,range_update);\r\n    } else if (t == 2) {\r\
    \n      int a,b,c; cin>>a>>b>>c; a--; b--;\r\n      cout << T.query<M>(a,b,query).v\
    \ << endl;\r\n    }\r\n  }\r\n}\n"
  code: "#define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/2450\"\r\n#include\
    \ \"../../include/mtl/hld.hpp\"\r\n#include \"../../include/mtl/segment_hld.hpp\"\
    \r\n#include <bits/stdc++.h>\r\n\r\nusing namespace std;\r\n\r\nconstexpr int\
    \ MINF = -1e9;\r\nstruct M {\r\n  int l,r,sum,v;\r\n  M() : v(MINF) {}\r\n  M(int\
    \ w) : l(w),r(w),sum(w),v(w) {}\r\n  friend M operator*(const M& lhs, const M&\
    \ rhs) {\r\n    if (lhs.v == MINF) return rhs;\r\n    if (rhs.v == MINF) return\
    \ lhs;\r\n    M ret;\r\n    ret.l = max(lhs.l, lhs.sum + rhs.l);\r\n    ret.r\
    \ = max(rhs.r, lhs.r + rhs.sum);\r\n    ret.v = max({lhs.v, rhs.v, lhs.r + rhs.l});\r\
    \n    ret.sum = lhs.sum + rhs.sum;\r\n    return ret;\r\n  }\r\n  M operator~()\
    \ {\r\n    M ret = *this;\r\n    swap(ret.l, ret.r);\r\n    return ret;\r\n  }\r\
    \n};\r\nM rev(M m) {\r\n  M ret = m;\r\n  swap(ret.l, ret.r);\r\n  return ret;\r\
    \n}\r\nstruct A {\r\n  int v;\r\n  bool f;\r\n  A() : f(false) {}\r\n  A(int v)\
    \ : v(v), f(true) {}\r\n  bool operator()() const { return f; }\r\n  A& operator*=(const\
    \ A& r) {\r\n    if (r.f) *this = r;\r\n    return *this;\r\n  }\r\n  M act(const\
    \ M& m,int sz) const {\r\n    assert(f);\r\n    M ret;\r\n    ret.sum = v*sz;\r\
    \n    if (v >= 0) {\r\n      ret.l = ret.r = ret.v = ret.sum;\r\n    } else {\r\
    \n      ret.l = ret.r = ret.v = v;\r\n    }\r\n    return ret;\r\n  }\r\n};\r\n\
    \r\nint main() {\r\n  int n,q; cin>>n>>q;\r\n  vector<int> W(n);\r\n  for (auto&\
    \ w:W) cin>>w;\r\n  Hld T(n);\r\n  for (int i = 0; i < n-1; i++) {\r\n    int\
    \ s,e; cin>>s>>e; s--; e--;\r\n    T.add_edge(s,e);\r\n  }\r\n  T.build();\r\n\
    \  vector<int> X(n);\r\n  for (int i = 0; i < n; i++) X[T.in[i]] = W[i];\r\n \
    \ LazySegmentHld<M,A> RQ(T, X.begin(), X.end());\r\n  auto range_update = [&](int\
    \ l, int r, int v) {\r\n    RQ.update(l,r,v);\r\n  };\r\n  auto query = [&](int\
    \ l, int r) {\r\n    return RQ.query(l,r);\r\n  };\r\n  for (int i = 0; i < q;\
    \ i++) {\r\n    int t; cin>>t;\r\n    if (t == 1) {\r\n      int a,b,c; cin>>a>>b>>c;\
    \ a--; b--;\r\n      T.update(a,b,c,range_update);\r\n    } else if (t == 2) {\r\
    \n      int a,b,c; cin>>a>>b>>c; a--; b--;\r\n      cout << T.query<M>(a,b,query).v\
    \ << endl;\r\n    }\r\n  }\r\n}"
  dependsOn:
  - include/mtl/hld.hpp
  - include/mtl/segment_hld.hpp
  isVerificationFile: true
  path: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
  requiredBy: []
  timestamp: '2023-04-08 02:15:04+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
layout: document
redirect_from:
- /verify/test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
- /verify/test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp.html
title: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
---
