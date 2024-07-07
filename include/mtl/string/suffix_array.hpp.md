---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/string/suffix_array.test.cpp
    title: test/string/suffix_array.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/string/suffix_array.hpp\"\n#include <vector>\r\
    \n#include <iterator>\r\n#include <limits>\r\n#include <cassert>\r\n#include <algorithm>\r\
    \n#include <numeric>\r\n#include <cstring>\r\n#include <iostream>\r\n\r\nvoid\
    \ sa_is(const std::vector<int>& S, std::vector<std::pair<int, int>>& bucket, std::vector<int>::iterator\
    \ dst, bool definitely_distinct_lmss) {\r\n  const int n = (int) S.size();\r\n\
    \  assert(S.back() == 0);\r\n  assert(*max_element(S.begin(), S.end()) < n);\r\
    \n  // Induced Suffix Sorting\r\n  if (n <= 3) {\r\n    if (n == 1)\r\n      dst[0]\
    \ = 0;\r\n    else if (n == 2) {\r\n      dst[0] = 1;\r\n      dst[1] = 0;\r\n\
    \    } else if (n == 3) {\r\n      dst[0] = 2;\r\n      if (S[0] < S[1]) {\r\n\
    \        dst[1] = 0;\r\n        dst[2] = 1;\r\n      } else {\r\n        dst[1]\
    \ = 1;\r\n        dst[2] = 0;\r\n      }\r\n    }\r\n    return;\r\n  }\r\n  std::vector<bool>\
    \ is_s(n);\r\n  is_s[n-1] = true;\r\n  for (int i = n-3; i >= 0; i--) {\r\n  \
    \  int l = S[i], r = S[i+1];\r\n    is_s[i] = l < r or (l == r and is_s[i+1]);\r\
    \n  }\r\n  auto is_lms = [&](int i) {\r\n    return i and is_s[i] and !is_s[i-1];\r\
    \n  };\r\n\r\n  std::vector<int> cnt(n);\r\n  for (int c : S)\r\n    cnt[c]++;\r\
    \n  assert(bucket.size() >= (size_t)n);\r\n  auto init_bucket = [&bucket, n](const\
    \ std::vector<int>& cnt) {\r\n    int sum = 0;\r\n    for (int i = 0; i < n; i++)\
    \ {\r\n      bucket[i] = {sum, sum+cnt[i]-1};\r\n      sum += cnt[i];\r\n    }\r\
    \n  };\r\n  init_bucket(cnt);\r\n\r\n  auto A = &*dst;\r\n  memset(A, -1, sizeof(int)*n);\r\
    \n  int cnt_lms = 0;\r\n  for (int i = n-1; i >= 1; i--) if (is_lms(i)) {\r\n\
    \      A[bucket[S[i]].second--] = i;\r\n      cnt_lms++;\r\n  }\r\n  auto induced_sort\
    \ = [&]() {\r\n    for (int i = 0; i < n; i++) {\r\n      if (A[i] == -1) continue;\r\
    \n      int p = A[i] ? A[i]-1 : n-1;\r\n      if (!is_s[p])\r\n        A[bucket[S[p]].first++]\
    \ = p;\r\n      if (is_lms(A[i])) {\r\n        assert(bucket[S[A[i]]].second+1\
    \ == i);\r\n        A[++bucket[S[A[i]]].second] = -1;\r\n      }\r\n    }\r\n\
    \    for (int i = n-1; i >= 0; i--) {\r\n      assert(A[i] != -1);\r\n      int\
    \ p = A[i] ? A[i]-1 : n-1;\r\n      if (is_s[p])\r\n        A[bucket[S[p]].second--]\
    \ = p;\r\n    }\r\n  };\r\n  induced_sort();\r\n\r\n  if (definitely_distinct_lmss)\r\
    \n    return;\r\n\r\n  // Build SA of LMS-substrings\r\n  std::vector<int> sa_lmss(cnt_lms),\
    \ lmss(n);\r\n  {\r\n    auto are_same_lms_substr = [&](int i, int j) {\r\n  \
    \    assert(is_lms(i) and is_lms(j));\r\n      if (S[i] != S[j])\r\n        return\
    \ false;\r\n      i++; j++;\r\n      while(true) {\r\n        if (S[i] != S[j]\
    \ or is_lms(i) != is_lms(j))\r\n          return false;\r\n        if (is_lms(i))\r\
    \n          break;\r\n        i++; j++;\r\n      }\r\n      return true;\r\n \
    \   };\r\n    int lms_ord = 0;\r\n    int plms = n-1;\r\n    auto cmp = [&](int\
    \ i) -> int& {\r\n      return lmss[cnt_lms + i/2];\r\n    };\r\n    assert(A[0]\
    \ == n-1);\r\n    cmp(A[0]) = 0;\r\n    for (int i = 1; i < n; i++) if (is_lms(A[i]))\
    \ {\r\n      if (!are_same_lms_substr(plms, A[i]))\r\n        ++lms_ord;\r\n \
    \     cmp(A[i]) = lms_ord;\r\n      plms = A[i];\r\n    }\r\n    int k = 0;\r\n\
    \    for (int i = 1; i < n; i++) if (is_lms(i))\r\n      lmss[k++] = cmp(i);\r\
    \n    lmss.resize(cnt_lms);\r\n    sa_is(lmss, bucket, sa_lmss.begin(), cnt_lms\
    \ - 1 == lms_ord);\r\n  }\r\n\r\n  // Induced-sort by ordered LMSs\r\n  auto&\
    \ citi = lmss; // Reuse memory\r\n  citi.clear();\r\n  for (int i = 1; i < n;\
    \ i++) if (is_lms(i))\r\n    citi.push_back(i);\r\n  init_bucket(cnt);\r\n  memset(A,\
    \ -1, sizeof(int)*n);\r\n  for (auto it = sa_lmss.rbegin(); it != sa_lmss.rend();\
    \ ++it)\r\n    A[bucket[S[citi[*it]]].second--] = citi[*it];\r\n  induced_sort();\r\
    \n}\r\n\r\ntemplate<typename It>\r\n[[nodiscard]]\r\nstd::vector<int> SuffixArray(It\
    \ begin, It end) {\r\n  using trait = std::iterator_traits<It>;\r\n  static_assert(std::is_base_of<std::random_access_iterator_tag,\
    \ typename trait::iterator_category>::value, \"\");\r\n\r\n  int n = end - begin;\r\
    \n  std::vector<int> S(n+1);\r\n  S[n] = 0;\r\n  auto mp = std::minmax_element(begin,\
    \ end);\r\n  if (0 < (int)*mp.first and (int)*mp.second <= n) {\r\n    std::copy(begin,\
    \ end, S.begin());\r\n  } else {\r\n    std::vector<int> ord(n);\r\n    std::iota(ord.begin(),\
    \ ord.end(), 0);\r\n    std::sort(ord.begin(), ord.end(), [&](int l, int r) {\r\
    \n      return *(begin+l) < *(begin+r);\r\n    });\r\n    int k = 1;\r\n    for\
    \ (int i = 0; i < n; i++)\r\n      S[ord[i]] = i and *(begin+ord[i]) != *(begin+ord[i-1])\
    \ ? ++k : k;\r\n  }\r\n  std::vector<int> A(n+1);\r\n  std::vector<std::pair<int,int>>\
    \ bucket(n+1);\r\n  sa_is(S, bucket, A.begin(), false);\r\n  return std::vector<int>(A.begin()+1,\
    \ A.end());\r\n}\n"
  code: "#pragma once\r\n#include <vector>\r\n#include <iterator>\r\n#include <limits>\r\
    \n#include <cassert>\r\n#include <algorithm>\r\n#include <numeric>\r\n#include\
    \ <cstring>\r\n#include <iostream>\r\n\r\nvoid sa_is(const std::vector<int>& S,\
    \ std::vector<std::pair<int, int>>& bucket, std::vector<int>::iterator dst, bool\
    \ definitely_distinct_lmss) {\r\n  const int n = (int) S.size();\r\n  assert(S.back()\
    \ == 0);\r\n  assert(*max_element(S.begin(), S.end()) < n);\r\n  // Induced Suffix\
    \ Sorting\r\n  if (n <= 3) {\r\n    if (n == 1)\r\n      dst[0] = 0;\r\n    else\
    \ if (n == 2) {\r\n      dst[0] = 1;\r\n      dst[1] = 0;\r\n    } else if (n\
    \ == 3) {\r\n      dst[0] = 2;\r\n      if (S[0] < S[1]) {\r\n        dst[1] =\
    \ 0;\r\n        dst[2] = 1;\r\n      } else {\r\n        dst[1] = 1;\r\n     \
    \   dst[2] = 0;\r\n      }\r\n    }\r\n    return;\r\n  }\r\n  std::vector<bool>\
    \ is_s(n);\r\n  is_s[n-1] = true;\r\n  for (int i = n-3; i >= 0; i--) {\r\n  \
    \  int l = S[i], r = S[i+1];\r\n    is_s[i] = l < r or (l == r and is_s[i+1]);\r\
    \n  }\r\n  auto is_lms = [&](int i) {\r\n    return i and is_s[i] and !is_s[i-1];\r\
    \n  };\r\n\r\n  std::vector<int> cnt(n);\r\n  for (int c : S)\r\n    cnt[c]++;\r\
    \n  assert(bucket.size() >= (size_t)n);\r\n  auto init_bucket = [&bucket, n](const\
    \ std::vector<int>& cnt) {\r\n    int sum = 0;\r\n    for (int i = 0; i < n; i++)\
    \ {\r\n      bucket[i] = {sum, sum+cnt[i]-1};\r\n      sum += cnt[i];\r\n    }\r\
    \n  };\r\n  init_bucket(cnt);\r\n\r\n  auto A = &*dst;\r\n  memset(A, -1, sizeof(int)*n);\r\
    \n  int cnt_lms = 0;\r\n  for (int i = n-1; i >= 1; i--) if (is_lms(i)) {\r\n\
    \      A[bucket[S[i]].second--] = i;\r\n      cnt_lms++;\r\n  }\r\n  auto induced_sort\
    \ = [&]() {\r\n    for (int i = 0; i < n; i++) {\r\n      if (A[i] == -1) continue;\r\
    \n      int p = A[i] ? A[i]-1 : n-1;\r\n      if (!is_s[p])\r\n        A[bucket[S[p]].first++]\
    \ = p;\r\n      if (is_lms(A[i])) {\r\n        assert(bucket[S[A[i]]].second+1\
    \ == i);\r\n        A[++bucket[S[A[i]]].second] = -1;\r\n      }\r\n    }\r\n\
    \    for (int i = n-1; i >= 0; i--) {\r\n      assert(A[i] != -1);\r\n      int\
    \ p = A[i] ? A[i]-1 : n-1;\r\n      if (is_s[p])\r\n        A[bucket[S[p]].second--]\
    \ = p;\r\n    }\r\n  };\r\n  induced_sort();\r\n\r\n  if (definitely_distinct_lmss)\r\
    \n    return;\r\n\r\n  // Build SA of LMS-substrings\r\n  std::vector<int> sa_lmss(cnt_lms),\
    \ lmss(n);\r\n  {\r\n    auto are_same_lms_substr = [&](int i, int j) {\r\n  \
    \    assert(is_lms(i) and is_lms(j));\r\n      if (S[i] != S[j])\r\n        return\
    \ false;\r\n      i++; j++;\r\n      while(true) {\r\n        if (S[i] != S[j]\
    \ or is_lms(i) != is_lms(j))\r\n          return false;\r\n        if (is_lms(i))\r\
    \n          break;\r\n        i++; j++;\r\n      }\r\n      return true;\r\n \
    \   };\r\n    int lms_ord = 0;\r\n    int plms = n-1;\r\n    auto cmp = [&](int\
    \ i) -> int& {\r\n      return lmss[cnt_lms + i/2];\r\n    };\r\n    assert(A[0]\
    \ == n-1);\r\n    cmp(A[0]) = 0;\r\n    for (int i = 1; i < n; i++) if (is_lms(A[i]))\
    \ {\r\n      if (!are_same_lms_substr(plms, A[i]))\r\n        ++lms_ord;\r\n \
    \     cmp(A[i]) = lms_ord;\r\n      plms = A[i];\r\n    }\r\n    int k = 0;\r\n\
    \    for (int i = 1; i < n; i++) if (is_lms(i))\r\n      lmss[k++] = cmp(i);\r\
    \n    lmss.resize(cnt_lms);\r\n    sa_is(lmss, bucket, sa_lmss.begin(), cnt_lms\
    \ - 1 == lms_ord);\r\n  }\r\n\r\n  // Induced-sort by ordered LMSs\r\n  auto&\
    \ citi = lmss; // Reuse memory\r\n  citi.clear();\r\n  for (int i = 1; i < n;\
    \ i++) if (is_lms(i))\r\n    citi.push_back(i);\r\n  init_bucket(cnt);\r\n  memset(A,\
    \ -1, sizeof(int)*n);\r\n  for (auto it = sa_lmss.rbegin(); it != sa_lmss.rend();\
    \ ++it)\r\n    A[bucket[S[citi[*it]]].second--] = citi[*it];\r\n  induced_sort();\r\
    \n}\r\n\r\ntemplate<typename It>\r\n[[nodiscard]]\r\nstd::vector<int> SuffixArray(It\
    \ begin, It end) {\r\n  using trait = std::iterator_traits<It>;\r\n  static_assert(std::is_base_of<std::random_access_iterator_tag,\
    \ typename trait::iterator_category>::value, \"\");\r\n\r\n  int n = end - begin;\r\
    \n  std::vector<int> S(n+1);\r\n  S[n] = 0;\r\n  auto mp = std::minmax_element(begin,\
    \ end);\r\n  if (0 < (int)*mp.first and (int)*mp.second <= n) {\r\n    std::copy(begin,\
    \ end, S.begin());\r\n  } else {\r\n    std::vector<int> ord(n);\r\n    std::iota(ord.begin(),\
    \ ord.end(), 0);\r\n    std::sort(ord.begin(), ord.end(), [&](int l, int r) {\r\
    \n      return *(begin+l) < *(begin+r);\r\n    });\r\n    int k = 1;\r\n    for\
    \ (int i = 0; i < n; i++)\r\n      S[ord[i]] = i and *(begin+ord[i]) != *(begin+ord[i-1])\
    \ ? ++k : k;\r\n  }\r\n  std::vector<int> A(n+1);\r\n  std::vector<std::pair<int,int>>\
    \ bucket(n+1);\r\n  sa_is(S, bucket, A.begin(), false);\r\n  return std::vector<int>(A.begin()+1,\
    \ A.end());\r\n}"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/string/suffix_array.hpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/string/suffix_array.test.cpp
documentation_of: include/mtl/string/suffix_array.hpp
layout: document
redirect_from:
- /library/include/mtl/string/suffix_array.hpp
- /library/include/mtl/string/suffix_array.hpp.html
title: include/mtl/string/suffix_array.hpp
---
