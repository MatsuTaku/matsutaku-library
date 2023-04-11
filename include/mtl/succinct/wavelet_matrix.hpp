#pragma once
#include "bit_vector.hpp"
#include "../bit_manip.hpp"
#include <limits>
#include <array>
#include <vector>
#include <iterator>
#include <algorithm>
#include <queue>
#include <tuple>

template<typename T>
struct WaveletMatrix {
  static constexpr unsigned H = 64 - bm::clz(std::numeric_limits<T>::max());

  size_t n,h;
  Bitmap B;
  Bitmap::rs_type rs_b;
  std::vector<size_t> RO,Z;

  WaveletMatrix() = default;
  template<typename It>
  WaveletMatrix(It begin, It end)
      : n(std::distance(begin, end)),
      h(std::max(1u, 64 - bm::clz(*max_element(begin, end)))),
      B(n*h, false),
      rs_b(),
      RO(h+1),
      Z(h)
  {
    using trait = std::iterator_traits<It>;
    static_assert(std::is_base_of<std::input_iterator_tag, typename trait::iterator_category>::value, "");
    static_assert(std::is_convertible<typename trait::value_type, T>::value, "");
    assert(*min_element(begin, end) >= 0);

    std::vector<T> S(begin, end), z, o;
    z.reserve(n);
    o.reserve(n);
    auto bit = B.begin();
    for (int k = h-1; k >= 0; k--) {
      for (size_t i = 0; i < n; i++) {
        bool b = S[i] >> k & 1;
        *bit++ = b;
        if (!b)
          z.push_back(S[i]);
        else
          o.push_back(S[i]);
      }
      Z[k] = n*(h-1-k+1) + z.size();
      auto j = n;
      while (!o.empty()) {
        S[--j] = o.back();
        o.pop_back();
      }
      while (!z.empty()) {
        S[--j] = z.back();
        z.pop_back();
      }
      assert(j == 0);
    }
    rs_b.build(&B);
    for (size_t i = 0; i <= h; i++)
      RO[i] = rs_b.rank1(n * i);
  }
  WaveletMatrix(const WaveletMatrix& rhs) noexcept :
    n(rhs.n),
    h(rhs.h),
    B(rhs.B),
    rs_b(rhs.rs_b),
    RO(rhs.RO),
    Z(rhs.Z) 
  {
    rs_b.set_ptr(&B);
  }
  WaveletMatrix& operator=(const WaveletMatrix& rhs) noexcept {
    n = rhs.n;
    h = rhs.h;
    B = rhs.B;
    rs_b = rhs.rs_b;
    rs_b.set_ptr(&B);
    RO = rhs.RO;
    Z = rhs.Z;
    return *this;
  }
  WaveletMatrix(WaveletMatrix&& rhs) noexcept :
    n(std::move(rhs.n)),
    h(std::move(rhs.h)),
    B(std::move(rhs.B)),
    rs_b(std::move(rhs.rs_b)),
    RO(std::move(rhs.RO)),
    Z(std::move(rhs.Z)) 
  {
    rs_b.set_ptr(&B);
  }
  WaveletMatrix& operator=(WaveletMatrix&& rhs) noexcept {
    n = std::move(rhs.n);
    h = std::move(rhs.h);
    B = std::move(rhs.B);
    rs_b = std::move(rhs.rs_b);
    rs_b.set_ptr(&B);
    RO = std::move(rhs.RO);
    Z = std::move(rhs.Z);
    return *this;
  }

  inline size_t _child0(size_t level, size_t i) const {
      return i + n + RO[level] - rs_b.rank1(i);
  }
  inline size_t _child1(size_t level, size_t i) const {
    return n*(level+2) + rs_b.rank1(i) - RO[level+1];
  }
  inline size_t child(size_t level, size_t i, bool bit) const {
    return !bit ? _child0(level, i) : _child1(level, i);
  }
  std::pair<size_t, size_t> _child_tie0(size_t level, size_t l, size_t r) const {
    return std::make_pair(_child0(level, l), _child0(level, r));
  }
  std::pair<size_t, size_t> _child_tie1(size_t level, size_t l, size_t r) const {
    return std::make_pair(_child1(level, l), _child1(level, r));
  }
  std::pair<size_t, size_t> child_tie(size_t level, size_t l, size_t r, bool bit) const {
    return !bit ? _child_tie0(level, l, r) : _child_tie1(level, l, r);
  }

