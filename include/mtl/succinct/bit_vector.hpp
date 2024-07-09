#pragma once
#include "traits.hpp"
#include "bitmap.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>
#include <iostream>
#include <array>
#include <bitset>

struct BitVector {
  using bitmap_type = Bitmap;
  bitmap_type bm;
  using rs_type = typename RankSelectTraits<bitmap_type>::rank_select_type;
  rs_type rs_support;
  // std::vector<uint64_t> _r, _s, _zs;

  BitVector() = default;
  explicit BitVector(size_t size) : bm(size) {}
  BitVector(size_t size, bool bit) : bm(size, bit) {}
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
  void resize(size_t new_size, bool bit = false) { bm.resize(new_size, bit); }
  void assign(size_t new_size, bool bit) { bm.assign(new_size, bit); }
  void reserve(size_t reserved_size) { bm.reserve(reserved_size); }
  bitmap_type& bitmap() { return bm; }
  const bitmap_type& bitmap() const { return bm; }
  bitmap_type::const_reference operator[](size_t i) const { return bm[i]; }
  bitmap_type::reference operator[](size_t i) { return bm[i]; }
  bitmap_type::const_iterator begin() const { return bm.begin(); }
  bitmap_type::const_iterator end() const { return bm.end(); }
  bitmap_type::iterator begin() { return bm.begin(); }
  bitmap_type::iterator end() { return bm.end(); }
  bitmap_type::const_iterator cbegin() const { return bm.cbegin(); }
  bitmap_type::const_iterator cend() const { return bm.cend(); }

  void build() {
    rs_support.build(&bm);
  }

  inline size_t rank(size_t i) const {
    return rs_support.rank1(i);
  }
  size_t rank1(size_t i) const {
    return rs_support.rank1(i);
  }
  size_t rank0(size_t i) const {
    return rs_support.rank0(i);
  }

  template<bool B>
  size_t select(size_t n) const {
    return rs_support.select<B>(n);
  }
  size_t select1(size_t n) const {
    return rs_support.select<1>(n);
  }
  size_t select0(size_t n) const {
    return rs_support.select<0>(n);
  }
};