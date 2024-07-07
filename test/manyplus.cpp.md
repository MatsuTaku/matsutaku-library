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
  bundledCode: "#line 1 \"test/manyplus.cpp\"\n#include <bits/stdc++.h>\r\n\r\nusing\
    \ namespace std;\r\nusing ll = long long;\r\n\r\ntemplate <typename T>\r\nT plus(T\
    \ a, T b) {\r\n  return a + b;\r\n}\r\n\r\nint main() {\r\n  cin.tie(nullptr);\
    \ ios::sync_with_stdio(false);\r\n\r\n  int T; cin >> T;\r\n  for (int t = 0;\
    \ t < T; t++) {\r\n    ll A,B; cin>>A>>B;\r\n    cout << ::plus(A,B) << endl;\r\
    \n  }\r\n\r\n  return 0;\r\n}\r\n"
  code: "#include <bits/stdc++.h>\r\n\r\nusing namespace std;\r\nusing ll = long long;\r\
    \n\r\ntemplate <typename T>\r\nT plus(T a, T b) {\r\n  return a + b;\r\n}\r\n\r\
    \nint main() {\r\n  cin.tie(nullptr); ios::sync_with_stdio(false);\r\n\r\n  int\
    \ T; cin >> T;\r\n  for (int t = 0; t < T; t++) {\r\n    ll A,B; cin>>A>>B;\r\n\
    \    cout << ::plus(A,B) << endl;\r\n  }\r\n\r\n  return 0;\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/manyplus.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/manyplus.cpp
layout: document
redirect_from:
- /library/test/manyplus.cpp
- /library/test/manyplus.cpp.html
title: test/manyplus.cpp
---
