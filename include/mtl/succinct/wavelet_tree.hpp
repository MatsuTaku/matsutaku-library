#pragma once
#include <limits>
#include <array>
#include <iterator>
#include <algorithm>
#include <queue>
#include <tuple>
#include "bit_vector.hpp"
#include "../bit_manip.hpp"

template<typename T>
struct WaveletTree {
  static constexpr unsigned H = 64 - bm::clz(std::numeric_limits<T>::max());
  size_t n,h;
  BitVector B;
  WaveletTree() = default;
  template<typename It>
  WaveletTree(It begin, It end)
    : n(std::distance(begin, end)), h(64 - bm::clz(*max_element(begin, end)))
  {
    using trait = std::iterator_traits<It>;
    static_assert(std::is_base_of<std::input_iterator_tag, typename trait::iterator_category>::value, "");
    static_assert(std::is_convertible<typename trait::value_type, T>::value, "");
    assert(*min_element(begin, end) >= 0);

    B.assign(h * n, false);
    std::vector<T> S(begin, end);
    std::vector<size_t> D = {0,n}, nd;
    auto bit = B.begin();
    std::queue<T> sz,so;
    for (int k = h-1; k > 0; k--) {
      for (size_t i = 0; i < n; i++, ++bit)
        *bit = (S[i] & (1ull<<k)) != 0;
      nd.clear();
      nd.push_back(0);
      for (size_t d = 0; d < D.size()-1; ++d) {
        size_t l = D[d], r = D[d+1];
        for (size_t i = l; i < r; ++i)
          if ((S[i] & (1ull<<k)) == 0)
            sz.push(S[i]);
          else
            so.push(S[i]);
        int zs = sz.size();
        {
          auto i = l;
          while (!sz.empty()) {
            S[i++] = sz.front();
            sz.pop();
          }
          while (!so.empty()) {
            S[i++] = so.front();
            so.pop();
          }
          assert(i == r);
        }
        if (zs)
          nd.push_back(l + zs);
        if (l + zs < r)
          nd.push_back(r);
      }
      std::swap(D, nd);
    }
    for (size_t i = 0; i < n; i++, ++bit)
      *bit = (S[i] & 1ull) != 0;
    B.build();
  }

  std::pair<size_t,size_t> child(bool b, size_t l, size_t r) {
    auto os = B.rank(r) - B.rank(l);
    auto zs = r-l - os;
    return !b ? std::make_pair(l + n, l + zs + n)
              : std::make_pair(l + zs + n, r + n);
  }

  T get(size_t i) const {
    size_t l = 0, r = n;
    T ret = 0;
    auto j = i;
    for (int k = h-1; k > 0; k--) {
      auto rl = B.rank(l),
          rj = B.rank(l + j),
          rr = B.rank(r);
      auto zs = r - l - (rr - rl);
      if (B[l + j]) {
        ret |= 1ull<<k;
        j = rj - rl;
        l += n + zs;
        r += n;
      } else {
        j = (l+j - rj) - (l - rl);
        l += n;
        r = l + zs;
      }
    }
    if (B[l + j])
      ret |= 1ull;
    return ret;
  }

  size_t rank(T c, size_t i) const {
    size_t l = 0, r = n;
    auto j = i;
    for (int k = h-1; k >= 0; k--) {
      auto rl = B.rank(l), rj = B.rank(l + j), rr = B.rank(r);
      auto zs = r - l - (rr - rl);
      if (c & (1ull<<k)) {
        j = rj - rl;
        l += n + zs;
        r += n;
      } else {
        j = (l+j - rj) - (l - rl);
        l += n;
        r = l + zs;
      }
    }
    return j;
  }

  size_t rank_lower(T c, size_t i) const {
    size_t l = 0, r = n;
    auto j = i;
    size_t ret = 0;
    for (int k = h-1; k >= 0; k--) {
      auto rl = B.rank(l), rj = B.rank(l+j), rr = B.rank(r);
      auto zs = r - l - (rr - rl);
      if (c & (1ull << k)) {
        ret += (l+j - rj) - (l - rl);
        j = rj - rl;
        l += n + zs;
        r += n;
      } else {
        j = (l+j - rj) - (l - rl);
        l += n;
        r = l + zs;
      }
    }
    return ret;
  }

