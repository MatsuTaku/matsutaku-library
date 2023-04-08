#pragma once
#include "fenwick_tree.hpp"
#include "succinct/wavelet_matrix.hpp"
#include "succinct/bit_vector.hpp"
#include "fenwick_tree.hpp"
#include <cstddef>
#include <limits>
#include <vector>

/**
 * @brief Ordinal Range Search
 *   Maintain 2-dimential points (x,y) and their weights.
*/
template<typename T, typename W = size_t, 
  T IndexMax = std::numeric_limits<T>::max(), 
  class BVec = BitVector // TODO: optimize size of succinct BV which size of B(n,u)+O(n) bit
  >
struct ORS {
  std::vector<std::tuple<T,T,W>> ps;
  WaveletMatrix<int> wm;
  FenwickTree<W> rsq;
  using bit_vector_type = BVec;
  bit_vector_type X, Y;
  T value_of_ith_x(size_t i) const {
    return X.template select<1>(i) - i;
  }
  size_t index_of_lower_bound_x(const T& x) const {
    return x==0 ? 0 : (X.template select<0>(x-1) - (x - 1));
  }
  std::pair<size_t, size_t> range_on_wm_lower_bound_x(const T& x) const {
    auto l = x==0 ? 0 : (X.template select<0>(x-1) - (x - 1));
    auto lbx = value_of_ith_x(l);
    auto r = X.template select<0>(lbx) - lbx;
    return std::make_pair(l, r);
  }
  T value_of_ith_y(size_t i) const {
    return Y.template select<1>(i);
  }
  size_t index_of_lower_bound_y(const T& y) const {
    return Y.rank(y);
  }

  ORS() = default;

  void add(T x, T y, W w = 1) {
    ps.emplace_back(x,y,w);
  }
  void build() {
    std::sort(ps.begin(), ps.end(),
              [](auto l, auto r) {
      return std::get<0>(l) != std::get<0>(r) ? std::get<0>(l) < std::get<0>(r)
                                : std::get<1>(l) < std::get<1>(r);
    });

    {
      std::vector<T> xs(ps.size()+1), ys(ps.size()+1);
      for (size_t i = 0; i < ps.size(); i++) {
        xs[i] = std::get<0>(ps[i]);
        ys[i] = std::get<1>(ps[i]);
      }
      xs.back() = IndexMax;
      X = bit_vector_type((size_t)IndexMax + xs.size());
      for (size_t i = 0; i < xs.size(); i++)
        X[xs[i] + i] = 1;
      X.build();

      ys.back() = IndexMax;
      std::sort(ys.begin(), ys.end());
      ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
      Y = bit_vector_type((size_t)IndexMax + 1);
      for (auto y:ys) Y[y] = 1;
      Y.build();
    }

    {
      std::vector<int> S(ps.size());
      std::vector<std::pair<int, W>> SW(ps.size()), swz, swo;
      for (size_t i = 0; i < ps.size(); i++) {
        S[i] = index_of_lower_bound_y(std::get<1>(ps[i]));
        SW[i] = {S[i], std::get<2>(ps[i])};
      }
      wm = WaveletMatrix<int>(S.begin(), S.end());
      rsq = FenwickTree<W>(wm.n * (wm.h+1));
      for (int k = wm.h-1; k >= 0; k--) {
        for (size_t i = 0; i < wm.n; i++) {
          if (((SW[i].first >> k) & 1u) == 0)
            swz.push_back(SW[i]);
          else
            swo.push_back(SW[i]);
          rsq.add(wm.n * (wm.h-1-k) + i, SW[i].second);
        }
        size_t j = wm.n-1;
        while (!swo.empty()) {
          SW[j--] = swo.back();
          swo.pop_back();
        }
        while (!swz.empty()) {
          SW[j--] = swz.back();
          swz.pop_back();
        }
      }
      for (size_t i = 0; i < wm.n; i++) {
        rsq.add(wm.n * wm.h + i, SW[i].second);
      }
    }
  }

 private:
  std::tuple<int, int, int, int>
  compress_idx(T xl, T xh, T yl, T yh) const {
    return std::make_tuple(
      index_of_lower_bound_x(xl),
      index_of_lower_bound_x(xh),
      index_of_lower_bound_y(yl),
      index_of_lower_bound_y(yh)
    );
  }

  template<typename F, typename G>
  void _traverse(T xl, T xh, T yl, T yh, F internal_fn, G leaf_fn) const {
    size_t i,j;
    int a,b;
    std::tie(i,j,a,b) = compress_idx(xl, xh, yl, yh);
    std::queue<std::tuple<size_t,size_t,T>> qs;
    qs.emplace(i,j,T(0));
    while (!qs.empty()) {
      size_t l,r;
      T c;
      std::tie(l,r,c) = qs.front(); qs.pop();
      size_t level = l/wm.n;
      int shift = wm.h-1-level;
      T clo = c, chi = c | ((1ull<<(shift+1))-1);
      if (chi < a or b <= clo)
        continue;
      if (a <= clo and chi < b) {
        internal_fn(l, r, c);
        continue;
      }
      if (level == wm.h) {
        leaf_fn(l, r, c);
        continue;
      }
      size_t nl,nr;
      std::tie(nl,nr) = wm._child_tie0(level, l, r);
      qs.emplace(nl, nr, c);
      std::tie(nl,nr) = wm._child_tie1(level, l, r);
      qs.emplace(nl, nr, c | (1ull<<shift));
    }
  }

 public:
  size_t freq(T xl, T xh, T yl, T yh) const {
    size_t i,j;
    int a,b;
    std::tie(i,j,a,b) = compress_idx(xl, xh, yl, yh);
    return wm.range_freq(i, j, a, b);
  }

  void weight_add(T x, T y, W w) {
    size_t l, r;
    std::tie(l, r) = range_on_wm_lower_bound_x(x);
    auto c = index_of_lower_bound_y(y);
    for (int k = wm.h-1; k >= 0; k--)
      std::tie(l,r) = wm.child_tie(wm.h-1-k, l, r, (c >> k) & 1u);
    assert(l != r);
    size_t j = l;
    for (size_t k = 0; k < wm.h; k++) {
      rsq.add(j, w);
      j = wm.parent(wm.h-1-k, j, (c >> k) & 1u);
    }
    rsq.add(j, w);
  }

  W sum(T xl, T xh, T yl, T yh) const {
    W ret = 0;
    _traverse(xl, xh, yl, yh,
              [&](size_t l, size_t r, auto _) { ret += rsq.range_sum(l, r); },
              [](auto,auto,auto){});
    return ret;
  }

  std::vector<std::pair<size_t, size_t>>
  enumerate(T xl, T xh, T yl, T yh) const {
    std::vector<std::pair<size_t,size_t>> ret;
    _traverse(xl, xh, yl, yh,
              [](auto,auto,auto){},
              [&](size_t l, size_t r, T c) {
      for (size_t idx = l; idx < r; idx++) {
        auto jdx = idx;
        for (size_t k = 0; k < wm.h; k++)
          jdx = wm.parent(wm.h-1-k, jdx, (c >> k) & 1u);
        ret.emplace_back(value_of_ith_x(jdx), value_of_ith_y(c));
      }
    });
    return ret;
  }
};