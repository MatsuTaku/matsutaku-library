---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/fibonacci_heap.hpp
    title: include/mtl/fibonacci_heap.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/min_cost_flow.hpp
    title: include/mtl/min_cost_flow.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_B
    links:
    - https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_B
  bundledCode: "#line 1 \"test/aoj/min_cost_flow.test.cpp\"\n#define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_B\"\
    \r\n#line 2 \"include/mtl/min_cost_flow.hpp\"\n#include <vector>\r\n#include <limits>\r\
    \n#line 2 \"include/mtl/fibonacci_heap.hpp\"\n#include <memory>\n#include <cassert>\n\
    #line 5 \"include/mtl/fibonacci_heap.hpp\"\n#include <array>\n#include <list>\n\
    #include <iostream>\n\ntemplate<typename T, typename Cond = std::less<>>\nclass\
    \ FibonacciHeap {\n  Cond cond_;\n\n public:\n  struct Node;\n  using node_ptr\
    \ = Node*;\n  using const_node_ptr = const Node*;\n  struct Node {\n    node_ptr\
    \ next = nullptr;\n    node_ptr prev = nullptr;\n    node_ptr child = nullptr;\n\
    \    node_ptr parent = nullptr;\n    int deg = 0;\n    bool marked = false;\n\
    \    bool enabled = false;\n    std::pair<int, T> value;\n\n    Node() = default;\n\
    \    void init(int k, T v) {\n      next = prev = this;\n      child = parent\
    \ = nullptr;\n      deg = 0;\n      marked = false;\n      enabled = true;\n \
    \     value = {k, v};\n    }\n    T priority() const { return value.second; }\n\
    \    void free() {\n      enabled = false;\n    }\n  };\n\n private:\n  node_ptr\
    \ root_ = nullptr;\n  size_t sz_ = 0;\n  std::vector<node_ptr> map_;\n  std::vector<Node>\
    \ container_;\n  std::vector<node_ptr> deg_table_;\n\n public:\n  explicit FibonacciHeap(size_t\
    \ size) : map_(size) {\n    container_.reserve(size);\n    std::array<size_t,2>\
    \ tb{1,1};\n    int k = 2;\n    while (tb[1] < size) {\n      auto x = tb[0]+tb[1];\n\
    \      tb[0] = tb[1];\n      tb[1] = x;\n      ++k;\n    }\n    deg_table_.resize(k);\n\
    \  }\n\n  inline std::pair<int, T> top() const {\n    assert(root_ and root_->enabled);\n\
    \    return root_->value;\n  }\n\n  inline std::pair<int, T> get(int key) const\
    \ {\n    assert(map_[key] and map_[key]->enabled);\n    return map_[key]->value;\n\
    \  }\n\n  // Time complexity: O(1)\n  void push(int key, T value) {\n    if (map_[key]\
    \ and map_[key]->enabled) {\n      update(key, value);\n      return;\n    }\n\
    \    if (!map_[key]) {\n      container_.emplace_back();\n      map_[key] = &container_.back();\n\
    \    }\n    node_ptr node = map_[key];\n    node->init(key, value);\n    _PushLink(node,\
    \ &deg_table_[0]);\n    if (!root_ or _CompareNodePriority(root_, node))\n   \
    \   root_ = node;\n    ++sz_;\n  }\n\n  // Time complexity: O(log n)\n  void update(int\
    \ key, T updated_value) {\n    if (!map_[key] or !map_[key]->enabled) {\n    \
    \  push(key, updated_value);\n      return;\n    }\n    auto node = map_[key];\n\
    \    if (!cond_(node->priority(), updated_value)) {\n      return;\n    }\n  \
    \  node->value.second = updated_value;\n    if (node->parent and _CompareNodePriority(node->parent,\
    \ node)) {\n      _Consolidate(node);\n    }\n    assert(root_);\n    if (_CompareNodePriority(root_,\
    \ node)) {\n      root_ = node;\n    }\n  }\n\n  // Time complexity: O(log n)\n\
    \  void pop() {\n    assert(root_);\n    auto r = root_;\n    root_ = nullptr;\n\
    \    _PopLink(r, &deg_table_[r->deg]);\n    _ExpandChildren(r);\n    _ExtractTop();\n\
    \    r->free();\n    --sz_;\n  }\n\n  // Time complexity: O(log n)\n  void erase(int\
    \ key) {\n    assert(map_[key] and map_[key]->enabled);\n    auto node = map_[key];\n\
    \    bool needs_extraction = root_ == node;\n    if (node->parent) {\n      _Consolidate(node);\n\
    \    }\n    assert(!node->parent);\n    _PopLink(node, &deg_table_[node->deg]);\n\
    \    _ExpandChildren(node);\n    if (needs_extraction)\n      _ExtractTop();\n\
    \    node->free();\n    --sz_;\n  }\n\n  size_t size() const { return sz_; }\n\
    \  bool empty() const { return size() == 0; }\n  void clear() {\n    root_ = nullptr;\n\
    \    sz_ = 0;\n    map_.fill(nullptr);\n    container_.clear();\n  }\n\n  void\
    \ print_for_debug() const {\n      for (int k = 0; k < (int)deg_table_.size();\
    \ k++) {\n        std::cout<<\"d=\"<<k<<std::endl;\n        if (deg_table_[k])\
    \ {\n          std::cout<<\"forward\"<<std::endl;\n          auto r = deg_table_[k];\n\
    \          do {\n            std::cout << r->priority() << ' ' << std::flush;\n\
    \            r = r->next;\n          } while (r != deg_table_[k]);\n         \
    \ std::cout<<std::endl;\n          std::cout<<\"deg\"<<std::endl;\n          r\
    \ = deg_table_[k];\n          do {\n            std::cout<<r->deg<<' '<<std::flush;\n\
    \            r = r->prev;\n          } while (r != deg_table_[k]);\n         \
    \ std::cout<<std::endl;\n        } else {\n          std::cout<<\"empty\"<<std::endl;\n\
    \        }\n      }\n  }\n\n private:\n  bool _CompareNodePriority(const_node_ptr\
    \ l, const_node_ptr r) const {\n    return cond_(l->priority(), r->priority());\n\
    \  }\n\n  void _PushLink(node_ptr node, node_ptr* link_head) {\n    if (!*link_head)\
    \ {\n      *link_head = node;\n      node->next = node;\n      node->prev = node;\n\
    \    } else {\n      auto b = (*link_head)->prev;\n      node->next = *link_head;\n\
    \      node->prev = b;\n      b->next = node;\n      (*link_head)->prev = node;\n\
    \    }\n  }\n\n  void _PopLink(node_ptr node, node_ptr* link_head) {\n    assert(*link_head);\n\
    \    if (node->next == node) {\n      assert(node->prev == node);\n      assert(*link_head\
    \ == node);\n      *link_head = nullptr;\n    } else {\n      auto n = node->next;\n\
    \      auto p = node->prev;\n      p->next = n;\n      n->prev = p;\n      if\
    \ (*link_head == node)\n        *link_head = n;\n      node->next = node;\n  \
    \    node->prev = node;\n    }\n  }\n\n  node_ptr _Meld(node_ptr l, node_ptr r)\
    \ {\n    assert(l->deg == r->deg);\n    if (_CompareNodePriority(l, r)) {\n  \
    \    std::swap(l, r);\n    }\n    _PushLink(r, &l->child);\n    r->parent = l;\n\
    \    l->deg++;\n    return l;\n  }\n\n  void _ExpandChildren(node_ptr node) {\n\
    \    while (node->child) {\n      auto c = node->child;\n      _PopLink(c, &node->child);\n\
    \      --node->deg;\n      c->parent = nullptr;\n      c->marked = false;\n  \
    \    _PushLink(c, &deg_table_[c->deg]);\n    }\n    assert(node->deg == 0);\n\
    \  }\n\n  void _ExtractTop() {\n    int k = 0;\n    while (k < (int)deg_table_.size())\
    \ {\n      while (k < (int)deg_table_.size() and !deg_table_[k]) ++k;\n      if\
    \ (k == (int)deg_table_.size())\n        break;\n      if (deg_table_[k]->next\
    \ != deg_table_[k]) { // Multiple trees at current degree\n        auto node =\
    \ deg_table_[k];\n        _PopLink(node, &deg_table_[k]);\n        assert(node->deg\
    \ == k);\n        while (deg_table_[node->deg]) {\n          auto d = node->deg;\n\
    \          auto u = deg_table_[d];\n          _PopLink(u, &deg_table_[d]);\n \
    \         node = _Meld(node, u);\n          assert(node->deg == d+1);\n      \
    \  }\n        _PushLink(node, &deg_table_[node->deg]);\n      } else { // Single\
    \ tree at current degree\n        if (!root_ or _CompareNodePriority(root_, deg_table_[k]))\n\
    \          root_ = deg_table_[k];\n        ++k;\n      }\n    }\n  }\n\n  void\
    \ _Consolidate(node_ptr node) {\n    assert(node->parent);\n    auto p = node->parent;\n\
    \    if (p->marked) {\n      assert(p->parent); // Parent is not one of the roots.\n\
    \      _Consolidate(p);\n    }\n    assert(!p->marked);\n    if (p->parent) {\n\
    \      p->marked = true;\n    } else {\n      _PopLink(p, &deg_table_[p->deg]);\n\
    \      _PushLink(p, &deg_table_[p->deg-1]);\n    }\n    _PopLink(node, &p->child);\n\
    \    p->deg--;\n    node->parent = nullptr;\n    node->marked = false;\n    _PushLink(node,\
    \ &deg_table_[node->deg]);\n  }\n\n};\n#line 5 \"include/mtl/min_cost_flow.hpp\"\
    \n\r\ntemplate<typename C>\r\nstruct MinCostFlowPath {\r\n  int t, rev, cap;\r\
    \n  C cost;\r\n  MinCostFlowPath(int t, int rev, int cap, C cost)\r\n      : t(t),\
    \ rev(rev), cap(cap), cost(cost) {}\r\n};\r\n\r\ntemplate<typename C>\r\nstruct\
    \ MinCostFlowGraph {\r\n  const C INF = std::numeric_limits<C>::max();\r\n  int\
    \ n;\r\n  std::vector<std::vector<MinCostFlowPath<C>>> g;\r\n  std::vector<C>\
    \ dist, h;\r\n  std::vector<int> prevv, preve;\r\n  explicit MinCostFlowGraph(int\
    \ n)\r\n      : n(n), g(n), dist(n), h(n), prevv(n), preve(n) {}\r\n\r\n  void\
    \ add_edge(int a, int b, int cap, C cost) {\r\n    g[a].emplace_back(b, g[b].size(),\
    \ cap, cost);\r\n    g[b].emplace_back(a, g[a].size()-1, 0, -cost);\r\n  }\r\n\
    \r\n  std::pair<int, C> flow(int s, int t, int flow_limit = std::numeric_limits<int>::max())\
    \ {\r\n    h.assign(n, 0);\r\n    auto f = 0;\r\n    C res = 0;\r\n    FibonacciHeap<C,\
    \ std::greater<C>> qs(n);\r\n    while (f < flow_limit) {\r\n      dist.assign(n,\
    \ INF);\r\n      dist[s] = 0;\r\n      qs.push(s, 0);\r\n      while (!qs.empty())\
    \ {\r\n        auto q = qs.top(); qs.pop();\r\n        auto u = q.first;\r\n \
    \       auto d = q.second;\r\n        for (size_t i = 0; i < g[u].size(); i++)\
    \ {\r\n          auto& e = g[u][i];\r\n          if (e.cap == 0) continue;\r\n\
    \          auto cost = d + e.cost + h[u] - h[e.t];\r\n          if (cost >= dist[e.t])\
    \ continue;\r\n          dist[e.t] = cost;\r\n          qs.push(e.t, cost);\r\n\
    \          prevv[e.t] = u;\r\n          preve[e.t] = i;\r\n        }\r\n     \
    \ }\r\n      if (dist[t] == INF)\r\n        break;\r\n      for (int i = 0; i\
    \ < n; i++) if (dist[i] != INF)\r\n        h[i] += dist[i];\r\n      int d = flow_limit\
    \ - f;\r\n      C cost_sum = 0;\r\n      for (int i = t; i != s; i = prevv[i])\
    \ {\r\n        auto& e = g[prevv[i]][preve[i]];\r\n        d = std::min(d, e.cap);\r\
    \n        cost_sum += e.cost;\r\n      }\r\n      f += d;\r\n      res += (C)\
    \ d * cost_sum;\r\n      for (int i = t; i != s; i = prevv[i]) {\r\n        auto&\
    \ e = g[prevv[i]][preve[i]];\r\n        e.cap -= d;\r\n        g[i][e.rev].cap\
    \ += d;\r\n      }\r\n    }\r\n    return std::make_pair(f, res);\r\n  }\r\n};\n\
    #line 3 \"test/aoj/min_cost_flow.test.cpp\"\n#include <bits/stdc++.h>\r\nusing\
    \ namespace std;\r\n\r\nint main() {\r\n  int V,E,F; cin>>V>>E>>F;\r\n  MinCostFlowGraph<int>\
    \ mcfg(V);\r\n  for (int i = 0; i < E; i++) {\r\n    int u,v,c,d; cin>>u>>v>>c>>d;\r\
    \n    mcfg.add_edge(u, v, c, d);\r\n  }\r\n  auto res = mcfg.flow(0, V-1, F);\r\
    \n  cout << (res.first == F ? res.second : -1) << endl;\r\n}\r\n"
  code: "#define PROBLEM \"https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_B\"\r\n\
    #include \"../../include/mtl/min_cost_flow.hpp\"\r\n#include <bits/stdc++.h>\r\
    \nusing namespace std;\r\n\r\nint main() {\r\n  int V,E,F; cin>>V>>E>>F;\r\n \
    \ MinCostFlowGraph<int> mcfg(V);\r\n  for (int i = 0; i < E; i++) {\r\n    int\
    \ u,v,c,d; cin>>u>>v>>c>>d;\r\n    mcfg.add_edge(u, v, c, d);\r\n  }\r\n  auto\
    \ res = mcfg.flow(0, V-1, F);\r\n  cout << (res.first == F ? res.second : -1)\
    \ << endl;\r\n}\r\n"
  dependsOn:
  - include/mtl/min_cost_flow.hpp
  - include/mtl/fibonacci_heap.hpp
  isVerificationFile: true
  path: test/aoj/min_cost_flow.test.cpp
  requiredBy: []
  timestamp: '2023-04-08 02:15:04+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/aoj/min_cost_flow.test.cpp
layout: document
redirect_from:
- /verify/test/aoj/min_cost_flow.test.cpp
- /verify/test/aoj/min_cost_flow.test.cpp.html
title: test/aoj/min_cost_flow.test.cpp
---
