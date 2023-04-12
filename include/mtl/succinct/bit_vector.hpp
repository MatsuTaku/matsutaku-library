#pragma once
#include "../bit_manip.hpp"
#include "select.hpp"
#include "bv.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>
#include <iostream>
#include <array>
#include <bitset>

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
  explicit Bitmap(size_t n = 0, bool bit = false) 
    : arr(required_word_size(n), word_filled_by(bit)), sz(n) {}
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
  void resize(size_t new_size, bool bit=false) { // TODO: fix when bit = true
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
  void reserve(size_t reserved_size) {
    arr.reserve(required_word_size(reserved_size));
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
  const_reference get(size_t i) const {
    return operator[](i);
  }
  /**
   * Usable without pre-set required size
  */
  void set(size_t i, bool b) {
    if (i >= size())
      resize(i + 1);
    operator[](i) = b;
  }
  /**
   * No build process is needed
  */
  void build() const {}
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

  void range_set(size_t b, size_t e, uint64_t x) {
    if (b >= e) return;
    auto r = b % 64;
    auto w = e-b;
    auto mask = w < 64 ? (1ull << w) - 1 : ~0ull;
    assert(x <= mask);
    arr[b/64] = (arr[b/64] & ~(mask << r)) | x << r;
    if (mask + r > 64) {
      arr[b/64+1] = (arr[b/64+1] & ~(mask >> (64-r))) | x >> (64-r);
    }
  }
  uint64_t range_get(size_t b, size_t e) const {
    if (b >= e) return 0;
    assert(e-b <= 64);
    auto r = b % 64;
    auto w = e-b;
    auto mask = w < 64 ? (1ull << w) - 1 : ~0ull;
    auto x = arr[b/64] >> r;
    if (w + r > 64) 
      x |= arr[b/64+1] << (64-r);
    return x & mask;
  }
  const uint64_t& get_word(size_t wi) const {
    return arr[wi];
  }
  size_t word_size() const {
    return arr.size();
  }
  // using rank_select_type = BV<Bitmap, 64>;
};

template<>
struct RankSelectTraits<Bitmap> {
  using rank_select_type = BV<Bitmap, 64>;
};

struct BitVector {
  Bitmap bm;
  using rs_type = typename RankSelectTraits<Bitmap>::rank_select_type;
  rs_type rs_support;
  // std::vector<uint64_t> _r, _s, _zs;

  BitVector(size_t size = 0, bool bit = false) : bm(size, bit) {}
  BitVector(const BitVector& rhs) : bm(rhs.bm), rs_support(rhs.rs_support) {
    rs_support.set_ptr(&bm);
  }
  BitVector& operator=(const BitVector& rhs) {
    bm = rhs.bm;
    rs_support = rhs.rs_support;
    rs_support.set_ptr(&bm);
    return *this;
  }
  BitVector(BitVector&& rhs) noexcept : 
    bm(std::move(rhs.bm)), 
    rs_support(std::move(rhs.rs_support)) {
    rs_support.set_ptr(&bm);
  }
  BitVector& operator=(BitVector&& rhs) noexcept {
    bm = std::move(rhs.bm);
    rs_support = std::move(rhs.rs_support);
    rs_support.set_ptr(&bm);
    return *this;
  }
  template<typename It>
  BitVector(It begin, It end) : bm(begin, end) {
    build();
  }
  size_t size() const { return bm.size(); }
  bool empty() const { return bm.empty(); }
  void push_back(bool bit) { bm.push_back(bit); }
  void resize(size_t new_size, bool bit) { bm.resize(new_size, bit); }
  void assign(size_t new_size, bool bit) { bm.assign(new_size, bit); }
  void reserve(size_t reserved_size) { bm.reserve(reserved_size); }
  Bitmap::const_reference operator[](size_t i) const { return bm[i]; }
  Bitmap::reference operator[](size_t i) { return bm[i]; }
  Bitmap::const_iterator begin() const { return bm.begin(); }
  Bitmap::const_iterator end() const { return bm.end(); }
  Bitmap::iterator begin() { return bm.begin(); }
  Bitmap::iterator end() { return bm.end(); }
  Bitmap::const_iterator cbegin() const { return bm.cbegin(); }
  Bitmap::const_iterator cend() const { return bm.cend(); }

  void build() {
    rs_support.build(&bm);
  }

  inline size_t rank(size_t i) const {
    return rs_support.rank1(i);
  }

  template<bool B>
  size_t select(size_t n) const {
    return rs_support.select<B>(n);
  }
};