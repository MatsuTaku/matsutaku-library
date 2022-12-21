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
  using value_type = typename std::conditional<kKeyOnly,
    T,
    std::pair<T const, V>
    >::type;
  using priority_type = uint32_t;
  struct iterator;
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
    inline const T& key() const {
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
  inline node_ptr _root() const {
    return sentinel_->left;
  }
  inline priority_type _pick_priority() { return dist(eng); }
  inline bool _comp_priority(node_ptr u, node_ptr v) const {
    return u->p < v->p;
  }
  inline void _turn_left(node_ptr u) {
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
  inline void _turn_right(node_ptr u) {
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
  inline void _bubble_up_cond(node_ptr u, Cond cond) {
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
  inline void _bubble_up(node_ptr u) {
    _bubble_up_cond(u, [&](node_ptr u) { return _comp_priority(u, u->parent.lock()); });
  }
  inline void _bubble_up_force(node_ptr u) {
    _bubble_up_cond(u, [&](node_ptr u) { return u->parent.lock() != sentinel_; });
    assert(u->parent.lock() == sentinel_);
    assert(_root() == u);
  }
  inline void _tricle_down(node_ptr u) {
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
  inline void _splice(node_ptr u) {
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
  inline std::pair<iterator, bool> _insert_node_subtree(node_ptr u, node_ptr new_node) {
    auto x = new_node->key();
    while (true) {
      if (u != sentinel_ and x == u->key())
        return std::make_pair(iterator(u), false);
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
    return std::make_pair(iterator(u), true);
  }
  inline std::pair<iterator, bool> _insert_node(node_ptr new_node) {
    return _insert_node_subtree(sentinel_, new_node);
  }
  inline iterator _insert_node_hint(iterator hint, node_ptr new_node) {
    auto x = new_node->key();
    auto u = hint.ptr_;
    if (!u->parent.expired()) {
      auto p = u->parent.lock();
      if (p != sentinel_) {
        T xp = p->key();
        if (xp == x) [[unlikely]]
          return iterator(p);
        // Check hint is malicious
        if (   (p->left == u and xp < x)
            or (p->right == u and x < xp)) [[unlikely]]
          return _insert_node(new_node).first;
        //
      }
    }
    return _insert_node_subtree(u, new_node).first;
  }

 public:
  inline size_t size() const { return size_; } // TODO: split break size
  inline bool empty() const { return _root() == nullptr; }
  inline void clear() {
    sentinel_->left = nullptr;
    size_ = 0;
  }

  inline iterator find(T x) const {
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
  inline size_t count(T x) const { return (size_t) (find(x) != end()); }
  inline iterator lower_bound(T x) const {
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
  inline iterator upper_bound(T x) const {
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
  inline iterator successor(T x) const { return upper_bound(x); }
  inline iterator predecessor(T x) const {
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
  inline node_ptr _create_node(Args&&... args) {
    auto p = _pick_priority();
    return std::make_shared<Node>(p, std::forward<Args>(args)...);
  }
 public:
  template<typename ...Args>
  inline std::pair<iterator, bool> emplace(Args&&... args) {
    return _insert_node(_create_node(std::forward<Args>(args)...));
  }
  template<typename ...Args>
  inline iterator emplace_hint(iterator hint, Args&&... args) {
    return _insert_node_hint(hint, _create_node(std::forward<Args>(args)...));
  }
  template<typename Value>
  inline std::pair<iterator, bool> insert(Value&& value) {
    static_assert(std::is_convertible<Value, value_type>::value, "");
    return emplace(std::forward<Value>(value));
  }
  inline std::pair<iterator, bool> insert(const value_type& e) {
    return emplace(e);
  }
  inline std::pair<iterator, bool> insert(value_type&& e) {
    return emplace(std::move(e));
  }
  template<class It>
  inline void insert(It begin, It end) {
    using traits = std::iterator_traits<It>;
    static_assert(std::is_convertible<typename traits::value_type, value_type>::value, "");
    static_assert(std::is_base_of<std::forward_iterator_tag, typename traits::iterator_category>::value, "");
    for (auto it = begin; it != end; ++it)
      emplace(*it);
  }
  inline void insert(std::initializer_list<value_type> list) {
    insert(list.begin(), list.end());
  }

  inline iterator erase(iterator it) {
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
  inline bool erase(T x) {
    auto it = find(x);
    if (it != end()) {
      erase(it);
      return 1;
    } else {
      return 0;
    }
  }
  inline iterator erase(iterator begin, iterator end) {
    auto _l = split(begin);
    auto _m = split(end);
    return absorb(&_l);
  }

  [[nodiscard]] inline Treap split(iterator it) {
    // !!! Breaking size_ value !!!
    auto u = it.ptr_;
    auto d = std::make_shared<Node>(std::numeric_limits<priority_type>::max());
    auto lu = u->left;
    d->left = lu;
    d->parent = u;
    u->left = d;
    if (lu) lu->parent = d;
    _bubble_up_force(d);
    auto l = d->left;
    auto r = d->right;
    sentinel_->left = r;
    if (r) r->parent = sentinel_;
    if (l) l->parent.reset();
    return Treap(l);
  }
  inline iterator absorb(Treap* s) {
    assert((s and s->empty()) or empty() or *--s->end() < *begin());
    if (s->count(147253) or count(147253)) {
      std::cerr<<"absorb "<<*(s->begin())<<' '<<*begin()<<' '<<*--end()<<std::endl;
    }
    auto it = begin();
    if (!s or s->empty()) return it;
    if (empty()) {
      sentinel_->left = s->_root();
      sentinel_->left->parent = sentinel_;
      size_ = s->size_;
      s->clear();
      return begin();
    }
    auto d = std::make_shared<Node>(0);
    d->left = s->_root();
    d->right = _root();
    d->parent = sentinel_;
    if (d->left)
      d->left->parent = d;
    if (d->right)
      d->right->parent = d;
    sentinel_->left = d;
    _tricle_down(d);
    _splice(d);
    size_ += s->size_;
    s->clear();
    return it;
  }

  struct iterator {
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
    inline bool operator==(const iterator& r) const { return ptr_ == r.ptr_; }
    inline bool operator!=(const iterator& r) const { return ptr_ != r.ptr_; }
    inline reference operator*() const { return ptr_->v; }
    inline pointer operator->() const { return &(ptr_->v); }
    inline iterator& operator++() {
      auto u = ptr_;
      if (u->right) {
        u = u->right;
        while (u->left)
          u = u->left;
        ptr_ = u;
      } else {
        node_ptr p;
        while ((p = u->parent.lock()) and p->left != u) {
          u = p;
        }
        assert(!u->parent.expired());
        assert(u->parent.lock()->left == u);
        ptr_ = u->parent.lock();
      }
      return *this;
    }
    inline iterator operator++(int) {
      iterator ret = *this;
      ++*this;
      return ret;
    }
    inline iterator& operator--() {
      auto u = ptr_;
      if (u->left) {
        u = u->left;
        while (u->right)
          u = u->right;
        ptr_ = u;
      } else {
        node_ptr p;
        while ((p = u->parent.lock()) and p->right != u) {
          u = p;
        }
        ptr_ = u->parent.lock();
      }
      return *this;
    }
    inline iterator operator--(int) {
      iterator ret = *this;
      --*this;
      return ret;
    }
  };
  inline iterator begin() const {
    auto u = sentinel_;
    while (u->left)
      u = u->left;
    return iterator(u);
  };
  inline iterator end() const {
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
  inline reference operator[](const T& x) {
    // TODO
//    return _base::try_emplace(std::move(x)).first->second;
    return _base::insert({x, mapped_type()}).first->second;
  }
  inline reference operator[](T&& x) {
    // TODO
//    return _base::try_emplace(std::move(x)).first->second;
    return _base::insert({std::move(x), mapped_type()}).first->second;
  }
};