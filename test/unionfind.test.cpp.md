---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/unionfind.hpp
    title: include/mtl/unionfind.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/unionfind
    links:
    - https://judge.yosupo.jp/problem/unionfind
  bundledCode: "#line 1 \"test/unionfind.test.cpp\"\n#define PROBLEM \"https://judge.yosupo.jp/problem/unionfind\"\
    \n#line 2 \"include/mtl/unionfind.hpp\"\n#include <vector>\n#include <numeric>\n\
    #include <cstddef>\n\nclass UnionFind {\n private:\n  // par_[i] = { size if par_[i]\
    \ < 0, root otherwise }\n  std::vector<int> par_;\n\n public:\n  UnionFind() =\
    \ default;\n  explicit UnionFind(size_t size) : par_(size, -1) {}\n\n  int root(int\
    \ u) {\n    if (par_[u] < 0)\n      return u;\n    return par_[u] = root(par_[u]);\n\
    \  }\n\n  bool are_union(int u, int v) {\n    return root(u) == root(v);\n  }\n\
    \n  int size_of(int u) {\n    return -par_[root(u)];\n  }\n\n  bool unite(int\
    \ u, int v) {\n    if (are_union(u,v))\n      return false;\n    if (size_of(u)\
    \ < size_of(v))\n      std::swap(u,v);\n    par_[root(u)] -= size_of(v);\n   \
    \ par_[root(v)] = root(u);\n    return true;\n  }\n};\n#line 3 \"test/unionfind.test.cpp\"\
    \n#include <iostream>\nusing namespace std;\nusing ll = long long;\n\nint main()\
    \ {\n  cin.tie(nullptr); ios::sync_with_stdio(false);\n\n  int N,Q; cin>>N>>Q;\n\
    \  UnionFind uf(N);\n\n  for (int q = 0; q < Q; q++) {\n    int t; cin>>t;\n \
    \   int u,v; cin>>u>>v;\n    if (t == 0) {\n      uf.unite(u,v);\n    } else if\
    \ (t == 1) {\n      int ans = uf.are_union(u,v) ? 1 : 0;\n      cout << ans <<\
    \ endl;\n    }\n  }\n\n  return 0;\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/unionfind\"\n#include \"\
    ../include/mtl/unionfind.hpp\"\n#include <iostream>\nusing namespace std;\nusing\
    \ ll = long long;\n\nint main() {\n  cin.tie(nullptr); ios::sync_with_stdio(false);\n\
    \n  int N,Q; cin>>N>>Q;\n  UnionFind uf(N);\n\n  for (int q = 0; q < Q; q++) {\n\
    \    int t; cin>>t;\n    int u,v; cin>>u>>v;\n    if (t == 0) {\n      uf.unite(u,v);\n\
    \    } else if (t == 1) {\n      int ans = uf.are_union(u,v) ? 1 : 0;\n      cout\
    \ << ans << endl;\n    }\n  }\n\n  return 0;\n}\n"
  dependsOn:
  - include/mtl/unionfind.hpp
  isVerificationFile: true
  path: test/unionfind.test.cpp
  requiredBy: []
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/unionfind.test.cpp
layout: document
redirect_from:
- /verify/test/unionfind.test.cpp
- /verify/test/unionfind.test.cpp.html
title: test/unionfind.test.cpp
---
