---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':warning:'
    path: include/mtl/integer_set.hpp
    title: include/mtl/integer_set.hpp
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/treap.hpp\"\n#include <memory>\r\n#include <iterator>\r\
    \n#include <cassert>\r\n#include <exception>\r\n#include <random>\r\n#include\
    \ <iostream>\r\n\r\ntemplate<typename T, typename V=void>\r\nclass Treap {\r\n\
    \ public:\r\n  using key_type = T;\r\n  static constexpr bool kKeyOnly = std::is_same<V,\
    \ void>::value;\r\n  using mapped_type = typename std::conditional<kKeyOnly, T,\
    \ V>::type;\r\n  using value_type = typename std::conditional<kKeyOnly, T, std::pair<T,\
    \ V>>::type;\r\n  using priority_type = uint32_t;\r\n  class iterator;\r\n private:\r\
    \n  struct Node;\r\n  using node_ptr = std::shared_ptr<Node>;\r\n  using node_weak\
    \ = std::weak_ptr<Node>;\r\n  struct Node {\r\n    node_ptr left, right;\r\n \
    \   node_weak parent;\r\n    priority_type p;\r\n    value_type v;\r\n    explicit\
    \ Node(priority_type p)\r\n      : left(nullptr), right(nullptr), p(p) {}\r\n\
    \    explicit Node(priority_type p, const value_type& v)\r\n        : left(nullptr),\
    \ right(nullptr), p(p), v(v) {}\r\n    explicit Node(priority_type p, value_type&&\
    \ v)\r\n        : left(nullptr), right(nullptr), p(p), v(std::forward<value_type>(v))\
    \ {}\r\n    template<typename... Args>\r\n    explicit Node(priority_type p, Args&&...\
    \ args)\r\n        : left(nullptr), right(nullptr), p(p),\r\n          v(std::forward<Args>(args)...)\
    \ {}\r\n    const T& key() const {\r\n      if constexpr (kKeyOnly)\r\n      \
    \  return v;\r\n      else\r\n        return v->first;\r\n    }\r\n  };\r\n  node_ptr\
    \ sentinel_;\r\n  size_t size_;\r\n  std::default_random_engine eng;\r\n  std::uniform_int_distribution<priority_type>\
    \ dist;\r\n\r\n public:\r\n  Treap() : sentinel_(std::make_shared<Node>(0)), size_(0)\
    \ {}\r\n  template<typename It>\r\n  explicit Treap(It begin, It end) : Treap()\
    \ {\r\n    insert(begin, end);\r\n  }\r\n  template<typename I>\r\n  Treap(std::initializer_list<I>\
    \ list) : Treap(list.begin(), list.end()) {}\r\n private:\r\n  void _clone(node_ptr\
    \ u, const node_ptr ru) {\r\n    if (ru->left) {\r\n      u->left = std::make_shared<Node>(ru->left->p,\
    \ ru->left->v);\r\n      u->left->parent = u;\r\n      _clone(u->left, ru->left);\r\
    \n    }\r\n    if (ru->right) {\r\n      u->right = std::make_shared<Node>(ru->right->p,\
    \ ru->right->v);\r\n      u->right->parent = u;\r\n      _clone(u->right, ru->right);\r\
    \n    }\r\n  }\r\n public:\r\n  Treap(const Treap& r) : Treap() {\r\n    _clone(sentinel_,\
    \ r.sentinel_);\r\n    size_ = r.size_;\r\n  }\r\n  Treap& operator=(const Treap&\
    \ r) {\r\n    clear();\r\n    _clone(sentinel_, r.sentinel_);\r\n    size_ = r.size_;\r\
    \n    return *this;\r\n  }\r\n  Treap(Treap&& r) noexcept = default;\r\n  Treap&\
    \ operator=(Treap&& r) noexcept = default;\r\n\r\n private:\r\n  bool _check_connection(node_ptr\
    \ u) const {\r\n    if (!u)\r\n      return false;\r\n    assert(u != sentinel_\
    \ or sentinel_->right == nullptr);\r\n    if (u == sentinel_ and u->right)\r\n\
    \      return true;\r\n    if (u->left)  {\r\n      assert(u->left->parent ==\
    \ u);\r\n      if (!(u->left->parent == u))\r\n        return true;\r\n    }\r\
    \n    if (u->right) {\r\n      assert(u->right->parent == u);\r\n      if (!(u->right->parent\
    \ == u))\r\n        return true;\r\n    }\r\n    if (_check_connection(u->left))\r\
    \n      return true;\r\n    if (_check_connection(u->right))\r\n      return true;\r\
    \n    return false;\r\n  }\r\n  node_ptr _root() const {\r\n    return sentinel_->left;\r\
    \n  }\r\n  priority_type _pick_priority() { return dist(eng); }\r\n  bool _comp_priority(node_ptr\
    \ u, node_ptr v) const {\r\n    return u->p < v->p;\r\n  }\r\n  void _turn_left(node_ptr\
    \ u) {\r\n    auto p = u->parent.lock();\r\n    auto r = u->right;\r\n    assert(p);\r\
    \n    assert(r);\r\n    if (p->left == u)\r\n      p->left = r;\r\n    else {\r\
    \n      assert(p->right == u);\r\n      p->right = r;\r\n    }\r\n    r->parent\
    \ = p;\r\n    auto rl = r->left;\r\n    u->right = rl;\r\n    if (rl) rl->parent\
    \ = u;\r\n    r->left = u;\r\n    u->parent = r;\r\n  }\r\n  void _turn_right(node_ptr\
    \ u) {\r\n    auto p = u->parent.lock();\r\n    auto l = u->left;\r\n    assert(p);\r\
    \n    assert(l);\r\n    if (p->left == u)\r\n      p->left = l;\r\n    else {\r\
    \n      assert(p->right == u);\r\n      p->right = l;\r\n    }\r\n    l->parent\
    \ = p;\r\n    auto lr = l->right;\r\n    u->left = lr;\r\n    if (lr) lr->parent\
    \ = u;\r\n    l->right = u;\r\n    u->parent = l;\r\n  }\r\n  template<typename\
    \ Cond>\r\n  void _bubble_up_cond(node_ptr u, Cond cond) {\r\n    while (cond(u))\
    \ {\r\n      assert(!u->parent.expired());\r\n      auto p = u->parent.lock();\r\
    \n      assert(p != sentinel_);\r\n      if (p->right == u) {\r\n        _turn_left(p);\r\
    \n      } else {\r\n        assert(p->left == u);\r\n        _turn_right(p);\r\
    \n      }\r\n    }\r\n  }\r\n  void _bubble_up(node_ptr u) {\r\n    _bubble_up_cond(u,\
    \ [&](node_ptr u) { return _comp_priority(u, u->parent.lock()); });\r\n  }\r\n\
    \  void _bubble_up_force(node_ptr u) {\r\n    _bubble_up_cond(u, [&](node_ptr\
    \ u) { return u->parent.lock() != sentinel_; });\r\n    assert(u->parent.lock()\
    \ == sentinel_);\r\n    assert(_root() == u);\r\n  }\r\n  void _tricle_down(node_ptr\
    \ u) {\r\n    while (u->left or u->right) {\r\n      if (!u->right) {\r\n    \
    \    _turn_right(u);\r\n      } else if (!u->left) {\r\n        _turn_left(u);\r\
    \n      } else if (_comp_priority(u->left, u->right)) {\r\n        _turn_right(u);\r\
    \n      } else {\r\n        _turn_left(u);\r\n      }\r\n    }\r\n  }\r\n  //\
    \ Used for leaf only (done after _tricle_down)\r\n  void _splice(node_ptr u) {\r\
    \n    assert(!u->left and !u->right);\r\n    auto p = u->parent.lock();\r\n  \
    \  assert(p);\r\n    if (p->left == u)\r\n      p->left = nullptr;\r\n    else\
    \ {\r\n      assert(p->right == u);\r\n      p->right = nullptr;\r\n    }\r\n\
    \    u->parent.reset();\r\n  }\r\n  // NOTE: value of size_ is broken.\r\n  Treap(node_ptr\
    \ node) : Treap() {\r\n    sentinel_->left = node;\r\n    if (node)\r\n      node->parent\
    \ = sentinel_;\r\n  }\r\n  iterator _insert_node_subtree(node_ptr u, node_ptr\
    \ new_node) {\r\n    auto x = new_node->key();\r\n    while (true) {\r\n     \
    \ if (u != sentinel_ and x == u->key())\r\n        return iterator(u);\r\n   \
    \   auto& c = u == sentinel_ or x < u->key() ? u->left : u->right;\r\n      if\
    \ (!c) {\r\n        c = new_node;\r\n        c->parent = u;\r\n        u = c;\r\
    \n        break;\r\n      } else {\r\n        u = c;\r\n      }\r\n    }\r\n \
    \   _bubble_up(u);\r\n    ++size_;\r\n    return iterator(u);\r\n  }\r\n  iterator\
    \ _insert_node(node_ptr new_node) {\r\n    return _insert_node_subtree(sentinel_,\
    \ new_node);\r\n  }\r\n  iterator _insert_node_hint(iterator hint, node_ptr new_node)\
    \ {\r\n    auto x = new_node->key();\r\n    auto u = hint.ptr_;\r\n    if (!u->parent.expired())\
    \ {\r\n      auto p = u->parent.lock();\r\n      if (p != sentinel_) {\r\n   \
    \     T xp = p->key();\r\n        if (xp == x)\r\n          return iterator(p);\r\
    \n        // Check hint is malicious\r\n        if (   (p->left == u and xp <\
    \ x)\r\n            or (p->right == u and x < xp))\r\n          return _insert_node(new_node);\r\
    \n        //\r\n      }\r\n    }\r\n    return _insert_node_subtree(u, new_node);\r\
    \n  }\r\n\r\n public:\r\n  size_t size() const { return size_; }\r\n  bool empty()\
    \ const { return _root() == nullptr; }\r\n  void clear() {\r\n    sentinel_->left\
    \ = nullptr;\r\n    size_ = 0;\r\n  }\r\n\r\n  iterator find(T x) const {\r\n\
    \    node_ptr u = _root();\r\n    while (u) {\r\n      if (u->key() == x)\r\n\
    \        return iterator(u);\r\n      if (x < u->key())\r\n        u = u->left;\r\
    \n      else\r\n        u = u->right;\r\n    }\r\n    return end();\r\n  }\r\n\
    \  size_t count(T x) const { return (size_t) (find(x) != end()); }\r\n  iterator\
    \ lower_bound(T x) const {\r\n    node_ptr u = _root();\r\n    node_ptr lb = sentinel_;\r\
    \n    while (u) {\r\n      if (u->key() == x)\r\n        return iterator(u);\r\
    \n      if (x < u->key()) {\r\n        lb = u;\r\n        u = u->left;\r\n   \
    \   } else {\r\n        u = u->right;\r\n      }\r\n    }\r\n    return iterator(lb);\r\
    \n  }\r\n  iterator upper_bound(T x) const {\r\n    node_ptr u = _root();\r\n\
    \    node_ptr ub = sentinel_;\r\n    while (u) {\r\n      if (x < u->key()) {\r\
    \n        ub = u;\r\n        u = u->left;\r\n      } else {\r\n        u = u->right;\r\
    \n      }\r\n    }\r\n    return iterator(ub);\r\n  }\r\n  iterator successor(T\
    \ x) const { return upper_bound(x); }\r\n  iterator predecessor(T x) const {\r\
    \n    auto u = _root();\r\n    node_ptr pr = sentinel_;\r\n    while (u) {\r\n\
    \      if (x <= u->key()) {\r\n        u = u->left;\r\n      } else {\r\n    \
    \    pr = u;\r\n        u = u->right;\r\n      }\r\n    }\r\n    return iterator(pr);\r\
    \n  }\r\n\r\n private:\r\n  template<typename... Args>\r\n  node_ptr _create_node(Args&&...\
    \ args) {\r\n    auto p = _pick_priority();\r\n    return std::make_shared<Node>(p,\
    \ std::forward<Args>(args)...);\r\n  }\r\n public:\r\n  template<typename ...Args>\r\
    \n  iterator emplace(Args&&... args) {\r\n    return _insert_node(_create_node(std::forward<Args>(args)...));\r\
    \n  }\r\n  template<typename ...Args>\r\n  iterator emplace_hint(iterator hint,\
    \ Args&&... args) {\r\n    return _insert_node_hint(hint, _create_node(std::forward<Args>(args)...));\r\
    \n  }\r\n  iterator insert(const value_type& e) {\r\n    return emplace(e);\r\n\
    \  }\r\n  iterator insert(value_type&& e) {\r\n    return emplace(std::forward<value_type>(e));\r\
    \n  }\r\n  template<class It>\r\n  void insert(It begin, It end) {\r\n    using\
    \ traits = std::iterator_traits<It>;\r\n    static_assert(std::is_convertible<typename\
    \ traits::value_type, value_type>::value, \"\");\r\n    static_assert(std::is_base_of<std::forward_iterator_tag,\
    \ typename traits::iterator_category>::value, \"\");\r\n    for (auto it = begin;\
    \ it != end; ++it)\r\n      emplace(*it);\r\n  }\r\n  void insert(std::initializer_list<value_type>\
    \ list) {\r\n    insert(list.begin(), list.end());\r\n  }\r\n\r\n  iterator erase(iterator\
    \ it) {\r\n    if (it == end())\r\n      return end();\r\n    auto u = it.ptr_;\r\
    \n    auto p = u->parent;\r\n    _tricle_down(u);\r\n    auto ret = ++iterator(u);\r\
    \n    _splice(u);\r\n    --size_;\r\n    return ret;\r\n  }\r\n  iterator erase(T\
    \ x) {\r\n    auto it = lower_bound(x);\r\n    if (it != end() and it.ptr_->key()\
    \ == x)\r\n      return erase(it);\r\n    else\r\n      return it;\r\n  }\r\n\
    \  iterator erase(iterator begin, iterator end) {\r\n    auto _l = split(begin);\r\
    \n    auto _m = split(end);\r\n    return absorb(&_l);\r\n  }\r\n\r\n  [[nodiscard]]\
    \ Treap split(iterator it) {\r\n    // !!! Breaking size_ value !!!\r\n    auto\
    \ u = it.ptr_;\r\n    auto d = std::make_shared<Node>(0);\r\n    auto lu = u->left;\r\
    \n    d->left = lu;\r\n    if (lu) lu->parent = d;\r\n    u->left = d;\r\n   \
    \ d->parent = u;\r\n    _bubble_up_force(d);\r\n    auto l = d->left;\r\n    auto\
    \ r = d->right;\r\n    sentinel_->left = r;\r\n    if (r) r->parent = sentinel_;\r\
    \n    if (l) l->parent.reset();\r\n    return Treap(l);\r\n  }\r\n  iterator absorb(Treap*\
    \ s) {\r\n    auto it = begin();\r\n    if (!s)\r\n      return it;\r\n    assert(s->empty()\
    \ or empty() or *--s->end() < *begin());\r\n    auto d = std::make_shared<Node>(0);\r\
    \n    d->left = s->_root();\r\n    if (d->left)\r\n      d->left->parent = d;\r\
    \n    d->right = _root();\r\n    if (d->right)\r\n      d->right->parent = d;\r\
    \n    sentinel_->left = d;\r\n    d->parent = sentinel_;\r\n    s->sentinel_->left\
    \ = nullptr;\r\n    _tricle_down(d);\r\n    _splice(d);\r\n    size_ += s->size_;\r\
    \n    s->size_ = 0;\r\n    return it;\r\n  }\r\n\r\n  class iterator {\r\n   public:\r\
    \n    using value_type = Treap::value_type;\r\n    using pointer = value_type*;\r\
    \n    using reference = value_type&;\r\n    using difference_type = long long;\r\
    \n    using iterator_category = std::bidirectional_iterator_tag;\r\n   private:\r\
    \n    node_ptr ptr_;\r\n    friend class Treap;\r\n   public:\r\n    explicit\
    \ iterator(node_ptr ptr) : ptr_(ptr) {}\r\n    bool operator==(const iterator&\
    \ r) const { return ptr_ == r.ptr_; }\r\n    bool operator!=(const iterator& r)\
    \ const { return ptr_ != r.ptr_; }\r\n    reference operator*() const { return\
    \ ptr_->v; }\r\n    pointer operator->() const { return &(ptr_->v); }\r\n    iterator&\
    \ operator++() {\r\n      auto u = ptr_;\r\n      if (u->right) {\r\n        u\
    \ = u->right;\r\n        while (u->left)\r\n          u = u->left;\r\n       \
    \ ptr_ = u;\r\n      } else {\r\n        node_ptr p;\r\n        while (!u->parent.expired()\
    \ and (p = u->parent.lock())->left != u) {\r\n          u = p;\r\n        }\r\n\
    \        assert(!u->parent.expired());\r\n        assert(u->parent.lock()->left\
    \ == u);\r\n        ptr_ = u->parent.lock();\r\n      }\r\n      return *this;\r\
    \n    }\r\n    iterator operator++(int) {\r\n      iterator ret = *this;\r\n \
    \     ++*this;\r\n      return ret;\r\n    }\r\n    iterator& operator--() {\r\
    \n      auto u = ptr_;\r\n      if (u->left) {\r\n        u = u->left;\r\n   \
    \     while (u->right)\r\n          u = u->right;\r\n        ptr_ = u;\r\n   \
    \   } else {\r\n        node_ptr p;\r\n        while (!u->parent.expired() and\
    \ (p = u->parent.lock())->right != u) {\r\n          u = p;\r\n        }\r\n \
    \       ptr_ = u->parent.lock();\r\n      }\r\n      return *this;\r\n    }\r\n\
    \    iterator operator--(int) {\r\n      iterator ret = *this;\r\n      --*this;\r\
    \n      return ret;\r\n    }\r\n  };\r\n  iterator begin() const {\r\n    auto\
    \ u = sentinel_;\r\n    while (u->left)\r\n      u = u->left;\r\n    return iterator(u);\r\
    \n  };\r\n  iterator end() const {\r\n    return iterator(sentinel_);\r\n  };\r\
    \n  void print_for_debug(node_ptr u = nullptr, int depth = -1) const {\r\n   \
    \ if (_root())\r\n      std::cout<<_root()->key()<<std::endl;\r\n    auto show\
    \ = [&](auto& f, node_ptr u, int d) {\r\n      if (d >= depth)\r\n        return;\r\
    \n      if (!u)\r\n        return;\r\n      std::cout << u->key() << std::endl;\r\
    \n      if (u->left)\r\n        std::cout << u->left->key() << ' ';\r\n      else\r\
    \n        std::cout << \"--- \";\r\n      if (u->right)\r\n        std::cout <<\
    \ u->right->key() << std::endl;\r\n      else\r\n        std::cout << \"---\"\
    \ << std::endl;\r\n      f(f, u->left, d+1);\r\n      f(f, u->right, d+1);\r\n\
    \    };\r\n    if (!u)\r\n      u = _root();\r\n    show(show, u, 0);\r\n    std::cout<<std::endl;\r\
    \n  }\r\n\r\n};\r\n\r\ntemplate<typename T, typename V>\r\nconstexpr bool Treap<T,\
    \ V>::kKeyOnly;\r\ntemplate<typename T>\r\nusing TreapSet = Treap<T, void>;\r\n\
    \r\ntemplate<typename T, typename V>\r\nclass TreapMap : public Treap<T, V> {\r\
    \n  static_assert(!std::is_same<V, void>::value, \"\");\r\n  using _base = Treap<T,\
    \ V>;\r\n public:\r\n  using typename _base::mapped_type;\r\n  using reference\
    \ = mapped_type&;\r\n  reference operator[](T x) {\r\n    auto it = _base::lower_bound(x);\r\
    \n    if (it == _base::end() or it.ptr_->key() != x)\r\n      it = _base::emplace_hint(it,\
    \ x);\r\n    return it->second;\r\n  }\r\n};\n"
  code: "#pragma once\r\n#include <memory>\r\n#include <iterator>\r\n#include <cassert>\r\
    \n#include <exception>\r\n#include <random>\r\n#include <iostream>\r\n\r\ntemplate<typename\
    \ T, typename V=void>\r\nclass Treap {\r\n public:\r\n  using key_type = T;\r\n\
    \  static constexpr bool kKeyOnly = std::is_same<V, void>::value;\r\n  using mapped_type\
    \ = typename std::conditional<kKeyOnly, T, V>::type;\r\n  using value_type = typename\
    \ std::conditional<kKeyOnly, T, std::pair<T, V>>::type;\r\n  using priority_type\
    \ = uint32_t;\r\n  class iterator;\r\n private:\r\n  struct Node;\r\n  using node_ptr\
    \ = std::shared_ptr<Node>;\r\n  using node_weak = std::weak_ptr<Node>;\r\n  struct\
    \ Node {\r\n    node_ptr left, right;\r\n    node_weak parent;\r\n    priority_type\
    \ p;\r\n    value_type v;\r\n    explicit Node(priority_type p)\r\n      : left(nullptr),\
    \ right(nullptr), p(p) {}\r\n    explicit Node(priority_type p, const value_type&\
    \ v)\r\n        : left(nullptr), right(nullptr), p(p), v(v) {}\r\n    explicit\
    \ Node(priority_type p, value_type&& v)\r\n        : left(nullptr), right(nullptr),\
    \ p(p), v(std::forward<value_type>(v)) {}\r\n    template<typename... Args>\r\n\
    \    explicit Node(priority_type p, Args&&... args)\r\n        : left(nullptr),\
    \ right(nullptr), p(p),\r\n          v(std::forward<Args>(args)...) {}\r\n   \
    \ const T& key() const {\r\n      if constexpr (kKeyOnly)\r\n        return v;\r\
    \n      else\r\n        return v->first;\r\n    }\r\n  };\r\n  node_ptr sentinel_;\r\
    \n  size_t size_;\r\n  std::default_random_engine eng;\r\n  std::uniform_int_distribution<priority_type>\
    \ dist;\r\n\r\n public:\r\n  Treap() : sentinel_(std::make_shared<Node>(0)), size_(0)\
    \ {}\r\n  template<typename It>\r\n  explicit Treap(It begin, It end) : Treap()\
    \ {\r\n    insert(begin, end);\r\n  }\r\n  template<typename I>\r\n  Treap(std::initializer_list<I>\
    \ list) : Treap(list.begin(), list.end()) {}\r\n private:\r\n  void _clone(node_ptr\
    \ u, const node_ptr ru) {\r\n    if (ru->left) {\r\n      u->left = std::make_shared<Node>(ru->left->p,\
    \ ru->left->v);\r\n      u->left->parent = u;\r\n      _clone(u->left, ru->left);\r\
    \n    }\r\n    if (ru->right) {\r\n      u->right = std::make_shared<Node>(ru->right->p,\
    \ ru->right->v);\r\n      u->right->parent = u;\r\n      _clone(u->right, ru->right);\r\
    \n    }\r\n  }\r\n public:\r\n  Treap(const Treap& r) : Treap() {\r\n    _clone(sentinel_,\
    \ r.sentinel_);\r\n    size_ = r.size_;\r\n  }\r\n  Treap& operator=(const Treap&\
    \ r) {\r\n    clear();\r\n    _clone(sentinel_, r.sentinel_);\r\n    size_ = r.size_;\r\
    \n    return *this;\r\n  }\r\n  Treap(Treap&& r) noexcept = default;\r\n  Treap&\
    \ operator=(Treap&& r) noexcept = default;\r\n\r\n private:\r\n  bool _check_connection(node_ptr\
    \ u) const {\r\n    if (!u)\r\n      return false;\r\n    assert(u != sentinel_\
    \ or sentinel_->right == nullptr);\r\n    if (u == sentinel_ and u->right)\r\n\
    \      return true;\r\n    if (u->left)  {\r\n      assert(u->left->parent ==\
    \ u);\r\n      if (!(u->left->parent == u))\r\n        return true;\r\n    }\r\
    \n    if (u->right) {\r\n      assert(u->right->parent == u);\r\n      if (!(u->right->parent\
    \ == u))\r\n        return true;\r\n    }\r\n    if (_check_connection(u->left))\r\
    \n      return true;\r\n    if (_check_connection(u->right))\r\n      return true;\r\
    \n    return false;\r\n  }\r\n  node_ptr _root() const {\r\n    return sentinel_->left;\r\
    \n  }\r\n  priority_type _pick_priority() { return dist(eng); }\r\n  bool _comp_priority(node_ptr\
    \ u, node_ptr v) const {\r\n    return u->p < v->p;\r\n  }\r\n  void _turn_left(node_ptr\
    \ u) {\r\n    auto p = u->parent.lock();\r\n    auto r = u->right;\r\n    assert(p);\r\
    \n    assert(r);\r\n    if (p->left == u)\r\n      p->left = r;\r\n    else {\r\
    \n      assert(p->right == u);\r\n      p->right = r;\r\n    }\r\n    r->parent\
    \ = p;\r\n    auto rl = r->left;\r\n    u->right = rl;\r\n    if (rl) rl->parent\
    \ = u;\r\n    r->left = u;\r\n    u->parent = r;\r\n  }\r\n  void _turn_right(node_ptr\
    \ u) {\r\n    auto p = u->parent.lock();\r\n    auto l = u->left;\r\n    assert(p);\r\
    \n    assert(l);\r\n    if (p->left == u)\r\n      p->left = l;\r\n    else {\r\
    \n      assert(p->right == u);\r\n      p->right = l;\r\n    }\r\n    l->parent\
    \ = p;\r\n    auto lr = l->right;\r\n    u->left = lr;\r\n    if (lr) lr->parent\
    \ = u;\r\n    l->right = u;\r\n    u->parent = l;\r\n  }\r\n  template<typename\
    \ Cond>\r\n  void _bubble_up_cond(node_ptr u, Cond cond) {\r\n    while (cond(u))\
    \ {\r\n      assert(!u->parent.expired());\r\n      auto p = u->parent.lock();\r\
    \n      assert(p != sentinel_);\r\n      if (p->right == u) {\r\n        _turn_left(p);\r\
    \n      } else {\r\n        assert(p->left == u);\r\n        _turn_right(p);\r\
    \n      }\r\n    }\r\n  }\r\n  void _bubble_up(node_ptr u) {\r\n    _bubble_up_cond(u,\
    \ [&](node_ptr u) { return _comp_priority(u, u->parent.lock()); });\r\n  }\r\n\
    \  void _bubble_up_force(node_ptr u) {\r\n    _bubble_up_cond(u, [&](node_ptr\
    \ u) { return u->parent.lock() != sentinel_; });\r\n    assert(u->parent.lock()\
    \ == sentinel_);\r\n    assert(_root() == u);\r\n  }\r\n  void _tricle_down(node_ptr\
    \ u) {\r\n    while (u->left or u->right) {\r\n      if (!u->right) {\r\n    \
    \    _turn_right(u);\r\n      } else if (!u->left) {\r\n        _turn_left(u);\r\
    \n      } else if (_comp_priority(u->left, u->right)) {\r\n        _turn_right(u);\r\
    \n      } else {\r\n        _turn_left(u);\r\n      }\r\n    }\r\n  }\r\n  //\
    \ Used for leaf only (done after _tricle_down)\r\n  void _splice(node_ptr u) {\r\
    \n    assert(!u->left and !u->right);\r\n    auto p = u->parent.lock();\r\n  \
    \  assert(p);\r\n    if (p->left == u)\r\n      p->left = nullptr;\r\n    else\
    \ {\r\n      assert(p->right == u);\r\n      p->right = nullptr;\r\n    }\r\n\
    \    u->parent.reset();\r\n  }\r\n  // NOTE: value of size_ is broken.\r\n  Treap(node_ptr\
    \ node) : Treap() {\r\n    sentinel_->left = node;\r\n    if (node)\r\n      node->parent\
    \ = sentinel_;\r\n  }\r\n  iterator _insert_node_subtree(node_ptr u, node_ptr\
    \ new_node) {\r\n    auto x = new_node->key();\r\n    while (true) {\r\n     \
    \ if (u != sentinel_ and x == u->key())\r\n        return iterator(u);\r\n   \
    \   auto& c = u == sentinel_ or x < u->key() ? u->left : u->right;\r\n      if\
    \ (!c) {\r\n        c = new_node;\r\n        c->parent = u;\r\n        u = c;\r\
    \n        break;\r\n      } else {\r\n        u = c;\r\n      }\r\n    }\r\n \
    \   _bubble_up(u);\r\n    ++size_;\r\n    return iterator(u);\r\n  }\r\n  iterator\
    \ _insert_node(node_ptr new_node) {\r\n    return _insert_node_subtree(sentinel_,\
    \ new_node);\r\n  }\r\n  iterator _insert_node_hint(iterator hint, node_ptr new_node)\
    \ {\r\n    auto x = new_node->key();\r\n    auto u = hint.ptr_;\r\n    if (!u->parent.expired())\
    \ {\r\n      auto p = u->parent.lock();\r\n      if (p != sentinel_) {\r\n   \
    \     T xp = p->key();\r\n        if (xp == x)\r\n          return iterator(p);\r\
    \n        // Check hint is malicious\r\n        if (   (p->left == u and xp <\
    \ x)\r\n            or (p->right == u and x < xp))\r\n          return _insert_node(new_node);\r\
    \n        //\r\n      }\r\n    }\r\n    return _insert_node_subtree(u, new_node);\r\
    \n  }\r\n\r\n public:\r\n  size_t size() const { return size_; }\r\n  bool empty()\
    \ const { return _root() == nullptr; }\r\n  void clear() {\r\n    sentinel_->left\
    \ = nullptr;\r\n    size_ = 0;\r\n  }\r\n\r\n  iterator find(T x) const {\r\n\
    \    node_ptr u = _root();\r\n    while (u) {\r\n      if (u->key() == x)\r\n\
    \        return iterator(u);\r\n      if (x < u->key())\r\n        u = u->left;\r\
    \n      else\r\n        u = u->right;\r\n    }\r\n    return end();\r\n  }\r\n\
    \  size_t count(T x) const { return (size_t) (find(x) != end()); }\r\n  iterator\
    \ lower_bound(T x) const {\r\n    node_ptr u = _root();\r\n    node_ptr lb = sentinel_;\r\
    \n    while (u) {\r\n      if (u->key() == x)\r\n        return iterator(u);\r\
    \n      if (x < u->key()) {\r\n        lb = u;\r\n        u = u->left;\r\n   \
    \   } else {\r\n        u = u->right;\r\n      }\r\n    }\r\n    return iterator(lb);\r\
    \n  }\r\n  iterator upper_bound(T x) const {\r\n    node_ptr u = _root();\r\n\
    \    node_ptr ub = sentinel_;\r\n    while (u) {\r\n      if (x < u->key()) {\r\
    \n        ub = u;\r\n        u = u->left;\r\n      } else {\r\n        u = u->right;\r\
    \n      }\r\n    }\r\n    return iterator(ub);\r\n  }\r\n  iterator successor(T\
    \ x) const { return upper_bound(x); }\r\n  iterator predecessor(T x) const {\r\
    \n    auto u = _root();\r\n    node_ptr pr = sentinel_;\r\n    while (u) {\r\n\
    \      if (x <= u->key()) {\r\n        u = u->left;\r\n      } else {\r\n    \
    \    pr = u;\r\n        u = u->right;\r\n      }\r\n    }\r\n    return iterator(pr);\r\
    \n  }\r\n\r\n private:\r\n  template<typename... Args>\r\n  node_ptr _create_node(Args&&...\
    \ args) {\r\n    auto p = _pick_priority();\r\n    return std::make_shared<Node>(p,\
    \ std::forward<Args>(args)...);\r\n  }\r\n public:\r\n  template<typename ...Args>\r\
    \n  iterator emplace(Args&&... args) {\r\n    return _insert_node(_create_node(std::forward<Args>(args)...));\r\
    \n  }\r\n  template<typename ...Args>\r\n  iterator emplace_hint(iterator hint,\
    \ Args&&... args) {\r\n    return _insert_node_hint(hint, _create_node(std::forward<Args>(args)...));\r\
    \n  }\r\n  iterator insert(const value_type& e) {\r\n    return emplace(e);\r\n\
    \  }\r\n  iterator insert(value_type&& e) {\r\n    return emplace(std::forward<value_type>(e));\r\
    \n  }\r\n  template<class It>\r\n  void insert(It begin, It end) {\r\n    using\
    \ traits = std::iterator_traits<It>;\r\n    static_assert(std::is_convertible<typename\
    \ traits::value_type, value_type>::value, \"\");\r\n    static_assert(std::is_base_of<std::forward_iterator_tag,\
    \ typename traits::iterator_category>::value, \"\");\r\n    for (auto it = begin;\
    \ it != end; ++it)\r\n      emplace(*it);\r\n  }\r\n  void insert(std::initializer_list<value_type>\
    \ list) {\r\n    insert(list.begin(), list.end());\r\n  }\r\n\r\n  iterator erase(iterator\
    \ it) {\r\n    if (it == end())\r\n      return end();\r\n    auto u = it.ptr_;\r\
    \n    auto p = u->parent;\r\n    _tricle_down(u);\r\n    auto ret = ++iterator(u);\r\
    \n    _splice(u);\r\n    --size_;\r\n    return ret;\r\n  }\r\n  iterator erase(T\
    \ x) {\r\n    auto it = lower_bound(x);\r\n    if (it != end() and it.ptr_->key()\
    \ == x)\r\n      return erase(it);\r\n    else\r\n      return it;\r\n  }\r\n\
    \  iterator erase(iterator begin, iterator end) {\r\n    auto _l = split(begin);\r\
    \n    auto _m = split(end);\r\n    return absorb(&_l);\r\n  }\r\n\r\n  [[nodiscard]]\
    \ Treap split(iterator it) {\r\n    // !!! Breaking size_ value !!!\r\n    auto\
    \ u = it.ptr_;\r\n    auto d = std::make_shared<Node>(0);\r\n    auto lu = u->left;\r\
    \n    d->left = lu;\r\n    if (lu) lu->parent = d;\r\n    u->left = d;\r\n   \
    \ d->parent = u;\r\n    _bubble_up_force(d);\r\n    auto l = d->left;\r\n    auto\
    \ r = d->right;\r\n    sentinel_->left = r;\r\n    if (r) r->parent = sentinel_;\r\
    \n    if (l) l->parent.reset();\r\n    return Treap(l);\r\n  }\r\n  iterator absorb(Treap*\
    \ s) {\r\n    auto it = begin();\r\n    if (!s)\r\n      return it;\r\n    assert(s->empty()\
    \ or empty() or *--s->end() < *begin());\r\n    auto d = std::make_shared<Node>(0);\r\
    \n    d->left = s->_root();\r\n    if (d->left)\r\n      d->left->parent = d;\r\
    \n    d->right = _root();\r\n    if (d->right)\r\n      d->right->parent = d;\r\
    \n    sentinel_->left = d;\r\n    d->parent = sentinel_;\r\n    s->sentinel_->left\
    \ = nullptr;\r\n    _tricle_down(d);\r\n    _splice(d);\r\n    size_ += s->size_;\r\
    \n    s->size_ = 0;\r\n    return it;\r\n  }\r\n\r\n  class iterator {\r\n   public:\r\
    \n    using value_type = Treap::value_type;\r\n    using pointer = value_type*;\r\
    \n    using reference = value_type&;\r\n    using difference_type = long long;\r\
    \n    using iterator_category = std::bidirectional_iterator_tag;\r\n   private:\r\
    \n    node_ptr ptr_;\r\n    friend class Treap;\r\n   public:\r\n    explicit\
    \ iterator(node_ptr ptr) : ptr_(ptr) {}\r\n    bool operator==(const iterator&\
    \ r) const { return ptr_ == r.ptr_; }\r\n    bool operator!=(const iterator& r)\
    \ const { return ptr_ != r.ptr_; }\r\n    reference operator*() const { return\
    \ ptr_->v; }\r\n    pointer operator->() const { return &(ptr_->v); }\r\n    iterator&\
    \ operator++() {\r\n      auto u = ptr_;\r\n      if (u->right) {\r\n        u\
    \ = u->right;\r\n        while (u->left)\r\n          u = u->left;\r\n       \
    \ ptr_ = u;\r\n      } else {\r\n        node_ptr p;\r\n        while (!u->parent.expired()\
    \ and (p = u->parent.lock())->left != u) {\r\n          u = p;\r\n        }\r\n\
    \        assert(!u->parent.expired());\r\n        assert(u->parent.lock()->left\
    \ == u);\r\n        ptr_ = u->parent.lock();\r\n      }\r\n      return *this;\r\
    \n    }\r\n    iterator operator++(int) {\r\n      iterator ret = *this;\r\n \
    \     ++*this;\r\n      return ret;\r\n    }\r\n    iterator& operator--() {\r\
    \n      auto u = ptr_;\r\n      if (u->left) {\r\n        u = u->left;\r\n   \
    \     while (u->right)\r\n          u = u->right;\r\n        ptr_ = u;\r\n   \
    \   } else {\r\n        node_ptr p;\r\n        while (!u->parent.expired() and\
    \ (p = u->parent.lock())->right != u) {\r\n          u = p;\r\n        }\r\n \
    \       ptr_ = u->parent.lock();\r\n      }\r\n      return *this;\r\n    }\r\n\
    \    iterator operator--(int) {\r\n      iterator ret = *this;\r\n      --*this;\r\
    \n      return ret;\r\n    }\r\n  };\r\n  iterator begin() const {\r\n    auto\
    \ u = sentinel_;\r\n    while (u->left)\r\n      u = u->left;\r\n    return iterator(u);\r\
    \n  };\r\n  iterator end() const {\r\n    return iterator(sentinel_);\r\n  };\r\
    \n  void print_for_debug(node_ptr u = nullptr, int depth = -1) const {\r\n   \
    \ if (_root())\r\n      std::cout<<_root()->key()<<std::endl;\r\n    auto show\
    \ = [&](auto& f, node_ptr u, int d) {\r\n      if (d >= depth)\r\n        return;\r\
    \n      if (!u)\r\n        return;\r\n      std::cout << u->key() << std::endl;\r\
    \n      if (u->left)\r\n        std::cout << u->left->key() << ' ';\r\n      else\r\
    \n        std::cout << \"--- \";\r\n      if (u->right)\r\n        std::cout <<\
    \ u->right->key() << std::endl;\r\n      else\r\n        std::cout << \"---\"\
    \ << std::endl;\r\n      f(f, u->left, d+1);\r\n      f(f, u->right, d+1);\r\n\
    \    };\r\n    if (!u)\r\n      u = _root();\r\n    show(show, u, 0);\r\n    std::cout<<std::endl;\r\
    \n  }\r\n\r\n};\r\n\r\ntemplate<typename T, typename V>\r\nconstexpr bool Treap<T,\
    \ V>::kKeyOnly;\r\ntemplate<typename T>\r\nusing TreapSet = Treap<T, void>;\r\n\
    \r\ntemplate<typename T, typename V>\r\nclass TreapMap : public Treap<T, V> {\r\
    \n  static_assert(!std::is_same<V, void>::value, \"\");\r\n  using _base = Treap<T,\
    \ V>;\r\n public:\r\n  using typename _base::mapped_type;\r\n  using reference\
    \ = mapped_type&;\r\n  reference operator[](T x) {\r\n    auto it = _base::lower_bound(x);\r\
    \n    if (it == _base::end() or it.ptr_->key() != x)\r\n      it = _base::emplace_hint(it,\
    \ x);\r\n    return it->second;\r\n  }\r\n};"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/treap.hpp
  requiredBy:
  - include/mtl/integer_set.hpp
  timestamp: '2022-12-19 16:09:09+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/treap.hpp
layout: document
redirect_from:
- /library/include/mtl/treap.hpp
- /library/include/mtl/treap.hpp.html
title: include/mtl/treap.hpp
---
