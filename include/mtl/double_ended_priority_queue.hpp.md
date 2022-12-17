---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/double_ended_priority_queue.hpp\"\n#include\
    \ <cassert>\r\n#include <vector>\r\n#include <iostream>\r\n\r\ntemplate<typename\
    \ T>\r\nstruct DoubleEndedPriorityQueue {\r\n  // index = [id | (0 if forward,\
    \ 1 backward)]\r\n  std::vector<T> arr;\r\n  DoubleEndedPriorityQueue() : arr(2)\
    \ {}\r\n  template<typename It>\r\n  DoubleEndedPriorityQueue(It begin, It end)\
    \ : arr(2) {\r\n    arr.insert(arr.end(), begin, end);\r\n    size_t n = end -\
    \ begin;\r\n    if (n <= 1) return;\r\n    if (n == 2) {\r\n      if (arr[2|0]\
    \ < arr[2|1])\r\n        std::swap(arr[2|0], arr[2|1]);\r\n      return;\r\n \
    \   }\r\n    for (size_t i = n+2-1; i > 1; i--) {\r\n      if (i^1) {\r\n    \
    \    auto l = (i^1) << 1 | 1;\r\n        if (l >= arr.size()) {\r\n          auto\
    \ co = i-1;\r\n          if (arr[co] < arr[i])\r\n            std::swap(arr[co],\
    \ arr[i]);\r\n        }\r\n        auto j = (i >> 1) | 1;\r\n        if (j > 1\
    \ and arr[i] < arr[j])\r\n          std::swap(arr[i], arr[j]);\r\n      } else\
    \ {\r\n        auto j = (i >> 1) & ~size_t(1);\r\n        if (j > 1 and arr[j]\
    \ < arr[i])\r\n          std::swap(arr[j], arr[i]);\r\n      }\r\n    }\r\n  }\r\
    \n\r\n  void push(T val) {\r\n    auto id = arr.size();\r\n    arr.push_back(val);\r\
    \n    id = bubble_up(id);\r\n    tricle_down(id);\r\n  }\r\n  void pop_max() {\r\
    \n    assert(!empty());\r\n    std::swap(arr[2|0], arr.back());\r\n    arr.pop_back();\r\
    \n    tricle_down(2|0);\r\n  }\r\n  void pop_min() {\r\n    assert(!empty());\r\
    \n    if (size() == 1) {\r\n      arr.pop_back();\r\n      return;\r\n    }\r\n\
    \    std::swap(arr[2|1], arr.back());\r\n    arr.pop_back();\r\n    bubble_up(2|1);\r\
    \n  }\r\n  T max() const {\r\n    assert(!empty());\r\n    return arr[2|0];\r\n\
    \  }\r\n  T min() const {\r\n    assert(!empty());\r\n    return size() > 1 ?\
    \ arr[2|1] : arr[2|0];\r\n  }\r\n  size_t size() const { return arr.size() - 2;\
    \ }\r\n  bool empty() const { return size() == 0; }\r\n\r\n  size_t lower_priority_parent(size_t\
    \ id) const {\r\n    if (id&1) {\r\n      auto l = (id^1) << 1 | 1;\r\n      auto\
    \ r = (id^1) << 1 | 3;\r\n      if (r < arr.size()) {\r\n        auto s = arr[l]\
    \ < arr[r] ? l : r;\r\n        return s;\r\n      } else if (l < arr.size()) {\r\
    \n        return l;\r\n      } else {\r\n        auto prev = id ^ 1;\r\n     \
    \   return prev;\r\n      }\r\n    } else {\r\n      return (id >> 1) & ~size_t(1);\r\
    \n    }\r\n  }\r\n\r\n  size_t upper_priority_child(size_t id) const {\r\n   \
    \ if (id&1) {\r\n      return (id >> 1) | 1;\r\n    } else {\r\n      auto l =\
    \ id << 1;\r\n      auto r = id << 1 | 2;\r\n      if (r < arr.size()) {\r\n \
    \       auto s = arr[l] < arr[r] ? r : l;\r\n        return s;\r\n      } else\
    \ if (l < arr.size()) {\r\n        return l;\r\n      } else {\r\n        auto\
    \ next = id ^ 1;\r\n        if (next < arr.size())\r\n          return next;\r\
    \n        else {\r\n          return (id >> 1) | 1;\r\n        }\r\n      }\r\n\
    \    }\r\n  }\r\n\r\n  size_t bubble_up(size_t id) {\r\n    if (id == (2|0)) return\
    \ id;\r\n    auto p = lower_priority_parent(id);\r\n    if (arr[p] < arr[id])\
    \ {\r\n      std::swap(arr[p], arr[id]);\r\n      return bubble_up(p);\r\n   \
    \ } else {\r\n      return id;\r\n    }\r\n  }\r\n  size_t tricle_down(size_t\
    \ id) {\r\n    if (id == (size() > 1 ? (2|1) : (2|0))) return id;\r\n    auto\
    \ c = upper_priority_child(id);\r\n    if (arr[id] < arr[c]) {\r\n      std::swap(arr[id],\
    \ arr[c]);\r\n      return tricle_down(c);\r\n    } else {\r\n      return id;\r\
    \n    }\r\n  }\r\n};\n"
  code: "#pragma once\r\n#include <cassert>\r\n#include <vector>\r\n#include <iostream>\r\
    \n\r\ntemplate<typename T>\r\nstruct DoubleEndedPriorityQueue {\r\n  // index\
    \ = [id | (0 if forward, 1 backward)]\r\n  std::vector<T> arr;\r\n  DoubleEndedPriorityQueue()\
    \ : arr(2) {}\r\n  template<typename It>\r\n  DoubleEndedPriorityQueue(It begin,\
    \ It end) : arr(2) {\r\n    arr.insert(arr.end(), begin, end);\r\n    size_t n\
    \ = end - begin;\r\n    if (n <= 1) return;\r\n    if (n == 2) {\r\n      if (arr[2|0]\
    \ < arr[2|1])\r\n        std::swap(arr[2|0], arr[2|1]);\r\n      return;\r\n \
    \   }\r\n    for (size_t i = n+2-1; i > 1; i--) {\r\n      if (i^1) {\r\n    \
    \    auto l = (i^1) << 1 | 1;\r\n        if (l >= arr.size()) {\r\n          auto\
    \ co = i-1;\r\n          if (arr[co] < arr[i])\r\n            std::swap(arr[co],\
    \ arr[i]);\r\n        }\r\n        auto j = (i >> 1) | 1;\r\n        if (j > 1\
    \ and arr[i] < arr[j])\r\n          std::swap(arr[i], arr[j]);\r\n      } else\
    \ {\r\n        auto j = (i >> 1) & ~size_t(1);\r\n        if (j > 1 and arr[j]\
    \ < arr[i])\r\n          std::swap(arr[j], arr[i]);\r\n      }\r\n    }\r\n  }\r\
    \n\r\n  void push(T val) {\r\n    auto id = arr.size();\r\n    arr.push_back(val);\r\
    \n    id = bubble_up(id);\r\n    tricle_down(id);\r\n  }\r\n  void pop_max() {\r\
    \n    assert(!empty());\r\n    std::swap(arr[2|0], arr.back());\r\n    arr.pop_back();\r\
    \n    tricle_down(2|0);\r\n  }\r\n  void pop_min() {\r\n    assert(!empty());\r\
    \n    if (size() == 1) {\r\n      arr.pop_back();\r\n      return;\r\n    }\r\n\
    \    std::swap(arr[2|1], arr.back());\r\n    arr.pop_back();\r\n    bubble_up(2|1);\r\
    \n  }\r\n  T max() const {\r\n    assert(!empty());\r\n    return arr[2|0];\r\n\
    \  }\r\n  T min() const {\r\n    assert(!empty());\r\n    return size() > 1 ?\
    \ arr[2|1] : arr[2|0];\r\n  }\r\n  size_t size() const { return arr.size() - 2;\
    \ }\r\n  bool empty() const { return size() == 0; }\r\n\r\n  size_t lower_priority_parent(size_t\
    \ id) const {\r\n    if (id&1) {\r\n      auto l = (id^1) << 1 | 1;\r\n      auto\
    \ r = (id^1) << 1 | 3;\r\n      if (r < arr.size()) {\r\n        auto s = arr[l]\
    \ < arr[r] ? l : r;\r\n        return s;\r\n      } else if (l < arr.size()) {\r\
    \n        return l;\r\n      } else {\r\n        auto prev = id ^ 1;\r\n     \
    \   return prev;\r\n      }\r\n    } else {\r\n      return (id >> 1) & ~size_t(1);\r\
    \n    }\r\n  }\r\n\r\n  size_t upper_priority_child(size_t id) const {\r\n   \
    \ if (id&1) {\r\n      return (id >> 1) | 1;\r\n    } else {\r\n      auto l =\
    \ id << 1;\r\n      auto r = id << 1 | 2;\r\n      if (r < arr.size()) {\r\n \
    \       auto s = arr[l] < arr[r] ? r : l;\r\n        return s;\r\n      } else\
    \ if (l < arr.size()) {\r\n        return l;\r\n      } else {\r\n        auto\
    \ next = id ^ 1;\r\n        if (next < arr.size())\r\n          return next;\r\
    \n        else {\r\n          return (id >> 1) | 1;\r\n        }\r\n      }\r\n\
    \    }\r\n  }\r\n\r\n  size_t bubble_up(size_t id) {\r\n    if (id == (2|0)) return\
    \ id;\r\n    auto p = lower_priority_parent(id);\r\n    if (arr[p] < arr[id])\
    \ {\r\n      std::swap(arr[p], arr[id]);\r\n      return bubble_up(p);\r\n   \
    \ } else {\r\n      return id;\r\n    }\r\n  }\r\n  size_t tricle_down(size_t\
    \ id) {\r\n    if (id == (size() > 1 ? (2|1) : (2|0))) return id;\r\n    auto\
    \ c = upper_priority_child(id);\r\n    if (arr[id] < arr[c]) {\r\n      std::swap(arr[id],\
    \ arr[c]);\r\n      return tricle_down(c);\r\n    } else {\r\n      return id;\r\
    \n    }\r\n  }\r\n};"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/double_ended_priority_queue.hpp
  requiredBy: []
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/double_ended_priority_queue.hpp
layout: document
redirect_from:
- /library/include/mtl/double_ended_priority_queue.hpp
- /library/include/mtl/double_ended_priority_queue.hpp.html
title: include/mtl/double_ended_priority_queue.hpp
---
