#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include <iostream>
#include <array>
#include <bitset>
#include "../bit_manip.hpp"

// Bitmap is likes std::vector<bool>
struct Bitmap {
  using value_type = bool;
  using W = uint64_t;
  std::vector<W> arr;
  size_t sz;
  static constexpr size_t required_word_size(size_t n) {
    return (n+63) / 64;
  }
  static constexpr W word_filled_by(bool bit) {
    return bit ? 0xFFFFFFFFFFFFFFFF : 0;
  }
  explicit Bitmap(size_t n = 0, bool bit = false) : arr(required_word_size(n), word_filled_by(bit)), sz(n) {}
  template<typename It>
  Bitmap(It begin, It end) : sz(0) {
    using trait = std::iterator_traits<It>;
    using iterator_category = typename trait::iterator_category;
    static_assert(std::is_base_of<std::input_iterator_tag, iterator_category>::value, "");
    static_assert(std::is_convertible<typename trait::value_type, bool>::value, "");
    if constexpr (std::is_base_of<std::random_access_iterator_tag, iterator_category>::value) {
      arr.reserve(required_word_size(std::distance(begin, end)));
    }
    for (auto it = begin; it != end; ++it)
      push_back((bool)*it);
  }

  size_t size() const { return sz; }
  bool empty() const { return size() == 0; }

  void push_back(bool bit) {
    auto r = sz % 64;
    if (r == 0) {
      arr.push_back((W)bit);
    } else {
      if (bit)
        arr.back() |= 1ull << r;
      else
        arr.back() &= ~(1ull << r);
    }
    ++sz;
  }
  void pop_back() {
    --sz;
  }
  void resize(size_t new_size, bool bit) {
    auto old_size = size();
    sz = new_size;
    if (new_size < old_size) {
      return;
    }
    arr.resize(required_word_size(new_size), word_filled_by(bit));
    auto r = old_size % 64;
    if (r) {
      W mask = (1ull << r) - 1;
      if (bit)
        arr[old_size / 64] |= ~mask;
      else
        arr[old_size / 64] &= mask;
    }
  }
  void assign(size_t new_size, bool bit) {
    sz = new_size;
    arr.assign(required_word_size(new_size), word_filled_by(bit));
  }

  struct const_reference;
  struct reference;
  template<bool>
  struct _iterator;
  using const_iterator = _iterator<true>;
  using iterator = _iterator<false>;

  const_reference operator[](size_t i) const {
    return const_reference(arr.data() + i / 64, 1ull << (i % 64));
  }
  reference operator[](size_t i) {
    return {arr.data() + i / 64, 1ull << (i % 64)};
  }
  const_iterator begin() const { return const_iterator(arr.data(), 0); }
  iterator begin() { return iterator(arr.data(), 0); }
  const_iterator cbegin() const { return begin(); }
  const_iterator end() const { return const_iterator(arr.data() + sz / 64, sz % 64); }
  iterator end() { return iterator(arr.data() + sz / 64, sz % 64); }
  const_iterator cend() const { return end(); }

