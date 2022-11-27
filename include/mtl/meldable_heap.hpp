#pragma once
#include <memory>
#include <cstddef>

template<typename T, typename Cond = less<>>
class MeldableHeap {
 public:
  struct Node;
  using NodePtr = shared_ptr<Node>;
  struct Node {
    T x;
    NodePtr left, right, parent;
  };

 private:
  NodePtr _r;
  size_t _sz;

  friend NodePtr _meld(NodePtr l, NodePtr r) {
    if (l == nullptr) return r;
    if (r == nullptr) return l;
    if (Cond()(l->x, r->x))
      return _meld(r, l);
    if (rand()%2 == 0) {
      l->left = _meld(l->left, r);
      l->left->parent = l;
    } else {
      l->right = _meld(l->right, r);
      l->right->parent = l;
    }
    return l;
  }

 public:
  MeldableHeap() : _r(nullptr), _sz(0) {}
  template<typename It, typename Traits = std::iterator_traits<It>>
  MeldableHeap(It begin, It end) : MeldableHeap() {
    for (It it = begin; it != end; ++it) {
      push(*it);
    }
  }

  MeldableHeap& meld(MeldableHeap& r) {
    _r = _meld(_r, r._r);
    _sz += r.size();
    return *this;
  }

  friend MeldableHeap& meld(MeldableHeap& l, MeldableHeap& r) {
    l.meld(r);
    return l;
  }

  void push(T x) {
    NodePtr u(new Node);
    u->x = x;
    _r = _meld(_r, u);
    _r->parent = nullptr;
    _sz++;
  }

  T top() const { return _r->x; }
  T pop() {
    assert(_r);
    T x = _r->x;
    _r = _meld(_r->left, _r->right);
    if (_r) _r->parent = nullptr;
    _sz--;
    return x;
  }

  size_t size() const { return _sz; }
  bool empty() const { return size() == 0; }
  void clear() {
    _r = nullptr;
    _sz = 0;
  }

};