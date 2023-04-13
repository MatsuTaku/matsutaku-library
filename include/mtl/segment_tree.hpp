#pragma once
#include "bit_manip.hpp"
#include <cstddef>
#include <vector>
#if __cplusplus >= 202002L
#include <concepts>

template<typename M>
concept SegmentTreeMonoid = requires (M m) {
  {m * m} -> std::same_as<M>;
};
#endif

template <typename M>
class SegmentTree {
#if __cplusplus >= 202002L
  static_assert(SegmentTreeMonoid<M>);
#endif
 private:
  size_t size_;
  std::vector<M> tree_;

 public:
  explicit SegmentTree(size_t size) : size_(size), tree_(size*2) {}

  template <typename Iter>
  explicit SegmentTree(Iter begin, Iter end) : SegmentTree(end-begin) {
    for (auto it = begin; it != end; ++it)
      tree_[size_ + it - begin] = *it;
    for (size_t i = size_-1; i > 0; i--)
      tree_[i] = tree_[i * 2] * tree_[i * 2 + 1];
  }

  M get(size_t index) const {
    return tree_[size_ + index];
  }
  M operator[](size_t index) const {
    return get(index);
  }

  void set(size_t index, M val) {
    auto i = size_ + index;
    tree_[i] = val;
    i >>= 1;
    while (i > 0) {
      tree_[i] = tree_[i*2] * tree_[i*2+1];
      i >>= 1;
    }
  }

  M query(size_t l, size_t r) const {
    M lhs,rhs;
    for (auto _l = l+size_, _r = r+size_; _l < _r; _l>>=1, _r>>=1) {
      if (_l&1) lhs = lhs * tree_[_l++];
      if (_r&1) rhs = tree_[--_r] * rhs;
    }
    return lhs * rhs;
  }

  template<typename F>
  size_t max_right(size_t begin, F f) const {
    if (begin == size_) return size_;
    M p;
    auto l = begin + size_;
    do {
      while (l % 2 == 0) l >>= 1;
      if (!f(p * tree_[l])) {
        while (l < size_) {
          l = l*2;
          if (f(p * tree_[l])) {
            p = p * tree_[l];
            l++;
          }
        }
        return l - size_;
      }
      p = p * tree_[l];
      l++;
    } while ((l & -l) != l);
    return size_;
  }
  template<bool (*F)(M)>
  size_t max_right(size_t begin) const {
    return find_last(begin, [](M x) { return F(x); });
  }

  template<typename F>
  size_t min_left(size_t end, F f) const {
    if (end == 0) return 0;
    M p;
    auto r = end + size_;
    do {
      r--;
      while (r > 1 and r % 2 == 1) r >>= 1;
      if (!f(tree_[r] * p)) {
        while (r < size_) {
          r = r*2+1;
          if (f(tree_[r] * p)) {
            p = tree_[r] * p;
            r--;
          }
        }
        return r + 1 - size_;
      }
      p = tree_[r] * p;
    } while ((r & -r) != r);
    return 0;
  }
  template<bool (*F)(M)>
  size_t min_left(size_t begin) const {
    return min_left(begin, [](M x) { return F(x); });
  }

};
