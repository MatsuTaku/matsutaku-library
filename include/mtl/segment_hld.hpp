#pragma once
#include "hld.hpp"
#include <cstddef>
#include <cassert>
#if __cplusplus >= 202002L
#include <concepts>

template<typename M>
concept SegmentHldMonoid = requires (M m) {
  {m * m} -> std::same_as<M>;
  {~m} -> std::same_as<M>;
};
template<typename A, typename M>
concept SegmentHldOperatorMonoid = requires (A a, M m) {
  {a()} -> std::same_as<bool>;
  {a *= a} -> std::convertible_to<A>;
  {a.act(m, 1)} -> std::same_as<M>;
};
#endif

template<typename Node>
class SegmentHldBase {
 protected:
  int n_;
  std::vector<Node> tree_;
  std::vector<int> target_;
 public:
  explicit SegmentHldBase(const Hld& tree) : n_(tree.n), target_(n_) {
    std::vector<long long> cw(n_+1);
    for (int i = 0; i < n_; i++) {
      auto w = tree.size[i];
      if (!tree.edge[i].empty() and tree.edge[i][0] != tree.par[i])
        w -= tree.size[tree.edge[i][0]];
      cw[i+1] = cw[i] + w;
    }
    tree_.reserve(n_*2);
    tree_.resize(1);
    tree_[0].l = 0;
    tree_[0].r = n_;
    for (int i = 0; i < (int)tree_.size(); i++) {
      if (tree_[i].size() == 1) {
        target_[tree_[i].l] = i;
        continue;
      }
      auto l = tree_[i].l;
      auto r = tree_[i].r;
      auto mid = upper_bound(cw.begin()+l, cw.begin()+r, (cw[r]+cw[l]+1)/2);
      assert(cw.begin()+l != mid);
      if (*std::prev(mid)-cw[l] > cw[r]-*mid)
        --mid;
      int m = mid-cw.begin();
      if (l < m) {
        tree_[i].lc = tree_.size();
        tree_.emplace_back();
        tree_.back().l = l;
        tree_.back().r = m;
        tree_.back().p = i;
      }
      if (m < r) {
        tree_[i].rc = tree_.size();
        tree_.emplace_back();
        tree_.back().l = m;
        tree_.back().r = r;
        tree_.back().p = i;
      }
    }
  }
  template<typename InputIt>
  explicit SegmentHldBase(const Hld& tree, InputIt begin, InputIt end) : SegmentHldBase(tree) {
      int i = 0;
      for (auto it = begin; it != end; ++it, ++i) {
        tree_[target_[i]].m = *it;
      }
      for (int i = (int)tree_.size()-1; i >= 0; i--) {
        if (tree_[i].size() == 1) continue;
        tree_[i].m = tree_[tree_[i].lc].m * tree_[tree_[i].rc].m;
      }
    }
};

template<typename M>
struct SegmentHldNode {
  using monoid_type = M;
  int l,r,p=-1,lc=-1,rc=-1;
  M m;
  int size() const {
    return r-l;
  }
};
template<typename M>
class SegmentHld : private SegmentHldBase<SegmentHldNode<M>> {
#if __cplusplus >= 202002L
  static_assert(SegmentHldMonoid<M>);
#endif
 private:
  using Node = SegmentHldNode<M>;
  using Base = SegmentHldBase<Node>;
  using Base::n_;
  using Base::tree_;
  using Base::target_;
 public:
  explicit SegmentHld(const Hld& tree) : Base(tree) {}
  template<typename InputIt>
  explicit SegmentHld(const Hld& tree, InputIt begin, InputIt end) : Base(tree, begin, end) {}
  template<typename T>
  void set(int index, T&& v) {
    int i = target_[index];
    tree_[i].m = std::forward<T>(v);
    i = tree_[i].p;
    while (i != -1) {
      tree_[i].m = tree_[tree_[i].lc].m * tree_[tree_[i].rc].m;
      i = tree_[i].p;
    }
  }
  M query(int l, int r) const {
    return _query(l,r,0);
  }
 private:
  M _query(int l, int r, int u) const {
    if (u == -1)
      return M();
    auto _l = tree_[u].l, _r = tree_[u].r;
    if (_r <= l or r <= _l)
      return M();
    if (l <= _l and _r <= r)
      return tree_[u].m;
    return _query(l, r, tree_[u].lc) * _query(l, r, tree_[u].rc);
  }
};


template<typename M, typename A>
struct LazySegmentHldNode : SegmentHldNode<M> {
  using operator_monoid_type = A;
  A a;
};
template<typename M, typename A>
class LazySegmentHld : private SegmentHldBase<LazySegmentHldNode<M,A>> {
#if __cplusplus >= 202002L
  static_assert(SegmentHldMonoid<M>);
  static_assert(SegmentHldOperatorMonoid<A, M>);
#endif
 private:
  using Node = LazySegmentHldNode<M,A>;
  using Base = SegmentHldBase<Node>;
  using Base::n_;
  using Base::tree_;
  using Base::target_;
 public:
  explicit LazySegmentHld(const Hld& tree) : Base(tree) {}
  template<typename InputIt>
  explicit LazySegmentHld(const Hld& tree, InputIt begin, InputIt end) : Base(tree, begin, end) {}
 private:
  inline void _propagate(int u) {
    auto& a = tree_[u].a;
    if (!a()) return;
    tree_[u].m = a.act(tree_[u].m, tree_[u].size());
    if (tree_[u].size() > 1) {
      tree_[tree_[u].lc].a *= a;
      tree_[tree_[u].rc].a *= a;
    }
    tree_[u].a = A();
  }
 public:
  template<typename T>
  void set(int index, T&& v) {
    std::vector<int> ids;
    int u = target_[index];
    ids.push_back(u);
    u = tree_[u].p;
    while (u != -1) {
      ids.push_back(u);
      u = tree_[u].p;
    }
    for (int i = (int)ids.size()-1; i >= 0; i--) {
      _propagate(ids[i]);
    }
    tree_[ids[0]].m = std::forward<T>(v);
    for (int i = 1; i < ids.size(); i++) {
      u = ids[i];
      auto lc = tree_[u].lc, rc = tree_[u].rc;
      auto ac = lc ^ rc ^ ids[i-1];
      _propagate(ac);
      tree_[u].m = tree_[lc].m * tree_[rc].m;
    }
  }
  M query(int l, int r) {
    return _query(l,r,0);
  }
 private:
  M _query(int l, int r, int u) {
    if (u == -1)
      return M();
    auto _l = tree_[u].l, _r = tree_[u].r;
    if (_r <= l or r <= _l)
      return M();
    _propagate(u);
    if (l <= _l and _r <= r) {
      return tree_[u].m;
    } else {
      return _query(l, r, tree_[u].lc) * _query(l, r, tree_[u].rc);
    }
  }
 public:
  template<typename T>
  void update(int l, int r, const T& v) {
    _update(l, r, v, 0);
  }
 private:
  template<typename T>
  void _update(int l, int r, const T& v, int u) {
    if (u == -1)
      return;
    auto _l = tree_[u].l, _r = tree_[u].r;
    if (_r <= l or r <= _l) {
      _propagate(u);
    } else if (l <= _l and _r <= r) {
      tree_[u].a *= v;
      _propagate(u);
    } else {
      _propagate(u);
      if (tree_[u].size() > 1) {
        auto lc = tree_[u].lc, rc = tree_[u].rc;
        _update(l, r, v, lc);
        _update(l, r, v, rc);
        tree_[u].m = tree_[lc].m * tree_[rc].m;
      }
    }
  }
};