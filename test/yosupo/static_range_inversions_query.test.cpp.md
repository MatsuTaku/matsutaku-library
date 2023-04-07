---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/fenwick_tree.hpp
    title: include/mtl/fenwick_tree.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/mo.hpp
    title: include/mtl/mo.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/static_range_inversions_query
    links:
    - https://judge.yosupo.jp/problem/static_range_inversions_query
  bundledCode: "#line 1 \"test/yosupo/static_range_inversions_query.test.cpp\"\n#define\
    \ PROBLEM \"https://judge.yosupo.jp/problem/static_range_inversions_query\"\r\n\
    #line 2 \"include/mtl/mo.hpp\"\n#include <algorithm>\r\n#include <cmath>\r\n#include\
    \ <tuple>\r\n#include <vector>\r\n\r\ntemplate <typename T, T (*pushl)(T, int),\
    \ T (*pushr)(T, int), T (*popl)(T, int),\r\n          T (*popr)(T, int), T (*e)()>\r\
    \nclass MoSolver {\r\n private:\r\n  std::vector<std::tuple<int, int, int>> segs;\r\
    \n  int n = 0;\r\n\r\n public:\r\n  MoSolver() = default;\r\n  void add_segment(int\
    \ l, int r) { segs.emplace_back(n++, l, r); }\r\n  void solve(std::vector<T> &dst)\
    \ {\r\n    using std::get;\r\n    sort(segs.begin(), segs.end(), [m = (int)std::sqrt(n)](auto\
    \ &l, auto &r) {\r\n      int ll = get<1>(l), rl = get<2>(l), lr = get<1>(r),\
    \ rr = get<2>(r);\r\n      int bl = ll / m;\r\n      int br = lr / m;\r\n    \
    \  return (bl != br) ? bl < br : (rl < rr) ^ (bl % 2);\r\n    });\r\n    dst.resize(n);\r\
    \n    int _l = 0, _r = 0;\r\n    T val = e();\r\n    for (auto &ilr : segs) {\r\
    \n      int i = get<0>(ilr), l = get<1>(ilr), r = get<2>(ilr);\r\n      while\
    \ (l < _l)\r\n        val = pushl(val, --_l);\r\n      while (_r < r)\r\n    \
    \    val = pushr(val, _r++);\r\n      while (_l < l)\r\n        val = popl(val,\
    \ _l++);\r\n      while (r < _r)\r\n        val = popr(val, --_r);\r\n      dst[i]\
    \ = val;\r\n    }\r\n  }\r\n};\r\n#line 2 \"include/mtl/bit_manip.hpp\"\n#include\
    \ <cstdint>\n#include <cassert>\n\nnamespace bm {\n\ninline constexpr uint64_t\
    \ popcnt_e8(uint64_t x) {\n  x = (x & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);\n\
    \  x = (x & 0x3333333333333333) + ((x>>2) & 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F)\
    \ + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n  return x;\n}\n// Count 1s\ninline constexpr\
    \ unsigned popcnt(uint64_t x) {\n  return (popcnt_e8(x) * 0x0101010101010101)\
    \ >> 56;\n}\n// Count trailing 0s. ...01101000 -> 3\ninline constexpr unsigned\
    \ ctz(uint64_t x) {\n  return popcnt((x & (-x)) - 1);\n}\ninline constexpr unsigned\
    \ ctz8(uint8_t x) {\n  return x == 0 ? 8 : popcnt_e8((x & (-x)) - 1);\n}\n// [00..0](8bit)\
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
    \ 3 \"include/mtl/fenwick_tree.hpp\"\n#include <cstddef>\n#line 5 \"include/mtl/fenwick_tree.hpp\"\
    \n\ntemplate <typename T>\nclass FenwickTree {\n private:\n  std::vector<T> tree_;\n\
    \n public:\n  FenwickTree() = default;\n  explicit FenwickTree(size_t size) :\
    \ tree_(size+1) {}\n\n  size_t size() const { return tree_.size()-1; }\n\n  template\
    \ <typename Iter>\n  explicit FenwickTree(Iter begin, Iter end) : FenwickTree(end-begin)\
    \ {\n    for (auto it = begin; it != end; ++it)\n      add(it-begin, *it);\n \
    \ }\n\n  void add(size_t index, T x) {\n    for (size_t i = index+1; i < tree_.size();\
    \ i += i&(-i))\n      tree_[i] += x;\n  }\n\n  T sum(size_t index) const {\n \
    \   T sum = 0;\n    for (size_t i = index+1; i > 0; i -= i&(-i))\n      sum +=\
    \ tree_[i];\n    return sum;\n  }\n\n  T range_sum(size_t l, size_t r) const {\n\
    \    auto sl = l > 0 ? sum(l-1) : 0;\n    auto sr = r > 0 ? sum(r-1) : 0;\n  \
    \  return sr - sl;\n  }\n\n  size_t lower_bound(T _sum) const {\n    size_t ret\
    \ = 0;\n    T s = 0;\n    for (int k = 63-bm::clz(size()); k >= 0; k--) {\n  \
    \    size_t j = ret | (1ull<<k);\n      if (j < tree_.size() and s + tree_[j]\
    \ < _sum) {\n        s += tree_[j];\n        ret = j;\n      }\n    }\n    return\
    \ ret;\n  }\n\n};\n\n#line 4 \"test/yosupo/static_range_inversions_query.test.cpp\"\
    \n#include <bits/stdc++.h>\r\nusing namespace std;\r\nusing lint = long long;\r\
    \n\r\nconstexpr int N = 1e5;\r\nFenwickTree<lint> C(N);\r\nint A[N];\r\nmap<int,int>\
    \ id;\r\nlint pushl(lint inv, int i) {\r\n  int vi = id[A[i]];\r\n  inv += C.range_sum(0,\
    \ vi);\r\n  C.add(vi, 1);\r\n  return inv;\r\n}\r\nlint pushr(lint inv, int i)\
    \ {\r\n  int vi = id[A[i]];\r\n  inv += C.range_sum(vi+1, N);\r\n  C.add(vi, 1);\r\
    \n  return inv;\r\n}\r\nlint popl(lint inv, int i) {\r\n  int vi = id[A[i]];\r\
    \n  C.add(vi, -1);\r\n  inv -= C.range_sum(0, vi);\r\n  return inv;\r\n}\r\nlint\
    \ popr(lint inv, int i) {\r\n  int vi = id[A[i]];\r\n  C.add(vi, -1);\r\n  inv\
    \ -= C.range_sum(vi+1, N);\r\n  return inv;\r\n}\r\nlint e() { return 0; }\r\n\
    \r\nint main() {\r\n  int n,q; cin>>n>>q;\r\n  for (int i = 0; i < n; i++) {\r\
    \n    cin>>A[i];\r\n    id[A[i]] = 0;\r\n  }\r\n  {\r\n    int k = 0;\r\n    for\
    \ (auto& [i,v] : id) v = k++;\r\n  }\r\n  MoSolver<lint, pushl, pushr, popl, popr,\
    \ e> mo;\r\n  for (int i = 0; i < q; i++) {\r\n    int l,r; cin>>l>>r;\r\n   \
    \ mo.add_segment(l, r);\r\n  }\r\n  vector<lint> ans;\r\n  mo.solve(ans);\r\n\
    \  for (lint v : ans) cout << v << endl;\r\n}\r\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/static_range_inversions_query\"\
    \r\n#include \"../../include/mtl/mo.hpp\"\r\n#include \"../../include/mtl/fenwick_tree.hpp\"\
    \r\n#include <bits/stdc++.h>\r\nusing namespace std;\r\nusing lint = long long;\r\
    \n\r\nconstexpr int N = 1e5;\r\nFenwickTree<lint> C(N);\r\nint A[N];\r\nmap<int,int>\
    \ id;\r\nlint pushl(lint inv, int i) {\r\n  int vi = id[A[i]];\r\n  inv += C.range_sum(0,\
    \ vi);\r\n  C.add(vi, 1);\r\n  return inv;\r\n}\r\nlint pushr(lint inv, int i)\
    \ {\r\n  int vi = id[A[i]];\r\n  inv += C.range_sum(vi+1, N);\r\n  C.add(vi, 1);\r\
    \n  return inv;\r\n}\r\nlint popl(lint inv, int i) {\r\n  int vi = id[A[i]];\r\
    \n  C.add(vi, -1);\r\n  inv -= C.range_sum(0, vi);\r\n  return inv;\r\n}\r\nlint\
    \ popr(lint inv, int i) {\r\n  int vi = id[A[i]];\r\n  C.add(vi, -1);\r\n  inv\
    \ -= C.range_sum(vi+1, N);\r\n  return inv;\r\n}\r\nlint e() { return 0; }\r\n\
    \r\nint main() {\r\n  int n,q; cin>>n>>q;\r\n  for (int i = 0; i < n; i++) {\r\
    \n    cin>>A[i];\r\n    id[A[i]] = 0;\r\n  }\r\n  {\r\n    int k = 0;\r\n    for\
    \ (auto& [i,v] : id) v = k++;\r\n  }\r\n  MoSolver<lint, pushl, pushr, popl, popr,\
    \ e> mo;\r\n  for (int i = 0; i < q; i++) {\r\n    int l,r; cin>>l>>r;\r\n   \
    \ mo.add_segment(l, r);\r\n  }\r\n  vector<lint> ans;\r\n  mo.solve(ans);\r\n\
    \  for (lint v : ans) cout << v << endl;\r\n}\r\n"
  dependsOn:
  - include/mtl/mo.hpp
  - include/mtl/fenwick_tree.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: true
  path: test/yosupo/static_range_inversions_query.test.cpp
  requiredBy: []
  timestamp: '2023-04-08 02:15:04+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/yosupo/static_range_inversions_query.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/static_range_inversions_query.test.cpp
- /verify/test/yosupo/static_range_inversions_query.test.cpp.html
title: test/yosupo/static_range_inversions_query.test.cpp
---
