#pragma once
#include <vector>
#include <cassert>
#include <cstddef>
#include "bit_manip.hpp"

template <typename M, typename A>
class LazySegmentTree {
 private:
  size_t size_;
  std::vector<std::pair<M,A>> tree_;
  std::vector<std::pair<size_t, size_t>> ids_;

 public:
  explicit LazySegmentTree(size_t size) :
      size_(1ull<<(64-bm::clz(size-1))),
      tree_(size_*2) {
    ids_.reserve((64-bm::clz(size-1))*2);
  }

  template <typename Iter>
  explicit LazySegmentTree(Iter begin, Iter end) : LazySegmentTree(end-begin) {
    for (auto it = begin; it != end; ++it) {
      tree_[size_ + it - begin].first = *it;
    }
    for (size_t i = size_-1; i > 0; i--) {
      tree_[i].first = tree_[i*2].first * tree_[i*2+1].first;
    }
  }

  M operator[](size_t index) {
    return query(index, index+1);
  }

  void range_update(size_t l, size_t r, A e) {
    assert(l <= r and r <= size_);
    if (l == r) return;
    _set_ids(l, r);
    for (int i = ids_.size()-1; i >= 0; --i) {
      _propagate(ids_[i].first, ids_[i].second);
    }

    for (size_t _l=l+size_, _r=r+size_, s=1; _l<_r; _l>>=1, _r>>=1, s*=2) {
      if (_l&1) {
        tree_[_l].second *= e;
        ++_l;
      }
      if (_r&1) {
        --_r;
        tree_[_r].second *= e;
      }
    }

    for (auto is : ids_) {
      auto id = is.first;
      auto sz = is.second;
      _propagate(id*2, sz/2);
      _propagate(id*2+1, sz/2);
      tree_[id].first = tree_[id*2].first * tree_[id*2+1].first;
    }
  }

  M query(size_t l, size_t r) {
    _set_ids(l, r);
    for (int i = ids_.size()-1; i >= 0; --i) {
      _propagate(ids_[i].first, ids_[i].second);
    }

    M lhs,rhs;
    for (size_t _l=l+size_, _r=r+size_, s=1; _l<_r; _l>>=1, _r>>=1, s*=2) {
      if (_l&1) {
        _propagate(_l, s);
        lhs = lhs * tree_[_l].first;
        ++_l;
      }
      if (_r&1) {
        --_r;
        _propagate(_r, s);
        rhs = tree_[_r].first * rhs;
      }
    }
    return lhs * rhs;
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
    auto e = tree_[id].second;
    if (e == A()) return;
    tree_[id].second = A();
    tree_[id].first = e.act(tree_[id].first, sz);
    if (id < size_) {
      tree_[id*2].second *= e;
      tree_[id*2+1].second *= e;
    }
  }

};

