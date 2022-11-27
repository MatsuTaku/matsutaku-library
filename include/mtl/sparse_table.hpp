#pragma once
#include <vector>
#include "bit_manip.hpp"

template <typename T, typename Predicate>
class SparseTable {
 private:
  Predicate predicate_;
  size_t size_;
  size_t log_n_;
  std::vector<std::vector<T>> table_;

 public:
  template <typename Iter>
  SparseTable(Iter begin, Iter end) {
    size_ = end - begin;
    log_n_ = 63-clz(size_);
    table_.resize(log_n_+1, std::vector<T>(size_));
    {
      for (size_t i = 0; i < size_; i++) {
        table_[0][i] = *(begin+i);
      }
    }
    for (size_t log_n = 1; log_n <= log_n_; log_n++) {
      size_t width = 1ull<<log_n;
      for (size_t i = 0; i + width <= size_; i++) {
        table_[log_n][i] = predicate_(table_[log_n-1][i],
                                      table_[log_n-1][i+width/2]);
      }
    }
  }

  T query(size_t l, size_t r) const {
    size_t p = 63-clz(r-l);
    return predicate_(table_[p][l], table_[p][r-(1ull<<p)]);
  }

};

template <typename T>
struct _max {
  T operator()(T a, T b) const { return std::max(a,b); }
};
template <typename T>
using max_SparseTable = SparseTable<T, _max<T>>;

template <typename T>
struct _min {
  T operator()(T a, T b) const { return std::min(a,b); }
};
template <typename T>
using min_SparseTable = SparseTable<T, _min<T>>;