  template<bool Const>
  struct reference_base {
    using _pointer = typename std::conditional<Const, const W*, W*>::type;
    using _iterator = typename std::conditional<Const, const_iterator, iterator>::type;
    _pointer ptr;
    W mask;
    reference_base(_pointer ptr, W mask) : ptr(ptr), mask(mask) {}
    reference_base(const reference_base&) = delete;
    reference_base& operator=(const reference_base&) = delete;
    reference_base(reference_base&&) noexcept = default;
    reference_base& operator=(reference_base&&) noexcept = default;
    inline operator bool() const {
      return (*ptr & mask) != 0;
    }
    inline bool operator==(bool r) const {
      return (bool) *this == r;
    }
    inline friend bool operator==(bool l, const reference_base& r) {
      return r == l;
    }
    inline bool operator!=(bool r) const {
      return (bool) *this != r;
    }
    inline friend bool operator!=(bool l, const reference_base& r) {
      return r != l;
    }
    _iterator operator&() const {
      return {ptr, bm::ctz(mask)};
    }
    std::ostream& operator<<(std::ostream& os) const {
      return os << (bool) *this;
    }
  };
  struct const_reference : public reference_base<true> {
    using _base = reference_base<true>;
    const_reference(_base::_pointer ptr, W mask) : _base(ptr, mask) {}
    const_reference(const reference& rhs) : _base(rhs.ptr, rhs.mask) {}
  };
  struct reference : public reference_base<false> {
    using _base = reference_base<false>;
    reference(_base::_pointer ptr, W mask) : _base(ptr, mask) {}
    inline reference& operator=(bool bit) {
      if (bit)
        *ptr |= mask;
      else
        *ptr &= ~mask;
      return *this;
    }
    inline reference& operator|=(bool bit) {
      if (bit)
        *ptr |= mask;
      return *this;
    }
    inline reference& operator&=(bool bit) {
      if (!bit)
        *ptr &= ~mask;
      return *this;
    }
    template<bool C>
    inline reference& operator=(const reference_base<C>& rhs) {
      return *this = (bool) rhs;
    }
    reference(const reference& rhs) = delete;
    reference(reference&& rhs) noexcept = default;
    reference& operator=(reference&& rhs) noexcept = default;
    std::istream& operator>>(std::istream& is) {
      bool b;
      is >> b;
      operator=(b);
      return is;
    }
  };

  template<bool Const>
  struct _iterator
      : public std::iterator<std::random_access_iterator_tag, bool, std::ptrdiff_t, iterator, typename std::conditional<Const, const_reference, reference>::type>
  {
    using _pointer = typename std::conditional<Const, const W*, W*>::type;
    using _reference = typename std::conditional<Const, const_reference, reference>::type;
    _pointer ptr;
    unsigned ctz;
    _iterator(_pointer ptr, unsigned ctz) : ptr(ptr), ctz(ctz) {}
    inline _reference operator*() const {
      return _reference(ptr, 1ull << ctz);
    }
    template<bool C>
    inline bool operator==(const _iterator<C>& r) const {
      return ptr == r.ptr and ctz == r.ctz;
    }
    template<bool C>
    inline bool operator!=(const _iterator<C>& r) const {
      return ptr != r.ptr or ctz != r.ctz;
    }
    inline _iterator& operator++() {
      if (ctz % 64 < 63) {
        ++ctz;
      } else {
        ++ptr;
        ctz = 0;
      }
      return *this;
    }
    inline _iterator operator++(int) {
      _iterator ret = *this;
      operator++();
      return ret;
    }
    inline _iterator& operator--() {
      if (ctz % 64 > 0) {
        --ctz;
      } else {
        --ptr;
        ctz = 63;
      }
      return *this;
    }
    inline _iterator operator--(int) {
      _iterator ret = *this;
      operator--();
      return ret;
    }
    inline _iterator& operator+=(std::ptrdiff_t d) {
      if (d < 0)
        return operator-=(-d);
      auto r = d % 64;
      if (ctz + r < 64) {
        ptr += d / 64;
        ctz += r;
      } else {
        ptr += d / 64 + 1;
        ctz = (ctz + r) - 64;
      }
      return *this;
    }
    inline _iterator operator+(std::ptrdiff_t d) const {
      return _iterator(*this) += d;
    }
    inline _iterator& operator-=(std::ptrdiff_t d) {
      if (d < 0)
        return operator+=(-d);
      auto r = d % 64;
      if (r <= ctz) {
        ptr -= d / 64;
        ctz -= r;
      } else {
        ptr -= d / 64 + 1;
        ctz = (ctz + 64 - r) - 64;
      }
      return *this;
    }
    inline _iterator operator-(std::ptrdiff_t d) const {
      return _iterator(*this) -= d;
    }
    inline _reference operator[](size_t i) const {
      return *(*this + i);
    }
  };
};

