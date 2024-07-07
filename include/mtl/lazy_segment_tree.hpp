#pragma once
#include "bit_manip.hpp"
// #include "monoid.hpp"
#include <cstddef>
#include <utility>
#include <vector>
#include <stack>
#include <cassert>
#if __cpp_concepts >= 202002L
#include <concepts>

template<typename M>
concept LazySegmentTreeMonoid = requires (M m) {
  {m * m} -> std::same_as<M>;
};
template<typename A, typename M>
concept LazySegmentTreeOperatorMonoid = requires(A a, M m) {
  {a *= a} -> std::same_as<A&>;
  {a.act(m)} -> std::same_as<M>;
};
#endif

template <typename M, typename A>
#if __cpp_concepts >= 202002L
requires LazySegmentTreeMonoid<M> &&
         LazySegmentTreeOperatorMonoid<A,M>
#endif
class LazySegmentTree {
 private:
  size_t size_;
  std::vector<std::pair<M, A>> tree_;
  std::vector<size_t> ids_;

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

  inline void range_update(size_t l, size_t r, const A& e) {
    assert(l <= r);
    assert(r <= size_);
    if (l == r) return;
    _lazy_propagation(l, r);

    for (size_t _l=l+size_, _r=r+size_; _l<_r; _l>>=1, _r>>=1) {
      if (_l&1) 
        tree_[_l++].second *= e;
      if (_r&1) 
        tree_[--_r].second *= e;
    }

    for (auto id : ids_) {
      _propagate(id*2);
      _propagate(id*2+1);
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
    _lazy_propagation(i, i+1);
    int u = i+size_;
    tree_[u].first = M(std::forward<T>(e));
    u >>= 1;
    while (u > 0) {
      tree_[u].first = tree_[u*2].first * tree_[u*2+1].first;
      u >>= 1;
    }
  }

  inline M query(size_t l, size_t r) {
    _lazy_propagation(l, r);

    M lhs,rhs;
    for (size_t _l=l+size_, _r=r+size_; _l<_r; _l>>=1, _r>>=1) {
      if (_l&1) {
        _propagate(_l);
        lhs = lhs * tree_[_l].first;
        ++_l;
      }
      if (_r&1) {
        --_r;
        _propagate(_r);
        rhs = tree_[_r].first * rhs;
      }
    }
    return lhs * rhs;
  }
  /// Alias for query(l, r)
  M prod(size_t l, size_t r) {
    return query(l, r);
  }

  inline const M& get(size_t index) {
    _lazy_propagation(index, index+1);
    auto l = index+size_;
    _propagate(l);
    return tree_[l].first;
  }

 private:
  void _set_ids(size_t l, size_t r) {
    ids_.clear();
    auto _l=l+size_, _r=r+size_;
    auto lth = _l/(_l&(-_l))/2;
    auto rth = _r/(_r&(-_r))/2;
    for (; _l<_r; _l>>=1, _r>>=1) {
      if (_r <= rth) ids_.push_back(_r);
      if (_l <= lth) ids_.push_back(_l);
    }
    for (; _l>0; _l>>=1)
      ids_.push_back(_l);
  }

  inline void _propagate(size_t id) {
    A& e = tree_[id].second;
    tree_[id].first = e.act(tree_[id].first);
    if (id < size_) {
      tree_[id*2].second *= e;
      tree_[id*2+1].second *= e;
    }
    tree_[id].second = A();
  }

  void _lazy_propagation(size_t l, size_t r) {
    if (l == r) return;
    _set_ids(l, r);
    for (auto it = ids_.rbegin(); it != ids_.rend(); ++it)
      _propagate(*it);
  }

 public:
  template<class F>
  size_t max_right(size_t begin, size_t end, F f) {
    if (begin == end) return end;
    M p;
    std::stack<std::pair<size_t, M>> rps;
    auto l = size_ + begin;
    auto r = size_ + end;
    _lazy_propagation(begin, end);
    auto access = [&](size_t i) {
      _propagate(i);
      return tree_[i].first;
    };
    while (l < r and f(p * access(l))) {
      if (l&1) p = p * tree_[l++].first;
      if (r&1) {
        rps.emplace(r, access(r-1));
        r--;
      }
      l>>=1; r>>=1;
    }
    if (l >= r) {
      while (rps.size()) {
        auto& [r, rp] = rps.top();
        if (!f(p * rp)) {
          l = r-1;
          break;
        }
        p = p * rp;
        rps.pop();
      }
      if (rps.empty()) return end;
    }
    while (l < size_) {
      assert(!f(p * access(l)));
      l <<= 1;
      auto pl = access(l);
      if (f(p * pl)) {
        p = p * pl;
        l++;
      }
    }
    return l - size_;
  }
  template<bool (*F)(M)>
  size_t max_right(size_t begin, size_t end) {
    return max_right(begin, end, [](M x) { return F(x); });
  }

  template<class F>
  size_t min_left(size_t begin, size_t end, F f) {
    if (end == begin) return begin;
    M p;
    std::stack<std::pair<size_t, M>> lps;
    auto l = size_ + begin;
    auto r = size_ + end;
    _lazy_propagation(begin, end);
    auto access = [&](size_t i) {
      _propagate(i);
      return tree_[i].first;
    };
    while (l < r and f(access(r-1) * p)) {
      if (l&1) {
        lps.emplace(l, access(l));
        l++;
      }
      if (r&1) p = tree_[r-1].first * p;
      l>>=1; r>>=1;
    }
    if (l >= r) {
      while (lps.size()) {
        auto& [l, lp] = lps.top();
        if (!f(lp * p)) {
          r = l+1;
          break;
        }
        p = lp * p;
        lps.pop();
      }
      if (lps.empty()) return begin;
    }
    while (r <= size_) {
      assert(!f(access(r-1) * p));
      r <<= 1;
      auto pr = access(r-1);
      if (f(pr * p)) {
        p = pr * p;
        --r;
      }
    }
    return r - size_;
  }
  template<bool (*F)(M)>
  size_t min_left(size_t begin, size_t end) {
    return min_left(begin, [](M x) { return F(x); });
  }

};

