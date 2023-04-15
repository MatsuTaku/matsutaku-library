---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/fenwick_tree.hpp
    title: include/mtl/fenwick_tree.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/hld.hpp
    title: include/mtl/hld.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/vertex_add_subtree_sum
    links:
    - https://judge.yosupo.jp/problem/vertex_add_subtree_sum
  bundledCode: "#line 1 \"test/yosupo/vertex_add_subtree_sum.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.yosupo.jp/problem/vertex_add_subtree_sum\"\n#line 2 \"include/mtl/hld.hpp\"\
    \n#include <cstddef>\n#include <vector>\n\nstruct Hld {\n  int r,n;\n  std::vector<std::vector<int>>\
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
    #line 2 \"include/mtl/bit_manip.hpp\"\n#include <cstdint>\n#include <cassert>\n\
    \nnamespace bm {\n\ninline constexpr uint64_t popcnt_e8(uint64_t x) {\n  x = (x\
    \ & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);\n  x = (x & 0x3333333333333333)\
    \ + ((x>>2) & 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) &\
    \ 0x0F0F0F0F0F0F0F0F);\n  return x;\n}\n// Count 1s\ninline constexpr unsigned\
    \ popcnt(uint64_t x) {\n  return (popcnt_e8(x) * 0x0101010101010101) >> 56;\n\
    }\n// Count trailing 0s. ...01101000 -> 3\ninline constexpr unsigned ctz(uint64_t\
    \ x) {\n  return popcnt((x & (-x)) - 1);\n}\ninline constexpr unsigned ctz8(uint8_t\
    \ x) {\n  return x == 0 ? 8 : popcnt_e8((x & (-x)) - 1);\n}\n// [00..0](8bit)\
    \ -> 0, [**..*](not only 0) -> 1\ninline constexpr uint8_t summary(uint64_t x)\
    \ {\n  constexpr uint64_t hmask = 0x8080808080808080ull;\n  constexpr uint64_t\
    \ lmask = 0x7F7F7F7F7F7F7F7Full;\n  auto a = x & hmask;\n  auto b = x & lmask;\n\
    \  b = hmask - b;\n  b = ~b;\n  auto c = (a | b) & hmask;\n  c *= 0x0002040810204081ull;\n\
    \  return uint8_t(c >> 56);\n}\n// Extract target area of bits\ninline constexpr\
    \ uint64_t bextr(uint64_t x, unsigned start, unsigned len) {\n  uint64_t mask\
    \ = len < 64 ? (1ull<<len)-1 : 0xFFFFFFFFFFFFFFFFull;\n  return (x >> start) &\
    \ mask;\n}\n// 00101101 -> 00111111 -count_1s-> 6\ninline constexpr unsigned log2p1(uint8_t\
    \ x) {\n  if (x & 0x80)\n    return 8;\n  uint64_t p = uint64_t(x) * 0x0101010101010101ull;\n\
    \  p -= 0x8040201008040201ull;\n  p = ~p & 0x8080808080808080ull;\n  p = (p >>\
    \ 7) * 0x0101010101010101ull;\n  p >>= 56;\n  return p;\n}\n// 00101100 -mask_mssb->\
    \ 00100000 -to_index-> 5\ninline constexpr unsigned mssb8(uint8_t x) {\n  assert(x\
    \ != 0);\n  return log2p1(x) - 1;\n}\n// 00101100 -mask_lssb-> 00000100 -to_index->\
    \ 2\ninline constexpr unsigned lssb8(uint8_t x) {\n  assert(x != 0);\n  return\
    \ popcnt_e8((x & -x) - 1);\n}\n// Count leading 0s. 00001011... -> 4\ninline constexpr\
    \ unsigned clz(uint64_t x) {\n  if (x == 0)\n    return 64;\n  auto i = mssb8(summary(x));\n\
    \  auto j = mssb8(bextr(x, 8 * i, 8));\n  return 63 - (8 * i + j);\n}\ninline\
    \ constexpr unsigned clz8(uint8_t x) {\n  return x == 0 ? 8 : 7 - mssb8(x);\n\
    }\ninline constexpr uint64_t bit_reverse(uint64_t x) {\n  x = ((x & 0x00000000FFFFFFFF)\
    \ << 32) | ((x & 0xFFFFFFFF00000000) >> 32);\n  x = ((x & 0x0000FFFF0000FFFF)\
    \ << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);\n  x = ((x & 0x00FF00FF00FF00FF)\
    \ << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);\n  x = ((x & 0x0F0F0F0F0F0F0F0F) <<\
    \ 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);\n  x = ((x & 0x3333333333333333) << 2)\
    \ | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);\n  x = ((x & 0x5555555555555555) << 1) |\
    \ ((x & 0xAAAAAAAAAAAAAAAA) >> 1);\n  return x;\n}\n\n} // namespace bm\n#line\
    \ 5 \"include/mtl/fenwick_tree.hpp\"\n\ntemplate <class T>\nclass FenwickTree\
    \ {\n private:\n  std::vector<T> tree_;\n\n public:\n  FenwickTree() = default;\n\
    \  explicit FenwickTree(size_t size) : tree_(size+1) {}\n\n  size_t size() const\
    \ { return tree_.size()-1; }\n\n  template <class Iter>\n  explicit FenwickTree(Iter\
    \ begin, Iter end) : FenwickTree(std::distance(begin, end)) {\n    size_t i =\
    \ 1;\n    for (auto it = begin; it != end; ++it) {\n      tree_[i] = tree_[i]\
    \ + *it;\n      auto j = i + (i&(-i));\n      if (j < tree_.size())\n        tree_[j]\
    \ = tree_[j] + tree_[i];\n      ++i;\n    }\n  }\n\n  template<class V>\n  void\
    \ add(size_t index, const V& x) {\n    for (size_t i = index+1; i < tree_.size();\
    \ i += i&(-i))\n      tree_[i] = tree_[i] + x;\n  }\n\n  T sum(size_t index) const\
    \ {\n    T sum = 0;\n    for (size_t i = index+1; i > 0; i -= i&(-i))\n      sum\
    \ = sum + tree_[i];\n    return sum;\n  }\n\n  T range_sum(size_t l, size_t r)\
    \ const {\n    auto sl = l > 0 ? sum(l-1) : 0;\n    auto sr = r > 0 ? sum(r-1)\
    \ : 0;\n    return sr - sl;\n  }\n\n  template<class V>\n  size_t lower_bound(const\
    \ V& _sum) const {\n    size_t ret = 0;\n    T s = 0;\n    for (int k = 63-bm::clz(size());\
    \ k >= 0; k--) {\n      size_t j = ret | (1ull<<k);\n      if (j < tree_.size()\
    \ and s + tree_[j] < _sum) {\n        s = s + tree_[j];\n        ret = j;\n  \
    \    }\n    }\n    return ret;\n  }\n\n};\n\n#line 4 \"test/yosupo/vertex_add_subtree_sum.test.cpp\"\
    \n#include <bits/stdc++.h>\nusing namespace std;\n\nint main() {\n    int n,q;\
    \ cin>>n>>q;\n    vector<int> A(n);\n    for (int i = 0; i < n; i++) cin>>A[i];\n\
    \    Hld T(n);\n    for (int i = 1; i < n; i++) {\n        int p; cin>>p;\n  \
    \      T.add_edge(p, i);\n    }\n    T.build(0);\n    decltype(A) B(n);\n    for\
    \ (int i = 0; i < n; i++) B[T.in[i]] = A[i];\n    FenwickTree<long long> ft(B.begin(),\
    \ B.end());\n    auto add = [&](int i, auto v) { ft.add(i, v); };\n    auto sum\
    \ = [&](int l, int r) { return ft.range_sum(l, r); };\n    T.subtree_build(add,\
    \ sum);\n    for (int i = 0; i < q; i++) {\n        int t; cin>>t;\n        if\
    \ (t == 0) {\n            int u,x; cin>>u>>x;\n            T.subtree_point_add(u,\
    \ add, x);\n        } else {\n            int u; cin>>u;\n            cout <<\
    \ T.subtree_sum<long long>(u, sum) << endl;\n        }\n    }\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/vertex_add_subtree_sum\"\
    \n#include \"../../include/mtl/hld.hpp\"\n#include \"../../include/mtl/fenwick_tree.hpp\"\
    \n#include <bits/stdc++.h>\nusing namespace std;\n\nint main() {\n    int n,q;\
    \ cin>>n>>q;\n    vector<int> A(n);\n    for (int i = 0; i < n; i++) cin>>A[i];\n\
    \    Hld T(n);\n    for (int i = 1; i < n; i++) {\n        int p; cin>>p;\n  \
    \      T.add_edge(p, i);\n    }\n    T.build(0);\n    decltype(A) B(n);\n    for\
    \ (int i = 0; i < n; i++) B[T.in[i]] = A[i];\n    FenwickTree<long long> ft(B.begin(),\
    \ B.end());\n    auto add = [&](int i, auto v) { ft.add(i, v); };\n    auto sum\
    \ = [&](int l, int r) { return ft.range_sum(l, r); };\n    T.subtree_build(add,\
    \ sum);\n    for (int i = 0; i < q; i++) {\n        int t; cin>>t;\n        if\
    \ (t == 0) {\n            int u,x; cin>>u>>x;\n            T.subtree_point_add(u,\
    \ add, x);\n        } else {\n            int u; cin>>u;\n            cout <<\
    \ T.subtree_sum<long long>(u, sum) << endl;\n        }\n    }\n}"
  dependsOn:
  - include/mtl/hld.hpp
  - include/mtl/fenwick_tree.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: true
  path: test/yosupo/vertex_add_subtree_sum.test.cpp
  requiredBy: []
  timestamp: '2023-04-13 21:51:40+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/yosupo/vertex_add_subtree_sum.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/vertex_add_subtree_sum.test.cpp
- /verify/test/yosupo/vertex_add_subtree_sum.test.cpp.html
title: test/yosupo/vertex_add_subtree_sum.test.cpp
---
