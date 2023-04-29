---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':question:'
    path: include/mtl/hld.hpp
    title: include/mtl/hld.hpp
  - icon: ':question:'
    path: include/mtl/modular.hpp
    title: include/mtl/modular.hpp
  - icon: ':question:'
    path: include/mtl/monoid.hpp
    title: include/mtl/monoid.hpp
  - icon: ':question:'
    path: include/mtl/segment_hld.hpp
    title: include/mtl/segment_hld.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/vertex_set_path_composite
    links:
    - https://judge.yosupo.jp/problem/vertex_set_path_composite
  bundledCode: "#line 1 \"test/yosupo/vertex_set_path_composite.test.cpp\"\n#define\
    \ PROBLEM \"https://judge.yosupo.jp/problem/vertex_set_path_composite\"\n#line\
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
    \n      }\r\n    }\r\n  }\r\n};\n#line 2 \"include/mtl/bit_manip.hpp\"\n#include\
    \ <cstdint>\n#line 4 \"include/mtl/bit_manip.hpp\"\n\nnamespace bm {\n\ninline\
    \ constexpr uint64_t popcnt_e8(uint64_t x) {\n  x = (x & 0x5555555555555555) +\
    \ ((x>>1) & 0x5555555555555555);\n  x = (x & 0x3333333333333333) + ((x>>2) & 0x3333333333333333);\n\
    \  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n  return x;\n\
    }\n// Count 1s\ninline constexpr unsigned popcnt(uint64_t x) {\n  return (popcnt_e8(x)\
    \ * 0x0101010101010101) >> 56;\n}\n// Count trailing 0s. ...01101000 -> 3\ninline\
    \ constexpr unsigned ctz(uint64_t x) {\n  return popcnt((x & (-x)) - 1);\n}\n\
    inline constexpr unsigned ctz8(uint8_t x) {\n  return x == 0 ? 8 : popcnt_e8((x\
    \ & (-x)) - 1);\n}\n// [00..0](8bit) -> 0, [**..*](not only 0) -> 1\ninline constexpr\
    \ uint8_t summary(uint64_t x) {\n  constexpr uint64_t hmask = 0x8080808080808080ull;\n\
    \  constexpr uint64_t lmask = 0x7F7F7F7F7F7F7F7Full;\n  auto a = x & hmask;\n\
    \  auto b = x & lmask;\n  b = hmask - b;\n  b = ~b;\n  auto c = (a | b) & hmask;\n\
    \  c *= 0x0002040810204081ull;\n  return uint8_t(c >> 56);\n}\n// Extract target\
    \ area of bits\ninline constexpr uint64_t bextr(uint64_t x, unsigned start, unsigned\
    \ len) {\n  uint64_t mask = len < 64 ? (1ull<<len)-1 : 0xFFFFFFFFFFFFFFFFull;\n\
    \  return (x >> start) & mask;\n}\n// 00101101 -> 00111111 -count_1s-> 6\ninline\
    \ constexpr unsigned log2p1(uint8_t x) {\n  if (x & 0x80)\n    return 8;\n  uint64_t\
    \ p = uint64_t(x) * 0x0101010101010101ull;\n  p -= 0x8040201008040201ull;\n  p\
    \ = ~p & 0x8080808080808080ull;\n  p = (p >> 7) * 0x0101010101010101ull;\n  p\
    \ >>= 56;\n  return p;\n}\n// 00101100 -mask_mssb-> 00100000 -to_index-> 5\ninline\
    \ constexpr unsigned mssb8(uint8_t x) {\n  assert(x != 0);\n  return log2p1(x)\
    \ - 1;\n}\n// 00101100 -mask_lssb-> 00000100 -to_index-> 2\ninline constexpr unsigned\
    \ lssb8(uint8_t x) {\n  assert(x != 0);\n  return popcnt_e8((x & -x) - 1);\n}\n\
    // Count leading 0s. 00001011... -> 4\ninline constexpr unsigned clz(uint64_t\
    \ x) {\n  if (x == 0)\n    return 64;\n  auto i = mssb8(summary(x));\n  auto j\
    \ = mssb8(bextr(x, 8 * i, 8));\n  return 63 - (8 * i + j);\n}\ninline constexpr\
    \ unsigned clz8(uint8_t x) {\n  return x == 0 ? 8 : 7 - mssb8(x);\n}\ninline constexpr\
    \ uint64_t bit_reverse(uint64_t x) {\n  x = ((x & 0x00000000FFFFFFFF) << 32) |\
    \ ((x & 0xFFFFFFFF00000000) >> 32);\n  x = ((x & 0x0000FFFF0000FFFF) << 16) |\
    \ ((x & 0xFFFF0000FFFF0000) >> 16);\n  x = ((x & 0x00FF00FF00FF00FF) << 8) | ((x\
    \ & 0xFF00FF00FF00FF00) >> 8);\n  x = ((x & 0x0F0F0F0F0F0F0F0F) << 4) | ((x &\
    \ 0xF0F0F0F0F0F0F0F0) >> 4);\n  x = ((x & 0x3333333333333333) << 2) | ((x & 0xCCCCCCCCCCCCCCCC)\
    \ >> 2);\n  x = ((x & 0x5555555555555555) << 1) | ((x & 0xAAAAAAAAAAAAAAAA) >>\
    \ 1);\n  return x;\n}\n\n} // namespace bm\n#line 3 \"include/mtl/modular.hpp\"\
    \n#include <iostream>\n#line 5 \"include/mtl/modular.hpp\"\n\ntemplate <int MOD>\n\
    class Modular {\n private:\n  unsigned int val_;\n\n public:\n  static constexpr\
    \ unsigned int mod() { return MOD; }\n  template<class T>\n  static constexpr\
    \ unsigned int safe_mod(T v) {\n    auto x = (long long)(v%(long long)mod());\n\
    \    if (x < 0) x += mod();\n    return (unsigned int) x;\n  }\n\n  constexpr\
    \ Modular() : val_(0) {}\n  template<class T,\n      std::enable_if_t<\n     \
    \     std::is_integral<T>::value && std::is_unsigned<T>::value\n      > * = nullptr>\n\
    \  constexpr Modular(T v) : val_(v%mod()) {}\n  template<class T,\n      std::enable_if_t<\n\
    \          std::is_integral<T>::value && !std::is_unsigned<T>::value\n      >\
    \ * = nullptr>\n  constexpr Modular(T v) : val_(safe_mod(v)) {}\n\n  constexpr\
    \ unsigned int val() const { return val_; }\n  constexpr Modular& operator+=(Modular\
    \ x) {\n    val_ += x.val();\n    if (val_ >= mod()) val_ -= mod();\n    return\
    \ *this;\n  }\n  constexpr Modular operator-() const { return {mod() - val_};\
    \ }\n  constexpr Modular& operator-=(Modular x) {\n    val_ += mod() - x.val();\n\
    \    if (val_ >= mod()) val_ -= mod();\n    return *this;\n  }\n  constexpr Modular&\
    \ operator*=(Modular x) {\n    auto v = (long long) val_ * x.val();\n    if (v\
    \ >= mod()) v %= mod();\n    val_ = v;\n    return *this;\n  }\n  constexpr Modular\
    \ pow(long long p) const {\n    assert(p >= 0);\n    Modular t = 1;\n    Modular\
    \ u = *this;\n    while (p) {\n      if (p & 1)\n        t *= u;\n      u *= u;\n\
    \      p >>= 1;\n    }\n    return t;\n  }\n  friend constexpr Modular pow(Modular\
    \ x, long long p) {\n    return x.pow(p);\n  }\n  constexpr Modular inv() const\
    \ { return pow(mod()-2); }\n  constexpr Modular& operator/=(Modular x) { return\
    \ *this *= x.inv(); }\n  constexpr Modular operator+(Modular x) const { return\
    \ Modular(*this) += x; }\n  constexpr Modular operator-(Modular x) const { return\
    \ Modular(*this) -= x; }\n  constexpr Modular operator*(Modular x) const { return\
    \ Modular(*this) *= x; }\n  constexpr Modular operator/(Modular x) const { return\
    \ Modular(*this) /= x; }\n  constexpr Modular& operator++() { return *this +=\
    \ 1; }\n  constexpr Modular operator++(int) { Modular c = *this; ++(*this); return\
    \ c; }\n  constexpr Modular& operator--() { return *this -= 1; }\n  constexpr\
    \ Modular operator--(int) { Modular c = *this; --(*this); return c; }\n\n  constexpr\
    \ bool operator==(Modular x) const { return val() == x.val(); }\n  constexpr bool\
    \ operator!=(Modular x) const { return val() != x.val(); }\n\n  constexpr bool\
    \ is_square() const {\n    return pow((mod()-1)/2) == 1;\n  }\n  /**\n   * Return\
    \ x s.t. x * x = a mod p\n   * reference: https://zenn.dev/peria/articles/c6afc72b6b003c\n\
    \  */\n  constexpr Modular sqrt() const {\n    if (!is_square()) \n      throw\
    \ std::runtime_error(\"not square\");\n    auto mod_eight = mod() % 8;\n    if\
    \ (mod_eight == 3 || mod_eight == 7) {\n      return pow((mod()+1)/4);\n    }\
    \ else if (mod_eight == 5) {\n      auto x = pow((mod()+3)/8);\n      if (x *\
    \ x != *this)\n        x *= Modular(2).pow((mod()-1)/4);\n      return x;\n  \
    \  } else {\n      Modular d = 2;\n      while (d.is_square())\n        d += 1;\n\
    \      auto t = mod()-1;\n      int s = bm::ctz(t);\n      t >>= s;\n      auto\
    \ a = pow(t);\n      auto D = d.pow(t);\n      int m = 0;\n      Modular dt =\
    \ 1;\n      Modular du = D;\n      for (int i = 0; i < s; i++) {\n        if ((a*dt).pow(1u<<(s-1-i))\
    \ == -1) {\n          m |= 1u << i;\n          dt *= du;\n        }\n        du\
    \ *= du;\n      }\n      return pow((t+1)/2) * D.pow(m/2);\n    }\n  }\n\n  friend\
    \ std::ostream& operator<<(std::ostream& os, const Modular& x) {\n    return os\
    \ << x.val();\n  }\n  friend std::istream& operator>>(std::istream& is, Modular&\
    \ x) {\n    return is >> x.val_;\n  }\n\n};\n\nusing Modular998244353 = Modular<998244353>;\n\
    using Modular1000000007 = Modular<(int)1e9+7>;\n\ntemplate<int Id=0>\nclass DynamicModular\
    \ {\n private:\n  static unsigned int mod_;\n  unsigned int val_;\n\n public:\n\
    \  static unsigned int mod() { return mod_; }\n  static void set_mod(unsigned\
    \ int m) { mod_ = m; }\n  template<class T>\n  static constexpr unsigned int safe_mod(T\
    \ v) {\n    auto x = (long long)(v%(long long)mod());\n    if (x < 0) x += mod();\n\
    \    return (unsigned int) x;\n  }\n\n  constexpr DynamicModular() : val_(0) {}\n\
    \  template<class T,\n      std::enable_if_t<\n          std::is_integral<T>::value\
    \ && std::is_unsigned<T>::value\n      > * = nullptr>\n  constexpr DynamicModular(T\
    \ v) : val_(v%mod()) {}\n  template<class T,\n      std::enable_if_t<\n      \
    \    std::is_integral<T>::value && !std::is_unsigned<T>::value\n      > * = nullptr>\n\
    \  constexpr DynamicModular(T v) : val_(safe_mod(v)) {}\n\n  constexpr unsigned\
    \ int val() const { return val_; }\n  constexpr DynamicModular& operator+=(DynamicModular\
    \ x) {\n    val_ += x.val();\n    if (val_ >= mod()) val_ -= mod();\n    return\
    \ *this;\n  }\n  constexpr DynamicModular operator-() const { return {mod() -\
    \ val_}; }\n  constexpr DynamicModular& operator-=(DynamicModular x) {\n    val_\
    \ += mod() - x.val();\n    if (val_ >= mod()) val_ -= mod();\n    return *this;\n\
    \  }\n  constexpr DynamicModular& operator*=(DynamicModular x) {\n    auto v =\
    \ (long long) val_ * x.val();\n    if (v >= mod()) v %= mod();\n    val_ = v;\n\
    \    return *this;\n  }\n  constexpr DynamicModular pow(long long p) const {\n\
    \    assert(p >= 0);\n    DynamicModular t = 1;\n    DynamicModular u = *this;\n\
    \    while (p) {\n      if (p & 1)\n        t *= u;\n      u *= u;\n      p >>=\
    \ 1;\n    }\n    return t;\n  }\n  friend constexpr DynamicModular pow(DynamicModular\
    \ x, long long p) {\n    return x.pow(p);\n  }\n  // TODO: implement when mod\
    \ is not prime\n  constexpr DynamicModular inv() const { return pow(mod()-2);\
    \ }\n  constexpr DynamicModular& operator/=(DynamicModular x) { return *this *=\
    \ x.inv(); }\n  constexpr DynamicModular operator+(DynamicModular x) const { return\
    \ DynamicModular(*this) += x; }\n  constexpr DynamicModular operator-(DynamicModular\
    \ x) const { return DynamicModular(*this) -= x; }\n  constexpr DynamicModular\
    \ operator*(DynamicModular x) const { return DynamicModular(*this) *= x; }\n \
    \ constexpr DynamicModular operator/(DynamicModular x) const { return DynamicModular(*this)\
    \ /= x; }\n  constexpr DynamicModular& operator++() { return *this += 1; }\n \
    \ constexpr DynamicModular operator++(int) { DynamicModular c = *this; ++(*this);\
    \ return c; }\n  constexpr DynamicModular& operator--() { return *this -= 1; }\n\
    \  constexpr DynamicModular operator--(int) { DynamicModular c = *this; --(*this);\
    \ return c; }\n\n  constexpr bool operator==(DynamicModular x) const { return\
    \ val() == x.val(); }\n  constexpr bool operator!=(DynamicModular x) const { return\
    \ val() != x.val(); }\n\n  constexpr bool is_square() const {\n    return val()\
    \ == 0 or pow((mod()-1)/2) == 1;\n  }\n  /**\n   * Return x s.t. x * x = a mod\
    \ p\n   * reference: https://zenn.dev/peria/articles/c6afc72b6b003c\n  */\n  constexpr\
    \ DynamicModular sqrt() const {\n    // assert mod is prime\n    if (!is_square())\
    \ \n      throw std::runtime_error(\"not square\");\n    if (val() < 2)\n    \
    \  return val();\n    auto mod_eight = mod() % 8;\n    if (mod_eight == 3 || mod_eight\
    \ == 7) {\n      return pow((mod()+1)/4);\n    } else if (mod_eight == 5) {\n\
    \      auto x = pow((mod()+3)/8);\n      if (x * x != *this)\n        x *= DynamicModular(2).pow((mod()-1)/4);\n\
    \      return x;\n    } else {\n      DynamicModular d = 2;\n      while (d.is_square())\n\
    \        ++d;\n      auto t = mod()-1;\n      int s = bm::ctz(t);\n      t >>=\
    \ s;\n      auto a = pow(t);\n      auto D = d.pow(t);\n      int m = 0;\n   \
    \   DynamicModular dt = 1;\n      DynamicModular du = D;\n      for (int i = 0;\
    \ i < s; i++) {\n        if ((a*dt).pow(1u<<(s-1-i)) == -1) {\n          m |=\
    \ 1u << i;\n          dt *= du;\n        }\n        du *= du;\n      }\n     \
    \ return pow((t+1)/2) * D.pow(m/2);\n    }\n  }\n\n  friend std::ostream& operator<<(std::ostream&\
    \ os, const DynamicModular& x) {\n    return os << x.val();\n  }\n  friend std::istream&\
    \ operator>>(std::istream& is, DynamicModular& x) {\n    return is >> x.val_;\n\
    \  }\n\n};\ntemplate<int Id>\nunsigned int DynamicModular<Id>::mod_;\n\n#line\
    \ 264 \"include/mtl/modular.hpp\"\n\ntemplate<class ModInt>\nstruct ModularUtil\
    \ {\n  static constexpr int mod = ModInt::mod();\n  static struct inv_table {\n\
    \    std::vector<ModInt> tb{0,1};\n    inv_table() : tb({0,1}) {}\n  } inv_;\n\
    \  void set_inv(int n) {\n    int m = inv_.tb.size();\n    if (m > n) return;\n\
    \    inv_.tb.resize(n+1);\n    for (int i = m; i < n+1; i++)\n      inv_.tb[i]\
    \ = -inv_.tb[mod % i] * (mod / i);\n  }\n  ModInt& inv(int i) {\n    set_inv(i);\n\
    \    return inv_.tb[i];\n  }\n};\ntemplate<class ModInt>\ntypename ModularUtil<ModInt>::inv_table\
    \ ModularUtil<ModInt>::inv_;\n\n#include <array>\n\nnamespace math {\n\nconstexpr\
    \ int mod_pow_constexpr(int x, int p, int m) {\n  int t = 1;\n  int u = x;\n \
    \ while (p) {\n    if (p & 1) {\n      t *= u;\n      t %= m;\n    }\n    u *=\
    \ u;\n    u %= m;\n    p >>= 1;\n  }\n  return t;\n}\n\nconstexpr int primitive_root_constexpr(int\
    \ m) {\n  if (m == 2) return 1;\n  if (m == 998244353) return 3;\n\n  std::array<int,\
    \ 20> divs{2};\n  int cnt = 1;\n  int x = (m-1) / 2;\n  for (int d = 3; d*d <=\
    \ x; d += 2) {\n    if (x % d == 0) {\n      divs[cnt++] = d;\n      while (x\
    \ % d == 0)\n        x /= d;\n    }\n  }\n  if (x > 1) divs[cnt++] = x;\n  for\
    \ (int g = 2; ; g++) {\n    bool ok = true;\n    for (int i = 0; i < cnt; i++)\
    \ {\n      if (mod_pow_constexpr(g, (m-1) / divs[cnt], m) == 1) {\n        ok\
    \ = false;\n        break;\n      }\n    }\n    if (ok) return g;\n  }\n}\n\n\
    template<int m>\nconstexpr int primitive_root = primitive_root_constexpr(m);\n\
    \n}\n#line 5 \"test/yosupo/vertex_set_path_composite.test.cpp\"\n#include <bits/stdc++.h>\n\
    using namespace std;\n\nusing mint = Modular<998244353>;\n\nstruct Composite {\n\
    \    mint a, b;\n    Composite(mint a=1, mint b=0):a(a),b(b) {}\n    Composite(pair<int,int>\
    \ p):a(p.first),b(p.second) {}\n    Composite operator*(const Composite& o) const\
    \ { \n        return Composite(a*o.a, b*o.a + o.b);\n    }\n    mint eval(mint\
    \ x) const {\n        return a*x+b;\n    }\n};\n\nint main() {\n    int n,q; cin>>n>>q;\n\
    \    vector<pair<int,int>> C(n);\n    for (int i = 0; i < n; i++) {\n        int\
    \ a,b; cin>>a>>b;\n        C[i] = {a,b};\n    }\n    Hld T(n);\n    for (int i\
    \ = 0; i < n-1; i++) {\n        int u,v; cin>>u>>v;\n        T.add_edge(u,v);\n\
    \    } \n    T.build();\n    decltype(C) D(n);\n    for (int i = 0; i < n; i++)\
    \ \n        D[T.in[i]] = C[i];\n    SegmentHld<Composite> path_sum(T, D.begin(),\
    \ D.end());\n    for (int i = 0; i < q; i++) {\n        int t; cin>>t;\n     \
    \   if (t == 0) {\n            int p,c,d; cin>>p>>c>>d;\n            T.set(p,\
    \ [&](auto i, auto v) {path_sum.set(i, v);}, Composite(c, d));\n        } else\
    \ {\n            int u,v,x; cin>>u>>v>>x;\n            auto lq = [&](int l, int\
    \ r) { \n                return path_sum.query(l, r); \n            };\n     \
    \       auto rq = [&](int l, int r) { \n                return path_sum.reverse_query(l,\
    \ r); \n            };\n            auto ret = T.query<Composite>(u,v,lq,rq).eval(x);\n\
    \            cout << ret << endl;\n        }\n    }\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/vertex_set_path_composite\"\
    \n#include \"../../include/mtl/hld.hpp\"\n#include \"../../include/mtl/segment_hld.hpp\"\
    \n#include \"../../include/mtl/modular.hpp\"\n#include <bits/stdc++.h>\nusing\
    \ namespace std;\n\nusing mint = Modular<998244353>;\n\nstruct Composite {\n \
    \   mint a, b;\n    Composite(mint a=1, mint b=0):a(a),b(b) {}\n    Composite(pair<int,int>\
    \ p):a(p.first),b(p.second) {}\n    Composite operator*(const Composite& o) const\
    \ { \n        return Composite(a*o.a, b*o.a + o.b);\n    }\n    mint eval(mint\
    \ x) const {\n        return a*x+b;\n    }\n};\n\nint main() {\n    int n,q; cin>>n>>q;\n\
    \    vector<pair<int,int>> C(n);\n    for (int i = 0; i < n; i++) {\n        int\
    \ a,b; cin>>a>>b;\n        C[i] = {a,b};\n    }\n    Hld T(n);\n    for (int i\
    \ = 0; i < n-1; i++) {\n        int u,v; cin>>u>>v;\n        T.add_edge(u,v);\n\
    \    } \n    T.build();\n    decltype(C) D(n);\n    for (int i = 0; i < n; i++)\
    \ \n        D[T.in[i]] = C[i];\n    SegmentHld<Composite> path_sum(T, D.begin(),\
    \ D.end());\n    for (int i = 0; i < q; i++) {\n        int t; cin>>t;\n     \
    \   if (t == 0) {\n            int p,c,d; cin>>p>>c>>d;\n            T.set(p,\
    \ [&](auto i, auto v) {path_sum.set(i, v);}, Composite(c, d));\n        } else\
    \ {\n            int u,v,x; cin>>u>>v>>x;\n            auto lq = [&](int l, int\
    \ r) { \n                return path_sum.query(l, r); \n            };\n     \
    \       auto rq = [&](int l, int r) { \n                return path_sum.reverse_query(l,\
    \ r); \n            };\n            auto ret = T.query<Composite>(u,v,lq,rq).eval(x);\n\
    \            cout << ret << endl;\n        }\n    }\n}\n"
  dependsOn:
  - include/mtl/hld.hpp
  - include/mtl/segment_hld.hpp
  - include/mtl/monoid.hpp
  - include/mtl/modular.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: true
  path: test/yosupo/vertex_set_path_composite.test.cpp
  requiredBy: []
  timestamp: '2023-04-29 13:51:09+09:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: test/yosupo/vertex_set_path_composite.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/vertex_set_path_composite.test.cpp
- /verify/test/yosupo/vertex_set_path_composite.test.cpp.html
title: test/yosupo/vertex_set_path_composite.test.cpp
---