  inline size_t _parent0(size_t level, size_t i) const {
    return rs_b.select<0>(i - n - RO[level]);
  }
  inline size_t _parent1(size_t level, size_t i) const {
    return rs_b.select<1>(i - Z[h-1-level] + RO[level]);
  }
  inline size_t parent(size_t level, size_t i, bool bit) const {
    return !bit ? _parent0(level, i) : _parent1(level, i);
  }

  T get(size_t i) const {
    T c = 0;
    size_t j = i;
    for (int k = h-1; k > 0; k--) {
      bool b = B[j];
      j = child(h-1-k, j, b);
      if (b)
        c |= 1ull<<k;
    }
    if (B[j])
      c |= 1u;
    return c;
  }

  size_t range_rank(T c, size_t l, size_t r) const {
    for (int k = h-1; k >= 0; k--) {
      if (l == r)
        break;
      std::tie(l,r) = child_tie(h-1-k, l, r, (c >> k) & 1u);
    }
    return r - l;
  }
  size_t rank(T c, size_t i) const {
    return range_rank(c, 0, i);
  }
  std::tuple<size_t, size_t, size_t> rank_3way(T c, size_t l, size_t r) const {
    size_t lt = 0, gt = 0;
    for (int k = h-1; k >= 0; k--) {
      size_t pr = r - l;
      if (pr == 0)
        break;
      if (((c >> k) & 1u) == 0) {
        std::tie(l,r) = _child_tie0(h-1-k, l, r);
        gt += pr - (r - l);
      } else {
        std::tie(l,r) = _child_tie1(h-1-k, l, r);
        lt += pr - (r - l);
      }
    }
    return std::make_tuple(lt, r - l, gt);
  }

  /// Get frequency of values which (x <= value < y) in S[l,r).
  size_t range_freq(size_t l, size_t r, T x, T y) const {
    size_t freq = 0;
    std::queue<std::tuple<size_t,size_t, T>> qs;
    qs.emplace(l, r, T(0));
    while (!qs.empty()) {
      size_t _l,_r;
      T c;
      std::tie(_l,_r,c) = qs.front();
      qs.pop();
      size_t level = _l/n;
      if (_l == _r)
        continue;
      int shift = h-1-level;
      T clo = c, chi = c | ((1ull<<(shift+1))-1);
      if (chi < x or y <= clo)
        continue;
      if (x <= clo and chi < y) {
        freq += _r - _l;
        continue;
      }
      assert(level < h);
      size_t nl,nr;
      std::tie(nl,nr) = child_tie(level, _l, _r, 0);
      qs.emplace(nl, nr, c);
      std::tie(nl,nr) = child_tie(level, _l, _r, 1);
      qs.emplace(nl, nr, c | (1ull << shift));
    }
    return freq;
  }

  size_t range_select(T c, size_t l, size_t r, size_t i) const {
    if (r - l <= i)
      return n;
    for (int k = h-1; k >= 0; k--) {
      std::tie(l,r) = child_tie(h-1-k, l, r, (c >> k) & 1u);
      if (r - l <= i)
        return n;
    }
    size_t j = l+i;
    for (size_t k = 0; k < h; k++) {
      j = parent(h-1-k, j, (c >> k) & 1u);
      assert((bool)((c>>k)&1u) == B[j]);
    }
    return j;
  }
  size_t select(T c, size_t i) const {
    return range_select(c, 0, n, i);
  }

  /// Get kth (0-indexed) smallest value in S[l,r).
  T quantile(size_t l, size_t r, size_t k) const {
    assert(r - l > k);
    T c = 0;
    for (int d = h-1; d > 0; d--) {
      auto os = rs_b.rank1(r) - rs_b.rank1(l);
      auto zs = r - l - os;
      if (k < zs) {
        std::tie(l,r) = _child_tie0(h-1-d, l, r);
      } else {
        c |= 1ull << d;
        k -= zs;
        std::tie(l,r) = _child_tie1(h-1-d, l, r);
      }
      assert(l < r);
    }
    auto os = rs_b.rank1(r) - rs_b.rank1(l);
    auto zs = r - l - os;
    if (k >= zs) {
      c |= 1ull;
    }
    return c;
  }

