#pragma once
#include <memory>
#include <iterator>
#include <cassert>
#include <exception>
#include <random>
#include <iostream>

template<typename T, typename V=void>
class Treap {
 public:
  using key_type = T;
  static constexpr bool kKeyOnly = std::is_same<V, void>::value;
  using mapped_type = typename std::conditional<kKeyOnly, T, V>::type;
  using value_type = typename std::conditional<kKeyOnly, T, std::pair<T, V>>::type;
  using priority_type = uint32_t;
  class iterator;
 private:
  struct Node;
  using node_ptr = std::shared_ptr<Node>;
  using node_weak = std::weak_ptr<Node>;
  struct Node {
    node_ptr left, right;
    node_weak parent;
    priority_type p;
    value_type v;
    explicit Node(priority_type p)
      : left(nullptr), right(nullptr), p(p) {}
    explicit Node(priority_type p, const value_type& v)
        : left(nullptr), right(nullptr), p(p), v(v) {}
    explicit Node(priority_type p, value_type&& v)
        : left(nullptr), right(nullptr), p(p), v(std::forward<value_type>(v)) {}
    template<typename... Args>
    explicit Node(priority_type p, Args&&... args)
        : left(nullptr), right(nullptr), p(p),
          v(std::forward<Args>(args)...) {}
    const T& key() const {
      if constexpr (kKeyOnly)
        return v;
      else
        return v->first;
    }
  };
  node_ptr sentinel_;
  size_t size_;
  std::default_random_engine eng;
  std::uniform_int_distribution<priority_type> dist;

 public:
  Treap() : sentinel_(std::make_shared<Node>(0)), size_(0) {}
  template<typename It>
  explicit Treap(It begin, It end) : Treap() {
    insert(begin, end);
  }
  template<typename I>
  Treap(std::initializer_list<I> list) : Treap(list.begin(), list.end()) {}
 private:
  void _clone(node_ptr u, const node_ptr ru) {
    if (ru->left) {
      u->left = std::make_shared<Node>(ru->left->p, ru->left->v);
      u->left->parent = u;
      _clone(u->left, ru->left);
    }
    if (ru->right) {
      u->right = std::make_shared<Node>(ru->right->p, ru->right->v);
      u->right->parent = u;
      _clone(u->right, ru->right);
    }
  }
 public:
  Treap(const Treap& r) : Treap() {
    _clone(sentinel_, r.sentinel_);
    size_ = r.size_;
  }
  Treap& operator=(const Treap& r) {
    clear();
    _clone(sentinel_, r.sentinel_);
    size_ = r.size_;
    return *this;
  }
  Treap(Treap&& r) noexcept = default;
  Treap& operator=(Treap&& r) noexcept = default;

