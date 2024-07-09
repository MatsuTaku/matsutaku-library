#pragma once
#include "bit_manip.hpp"
#include <vector>
#include <algorithm>

template <typename T, typename Predicate>
class SparseTable {
 public:
  using value_type = T;
 private:
  Predicate predicate_;
  size_t size_;
  size_t log_n_;
  std::vector<std::vector<value_type>> table_;

 public:
  SparseTable() = default;
  template <typename Iter>
  SparseTable(Iter begin, Iter end) {
    size_ = std::distance(begin, end);
    log_n_ = 63-bm::clz(size_);
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

  const value_type& query(size_t l, size_t r) const {
    size_t p = 63-bm::clz(r-l);
    return predicate_(table_[p][l], table_[p][r-(1ull<<p)]);
  }

};

template <class T>
struct _max {
  const T& operator()(const T& a, const T& b) const { return std::max(a,b); }
};
template <class T>
using max_SparseTable = SparseTable<T, _max<T>>;

template <class T>
struct _min {
  const T& operator()(const T& a, const T& b) const { return std::min(a,b); }
};
template <class T>
using min_SparseTable = SparseTable<T, _min<T>>;

