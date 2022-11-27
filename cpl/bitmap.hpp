#pragma once
#include <cstddef>
#include <cstdint>
#include <array>
#include <vector>
#include "fenwick_tree.hpp"
#include "bit_manip.hpp"
#include <iostream>

class Bitmap {
 public:
  using size_type = int;
  using RSQ = FenwickTree<size_type>;
 private:
  RSQ rsq;
  std::vector<uint64_t> bv;

  size_t word_size(size_t sz) const {
    return (sz + 64-1) / 64;
  }

  std::array<std::array<uint8_t, 9>, 1<<8> sel_tb;
  constexpr void init_sel_tb() {
    for (int i = 0; i < 1<<8; i++) {
      int c = 0;
      int x = i;
      for (int j = 0; j < 8; j++) {
        if (x & 1)
          sel_tb[i][++c] = j;
        x >>= 1;
      }
    }
  }

 public:
  explicit Bitmap(size_t size = 0, bool init_b = false)
      : rsq(word_size(size)),
        bv(word_size(size), init_b ? 0xFFFFFFFFFFFFFFFF : 0) {
    init_sel_tb();
  }
  template<typename It>
  explicit Bitmap(It begin, It end) : Bitmap(end-begin) {
    using traits = std::iterator_traits<It>;
    static_assert(std::is_convertible<typename traits::value_type,
                                      bool>::value, "");
    static_assert(std::is_base_of<std::random_access_iterator_tag,
                                  typename traits::iterator_category>::value, "");
    size_type i = 0;
    for (auto it = begin; it != end; ++it)
      set(i++, *it);
  }

  bool get(size_t i) const {
    return (bv[i/64] >> (i%64)) & 1ull;
  }
  void set(size_t i, bool b) {
    int q = i/64, r = i%64;
    rsq.add(q, (int) b - get(i));
    if (b) bv[q] |= 1ull<<r;
    else   bv[q] &= ~(1ull<<r);
  }

  /* Count 1s in [0, i) */
  size_type rank(size_t i) const {
    if (i == 0) return 0;
    int b = i/64;
    return (size_type) (b > 0 ? rsq.sum(b-1) : 0)
        + bm::popcnt(bv[i/64] & ((1ull<<(i%64))-1));
  }
  /* Position (0-indexed) of the (n+1) th 1 */
  size_type select(size_t n) const { // 0-index
    n += 1; // to 1-index
    int s = rsq.lower_bound(n);
    int m = n - (s > 0 ? rsq.sum(s-1) : 0);
    uint64_t w = bv[s];
    auto x = bm::popcnt_e8(w) * 0x0101010101010100ull; // Cumulative sum for 8bit blocks.
    auto b_rank = (const uint8_t*) &x;
    int b = 0;
    {
      int d = 8;
      while (d > 1) {
        int c = b + d/2;
        if (b_rank[c] < m)
          b = c;
        d /= 2;
      }
    }
    int k = m - b_rank[b];
    int B = ((const uint8_t*) &w)[b];
    return 64 * s + 8 * b + sel_tb[B][k];
  }
};