#pragma once
#include "fenwick_tree.hpp"
#include "succinct/wavelet_matrix.hpp"
#include "succinct/bit_vector.hpp"
#include "succinct/rrr.hpp"
#include "succinct/binary_set.hpp"
#include <cstddef>
#include <limits>
#include <vector>

/**
 * @brief Ordinal Range Search
 *   Maintain 2-dimential points (x,y) and their weights.
*/
template<typename T, typename W = long long,
  class BitmapType = RRR<>
  >
struct ORS {
  using index_type = T;
  using weight_type = W;
  static constexpr index_type IndexLimit = std::numeric_limits<index_type>::max();
  std::vector<std::tuple<index_type,index_type,weight_type>> ps;
  using x_index_multiset = BinaryMultiset<BitmapType>;
  using y_index_set = BinarySet<BitmapType>;
  x_index_multiset X;
  y_index_set Y;
  using wm_type = WaveletMatrix<int>;
  wm_type wm;
  using range_sum_type = FenwickTree<W>;
  range_sum_type rsq;
  index_type value_of_ith_x(size_t i) const {
    return X.select(i);
  }
  size_t index_of_lower_bound_x(const T& x) const {
    return X.rank(x);
  }
  std::pair<size_t, size_t> range_on_wm_lower_bound_x(const T& x) const {
    auto l = index_of_lower_bound_x(x);
    auto lb = value_of_ith_x(l);
    auto c = X.count(lb);
    return std::make_pair(l, l+c);
  }
  index_type value_of_ith_y(size_t i) const {
    return Y.select(i);
  }
  size_t index_of_lower_bound_y(const T& y) const {
    return Y.rank(y);
  }

  ORS() = default;

  void add(index_type x, index_type y, W w = 1) {
    ps.emplace_back(x,y,w);
  }
  void build(index_type max_index = IndexLimit) {
    std::sort(ps.begin(), ps.end(),
              [](auto l, auto r) {
      return std::get<0>(l) != std::get<0>(r) ? std::get<0>(l) < std::get<0>(r)
                                              : std::get<1>(l) < std::get<1>(r);
    });

    for (size_t i = 0; i < ps.size(); i++) {
      auto x = std::get<0>(ps[i]);
      auto y = std::get<1>(ps[i]);
      X.insert(x);
      Y.insert(y);
    }
    X.insert(max_index);
    Y.insert(max_index);
    X.build();
    Y.build();

    {
      std::vector<int> S(ps.size());
      std::vector<std::pair<int, W>> SW(ps.size()), swz, swo;
      for (size_t i = 0; i < ps.size(); i++) {
        auto y = std::get<1>(ps[i]);
        auto w = std::get<2>(ps[i]);
        S[i] = index_of_lower_bound_y(y);
        SW[i] = {S[i], w};
      }
      ps.clear();
      ps.shrink_to_fit();
      wm = wm_type(S.begin(), S.end());
      rsq = range_sum_type(wm.n * (wm.h+1));
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
  compress_idx(const index_type& xl, 
               const index_type& xh, 
               const index_type& yl, 
               const index_type& yh) const {
    return std::make_tuple(
      index_of_lower_bound_x(xl),
      index_of_lower_bound_x(xh),
      index_of_lower_bound_y(yl),
      index_of_lower_bound_y(yh)
    );
  }

  template<typename F, typename G>
  void _traverse(const index_type& xl, 
                 const index_type& xh, 
                 const index_type& yl, 
                 const index_type& yh, 
                 F internal_fn, 
                 G leaf_fn) const {
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
  size_t freq(const index_type& xl, 
              const index_type& xh, 
              const index_type& yl, 
              const index_type& yh) const {
    size_t i,j;
    int a,b;
    std::tie(i,j,a,b) = compress_idx(xl, xh, yl, yh);
    return wm.range_freq(i, j, a, b);
  }

  void weight_add(const index_type& x, const index_type& y, const weight_type& w) {
    size_t l, r;
    std::tie(l,r) = range_on_wm_lower_bound_x(x);
    auto c = index_of_lower_bound_y(y);
    for (int k = wm.h-1; k >= 0; k--) {
      std::tie(l,r) = wm.child_tie(wm.h-1-k, l, r, (c >> k) & 1u);
    }
    assert(l != r);
    size_t j = l;
    for (size_t k = 0; k < wm.h; k++) {
      rsq.add(j, w);
      j = wm.parent(wm.h-1-k, j, (c >> k) & 1u);
    }
    rsq.add(j, w);
  }

  W sum(const index_type& xl, 
        const index_type& xh, 
        const index_type& yl, 
        const index_type& yh) const {
    W ret = 0;
    _traverse(xl, xh, yl, yh,
              [&](size_t l, size_t r, auto _) { ret += rsq.range_sum(l, r); },
              [](auto,auto,auto){});
    return ret;
  }

  std::vector<std::pair<size_t, size_t>>
  enumerate(const index_type& xl, 
            const index_type& xh, 
            const index_type& yl, 
            const index_type& yh) const {
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