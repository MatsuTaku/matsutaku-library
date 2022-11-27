#pragma once
#include <utility>
#include <cassert>
#include <tuple>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <limits>
#include "search.hpp"

template<typename T, typename C = std::less<T>>
struct ConvexHullTrickDeque {
  std::deque<std::pair<T, T>> L;
  C cmp;
  ConvexHullTrickDeque() = default;

 private:
  template<typename U, typename V, typename W>
  bool intersect_less(const U& g, const V& h, const W& i) {
    assert(g.first != h.first and h.first != i.first);
    // x0 = (d-b)/(a-c)
    // x1 = (f-d)/(c-e)
    // x0 < x1 <=> (d-b)/(a-c) < (f-d)/(c-e)
    //         <=> (d-b)(c-e) < (f-d)(a-c)
    return (h.second-g.second)*(h.first-i.first) <
        (i.second-h.second)*(g.first-h.first);
  }

 public:
  void clear() {
    L.clear();
  }
  void push_back(T a, T b) {
    assert(L.empty() or cmp(L.back().first, a));
    auto l = std::make_pair(a, b);
    while (L.size() >= 2 and !intersect_less(L[L.size()-2], L[L.size()-1], l))
      L.pop_back();
    L.push_back(l);
  }
  void push_front(T a, T b) {
    assert(L.empty() or cmp(a, L.front().first));
    auto l = std::make_pair(a, b);
    while (L.size() >= 2 and !intersect_less(l, L[0], L[1]))
      L.pop_front();
    L.push_front(l);
  }

  T get(T x) const {
    auto i = bisect_int(-1, (int)L.size()-1, [&](int c) {
      if (c == -1) return true;
      auto yl = L[c].first * x + L[c].second;
      auto yr = L[c+1].first * x + L[c+1].second;
      return cmp(yl, yr);
    });
    int r = i+1;
    return L[r].first * x + L[r].second;
  }

  T get_incremental(T x) {
    assert(!L.empty());
    T ret = L[0].first * x + L[0].second;
    while (L.size() >= 2 and cmp(ret, L[1].first * x + L[1].second)) {
      ret = L[1].first * x + L[1].second;
      L.pop_front();
    }
    return ret;
  }

  T get_decremental(T x) {
    assert(!L.empty());
    T ret = L.back().first * x + L.back().second;
    while (L.size() >= 2 and cmp(ret, L[L.size()-2].first * x + L[L.size()-2].second)) {
      ret = L[L.size()-2].first * x + L[L.size()-2].second;
      L.pop_back();
    }
    return ret;
  }
};


template<typename T, typename C = std::less<T>>
struct ConvexHullTrick {
  using Line = std::pair<T, T>;
  struct Node;
  std::map<Node, Line> tr;
  std::map<T, T, C> L;
  C cmp;
  static constexpr T INF = std::numeric_limits<T>::max();
  static constexpr T MINF = std::numeric_limits<T>::min();
  static constexpr T AX_LO = C()(MINF, INF) ? MINF : INF;
  static constexpr T AX_HI = C()(MINF, INF) ? INF : MINF;
  ConvexHullTrick() {
    Line lo = {AX_LO, 0};
    Line hi = {AX_HI, 0};
    tr.emplace(Node(lo, hi), lo);
    L.insert(lo);
    L.emplace_hint(L.end(), hi);
  }

  struct Node {
    T u,v; // a-c, d-b
    Node() = default;
    Node(T _u, T _v) : u(_u), v(_v) {}
    explicit Node(const Line& f, const Line& g) {
      if (f.first == AX_LO) {
        u = 0, v = MINF;
      } else if (g.first == AX_HI) {
        u = 0, v = INF;
      } else {
        u = f.first - g.first, v = g.second - f.second;
        if (u < 0) {
          u = -u;
          v = -v;
        }
      }
    }
    inline bool operator==(const Node& rhs) const {
      return u == rhs.u and v == rhs.v;
    }
    inline bool operator<(const Node& rhs) const {
      if (v == INF or rhs.v == MINF)
        return false;
      if (v == MINF or rhs.v == INF)
        return true;
      // x0 = (d-b)/(a-c)
      // x1 = (f-d)/(c-e)
      // x0 < x1 <=> (d-b)/(a-c) < (f-d)/(c-e)
      //         <=> (d-b)(c-e) < (f-d)(a-c)
      assert((double)MINF < (double) v*rhs.u and (double) v*rhs.u < (double)INF);
      assert((double)MINF < (double) rhs.v * u and (double) rhs.v * u < (double)INF);
      return v * rhs.u < rhs.v * u;
    }
  };

