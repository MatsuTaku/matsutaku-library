#pragma once
#include "bit_manip.hpp"
#include <cstddef>
#include <vector>
#include <algorithm>
#include <cassert>
#if _cplusplus >= 202002L
#include <concept>

template<typename M>
concept DualSegmentTreeMonoid = requires (M m) {
  {m *= m} -> std::same_as<M>;
};
#endif

template <typename M>
class DualSegmentTree {
#if _cplusplus >= 202002L
  static_assert(DualSegmentTreeMonoid<M>);
#endif
 private:
  size_t size_;
  std::vector<M> tree_;
  std::vector<std::pair<size_t, size_t>> ids_;

  int log(size_t x) const {
    return 64 - bm::clz(x-1);
  }

 public:
  explicit DualSegmentTree(size_t size) :
      size_(1ull<<log(size)),
      tree_(size_*2) {
    ids_.reserve(log(size)*2);
  }

  template <typename Iter>
  explicit DualSegmentTree(Iter begin, Iter end)
    : DualSegmentTree(std::distance(begin, end)) {
    static_assert(std::is_convertible<typename std::iterator_traits<Iter>::value_type, M>::value, "");
    std::copy(begin, end, tree_.begin()+size_);
  }

  void update(size_t l, size_t r, const M& e) {
    assert(l <= r and r <= size_);
    if (l == r) return;
    _set_ids(l, r);
    for (int i = ids_.size()-1; i >= 0; --i) 
      _propagate(ids_[i].first, ids_[i].second);

    for (size_t _l=l+size_, _r=r+size_, s=1; _l<_r; _l>>=1, _r>>=1, s*=2) {
      if (_l&1) {
        tree_[_l] *= e;
        ++_l;
      }
      if (_r&1) {
        --_r;
        tree_[_r] *= e;
      }
    }
  }
  void update(size_t i, const M& e) {
    update(i, i+1, e);
  }

  M get(size_t index) {
    assert(index < size_);
    _set_ids(index, index+1);
    for (int i = ids_.size()-1; i >= 0; --i) 
      _propagate(ids_[i].first, ids_[i].second);
    return tree_[size_ + index];
  }

 private:
  void _set_ids(size_t l, size_t r) {
    ids_.clear();
    auto _l=l+size_, _r=r+size_;
    auto lth = _l/(_l&(-_l))/2;
    auto rth = _r/(_r&(-_r))/2;
    size_t s = 1;
    for (; _l<_r; _l>>=1, _r>>=1, s*=2) {
      if (_r <= rth) ids_.emplace_back(_r, s);
      if (_l <= lth) ids_.emplace_back(_l, s);
    }
    for (; _l>0; _l>>=1, s*=2) {
      ids_.emplace_back(_l, s);
    }
  }

  void _propagate(size_t id, size_t sz) {
    if (id >= size_) return;
    M e = tree_[id];
    tree_[id] = M();
    tree_[id*2] *= e;
    tree_[id*2+1] *= e;
  }

};

