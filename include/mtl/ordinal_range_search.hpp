#pragma once
#include "succinct/wavelet_matrix.hpp"
#include "compress_int.hpp"
#include "fenwick_tree.hpp"
#include <cstddef>
#include <vector>
#include <algorithm>
#include <tuple>
#include <queue>

/**
 * @brief Ordinal Range Search
 *   Maintain 2-dimential points (x,y) and their weights.
*/
template<typename T, typename W = long long>
struct ORS {
  std::vector<std::tuple<T,T,W>> ps;
  WaveletMatrix<int> wm;
  FenwickTree<W> rsq;
  std::vector<size_t> ls;
  std::vector<T> xs, ys;
  std::unordered_map<T, int> xc, yc;

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

    xs.clear();
    std::vector<std::tuple<T, size_t, W>> yi;
    for (size_t i = 0; i < ps.size(); i++) {
      T x,y;
      W w;
      std::tie(x,y,w) = ps[i];
      if (xs.empty() or xs.back() != x)
        xs.push_back(x);
      yi.emplace_back(y, i, w);
    }
    if (xs.empty() or xs.back() != std::numeric_limits<T>::max())
      xs.push_back(std::numeric_limits<T>::max());

    xc.clear();
    xc.reserve(xs.size());
    for (size_t i = 0; i < xs.size(); i++)
      xc[xs[i]] = i;

    std::sort(yi.begin(), yi.end());
    ys.resize(yi.size());
    for (size_t i = 0; i < yi.size(); i++)
      ys[i] = std::get<0>(yi[i]);
    ys.push_back(std::numeric_limits<T>::max());
    ys.erase(std::unique(ys.begin(), ys.end()), ys.end());

    yc.clear();
    yc.reserve(ys.size());
    for (size_t i = 0; i < ys.size(); i++)
      yc[ys[i]] = i;

    {
      std::vector<int> S(ps.size());
      std::vector<std::pair<int,W>> SW(ps.size()),swz,swo;
      for (size_t i = 0; i < ps.size(); i++) {
        T x,y;
        W w;
        std::tie(x,y,w) = ps[i];
        S[i] = yc[y];
        SW[i] = {yc[y], w};
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
    ls.resize(xs.size());
    int k = 0;
    ls[k++] = 0;
    for (size_t i = 1; i < ps.size(); i++) {
      if (std::get<0>(ps[i-1]) != std::get<0>(ps[i]))
        ls[k++] = i;
    }
    ls[k] = ps.size();
  }

 private:
  std::tuple<int, int, int, int>
  compress_idx(T xl, T xh, T yl, T yh) const {
    auto _xl = std::lower_bound(xs.begin(), xs.end(), xl);
    auto _xh = std::lower_bound(_xl, xs.end(), xh);
    auto _yl = std::lower_bound(ys.begin(), ys.end(), yl);
    auto _yh = std::lower_bound(_yl, ys.end(), yh);
    return std::make_tuple(ls[_xl-xs.begin()], ls[_xh-xs.begin()],
                           (int)(_yl-ys.begin()), (int)(_yh-ys.begin()));
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
    assert(xc.count(x));
    assert(yc.count(y));
    int cx = xc[x];
    int l = ls[cx], r = ls[cx+1];
    int c = yc[y];
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
        auto lit = upper_bound(ls.begin(), ls.end(), jdx);
        ret.emplace_back(xs[lit-1-ls.begin()], ys[c]);
      }
    });
    return ret;
  }
};