 private:
  bool _check_connection(node_ptr u) const {
    if (!u)
      return false;
    assert(u != sentinel_ or sentinel_->right == nullptr);
    if (u == sentinel_ and u->right)
      return true;
    if (u->left)  {
      assert(u->left->parent == u);
      if (!(u->left->parent == u))
        return true;
    }
    if (u->right) {
      assert(u->right->parent == u);
      if (!(u->right->parent == u))
        return true;
    }
    if (_check_connection(u->left))
      return true;
    if (_check_connection(u->right))
      return true;
    return false;
  }
  node_ptr _root() const {
    return sentinel_->left;
  }
  priority_type _pick_priority() { return dist(eng); }
  bool _comp_priority(node_ptr u, node_ptr v) const {
    return u->p < v->p;
  }
  void _turn_left(node_ptr u) {
    auto p = u->parent.lock();
    auto r = u->right;
    assert(p);
    assert(r);
    if (p->left == u)
      p->left = r;
    else {
      assert(p->right == u);
      p->right = r;
    }
    r->parent = p;
    auto rl = r->left;
    u->right = rl;
    if (rl) rl->parent = u;
    r->left = u;
    u->parent = r;
  }
  void _turn_right(node_ptr u) {
    auto p = u->parent.lock();
    auto l = u->left;
    assert(p);
    assert(l);
    if (p->left == u)
      p->left = l;
    else {
      assert(p->right == u);
      p->right = l;
    }
    l->parent = p;
    auto lr = l->right;
    u->left = lr;
    if (lr) lr->parent = u;
    l->right = u;
    u->parent = l;
  }
  template<typename Cond>
  void _bubble_up_cond(node_ptr u, Cond cond) {
    while (cond(u)) {
      assert(!u->parent.expired());
      auto p = u->parent.lock();
      assert(p != sentinel_);
      if (p->right == u) {
        _turn_left(p);
      } else {
        assert(p->left == u);
        _turn_right(p);
      }
    }
  }
  void _bubble_up(node_ptr u) {
    _bubble_up_cond(u, [&](node_ptr u) { return _comp_priority(u, u->parent.lock()); });
  }
  void _bubble_up_force(node_ptr u) {
    _bubble_up_cond(u, [&](node_ptr u) { return u->parent.lock() != sentinel_; });
    assert(u->parent.lock() == sentinel_);
    assert(_root() == u);
  }
  void _tricle_down(node_ptr u) {
    while (u->left or u->right) {
      if (!u->right) {
        _turn_right(u);
      } else if (!u->left) {
        _turn_left(u);
      } else if (_comp_priority(u->left, u->right)) {
        _turn_right(u);
      } else {
        _turn_left(u);
      }
    }
  }
  // Used for leaf only (done after _tricle_down)
  void _splice(node_ptr u) {
    assert(!u->left and !u->right);
    auto p = u->parent.lock();
    assert(p);
    if (p->left == u)
      p->left = nullptr;
    else {
      assert(p->right == u);
      p->right = nullptr;
    }
    u->parent.reset();
  }
  // NOTE: value of size_ is broken.
  Treap(node_ptr node) : Treap() {
    sentinel_->left = node;
    if (node)
      node->parent = sentinel_;
  }
  iterator _insert_node_subtree(node_ptr u, node_ptr new_node) {
    auto x = new_node->key();
    while (true) {
      if (u != sentinel_ and x == u->key())
        return iterator(u);
      auto& c = u == sentinel_ or x < u->key() ? u->left : u->right;
      if (!c) {
        c = new_node;
        c->parent = u;
        u = c;
        break;
      } else {
        u = c;
      }
    }
    _bubble_up(u);
    ++size_;
    return iterator(u);
  }
  iterator _insert_node(node_ptr new_node) {
    return _insert_node_subtree(sentinel_, new_node);
  }
  iterator _insert_node_hint(iterator hint, node_ptr new_node) {
    auto x = new_node->key();
    auto u = hint.ptr_;
    if (!u->parent.expired()) {
      auto p = u->parent.lock();
      if (p != sentinel_) {
        T xp = p->key();
        if (xp == x)
          return iterator(p);
        // Check hint is malicious
        if (   (p->left == u and xp < x)
            or (p->right == u and x < xp))
          return _insert_node(new_node);
        //
      }
    }
    return _insert_node_subtree(u, new_node);
  }

 public:
  size_t size() const { return size_; }
  bool empty() const { return _root() == nullptr; }
  void clear() {
    sentinel_->left = nullptr;
    size_ = 0;
  }

  iterator find(T x) const {
    node_ptr u = _root();
    while (u) {
      if (u->key() == x)
        return iterator(u);
      if (x < u->key())
        u = u->left;
      else
        u = u->right;
    }
    return end();
  }
  size_t count(T x) const { return (size_t) (find(x) != end()); }
  iterator lower_bound(T x) const {
    node_ptr u = _root();
    node_ptr lb = sentinel_;
    while (u) {
      if (u->key() == x)
        return iterator(u);
      if (x < u->key()) {
        lb = u;
        u = u->left;
      } else {
        u = u->right;
      }
    }
    return iterator(lb);
  }
  iterator upper_bound(T x) const {
    node_ptr u = _root();
    node_ptr ub = sentinel_;
    while (u) {
      if (x < u->key()) {
        ub = u;
        u = u->left;
      } else {
        u = u->right;
      }
    }
    return iterator(ub);
  }
  iterator successor(T x) const { return upper_bound(x); }
  iterator predecessor(T x) const {
    auto u = _root();
    node_ptr pr = sentinel_;
    while (u) {
      if (x <= u->key()) {
        u = u->left;
      } else {
        pr = u;
        u = u->right;
      }
    }
    return iterator(pr);
  }

 private:
  template<typename... Args>
  node_ptr _create_node(Args&&... args) {
    auto p = _pick_priority();
    return std::make_shared<Node>(p, std::forward<Args>(args)...);
  }
 public:
  template<typename ...Args>
  iterator emplace(Args&&... args) {
    return _insert_node(_create_node(std::forward<Args>(args)...));
  }
  template<typename ...Args>
  iterator emplace_hint(iterator hint, Args&&... args) {
    return _insert_node_hint(hint, _create_node(std::forward<Args>(args)...));
  }
  iterator insert(const value_type& e) {
    return emplace(e);
  }
  iterator insert(value_type&& e) {
    return emplace(std::forward<value_type>(e));
  }
  template<class It>
  void insert(It begin, It end) {
    using traits = std::iterator_traits<It>;
    static_assert(std::is_convertible<typename traits::value_type, value_type>::value, "");
    static_assert(std::is_base_of<std::forward_iterator_tag, typename traits::iterator_category>::value, "");
    for (auto it = begin; it != end; ++it)
      emplace(*it);
  }
  void insert(std::initializer_list<value_type> list) {
    insert(list.begin(), list.end());
  }

