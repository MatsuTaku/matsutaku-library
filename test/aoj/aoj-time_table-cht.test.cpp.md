---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/convex_hull_trick.hpp
    title: include/mtl/convex_hull_trick.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/search.hpp
    title: include/mtl/search.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://onlinejudge.u-aizu.ac.jp/problems/2603
    links:
    - https://onlinejudge.u-aizu.ac.jp/problems/2603
  bundledCode: "#line 1 \"test/aoj/aoj-time_table-cht.test.cpp\"\n#define PROBLEM\
    \ \"https://onlinejudge.u-aizu.ac.jp/problems/2603\"\r\n#line 2 \"include/mtl/convex_hull_trick.hpp\"\
    \n#include <utility>\r\n#include <cassert>\r\n#include <tuple>\r\n#include <vector>\r\
    \n#include <deque>\r\n#include <set>\r\n#include <map>\r\n#include <algorithm>\r\
    \n#include <cstddef>\r\n#include <iostream>\r\n#include <limits>\r\n#line 3 \"\
    include/mtl/search.hpp\"\n#include <type_traits>\r\n#include <functional>\r\n\
    #include <cmath>\r\n#line 8 \"include/mtl/search.hpp\"\n\r\nconstexpr double EPS_DEFAULT\
    \ = 1e-9;\r\n\r\ntemplate<typename I, typename F>\r\ntypename std::remove_reference<I>::type\r\
    \nbisect_int(I ok, I ng, F f) {\r\n  while (std::abs(ng - ok) > 1) {\r\n    auto\
    \ c = ok + (ng - ok) / 2;\r\n    if (f(c))\r\n      ok = c;\r\n    else\r\n  \
    \    ng = c;\r\n  }\r\n  return ok;\r\n}\r\n\r\ntemplate<typename F>\r\ndouble\r\
    \nbisect_float(double ok, double ng, F fn, double eps = EPS_DEFAULT) {\r\n  while\
    \ (std::abs(ok - ng) > eps) {\r\n    double c = ok + (ng - ok) / 2;\r\n    if\
    \ (fn(c))\r\n      ok = c;\r\n    else\r\n      ng = c;\r\n  }\r\n  return ok;\r\
    \n}\r\n\r\ntemplate<typename T, typename I, typename F, typename C>\r\nstd::pair<typename\
    \ std::remove_reference<I>::type, T>\r\nfibonacci_search(I l, I r, F fn, C cmp\
    \ = std::less<T>()) {\r\n  using idx_type = typename std::remove_reference<I>::type;\r\
    \n  assert(r - l >= 2);\r\n  idx_type d = r-l;\r\n  std::vector<idx_type> fib{1,1};\r\
    \n  while (fib.back() < d) fib.push_back(fib[fib.size()-2] + fib[fib.size()-1]);\r\
    \n  auto k = --fib.cend();\r\n  idx_type il = l + *(k-2);\r\n  idx_type ir = l\
    \ + *(k-1);\r\n  T lv = fn(il);\r\n  T rv = fn(ir);\r\n  std::pair<idx_type, T>\
    \ ret = cmp(lv, rv) ? std::make_pair(ir, rv) : std::make_pair(il, lv);\r\n  while\
    \ (*k > 3) {\r\n    if (cmp(lv, rv)) {\r\n      l += *(k-2);\r\n      lv = rv;\r\
    \n      auto i = std::min(r-1, l + *(k-2));\r\n      rv = fn(i);\r\n      if (cmp(ret.second,\
    \ rv))\r\n        ret = std::make_pair(i, rv);\r\n    } else {\r\n      rv = lv;\r\
    \n      auto i = std::min(r-1, l + *(k-3));\r\n      lv = fn(i);\r\n      if (cmp(ret.second,\
    \ lv))\r\n        ret = std::make_pair(i, lv);\r\n    }\r\n    --k;\r\n  }\r\n\
    \  return ret;\r\n}\r\ntemplate<typename T, typename I, typename F, typename C\
    \ = std::less<T>>\r\nstd::pair<typename std::remove_reference<I>::type, T>\r\n\
    trisect_int(I l, I r, F fn, C cmp = C()) {\r\n  return fibonacci_search<T>(l,\
    \ r, fn, cmp);\r\n}\r\n\r\nconstexpr double GOLDEN_RATIO = 1.61803398875;\r\n\
    template<typename T, typename F, typename C>\r\nstd::pair<double, T>\r\ngolden_ratio_search(double\
    \ l, double r, F fn, double eps = EPS_DEFAULT, C cmp = std::less<T>()) {\r\n \
    \ std::pair<double, T> ret;\r\n  while (std::abs(r - l) > eps) {\r\n    double\
    \ il = (l * GOLDEN_RATIO + r) / (GOLDEN_RATIO + 1.);\r\n    double ir = (l + r\
    \ * GOLDEN_RATIO) / (GOLDEN_RATIO + 1.);\r\n    T vl = fn(il);\r\n    T vr = fn(ir);\r\
    \n    if (cmp(vr, vl)) {\r\n      ret = std::make_pair(il, vl);\r\n      r = ir;\r\
    \n    } else {\r\n      ret = std::make_pair(ir, vr);\r\n      l = il;\r\n   \
    \ }\r\n  }\r\n  return ret;\r\n}\r\ntemplate<typename T, typename F, typename\
    \ C>\r\nstd::pair<double, T>\r\ntrisect_float(double l, double r, F fn, double\
    \ eps = EPS_DEFAULT, C cmp = std::less<T>()) {\r\n  return golden_ratio_search<T>(l,\
    \ r, fn, eps, cmp);\r\n}\r\n#line 14 \"include/mtl/convex_hull_trick.hpp\"\n\r\
    \ntemplate<typename T, typename C = std::less<T>>\r\nstruct ConvexHullTrickDeque\
    \ {\r\n  std::deque<std::pair<T, T>> L;\r\n  C cmp;\r\n  ConvexHullTrickDeque()\
    \ = default;\r\n\r\n private:\r\n  template<typename U, typename V, typename W>\r\
    \n  bool intersect_less(const U& g, const V& h, const W& i) {\r\n    assert(g.first\
    \ != h.first and h.first != i.first);\r\n    // x0 = (d-b)/(a-c)\r\n    // x1\
    \ = (f-d)/(c-e)\r\n    // x0 < x1 <=> (d-b)/(a-c) < (f-d)/(c-e)\r\n    //    \
    \     <=> (d-b)(c-e) < (f-d)(a-c)\r\n    return (h.second-g.second)*(h.first-i.first)\
    \ <\r\n        (i.second-h.second)*(g.first-h.first);\r\n  }\r\n\r\n public:\r\
    \n  void clear() {\r\n    L.clear();\r\n  }\r\n  void push_back(T a, T b) {\r\n\
    \    assert(L.empty() or cmp(L.back().first, a));\r\n    auto l = std::make_pair(a,\
    \ b);\r\n    while (L.size() >= 2 and !intersect_less(L[L.size()-2], L[L.size()-1],\
    \ l))\r\n      L.pop_back();\r\n    L.push_back(l);\r\n  }\r\n  void push_front(T\
    \ a, T b) {\r\n    assert(L.empty() or cmp(a, L.front().first));\r\n    auto l\
    \ = std::make_pair(a, b);\r\n    while (L.size() >= 2 and !intersect_less(l, L[0],\
    \ L[1]))\r\n      L.pop_front();\r\n    L.push_front(l);\r\n  }\r\n\r\n  T get(T\
    \ x) const {\r\n    auto i = bisect_int(-1, (int)L.size()-1, [&](int c) {\r\n\
    \      if (c == -1) return true;\r\n      auto yl = L[c].first * x + L[c].second;\r\
    \n      auto yr = L[c+1].first * x + L[c+1].second;\r\n      return cmp(yl, yr);\r\
    \n    });\r\n    int r = i+1;\r\n    return L[r].first * x + L[r].second;\r\n\
    \  }\r\n\r\n  T get_incremental(T x) {\r\n    assert(!L.empty());\r\n    T ret\
    \ = L[0].first * x + L[0].second;\r\n    while (L.size() >= 2 and cmp(ret, L[1].first\
    \ * x + L[1].second)) {\r\n      ret = L[1].first * x + L[1].second;\r\n     \
    \ L.pop_front();\r\n    }\r\n    return ret;\r\n  }\r\n\r\n  T get_decremental(T\
    \ x) {\r\n    assert(!L.empty());\r\n    T ret = L.back().first * x + L.back().second;\r\
    \n    while (L.size() >= 2 and cmp(ret, L[L.size()-2].first * x + L[L.size()-2].second))\
    \ {\r\n      ret = L[L.size()-2].first * x + L[L.size()-2].second;\r\n      L.pop_back();\r\
    \n    }\r\n    return ret;\r\n  }\r\n};\r\n\r\n\r\ntemplate<typename T, typename\
    \ C = std::less<T>>\r\nstruct ConvexHullTrick {\r\n  using Line = std::pair<T,\
    \ T>;\r\n  struct Node;\r\n  std::map<Node, Line> tr;\r\n  std::map<T, T, C> L;\r\
    \n  C cmp;\r\n  static constexpr T INF = std::numeric_limits<T>::max();\r\n  static\
    \ constexpr T MINF = std::numeric_limits<T>::min();\r\n  static constexpr T AX_LO\
    \ = C()(MINF, INF) ? MINF : INF;\r\n  static constexpr T AX_HI = C()(MINF, INF)\
    \ ? INF : MINF;\r\n  ConvexHullTrick() {\r\n    Line lo = {AX_LO, 0};\r\n    Line\
    \ hi = {AX_HI, 0};\r\n    tr.emplace(Node(lo, hi), lo);\r\n    L.insert(lo);\r\
    \n    L.emplace_hint(L.end(), hi);\r\n  }\r\n\r\n  struct Node {\r\n    T u,v;\
    \ // a-c, d-b\r\n    Node() = default;\r\n    Node(T _u, T _v) : u(_u), v(_v)\
    \ {}\r\n    explicit Node(const Line& f, const Line& g) {\r\n      if (f.first\
    \ == AX_LO) {\r\n        u = 0, v = MINF;\r\n      } else if (g.first == AX_HI)\
    \ {\r\n        u = 0, v = INF;\r\n      } else {\r\n        u = f.first - g.first,\
    \ v = g.second - f.second;\r\n        if (u < 0) {\r\n          u = -u;\r\n  \
    \        v = -v;\r\n        }\r\n      }\r\n    }\r\n    inline bool operator==(const\
    \ Node& rhs) const {\r\n      return u == rhs.u and v == rhs.v;\r\n    }\r\n \
    \   inline bool operator<(const Node& rhs) const {\r\n      if (v == INF or rhs.v\
    \ == MINF)\r\n        return false;\r\n      if (v == MINF or rhs.v == INF)\r\n\
    \        return true;\r\n      // x0 = (d-b)/(a-c)\r\n      // x1 = (f-d)/(c-e)\r\
    \n      // x0 < x1 <=> (d-b)/(a-c) < (f-d)/(c-e)\r\n      //         <=> (d-b)(c-e)\
    \ < (f-d)(a-c)\r\n      assert((double)MINF < (double) v*rhs.u and (double) v*rhs.u\
    \ < (double)INF);\r\n      assert((double)MINF < (double) rhs.v * u and (double)\
    \ rhs.v * u < (double)INF);\r\n      return v * rhs.u < rhs.v * u;\r\n    }\r\n\
    \  };\r\n\r\n private:\r\n  template<typename U, typename V, typename W>\r\n \
    \ bool intersect_less(const U& g, const V& h, const W& i) {\r\n    if (g.first\
    \ == h.first)\r\n      return cmp(g.second, h.second);\r\n    if (h.first == i.first)\r\
    \n      return cmp(i.second, h.second);\r\n    if (g.first == AX_LO or i.first\
    \ == AX_HI)\r\n      return true;\r\n    // x0 = (d-b)/(a-c)\r\n    // x1 = (f-d)/(c-e)\r\
    \n    // x0 < x1 <=> (d-b)/(a-c) < (f-d)/(c-e)\r\n    //         <=> (d-b)(c-e)\
    \ < (f-d)(a-c)\r\n    auto s = (g.first-h.first), t = (h.second-g.second);\r\n\
    \    auto u = (h.first-i.first), v = (i.second-h.second);\r\n    assert((double)MINF\
    \ < (double)t*u and (double)t*u < (double)INF);\r\n    assert((double)MINF < (double)t*u\
    \ and (double)v*s < (double)INF);\r\n    return t * u < v * s;\r\n  }\r\n\r\n\
    \ public:\r\n  void add(T a, T b) {\r\n    Line line{a,b};\r\n    auto it = L.lower_bound(a);\r\
    \n    assert(it != L.begin());\r\n    assert(it != L.end());\r\n    using std::prev;\r\
    \n    using std::next;\r\n    auto lit = prev(it);\r\n    if (it->first == a)\
    \ {\r\n      if (!cmp(it->second, b))\r\n        return;\r\n      lit = it++;\r\
    \n    } else if (!intersect_less(*lit, line, *it)) {\r\n      return;\r\n    }\r\
    \n    auto tit = tr.find(Node(*lit, *it));\r\n    assert(tit != tr.end());\r\n\
    \    auto teit = next(tit);\r\n    while (lit != L.begin() and !intersect_less(*prev(lit),\
    \ *lit, line)) {\r\n      --tit;\r\n      --lit;\r\n    }\r\n    while (next(it)\
    \ != L.end() and !intersect_less(line, *it, *next(it))) {\r\n      ++teit;\r\n\
    \      ++it;\r\n    }\r\n    tit = tr.erase(tit, teit);\r\n    it = L.erase(next(lit),\
    \ it);\r\n    assert(tit == tr.end() or Node(line,*it) < tit->first);\r\n    tit\
    \ = tr.emplace_hint(tit, Node(line, *it), line);\r\n    assert(tit != tr.end()\
    \ and Node(*lit,line) < tit->first);\r\n    tr.emplace_hint(tit, Node(*lit, line),\
    \ *lit);\r\n    L.emplace_hint(it, line);\r\n  }\r\n\r\n private:\r\n  inline\
    \ T f(const Line& l, T x) const {\r\n    return l.first * x + l.second;\r\n  }\r\
    \n\r\n public:\r\n  T get(T x) const {\r\n    auto it = tr.lower_bound(Node(1,x));\r\
    \n    assert(it != tr.end());\r\n    return f(it->second, x);\r\n  }\r\n\r\n \
    \ T get_incremental(T x) {\r\n    auto ts = tr.begin();\r\n    auto te = ts;\r\
    \n    auto s = next(L.begin());\r\n    auto e = s;\r\n    auto ne = next(e);\r\
    \n    auto le = prev(L.end());\r\n    while (ne != le and cmp(f(*e, x), f(*ne,\
    \ x))) {\r\n      ++te;\r\n      e = ne++;\r\n    }\r\n    if (ts != te) {\r\n\
    \      ++te;\r\n      ts = tr.erase(ts, te);\r\n      s = L.erase(s, e);\r\n \
    \     tr.emplace_hint(ts, Node(*prev(s), *s), *prev(s));\r\n    }\r\n    return\
    \ f(*s, x);\r\n  }\r\n\r\n  T get_decremental(T x) {\r\n    auto ts = tr.end();\r\
    \n    auto te = ts;\r\n    auto s = prev(prev(L.end()));\r\n    auto ps = prev(s);\r\
    \n    auto e = s;\r\n    while (ps != L.begin() and cmp(f(*s, x), f(*ps, x)))\
    \ {\r\n      --ts;\r\n      s = ps--;\r\n    }\r\n    if (ts != te) {\r\n    \
    \  --ts;\r\n      ts = tr.erase(ts, te);\r\n      L.erase(next(s), next(e));\r\
    \n      tr.emplace_hint(ts, Node(*s, *next(s)), *s);\r\n    }\r\n    return f(*s,\
    \ x);\r\n  }\r\n};\r\n#line 3 \"test/aoj/aoj-time_table-cht.test.cpp\"\n#include\
    \ <bits/stdc++.h>\r\nusing namespace std;\r\n\r\nint main() {\r\n  int s,n,m;\
    \ cin>>s>>n>>m;\r\n  vector<int> X(s);\r\n  for (int i = 0; i < s; i++) cin>>X[i];\r\
    \n  vector<pair<int,int>> B(n);\r\n  vector<int> T(n);\r\n  for (int i = 0; i\
    \ < n; i++) {\r\n    int t,p; cin>>t>>p; p--;\r\n    B[i] = {t, X[p]};\r\n   \
    \ T[i] = t-X[p];\r\n  }\r\n  sort(T.begin(), T.end());\r\n  vector<int> TS(n+1);\r\
    \n  for (int i = 1; i <= n; i++)\r\n    TS[i] = TS[i-1] + T[i-1];\r\n  // dp[r]\
    \ = min_{0<=l<=r} dp[l] + (r-l)*T[r-1] - (TS[r]-TS[l])\r\n  //       = min   \
    \        dp[l] - (l*T[r-1] - TS[l]) + r*T[r-1] - TS[r]\r\n  constexpr int INF\
    \ = 1e9;\r\n  vector<int> dp(n+1, INF);\r\n  dp[0] = 0;\r\n  ConvexHullTrickDeque<int,\
    \ greater<>> cht;\r\n  for (int i = 0; i < m; i++) {\r\n    cht.clear();\r\n \
    \   cht.push_back(-0, dp[0] + TS[0]);\r\n    for (int j = 1; j <= n; j++) {\r\n\
    \      auto x = T[j-1];\r\n      cht.push_back(-j, dp[j] + TS[j]);\r\n      dp[j]\
    \ = cht.get(x) + j * x - TS[j];\r\n    }\r\n  }\r\n  cout << dp[n] << endl;\r\n\
    }\n"
  code: "#define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/2603\"\r\n#include\
    \ \"../../include/mtl/convex_hull_trick.hpp\"\r\n#include <bits/stdc++.h>\r\n\
    using namespace std;\r\n\r\nint main() {\r\n  int s,n,m; cin>>s>>n>>m;\r\n  vector<int>\
    \ X(s);\r\n  for (int i = 0; i < s; i++) cin>>X[i];\r\n  vector<pair<int,int>>\
    \ B(n);\r\n  vector<int> T(n);\r\n  for (int i = 0; i < n; i++) {\r\n    int t,p;\
    \ cin>>t>>p; p--;\r\n    B[i] = {t, X[p]};\r\n    T[i] = t-X[p];\r\n  }\r\n  sort(T.begin(),\
    \ T.end());\r\n  vector<int> TS(n+1);\r\n  for (int i = 1; i <= n; i++)\r\n  \
    \  TS[i] = TS[i-1] + T[i-1];\r\n  // dp[r] = min_{0<=l<=r} dp[l] + (r-l)*T[r-1]\
    \ - (TS[r]-TS[l])\r\n  //       = min           dp[l] - (l*T[r-1] - TS[l]) + r*T[r-1]\
    \ - TS[r]\r\n  constexpr int INF = 1e9;\r\n  vector<int> dp(n+1, INF);\r\n  dp[0]\
    \ = 0;\r\n  ConvexHullTrickDeque<int, greater<>> cht;\r\n  for (int i = 0; i <\
    \ m; i++) {\r\n    cht.clear();\r\n    cht.push_back(-0, dp[0] + TS[0]);\r\n \
    \   for (int j = 1; j <= n; j++) {\r\n      auto x = T[j-1];\r\n      cht.push_back(-j,\
    \ dp[j] + TS[j]);\r\n      dp[j] = cht.get(x) + j * x - TS[j];\r\n    }\r\n  }\r\
    \n  cout << dp[n] << endl;\r\n}"
  dependsOn:
  - include/mtl/convex_hull_trick.hpp
  - include/mtl/search.hpp
  isVerificationFile: true
  path: test/aoj/aoj-time_table-cht.test.cpp
  requiredBy: []
  timestamp: '2023-04-08 02:15:04+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/aoj/aoj-time_table-cht.test.cpp
layout: document
redirect_from:
- /verify/test/aoj/aoj-time_table-cht.test.cpp
- /verify/test/aoj/aoj-time_table-cht.test.cpp.html
title: test/aoj/aoj-time_table-cht.test.cpp
---
