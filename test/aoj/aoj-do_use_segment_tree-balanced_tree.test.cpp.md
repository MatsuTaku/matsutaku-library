---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/hld.hpp
    title: include/mtl/hld.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/monoid.hpp
    title: include/mtl/monoid.hpp
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
    \ {\n  int r,n;\n  std::vector<std::vector<int>> edge;\n  std::vector<int> size,\
    \ in, out, head, rev, par, depth, clen;\n private:\n  void dfs_sz(int v, int p,\
    \ int d) {\n    par[v] = p;\n    size[v] = 1;\n    if (!edge[v].empty() and edge[v][0]\
    \ == p)\n      std::swap(edge[v][0], edge[v].back());\n    for (auto& t:edge[v])\
    \ {\n      if (t == p) continue;\n      dfs_sz(t, v, d+1);\n      size[v] += size[t];\n\
    \      if (size[edge[v][0]] < size[t])\n        std::swap(edge[v][0], t);\n  \
    \  }\n  }\n  void dfs_hld(int v, int p, int& times) {\n    in[v] = times++;\n\
    \    rev[in[v]] = v;\n    clen[v] = 1;\n    if (!edge[v].empty() and edge[v][0]\
    \ != p) {\n      int t = edge[v][0];\n      head[t] = head[v];\n      depth[t]\
    \ = depth[v];\n      dfs_hld(t, v, times);\n      clen[v] += clen[t];\n    }\n\
    \    for (size_t i = 1; i < edge[v].size(); i++) {\n      int t = edge[v][i];\n\
    \      if (t == p) continue;\n      head[t] = t;\n      depth[t] = depth[v] +\
    \ 1;\n      dfs_hld(t, v, times);\n    }\n    out[v] = times;\n  }\n\n public:\n\
    \  Hld(int n) : r(0), n(n), edge(n), size(n), in(n), out(n), head(n), rev(n),\
    \ par(n), depth(n), clen(n) {}\n\n  inline void add_edge(int a, int b) {\n   \
    \ edge[a].push_back(b);\n    edge[b].push_back(a);\n  }\n\n  void build(int root\
    \ = 0) {\n    r = root;\n    dfs_sz(root, -1, 0);\n    int t = 0;\n    head[root]\
    \ = root;\n    depth[root] = 0;\n    dfs_hld(root, -1, t);\n  }\n\n  inline int\
    \ lca(int a, int b) const {\n    if (depth[a] > depth[b]) std::swap(a, b);\n \
    \   while (depth[a] < depth[b]) {\n      b = par[head[b]];\n    }\n    while (head[a]\
    \ != head[b]) {\n      a = par[head[a]];\n      b = par[head[b]];\n    }\n   \
    \ return in[a] < in[b] ? a : b;\n  }\n\n private:\n  template<class T, class Query,\
    \ class ReverseQuery>\n  T _query(int u, int v, Query Q, ReverseQuery RQ, bool\
    \ include_lca) const {\n    T um, vm;\n    auto u_up = [&]() {\n      um = um\
    \ * (T)RQ(in[head[u]], in[u]+1);\n      u = par[head[u]];\n    };\n    auto v_up\
    \ = [&]() {\n      vm = (T)Q(in[head[v]], in[v]+1) * vm;\n      v = par[head[v]];\n\
    \    };\n    while (depth[u] > depth[v])\n      u_up();\n    while (depth[u] <\
    \ depth[v])\n      v_up();\n    while (head[u] != head[v]) {\n      u_up();\n\
    \      v_up();\n    }\n    if (in[u] < in[v]) {\n      int l = include_lca ? in[u]\
    \ : in[u]+1;\n      return um * (T)Q(l, in[v]+1) * vm;\n    } else {\n      int\
    \ l = include_lca ? in[v] : in[v]+1;\n      return um * (T)RQ(l, in[u]+1) * vm;\n\
    \    }\n  }\n\n public:\n  template<class T, class Query, class ReverseQuery>\n\
    \  T query(int u, int v, Query Q, ReverseQuery RQ, bool include_lca = true) const\
    \ {\n    return _query<T>(u, v, Q, RQ, include_lca);\n  }\n\n  /// Query for commutative\
    \ monoid\n  template<class T, class Query>\n  T query(int u, int v, Query Q, bool\
    \ include_lca = true) const {\n    return _query<T>(u, v, Q, Q, include_lca);\n\
    \  }\n\n  template<class Set, class T>\n  void set(int i, Set S, T&& val) const\
    \ {\n    S(in[i], std::forward<T>(val));\n  }\n\n  template<typename Upd, typename\
    \ T>\n  void update(int u, int v, Upd U, const T& val, bool include_lca = true)\
    \ const {\n    if (depth[u] > depth[v]) std::swap(u,v);\n    auto up = [&](int&\
    \ v) {\n      U(in[head[v]], in[v]+1, val);\n      v = par[head[v]];\n    };\n\
    \    while (depth[u] < depth[v]) {\n      up(v);\n    }\n    while (head[u] !=\
    \ head[v]) {\n      up(u);\n      up(v);\n    }\n    if (in[u] > in[v]) std::swap(u,v);\n\
    \    int l = include_lca ? in[u] : in[u]+1;\n    U(l, in[v]+1, val);\n  }\n\n\
    public:\n  template<class Add, class Sum>\n  void subtree_build(Add A, Sum S)\
    \ const {\n    dfs_subtree_build(A, S, r);\n  }\n private:\n  template<class Add,\
    \ class Sum>\n  void dfs_subtree_build(Add A, Sum S, int u) const {\n    for (size_t\
    \ i = 0; i < edge[u].size(); i++) {\n      auto v = edge[u][i];\n      if (v ==\
    \ par[u]) continue;\n      dfs_subtree_build(A, S, v);\n      if (i > 0)\n   \
    \     A(in[u], S(in[v], in[v]+clen[v]));\n    }\n  }\n public:\n  template<class\
    \ T, class Sum>\n  T subtree_sum(int r, Sum S) const {\n    return (T)S(in[r],\
    \ in[r]+clen[r]);\n  }\n  template<class T, class Add>\n  void subtree_point_add(int\
    \ u, Add A, const T& val) const {\n    while (u != -1) {\n      A(in[u], val);\n\
    \      u = par[head[u]];\n    }\n  }\n};\n#line 2 \"include/mtl/monoid.hpp\"\n\
    #if __cpp_concepts >= 202002L\n#include <concepts>\n#endif\n\ntemplate<class T,\
    \ T (*op)(T, T), T (*e)()>\nstruct Monoid {\n  T x;\n  Monoid() : x(e()) {}\n\
    \  template<class... Args>\n  Monoid(Args&&... args) : x(std::forward<Args>(args)...)\
    \ {}\n  Monoid operator*(const Monoid& rhs) const {\n    return Monoid(op(x, rhs.x));\n\
    \  }\n};\n\nstruct VoidMonoid {\n  VoidMonoid() {}\n  VoidMonoid operator*(const\
    \ VoidMonoid& rhs) const {\n    return VoidMonoid();\n  }\n};\n\n#if __cpp_concepts\
    \ >= 202002L\ntemplate<class T>\nconcept IsMonoid = requires (T m) {\n  { m *\
    \ m } -> std::same_as<T>;\n};\n#endif\n\ntemplate<class T, T (*op)(T, T), T (*e)()>\n\
    struct CommutativeMonoid : public Monoid<T, op, e> {\n    using Base = Monoid<T,\
    \ op, e>;\n    CommutativeMonoid(T x=e()) : Base(x) {}\n    CommutativeMonoid\
    \ operator+(const CommutativeMonoid& rhs) const {\n        return CommutativeMonoid(*this\
    \ * rhs);\n    }\n};\n\n#if __cpp_concepts >= 202002L\ntemplate<class T>\nconcept\
    \ IsCommutativeMonoid = requires (T m) {\n  { m + m } -> std::same_as<T>;\n};\n\
    #endif\n\ntemplate<class S, class F, S (*mapping)(F, S), S (*composition)(F, S),\
    \ F (*id)()>\nstruct OperatorMonoid {\n    F f;\n    OperatorMonoid() : f(id())\
    \ {}\n    template<class... Args>\n    OperatorMonoid(Args&&... args) : f(std::forward<Args>(args)...)\
    \ {}\n    OperatorMonoid& operator*=(const OperatorMonoid& rhs) {\n        f =\
    \ composition(rhs.f, f);\n        return *this;\n    }\n    S act(const S& s)\
    \ const {\n        return mapping(f, s);\n    }\n};\n\nstruct VoidOperatorMonoid\
    \ {\n    VoidOperatorMonoid() {}\n    VoidOperatorMonoid& operator*=(const VoidOperatorMonoid&\
    \ rhs) {\n        return *this;\n    }\n    template<class T>\n    T act(const\
    \ T& s) const {\n        return s;\n    }\n};\n\n#if __cpp_concepts >= 202002L\n\
    template<class F, class S>\nconcept IsOperatorMonoid = requires (F f, S s) {\n\
    \    { f *= f } -> std::same_as<F&>;\n    { f.act(s) } -> std::same_as<S>;\n};\n\
    #endif\n#line 5 \"include/mtl/segment_hld.hpp\"\n#include <cassert>\r\n\r\ntemplate<typename\
    \ Node>\r\nclass SegmentHldBase {\r\n public:\r\n  using monoid_type = typename\
    \ Node::monoid_type;\r\n protected:\r\n  int n_;\r\n  std::vector<Node> tree_;\r\
    \n  std::vector<int> target_;\r\n public:\r\n  explicit SegmentHldBase(const Hld&\
    \ tree) : n_(tree.n), target_(n_) {\r\n    std::vector<long long> cw(n_+1);\r\n\
    \    for (int i = 0; i < n_; i++) {\r\n      auto w = tree.size[i];\r\n      if\
    \ (!tree.edge[i].empty() and tree.edge[i][0] != tree.par[i])\r\n        w -= tree.size[tree.edge[i][0]];\r\
    \n      cw[i+1] = cw[i] + w;\r\n    }\r\n    tree_.reserve(n_*2);\r\n    tree_.resize(1);\r\
    \n    tree_[0].l = 0;\r\n    tree_[0].r = n_;\r\n    for (int i = 0; i < (int)tree_.size();\
    \ i++) {\r\n      if (tree_[i].size() == 1) {\r\n        target_[tree_[i].l] =\
    \ i;\r\n        continue;\r\n      }\r\n      auto l = tree_[i].l;\r\n      auto\
    \ r = tree_[i].r;\r\n      auto mid = upper_bound(cw.begin()+l, cw.begin()+r,\
    \ (cw[r]+cw[l]+1)/2);\r\n      assert(cw.begin()+l != mid);\r\n      if (*std::prev(mid)-cw[l]\
    \ > cw[r]-*mid)\r\n        --mid;\r\n      int m = mid-cw.begin();\r\n      if\
    \ (l < m) {\r\n        tree_[i].lc = tree_.size();\r\n        tree_.emplace_back();\r\
    \n        tree_.back().l = l;\r\n        tree_.back().r = m;\r\n        tree_.back().p\
    \ = i;\r\n      }\r\n      if (m < r) {\r\n        tree_[i].rc = tree_.size();\r\
    \n        tree_.emplace_back();\r\n        tree_.back().l = m;\r\n        tree_.back().r\
    \ = r;\r\n        tree_.back().p = i;\r\n      }\r\n    }\r\n  }\r\n  template<typename\
    \ InputIt>\r\n  explicit SegmentHldBase(const Hld& tree, InputIt begin, InputIt\
    \ end) : SegmentHldBase(tree) {\r\n    using iterator_value_type = typename std::iterator_traits<InputIt>::value_type;\r\
    \n    static_assert(std::is_convertible<iterator_value_type, monoid_type>::value,\
    \ \r\n                  \"SegmentHldBaseInputIt must be convertible to Monoid\"\
    );\r\n    int i = 0;\r\n    for (auto it = begin; it != end; ++it, ++i) {\r\n\
    \      tree_[target_[i]].set(monoid_type(*it));\r\n    }\r\n    for (int i = (int)tree_.size()-1;\
    \ i >= 0; i--) {\r\n      if (tree_[i].size() == 1) continue;\r\n      tree_[i].take(tree_[tree_[i].lc],\
    \ tree_[tree_[i].rc]);\r\n    }\r\n  }\r\n};\r\n\r\ntemplate<typename M>\r\nstruct\
    \ SegmentHldNode {\r\n  using monoid_type = M;\r\n  int l,r,p=-1,lc=-1,rc=-1;\r\
    \n  monoid_type m, rm;\r\n  int size() const {\r\n    return r-l;\r\n  }\r\n \
    \ void set(const monoid_type& monoid) {\r\n    m = rm = monoid;\r\n  }\r\n  void\
    \ take(const SegmentHldNode& lhs, const SegmentHldNode& rhs) {\r\n    m = lhs.m\
    \ * rhs.m;\r\n    rm = rhs.rm * lhs.rm;\r\n  }\r\n};\r\ntemplate<\r\n#if __cpp_concepts\
    \ >= 202002L\r\n  IsMonoid\r\n#else\r\n  class\r\n#endif\r\n    M>\r\nclass SegmentHld\
    \ : private SegmentHldBase<SegmentHldNode<M>> {\r\n public:\r\n  using monoid_type\
    \ = M; \r\n private:\r\n  using Node = SegmentHldNode<M>;\r\n  using Base = SegmentHldBase<Node>;\r\
    \n  using Base::n_;\r\n  using Base::tree_;\r\n  using Base::target_;\r\n public:\r\
    \n  explicit SegmentHld(const Hld& tree) : Base(tree) {}\r\n  template<typename\
    \ InputIt>\r\n  explicit SegmentHld(const Hld& tree, InputIt begin, InputIt end)\
    \ : Base(tree, begin, end) {}\r\n  const monoid_type& get(int index) const {\r\
    \n    return tree_[target_[index]].m;\r\n  }\r\n  const monoid_type& get_reversed(int\
    \ index) const {\r\n    return tree_[target_[index]].rm;\r\n  }\r\n  template<class...\
    \ Args>\r\n  void set(int index, Args&&... args) {\r\n    int i = target_[index];\r\
    \n    tree_[i].set(M(std::forward<Args>(args)...));\r\n    i = tree_[i].p;\r\n\
    \    while (i != -1) {\r\n      auto lc = tree_[i].lc, rc = tree_[i].rc;\r\n \
    \     tree_[i].take(tree_[lc], tree_[rc]);\r\n      i = tree_[i].p;\r\n    }\r\
    \n  }\r\n  M query(int l, int r) const {\r\n    return _query<0>(l,r,0);\r\n \
    \ }\r\n  M reverse_query(int l, int r) const {\r\n    return _query<1>(l,r,0);\r\
    \n  }\r\n private:\r\n  template<bool Reverse>\r\n  M _query(int l, int r, int\
    \ u) const {\r\n    if (u == -1)\r\n      return M();\r\n    auto _l = tree_[u].l,\
    \ _r = tree_[u].r;\r\n    if (_r <= l or r <= _l)\r\n      return M();\r\n   \
    \ if (l <= _l and _r <= r) {\r\n      if constexpr (!Reverse)\r\n        return\
    \ tree_[u].m;\r\n      else\r\n        return tree_[u].rm;\r\n    }\r\n    auto\
    \ lc = tree_[u].lc, rc = tree_[u].rc;\r\n    if constexpr (!Reverse)\r\n     \
    \ return _query<0>(l, r, lc) * _query<0>(l, r, rc);\r\n    else\r\n      return\
    \ _query<1>(l, r, rc) * _query<1>(l, r, lc);\r\n  }\r\n};\r\n\r\n\r\ntemplate<typename\
    \ M, typename A>\r\nstruct LazySegmentHldNode : SegmentHldNode<M> {\r\n  using\
    \ operator_monoid_type = A;\r\n  A a;\r\n};\r\ntemplate<typename M, typename A>\r\
    \n#if __cpp_concepts >= 202002L\r\nrequires IsMonoid<M> && IsOperatorMonoid<A,\
    \ M>\r\n#endif\r\nclass LazySegmentHld : private SegmentHldBase<LazySegmentHldNode<M,A>>\
    \ {\r\n public:\r\n  using monoid_type = M;\r\n  using operator_monoid_type =\
    \ A;\r\n private:\r\n  using Node = LazySegmentHldNode<M,A>;\r\n  using Base =\
    \ SegmentHldBase<Node>;\r\n  using Base::n_;\r\n  using Base::tree_;\r\n  using\
    \ Base::target_;\r\n public:\r\n  explicit LazySegmentHld(const Hld& tree) : Base(tree)\
    \ {}\r\n  template<typename InputIt>\r\n  explicit LazySegmentHld(const Hld& tree,\
    \ InputIt begin, InputIt end) : Base(tree, begin, end) {}\r\n private:\r\n  inline\
    \ void _propagate(int u) {\r\n    auto& n = tree_[u];\r\n    auto& a = n.a;\r\n\
    \    if (!a()) return;\r\n    n.m = a.act(n.m, n.size());\r\n    n.rm = a.act(n.rm,\
    \ n.size());\r\n    if (n.size() > 1) {\r\n      tree_[n.lc].a *= a;\r\n     \
    \ tree_[n.rc].a *= a;\r\n    }\r\n    n.a = A();\r\n  }\r\n public:\r\n  template<typename\
    \ T>\r\n  void set(int index, T&& v) {\r\n    std::vector<int> ids;\r\n    int\
    \ u = target_[index];\r\n    ids.push_back(u);\r\n    u = tree_[u].p;\r\n    while\
    \ (u != -1) {\r\n      ids.push_back(u);\r\n      u = tree_[u].p;\r\n    }\r\n\
    \    for (int i = (int)ids.size()-1; i >= 0; i--) {\r\n      _propagate(ids[i]);\r\
    \n    }\r\n    tree_[ids[0]].set(monoid_type(std::forward<T>(v)));\r\n    for\
    \ (int i = 1; i < ids.size(); i++) {\r\n      u = ids[i];\r\n      auto lc = tree_[u].lc,\
    \ rc = tree_[u].rc;\r\n      auto ac = lc ^ rc ^ ids[i-1];\r\n      _propagate(ac);\r\
    \n      tree_[u].take(tree_[lc], tree_[rc]);\r\n    }\r\n  }\r\n  M query(int\
    \ l, int r) {\r\n    return _query<0>(l,r,0);\r\n  }\r\n  M reverse_query(int\
    \ l, int r) {\r\n    return _query<1>(l,r,0);\r\n  }\r\n private:\r\n  template<bool\
    \ Reverse>\r\n  M _query(int l, int r, int u) {\r\n    if (u == -1)\r\n      return\
    \ M();\r\n    auto _l = tree_[u].l, _r = tree_[u].r;\r\n    if (_r <= l or r <=\
    \ _l)\r\n      return M();\r\n    _propagate(u);\r\n    if (l <= _l and _r <=\
    \ r) {\r\n      if constexpr (!Reverse)\r\n        return tree_[u].m;\r\n    \
    \  else\r\n        return tree_[u].rm;\r\n    } else {\r\n      if constexpr (!Reverse)\r\
    \n        return _query<0>(l, r, tree_[u].lc) * _query<0>(l, r, tree_[u].rc);\r\
    \n      else\r\n        return _query<1>(l, r, tree_[u].rc) * _query<1>(l, r,\
    \ tree_[u].lc);\r\n    }\r\n  }\r\n public:\r\n  template<typename T>\r\n  void\
    \ update(int l, int r, const T& v) {\r\n    _update(l, r, v, 0);\r\n  }\r\n private:\r\
    \n  template<typename T>\r\n  void _update(int l, int r, const T& v, int u) {\r\
    \n    if (u == -1)\r\n      return;\r\n    auto _l = tree_[u].l, _r = tree_[u].r;\r\
    \n    if (_r <= l or r <= _l) {\r\n      _propagate(u);\r\n    } else if (l <=\
    \ _l and _r <= r) {\r\n      tree_[u].a *= v;\r\n      _propagate(u);\r\n    }\
    \ else {\r\n      _propagate(u);\r\n      if (tree_[u].size() > 1) {\r\n     \
    \   auto lc = tree_[u].lc, rc = tree_[u].rc;\r\n        _update(l, r, v, lc);\r\
    \n        _update(l, r, v, rc);\r\n        tree_[u].take(tree_[lc], tree_[rc]);\r\
    \n      }\r\n    }\r\n  }\r\n};\n#line 4 \"test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp\"\
    \n#include <bits/stdc++.h>\r\n\r\nusing namespace std;\r\n\r\nconstexpr int MINF\
    \ = -1e9;\r\nstruct M {\r\n  int l,r,sum,v;\r\n  M() : v(MINF) {}\r\n  M(int w)\
    \ : l(w),r(w),sum(w),v(w) {}\r\n  friend M operator*(const M& lhs, const M& rhs)\
    \ {\r\n    if (lhs.v == MINF) return rhs;\r\n    if (rhs.v == MINF) return lhs;\r\
    \n    M ret;\r\n    ret.l = max(lhs.l, lhs.sum + rhs.l);\r\n    ret.r = max(lhs.r\
    \ + rhs.sum, rhs.r);\r\n    ret.v = max({lhs.v, lhs.r + rhs.l, rhs.v});\r\n  \
    \  ret.sum = lhs.sum + rhs.sum;\r\n    return ret;\r\n  }\r\n};\r\nM rev(M m)\
    \ {\r\n  M ret = m;\r\n  swap(ret.l, ret.r);\r\n  return ret;\r\n}\r\nstruct A\
    \ {\r\n  int v;\r\n  bool f;\r\n  A() : f(false) {}\r\n  A(int v) : v(v), f(true)\
    \ {}\r\n  bool operator()() const { return f; }\r\n  A& operator*=(const A& r)\
    \ {\r\n    if (r.f) *this = r;\r\n    return *this;\r\n  }\r\n  M act(const M&\
    \ m,int sz) const {\r\n    assert(f);\r\n    M ret;\r\n    ret.sum = v*sz;\r\n\
    \    if (v >= 0) {\r\n      ret.l = ret.r = ret.v = ret.sum;\r\n    } else {\r\
    \n      ret.l = ret.r = ret.v = v;\r\n    }\r\n    return ret;\r\n  }\r\n};\r\n\
    \r\nint main() {\r\n  int n,q; cin>>n>>q;\r\n  vector<int> W(n);\r\n  for (auto&\
    \ w:W) cin>>w;\r\n  Hld T(n);\r\n  for (int i = 0; i < n-1; i++) {\r\n    int\
    \ s,e; cin>>s>>e; s--; e--;\r\n    T.add_edge(s,e);\r\n  }\r\n  T.build();\r\n\
    \  vector<int> X(n);\r\n  for (int i = 0; i < n; i++) X[T.in[i]] = W[i];\r\n \
    \ LazySegmentHld<M,A> RQ(T, X.begin(), X.end());\r\n  auto range_update = [&](int\
    \ l, int r, int v) {\r\n    RQ.update(l,r,v);\r\n  };\r\n  auto query = [&](int\
    \ l, int r) {\r\n    return RQ.query(l,r);\r\n  };\r\n  auto r_query = [&](int\
    \ l, int r) {\r\n    return RQ.reverse_query(l,r);\r\n  };\r\n  for (int i = 0;\
    \ i < q; i++) {\r\n    int t; cin>>t;\r\n    if (t == 1) {\r\n      int a,b,c;\
    \ cin>>a>>b>>c; a--; b--;\r\n      T.update(a,b,range_update, c);\r\n    } else\
    \ if (t == 2) {\r\n      int a,b,c; cin>>a>>b>>c; a--; b--;\r\n      cout << T.query<M>(a,b,query,r_query).v\
    \ << endl;\r\n    }\r\n  }\r\n}\n"
  code: "#define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/2450\"\r\n#include\
    \ \"../../include/mtl/hld.hpp\"\r\n#include \"../../include/mtl/segment_hld.hpp\"\
    \r\n#include <bits/stdc++.h>\r\n\r\nusing namespace std;\r\n\r\nconstexpr int\
    \ MINF = -1e9;\r\nstruct M {\r\n  int l,r,sum,v;\r\n  M() : v(MINF) {}\r\n  M(int\
    \ w) : l(w),r(w),sum(w),v(w) {}\r\n  friend M operator*(const M& lhs, const M&\
    \ rhs) {\r\n    if (lhs.v == MINF) return rhs;\r\n    if (rhs.v == MINF) return\
    \ lhs;\r\n    M ret;\r\n    ret.l = max(lhs.l, lhs.sum + rhs.l);\r\n    ret.r\
    \ = max(lhs.r + rhs.sum, rhs.r);\r\n    ret.v = max({lhs.v, lhs.r + rhs.l, rhs.v});\r\
    \n    ret.sum = lhs.sum + rhs.sum;\r\n    return ret;\r\n  }\r\n};\r\nM rev(M\
    \ m) {\r\n  M ret = m;\r\n  swap(ret.l, ret.r);\r\n  return ret;\r\n}\r\nstruct\
    \ A {\r\n  int v;\r\n  bool f;\r\n  A() : f(false) {}\r\n  A(int v) : v(v), f(true)\
    \ {}\r\n  bool operator()() const { return f; }\r\n  A& operator*=(const A& r)\
    \ {\r\n    if (r.f) *this = r;\r\n    return *this;\r\n  }\r\n  M act(const M&\
    \ m,int sz) const {\r\n    assert(f);\r\n    M ret;\r\n    ret.sum = v*sz;\r\n\
    \    if (v >= 0) {\r\n      ret.l = ret.r = ret.v = ret.sum;\r\n    } else {\r\
    \n      ret.l = ret.r = ret.v = v;\r\n    }\r\n    return ret;\r\n  }\r\n};\r\n\
    \r\nint main() {\r\n  int n,q; cin>>n>>q;\r\n  vector<int> W(n);\r\n  for (auto&\
    \ w:W) cin>>w;\r\n  Hld T(n);\r\n  for (int i = 0; i < n-1; i++) {\r\n    int\
    \ s,e; cin>>s>>e; s--; e--;\r\n    T.add_edge(s,e);\r\n  }\r\n  T.build();\r\n\
    \  vector<int> X(n);\r\n  for (int i = 0; i < n; i++) X[T.in[i]] = W[i];\r\n \
    \ LazySegmentHld<M,A> RQ(T, X.begin(), X.end());\r\n  auto range_update = [&](int\
    \ l, int r, int v) {\r\n    RQ.update(l,r,v);\r\n  };\r\n  auto query = [&](int\
    \ l, int r) {\r\n    return RQ.query(l,r);\r\n  };\r\n  auto r_query = [&](int\
    \ l, int r) {\r\n    return RQ.reverse_query(l,r);\r\n  };\r\n  for (int i = 0;\
    \ i < q; i++) {\r\n    int t; cin>>t;\r\n    if (t == 1) {\r\n      int a,b,c;\
    \ cin>>a>>b>>c; a--; b--;\r\n      T.update(a,b,range_update, c);\r\n    } else\
    \ if (t == 2) {\r\n      int a,b,c; cin>>a>>b>>c; a--; b--;\r\n      cout << T.query<M>(a,b,query,r_query).v\
    \ << endl;\r\n    }\r\n  }\r\n}"
  dependsOn:
  - include/mtl/hld.hpp
  - include/mtl/segment_hld.hpp
  - include/mtl/monoid.hpp
  isVerificationFile: true
  path: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
  requiredBy: []
  timestamp: '2023-04-20 08:14:31+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
layout: document
redirect_from:
- /verify/test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
- /verify/test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp.html
title: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
---