  iterator erase(iterator it) {
    if (it == end())
      return end();
    auto u = it.ptr_;
    auto p = u->parent;
    _tricle_down(u);
    auto ret = ++iterator(u);
    _splice(u);
    --size_;
    return ret;
  }
  iterator erase(T x) {
    auto it = lower_bound(x);
    if (it != end() and it.ptr_->key() == x)
      return erase(it);
    else
      return it;
  }
  iterator erase(iterator begin, iterator end) {
    auto _l = split(begin);
    auto _m = split(end);
    return absorb(&_l);
  }

  [[nodiscard]] Treap split(iterator it) {
    // !!! Breaking size_ value !!!
    auto u = it.ptr_;
    auto d = std::make_shared<Node>(0);
    auto lu = u->left;
    d->left = lu;
    if (lu) lu->parent = d;
    u->left = d;
    d->parent = u;
    _bubble_up_force(d);
    auto l = d->left;
    auto r = d->right;
    sentinel_->left = r;
    if (r) r->parent = sentinel_;
    if (l) l->parent.reset();
    return Treap(l);
  }
  iterator absorb(Treap* s) {
    auto it = begin();
    if (!s)
      return it;
    assert(s->empty() or empty() or *--s->end() < *begin());
    auto d = std::make_shared<Node>(0);
    d->left = s->_root();
    if (d->left)
      d->left->parent = d;
    d->right = _root();
    if (d->right)
      d->right->parent = d;
    sentinel_->left = d;
    d->parent = sentinel_;
    s->sentinel_->left = nullptr;
    _tricle_down(d);
    _splice(d);
    size_ += s->size_;
    s->size_ = 0;
    return it;
  }

  class iterator {
   public:
    using value_type = Treap::value_type;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = long long;
    using iterator_category = std::bidirectional_iterator_tag;
   private:
    node_ptr ptr_;
    friend class Treap;
   public:
    explicit iterator(node_ptr ptr) : ptr_(ptr) {}
    bool operator==(const iterator& r) const { return ptr_ == r.ptr_; }
    bool operator!=(const iterator& r) const { return ptr_ != r.ptr_; }
    reference operator*() const { return ptr_->v; }
    pointer operator->() const { return &(ptr_->v); }
    iterator& operator++() {
      auto u = ptr_;
      if (u->right) {
        u = u->right;
        while (u->left)
          u = u->left;
        ptr_ = u;
      } else {
        node_ptr p;
        while (!u->parent.expired() and (p = u->parent.lock())->left != u) {
          u = p;
        }
        assert(!u->parent.expired());
        assert(u->parent.lock()->left == u);
        ptr_ = u->parent.lock();
      }
      return *this;
    }
    iterator operator++(int) {
      iterator ret = *this;
      ++*this;
      return ret;
    }
    iterator& operator--() {
      auto u = ptr_;
      if (u->left) {
        u = u->left;
        while (u->right)
          u = u->right;
        ptr_ = u;
      } else {
        node_ptr p;
        while (!u->parent.expired() and (p = u->parent.lock())->right != u) {
          u = p;
        }
        ptr_ = u->parent.lock();
      }
      return *this;
    }
    iterator operator--(int) {
      iterator ret = *this;
      --*this;
      return ret;
    }
  };
  iterator begin() const {
    auto u = sentinel_;
    while (u->left)
      u = u->left;
    return iterator(u);
  };
  iterator end() const {
    return iterator(sentinel_);
  };
  void print_for_debug(node_ptr u = nullptr, int depth = -1) const {
    if (_root())
      std::cout<<_root()->key()<<std::endl;
    auto show = [&](auto& f, node_ptr u, int d) {
      if (d >= depth)
        return;
      if (!u)
        return;
      std::cout << u->key() << std::endl;
      if (u->left)
        std::cout << u->left->key() << ' ';
      else
        std::cout << "--- ";
      if (u->right)
        std::cout << u->right->key() << std::endl;
      else
        std::cout << "---" << std::endl;
      f(f, u->left, d+1);
      f(f, u->right, d+1);
    };
    if (!u)
      u = _root();
    show(show, u, 0);
    std::cout<<std::endl;
  }

};

template<typename T, typename V>
constexpr bool Treap<T, V>::kKeyOnly;
template<typename T>
using TreapSet = Treap<T, void>;

template<typename T, typename V>
class TreapMap : public Treap<T, V> {
  static_assert(!std::is_same<V, void>::value, "");
  using _base = Treap<T, V>;
 public:
  using typename _base::mapped_type;
  using reference = mapped_type&;
  reference operator[](T x) {
    auto it = _base::lower_bound(x);
    if (it == _base::end() or it.ptr_->key() != x)
      it = _base::emplace_hint(it, x);
    return it->second;
  }
};