struct BitVector {
  static constexpr unsigned L = 512;
  static constexpr unsigned S = 64;
  static constexpr unsigned S_PER_L = L / S;
  Bitmap bm;
  std::vector<uint64_t> _r, _s, _zs;

  std::array<std::array<uint8_t, 9>, 1<<8> sel_tb;
  constexpr void init_sel_tb() {
    for (int i = 0; i < 1<<8; i++) {
      int c = 0;
      int x = i;
      sel_tb[i].fill(8);
      for (int j = 0; j < 8; j++) {
        if (x & 1)
          sel_tb[i][++c] = j;
        x >>= 1;
      }
    }
  }

  BitVector(size_t size = 0, bool bit = false) : bm(size, bit) {
    init_sel_tb();
  }
  template<typename It>
  BitVector(It begin, It end) : bm(begin, end) {
    init_sel_tb();
  }
  size_t size() const { return bm.size(); }
  bool empty() const { return bm.empty(); }
  void push_back(bool bit) { bm.push_back(bit); }
  void resize(size_t new_size, bool bit) { bm.resize(new_size, bit); }
  void assign(size_t new_size, bool bit) { bm.assign(new_size, bit); }
  Bitmap::const_reference operator[](size_t i) const { return bm[i]; }
  Bitmap::reference operator[](size_t i) { return bm[i]; }
  Bitmap::const_iterator begin() const { return bm.begin(); }
  Bitmap::const_iterator end() const { return bm.end(); }
  Bitmap::iterator begin() { return bm.begin(); }
  Bitmap::iterator end() { return bm.end(); }
  Bitmap::const_iterator cbegin() const { return bm.cbegin(); }
  Bitmap::const_iterator cend() const { return bm.cend(); }

  void build() {
    const auto num_l = (bm.size() + L-1) / L;
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
        if (l * S_PER_L + s < bm.arr.size())
          sum_l += bm::popcnt(bm.arr[l * S_PER_L + s]);
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
  inline size_t get_l(size_t l) const {
    return B ? _r[l * 2] : l * L - _r[l * 2];
  }
  template<bool B>
  inline size_t get_s(size_t l, size_t s) const {
    auto b = (_r[l*2+1] >> ((7-s)*9)) % (1ull<<9);
    return B ? b : s * S - b;
  }

  inline size_t rank(size_t i) const {
    auto l = i / L;
    auto s = i % L / S;
    auto q = i / S;
    auto r = i % S;
    return get_l<1>(l) + get_s<1>(l, s) + bm::popcnt(bm.arr[q] & ((1ull<<r)-1));
  }

  template<bool B>
  size_t select(size_t n) const {
    ++n; // to 1-indexed
    if (n > (B ? rank(size()) : size() - rank(size())))
      return size();
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
      if (!B)
        a = 0x10100C08050301C0ull - a; // to 0s sum
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
    // Find byte
    uint64_t w = bm.arr[l * S_PER_L + s];
    if constexpr (!B) w = ~w;
    auto _bs = (uint64_t) bm::popcnt_e8(w) * 0x0101010101010100ull;
    auto bs = (const uint8_t*) &_bs;
    size_t b = 0;
    auto o = m - get_s<B>(l, s);
    /* Following bit-manipulates code is same as ... */
//    {
//      auto d = 8;
//      while (d > 1) {
//        auto c = b + d/2;
//        if (bs[c] < o)
//          b = c;
//        d /= 2;
//      }
//    }
    {
      uint64_t x = (uint64_t) o * 0x0101010101010101ull;
      uint64_t bmx = (_bs | 0x8080808080808080ull) - x;
      uint64_t y = ((bmx & 0x8080808080808080ull) * 0x02040810204081ull) >> (64-8);
      size_t nb = bm::ctz8(y) - 1;
//      assert(b == nb);
      b = nb;
    }
    // Calc select
    auto r = o - bs[b];
    uint8_t byte = ((const uint8_t*)(&w))[b];
    assert(r and r <= bm::popcnt(byte));
    return l * L + s * S + b * 8 + sel_tb[byte][r];
  }
};