---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/dual_sparse_table.hpp
    title: Apply to [l, r)
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://yukicoder.me/problems/no/945
    links:
    - https://yukicoder.me/problems/no/945
  bundledCode: "#line 1 \"test/yuki-no945_ykc_manju.test.cpp\"\n#define PROBLEM \"\
    https://yukicoder.me/problems/no/945\"\n#line 2 \"include/mtl/bit_manip.hpp\"\n\
    #include <cstdint>\n#include <cassert>\n\nnamespace bm {\n\ninline constexpr uint64_t\
    \ popcnt_e8(uint64_t x) {\n  x = (x & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);\n\
    \  x = (x & 0x3333333333333333) + ((x>>2) & 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F)\
    \ + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n  return x;\n}\n// Count 1s\ninline constexpr\
    \ unsigned popcnt(uint64_t x) {\n  return (popcnt_e8(x) * 0x0101010101010101)\
    \ >> 56;\n}\n// Count trailing 0s. ...01101000 -> 3\ninline constexpr unsigned\
    \ ctz(uint64_t x) {\n  return popcnt((x & (-x)) - 1);\n}\ninline constexpr unsigned\
    \ ctz8(uint8_t x) {\n  return x == 0 ? 8 : popcnt_e8((x & (-x)) - 1);\n}\n// [00..0](8bit)\
    \ -> 0, [**..*](not only 0) -> 1\ninline constexpr uint8_t summary(uint64_t x)\
    \ {\n  constexpr uint64_t hmask = 0x8080808080808080ull;\n  constexpr uint64_t\
    \ lmask = 0x7F7F7F7F7F7F7F7Full;\n  auto a = x & hmask;\n  auto b = x & lmask;\n\
    \  b = hmask - b;\n  b = ~b;\n  auto c = (a | b) & hmask;\n  c *= 0x0002040810204081ull;\n\
    \  return uint8_t(c >> 56);\n}\n// Extract target area of bits\ninline constexpr\
    \ uint64_t bextr(uint64_t x, unsigned start, unsigned len) {\n  uint64_t mask\
    \ = len < 64 ? (1ull<<len)-1 : 0xFFFFFFFFFFFFFFFFull;\n  return (x >> start) &\
    \ mask;\n}\n// 00101101 -> 00111111 -count_1s-> 6\ninline constexpr unsigned log2p1(uint8_t\
    \ x) {\n  if (x & 0x80)\n    return 8;\n  uint64_t p = uint64_t(x) * 0x0101010101010101ull;\n\
    \  p -= 0x8040201008040201ull;\n  p = ~p & 0x8080808080808080ull;\n  p = (p >>\
    \ 7) * 0x0101010101010101ull;\n  p >>= 56;\n  return p;\n}\n// 00101100 -mask_mssb->\
    \ 00100000 -to_index-> 5\ninline constexpr unsigned mssb8(uint8_t x) {\n  assert(x\
    \ != 0);\n  return log2p1(x) - 1;\n}\n// 00101100 -mask_lssb-> 00000100 -to_index->\
    \ 2\ninline constexpr unsigned lssb8(uint8_t x) {\n  assert(x != 0);\n  return\
    \ popcnt_e8((x & -x) - 1);\n}\n// Count leading 0s. 00001011... -> 4\ninline constexpr\
    \ unsigned clz(uint64_t x) {\n  if (x == 0)\n    return 64;\n  auto i = mssb8(summary(x));\n\
    \  auto j = mssb8(bextr(x, 8 * i, 8));\n  return 63 - (8 * i + j);\n}\ninline\
    \ constexpr unsigned clz8(uint8_t x) {\n  return x == 0 ? 8 : 7 - mssb8(x);\n\
    }\ninline constexpr uint64_t bit_reverse(uint64_t x) {\n  x = ((x & 0x00000000FFFFFFFF)\
    \ << 32) | ((x & 0xFFFFFFFF00000000) >> 32);\n  x = ((x & 0x0000FFFF0000FFFF)\
    \ << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);\n  x = ((x & 0x00FF00FF00FF00FF)\
    \ << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);\n  x = ((x & 0x0F0F0F0F0F0F0F0F) <<\
    \ 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);\n  x = ((x & 0x3333333333333333) << 2)\
    \ | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);\n  x = ((x & 0x5555555555555555) << 1) |\
    \ ((x & 0xAAAAAAAAAAAAAAAA) >> 1);\n  return x;\n}\n\n} // namespace bm\n#line\
    \ 3 \"include/mtl/dual_sparse_table.hpp\"\n#include <vector>\n#include <algorithm>\n\
    #include <iostream>\ntemplate <typename T, T (*op)(T, T), T (*e)()>\nclass DualSparseTable\
    \ {\n private:\n  size_t size_, log_n_;\n  std::vector<std::vector<T>> table_;\n\
    \n public:\n  DualSparseTable(size_t size) :\n      size_(size),\n      log_n_(63-bm::clz(size)),\n\
    \      table_(log_n_+1, std::vector<T>(size, e())) {}\n  template <typename Iter>\n\
    \  DualSparseTable(Iter begin, Iter end) :\n      DualSparseTable(std::distance(begin,\
    \ end)) {\n    std::transform(begin, end, table_[0].begin(), [](const auto& v)\
    \ {return v;});\n  }\n\n/***\n * @brief Apply to [l, r)\n * @note Complexity:\
    \ O(1)\n */\n  void apply(size_t l, size_t r, const T& a) {\n    if (l>=r) return;\n\
    \    auto d = 63-bm::clz(r-l);\n    table_[d][l] = op(table_[d][l], a);\n    table_[d][r-(1ull<<d)]\
    \ = op(table_[d][r-(1ull<<d)], a);\n  }\n\n/***\n * @brief Build the table\n *\
    \ @note Complexity: O(n log n)\n */\n  void build() {\n    for (int log_n = (int)log_n_-1;\
    \ log_n >= 0; log_n--) {\n      size_t width = 1ull<<log_n;\n      for (size_t\
    \ i = 0; i < size_-width; i++) {\n        table_[log_n][i] = op(table_[log_n][i],\
    \ table_[log_n+1][i]);\n        table_[log_n][i+width] = op(table_[log_n][i+width],\
    \ table_[log_n+1][i]);\n      }\n    }\n  }\n\n  T get(size_t i) const {\n   \
    \ return table_[0][i];\n  }\n\n};\n#line 3 \"test/yuki-no945_ykc_manju.test.cpp\"\
    \n#include <bits/stdc++.h>\nusing namespace std;\n\nusing P = pair<int,int>;\n\
    P f(P a, P b) {return min(a,b);}\nconstexpr int INF = 1e9;\nP e() {return {INF,\
    \ 3};}\nusing DST = DualSparseTable<P, f, e>;\n\nint main() {\n    int n,m; cin>>n>>m;\n\
    \    map<char,int> mp;\n    mp['Y']=0;\n    mp['K']=1;\n    mp['C']=2;\n    DST\
    \ dst(n);\n    for (int i = 0; i < m; i++) {\n        int l,r;\n        char t;\n\
    \        cin>>l>>r>>t;\n        l--;\n        dst.apply(l, r, {i, mp[t]});\n \
    \   } \n    dst.build();\n    array<int,4> ans{};\n    for (int i = 0; i < n;\
    \ i++) {\n        auto [id,t] = dst.get(i);\n        ans[t]++;\n    }\n    cout\
    \ << ans[0] << ' ' << ans[1] << ' ' << ans[2] << endl;\n}\n"
  code: "#define PROBLEM \"https://yukicoder.me/problems/no/945\"\n#include \"../include/mtl/dual_sparse_table.hpp\"\
    \n#include <bits/stdc++.h>\nusing namespace std;\n\nusing P = pair<int,int>;\n\
    P f(P a, P b) {return min(a,b);}\nconstexpr int INF = 1e9;\nP e() {return {INF,\
    \ 3};}\nusing DST = DualSparseTable<P, f, e>;\n\nint main() {\n    int n,m; cin>>n>>m;\n\
    \    map<char,int> mp;\n    mp['Y']=0;\n    mp['K']=1;\n    mp['C']=2;\n    DST\
    \ dst(n);\n    for (int i = 0; i < m; i++) {\n        int l,r;\n        char t;\n\
    \        cin>>l>>r>>t;\n        l--;\n        dst.apply(l, r, {i, mp[t]});\n \
    \   } \n    dst.build();\n    array<int,4> ans{};\n    for (int i = 0; i < n;\
    \ i++) {\n        auto [id,t] = dst.get(i);\n        ans[t]++;\n    }\n    cout\
    \ << ans[0] << ' ' << ans[1] << ' ' << ans[2] << endl;\n}"
  dependsOn:
  - include/mtl/dual_sparse_table.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: true
  path: test/yuki-no945_ykc_manju.test.cpp
  requiredBy: []
  timestamp: '2023-04-04 01:36:55+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/yuki-no945_ykc_manju.test.cpp
layout: document
redirect_from:
- /verify/test/yuki-no945_ykc_manju.test.cpp
- /verify/test/yuki-no945_ykc_manju.test.cpp.html
title: test/yuki-no945_ykc_manju.test.cpp
---
