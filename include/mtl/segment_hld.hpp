#pragma once
#include <iostream>
#include "hld.hpp"

template<typename Node>
struct SegmentHldBase {
  int n_;
  std::vector<Node> tree_;
  std::vector<int> target_;
  SegmentHldBase(const Hld& tree) : n_(tree.n), target_(n_) {
    std::vector<size_t> w(n_),cw(n_+1);
    for (int i = 0; i < n_; i++) {
      w[i] = tree.size[i];
      if (!tree.edge[i].empty() and tree.edge[i][0] != tree.par[i])
        w[i] -= tree.size[tree.edge[i][0]];
      cw[i+1] = cw[i] + w[i];
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
      auto mid = upper_bound(cw.begin()+l, cw.begin()+r, (cw[r]+cw[l])/2);
      if (mid != cw.begin()+l and *std::prev(mid)-cw[l] > cw[r]-*mid)
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
};

template<typename M>
struct SegmentHldNode {
  int l,r,p=-1,lc=-1,rc=-1;
  M m;
  int size() const {
    return r-l;
  }
};
template<typename M>
struct SegmentHld : SegmentHldBase<SegmentHldNode<M>> {
  using Node = SegmentHldNode<M>;
  using Base = SegmentHldBase<Node>;
  using Base::n_;
  using Base::tree_;
  using Base::target_;
  SegmentHld(const Hld& tree) : Base(tree) {}
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
struct LazySegmentHldNode {
  int l,r,p=-1,lc=-1,rc=-1;
  M m;
  A a;
  int size() const {
    return r-l;
  }
};
template<typename M, typename A>
struct LazySegmentHld : SegmentHldBase<LazySegmentHldNode<M,A>> {
  using Node = LazySegmentHldNode<M,A>;
  using Base = SegmentHldBase<Node>;
  using Base::n_;
  using Base::tree_;
  using Base::target_;
  LazySegmentHld(const Hld& tree) : Base(tree) {}
  template<typename InputIt>
  LazySegmentHld(const Hld& tree, InputIt begin, InputIt end) : Base(tree) {
    int n = std::distance(begin, end);
    auto it = begin;
    for (int i = 0; i < n; i++, ++it) {
      tree_[target_[i]].m = *it;
    }
    for (int i = tree_.size()-1; i >= 0; i--) {
      if (tree_[i].size() == 1) continue;
      tree_[i].m = tree_[tree_[i].lc].m * tree_[tree_[i].rc].m;
    }
  }
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
  template<typename T>
  void update(int l, int r, const T& v) {
    _update(l, r, v, 0);
  }
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