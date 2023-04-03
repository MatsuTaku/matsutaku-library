#pragma once
#include "bit_manip.hpp"
#include <cstddef>
#include <vector>

template <typename T>
class FenwickTree {
 private:
  std::vector<T> tree_;

 public:
  FenwickTree() = default;
  explicit FenwickTree(size_t size) : tree_(size+1) {}

  size_t size() const { return tree_.size()-1; }

  template <typename Iter>
  explicit FenwickTree(Iter begin, Iter end) : FenwickTree(end-begin) {
    for (auto it = begin; it != end; ++it)
      add(it-begin, *it);
  }

  void add(size_t index, T x) {
    for (size_t i = index+1; i < tree_.size(); i += i&(-i))
      tree_[i] += x;
  }

  T sum(size_t index) const {
    T sum = 0;
    for (size_t i = index+1; i > 0; i -= i&(-i))
      sum += tree_[i];
    return sum;
  }

  T range_sum(size_t l, size_t r) const {
    auto sl = l > 0 ? sum(l-1) : 0;
    auto sr = r > 0 ? sum(r-1) : 0;
    return sr - sl;
  }

  size_t lower_bound(T _sum) const {
    size_t ret = 0;
    T s = 0;
    for (int k = 63-bm::clz(size()); k >= 0; k--) {
      size_t j = ret | (1ull<<k);
      if (j < tree_.size() and s + tree_[j] < _sum) {
        s += tree_[j];
        ret = j;
      }
    }
    return ret;
  }

};

