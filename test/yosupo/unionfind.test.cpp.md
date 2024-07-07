---
data:
  _extendedDependsOn:
  - icon: ':x:'
    path: include/mtl/unionfind.hpp
    title: include/mtl/unionfind.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/unionfind
    links:
    - https://judge.yosupo.jp/problem/unionfind
  bundledCode: "#line 1 \"test/yosupo/unionfind.test.cpp\"\n#define PROBLEM \"https://judge.yosupo.jp/problem/unionfind\"\
    \r\n#line 2 \"include/mtl/unionfind.hpp\"\n#include <vector>\n#include <numeric>\n\
    #include <cstddef>\n\nclass UnionFind {\n private:\n  // par_[i] = { size if par_[i]\
    \ < 0, root otherwise }\n  std::vector<int> par_;\n\n public:\n  UnionFind() =\
    \ default;\n  explicit UnionFind(size_t size) : par_(size, -1) {}\n\n  int root(int\
    \ u) {\n    if (par_[u] < 0)\n      return u;\n    return par_[u] = root(par_[u]);\n\
    \  }\n\n  bool are_union(int u, int v) {\n    return root(u) == root(v);\n  }\n\
    \n  int size_of(int u) {\n    return -par_[root(u)];\n  }\n\n  bool unite(int\
    \ u, int v) {\n    if (are_union(u,v))\n      return false;\n    if (size_of(u)\
    \ < size_of(v))\n      std::swap(u,v);\n    par_[root(u)] -= size_of(v);\n   \
    \ par_[root(v)] = root(u);\n    return true;\n  }\n};\n#line 3 \"test/yosupo/unionfind.test.cpp\"\
    \n#include <iostream>\r\nusing namespace std;\r\nusing ll = long long;\r\n\r\n\
    int main() {\r\n  cin.tie(nullptr); ios::sync_with_stdio(false);\r\n\r\n  int\
    \ N,Q; cin>>N>>Q;\r\n  UnionFind uf(N);\r\n\r\n  for (int q = 0; q < Q; q++) {\r\
    \n    int t; cin>>t;\r\n    int u,v; cin>>u>>v;\r\n    if (t == 0) {\r\n     \
    \ uf.unite(u,v);\r\n    } else if (t == 1) {\r\n      int ans = uf.are_union(u,v)\
    \ ? 1 : 0;\r\n      cout << ans << endl;\r\n    }\r\n  }\r\n\r\n  return 0;\r\n\
    }\r\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/unionfind\"\r\n#include\
    \ \"../../include/mtl/unionfind.hpp\"\r\n#include <iostream>\r\nusing namespace\
    \ std;\r\nusing ll = long long;\r\n\r\nint main() {\r\n  cin.tie(nullptr); ios::sync_with_stdio(false);\r\
    \n\r\n  int N,Q; cin>>N>>Q;\r\n  UnionFind uf(N);\r\n\r\n  for (int q = 0; q <\
    \ Q; q++) {\r\n    int t; cin>>t;\r\n    int u,v; cin>>u>>v;\r\n    if (t == 0)\
    \ {\r\n      uf.unite(u,v);\r\n    } else if (t == 1) {\r\n      int ans = uf.are_union(u,v)\
    \ ? 1 : 0;\r\n      cout << ans << endl;\r\n    }\r\n  }\r\n\r\n  return 0;\r\n\
    }\r\n"
  dependsOn:
  - include/mtl/unionfind.hpp
  isVerificationFile: true
  path: test/yosupo/unionfind.test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: test/yosupo/unionfind.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/unionfind.test.cpp
- /verify/test/yosupo/unionfind.test.cpp.html
title: test/yosupo/unionfind.test.cpp
---
