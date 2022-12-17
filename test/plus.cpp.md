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
  bundledCode: "#line 1 \"test/plus.cpp\"\n#include <bits/stdc++.h>\n\nusing namespace\
    \ std;\nusing ll = long long;\n\ntemplate <typename T>\nT plus(T a, T b) {\n \
    \ return a + b;\n}\n\nint main() {\n  cin.tie(nullptr); ios::sync_with_stdio(false);\n\
    \n  ll a,b; cin>>a>>b;\n  cout << ::plus(a,b) << endl;\n\n  return 0;\n}\n"
  code: "#include <bits/stdc++.h>\n\nusing namespace std;\nusing ll = long long;\n\
    \ntemplate <typename T>\nT plus(T a, T b) {\n  return a + b;\n}\n\nint main()\
    \ {\n  cin.tie(nullptr); ios::sync_with_stdio(false);\n\n  ll a,b; cin>>a>>b;\n\
    \  cout << ::plus(a,b) << endl;\n\n  return 0;\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: test/plus.cpp
  requiredBy: []
  timestamp: '2022-11-27 14:51:20+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/plus.cpp
layout: document
redirect_from:
- /library/test/plus.cpp
- /library/test/plus.cpp.html
title: test/plus.cpp
---