 private:
  template<typename U, typename V, typename W>
  bool intersect_less(const U& g, const V& h, const W& i) {
    if (g.first == h.first)
      return cmp(g.second, h.second);
    if (h.first == i.first)
      return cmp(i.second, h.second);
    if (g.first == AX_LO or i.first == AX_HI)
      return true;
    // x0 = (d-b)/(a-c)
    // x1 = (f-d)/(c-e)
    // x0 < x1 <=> (d-b)/(a-c) < (f-d)/(c-e)
    //         <=> (d-b)(c-e) < (f-d)(a-c)
    auto s = (g.first-h.first), t = (h.second-g.second);
    auto u = (h.first-i.first), v = (i.second-h.second);
    assert((double)MINF < (double)t*u and (double)t*u < (double)INF);
    assert((double)MINF < (double)t*u and (double)v*s < (double)INF);
    return t * u < v * s;
  }

 public:
  void add(T a, T b) {
    Line line{a,b};
    auto it = L.lower_bound(a);
    assert(it != L.begin());
    assert(it != L.end());
    using std::prev;
    using std::next;
    auto lit = prev(it);
    if (it->first == a) {
      if (!cmp(it->second, b))
        return;
      lit = it++;
    } else if (!intersect_less(*lit, line, *it)) {
      return;
    }
    auto tit = tr.find(Node(*lit, *it));
    assert(tit != tr.end());
    auto teit = next(tit);
    while (lit != L.begin() and !intersect_less(*prev(lit), *lit, line)) {
      --tit;
      --lit;
    }
    while (next(it) != L.end() and !intersect_less(line, *it, *next(it))) {
      ++teit;
      ++it;
    }
    tit = tr.erase(tit, teit);
    it = L.erase(next(lit), it);
    assert(tit == tr.end() or Node(line,*it) < tit->first);
    tit = tr.emplace_hint(tit, Node(line, *it), line);
    assert(tit != tr.end() and Node(*lit,line) < tit->first);
    tr.emplace_hint(tit, Node(*lit, line), *lit);
    L.emplace_hint(it, line);
  }

 private:
  inline T f(const Line& l, T x) const {
    return l.first * x + l.second;
  }

 public:
  T get(T x) const {
    auto it = tr.lower_bound(Node(1,x));
    assert(it != tr.end());
    return f(it->second, x);
  }

  T get_incremental(T x) {
    auto ts = tr.begin();
    auto te = ts;
    auto s = next(L.begin());
    auto e = s;
    auto ne = next(e);
    auto le = prev(L.end());
    while (ne != le and cmp(f(*e, x), f(*ne, x))) {
      ++te;
      e = ne++;
    }
    if (ts != te) {
      ++te;
      ts = tr.erase(ts, te);
      s = L.erase(s, e);
      tr.emplace_hint(ts, Node(*prev(s), *s), *prev(s));
    }
    return f(*s, x);
  }

  T get_decremental(T x) {
    auto ts = tr.end();
    auto te = ts;
    auto s = prev(prev(L.end()));
    auto ps = prev(s);
    auto e = s;
    while (ps != L.begin() and cmp(f(*s, x), f(*ps, x))) {
      --ts;
      s = ps--;
    }
    if (ts != te) {
      --ts;
      ts = tr.erase(ts, te);
      L.erase(next(s), next(e));
      tr.emplace_hint(ts, Node(*s, *next(s)), *s);
    }
    return f(*s, x);
  }
};
