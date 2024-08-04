#pragma once
#include "bit_manip.hpp"
#include <cstddef>
#include <vector>

template <class T, T (*op)(T, T)>
class SparseTable {
 public:
  using value_type = T;
 private:
  size_t size_;
  size_t log_n_;
  std::vector<std::vector<value_type>> table_;

 public:
  SparseTable() = default;
  template <class Iter>
  SparseTable(Iter begin, Iter end) : 
    size_(std::distance(begin, end)), 
    log_n_(63-bm::clz(size_)), 
    table_(log_n_+1, std::vector<value_type>(size_)) 
  {
    std::copy(begin, end, table_[0].begin());
    for (size_t log_n = 1; log_n <= log_n_; log_n++) {
      size_t width = 1ull<<log_n;
      for (size_t i = 0; i + width <= size_; i++) {
        table_[log_n][i] = op(table_[log_n-1][i],
                              table_[log_n-1][i+width/2]);
      }
    }
  }

  value_type query(size_t l, size_t r) const {
    size_t p = 63-bm::clz(r-l);
    return op(table_[p][l], table_[p][r-(1ull<<p)]);
  }
};
