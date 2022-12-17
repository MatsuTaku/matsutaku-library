---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: include/mtl/ordinal_range_search.hpp
    title: include/mtl/ordinal_range_search.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/yosupo-line_add_get_min.test.cpp
    title: test/yosupo-line_add_get_min.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo-point_add_rectangle_sum-wm.test.cpp
    title: test/yosupo-point_add_rectangle_sum-wm.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo-segment_add_get_min.test.cpp
    title: test/yosupo-segment_add_get_min.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/compress_int.hpp\"\n#include <set>\r\n#include\
    \ <unordered_map>\r\n#include <vector>\r\n#include <algorithm>\r\n\r\ntemplate<typename\
    \ T>\r\nstruct Compressor {\r\n  std::vector<T> vs;\r\n  Compressor() = default;\r\
    \n  template<typename It>\r\n  Compressor(It begin, It end) : vs(begin, end) {}\r\
    \n  void clear() { vs.clear(); }\r\n  void add(T x) {\r\n    vs.push_back(x);\r\
    \n  }\r\n  template<typename It>\r\n  void add(It begin, It end) {\r\n    vs.insert(vs.end(),\
    \ begin, end);\r\n  }\r\n  using map_type = std::unordered_map<T,int>;\r\n  map_type\
    \ release() {\r\n    std::sort(vs.begin(), vs.end());\r\n    vs.erase(std::unique(vs.begin(),\
    \ vs.end()), vs.end());\r\n    std::unordered_map<T,int> mp;\r\n    mp.reserve(vs.size());\r\
    \n    int k = 0;\r\n    for (auto v : vs) mp[v] = k++;\r\n    return mp;\r\n \
    \ }\r\n  std::pair<map_type, std::vector<T>> release_tie() {\r\n    return std::make_pair(release(),\
    \ vs);\r\n  }\r\n  template<typename It>\r\n  static std::unordered_map<T,int>\
    \ compress(It begin, It end) {\r\n    return Compressor(begin, end).release();\r\
    \n  }\r\n};\r\n"
  code: "#pragma once\r\n#include <set>\r\n#include <unordered_map>\r\n#include <vector>\r\
    \n#include <algorithm>\r\n\r\ntemplate<typename T>\r\nstruct Compressor {\r\n\
    \  std::vector<T> vs;\r\n  Compressor() = default;\r\n  template<typename It>\r\
    \n  Compressor(It begin, It end) : vs(begin, end) {}\r\n  void clear() { vs.clear();\
    \ }\r\n  void add(T x) {\r\n    vs.push_back(x);\r\n  }\r\n  template<typename\
    \ It>\r\n  void add(It begin, It end) {\r\n    vs.insert(vs.end(), begin, end);\r\
    \n  }\r\n  using map_type = std::unordered_map<T,int>;\r\n  map_type release()\
    \ {\r\n    std::sort(vs.begin(), vs.end());\r\n    vs.erase(std::unique(vs.begin(),\
    \ vs.end()), vs.end());\r\n    std::unordered_map<T,int> mp;\r\n    mp.reserve(vs.size());\r\
    \n    int k = 0;\r\n    for (auto v : vs) mp[v] = k++;\r\n    return mp;\r\n \
    \ }\r\n  std::pair<map_type, std::vector<T>> release_tie() {\r\n    return std::make_pair(release(),\
    \ vs);\r\n  }\r\n  template<typename It>\r\n  static std::unordered_map<T,int>\
    \ compress(It begin, It end) {\r\n    return Compressor(begin, end).release();\r\
    \n  }\r\n};\r\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/compress_int.hpp
  requiredBy:
  - include/mtl/ordinal_range_search.hpp
  timestamp: '2022-11-27 16:09:45+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/yosupo-line_add_get_min.test.cpp
  - test/yosupo-point_add_rectangle_sum-wm.test.cpp
  - test/yosupo-segment_add_get_min.test.cpp
documentation_of: include/mtl/compress_int.hpp
layout: document
redirect_from:
- /library/include/mtl/compress_int.hpp
- /library/include/mtl/compress_int.hpp.html
title: include/mtl/compress_int.hpp
---