  std::tuple<size_t, size_t, size_t> rank_3way(T c, size_t i) const {
    size_t lt = 0, gt = 0;
    size_t l = 0, r = n, j = i;
    for (int k = h-1; k >= 0; k--) {
      auto rl = B.rank(l), rj = B.rank(l+j), rr = B.rank(r);
      auto zs = r - l - (rr - rl);
      if (c & (1ull << k)) {
        lt += (l+j - rj) - (l - rl);
        j = rj - rl;
        l += n + zs;
        r += n;
      } else {
        gt += rj - rl;
        j = (l+j - rj) - (l - rl);
        l += n;
        r = l + zs;
      }
    }
    return std::make_tuple(lt, j, gt);
  }

  std::array<int, H> ls;
  size_t select(T c, size_t i) { // 0-indexed
    size_t l = 0, r = n;
    for (int k = h-1; k >= 0; k--) {
      ls[k] = l;
      auto rl = B.rank(l), rr = B.rank(r);
      auto zs = r - l - (rr - rl);
      if (c & (1ull<<k)) {
        l += n + zs;
        r += n;
      } else {
        l += n;
        r = l + zs;
      }
    }
    if (i >= r - l)
      return n; // Not found
    auto j = i;
    for (int k = 0; k < (int)h; k++) {
      auto rl = B.rank(ls[k]);
      if (c & (1ull<<k)) {
        r = rl;
        j = B.select<1>(j + r) - ls[k];
      } else {
        r = ls[k] - rl;
        j = B.select<0>(j + r) - ls[k];
      }
    }
    return j;
  }

 private:
  void _enumerate(size_t l, size_t r, size_t a, size_t b, T c,
                  std::vector<std::pair<T, size_t>>& enm) const {
    if (b-a == 0)
      return;
    if (l >= n*h) {
      enm.emplace_back(c, b-a);
      return;
    }
    auto rl = B.rank(l),
        rr = B.rank(r),
        ra = B.rank(l+a) - rl,
        rb = B.rank(l+b) - rl;
    auto zs = (r - rr) - (l - rl);
    _enumerate(l + n, l + n + zs, a - ra, b - rb, c, enm);
    auto k = h-1 - l/n;
    _enumerate(l + n + zs, r + n, ra, rb, c | (1ull << k), enm);
  }
 public:
  std::vector<std::pair<T, size_t>> enumerate(size_t l, size_t r) const {
    std::vector<std::pair<T, size_t>> enm;
    _enumerate(0, n, l, r, T{0}, enm);
    return enm;
  }

 private:
  std::pair<size_t, size_t> child_tie(bool bit, size_t l, size_t r) const {
    auto zs = (r-l) - (B.rank(r) - B.rank(l));
    if (!bit)
      return std::make_pair(l+n, l+n+zs);
    else
      return std::make_pair(l+n+zs, r+n);
  }

 public:
  // Get frequency of values which (x <= value < y) in S[l,r).
  size_t range_freq(size_t l, size_t r, T x, T y) const {
    size_t freq = 0;
    std::queue<std::tuple<size_t, size_t, T>> qs;
    qs.emplace(l,r,0,n,T(0));
    while (!qs.empty(0)) {
      size_t i,j,_l,_r;
      T c;
      std::tie(i,j,_l,_r,c) = qs.front(); qs.pop();
      if (i == j)
        continue;
      size_t level = i/n;
      int shift = h-1-level;
      T clo = c, chi = c | ((1ull<<(shift+1))-1);
      if (chi < x or y <= clo)
        continue;
      if (x <= clo and chi < y) {
        freq += j - i;
        continue;
      }
      assert(level < h);
      size_t rl = B.rank(_l), ri = B.rank(_l+i), rj = B.rank(_l+j), rr = B.rank(_r);
      size_t zs = (_r - rr) - (_l - rl);
      qs.emplace((_l+i - ri) - (_l - rl), (_l+j - rj) - (_l - rl),
                 _l+n, _l+n+zs, c);
      qs.emplace(ri - rl, rj - rl,
                 _l+n+zs, _r+n, c|(1ull<<shift));
    }
    return freq;
  }
};