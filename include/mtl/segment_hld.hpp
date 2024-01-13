#pragma once
#include "hld.hpp"
#include "monoid.hpp"
#include <cstddef>
#include <cassert>

template<typename Node>
class SegmentHldBase {
 public:
  using monoid_type = typename Node::monoid_type;
 protected:
  int n_;
  std::vector<Node> tree_;
  std::vector<int> target_;
 public:
  explicit SegmentHldBase(const Hld& tree) : n_(tree.n), target_(n_) {
    std::vector<long long> cw(n_+1);
    for (int i = 0; i < n_; i++) {
      int u = tree.rev[i];
      auto w = tree.size[u];
      if (!tree.edge[u].empty() and tree.edge[u][0] != tree.par[u])
        w -= tree.size[tree.edge[u][0]];
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
    using iterator_value_type = typename std::iterator_traits<InputIt>::value_type;
    static_assert(std::is_convertible<iterator_value_type, monoid_type>::value, 
                  "SegmentHldBaseInputIt must be convertible to Monoid");
    int i = 0;
    for (auto it = begin; it != end; ++it, ++i) {
      tree_[target_[i]].set(monoid_type(*it));
    }
    for (int i = (int)tree_.size()-1; i >= 0; i--) {
      if (tree_[i].size() == 1) continue;
      tree_[i].take(tree_[tree_[i].lc], tree_[tree_[i].rc]);
    }
  }
};

template<typename M>
struct SegmentHldNode {
  using monoid_type = M;
  int l,r,p=-1,lc=-1,rc=-1;
  monoid_type m, rm;
  int size() const {
    return r-l;
  }
  void set(const monoid_type& monoid) {
    m = rm = monoid;
  }
  void take(const SegmentHldNode& lhs, const SegmentHldNode& rhs) {
    m = lhs.m * rhs.m;
    rm = rhs.rm * lhs.rm;
  }
};
template<
#if __cpp_concepts >= 202002L
  IsMonoid
#else
  class
#endif
    M>
class SegmentHld : private SegmentHldBase<SegmentHldNode<M>> {
 public:
  using monoid_type = M; 
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
  const monoid_type& get(int index) const {
    return tree_[target_[index]].m;
  }
  const monoid_type& get_reversed(int index) const {
    return tree_[target_[index]].rm;
  }
  template<class... Args>
  void set(int index, Args&&... args) {
    int i = target_[index];
    tree_[i].set(M(std::forward<Args>(args)...));
    i = tree_[i].p;
    while (i != -1) {
      auto lc = tree_[i].lc, rc = tree_[i].rc;
      tree_[i].take(tree_[lc], tree_[rc]);
      i = tree_[i].p;
    }
  }
  M query(int l, int r) const {
    return _query<0>(l,r,0);
  }
  M reverse_query(int l, int r) const {
    return _query<1>(l,r,0);
  }
 private:
  template<bool Reverse>
  M _query(int l, int r, int u) const {
    if (u == -1)
      return M();
    auto _l = tree_[u].l, _r = tree_[u].r;
    if (_r <= l or r <= _l)
      return M();
    if (l <= _l and _r <= r) {
      if constexpr (!Reverse)
        return tree_[u].m;
      else
        return tree_[u].rm;
    }
    auto lc = tree_[u].lc, rc = tree_[u].rc;
    if constexpr (!Reverse)
      return _query<0>(l, r, lc) * _query<0>(l, r, rc);
    else
      return _query<1>(l, r, rc) * _query<1>(l, r, lc);
  }
};


template<typename M, typename A>
struct LazySegmentHldNode : SegmentHldNode<M> {
  using operator_monoid_type = A;
  A a;
};
template<typename M, typename A>
#if __cpp_concepts >= 202002L
requires IsMonoid<M> && IsOperatorMonoid<A, M>
#endif
class LazySegmentHld : private SegmentHldBase<LazySegmentHldNode<M,A>> {
 public:
  using monoid_type = M;
  using operator_monoid_type = A;
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
    auto& n = tree_[u];
    auto& a = n.a;
    if (!a()) return;
    n.m = a.act(n.m);
    n.rm = a.act(n.rm);
    if (n.size() > 1) {
      tree_[n.lc].a *= a;
      tree_[n.rc].a *= a;
    }
    n.a = A();
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
    tree_[ids[0]].set(monoid_type(std::forward<T>(v)));
    for (int i = 1; i < ids.size(); i++) {
      u = ids[i];
      auto lc = tree_[u].lc, rc = tree_[u].rc;
      auto ac = lc ^ rc ^ ids[i-1];
      _propagate(ac);
      tree_[u].take(tree_[lc], tree_[rc]);
    }
  }
  M query(int l, int r) {
    return _query<0>(l,r,0);
  }
  M reverse_query(int l, int r) {
    return _query<1>(l,r,0);
  }
 private:
  template<bool Reverse>
  M _query(int l, int r, int u) {
    if (u == -1)
      return M();
    auto _l = tree_[u].l, _r = tree_[u].r;
    if (_r <= l or r <= _l)
      return M();
    _propagate(u);
    if (l <= _l and _r <= r) {
      if constexpr (!Reverse)
        return tree_[u].m;
      else
        return tree_[u].rm;
    } else {
      if constexpr (!Reverse)
        return _query<0>(l, r, tree_[u].lc) * _query<0>(l, r, tree_[u].rc);
      else
        return _query<1>(l, r, tree_[u].rc) * _query<1>(l, r, tree_[u].lc);
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
        tree_[u].take(tree_[lc], tree_[rc]);
      }
    }
  }
};