#pragma once
#include "bit_manip.hpp"
#include <cstddef>
#include <vector>
#include <cassert>
#if __cplusplus >= 202002L
#include <concepts>

template<typename M>
concept LazySegmentTreeMonoid = requires (M m) {
  {m * m} -> std::same_as<M>;
};
template<typename A, typename M>
concept LazySegmentTreeOperatorMonoid = requires(A a, M m) {
  {a()} -> std::same_as<bool>;
  {a *= a} -> std::same_as<A>;
  {a.act(m, 1)} -> std::same_as<M>;
};
#endif

template <typename M, typename A>
class LazySegmentTree {
#if __cplusplus >= 202002L
  static_assert(LazySegmentTreeMonoid<M>);
  static_assert(LazySegmentTreeOperatorMonoid<A,M>);
#endif
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
  explicit LazySegmentTree(Iter begin, Iter end)
    : LazySegmentTree(std::distance(begin, end)) {
    static_assert(std::is_convertible<typename std::iterator_traits<Iter>::value_type, M>::value, "");
    for (auto it = begin; it != end; ++it) {
      tree_[size_ + it - begin].first = *it;
    }
    for (size_t i = size_-1; i > 0; i--) {
      tree_[i].first = tree_[i*2].first * tree_[i*2+1].first;
    }
  }

  void range_update(size_t l, size_t r, const A& e) {
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
  inline void update(size_t l, size_t r, const A& e) {
    range_update(l, r, e);
  }
  inline void update(size_t i, const A& e) {
    range_update(i, i+1, e);
  }

  template<typename T>
  inline void set(size_t i, T&& e) {
    _set_ids(i,i+1);
    for (long long j = ids_.size()-1; j >= 0; --j)
      _propagate(ids_[j].first, ids_[j].second);
    int u = i+size_;
    tree_[u].first = M(std::forward(e));
    u /= 2;
    while (u > 0) {
      tree_[u].first = tree_[u*2].first * tree_[u*2+1].first;
      u /= 2;
    }
  }

  inline M query(size_t l, size_t r) {
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

  inline M get(size_t index) {
    return query(index, index+1);
  }

 private:
  inline void _set_ids(size_t l, size_t r) {
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

  inline void _propagate(size_t id, size_t sz) {
    A e = tree_[id].second;
    if (!e()) return;
    tree_[id].second = A();
    tree_[id].first = e.act(tree_[id].first, sz);
    if (id < size_) {
      tree_[id*2].second *= e;
      tree_[id*2+1].second *= e;
    }
  }

};

