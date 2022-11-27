#pragma once
#include <utility>
#include <vector>
#include <limits>
#include <numeric>
#include <functional>
#include <cstring>
#include <iostream>
#include "bit_manip.hpp"

template<typename T, typename Comp = std::less<T>>
struct LiChaoTree {
  using Line = std::pair<T, T>;
  static constexpr T INF = std::numeric_limits<T>::max();
  static constexpr T MINF = std::numeric_limits<T>::min();
  static constexpr T INV = Comp()(MINF, INF) ? MINF : INF;
  size_t n, N;
  std::vector<T> X;
  std::vector<Line> seg;
  Comp cmp;

  LiChaoTree() = default;
  explicit LiChaoTree(size_t _n) : n(_n), N(_n ? 1ull<<(64-bm::clz(_n-1)) : 0), X(n), seg(N*2, {0, INV}) {
    std::iota(X.begin(), X.begin()+n, 0ull);
  }
  void set_x(size_t i, T x) {
    X[i] = x;
  }
  template<typename It>
  LiChaoTree(It begin, It end) : LiChaoTree(std::distance(begin, end)) {
    std::copy(begin, end, X.begin());
  }

 private:
  inline static T f(const Line& line, T x) {
    return line.first * x + line.second;
  }

  void _add_line(Line line, size_t u, size_t l, size_t r) {
    if (line.second == INV)
      return;
    auto mid = l + (r-l)/2;
    bool enabled_left = l < n and cmp(f(seg[u], X[l]), f(line, X[l]));
    bool enabled_mid = mid < n and cmp(f(seg[u], X[mid]), f(line, X[mid]));
    if (enabled_mid)
      std::swap(line, seg[u]);
    if (r-l == 1)
      return;
    if (enabled_left != enabled_mid)
      _add_line(line, u<<1, l, mid);
    else
      _add_line(line, (u<<1)|1u, mid, r);
  }
 public:
  void add_line(T a, T b) {
    _add_line({a, b}, 1, 0, N);
  }

  void add_segment_idx(T a, T b, size_t l, size_t r) {
    auto u = l+N, v = r+N;
    auto L = l, R = r;
    size_t range = 1;
    Line line{a,b};
    while (u < v) {
      if (u&1u) {
        _add_line(line, u++, L, L+range);
        L += range;
      }
      if (v&1u) {
        R -= range;
        _add_line(line, --v, R, R+range);
      }
      u >>= 1;
      v >>= 1;
      range <<= 1;
    }
    assert(L == R);
  }
  void add_segment(T a, T b, T xl, T xr) {
    assert(xl <= xr);
    auto xlit = std::lower_bound(X.begin(), X.end(), xl);
    auto xrit = std::lower_bound(xlit, X.end(), xr);
    add_segment_idx(a, b, xlit-X.begin(), xrit-X.begin());
  }

  T get_idx(T i) const {
    size_t u = i + N;
    T x = X[i];
    T ret = INV;
    while (u > 0) {
      auto v = f(seg[u], x);
      if (cmp(ret, v)) ret = v;
      u /= 2;
    }
    return ret;
  }
  T get(T x) const {
    auto it = std::lower_bound(X.begin(), X.end(), x);
    assert(it != X.end());
    return get_idx(it - X.begin());
  }
};
template<typename T, typename Comp>
constexpr T LiChaoTree<T, Comp>::INF;
template<typename T, typename Comp>
constexpr T LiChaoTree<T, Comp>::MINF;
template<typename T, typename Comp>
constexpr T LiChaoTree<T, Comp>::INV;
