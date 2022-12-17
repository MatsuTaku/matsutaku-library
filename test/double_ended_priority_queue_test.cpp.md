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
  bundledCode: "#line 1 \"test/double_ended_priority_queue_test.cpp\"\n#include <double_ended_priority_queue.hpp>\n\
    #include <vector>\n#include <random>\n#include <iostream>\n#include <set>\n#include\
    \ <cassert>\n\nusing P = std::pair<int,int>;\nconstexpr int size = 1<<16;\n\n\
    bool external_sorting_step_test() {\n  std::vector<int> A(size);\n  for (int i\
    \ = 0; i < size; i++) A[i] = rand()%size;\n  std::multiset<int> s;\n  DoubleEndedPriorityQueue<int>\
    \ qs;\n  constexpr int memory_size = size>>3;\n  for (int i = 0; i < memory_size;\
    \ i++) {\n    s.insert(A[i]);\n    qs.push(A[i]);\n  }\n  for (int i = memory_size;\
    \ i < size; i++) {\n    if (s.size() != qs.size()) {\n      std::cerr<<\"s.size\
    \ != qs.size: \"<<s.size()<<\", \"<<qs.size()<<std::endl;\n      return false;\n\
    \    }\n    if (*--s.end() != qs.max()) {\n      std::cerr<<\"s.max() != qs.max():\
    \ \"<<(*--s.end())<<\", \"<<qs.max()<<std::endl;\n      return false;\n    }\n\
    \    if (*s.begin() != qs.min()) {\n      std::cerr<<\"s.min() != qs.min(): \"\
    <<(*s.begin())<<\", \"<<qs.min()<<std::endl;\n      return false;\n    }\n   \
    \ if (A[i] < qs.min()) {\n      // Push A[i] to left array\n    } else if (A[i]\
    \ > qs.max()) {\n      // Push A[i] to right array\n    } else {\n      auto b\
    \ = rand()%2;\n      if (b) {\n        s.erase(--s.end());\n        qs.pop_max();\n\
    \        // Push max pivot to right array\n      } else {\n        s.erase(s.begin());\n\
    \        qs.pop_min();\n        // Push min pivot to left array\n      }\n   \
    \   s.insert(A[i]);\n      qs.push(A[i]);\n    }\n  }\n  // Push remaining values\
    \ to middle array\n  // Return external_sorting(left_array) + middle_array + external_sorting(right_array)\n\
    \  return true;\n}\n\nint main() {\n  if (!external_sorting_step_test()) {\n \
    \   assert(false);\n    exit(EXIT_FAILURE);\n  }\n\n  std::cout << \"OK\" << std::endl;\n\
    }\n"
  code: "#include <double_ended_priority_queue.hpp>\n#include <vector>\n#include <random>\n\
    #include <iostream>\n#include <set>\n#include <cassert>\n\nusing P = std::pair<int,int>;\n\
    constexpr int size = 1<<16;\n\nbool external_sorting_step_test() {\n  std::vector<int>\
    \ A(size);\n  for (int i = 0; i < size; i++) A[i] = rand()%size;\n  std::multiset<int>\
    \ s;\n  DoubleEndedPriorityQueue<int> qs;\n  constexpr int memory_size = size>>3;\n\
    \  for (int i = 0; i < memory_size; i++) {\n    s.insert(A[i]);\n    qs.push(A[i]);\n\
    \  }\n  for (int i = memory_size; i < size; i++) {\n    if (s.size() != qs.size())\
    \ {\n      std::cerr<<\"s.size != qs.size: \"<<s.size()<<\", \"<<qs.size()<<std::endl;\n\
    \      return false;\n    }\n    if (*--s.end() != qs.max()) {\n      std::cerr<<\"\
    s.max() != qs.max(): \"<<(*--s.end())<<\", \"<<qs.max()<<std::endl;\n      return\
    \ false;\n    }\n    if (*s.begin() != qs.min()) {\n      std::cerr<<\"s.min()\
    \ != qs.min(): \"<<(*s.begin())<<\", \"<<qs.min()<<std::endl;\n      return false;\n\
    \    }\n    if (A[i] < qs.min()) {\n      // Push A[i] to left array\n    } else\
    \ if (A[i] > qs.max()) {\n      // Push A[i] to right array\n    } else {\n  \
    \    auto b = rand()%2;\n      if (b) {\n        s.erase(--s.end());\n       \
    \ qs.pop_max();\n        // Push max pivot to right array\n      } else {\n  \
    \      s.erase(s.begin());\n        qs.pop_min();\n        // Push min pivot to\
    \ left array\n      }\n      s.insert(A[i]);\n      qs.push(A[i]);\n    }\n  }\n\
    \  // Push remaining values to middle array\n  // Return external_sorting(left_array)\
    \ + middle_array + external_sorting(right_array)\n  return true;\n}\n\nint main()\
    \ {\n  if (!external_sorting_step_test()) {\n    assert(false);\n    exit(EXIT_FAILURE);\n\
    \  }\n\n  std::cout << \"OK\" << std::endl;\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: test/double_ended_priority_queue_test.cpp
  requiredBy: []
  timestamp: '2022-11-27 14:51:20+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/double_ended_priority_queue_test.cpp
layout: document
redirect_from:
- /library/test/double_ended_priority_queue_test.cpp
- /library/test/double_ended_priority_queue_test.cpp.html
title: test/double_ended_priority_queue_test.cpp
---
