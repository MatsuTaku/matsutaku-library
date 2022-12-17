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
  bundledCode: "#line 1 \"test/manyplus.cpp\"\n#include <bits/stdc++.h>\n\nusing namespace\
    \ std;\nusing ll = long long;\n\ntemplate <typename T>\nT plus(T a, T b) {\n \
    \ return a + b;\n}\n\nint main() {\n  cin.tie(nullptr); ios::sync_with_stdio(false);\n\
    \n  int T; cin >> T;\n  for (int t = 0; t < T; t++) {\n    ll A,B; cin>>A>>B;\n\
    \    cout << ::plus(A,B) << endl;\n  }\n\n  return 0;\n}\n"
  code: "#include <bits/stdc++.h>\n\nusing namespace std;\nusing ll = long long;\n\
    \ntemplate <typename T>\nT plus(T a, T b) {\n  return a + b;\n}\n\nint main()\
    \ {\n  cin.tie(nullptr); ios::sync_with_stdio(false);\n\n  int T; cin >> T;\n\
    \  for (int t = 0; t < T; t++) {\n    ll A,B; cin>>A>>B;\n    cout << ::plus(A,B)\
    \ << endl;\n  }\n\n  return 0;\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: test/manyplus.cpp
  requiredBy: []
  timestamp: '2022-11-27 14:51:20+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/manyplus.cpp
layout: document
redirect_from:
- /library/test/manyplus.cpp
- /library/test/manyplus.cpp.html
title: test/manyplus.cpp
---
