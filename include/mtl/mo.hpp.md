---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':x:'
    path: test/yosupo/static_range_inversions_query-mo.test.cpp
    title: test/yosupo/static_range_inversions_query-mo.test.cpp
  - icon: ':x:'
    path: test/yosupo/static_range_inversions_query-mo_rollback.test.cpp
    title: test/yosupo/static_range_inversions_query-mo_rollback.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':x:'
  attributes:
    document_title: 'Mo''s algorithm: solve offline segment queries on a sequence'
    links:
    - https://noshi91.hatenablog.com/entry/2023/04/13/224811
    - https://snuke.hatenablog.com/entry/2016/07/01/000000
  bundledCode: "#line 2 \"include/mtl/mo.hpp\"\n#include <algorithm>\r\n#include <numeric>\r\
    \n#include <cmath>\r\n#include <tuple>\r\n#include <vector>\r\n#include <iostream>\r\
    \n#include <cassert>\r\n\r\n/**\r\n * @brief Mo's algorithm: solve offline segment\
    \ queries on a sequence\r\n * @note  This implementation is optimized by noshi's\
    \ idea\r\n *          complexity of N sqrt(Q) + O(N).\r\n *        - \u5B9A\u6570\
    \u500D\u304C\u6700\u9069\u306A Mo's Algorithm. noshi91\u306E\u30E1\u30E2. 2023/04/13.\r\
    \n *          https://noshi91.hatenablog.com/entry/2023/04/13/224811\r\n * @note\
    \  Mo's algorithm with rollback is based on snuke's idea\r\n *        - Mo's algorithm\
    \ \u3068\u305D\u306E\u4E0A\u4F4D\u4E92\u63DB\u306E\u8A71. \u3042\u306A\u305F\u306F\
    \u5618\u3064\u304D\u3067\u3059\u304B\u3068\u805E\u304B\u308C\u305F\u3089\u300C\
    YES\u300D\u3068\u7B54\u3048\u308B\u30D6\u30ED\u30B0. 2016/07/01.\r\n *       \
    \   https://snuke.hatenablog.com/entry/2016/07/01/000000\r\n*/\r\nstruct MoSolver\
    \ {\r\n  std::vector<std::tuple<int, int, int>> segs;\r\n  int q = 0;\r\n\r\n\
    \  void add_segment(int l, int r) { \r\n    assert(l <= r);\r\n    segs.emplace_back(q++,\
    \ l, r);\r\n  }\r\n\r\n  void calc_mos_move(std::vector<int>& dst) const {\r\n\
    \    using std::get;\r\n    int n = 0;\r\n    for (auto s:segs)\r\n      n = std::max(n,\
    \ get<2>(s));\r\n    auto rtq = std::sqrt(q);\r\n    int b = std::ceil((double)n\
    \ / rtq);\r\n    auto bf = b-b/2;\r\n    auto get_bo = [&](int x) {\r\n      if\
    \ (x < bf) return 0;\r\n      return (x-bf)/b+1;\r\n    };\r\n    auto EvenComp\
    \ = [&](int u, int v) {\r\n      auto &s = segs[u], &t = segs[v];\r\n      auto\
    \ ls = get<1>(s), rs = get<2>(s), lt = get<1>(t), rt = get<2>(t);\r\n      auto\
    \ bs = ls / b, bt = lt / b;\r\n      return bs != bt ? ls < lt : (bs%2==0 ? rs\
    \ < rt : rs > rt);\r\n    };\r\n    auto OddComp = [&](int u, int v) {\r\n   \
    \   auto &s = segs[u], &t = segs[v];\r\n      auto ls = get<1>(s), rs = get<2>(s),\
    \ lt = get<1>(t), rt = get<2>(t);\r\n      auto bs = get_bo(ls), bt = get_bo(lt);\r\
    \n      return bs != bt ? ls < lt : (bs%2==0 ? rs < rt : rs > rt);\r\n    };\r\
    \n    auto& IE = dst;\r\n    IE.resize(q);\r\n    std::iota(IE.begin(), IE.end(),\
    \ 0);\r\n    std::sort(IE.begin(), IE.end(), \r\n      EvenComp);\r\n    auto\
    \ IO = IE;\r\n    std::sort(IO.begin(), IO.end(), \r\n      OddComp);\r\n    auto\
    \ move_distance = [&](const std::vector<int>& ids) {\r\n      long long d = 0;\r\
    \n      for (int i = 0; i < q-1; i++) {\r\n        int j = ids[i], k = ids[i+1];\r\
    \n        d += std::abs(get<1>(segs[j]) - get<1>(segs[k]));\r\n        d += std::abs(get<2>(segs[j])\
    \ - get<2>(segs[k]));\r\n      }\r\n      return d;\r\n    };\r\n    if (move_distance(IE)\
    \ > move_distance(IO))\r\n      dst = std::move(IO); // IE is reference of dst\r\
    \n  }\r\n\r\n  template<class PUSH_L, class PUSH_R, class POP_L, class POP_R,\
    \ class REM>\r\n  void solve(PUSH_L pushl, PUSH_R pushr, POP_L popl, POP_R popr,\
    \ REM rem) const {\r\n    if (q == 0) return;\r\n    std::vector<int> I;\r\n \
    \   calc_mos_move(I);\r\n    int _l = 0, _r = 0;\r\n    for (int i:I) {\r\n  \
    \    int t,l,r;\r\n      std::tie(t,l,r) = segs[i];\r\n      while (l < _l)\r\n\
    \        pushl(--_l);\r\n      while (_r < r)\r\n        pushr(_r++);\r\n    \
    \  while (_l < l)\r\n        popl(_l++);\r\n      while (r < _r)\r\n        popr(--_r);\r\
    \n      rem(t);\r\n    }\r\n  }\r\n\r\n  template<class Block, class Bend>\r\n\
    \  long long calc_mos_rollback_move(std::vector<int>& idx, std::vector<std::pair<int,int>>&\
    \ blocks, Block block, Bend bend) const {\r\n    std::sort(idx.begin(), idx.end(),\
    \ [&](auto a, auto b) {\r\n      auto [ta, la, ra] = segs[a];\r\n      auto [tb,\
    \ lb, rb] = segs[b];\r\n      auto ba = block(la), bb = block(lb);\r\n      return\
    \ ba != bb ? la < lb : ra < rb;\r\n    });\r\n    long long dist = 0;\r\n    int\
    \ cb = -1;\r\n    int _l = 0, _r = 0;\r\n    for (size_t i = 0; i < idx.size();\
    \ i++) {\r\n      auto [t,l,r] = segs[idx[i]];\r\n      auto bi = block(l);\r\n\
    \      auto be = bend(bi);\r\n      blocks[i] = std::make_pair(bi, be);\r\n  \
    \    if (bi != cb) {\r\n        _r = be;\r\n        cb = bi;\r\n      }\r\n  \
    \    dist += r-_r;\r\n      _r = r;\r\n      _l = be;\r\n      dist += _l-l;\r\
    \n    }\r\n    return dist;\r\n  }\r\n\r\n  template<class PUSH_L, class PUSH_R,\
    \ class REM, \r\n           class INIT, class SNAPSHOT, class ROLLBACK> \r\n \
    \ void solve_rollback(PUSH_L pushl, PUSH_R pushr, REM rem, \r\n             INIT\
    \ init, SNAPSHOT snapshot, ROLLBACK rollback) const {\r\n    if (q == 0) return;\r\
    \n    int n = 0;\r\n    for (auto s:segs)\r\n      n = std::max(n, std::get<2>(s));\r\
    \n//  * (2|xi-c|+b/2)q, |xi-c| < b/2, (mean |xi-c| = b/4) -> bq\r\n//  * min bq\
    \ + n^2/b, \r\n//    from AMGM, bq = n^2/b => b^2 = n^2 /q => b = n / sqrt(q)\r\
    \n//  * F = (bq + n^2/b)/2\r\n//      = bq\r\n//      = n sqrt(q)\r\n    const\
    \ int b = std::ceil((double)n / std::sqrt(q));\r\n    std::vector<int> J;\r\n\
    \    for (int i = 0; i < q; i++) {\r\n      auto [t,l,r] = segs[i];\r\n      if\
    \ (r-l < b) {\r\n        init();\r\n        for (int j = l; j < r; j++)\r\n  \
    \        pushr(j);\r\n        rem(t);\r\n      } else {\r\n        J.push_back(i);\r\
    \n      }\r\n    }\r\n    \r\n    std::vector<std::pair<int,int>> B(J.size());\r\
    \n    {\r\n      auto& b_even = B;\r\n      auto block_even = [&](int x) {\r\n\
    \        return x / b;\r\n      };\r\n      auto bend_even = [&](int bi) {\r\n\
    \        return (bi+1)*b;\r\n      };\r\n      auto dist_e = calc_mos_rollback_move(J,\
    \ b_even, block_even, bend_even);\r\n\r\n      auto K = J;\r\n      auto b_odd\
    \ = B;\r\n      const auto bf = b-b/2;\r\n      auto block_odd = [&](int x) {\r\
    \n        return x < bf ? 0 : (x-bf)/b+1;\r\n      };\r\n      auto bend_odd =\
    \ [&](int bi) {\r\n        return bf+bi*b;\r\n      };\r\n      auto dist_o =\
    \ calc_mos_rollback_move(K, b_odd, block_odd, bend_odd);\r\n\r\n      if (dist_e\
    \ > dist_o) {\r\n        J = std::move(K);\r\n        B = std::move(b_odd);\r\n\
    \      }\r\n    }\r\n\r\n    int cb = -1;\r\n    int _l = 0, _r = 0;\r\n    for\
    \ (auto it = J.begin(); it != J.end(); ++it) {\r\n      auto [t,l,r] = segs[*it];\r\
    \n      auto [bi,be] = B[it-J.begin()];\r\n      if (bi != cb) {\r\n        init();\r\
    \n        _r = be;\r\n        cb = bi;\r\n      }\r\n      assert(_r <= r);\r\n\
    \      while (_r < r) {\r\n        pushr(_r++);\r\n      }\r\n      snapshot();\r\
    \n      _l = be;\r\n      assert(l <= _l);\r\n      assert(_l-l <= b);\r\n   \
    \   while (l < _l) {\r\n        pushl(--_l);\r\n      }\r\n      rem(t);\r\n \
    \     rollback();\r\n    }\r\n  }\r\n};\r\n"
  code: "#pragma once\r\n#include <algorithm>\r\n#include <numeric>\r\n#include <cmath>\r\
    \n#include <tuple>\r\n#include <vector>\r\n#include <iostream>\r\n#include <cassert>\r\
    \n\r\n/**\r\n * @brief Mo's algorithm: solve offline segment queries on a sequence\r\
    \n * @note  This implementation is optimized by noshi's idea\r\n *          complexity\
    \ of N sqrt(Q) + O(N).\r\n *        - \u5B9A\u6570\u500D\u304C\u6700\u9069\u306A\
    \ Mo's Algorithm. noshi91\u306E\u30E1\u30E2. 2023/04/13.\r\n *          https://noshi91.hatenablog.com/entry/2023/04/13/224811\r\
    \n * @note  Mo's algorithm with rollback is based on snuke's idea\r\n *      \
    \  - Mo's algorithm \u3068\u305D\u306E\u4E0A\u4F4D\u4E92\u63DB\u306E\u8A71. \u3042\
    \u306A\u305F\u306F\u5618\u3064\u304D\u3067\u3059\u304B\u3068\u805E\u304B\u308C\
    \u305F\u3089\u300CYES\u300D\u3068\u7B54\u3048\u308B\u30D6\u30ED\u30B0. 2016/07/01.\r\
    \n *          https://snuke.hatenablog.com/entry/2016/07/01/000000\r\n*/\r\nstruct\
    \ MoSolver {\r\n  std::vector<std::tuple<int, int, int>> segs;\r\n  int q = 0;\r\
    \n\r\n  void add_segment(int l, int r) { \r\n    assert(l <= r);\r\n    segs.emplace_back(q++,\
    \ l, r);\r\n  }\r\n\r\n  void calc_mos_move(std::vector<int>& dst) const {\r\n\
    \    using std::get;\r\n    int n = 0;\r\n    for (auto s:segs)\r\n      n = std::max(n,\
    \ get<2>(s));\r\n    auto rtq = std::sqrt(q);\r\n    int b = std::ceil((double)n\
    \ / rtq);\r\n    auto bf = b-b/2;\r\n    auto get_bo = [&](int x) {\r\n      if\
    \ (x < bf) return 0;\r\n      return (x-bf)/b+1;\r\n    };\r\n    auto EvenComp\
    \ = [&](int u, int v) {\r\n      auto &s = segs[u], &t = segs[v];\r\n      auto\
    \ ls = get<1>(s), rs = get<2>(s), lt = get<1>(t), rt = get<2>(t);\r\n      auto\
    \ bs = ls / b, bt = lt / b;\r\n      return bs != bt ? ls < lt : (bs%2==0 ? rs\
    \ < rt : rs > rt);\r\n    };\r\n    auto OddComp = [&](int u, int v) {\r\n   \
    \   auto &s = segs[u], &t = segs[v];\r\n      auto ls = get<1>(s), rs = get<2>(s),\
    \ lt = get<1>(t), rt = get<2>(t);\r\n      auto bs = get_bo(ls), bt = get_bo(lt);\r\
    \n      return bs != bt ? ls < lt : (bs%2==0 ? rs < rt : rs > rt);\r\n    };\r\
    \n    auto& IE = dst;\r\n    IE.resize(q);\r\n    std::iota(IE.begin(), IE.end(),\
    \ 0);\r\n    std::sort(IE.begin(), IE.end(), \r\n      EvenComp);\r\n    auto\
    \ IO = IE;\r\n    std::sort(IO.begin(), IO.end(), \r\n      OddComp);\r\n    auto\
    \ move_distance = [&](const std::vector<int>& ids) {\r\n      long long d = 0;\r\
    \n      for (int i = 0; i < q-1; i++) {\r\n        int j = ids[i], k = ids[i+1];\r\
    \n        d += std::abs(get<1>(segs[j]) - get<1>(segs[k]));\r\n        d += std::abs(get<2>(segs[j])\
    \ - get<2>(segs[k]));\r\n      }\r\n      return d;\r\n    };\r\n    if (move_distance(IE)\
    \ > move_distance(IO))\r\n      dst = std::move(IO); // IE is reference of dst\r\
    \n  }\r\n\r\n  template<class PUSH_L, class PUSH_R, class POP_L, class POP_R,\
    \ class REM>\r\n  void solve(PUSH_L pushl, PUSH_R pushr, POP_L popl, POP_R popr,\
    \ REM rem) const {\r\n    if (q == 0) return;\r\n    std::vector<int> I;\r\n \
    \   calc_mos_move(I);\r\n    int _l = 0, _r = 0;\r\n    for (int i:I) {\r\n  \
    \    int t,l,r;\r\n      std::tie(t,l,r) = segs[i];\r\n      while (l < _l)\r\n\
    \        pushl(--_l);\r\n      while (_r < r)\r\n        pushr(_r++);\r\n    \
    \  while (_l < l)\r\n        popl(_l++);\r\n      while (r < _r)\r\n        popr(--_r);\r\
    \n      rem(t);\r\n    }\r\n  }\r\n\r\n  template<class Block, class Bend>\r\n\
    \  long long calc_mos_rollback_move(std::vector<int>& idx, std::vector<std::pair<int,int>>&\
    \ blocks, Block block, Bend bend) const {\r\n    std::sort(idx.begin(), idx.end(),\
    \ [&](auto a, auto b) {\r\n      auto [ta, la, ra] = segs[a];\r\n      auto [tb,\
    \ lb, rb] = segs[b];\r\n      auto ba = block(la), bb = block(lb);\r\n      return\
    \ ba != bb ? la < lb : ra < rb;\r\n    });\r\n    long long dist = 0;\r\n    int\
    \ cb = -1;\r\n    int _l = 0, _r = 0;\r\n    for (size_t i = 0; i < idx.size();\
    \ i++) {\r\n      auto [t,l,r] = segs[idx[i]];\r\n      auto bi = block(l);\r\n\
    \      auto be = bend(bi);\r\n      blocks[i] = std::make_pair(bi, be);\r\n  \
    \    if (bi != cb) {\r\n        _r = be;\r\n        cb = bi;\r\n      }\r\n  \
    \    dist += r-_r;\r\n      _r = r;\r\n      _l = be;\r\n      dist += _l-l;\r\
    \n    }\r\n    return dist;\r\n  }\r\n\r\n  template<class PUSH_L, class PUSH_R,\
    \ class REM, \r\n           class INIT, class SNAPSHOT, class ROLLBACK> \r\n \
    \ void solve_rollback(PUSH_L pushl, PUSH_R pushr, REM rem, \r\n             INIT\
    \ init, SNAPSHOT snapshot, ROLLBACK rollback) const {\r\n    if (q == 0) return;\r\
    \n    int n = 0;\r\n    for (auto s:segs)\r\n      n = std::max(n, std::get<2>(s));\r\
    \n//  * (2|xi-c|+b/2)q, |xi-c| < b/2, (mean |xi-c| = b/4) -> bq\r\n//  * min bq\
    \ + n^2/b, \r\n//    from AMGM, bq = n^2/b => b^2 = n^2 /q => b = n / sqrt(q)\r\
    \n//  * F = (bq + n^2/b)/2\r\n//      = bq\r\n//      = n sqrt(q)\r\n    const\
    \ int b = std::ceil((double)n / std::sqrt(q));\r\n    std::vector<int> J;\r\n\
    \    for (int i = 0; i < q; i++) {\r\n      auto [t,l,r] = segs[i];\r\n      if\
    \ (r-l < b) {\r\n        init();\r\n        for (int j = l; j < r; j++)\r\n  \
    \        pushr(j);\r\n        rem(t);\r\n      } else {\r\n        J.push_back(i);\r\
    \n      }\r\n    }\r\n    \r\n    std::vector<std::pair<int,int>> B(J.size());\r\
    \n    {\r\n      auto& b_even = B;\r\n      auto block_even = [&](int x) {\r\n\
    \        return x / b;\r\n      };\r\n      auto bend_even = [&](int bi) {\r\n\
    \        return (bi+1)*b;\r\n      };\r\n      auto dist_e = calc_mos_rollback_move(J,\
    \ b_even, block_even, bend_even);\r\n\r\n      auto K = J;\r\n      auto b_odd\
    \ = B;\r\n      const auto bf = b-b/2;\r\n      auto block_odd = [&](int x) {\r\
    \n        return x < bf ? 0 : (x-bf)/b+1;\r\n      };\r\n      auto bend_odd =\
    \ [&](int bi) {\r\n        return bf+bi*b;\r\n      };\r\n      auto dist_o =\
    \ calc_mos_rollback_move(K, b_odd, block_odd, bend_odd);\r\n\r\n      if (dist_e\
    \ > dist_o) {\r\n        J = std::move(K);\r\n        B = std::move(b_odd);\r\n\
    \      }\r\n    }\r\n\r\n    int cb = -1;\r\n    int _l = 0, _r = 0;\r\n    for\
    \ (auto it = J.begin(); it != J.end(); ++it) {\r\n      auto [t,l,r] = segs[*it];\r\
    \n      auto [bi,be] = B[it-J.begin()];\r\n      if (bi != cb) {\r\n        init();\r\
    \n        _r = be;\r\n        cb = bi;\r\n      }\r\n      assert(_r <= r);\r\n\
    \      while (_r < r) {\r\n        pushr(_r++);\r\n      }\r\n      snapshot();\r\
    \n      _l = be;\r\n      assert(l <= _l);\r\n      assert(_l-l <= b);\r\n   \
    \   while (l < _l) {\r\n        pushl(--_l);\r\n      }\r\n      rem(t);\r\n \
    \     rollback();\r\n    }\r\n  }\r\n};\r\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/mo.hpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_ALL_WA
  verifiedWith:
  - test/yosupo/static_range_inversions_query-mo.test.cpp
  - test/yosupo/static_range_inversions_query-mo_rollback.test.cpp
documentation_of: include/mtl/mo.hpp
layout: document
redirect_from:
- /library/include/mtl/mo.hpp
- /library/include/mtl/mo.hpp.html
title: 'Mo''s algorithm: solve offline segment queries on a sequence'
---
