#pragma once
#include "bit_manip.hpp"
#include <vector>
#include <algorithm>

template <typename T, T (*op)(T, T), T E>
class DualSparseTable {
 private:
  size_t size_, log_n_;
  std::vector<std::vector<T>> table_;

 public:
  DualSparseTable(size_t size) :
      size_(size),
      log_n_(63-bm::clz(size)),
      table_(log_n_+1, std::vector<T>(size, E)) {}
  template <typename Iter>
  DualSparseTable(Iter begin, Iter end) :
      DualSparseTable(std::distance(begin, end)) {
    std::transform(begin, end, table_[0].begin());
  }

/***
 * @brief Apply to [l, r)
 * @note Complexity: O(1)
 */
  void apply(size_t l, size_t r, const T& a) {
    if (l>=r) return;
    auto d = 63-bm::clz(r-l);
    table_[d][l] = op(table_[d][l], a);
    table_[d][r-(1ull<<d)] = op(table_[d][r-(1ull<<d)], a);
  }

/***
 * @brief Build the table
 * @note Complexity: O(n log n)
 */
  void build() {
    for (int log_n = (int)log_n_-1; log_n >= 0; log_n--) {
      size_t width = 1ull<<log_n;
      for (size_t i = 0; i < size_-width; i++) {
        table_[log_n][i] = op(table_[log_n][i], table_[log_n+1][i]);
        table_[log_n][i+width] = op(table_[log_n][i+width], table_[log_n+1][i]);
      }
    }
  }

/***
 * @brief Query of [l, r)
 * @note Complexity: O(1)
 */
  T query(size_t l, size_t r) const {
    size_t p = 63-bm::clz(r-l);
    return op(table_[p][l], table_[p][r-(1ull<<p)]);
  }
  T get(size_t i) const {
    return table_[0][i];
  }

};
