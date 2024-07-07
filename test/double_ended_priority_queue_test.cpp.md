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
  bundledCode: "#line 1 \"test/double_ended_priority_queue_test.cpp\"\n#include <double_ended_priority_queue.hpp>\r\
    \n#include <vector>\r\n#include <random>\r\n#include <iostream>\r\n#include <set>\r\
    \n#include <cassert>\r\n\r\nusing P = std::pair<int,int>;\r\nconstexpr int size\
    \ = 1<<16;\r\n\r\nbool external_sorting_step_test() {\r\n  std::vector<int> A(size);\r\
    \n  for (int i = 0; i < size; i++) A[i] = rand()%size;\r\n  std::multiset<int>\
    \ s;\r\n  DoubleEndedPriorityQueue<int> qs;\r\n  constexpr int memory_size = size>>3;\r\
    \n  for (int i = 0; i < memory_size; i++) {\r\n    s.insert(A[i]);\r\n    qs.push(A[i]);\r\
    \n  }\r\n  for (int i = memory_size; i < size; i++) {\r\n    if (s.size() != qs.size())\
    \ {\r\n      std::cerr<<\"s.size != qs.size: \"<<s.size()<<\", \"<<qs.size()<<std::endl;\r\
    \n      return false;\r\n    }\r\n    if (*--s.end() != qs.max()) {\r\n      std::cerr<<\"\
    s.max() != qs.max(): \"<<(*--s.end())<<\", \"<<qs.max()<<std::endl;\r\n      return\
    \ false;\r\n    }\r\n    if (*s.begin() != qs.min()) {\r\n      std::cerr<<\"\
    s.min() != qs.min(): \"<<(*s.begin())<<\", \"<<qs.min()<<std::endl;\r\n      return\
    \ false;\r\n    }\r\n    if (A[i] < qs.min()) {\r\n      // Push A[i] to left\
    \ array\r\n    } else if (A[i] > qs.max()) {\r\n      // Push A[i] to right array\r\
    \n    } else {\r\n      auto b = rand()%2;\r\n      if (b) {\r\n        s.erase(--s.end());\r\
    \n        qs.pop_max();\r\n        // Push max pivot to right array\r\n      }\
    \ else {\r\n        s.erase(s.begin());\r\n        qs.pop_min();\r\n        //\
    \ Push min pivot to left array\r\n      }\r\n      s.insert(A[i]);\r\n      qs.push(A[i]);\r\
    \n    }\r\n  }\r\n  // Push remaining values to middle array\r\n  // Return external_sorting(left_array)\
    \ + middle_array + external_sorting(right_array)\r\n  return true;\r\n}\r\n\r\n\
    int main() {\r\n  if (!external_sorting_step_test()) {\r\n    assert(false);\r\
    \n    exit(EXIT_FAILURE);\r\n  }\r\n\r\n  std::cout << \"OK\" << std::endl;\r\n\
    }\r\n"
  code: "#include <double_ended_priority_queue.hpp>\r\n#include <vector>\r\n#include\
    \ <random>\r\n#include <iostream>\r\n#include <set>\r\n#include <cassert>\r\n\r\
    \nusing P = std::pair<int,int>;\r\nconstexpr int size = 1<<16;\r\n\r\nbool external_sorting_step_test()\
    \ {\r\n  std::vector<int> A(size);\r\n  for (int i = 0; i < size; i++) A[i] =\
    \ rand()%size;\r\n  std::multiset<int> s;\r\n  DoubleEndedPriorityQueue<int> qs;\r\
    \n  constexpr int memory_size = size>>3;\r\n  for (int i = 0; i < memory_size;\
    \ i++) {\r\n    s.insert(A[i]);\r\n    qs.push(A[i]);\r\n  }\r\n  for (int i =\
    \ memory_size; i < size; i++) {\r\n    if (s.size() != qs.size()) {\r\n      std::cerr<<\"\
    s.size != qs.size: \"<<s.size()<<\", \"<<qs.size()<<std::endl;\r\n      return\
    \ false;\r\n    }\r\n    if (*--s.end() != qs.max()) {\r\n      std::cerr<<\"\
    s.max() != qs.max(): \"<<(*--s.end())<<\", \"<<qs.max()<<std::endl;\r\n      return\
    \ false;\r\n    }\r\n    if (*s.begin() != qs.min()) {\r\n      std::cerr<<\"\
    s.min() != qs.min(): \"<<(*s.begin())<<\", \"<<qs.min()<<std::endl;\r\n      return\
    \ false;\r\n    }\r\n    if (A[i] < qs.min()) {\r\n      // Push A[i] to left\
    \ array\r\n    } else if (A[i] > qs.max()) {\r\n      // Push A[i] to right array\r\
    \n    } else {\r\n      auto b = rand()%2;\r\n      if (b) {\r\n        s.erase(--s.end());\r\
    \n        qs.pop_max();\r\n        // Push max pivot to right array\r\n      }\
    \ else {\r\n        s.erase(s.begin());\r\n        qs.pop_min();\r\n        //\
    \ Push min pivot to left array\r\n      }\r\n      s.insert(A[i]);\r\n      qs.push(A[i]);\r\
    \n    }\r\n  }\r\n  // Push remaining values to middle array\r\n  // Return external_sorting(left_array)\
    \ + middle_array + external_sorting(right_array)\r\n  return true;\r\n}\r\n\r\n\
    int main() {\r\n  if (!external_sorting_step_test()) {\r\n    assert(false);\r\
    \n    exit(EXIT_FAILURE);\r\n  }\r\n\r\n  std::cout << \"OK\" << std::endl;\r\n\
    }\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/double_ended_priority_queue_test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/double_ended_priority_queue_test.cpp
layout: document
redirect_from:
- /library/test/double_ended_priority_queue_test.cpp
- /library/test/double_ended_priority_queue_test.cpp.html
title: test/double_ended_priority_queue_test.cpp
---
