#pragma once
#include "bit_manip.hpp"
#include <cstddef>
#include <vector>

template<class NodeContainer>
class SegmentTreeDivideAndConquer {
 private:
  size_t time_, size_;
  std::vector<NodeContainer> tree_;

 public:
  SegmentTreeDivideAndConquer() = default;
  explicit SegmentTreeDivideAndConquer(size_t time) : time_(time), size_(bm::bit_ceil(time)), tree_(size_+time_) {}
  
  template<class Set>
  void set(size_t l, size_t r, Set set) {
    for (auto _l = l+size_, _r = r+size_; _l < _r; _l>>=1, _r>>=1) {
      if (_l&1) set(tree_[_l++]);
      if (_r&1) set(tree_[--_r]);
    }
  }

  template<class F, class Upd, class Undo>
  void dfs(F fn, Upd upd, Undo undo) const {
    _dfs(1, fn, upd, undo);
  }

 private:
  template<class F, class Upd, class Undo>
  void _dfs(size_t u, F fn, Upd upd, Undo undo) const {
    if (u >= size_+time_) return;
    upd(tree_[u]);
    if (u < size_) {
      _dfs(u*2, fn, upd, undo);
      _dfs(u*2+1, fn, upd, undo);
    } else {
      fn(u-size_);
    }
    undo();
  }

};