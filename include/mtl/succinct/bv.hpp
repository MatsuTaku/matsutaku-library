#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>
#include <bitset>
#include <iostream>

#if __cpp_concepts >= 202002L
#include <concepts>
template<class T>
concept ConstructableBV = requires(T t, size_t s) {
  { t.size() } -> std::same_as<size_t>;
  { t.word_size() } -> std::same_as<size_t>;
  { t.get_word(s) } -> std::convertible_to<uint64_t>;
};
#endif

template<
#if __cpp_concepts >= 202002L
  ConstructableBV
#else
  class 
#endif
    BitVec, unsigned WordSize>
struct BV {
  static_assert(WordSize <= 64, "WordSize must be <= 64");
  static constexpr unsigned S = WordSize;
  static constexpr unsigned S_PER_L = 8;
  static constexpr unsigned L = S*S_PER_L;
  using bitvec_type = BitVec;
  const bitvec_type* bm_;
  std::vector<uint64_t> _r, _s, _zs;
  
  BV() = default;
  BV(const bitvec_type* bm) {
    build(bm);
  }
  void set_ptr(const bitvec_type* bm) {
    bm_ = bm;
  }
  void build(const bitvec_type* bm) {
    set_ptr(bm);
    const auto num_l = (bm->size() + L-1) / L;
    _r.assign((num_l + 1) * 2, 0);
    _s.clear();
    _s.push_back(0);
    _zs.clear();
    _zs.push_back(0);
    uint64_t sum = 0;
    for (size_t l = 0; l < num_l; ++l) {
      auto psum = sum;
      uint64_t sum_l = 0;
      for (size_t s = 0; s < S_PER_L; ++s) {
        if (l * S_PER_L + s < bm->word_size())
          sum_l += bm::popcnt(bm->get_word(l * S_PER_L + s));
        if (s < S_PER_L-1)
          _r[l * 2 + 1] |= sum_l << ((7-(s+1)) * 9);
      }
      sum += sum_l;
      _r[(l + 1) * 2] = sum;
      if (psum / L != sum / L) {
        _s.push_back(l);
      }
      if ((L*l - psum) / L != (L*(l+1) - sum) / L) {
        _zs.push_back(l);
      }
    }
    _s.push_back(num_l);
    _zs.push_back(num_l);
  }

  template<bool B>
  size_t get_l(size_t l) const {
    auto b = _r[l*2];
    return B ? b : l * L - b;
  }
  static size_t s_off(size_t s) {
    return (7-s) * 9;
  }
  template<bool B>
  size_t get_s(size_t l, size_t s) const {
    auto b = (_r[l*2+1] >> s_off(s)) % (1ull<<9);
    return B ? b : s * S - b;
  }
  uint64_t mask(size_t width) const {
    return width == 64 ? ~0ull : (1ull << width) - 1;
  }
  size_t rank1(size_t i) const {
    auto l = i / L;
    auto s = i % L / S;
    auto q = i / S;
    auto r = i % S;
    auto w = bm_->get_word(q) & mask(r);
    return get_l<1>(l) + 
           get_s<1>(l, s) + 
           bm::popcnt(w);
  }
  size_t rank0(size_t i) const { 
    return i - rank1(i);
  }
  template<bool B>
  size_t rank(size_t i) const {
    if constexpr (B)
      return rank1(i);
    else
      return rank0(i);
  }

  static struct l_block_cap_mask {
    uint64_t mask;
    constexpr l_block_cap_mask() {
      mask = 0;
      for (unsigned i = 0; i < S_PER_L; i++) {
        uint64_t cap = i * S;
        mask |= cap << s_off(i);
      }
    }
  } l_block_cap;

  template<bool B>
  size_t select(size_t ith) const {
    auto n = ith+1; // to 1-indexed
    if (n > rank<B>(bm_->size()))
      return bm_->size();
    // Find L block
    auto& idx = B ? _s : _zs;
    size_t l = idx[n / L];
    {
      auto r = idx[n / L + 1] + 1;
      while (l+1 < r) {
        auto c = l + (r-l)/2;
        if (get_l<B>(c) < n)
          l = c;
        else
          r = c;
      }
    }
    // Find S block
    size_t s = 0;
    auto m = n - get_l<B>(l);
    /* Following bit-manipulates code is same as ... */
//    {
//      auto d = 8;
//      while (d > 1) {
//        auto c = s + d/2;
//        if (get_s<B>(l, c) < m)
//          s = c;
//        d /= 2;
//      }
//    }
    {
      uint64_t x = (uint64_t) (m-1) * 0x0040201008040201ull;
      uint64_t a = _r[l*2+1];
      if constexpr (!B)
        a = l_block_cap.mask - a; // to 0s sum
      uint64_t xda = x - a;
      uint64_t sm = 0x4020100804020100ull;
      uint64_t ok = (~x | a) & sm;
      uint64_t ng = (~x & a) & sm;
      uint64_t y = ((x ^ a ^ xda) & ok) | ng;
      y = y * 0x0001010101010101ull >> (64-1-7);
      auto id = bm::clz8(y)-1;
      auto clo = bm::clz((~xda << 1 | 1) << (9*id));
      auto ns = id + (clo ? (clo - 1) / 9 : 0);
      s = ns;
    }
    // Calc select
    auto o = m - get_s<B>(l, s);
    uint64_t w = bm_->get_word(l * S_PER_L + s);
    return l * L + 
           s * S + 
           select64::select<B>(o-1, w);
  }
  size_t select1(size_t ith) const {
    return select<1>(ith);
  }
  size_t select0(size_t ith) const {
    return select<0>(ith);
  }
};

template<class BitVec, unsigned WordSize>
typename BV<BitVec, WordSize>::l_block_cap_mask BV<BitVec, WordSize>::l_block_cap;

template<class T>
struct RankSelectTraits : std::false_type {};