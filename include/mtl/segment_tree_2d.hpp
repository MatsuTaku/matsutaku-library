#pragma once
#include "segment_tree.hpp"
#include <cassert>
#include <vector>

// In-memory 2D segment tree.
// It requires enough memory size of O(nm).
template<typename M>
class SegmentTree2D {
 public:
  using seg_type = SegmentTree<M>;
 private:
  size_t n_,m_;
  std::vector<seg_type> tb_;
 public:
  explicit SegmentTree2D(int n, int m) : n_(n), m_(m), tb_(n*2, seg_type(m)) {}
  void set(int i, int j, M val) {
    assert(i >= 0 and i < (int)n_ and j >= 0 and j < (int)m_);
    int x = n_ + i;
    tb_[x].set(j, val);
    x >>= 1;
    while (x > 0) {
      tb_[x].set(j, tb_[x*2][j] * tb_[x*2+1][j]);
      x >>= 1;
    }
  }
  M query(int rl, int cl, int rr, int cr) const {
    assert(rl >= 0 and rl <= (int)n_ and rr >= 0 and rr <= (int)n_);
    assert(cl >= 0 and cl <= (int)m_ and cr >= 0 and cr <= (int)m_);
    M lhs, rhs;
    for (auto l = rl+n_, r = rr+n_; l < r; l>>=1, r>>=1) {
      if (l&1) lhs = lhs * tb_[l++].query(cl,cr);
      if (r&1) rhs = tb_[--r].query(cl,cr) * rhs;
    }
    return lhs * rhs;
  }
};