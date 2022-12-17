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
  bundledCode: "#line 1 \"test/persistent_stack_test.cpp\"\n#include <persistent_stack.hpp>\r\
    \n\r\n#include <vector>\r\n#include <stack>\r\n#include <algorithm>\r\n#include\
    \ <numeric>\r\n#include <iostream>\r\n#include <cassert>\r\n\r\nconstexpr int\
    \ N = 1e5;\r\nint main() {\r\n  std::vector<std::pair<int,int>> U(N);\r\n  int\
    \ sz = 0;\r\n  for (int i = 0; i < N; i++) {\r\n    int t = sz > 0 ? (rand()%3\
    \ != 0) : 1;\r\n    if (t == 0) {\r\n      U[i] = {t, 0};\r\n      sz--;\r\n \
    \   } else {\r\n      U[i] = {t, rand()};\r\n      sz++;\r\n    }\r\n  }\r\n \
    \ std::vector<int> Q(N);\r\n  for (int i = 0; i < N; i++) {\r\n    Q[i] = rand()\
    \ % N + 1;\r\n  }\r\n  std::vector<int> expected(N+1, -1);\r\n  std::stack<int>\
    \ st;\r\n  for (int i = 0; i < (int)U.size(); i++) {\r\n    if (U[i].first ==\
    \ 0) {\r\n      st.pop();\r\n    } else {\r\n      st.push(U[i].second);\r\n \
    \   }\r\n    expected[i+1] = st.empty() ? -1 : st.top();\r\n  }\r\n\r\n  std::vector<PersistentStack<int>>\
    \ ps(N+1);\r\n  for (int i = 0; i < N; i++) {\r\n    if (U[i].first == 0) {\r\n\
    \      ps[i+1] = ps[i].pop();\r\n    } else {\r\n      ps[i+1] = ps[i].push(U[i].second);\r\
    \n    }\r\n  }\r\n\r\n  for (auto q : Q) {\r\n    int v = ps[q].empty() ? -1 :\
    \ ps[q].top();\r\n    if (expected[q] != v) {\r\n      std::cout << \"expected[q]\
    \ != ps[q].top(): \" << expected[q] << ' ' << v << std::endl;\r\n      exit(EXIT_FAILURE);\r\
    \n    }\r\n  }\r\n  std::cout << \"OK\" << std::endl;\r\n}\r\n"
  code: "#include <persistent_stack.hpp>\r\n\r\n#include <vector>\r\n#include <stack>\r\
    \n#include <algorithm>\r\n#include <numeric>\r\n#include <iostream>\r\n#include\
    \ <cassert>\r\n\r\nconstexpr int N = 1e5;\r\nint main() {\r\n  std::vector<std::pair<int,int>>\
    \ U(N);\r\n  int sz = 0;\r\n  for (int i = 0; i < N; i++) {\r\n    int t = sz\
    \ > 0 ? (rand()%3 != 0) : 1;\r\n    if (t == 0) {\r\n      U[i] = {t, 0};\r\n\
    \      sz--;\r\n    } else {\r\n      U[i] = {t, rand()};\r\n      sz++;\r\n \
    \   }\r\n  }\r\n  std::vector<int> Q(N);\r\n  for (int i = 0; i < N; i++) {\r\n\
    \    Q[i] = rand() % N + 1;\r\n  }\r\n  std::vector<int> expected(N+1, -1);\r\n\
    \  std::stack<int> st;\r\n  for (int i = 0; i < (int)U.size(); i++) {\r\n    if\
    \ (U[i].first == 0) {\r\n      st.pop();\r\n    } else {\r\n      st.push(U[i].second);\r\
    \n    }\r\n    expected[i+1] = st.empty() ? -1 : st.top();\r\n  }\r\n\r\n  std::vector<PersistentStack<int>>\
    \ ps(N+1);\r\n  for (int i = 0; i < N; i++) {\r\n    if (U[i].first == 0) {\r\n\
    \      ps[i+1] = ps[i].pop();\r\n    } else {\r\n      ps[i+1] = ps[i].push(U[i].second);\r\
    \n    }\r\n  }\r\n\r\n  for (auto q : Q) {\r\n    int v = ps[q].empty() ? -1 :\
    \ ps[q].top();\r\n    if (expected[q] != v) {\r\n      std::cout << \"expected[q]\
    \ != ps[q].top(): \" << expected[q] << ' ' << v << std::endl;\r\n      exit(EXIT_FAILURE);\r\
    \n    }\r\n  }\r\n  std::cout << \"OK\" << std::endl;\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/persistent_stack_test.cpp
  requiredBy: []
  timestamp: '2022-11-27 14:51:20+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/persistent_stack_test.cpp
layout: document
redirect_from:
- /library/test/persistent_stack_test.cpp
- /library/test/persistent_stack_test.cpp.html
title: test/persistent_stack_test.cpp
---
