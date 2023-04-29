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
    path: include/mtl/lazy_segment_tree.hpp
    title: include/mtl/lazy_segment_tree.hpp
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
  bundledCode: "#line 1 \"test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp\"\n\
    #define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/2450\"\r\n#line 2\
    \ \"include/mtl/bit_manip.hpp\"\n#include <cstdint>\n#include <cassert>\n\nnamespace\
    \ bm {\n\ninline constexpr uint64_t popcnt_e8(uint64_t x) {\n  x = (x & 0x5555555555555555)\
    \ + ((x>>1) & 0x5555555555555555);\n  x = (x & 0x3333333333333333) + ((x>>2) &\
    \ 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n\
    \  return x;\n}\n// Count 1s\ninline constexpr unsigned popcnt(uint64_t x) {\n\
    \  return (popcnt_e8(x) * 0x0101010101010101) >> 56;\n}\n// Count trailing 0s.\
    \ ...01101000 -> 3\ninline constexpr unsigned ctz(uint64_t x) {\n  return popcnt((x\
    \ & (-x)) - 1);\n}\ninline constexpr unsigned ctz8(uint8_t x) {\n  return x ==\
    \ 0 ? 8 : popcnt_e8((x & (-x)) - 1);\n}\n// [00..0](8bit) -> 0, [**..*](not only\
    \ 0) -> 1\ninline constexpr uint8_t summary(uint64_t x) {\n  constexpr uint64_t\
    \ hmask = 0x8080808080808080ull;\n  constexpr uint64_t lmask = 0x7F7F7F7F7F7F7F7Full;\n\
    \  auto a = x & hmask;\n  auto b = x & lmask;\n  b = hmask - b;\n  b = ~b;\n \
    \ auto c = (a | b) & hmask;\n  c *= 0x0002040810204081ull;\n  return uint8_t(c\
    \ >> 56);\n}\n// Extract target area of bits\ninline constexpr uint64_t bextr(uint64_t\
    \ x, unsigned start, unsigned len) {\n  uint64_t mask = len < 64 ? (1ull<<len)-1\
    \ : 0xFFFFFFFFFFFFFFFFull;\n  return (x >> start) & mask;\n}\n// 00101101 -> 00111111\
    \ -count_1s-> 6\ninline constexpr unsigned log2p1(uint8_t x) {\n  if (x & 0x80)\n\
    \    return 8;\n  uint64_t p = uint64_t(x) * 0x0101010101010101ull;\n  p -= 0x8040201008040201ull;\n\
    \  p = ~p & 0x8080808080808080ull;\n  p = (p >> 7) * 0x0101010101010101ull;\n\
    \  p >>= 56;\n  return p;\n}\n// 00101100 -mask_mssb-> 00100000 -to_index-> 5\n\
    inline constexpr unsigned mssb8(uint8_t x) {\n  assert(x != 0);\n  return log2p1(x)\
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
    \ 1);\n  return x;\n}\n\n} // namespace bm\n#line 3 \"include/mtl/lazy_segment_tree.hpp\"\
    \n#include <cstddef>\n#include <vector>\n#line 6 \"include/mtl/lazy_segment_tree.hpp\"\
    \n#if __cpp_concepts >= 202002L\n#include <concepts>\n\ntemplate<typename M>\n\
    concept LazySegmentTreeMonoid = requires (M m) {\n  {m * m} -> std::same_as<M>;\n\
    };\ntemplate<typename A, typename M>\nconcept LazySegmentTreeOperatorMonoid =\
    \ requires(A a, M m) {\n  {a()} -> std::same_as<bool>;\n  {a *= a} -> std::same_as<A>;\n\
    \  {a.act(m, 1)} -> std::same_as<M>;\n};\n#endif\n\n\n\ntemplate <typename M,\
    \ typename A>\n#if __cpp_concepts >= 202002L\nrequired LazySegmentTreeMonoid<M>\
    \ &&\n         LazySegmentTreeOperatorMonoid<A,M>\n#endif\nclass LazySegmentTree\
    \ {\n private:\n  size_t size_;\n  std::vector<std::pair<M,A>> tree_;\n  std::vector<std::pair<size_t,\
    \ size_t>> ids_;\n\n public:\n  explicit LazySegmentTree(size_t size) :\n    \
    \  size_(1ull<<(64-bm::clz(size-1))),\n      tree_(size_*2) {\n    ids_.reserve((64-bm::clz(size-1))*2);\n\
    \  }\n\n  template <typename Iter>\n  explicit LazySegmentTree(Iter begin, Iter\
    \ end)\n    : LazySegmentTree(std::distance(begin, end)) {\n    static_assert(std::is_convertible<typename\
    \ std::iterator_traits<Iter>::value_type, M>::value, \"\");\n    for (auto it\
    \ = begin; it != end; ++it) {\n      tree_[size_ + it - begin].first = *it;\n\
    \    }\n    for (size_t i = size_-1; i > 0; i--) {\n      tree_[i].first = tree_[i*2].first\
    \ * tree_[i*2+1].first;\n    }\n  }\n\n  void range_update(size_t l, size_t r,\
    \ const A& e) {\n    assert(l <= r and r <= size_);\n    if (l == r) return;\n\
    \    _set_ids(l, r);\n    for (int i = ids_.size()-1; i >= 0; --i) {\n      _propagate(ids_[i].first,\
    \ ids_[i].second);\n    }\n\n    for (size_t _l=l+size_, _r=r+size_, s=1; _l<_r;\
    \ _l>>=1, _r>>=1, s*=2) {\n      if (_l&1) {\n        tree_[_l].second *= e;\n\
    \        ++_l;\n      }\n      if (_r&1) {\n        --_r;\n        tree_[_r].second\
    \ *= e;\n      }\n    }\n\n    for (auto is : ids_) {\n      auto id = is.first;\n\
    \      auto sz = is.second;\n      _propagate(id*2, sz/2);\n      _propagate(id*2+1,\
    \ sz/2);\n      tree_[id].first = tree_[id*2].first * tree_[id*2+1].first;\n \
    \   }\n  }\n  inline void update(size_t l, size_t r, const A& e) {\n    range_update(l,\
    \ r, e);\n  }\n  inline void update(size_t i, const A& e) {\n    range_update(i,\
    \ i+1, e);\n  }\n\n  template<typename T>\n  inline void set(size_t i, T&& e)\
    \ {\n    _set_ids(i,i+1);\n    for (long long j = ids_.size()-1; j >= 0; --j)\n\
    \      _propagate(ids_[j].first, ids_[j].second);\n    int u = i+size_;\n    tree_[u].first\
    \ = M(std::forward(e));\n    u /= 2;\n    while (u > 0) {\n      tree_[u].first\
    \ = tree_[u*2].first * tree_[u*2+1].first;\n      u /= 2;\n    }\n  }\n\n  inline\
    \ M query(size_t l, size_t r) {\n    _set_ids(l, r);\n    for (int i = ids_.size()-1;\
    \ i >= 0; --i) {\n      _propagate(ids_[i].first, ids_[i].second);\n    }\n\n\
    \    M lhs,rhs;\n    for (size_t _l=l+size_, _r=r+size_, s=1; _l<_r; _l>>=1, _r>>=1,\
    \ s*=2) {\n      if (_l&1) {\n        _propagate(_l, s);\n        lhs = lhs *\
    \ tree_[_l].first;\n        ++_l;\n      }\n      if (_r&1) {\n        --_r;\n\
    \        _propagate(_r, s);\n        rhs = tree_[_r].first * rhs;\n      }\n \
    \   }\n    return lhs * rhs;\n  }\n\n  inline M get(size_t index) {\n    return\
    \ query(index, index+1);\n  }\n\n private:\n  inline void _set_ids(size_t l, size_t\
    \ r) {\n    ids_.clear();\n    auto _l=l+size_, _r=r+size_;\n    auto lth = _l/(_l&(-_l))/2;\n\
    \    auto rth = _r/(_r&(-_r))/2;\n    size_t s = 1;\n    for (; _l<_r; _l>>=1,\
    \ _r>>=1, s*=2) {\n      if (_r <= rth) ids_.emplace_back(_r, s);\n      if (_l\
    \ <= lth) ids_.emplace_back(_l, s);\n    }\n    for (; _l>0; _l>>=1, s*=2) {\n\
    \      ids_.emplace_back(_l, s);\n    }\n  }\n\n  inline void _propagate(size_t\
    \ id, size_t sz) {\n    A e = tree_[id].second;\n    if (!e()) return;\n    tree_[id].second\
    \ = A();\n    tree_[id].first = e.act(tree_[id].first, sz);\n    if (id < size_)\
    \ {\n      tree_[id*2].second *= e;\n      tree_[id*2+1].second *= e;\n    }\n\
    \  }\n\n};\n\n#line 4 \"include/mtl/hld.hpp\"\n\nstruct Hld {\n  int r,n;\n  std::vector<std::vector<int>>\
    \ edge;\n  std::vector<int> size, in, out, head, rev, par, depth, clen;\n private:\n\
    \  void dfs_sz(int v, int p, int d) {\n    par[v] = p;\n    size[v] = 1;\n   \
    \ if (!edge[v].empty() and edge[v][0] == p)\n      std::swap(edge[v][0], edge[v].back());\n\
    \    for (auto& t:edge[v]) {\n      if (t == p) continue;\n      dfs_sz(t, v,\
    \ d+1);\n      size[v] += size[t];\n      if (size[edge[v][0]] < size[t])\n  \
    \      std::swap(edge[v][0], t);\n    }\n  }\n  void dfs_hld(int v, int p, int&\
    \ times) {\n    in[v] = times++;\n    rev[in[v]] = v;\n    clen[v] = 1;\n    if\
    \ (!edge[v].empty() and edge[v][0] != p) {\n      int t = edge[v][0];\n      head[t]\
    \ = head[v];\n      depth[t] = depth[v];\n      dfs_hld(t, v, times);\n      clen[v]\
    \ += clen[t];\n    }\n    for (size_t i = 1; i < edge[v].size(); i++) {\n    \
    \  int t = edge[v][i];\n      if (t == p) continue;\n      head[t] = t;\n    \
    \  depth[t] = depth[v] + 1;\n      dfs_hld(t, v, times);\n    }\n    out[v] =\
    \ times;\n  }\n\n public:\n  Hld(int n) : r(0), n(n), edge(n), size(n), in(n),\
    \ out(n), head(n), rev(n), par(n), depth(n), clen(n) {}\n\n  inline void add_edge(int\
    \ a, int b) {\n    edge[a].push_back(b);\n    edge[b].push_back(a);\n  }\n\n \
    \ void build(int root = 0) {\n    r = root;\n    dfs_sz(root, -1, 0);\n    int\
    \ t = 0;\n    head[root] = root;\n    depth[root] = 0;\n    dfs_hld(root, -1,\
    \ t);\n  }\n\n  inline int lca(int a, int b) const {\n    if (depth[a] > depth[b])\
    \ std::swap(a, b);\n    while (depth[a] < depth[b]) {\n      b = par[head[b]];\n\
    \    }\n    while (head[a] != head[b]) {\n      a = par[head[a]];\n      b = par[head[b]];\n\
    \    }\n    return in[a] < in[b] ? a : b;\n  }\n\n private:\n  template<class\
    \ T, class Query, class ReverseQuery>\n  T _query(int u, int v, Query Q, ReverseQuery\
    \ RQ, bool include_lca) const {\n    T um, vm;\n    auto u_up = [&]() {\n    \
    \  um = um * (T)RQ(in[head[u]], in[u]+1);\n      u = par[head[u]];\n    };\n \
    \   auto v_up = [&]() {\n      vm = (T)Q(in[head[v]], in[v]+1) * vm;\n      v\
    \ = par[head[v]];\n    };\n    while (depth[u] > depth[v])\n      u_up();\n  \
    \  while (depth[u] < depth[v])\n      v_up();\n    while (head[u] != head[v])\
    \ {\n      u_up();\n      v_up();\n    }\n    if (in[u] < in[v]) {\n      int\
    \ l = include_lca ? in[u] : in[u]+1;\n      return um * (T)Q(l, in[v]+1) * vm;\n\
    \    } else {\n      int l = include_lca ? in[v] : in[v]+1;\n      return um *\
    \ (T)RQ(l, in[u]+1) * vm;\n    }\n  }\n\n public:\n  template<class T, class Query,\
    \ class ReverseQuery>\n  T query(int u, int v, Query Q, ReverseQuery RQ, bool\
    \ include_lca = true) const {\n    return _query<T>(u, v, Q, RQ, include_lca);\n\
    \  }\n\n  /// Query for commutative monoid\n  template<class T, class Query>\n\
    \  T query(int u, int v, Query Q, bool include_lca = true) const {\n    return\
    \ _query<T>(u, v, Q, Q, include_lca);\n  }\n\n  template<class Set, class T>\n\
    \  void set(int i, Set S, T&& val) const {\n    S(in[i], std::forward<T>(val));\n\
    \  }\n\n  template<typename Upd, typename T>\n  void update(int u, int v, Upd\
    \ U, const T& val, bool include_lca = true) const {\n    if (depth[u] > depth[v])\
    \ std::swap(u,v);\n    auto up = [&](int& v) {\n      U(in[head[v]], in[v]+1,\
    \ val);\n      v = par[head[v]];\n    };\n    while (depth[u] < depth[v]) {\n\
    \      up(v);\n    }\n    while (head[u] != head[v]) {\n      up(u);\n      up(v);\n\
    \    }\n    if (in[u] > in[v]) std::swap(u,v);\n    int l = include_lca ? in[u]\
    \ : in[u]+1;\n    U(l, in[v]+1, val);\n  }\n\npublic:\n  template<class Add, class\
    \ Sum>\n  void subtree_build(Add A, Sum S) const {\n    dfs_subtree_build(A, S,\
    \ r);\n  }\n private:\n  template<class Add, class Sum>\n  void dfs_subtree_build(Add\
    \ A, Sum S, int u) const {\n    for (size_t i = 0; i < edge[u].size(); i++) {\n\
    \      auto v = edge[u][i];\n      if (v == par[u]) continue;\n      dfs_subtree_build(A,\
    \ S, v);\n      if (i > 0)\n        A(in[u], S(in[v], in[v]+clen[v]));\n    }\n\
    \  }\n public:\n  template<class T, class Sum>\n  T subtree_sum(int r, Sum S)\
    \ const {\n    return (T)S(in[r], in[r]+clen[r]);\n  }\n  template<class T, class\
    \ Add>\n  void subtree_point_add(int u, Add A, const T& val) const {\n    while\
    \ (u != -1) {\n      A(in[u], val);\n      u = par[head[u]];\n    }\n  }\n};\n\
    #line 4 \"test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp\"\n#include <bits/stdc++.h>\r\
    \n\r\nusing namespace std;\r\n\r\nconstexpr int MINF = -1e9;\r\nstruct M {\r\n\
    \  int l,r,sum,v;\r\n  M() : v(MINF) {}\r\n  M(int w) : l(w),r(w),sum(w),v(w)\
    \ {}\r\n  friend M operator*(const M& lhs, const M& rhs) {\r\n    if (lhs.v ==\
    \ MINF) return rhs;\r\n    if (rhs.v == MINF) return lhs;\r\n    M ret;\r\n  \
    \  ret.l = max(lhs.l, lhs.sum + rhs.l);\r\n    ret.r = max(rhs.r, lhs.r + rhs.sum);\r\
    \n    ret.v = max({lhs.v, rhs.v, lhs.r + rhs.l});\r\n    ret.sum = lhs.sum + rhs.sum;\r\
    \n    return ret;\r\n  }\r\n  M operator~() {\r\n    M ret = *this;\r\n    swap(ret.l,\
    \ ret.r);\r\n    return ret;\r\n  }\r\n};\r\nstruct A {\r\n  int v;\r\n  bool\
    \ f;\r\n  A() : f(false) {}\r\n  A(int v) : v(v), f(true) {}\r\n  bool operator()()\
    \ const { return f; }\r\n  A& operator*=(const A& r) {\r\n    if (r.f) *this =\
    \ r;\r\n    return *this;\r\n  }\r\n  M act(const M& m,int sz) const {\r\n   \
    \ assert(f);\r\n    M ret;\r\n    ret.sum = v*sz;\r\n    if (v >= 0) {\r\n   \
    \   ret.l = ret.r = ret.v = ret.sum;\r\n    } else {\r\n      ret.l = ret.r =\
    \ ret.v = v;\r\n    }\r\n    return ret;\r\n  }\r\n};\r\n\r\nint main() {\r\n\
    \  int n,q; cin>>n>>q;\r\n  vector<int> W(n);\r\n  for (auto& w:W) cin>>w;\r\n\
    \  Hld T(n);\r\n  for (int i = 0; i < n-1; i++) {\r\n    int s,e; cin>>s>>e; s--;\
    \ e--;\r\n    T.add_edge(s,e);\r\n  }\r\n  T.build();\r\n  vector<int> X(n*2);\r\
    \n  for (int i = 0; i < n; i++)\r\n    X[T.in[i]] = X[n+n-1-T.in[i]] = W[i];\r\
    \n  LazySegmentTree<M,A> RQ(X.begin(), X.end());\r\n  auto range_update = [&](int\
    \ l, int r, int v) {\r\n    RQ.update(l,r,v);\r\n    RQ.update(n+n-r, n+n-l, v);\r\
    \n  };\r\n  auto query = [&](int l, int r) {\r\n    return RQ.query(l,r);\r\n\
    \  };\r\n  auto reverse_query = [&](int l, int r) {\r\n    return RQ.query(n+n-r,\
    \ n+n-l);\r\n  };\r\n  for (int i = 0; i < q; i++) {\r\n    int t; cin>>t;\r\n\
    \    if (t == 1) {\r\n      int a,b,c; cin>>a>>b>>c; a--; b--;\r\n      T.update(a,b,range_update,c);\r\
    \n    } else if (t == 2) {\r\n      int a,b,c; cin>>a>>b>>c; a--; b--;\r\n   \
    \   cout << T.query<M>(a,b,query,reverse_query).v << endl;\r\n    }\r\n  }\r\n\
    }\n"
  code: "#define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/2450\"\r\n#include\
    \ \"../../include/mtl/lazy_segment_tree.hpp\"\r\n#include \"../../include/mtl/hld.hpp\"\
    \r\n#include <bits/stdc++.h>\r\n\r\nusing namespace std;\r\n\r\nconstexpr int\
    \ MINF = -1e9;\r\nstruct M {\r\n  int l,r,sum,v;\r\n  M() : v(MINF) {}\r\n  M(int\
    \ w) : l(w),r(w),sum(w),v(w) {}\r\n  friend M operator*(const M& lhs, const M&\
    \ rhs) {\r\n    if (lhs.v == MINF) return rhs;\r\n    if (rhs.v == MINF) return\
    \ lhs;\r\n    M ret;\r\n    ret.l = max(lhs.l, lhs.sum + rhs.l);\r\n    ret.r\
    \ = max(rhs.r, lhs.r + rhs.sum);\r\n    ret.v = max({lhs.v, rhs.v, lhs.r + rhs.l});\r\
    \n    ret.sum = lhs.sum + rhs.sum;\r\n    return ret;\r\n  }\r\n  M operator~()\
    \ {\r\n    M ret = *this;\r\n    swap(ret.l, ret.r);\r\n    return ret;\r\n  }\r\
    \n};\r\nstruct A {\r\n  int v;\r\n  bool f;\r\n  A() : f(false) {}\r\n  A(int\
    \ v) : v(v), f(true) {}\r\n  bool operator()() const { return f; }\r\n  A& operator*=(const\
    \ A& r) {\r\n    if (r.f) *this = r;\r\n    return *this;\r\n  }\r\n  M act(const\
    \ M& m,int sz) const {\r\n    assert(f);\r\n    M ret;\r\n    ret.sum = v*sz;\r\
    \n    if (v >= 0) {\r\n      ret.l = ret.r = ret.v = ret.sum;\r\n    } else {\r\
    \n      ret.l = ret.r = ret.v = v;\r\n    }\r\n    return ret;\r\n  }\r\n};\r\n\
    \r\nint main() {\r\n  int n,q; cin>>n>>q;\r\n  vector<int> W(n);\r\n  for (auto&\
    \ w:W) cin>>w;\r\n  Hld T(n);\r\n  for (int i = 0; i < n-1; i++) {\r\n    int\
    \ s,e; cin>>s>>e; s--; e--;\r\n    T.add_edge(s,e);\r\n  }\r\n  T.build();\r\n\
    \  vector<int> X(n*2);\r\n  for (int i = 0; i < n; i++)\r\n    X[T.in[i]] = X[n+n-1-T.in[i]]\
    \ = W[i];\r\n  LazySegmentTree<M,A> RQ(X.begin(), X.end());\r\n  auto range_update\
    \ = [&](int l, int r, int v) {\r\n    RQ.update(l,r,v);\r\n    RQ.update(n+n-r,\
    \ n+n-l, v);\r\n  };\r\n  auto query = [&](int l, int r) {\r\n    return RQ.query(l,r);\r\
    \n  };\r\n  auto reverse_query = [&](int l, int r) {\r\n    return RQ.query(n+n-r,\
    \ n+n-l);\r\n  };\r\n  for (int i = 0; i < q; i++) {\r\n    int t; cin>>t;\r\n\
    \    if (t == 1) {\r\n      int a,b,c; cin>>a>>b>>c; a--; b--;\r\n      T.update(a,b,range_update,c);\r\
    \n    } else if (t == 2) {\r\n      int a,b,c; cin>>a>>b>>c; a--; b--;\r\n   \
    \   cout << T.query<M>(a,b,query,reverse_query).v << endl;\r\n    }\r\n  }\r\n\
    }"
  dependsOn:
  - include/mtl/lazy_segment_tree.hpp
  - include/mtl/bit_manip.hpp
  - include/mtl/hld.hpp
  isVerificationFile: true
  path: test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp
  requiredBy: []
  timestamp: '2023-04-19 10:11:27+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp
layout: document
redirect_from:
- /verify/test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp
- /verify/test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp.html
title: test/aoj/aoj-do_use_segment_tree-binary_tree.test.cpp
---
