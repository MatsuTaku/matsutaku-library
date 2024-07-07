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
  bundledCode: "#line 1 \"test/succinct/ty_test.cpp\"\n#include <succinct/ty.hpp>\n\
    #include <iostream>\n\nusing namespace std;\nconstexpr int n = 1e7;\n\nint main()\
    \ {\n    vector<int> A(n);\n    constexpr int max_diff = (1<<10)-1;\n    A[0]\
    \ = rand()%max_diff;\n    for (int i = 1; i < n; i++) {\n        A[i] = A[i-1]\
    \ + rand()%max_diff;\n    }\n    TY<int> ty;\n    for (auto a:A) ty.push_back(a);\n\
    \    for (int i = 0; i < n; i++) {\n        auto v = ty[i];\n        if (v !=\
    \ A[i]) {\n            std::cout << \"Failed get: \" << i << \" ty.get \" << v\
    \ << \" != A \" << A[i] << std::endl;\n            return 1;\n        }\n    }\n\
    \    std::cout << \"OK\" << std::endl;\n\n}\n"
  code: "#include <succinct/ty.hpp>\n#include <iostream>\n\nusing namespace std;\n\
    constexpr int n = 1e7;\n\nint main() {\n    vector<int> A(n);\n    constexpr int\
    \ max_diff = (1<<10)-1;\n    A[0] = rand()%max_diff;\n    for (int i = 1; i <\
    \ n; i++) {\n        A[i] = A[i-1] + rand()%max_diff;\n    }\n    TY<int> ty;\n\
    \    for (auto a:A) ty.push_back(a);\n    for (int i = 0; i < n; i++) {\n    \
    \    auto v = ty[i];\n        if (v != A[i]) {\n            std::cout << \"Failed\
    \ get: \" << i << \" ty.get \" << v << \" != A \" << A[i] << std::endl;\n    \
    \        return 1;\n        }\n    }\n    std::cout << \"OK\" << std::endl;\n\n\
    }"
  dependsOn: []
  isVerificationFile: false
  path: test/succinct/ty_test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/succinct/ty_test.cpp
layout: document
redirect_from:
- /library/test/succinct/ty_test.cpp
- /library/test/succinct/ty_test.cpp.html
title: test/succinct/ty_test.cpp
---