  /// Get tuples (value, frequency) of the most k frequently occurring values in S[l,r).
  std::vector<std::pair<T, size_t>> top_k(size_t l, size_t r, size_t k) const {
    std::vector<std::pair<T, size_t>> ret;
    std::priority_queue<std::tuple<size_t, size_t, T>> qs;
    qs.emplace(r-l, l, 0);
    while (!qs.empty()) {
      size_t range, s;
      T c;
      std::tie(range, s, c) = qs.top();
      qs.pop();
      auto level = s/n;
      if (level == h) {
        ret.emplace_back(c, range);
        if (ret.size() == k)
          break;
      } else {
        size_t _l, _r;
        for (int b = 0; b < 2; b++) {
          std::tie(_l,_r) = child_tie(level, s, s+range, b);
          if (_l != _r)
            qs.emplace(_r-_l, _l, c | (uint64_t(b) << (h-1-level)));
        }
      }
    }
    return ret;
  }
  /// Get sum of S[l,r) in O(min(r-l, \sigma) log \sigma) times.
  template<typename U=T>
  U sum(size_t l, size_t r) const {
    U ret = 0;
    for (auto p : top_k(l, r, r-l))
      ret += (U) p.first * p.second;
    return ret;
  }

  /// Get k tuples of (value, frequency) that value satisfies condition (x <= value < y) in S[l,r) from the smallest (or largest).
  /// The complexity is O(k log \sigma).
  template<bool ASCENDING, bool VALUE_RANGE = true>
  std::vector<std::pair<T, size_t>>
  range_list_k(size_t l, size_t r, size_t k, T x, T y) const {
    std::vector<std::pair<T, size_t>> ret;
    std::queue<std::tuple<size_t, size_t, T>> qs;
    qs.emplace(r-l, l, T(0));
    size_t range,s;
    T c;
    while (!qs.empty()) {
      std::tie(range,s,c) = qs.top();
      qs.pop();
      auto level = s/n;
      if (level == h) {
        assert(!VALUE_RANGE or (x <= c and c < y));
        ret.emplace_back(c, range);
        if (ret.size() == k)
          break;
      } else {
        auto shift = (h-1-level);
        for (int b = ASCENDING ? 0 : 1;
             ASCENDING ? b < 2 : b >= 0;
             ASCENDING ? b++ : b--) {
          T nc = (c << 1) | b;
          if (VALUE_RANGE and (nc < (x >> shift) or ((y-1) >> shift) < nc))
            continue;
          size_t nl,nr;
          std::tie(nl,nr) = child_tie(level, s, s+range, b);
          if (nl != nr)
            qs.emplace(nr-nl, nl, nc);
        }
      }
    }
    return ret;
  }

  /// Get tuples of (value, frequency) that value satisfies condition (x <= value < y) in S[l,r).
  /// The complexity is O(k log \sigma).
  std::vector<std::pair<T, size_t>> range_list(size_t l, size_t r, T x, T y) const {
    return range_list_k<true>(l, r, r - l, x, y);
  }

  /// Get k tuples of (value, frequency) that value satisfies condition (x <= value < y) in S[l,r) from the largest.
  /// The complexity is O(k log \sigma).
  std::vector<std::pair<T, size_t>> range_max_k(size_t l, size_t r, size_t k) const {
    return range_list_k<false, false>(l, r, k, 0, 0);
  }
  /// Get k tuples of (value, frequency) that value satisfies condition (x <= value < y) in S[l,r) from the smallest.
  // The complexity is O(k log \sigma).
  std::vector<std::pair<T, size_t>> range_min_k(size_t l, size_t r, size_t k) const {
    return range_list_k<true, false>(l, r, k, 0, 0);
  }

  /// Get tuples (value, frequency of T1, frequency of T2) that commonly occur between T1=S[l1,r1) and T2=S[l2,r2).
  std::vector<std::tuple<T, size_t, size_t>> intersect(size_t l1, size_t r1, size_t l2, size_t r2) const {
    std::vector<std::tuple<T, size_t, size_t>> ret;
    std::queue<std::pair<std::array<size_t,4>, T>> qs;
    qs.emplace({l1,r1,l2,r2}, 0);
    std::array<size_t,4> nrs{};
    while (!qs.empty()) {
      const auto& rs = qs.front().first;
      T c = qs.front().second;
      auto level = rs[0]/n;
      if (level == h) {
        ret.emplace_back(c, rs[1]-rs[0], rs[3]-rs[2]);
      } else {
        for (int b = 0; b < 2; b++) {
          for (int i = 0; i < 4; i++)
            nrs[i] = child(level, rs[i], b);
          if (nrs[0] != nrs[1] and nrs[2] != nrs[3])
            qs.emplace(nrs, c | (uint64_t(b) << (h-1-level)));
        }
      }
      qs.pop();
    }
    return ret;
  }
};
