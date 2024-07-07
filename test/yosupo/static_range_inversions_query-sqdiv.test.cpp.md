---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/static_range_inversions_query
    links:
    - https://judge.yosupo.jp/problem/static_range_inversions_query
  bundledCode: "#line 1 \"test/yosupo/static_range_inversions_query-sqdiv.test.cpp\"\
    \n#define PROBLEM \"https://judge.yosupo.jp/problem/static_range_inversions_query\"\
    \r\n#include <bits/stdc++.h>\r\nusing namespace std;\r\nusing lint = long long;\r\
    \n\r\nconstexpr int N = 1e5;\r\n\r\nint main() {\r\n  int n,q; cin>>n>>q;\r\n\
    \  vector<int> A(n);\r\n  for (int i = 0; i < n; i++) {\r\n    cin>>A[i];\r\n\
    \  }\r\n  vector<int> I(n);\r\n  iota(I.begin(), I.end(), 0);\r\n  stable_sort(I.begin(),\
    \ I.end(), [&](int l, int r) {return A[l]<A[r];});\r\n  int b = ceil(sqrt(n));\r\
    \n  int m = n/b+1;\r\n  vector<vector<int>> v(m, vector(b,n));\r\n  vector<vector<int>>\
    \ sorted_idx(m);\r\n  for (int i = 0; i < n; i++) {\r\n    v[I[i]/b][I[i]%b] =\
    \ i;\r\n    sorted_idx[I[i]/b].push_back(I[i]%b);\r\n  }\r\n  vector<vector<int>>\
    \ pref_inv(m);\r\n  vector<vector<lint>> block_inv(m, vector(m,0ll));\r\n  vector<int>\
    \ freq(n+1);\r\n  vector<vector<int>> rank(m, vector(n,0));\r\n  for (int i =\
    \ 0; i < m; i++) {\r\n    lint inv = 0;\r\n    pref_inv[i].push_back(0);\r\n \
    \   for (int j = 0; j < b; j++) {\r\n      for (int k = 0; k < j; k++) {\r\n \
    \       if (v[i][k] > v[i][j])\r\n          inv++;\r\n      }\r\n      pref_inv[i].push_back(inv);\r\
    \n    }\r\n    if (i+1 < m) {\r\n      block_inv[i+1][i] = inv;\r\n      for (int\
    \ j = i-1; j >= 0; j--) {\r\n        auto ri = sorted_idx[i].begin();\r\n    \
    \    for (int k:sorted_idx[j]) {\r\n          while (ri != sorted_idx[i].end()\
    \ and v[i][*ri] < v[j][k]) {\r\n            ++ri;\r\n          }\r\n         \
    \ inv += ri-sorted_idx[i].begin();\r\n        }\r\n        block_inv[i+1][j] =\
    \ block_inv[i][j] + inv;\r\n      }\r\n    }\r\n    for (int j = 0; j < n; j++)\r\
    \n      rank[i][j+1] = rank[i][j] + freq[j];\r\n    for (int j = 0; j < b; j++)\
    \ {\r\n      freq[v[i][j]]++;\r\n    }\r\n  }\r\n  auto range_inversions_query\
    \ = [&](int l, int r) {\r\n    int lq = l/b, rq = r/b;\r\n    int lr = l%b, rr\
    \ = r%b;\r\n    lint ans = 0;\r\n    if (lq == rq) {\r\n      ans = pref_inv[lq][rr]\
    \ - pref_inv[lq][lr];\r\n      int less_sum = 0;\r\n      for (int i : sorted_idx[lq])\
    \ {\r\n        if (i < lr) \r\n          ans -= less_sum;\r\n        else if (i\
    \ < rr)        \r\n          less_sum++;\r\n      }\r\n    } else {\r\n      ans\
    \ = block_inv[rq][lq];\r\n      for (int i = 0; i < lr; i++) {\r\n        auto\
    \ vli = v[lq][i];\r\n        ans -= rank[rq][vli] - rank[lq][vli];\r\n      }\r\
    \n      ans += lr*(lr-1)/2 - pref_inv[lq][lr];\r\n      ans += pref_inv[rq][rr];\r\
    \n      ans += (rq-lq)*b*rr;\r\n      for (int i = 0; i < rr; i++) {\r\n     \
    \   auto vri = v[rq][i];\r\n        ans -= rank[rq][vri]-rank[lq][vri];\r\n  \
    \    }\r\n      auto ri = sorted_idx[rq].begin();\r\n      int less_sum = 0;\r\
    \n      for (int i:sorted_idx[lq]) if (i<lr) {\r\n        while (ri != sorted_idx[rq].end()\
    \ and v[rq][*ri] < v[lq][i]) {\r\n          less_sum += *ri < rr;\r\n        \
    \  ++ri;\r\n        }\r\n        if (i < lr)\r\n          ans -= less_sum;\r\n\
    \      }\r\n    }\r\n    return ans;\r\n  };\r\n  for (int i = 0; i < q; i++)\
    \ {\r\n    int l,r; cin>>l>>r;\r\n    cout << range_inversions_query(l, r) <<\
    \ endl;\r\n  }\r\n}\r\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/static_range_inversions_query\"\
    \r\n#include <bits/stdc++.h>\r\nusing namespace std;\r\nusing lint = long long;\r\
    \n\r\nconstexpr int N = 1e5;\r\n\r\nint main() {\r\n  int n,q; cin>>n>>q;\r\n\
    \  vector<int> A(n);\r\n  for (int i = 0; i < n; i++) {\r\n    cin>>A[i];\r\n\
    \  }\r\n  vector<int> I(n);\r\n  iota(I.begin(), I.end(), 0);\r\n  stable_sort(I.begin(),\
    \ I.end(), [&](int l, int r) {return A[l]<A[r];});\r\n  int b = ceil(sqrt(n));\r\
    \n  int m = n/b+1;\r\n  vector<vector<int>> v(m, vector(b,n));\r\n  vector<vector<int>>\
    \ sorted_idx(m);\r\n  for (int i = 0; i < n; i++) {\r\n    v[I[i]/b][I[i]%b] =\
    \ i;\r\n    sorted_idx[I[i]/b].push_back(I[i]%b);\r\n  }\r\n  vector<vector<int>>\
    \ pref_inv(m);\r\n  vector<vector<lint>> block_inv(m, vector(m,0ll));\r\n  vector<int>\
    \ freq(n+1);\r\n  vector<vector<int>> rank(m, vector(n,0));\r\n  for (int i =\
    \ 0; i < m; i++) {\r\n    lint inv = 0;\r\n    pref_inv[i].push_back(0);\r\n \
    \   for (int j = 0; j < b; j++) {\r\n      for (int k = 0; k < j; k++) {\r\n \
    \       if (v[i][k] > v[i][j])\r\n          inv++;\r\n      }\r\n      pref_inv[i].push_back(inv);\r\
    \n    }\r\n    if (i+1 < m) {\r\n      block_inv[i+1][i] = inv;\r\n      for (int\
    \ j = i-1; j >= 0; j--) {\r\n        auto ri = sorted_idx[i].begin();\r\n    \
    \    for (int k:sorted_idx[j]) {\r\n          while (ri != sorted_idx[i].end()\
    \ and v[i][*ri] < v[j][k]) {\r\n            ++ri;\r\n          }\r\n         \
    \ inv += ri-sorted_idx[i].begin();\r\n        }\r\n        block_inv[i+1][j] =\
    \ block_inv[i][j] + inv;\r\n      }\r\n    }\r\n    for (int j = 0; j < n; j++)\r\
    \n      rank[i][j+1] = rank[i][j] + freq[j];\r\n    for (int j = 0; j < b; j++)\
    \ {\r\n      freq[v[i][j]]++;\r\n    }\r\n  }\r\n  auto range_inversions_query\
    \ = [&](int l, int r) {\r\n    int lq = l/b, rq = r/b;\r\n    int lr = l%b, rr\
    \ = r%b;\r\n    lint ans = 0;\r\n    if (lq == rq) {\r\n      ans = pref_inv[lq][rr]\
    \ - pref_inv[lq][lr];\r\n      int less_sum = 0;\r\n      for (int i : sorted_idx[lq])\
    \ {\r\n        if (i < lr) \r\n          ans -= less_sum;\r\n        else if (i\
    \ < rr)        \r\n          less_sum++;\r\n      }\r\n    } else {\r\n      ans\
    \ = block_inv[rq][lq];\r\n      for (int i = 0; i < lr; i++) {\r\n        auto\
    \ vli = v[lq][i];\r\n        ans -= rank[rq][vli] - rank[lq][vli];\r\n      }\r\
    \n      ans += lr*(lr-1)/2 - pref_inv[lq][lr];\r\n      ans += pref_inv[rq][rr];\r\
    \n      ans += (rq-lq)*b*rr;\r\n      for (int i = 0; i < rr; i++) {\r\n     \
    \   auto vri = v[rq][i];\r\n        ans -= rank[rq][vri]-rank[lq][vri];\r\n  \
    \    }\r\n      auto ri = sorted_idx[rq].begin();\r\n      int less_sum = 0;\r\
    \n      for (int i:sorted_idx[lq]) if (i<lr) {\r\n        while (ri != sorted_idx[rq].end()\
    \ and v[rq][*ri] < v[lq][i]) {\r\n          less_sum += *ri < rr;\r\n        \
    \  ++ri;\r\n        }\r\n        if (i < lr)\r\n          ans -= less_sum;\r\n\
    \      }\r\n    }\r\n    return ans;\r\n  };\r\n  for (int i = 0; i < q; i++)\
    \ {\r\n    int l,r; cin>>l>>r;\r\n    cout << range_inversions_query(l, r) <<\
    \ endl;\r\n  }\r\n}\r\n"
  dependsOn: []
  isVerificationFile: true
  path: test/yosupo/static_range_inversions_query-sqdiv.test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: test/yosupo/static_range_inversions_query-sqdiv.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/static_range_inversions_query-sqdiv.test.cpp
- /verify/test/yosupo/static_range_inversions_query-sqdiv.test.cpp.html
title: test/yosupo/static_range_inversions_query-sqdiv.test.cpp
---
