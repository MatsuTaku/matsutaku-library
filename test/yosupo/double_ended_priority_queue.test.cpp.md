---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/double_ended_priority_queue.hpp
    title: Double Ended Priority Queue
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/double_ended_priority_queue
    links:
    - https://judge.yosupo.jp/problem/double_ended_priority_queue
  bundledCode: "#line 1 \"test/yosupo/double_ended_priority_queue.test.cpp\"\n#define\
    \ PROBLEM \"https://judge.yosupo.jp/problem/double_ended_priority_queue\"\n#line\
    \ 2 \"include/mtl/double_ended_priority_queue.hpp\"\n#include <cassert>\r\n#include\
    \ <vector>\r\n#include <iostream>\r\n#include <algorithm>\r\n\r\n/** \r\n * @brief\
    \ Double Ended Priority Queue\r\n * @query\r\n *   - initalize: $O(n)$\r\n * \
    \  - push: $O(\\log n)$\r\n *   - get_min: $O(1)$\r\n *   - get_max: $O(1)$\r\n\
    \ *   - pop_min: $O(\\log n)$\r\n *   - pop_max: $O(\\log n)$\r\n*/\r\ntemplate<typename\
    \ T>\r\nstruct DoubleEndedPriorityQueue {\r\n  // index = [id | (0 if forward,\
    \ 1 backward)]\r\n  size_t index(size_t id, bool b) const { return (id<<1)|b;\
    \ }\r\n  size_t parent(size_t i) const { return index(((i>>1)-1)/2, i&1); }\r\n\
    \  size_t left(size_t i) const { return index((i>>1)*2+1, i&1); }\r\n  size_t\
    \ right(size_t i) const { return index((i>>1)*2+2, i&1); }\r\n  size_t sibling(size_t\
    \ i) const { return i^1; }\r\n\r\n  std::vector<T> arr;\r\n\r\n  DoubleEndedPriorityQueue()\
    \ {}\r\n  template<typename It>\r\n  DoubleEndedPriorityQueue(It begin, It end)\
    \ : arr(begin, end) {\r\n    size_t n = arr.size();\r\n    if (n <= 1) return;\r\
    \n    if (n == 2) {\r\n      if (arr[index(0,0)] < arr[index(0,1)])\r\n      \
    \  std::swap(arr[index(0,0)], arr[index(0,1)]);\r\n      return;\r\n    }\r\n\
    \    for (long long i = arr.size()-1; i >= 0; i--) {\r\n      if ((i & 1) and\
    \ arr[i-1] < arr[i])\r\n        std::swap(arr[i-1], arr[i]);\r\n      heapify(i,\
    \ true);\r\n    }\r\n  }\r\n\r\n  template<class Compare>\r\n  size_t heap_down(size_t\
    \ i) {\r\n    Compare comp;\r\n    size_t k;\r\n    while ((k = left(i)) < arr.size())\
    \ {\r\n      auto r = right(i);\r\n      if ((r&1) and r==size()) --r;\r\n   \
    \   if (r < arr.size() and comp(arr[k], arr[r])) k = r;\r\n      if (comp(arr[i],\
    \ arr[k])) {\r\n        std::swap(arr[i], arr[k]);\r\n        i = k;\r\n     \
    \ } else break;\r\n    }\r\n    return i;\r\n  }\r\n  size_t min_heap_down(size_t\
    \ i) {\r\n    assert(i&1);\r\n    return heap_down<std::greater<>>(i);\r\n  }\r\
    \n  size_t max_heap_down(size_t i) {\r\n    assert((i&1)==0);\r\n    return heap_down<std::less<>>(i);\r\
    \n  }\r\n\r\n  size_t heap_leaf(size_t i) {\r\n    if ((i|1) < arr.size() and\
    \ arr[i&~1ull] < arr[i|1]) {\r\n      std::swap(arr[i], arr[i^1]);\r\n      i\
    \ ^= 1;\r\n    }\r\n    return i;\r\n  }\r\n\r\n  size_t min_heap_up(size_t i,\
    \ size_t root) {\r\n    size_t p;\r\n    while (i>>1 > root>>1 and (p = parent(i)|1)\
    \ >= root and arr[i] < arr[p]) {\r\n      std::swap(arr[p], arr[i]);\r\n     \
    \ i = p;\r\n    }\r\n    return i;\r\n  }\r\n  size_t max_heap_up(size_t i, size_t\
    \ root) {\r\n    size_t p;\r\n    while (i>>1 > root>>1 and (p = parent(i)&~1ull)\
    \ >= root and arr[p] < arr[i]) {\r\n      std::swap(arr[p], arr[i]);\r\n     \
    \ i = p;\r\n    }\r\n    return i;\r\n  }\r\n\r\n  void heapify(size_t i, bool\
    \ limited) {\r\n    auto j = (i&1) ? min_heap_down(i) : max_heap_down(i);\r\n\
    \    auto k = heap_leaf(j);\r\n    auto root = limited ? i : 0;\r\n    max_heap_up(k,\
    \ root);\r\n    min_heap_up(k, root);\r\n  }\r\n\r\n  template<class U>\r\n  void\
    \ push(U&& val) {\r\n    static_assert(std::is_convertible<U,T>::value, \"\");\r\
    \n    auto id = arr.size();\r\n    arr.push_back(std::forward<U>(val));\r\n  \
    \  heapify(id, false);\r\n  }\r\n  void pop_max() {\r\n    assert(!empty());\r\
    \n    std::swap(arr[index(0,0)], arr.back());\r\n    arr.pop_back();\r\n    if\
    \ (index(0,0) < arr.size())\r\n      heapify(index(0,0), false);\r\n  }\r\n  void\
    \ pop_min() {\r\n    assert(!empty());\r\n    if (size() == 1) {\r\n      arr.pop_back();\r\
    \n      return;\r\n    }\r\n    std::swap(arr[index(0,1)], arr.back());\r\n  \
    \  arr.pop_back();\r\n    if (index(0,1) < arr.size())\r\n      heapify(index(0,1),\
    \ false);\r\n  }\r\n\r\n  const T& max() const {\r\n    assert(!empty());\r\n\
    \    return arr[index(0,0)];\r\n  }\r\n  const T& min() const {\r\n    assert(!empty());\r\
    \n    return arr[index(0, size() > 1 ? 1 : 0)];\r\n  }\r\n\r\n  size_t size()\
    \ const { return arr.size(); }\r\n  bool empty() const { return size() == 0; }\r\
    \n  void clear() { arr.clear(); }\r\n\r\n};\n#line 3 \"test/yosupo/double_ended_priority_queue.test.cpp\"\
    \n#include <bits/stdc++.h>\nusing namespace std;\n\nint main() {\n    int n,q;\
    \ cin>>n>>q;\n    vector<int> S(n);\n    for (int i = 0; i < n; i++) cin>>S[i];\n\
    \    DoubleEndedPriorityQueue<int> depq(S.begin(), S.end());\n    for (int i =\
    \ 0; i < q; i++) {\n        int t; cin>>t;\n        if (t == 0) {\n          \
    \  int x; cin>>x;\n            depq.push(x);\n        } else if (t == 1) {\n \
    \           cout << depq.min() << endl;\n            depq.pop_min();\n       \
    \ } else {\n            cout << depq.max() << endl;\n            depq.pop_max();\n\
    \        }\n    }\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/double_ended_priority_queue\"\
    \n#include \"../../include/mtl/double_ended_priority_queue.hpp\"\n#include <bits/stdc++.h>\n\
    using namespace std;\n\nint main() {\n    int n,q; cin>>n>>q;\n    vector<int>\
    \ S(n);\n    for (int i = 0; i < n; i++) cin>>S[i];\n    DoubleEndedPriorityQueue<int>\
    \ depq(S.begin(), S.end());\n    for (int i = 0; i < q; i++) {\n        int t;\
    \ cin>>t;\n        if (t == 0) {\n            int x; cin>>x;\n            depq.push(x);\n\
    \        } else if (t == 1) {\n            cout << depq.min() << endl;\n     \
    \       depq.pop_min();\n        } else {\n            cout << depq.max() << endl;\n\
    \            depq.pop_max();\n        }\n    }\n}"
  dependsOn:
  - include/mtl/double_ended_priority_queue.hpp
  isVerificationFile: true
  path: test/yosupo/double_ended_priority_queue.test.cpp
  requiredBy: []
  timestamp: '2023-04-06 14:40:12+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/yosupo/double_ended_priority_queue.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/double_ended_priority_queue.test.cpp
- /verify/test/yosupo/double_ended_priority_queue.test.cpp.html
title: test/yosupo/double_ended_priority_queue.test.cpp
---
