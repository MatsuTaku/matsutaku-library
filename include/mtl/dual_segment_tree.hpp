#pragma once
#include "bit_manip.hpp"
#include <cstddef>
#include <vector>
#include <algorithm>
#include <stack>
#include <cassert>
#if __cpp_concepts >= 202002L
#include <concepts>

template<typename M>
concept IdDualSegmentTreeMonoid = requires (M m) {
  {m *= m} -> std::same_as<M&>;
};
#endif

template <
#if __cpp_concepts >= 202002L
  IdDualSegmentTreeMonoid
#else
  class
#endif
    M
>
class DualSegmentTree {
 private:
  size_t size_;
  using tree_type = std::vector<M>;
  tree_type tree_;
  std::vector<size_t> ids_;

  int log(size_t x) const {
    return 64 - bm::clz(x-1);
  }

 public:
  explicit DualSegmentTree(size_t size) :
      size_(size),
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
    _lazy_propagation(l, r);

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

  void set(size_t index, const M& e) {
    assert(index < size_);
    _lazy_propagation(index, index+1);
    tree_[size_ + index] = e;
  }

  M get(size_t index) {
    assert(index < size_);
    _lazy_propagation(index, index+1);
    return tree_[size_ + index];
  }

 private:
  void _set_ids(size_t l, size_t r) {
    ids_.clear();
    auto _l=l+size_, _r=r+size_;
    auto lth = _l/(_l&(-_l))/2;
    auto rth = _r/(_r&(-_r))/2;
    for (; _l<_r; _l>>=1, _r>>=1) {
      if (_r <= rth) ids_.emplace_back(_r);
      if (_l <= lth) ids_.emplace_back(_l);
    }
    for (; _l>0; _l>>=1) {
      ids_.emplace_back(_l);
    }
  }

  void _propagate(size_t id) {
    if (id >= size_) return;
    M e = tree_[id];
    tree_[id] = M();
    tree_[id*2] *= e;
    tree_[id*2+1] *= e;
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

 private:
  template<bool> struct iterator_base;
  template<bool> friend struct iterator_base;
  template<bool Const>
  struct iterator_base {
    using value_type = std::conditional_t<Const, const M, M>;
    using pointer = std::conditional_t<Const, const M*, M*>;
    using reference = std::conditional_t<Const, const M&, M&>;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;
    DualSegmentTree* tree_ptr;
    size_t idx;
    iterator_base(DualSegmentTree* tree_ptr, size_t idx) : tree_ptr(tree_ptr), idx(idx) {
      assert(idx >= tree_ptr->size_ and idx <= tree_ptr->size_*2);
      if (idx < tree_ptr->tree_.size()) tree_ptr->get(idx - tree_ptr->size_);
    }
    iterator_base& operator+=(difference_type n) { 
      auto l = idx;
      idx += n;
      if (idx == tree_ptr->tree_.size()) return *this;
      auto x = l ^ idx;
      int h = bm::ctz(~x);
      --h;
      while (h >= 0) {
        tree_ptr->_propagate(idx>>h);
        --h;
      }
      return *this;
    }
    iterator_base& operator-=(difference_type n) { 
      auto r = idx;
      idx -= n;
      auto x = idx ^ r;
      int h = bm::ctz(~x);
      --h;
      while (h >= 0) {
        tree_ptr->_propagate(idx>>h);
        --h;
      }
      return *this;
    }
    iterator_base& operator++() { 
      return *this += 1;
    }
    iterator_base& operator--() {
      return *this -= 1;
    }
    iterator_base operator++(int) { auto tmp = *this; ++idx; return tmp; }
    iterator_base operator--(int) { auto tmp = *this; --idx; return tmp; }
    iterator_base operator+(difference_type n) const { return iterator_base(*this) += n; }
    iterator_base operator-(difference_type n) const { return iterator_base(*this) -= n; }
    bool operator==(const iterator_base& o) const { return tree_ptr == o.tree_ptr and idx == o.idx; }
    bool operator!=(const iterator_base& o) const { return tree_ptr != o.tree_ptr or idx != o.idx; }
    difference_type operator-(const iterator_base& o) const { return idx - o.idx; }
    reference operator*() const { return tree_ptr->tree_[idx]; }
    pointer operator->() const { return &tree_ptr->tree_[idx]; }
  };
 public:
  using iterator = iterator_base<false>;
  using const_iterator = iterator_base<true>;
  const_iterator begin() const { return const_iterator(this, size_); }
  const_iterator end() const { return const_iterator(this, size_*2); }
  const_iterator cbegin() const { return const_iterator(this, size_); }
  const_iterator cend() const { return const_iterator(this, size_*2); }
  iterator begin() { return iterator(this, size_); }
  iterator end() { return iterator(this, size_*2); }

};

