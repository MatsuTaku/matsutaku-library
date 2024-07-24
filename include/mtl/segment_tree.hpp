#pragma once
#include "bit_manip.hpp"
#include <cstddef>
#include <vector>
#include <stack>
#if __cplusplus >= 202002L
#include <concepts>

template<class M>
concept SegmentTreeMonoid = requires (M m) {
  {m * m} -> std::same_as<M>;
};
#endif

template <class M>
class SegmentTree {
#if __cplusplus >= 202002L
  static_assert(SegmentTreeMonoid<M>);
#endif
 public:
  using monoid_type = M;
  using value_type = monoid_type;
 private:
  size_t size_;
  std::vector<value_type> tree_;

 public:
  explicit SegmentTree(size_t size) : size_(size), tree_(size*2) {}

  template <class Iter>
  explicit SegmentTree(Iter begin, Iter end) : SegmentTree(std::distance(begin, end)) {
    if (size_==0) return;
    std::copy(begin, end, tree_.begin() + size_);
    for (size_t i = size_-1; i > 0; i--)
      tree_[i] = tree_[i<<1] * tree_[(i<<1)+1];
  }

  value_type get(size_t index) const {
    return tree_[size_ + index];
  }
  value_type operator[](size_t index) const {
    return get(index);
  }

 private:
  template<class T>
  void _set(size_t index, T&& val) {
    auto i = size_ + index;
    tree_[i] = std::forward<T>(val);
    i >>= 1;
    while (i > 0) {
      tree_[i] = tree_[i<<1] * tree_[(i<<1)+1];
      i >>= 1;
    }
  }
 public:
  template<class T>
  void set(size_t index, T&& val) {
    return _set(index, std::forward<T>(val));
  }
  void set(size_t index, const value_type& val) {
    _set(index, val);
  }
  void set(size_t index, value_type&& val) {
    _set(index, std::move(val));
  }

  value_type query(size_t l, size_t r) const {
    value_type lhs,rhs;
    for (auto _l = l+size_, _r = r+size_; _l < _r; _l>>=1, _r>>=1) {
      if (_l&1) lhs = lhs * tree_[_l++];
      if (_r&1) rhs = tree_[--_r] * rhs;
    }
    return lhs * rhs;
  }

  template<class F>
  size_t max_right(size_t begin, size_t end, F f) const {
    if (begin == end) return end;
    monoid_type p;
    std::stack<std::pair<size_t, monoid_type>> rps;
    auto l = size_ + begin;
    auto r = size_ + end;
    while (l < r and f(p * tree_[l])) {
      if (l&1) p = p * tree_[l++];
      if (r&1) {
        rps.emplace(r, tree_[r-1]);
        r--;
      }
      l>>=1; r>>=1;
    }
    if (l >= r) {
      while (rps.size()) {
        auto& [r, rp] = rps.top();
        if (!f(p * rp)) {
          l = r-1;
          break;
        }
        p = p * rp;
        rps.pop();
      }
      if (rps.empty()) return end;
    }
    while (l < size_) {
      assert(!f(p * tree_[l]));
      l <<= 1;
      if (f(p * tree_[l]))
        p = p * tree_[l++];
    }
    return l - size_;
  }

  template<class F>
  size_t min_left(size_t begin, size_t end, F f) const {
    if (end == begin) return begin;
    monoid_type p;
    std::stack<std::pair<size_t, monoid_type>> lps;
    auto l = size_ + begin;
    auto r = size_ + end;
    while (l < r and f(tree_[r-1] * p)) {
      if (l&1) {
        lps.emplace(l, tree_[l]);
        l++;
      }
      if (r&1) p = tree_[r-1] * p;
      l>>=1; r>>=1;
    }
    if (l >= r) {
      while (lps.size()) {
        auto& [l, lp] = lps.top();
        if (!f(lp * p)) {
          r = l+1;
          break;
        }
        p = lp * p;
        lps.pop();
      }
      if (lps.empty()) return begin;
    }
    while (r <= size_) {
      assert(!f(tree_[r-1] * p));
      r <<= 1;
      if (f(tree_[r-1] * p)) 
        p = tree_[--r] * p;
    }
    return r - size_;
  }
  template<bool (*F)(value_type)>
  size_t min_left(size_t begin, size_t end) const {
    return min_left(begin, [](value_type x) { return F(x); });
  }

};
