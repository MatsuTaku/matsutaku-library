---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 1 \"test/succinct/wavelet_tree_test.cpp\"\n#include <succinct/wavelet_tree.hpp>\r\
    \n#include <bits/stdc++.h>\r\nusing namespace std;\r\n\r\nint main() {\r\n  const\
    \ int n = 1e6;\r\n  std::vector<char> S(n);\r\n  std::array<int, 26> cnt{};\r\n\
    \  std::array<std::vector<int>, 26> rank, select, rank_c;\r\n  for (int i = 0;\
    \ i < 26; i++) {\r\n    rank[i].resize(n+1);\r\n    rank_c[i].resize(n+1);\r\n\
    \  }\r\n  for (int i = 0; i < n; i++) {\r\n    int c = rand()%26;\r\n    S[i]\
    \ = c;\r\n    for (int j = 0; j < 26; j++) {\r\n      rank[j][i] = cnt[j];\r\n\
    \      if (j) rank_c[j][i] = rank_c[j-1][i] + cnt[j-1];\r\n    }\r\n    cnt[c]++;\r\
    \n    select[c].push_back(i);\r\n  }\r\n  for (int i = 0; i < 26; i++) {\r\n \
    \   rank[i][n] = cnt[i];\r\n    if (i) rank_c[i][n] = rank_c[i-1][n] + cnt[i-1];\r\
    \n    select[i].push_back(n);\r\n  }\r\n\r\n  WaveletTree<char> wm(S.begin(),\
    \ S.end());\r\n  // access\r\n  for (int i = 0; i < n; i++) {\r\n    char v =\
    \ wm.get(i);\r\n    if (v != S[i]) {\r\n      cout << \"Failed access \"<<i<<\"\
    \ wm.get: \"<<char(v+'a')<<\" != S: \"<<char(S[i]+'a')<<endl;\r\n      return\
    \ 1;\r\n    }\r\n  }\r\n  // rank\r\n  for (int i = 0; i <= n; i++) {\r\n    for\
    \ (int c = 0; c < 26; c++) {\r\n      int v = wm.rank(c, i);\r\n      if (v !=\
    \ rank[c][i]) {\r\n        cout << \"Failed rank \"<<char(c+'a')<<\", \"<<i<<\"\
    \ wm.rank: \"<<v<<\" != rank: \"<<rank[c][i]<<endl;\r\n        return 1;\r\n \
    \     }\r\n    }\r\n  }\r\n  // select\r\n  std::array<int, 26> selected{};\r\n\
    \  for (int i = 0; i < n; i++) {\r\n    int c = S[i];\r\n    int k = selected[c]++;\r\
    \n    int v = wm.select(c, k);\r\n    if (v != select[c][k] or v != i) {\r\n \
    \     cout << \"Failed select \"<<char(S[i]+'a')<<\", \"<<k<<\" wm.select: \"\
    <<v<<\" != select: \"<<select[c][k]<<endl;\r\n      return 1;\r\n    }\r\n  }\r\
    \n  for (int c = 0; c < 26; c++) {\r\n    int k = selected[c];\r\n    int v =\
    \ wm.select(c, k);\r\n    if (v != select[c][k] or v != n) {\r\n      cout <<\
    \ \"Failed select \"<<char(c + 'a')<<\", \"<<k<<\" wm.select: \"<<v<<\" != select:\
    \ \"<<select[c][k]<<endl;\r\n      return 1;\r\n    }\r\n  }\r\n  // rank_c\r\n\
    \  for (int i = 0; i <= n; i++) {\r\n    for (int c = 0; c < 26; c++) {\r\n  \
    \    int v = wm.rank_lower(c, i);\r\n      if (v != rank_c[c][i]) {\r\n      \
    \  cout << \"Failed rank_c \"<<char(c+'a')<<\", \"<<i<<\" wm.rank_lower: \"<<v<<\"\
    \ != rank_c: \"<<rank_c[c][i]<<endl;\r\n        return 1;\r\n      }\r\n    }\r\
    \n  }\r\n  // enumerate\r\n  const int m = std::min(n, (int)cbrt(n*6));\r\n  for\
    \ (int l = 0; l < m; l++) {\r\n    for (int r = l+1; r <= m; r++) {\r\n      std::array<int,\
    \ 26> enm{};\r\n      for (int c = 0; c < 26; c++) enm[c] = rank[c][r] - rank[c][l];\r\
    \n      auto _v = wm.enumerate(l, r);\r\n      std::array<int, 26> v{};\r\n  \
    \    for (auto p : _v) v[p.first] = p.second;\r\n      for (int c = 0; c < 26;\
    \ c++) {\r\n        if (v[c] != enm[c]) {\r\n          cout << \"Failed enumerate\
    \ \"<<l<<\", \"<<r<<\", \"<<char(c+'a')<<\" wm.enumerate: \"<<v[c]<<\" != \"<<enm[c]<<endl;\r\
    \n          return 1;\r\n        }\r\n      }\r\n    }\r\n  }\r\n  std::cout <<\
    \ \"OK\" << std::endl;\r\n}\n"
  code: "#include <succinct/wavelet_tree.hpp>\r\n#include <bits/stdc++.h>\r\nusing\
    \ namespace std;\r\n\r\nint main() {\r\n  const int n = 1e6;\r\n  std::vector<char>\
    \ S(n);\r\n  std::array<int, 26> cnt{};\r\n  std::array<std::vector<int>, 26>\
    \ rank, select, rank_c;\r\n  for (int i = 0; i < 26; i++) {\r\n    rank[i].resize(n+1);\r\
    \n    rank_c[i].resize(n+1);\r\n  }\r\n  for (int i = 0; i < n; i++) {\r\n   \
    \ int c = rand()%26;\r\n    S[i] = c;\r\n    for (int j = 0; j < 26; j++) {\r\n\
    \      rank[j][i] = cnt[j];\r\n      if (j) rank_c[j][i] = rank_c[j-1][i] + cnt[j-1];\r\
    \n    }\r\n    cnt[c]++;\r\n    select[c].push_back(i);\r\n  }\r\n  for (int i\
    \ = 0; i < 26; i++) {\r\n    rank[i][n] = cnt[i];\r\n    if (i) rank_c[i][n] =\
    \ rank_c[i-1][n] + cnt[i-1];\r\n    select[i].push_back(n);\r\n  }\r\n\r\n  WaveletTree<char>\
    \ wm(S.begin(), S.end());\r\n  // access\r\n  for (int i = 0; i < n; i++) {\r\n\
    \    char v = wm.get(i);\r\n    if (v != S[i]) {\r\n      cout << \"Failed access\
    \ \"<<i<<\" wm.get: \"<<char(v+'a')<<\" != S: \"<<char(S[i]+'a')<<endl;\r\n  \
    \    return 1;\r\n    }\r\n  }\r\n  // rank\r\n  for (int i = 0; i <= n; i++)\
    \ {\r\n    for (int c = 0; c < 26; c++) {\r\n      int v = wm.rank(c, i);\r\n\
    \      if (v != rank[c][i]) {\r\n        cout << \"Failed rank \"<<char(c+'a')<<\"\
    , \"<<i<<\" wm.rank: \"<<v<<\" != rank: \"<<rank[c][i]<<endl;\r\n        return\
    \ 1;\r\n      }\r\n    }\r\n  }\r\n  // select\r\n  std::array<int, 26> selected{};\r\
    \n  for (int i = 0; i < n; i++) {\r\n    int c = S[i];\r\n    int k = selected[c]++;\r\
    \n    int v = wm.select(c, k);\r\n    if (v != select[c][k] or v != i) {\r\n \
    \     cout << \"Failed select \"<<char(S[i]+'a')<<\", \"<<k<<\" wm.select: \"\
    <<v<<\" != select: \"<<select[c][k]<<endl;\r\n      return 1;\r\n    }\r\n  }\r\
    \n  for (int c = 0; c < 26; c++) {\r\n    int k = selected[c];\r\n    int v =\
    \ wm.select(c, k);\r\n    if (v != select[c][k] or v != n) {\r\n      cout <<\
    \ \"Failed select \"<<char(c + 'a')<<\", \"<<k<<\" wm.select: \"<<v<<\" != select:\
    \ \"<<select[c][k]<<endl;\r\n      return 1;\r\n    }\r\n  }\r\n  // rank_c\r\n\
    \  for (int i = 0; i <= n; i++) {\r\n    for (int c = 0; c < 26; c++) {\r\n  \
    \    int v = wm.rank_lower(c, i);\r\n      if (v != rank_c[c][i]) {\r\n      \
    \  cout << \"Failed rank_c \"<<char(c+'a')<<\", \"<<i<<\" wm.rank_lower: \"<<v<<\"\
    \ != rank_c: \"<<rank_c[c][i]<<endl;\r\n        return 1;\r\n      }\r\n    }\r\
    \n  }\r\n  // enumerate\r\n  const int m = std::min(n, (int)cbrt(n*6));\r\n  for\
    \ (int l = 0; l < m; l++) {\r\n    for (int r = l+1; r <= m; r++) {\r\n      std::array<int,\
    \ 26> enm{};\r\n      for (int c = 0; c < 26; c++) enm[c] = rank[c][r] - rank[c][l];\r\
    \n      auto _v = wm.enumerate(l, r);\r\n      std::array<int, 26> v{};\r\n  \
    \    for (auto p : _v) v[p.first] = p.second;\r\n      for (int c = 0; c < 26;\
    \ c++) {\r\n        if (v[c] != enm[c]) {\r\n          cout << \"Failed enumerate\
    \ \"<<l<<\", \"<<r<<\", \"<<char(c+'a')<<\" wm.enumerate: \"<<v[c]<<\" != \"<<enm[c]<<endl;\r\
    \n          return 1;\r\n        }\r\n      }\r\n    }\r\n  }\r\n  std::cout <<\
    \ \"OK\" << std::endl;\r\n}"
  dependsOn: []
  isVerificationFile: false
  path: test/succinct/wavelet_tree_test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/succinct/wavelet_tree_test.cpp
layout: document
redirect_from:
- /library/test/succinct/wavelet_tree_test.cpp
- /library/test/succinct/wavelet_tree_test.cpp.html
title: test/succinct/wavelet_tree_test.cpp
---
