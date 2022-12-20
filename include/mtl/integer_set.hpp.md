---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':warning:'
    path: include/mtl/treap.hpp
    title: include/mtl/treap.hpp
  _extendedRequiredBy: []
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
    \ void>::value;\r\n  using value_type = typename std::conditional<kKeyOnly, T,\
    \ V>::type;\r\n  using element_type = typename std::conditional<kKeyOnly, T, std::pair<T,\
    \ V>>::type;\r\n  using priority_type = uint32_t;\r\n  class iterator;\r\n private:\r\
    \n  struct Node;\r\n  using node_ptr = std::shared_ptr<Node>;\r\n  using node_weak\
    \ = std::weak_ptr<Node>;\r\n  struct Node {\r\n    node_ptr left, right;\r\n \
    \   node_weak parent;\r\n    priority_type p;\r\n    element_type v;\r\n    explicit\
    \ Node(priority_type p)\r\n      : left(nullptr), right(nullptr), p(p) {}\r\n\
    \    explicit Node(priority_type p, T v)\r\n      : left(nullptr), right(nullptr),\
    \ p(p), v(v) {}\r\n    template<typename... Args>\r\n    explicit Node(priority_type\
    \ p, Args&&... args)\r\n        : left(nullptr), right(nullptr), p(p),\r\n   \
    \       v(std::forward<Args>(args)...) {}\r\n    T key() const {\r\n      if constexpr\
    \ (kKeyOnly)\r\n        return v;\r\n      else\r\n        return v->first;\r\n\
    \    }\r\n  };\r\n  node_ptr sentinel_;\r\n  size_t size_;\r\n  std::default_random_engine\
    \ eng;\r\n  std::uniform_int_distribution<priority_type> dist;\r\n\r\n public:\r\
    \n  Treap() : sentinel_(std::make_shared<Node>(0)), size_(0) {}\r\n  template<typename\
    \ It>\r\n  explicit Treap(It begin, It end) : Treap() {\r\n    insert(begin, end);\r\
    \n  }\r\n  template<typename I>\r\n  Treap(std::initializer_list<I> list) : Treap(list.begin(),\
    \ list.end()) {}\r\n  Treap(const Treap& r) : Treap() {\r\n    clear();\r\n  \
    \  insert(r.begin(), r.end());\r\n  }\r\n  Treap& operator=(const Treap& r) {\r\
    \n    _del_node(_root());\r\n    sentinel_->left = nullptr;\r\n    size_ = 0;\r\
    \n    insert(r.begin(), r.end());\r\n    return *this;\r\n  }\r\n  Treap(Treap&&\
    \ r) : Treap() {\r\n    sentinel_ = std::move(r.sentinel_);\r\n    size_ = std::move(r.size_);\r\
    \n  }\r\n  Treap& operator=(Treap&& r) {\r\n    _del_node(_root());\r\n    sentinel_->left\
    \ = nullptr;\r\n    sentinel_ = std::move(r.sentinel_);\r\n    size_ = std::move(r.size_);\r\
    \n    return *this;\r\n  }\r\n  ~Treap() {\r\n    _del();\r\n    sentinel_ = nullptr;\r\
    \n  }\r\n\r\n private:\r\n  bool _check_connection(node_ptr u) const {\r\n   \
    \ if (!u)\r\n      return false;\r\n    assert(u != sentinel_ or sentinel_->right\
    \ == nullptr);\r\n    if (u == sentinel_ and u->right)\r\n      return true;\r\
    \n    if (u->left)  {\r\n      assert(u->left->parent == u);\r\n      if (!(u->left->parent\
    \ == u))\r\n        return true;\r\n    }\r\n    if (u->right) {\r\n      assert(u->right->parent\
    \ == u);\r\n      if (!(u->right->parent == u))\r\n        return true;\r\n  \
    \  }\r\n    if (_check_connection(u->left))\r\n      return true;\r\n    if (_check_connection(u->right))\r\
    \n      return true;\r\n    return false;\r\n  }\r\n  node_ptr _root() const {\r\
    \n    return sentinel_->left;\r\n  }\r\n  priority_type _pick_priority() { return\
    \ dist(eng); }\r\n  bool _comp_priority(node_ptr u, node_ptr v) const {\r\n  \
    \  return u->p < v->p;\r\n  }\r\n  void _turn_left(node_ptr u) {\r\n    auto p\
    \ = u->parent.lock();\r\n    auto r = u->right;\r\n    assert(p);\r\n    assert(r);\r\
    \n    if (p->left == u)\r\n      p->left = r;\r\n    else {\r\n      assert(p->right\
    \ == u);\r\n      p->right = r;\r\n    }\r\n    r->parent = p;\r\n    auto rl\
    \ = r->left;\r\n    u->right = rl;\r\n    if (rl) rl->parent = u;\r\n    r->left\
    \ = u;\r\n    u->parent = r;\r\n  }\r\n  void _turn_right(node_ptr u) {\r\n  \
    \  auto p = u->parent.lock();\r\n    auto l = u->left;\r\n    assert(p);\r\n \
    \   assert(l);\r\n    if (p->left == u)\r\n      p->left = l;\r\n    else {\r\n\
    \      assert(p->right == u);\r\n      p->right = l;\r\n    }\r\n    l->parent\
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
    \    u->parent.reset();\r\n  }\r\n  Treap(node_ptr node) : Treap() {\r\n    sentinel_->left\
    \ = node;\r\n    if (node)\r\n      node->parent = sentinel_;\r\n  }\r\n  void\
    \ _del_node(node_ptr u) {\r\n    if (!u)\r\n      return;\r\n    _del_node(u->left);\r\
    \n    u->left = nullptr;\r\n    _del_node(u->right);\r\n    u->right = nullptr;\r\
    \n    u->parent.reset();\r\n  }\r\n  void _del() {\r\n    _del_node(sentinel_);\r\
    \n    sentinel_ = nullptr;\r\n  }\r\n  iterator _insert_node_subtree(node_ptr\
    \ u, node_ptr new_node) {\r\n    auto x = new_node->key();\r\n    while (true)\
    \ {\r\n      if (u != sentinel_ and x == u->key())\r\n        return iterator(u);\r\
    \n      auto& c = u == sentinel_ or x < u->key() ? u->left : u->right;\r\n   \
    \   if (!c) {\r\n        c = new_node;\r\n        c->parent = u;\r\n        u\
    \ = c;\r\n        break;\r\n      } else {\r\n        u = c;\r\n      }\r\n  \
    \  }\r\n    _bubble_up(u);\r\n    ++size_;\r\n    return iterator(u);\r\n  }\r\
    \n  iterator _insert_node(node_ptr new_node) {\r\n    return _insert_node_subtree(sentinel_,\
    \ new_node);\r\n  }\r\n  iterator _insert_node_hint(iterator hint, node_ptr new_node)\
    \ {\r\n    auto x = new_node->key();\r\n    auto u = hint.ptr_;\r\n    if (!u->parent.expired())\
    \ {\r\n      auto p = u->parent.lock();\r\n      if (p != sentinel_) {\r\n   \
    \     T xp = p->key();\r\n        if (xp == x)\r\n          return iterator(p);\r\
    \n        // Check hint is malicious\r\n        if (   (p->left == u and xp <\
    \ x)\r\n            or (p->right == u and x < xp))\r\n          return _insert_node(new_node);\r\
    \n        //\r\n      }\r\n    }\r\n    return _insert_node_subtree(u, new_node);\r\
    \n  }\r\n\r\n public:\r\n  size_t size() const { return size_; }\r\n  bool empty()\
    \ const { return _root() == nullptr; }\r\n  void clear() {\r\n    _del_node(_root());\r\
    \n    sentinel_->left = nullptr;\r\n    size_ = 0;\r\n  }\r\n\r\n  iterator find(T\
    \ x) const {\r\n    node_ptr u = _root();\r\n    if (!u)\r\n      return end();\r\
    \n    while (u) {\r\n      if (u->key() == x)\r\n        return iterator(u);\r\
    \n      if (x < u->key())\r\n        u = u->left;\r\n      else\r\n        u =\
    \ u->right;\r\n    }\r\n    return end();\r\n  }\r\n  size_t count(T x) const\
    \ { return (size_t) (find(x) != end()); }\r\n  iterator lower_bound(T x) const\
    \ {\r\n    node_ptr u = _root();\r\n    node_ptr lb = sentinel_;\r\n    while\
    \ (u) {\r\n      if (u->key() == x)\r\n        return iterator(u);\r\n      if\
    \ (x < u->key()) {\r\n        lb = u;\r\n        u = u->left;\r\n      } else\
    \ {\r\n        u = u->right;\r\n      }\r\n    }\r\n    return iterator(lb);\r\
    \n  }\r\n  iterator upper_bound(T x) const {\r\n    node_ptr u = _root();\r\n\
    \    node_ptr ub = sentinel_;\r\n    while (u) {\r\n      if (x < u->key()) {\r\
    \n        ub = u;\r\n        u = u->left;\r\n      } else {\r\n        u = u->right;\r\
    \n      }\r\n    }\r\n    return iterator(ub);\r\n  }\r\n  iterator successor(T\
    \ x) const { return upper_bound(x); }\r\n  iterator predecessor(T x) const {\r\
    \n    auto u = _root();\r\n    node_ptr pr = sentinel_;\r\n    while (u) {\r\n\
    \      if (x <= u->key()) {\r\n        u = u->left;\r\n      } else {\r\n    \
    \    pr = u;\r\n        u = u->right;\r\n      }\r\n    }\r\n    return iterator(pr);\r\
    \n  }\r\n\r\n  iterator insert(const element_type& e) {\r\n    auto p = _pick_priority();\r\
    \n    auto new_node = std::make_shared<Node>(p, e);\r\n    return _insert_node(new_node);\r\
    \n  }\r\n  template<class It>\r\n  void insert(It begin, It end) {\r\n    using\
    \ traits = std::iterator_traits<It>;\r\n    static_assert(std::is_convertible<typename\
    \ traits::value_type, element_type>::value, \"\");\r\n    static_assert(std::is_base_of<std::forward_iterator_tag,\
    \ typename traits::iterator_category>::value, \"\");\r\n    if (begin == end)\r\
    \n      return;\r\n    It it = begin;\r\n    auto hint = insert(*(it++));\r\n\
    \    while (it != end)\r\n      hint = emplace_hint(hint, *(it++));\r\n  }\r\n\
    \  void insert(std::initializer_list<element_type> list) {\r\n    insert(list.begin(),\
    \ list.end());\r\n  }\r\n  template<class ...Args>\r\n  iterator emplace(Args&&...\
    \ args) {\r\n    auto p = _pick_priority();\r\n    auto new_node = std::make_shared<Node>(p,\
    \ std::forward<Args>(args)...);\r\n    return _insert_node(new_node);\r\n  }\r\
    \n  template<class ...Args>\r\n  iterator emplace_hint(iterator hint, Args&&...\
    \ args) {\r\n    auto p = _pick_priority();\r\n    auto new_node = std::make_shared<Node>(p,\
    \ std::forward<Args>(args)...);\r\n    return _insert_node_hint(hint, new_node);\r\
    \n  }\r\n\r\n  iterator erase(iterator it) {\r\n    if (it == end())\r\n     \
    \ return end();\r\n    auto u = it.ptr_;\r\n    auto p = u->parent;\r\n    _tricle_down(u);\r\
    \n    auto ret = ++iterator(u);\r\n    _splice(u);\r\n    --size_;\r\n    return\
    \ ret;\r\n  }\r\n  iterator erase(const T& x) {\r\n    auto it = lower_bound(x);\r\
    \n    if (it != end() and it.ptr_->key() == x)\r\n      return erase(it);\r\n\
    \    else\r\n      return it;\r\n  }\r\n  iterator erase(iterator begin, iterator\
    \ end) {\r\n    auto _l = split(begin);\r\n    auto _m = split(end);\r\n    return\
    \ absorb(&_l);\r\n  }\r\n\r\n  [[nodiscard]] Treap split(iterator it) {\r\n  \
    \  // !!! Breaking size_ value !!!\r\n    auto u = it.ptr_;\r\n    auto d = std::make_shared<Node>(0);\r\
    \n    auto lu = u->left;\r\n    d->left = lu;\r\n    if (lu) lu->parent = d;\r\
    \n    u->left = d;\r\n    d->parent = u;\r\n    _bubble_up_force(d);\r\n    auto\
    \ l = d->left;\r\n    auto r = d->right;\r\n    sentinel_->left = r;\r\n    if\
    \ (r) r->parent = sentinel_;\r\n    if (l) l->parent.reset();\r\n    return Treap(l);\r\
    \n  }\r\n  iterator absorb(Treap* s) {\r\n    auto it = begin();\r\n    if (!s)\r\
    \n      return it;\r\n    assert(s->empty() or empty() or *--s->end() < *begin());\r\
    \n    auto d = std::make_shared<Node>(0);\r\n    d->left = s->_root();\r\n   \
    \ if (d->left)\r\n      d->left->parent = d;\r\n    d->right = _root();\r\n  \
    \  if (d->right)\r\n      d->right->parent = d;\r\n    sentinel_->left = d;\r\n\
    \    d->parent = sentinel_;\r\n    s->sentinel_->left = nullptr;\r\n    _tricle_down(d);\r\
    \n    _splice(d);\r\n    size_ += s->size_;\r\n    s->size_ = 0;\r\n    return\
    \ it;\r\n  }\r\n\r\n  class iterator {\r\n   public:\r\n    using value_type =\
    \ T;\r\n    using pointer = T*;\r\n    using reference = T&;\r\n    using difference_type\
    \ = long long;\r\n    using iterator_category = std::bidirectional_iterator_tag;\r\
    \n   private:\r\n    node_ptr ptr_;\r\n    friend class Treap;\r\n   public:\r\
    \n    explicit iterator(node_ptr ptr) : ptr_(ptr) {}\r\n    bool operator==(const\
    \ iterator& r) const { return ptr_ == r.ptr_; }\r\n    bool operator!=(const iterator&\
    \ r) const { return ptr_ != r.ptr_; }\r\n    reference operator*() const { return\
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
    \n  }\r\n\r\n\r\n};\r\ntemplate<typename T, typename V>\r\nconstexpr bool Treap<T,\
    \ V>::kKeyOnly;\r\ntemplate<typename T>\r\nusing TreapSet = Treap<T, void>;\r\n\
    \r\n\r\ntemplate<typename T, typename V>\r\nclass TreapMap : public Treap<T, V>\
    \ {\r\n  using _base = Treap<T, V>;\r\n public:\r\n  using typename _base::value_type;\r\
    \n  using _base::kKeyOnly;\r\n  using reference = value_type&;\r\n  reference\
    \ operator[](T x) {\r\n    auto it = _base::lower_bound(x);\r\n    if (it == _base::end()\
    \ or it.ptr_->key() != x)\r\n      it = _base::emplace_hint(it, x);\r\n    if\
    \ constexpr (kKeyOnly)\r\n      return *it;\r\n    else\r\n      return it->second;\r\
    \n  }\r\n};\n#line 2 \"include/mtl/bit_manip.hpp\"\n#include <cstdint>\n#line\
    \ 4 \"include/mtl/bit_manip.hpp\"\n\nnamespace bm {\n\ninline constexpr uint64_t\
    \ popcnt_e8(uint64_t x) {\n  x = (x & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);\n\
    \  x = (x & 0x3333333333333333) + ((x>>2) & 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F)\
    \ + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n  return x;\n}\n// Count 1s\ninline constexpr\
    \ unsigned popcnt(uint64_t x) {\n  return (popcnt_e8(x) * 0x0101010101010101)\
    \ >> 56;\n}\n// Count trailing 0s. ...01101000 -> 3\ninline constexpr unsigned\
    \ ctz(uint64_t x) {\n  return popcnt((x & (-x)) - 1);\n}\ninline constexpr unsigned\
    \ ctz8(uint8_t x) {\n  return x == 0 ? 8 : popcnt_e8((x & (-x)) - 1);\n}\n// [00..0](8bit)\
    \ -> 0, [**..*](not only 0) -> 1\ninline constexpr uint8_t summary(uint64_t x)\
    \ {\n  constexpr uint64_t hmask = 0x8080808080808080ull;\n  constexpr uint64_t\
    \ lmask = 0x7F7F7F7F7F7F7F7Full;\n  auto a = x & hmask;\n  auto b = x & lmask;\n\
    \  b = hmask - b;\n  b = ~b;\n  auto c = (a | b) & hmask;\n  c *= 0x0002040810204081ull;\n\
    \  return uint8_t(c >> 56);\n}\n// Extract target area of bits\ninline constexpr\
    \ uint64_t bextr(uint64_t x, unsigned start, unsigned len) {\n  uint64_t mask\
    \ = len < 64 ? (1ull<<len)-1 : 0xFFFFFFFFFFFFFFFFull;\n  return (x >> start) &\
    \ mask;\n}\n// 00101101 -> 00111111 -count_1s-> 6\ninline constexpr unsigned log2p1(uint8_t\
    \ x) {\n  if (x & 0x80)\n    return 8;\n  uint64_t p = uint64_t(x) * 0x0101010101010101ull;\n\
    \  p -= 0x8040201008040201ull;\n  p = ~p & 0x8080808080808080ull;\n  p = (p >>\
    \ 7) * 0x0101010101010101ull;\n  p >>= 56;\n  return p;\n}\n// 00101100 -mask_mssb->\
    \ 00100000 -to_index-> 5\ninline constexpr unsigned mssb8(uint8_t x) {\n  assert(x\
    \ != 0);\n  return log2p1(x) - 1;\n}\n// 00101100 -mask_lssb-> 00000100 -to_index->\
    \ 2\ninline constexpr unsigned lssb8(uint8_t x) {\n  assert(x != 0);\n  return\
    \ popcnt_e8((x & -x) - 1);\n}\n// Count leading 0s. 00001011... -> 4\ninline constexpr\
    \ unsigned clz(uint64_t x) {\n  if (x == 0)\n    return 64;\n  auto i = mssb8(summary(x));\n\
    \  auto j = mssb8(bextr(x, 8 * i, 8));\n  return 63 - (8 * i + j);\n}\ninline\
    \ constexpr unsigned clz8(uint8_t x) {\n  return x == 0 ? 8 : 7 - mssb8(x);\n\
    }\ninline constexpr uint64_t bit_reverse(uint64_t x) {\n  x = ((x & 0x00000000FFFFFFFF)\
    \ << 32) | ((x & 0xFFFFFFFF00000000) >> 32);\n  x = ((x & 0x0000FFFF0000FFFF)\
    \ << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);\n  x = ((x & 0x00FF00FF00FF00FF)\
    \ << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);\n  x = ((x & 0x0F0F0F0F0F0F0F0F) <<\
    \ 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);\n  x = ((x & 0x3333333333333333) << 2)\
    \ | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);\n  x = ((x & 0x5555555555555555) << 1) |\
    \ ((x & 0xAAAAAAAAAAAAAAAA) >> 1);\n  return x;\n}\n\n} // namespace bm\n#line\
    \ 7 \"include/mtl/integer_set.hpp\"\n#include <unordered_map>\r\n#include <initializer_list>\r\
    \n\r\ntemplate<typename T, unsigned BITS = 64>\r\nclass _XFastTrie {\r\n public:\r\
    \n  using U = uint64_t;\r\n  static constexpr int W = BITS;\r\n  class iterator;\r\
    \n  static constexpr bool kKeyOnly = std::is_same<T, void>::value;\r\n  using\
    \ element_type = typename std::conditional<kKeyOnly, U, std::pair<U, T>>::type;\r\
    \n private:\r\n  struct Node;\r\n  using node_ptr = std::shared_ptr<Node>;\r\n\
    \  using node_weak = std::weak_ptr<Node>;\r\n  struct Node {\r\n    // common\r\
    \n    uint8_t cmask;\r\n    std::array<node_ptr, 2> child;\r\n    node_weak parent;\r\
    \n    // leaf\r\n    element_type* vptr;\r\n    Node() : cmask(0), child({nullptr,\
    \ nullptr}), vptr(nullptr) {}\r\n    U key() const {\r\n      if constexpr (kKeyOnly)\r\
    \n        return *vptr;\r\n      else\r\n        return vptr->first;\r\n    }\r\
    \n  };\r\n private:\r\n  using _hash_table = std::unordered_map<U, node_ptr>;\r\
    \n  node_ptr root_, sentinel_;\r\n  size_t size_;\r\n  std::array<_hash_table,\
    \ W+1> xmap_;\r\n\r\n public:\r\n  _XFastTrie()\r\n      : root_(std::make_shared<Node>()),\
    \ sentinel_(std::make_shared<Node>()), size_(0)\r\n  {\r\n    sentinel_->child[0]\
    \ = sentinel_->child[1] = root_->child[0] = root_->child[1] = sentinel_;\r\n \
    \   xmap_[0].emplace(0, root_);\r\n  }\r\n  template<typename It>\r\n  explicit\
    \ _XFastTrie(It begin, It end) : _XFastTrie() {\r\n    insert(begin, end);\r\n\
    \  }\r\n  _XFastTrie(std::initializer_list<element_type> list) : _XFastTrie(list.begin(),\
    \ list.end()) {}\r\n private:\r\n  void _del_node(node_ptr u) {\r\n    u->parent.reset();\r\
    \n    for (int i = 0; i < 2; i++) {\r\n      if (u->cmask & (1<<i) and u->child[i]\
    \ != u) {\r\n        _del_node(u->child[i]);\r\n      }\r\n      u->child[i] =\
    \ nullptr;\r\n    }\r\n    if (u->vptr) {\r\n      delete u->vptr;\r\n    }\r\n\
    \  }\r\n  void _del() {\r\n    if (root_)\r\n      _del_node(root_);\r\n    if\
    \ (sentinel_)\r\n      sentinel_->child[0] = sentinel_->child[1] = nullptr;\r\n\
    \  }\r\n public:\r\n//  TODO: Optimal copy implementation\r\n  _XFastTrie(const\
    \ _XFastTrie& r) : _XFastTrie(r.begin(), r.end()) {}\r\n  _XFastTrie& operator=(const\
    \ _XFastTrie& r) {\r\n    _del();\r\n    root_ = std::make_shared<Node>();\r\n\
    \    sentinel_ = std::make_shared<Node>();\r\n    size_ = 0;\r\n    sentinel_->child[0]\
    \ = sentinel_->child[1] =\r\n        root_->child[0] = root_->child[1] = sentinel_;\r\
    \n    xmap_[0].emplace(0, root_);\r\n    insert(r.begin(), r.end());\r\n    return\
    \ *this;\r\n  }\r\n  _XFastTrie(_XFastTrie&& r)\r\n    : root_(std::move(r.root_)),\
    \ sentinel_(std::move(r.sentinel_)),\r\n      size_(std::move(r.size_)), xmap_(std::move(r.xmap_))\
    \ {}\r\n  _XFastTrie& operator=(_XFastTrie&& r) noexcept {\r\n    _del();\r\n\
    \    root_ = std::move(r.root_);\r\n    sentinel_ = std::move(r.sentinel_);\r\n\
    \    size_ = std::move(r.size_);\r\n    xmap_ = std::move(r.xmap_);\r\n    return\
    \ *this;\r\n  }\r\n  ~_XFastTrie() {\r\n    _del();\r\n  }\r\n\r\n  size_t size()\
    \ const { return size_; }\r\n  bool empty() const { return size() == 0; }\r\n\
    \  void clear() {\r\n    *this = _XFastTrie();\r\n  }\r\n\r\n  iterator find(U\
    \ x) const {\r\n    auto it = xmap_[W].find(x);\r\n    return it != xmap_[W].end()\
    \ ? iterator(it->second) : end();\r\n  }\r\n  size_t count(U x) const { return\
    \ (size_t) (find(x) != end()); }\r\n  bool contains(U x) const { return (bool)\
    \ count(x); }\r\n  iterator lower_bound(U x) const {\r\n    int l = 0, r = W+1;\r\
    \n    auto u = root_;\r\n    while (l+1<r) {\r\n      int c = (l+r)/2;\r\n   \
    \   auto vit = xmap_[c].find(x >> (W-c));\r\n      if (vit != xmap_[c].end())\
    \ {\r\n        u = vit->second;\r\n        l = c;\r\n      } else {\r\n      \
    \  r = c;\r\n      }\r\n    }\r\n    if (l == W) return iterator(u);\r\n    auto\
    \ next = ((x>>(W-1-l))&1) == 0 ? u->child[0] : u->child[1]->child[1];\r\n    return\
    \ iterator(next);\r\n  }\r\n  iterator upper_bound(U x) const {\r\n    auto it\
    \ = lower_bound(x);\r\n    return (it != end() and it.ptr_->key() == x) ? ++it\
    \ : it;\r\n  }\r\n  iterator successor(U x) const {\r\n    return upper_bound(x);\r\
    \n  }\r\n  iterator predecessor(U x) const {\r\n    auto it = lower_bound(x);\r\
    \n    return it != begin() ? --it : end();\r\n  }\r\n\r\n private:\r\n  U _key_of_elm(element_type&\
    \ e) const {\r\n    if constexpr (kKeyOnly)\r\n      return e;\r\n    else\r\n\
    \      return e.first;\r\n  }\r\n  U _key_of(iterator it) const {\r\n    return\
    \ _key_of_elm(*(it.ptr_->vptr));\r\n  }\r\n  template<typename... Args>\r\n  U\
    \ _key_of_elm_args(U x, Args&&...) const { return x; }\r\n  iterator _insert_subtree(int\
    \ depth, node_ptr u, U x, element_type* eptr) {\r\n    int i = depth;\r\n    int\
    \ c = (x >> (W-i-1)) & 1;\r\n    auto pred = c == 1 ? u->child[1] : u->child[0]->child[0];\r\
    \n    assert(pred);\r\n    for (; i < W; i++) {\r\n      c = (x >> (W-1-i)) &\
    \ 1;\r\n      u->cmask |= 1u<<c;\r\n      u->child[c] = std::make_shared<Node>();\r\
    \n      u->child[c]->parent = u;\r\n      u = u->child[c];\r\n      xmap_[i+1].emplace(x>>(W-1-i),\
    \ u);\r\n    }\r\n    u->vptr = eptr;\r\n    u->child[0] = pred;\r\n    u->child[1]\
    \ = pred->child[1];\r\n    pred->child[1]->child[0] = u;\r\n    pred->child[1]\
    \ = u;\r\n    auto v = u->parent.lock();\r\n    for (i = W-1; i >= 0; i--) {\r\
    \n      assert(v);\r\n      c = (x >> (W-1-i)) & 1;\r\n      int ic = c^1;\r\n\
    \      if ((v->cmask & (1u<<ic)) == 0\r\n          and (!v->child[ic] or v->child[ic]\
    \ == sentinel_\r\n              or (ic == 0 ? v->child[ic]->key() > x\r\n    \
    \                      : v->child[ic]->key() < x)))\r\n        v->child[ic] =\
    \ u;\r\n      assert(v->child[0] and v->child[1]);\r\n      v = v->parent.lock();\r\
    \n    }\r\n    size_++;\r\n    return iterator(u);\r\n  }\r\n  iterator _insert(U\
    \ x, element_type* eptr) {\r\n    auto u = root_;\r\n    int i, c;\r\n    for\
    \ (i = 0; i < W; i++) {\r\n      c = (x >> (W-1-i)) & 1;\r\n      if ((u->cmask\
    \ & (1u<<c)) == 0) break;\r\n      u = u->child[c];\r\n    }\r\n    if (i == W)\
    \ return iterator(u);\r\n    return _insert_subtree(i, u, x, eptr);\r\n  }\r\n\
    \  iterator _insert_hint(iterator hint, U x, element_type* eptr) {\r\n    // Check\
    \ hint isn't malicious.\r\n    U lx=0, rx=0;\r\n    if ((hint != begin() and x\
    \ < (lx = _key_of(std::prev(hint)))) or\r\n        (hint != end() and (rx = _key_of(hint))\
    \ < x))\r\n      return _insert(x, eptr);\r\n    // Find longest common path on\
    \ the trie.\r\n    int d = 0;\r\n    if (hint != begin())\r\n      d = std::max(d,\
    \ (int) bm::clz(x ^ lx) - (64-W));\r\n    if (hint != end())\r\n      d = std::max(d,\
    \ (int) bm::clz(x ^ rx) - (64-W));\r\n    if (d == W) return iterator(xmap_[W][x]);\r\
    \n    return _insert_subtree(d, xmap_[d][x >> (W-d)], x, eptr);\r\n  }\r\n public:\r\
    \n  iterator insert(const element_type& e) {\r\n    return _insert(_key_of_elm(e),\r\
    \n                   new element_type(e));\r\n  }\r\n  iterator insert(element_type&&\
    \ e) {\r\n    return _insert(_key_of_elm(e),\r\n                   new element_type(std::move(e)));\r\
    \n  }\r\n  template<typename... Args>\r\n  iterator emplace(Args&&... args) {\r\
    \n    return _insert(_key_of_elm_args(args...),\r\n                   new element_type(std::forward<Args>(args)...));\r\
    \n  }\r\n  template<typename... Args>\r\n  iterator emplace_hint(iterator hint,\
    \ Args&&... args) {\r\n    return _insert_hint(hint,\r\n                     \
    \   _key_of_elm_args(args...),\r\n                        new element_type(std::forward<Args>(args)...));\r\
    \n  }\r\n  template<typename It>\r\n  void insert(It begin, It end) {\r\n    using\
    \ traits = std::iterator_traits<It>;\r\n    static_assert(std::is_convertible<typename\
    \ traits::value_type, element_type>::value, \"\");\r\n    static_assert(std::is_base_of<std::forward_iterator_tag,\
    \ typename traits::iterator_category>::value, \"\");\r\n    for (auto it = begin;\
    \ it != end; ++it)\r\n      insert(*it);\r\n  }\r\n  void insert(std::initializer_list<element_type>\
    \ list) {\r\n    insert(list.begin(), list.end());\r\n  }\r\n  iterator erase(iterator\
    \ it) {\r\n    if (it == end())\r\n      return it;\r\n    auto u = it.ptr_;\r\
    \n    U x = u->key();\r\n    delete u->vptr;\r\n    auto next = u->child[0];\r\
    \n    u->child[0]->child[1] = u->child[1];\r\n    u->child[1]->child[0] = u->child[0];\r\
    \n    auto v = u;\r\n    int i, c;\r\n    for (i = W-1; i >= 0; i--) {\r\n   \
    \   c = (x >> (W-1-i)) & 1;\r\n      auto p = v->parent.lock();\r\n      v->parent.reset();\r\
    \n      p->cmask ^= 1u<<c;\r\n      p->child[c] = nullptr;\r\n      assert(xmap_[i+1].find(x>>(W-1-i))->second\
    \ == v);\r\n      xmap_[i+1].erase(x>>(W-1-i));\r\n      v = p;\r\n      if ((p->cmask\
    \ & (1u<<(c^1))) != 0) break;\r\n    }\r\n    if (i >= 0) {\r\n      c = (x >>\
    \ (W-1-i)) & 1;\r\n      v->child[c] = u->child[c^1];\r\n      v = v->parent.lock();\r\
    \n      i--;\r\n      for (; i >= 0; i--) {\r\n        c = (x >> (W-1-i)) & 1;\r\
    \n        int ic = c^1;\r\n        if (v->child[ic] == u) {\r\n          v->child[ic]\
    \ = u->child[c];\r\n        }\r\n        v = v->parent.lock();\r\n      }\r\n\
    \    }\r\n    size_--;\r\n    return iterator(next);\r\n  }\r\n  iterator erase(U\
    \ x) {\r\n    auto it = lower_bound(x);\r\n    if (it.key() != x)\r\n      return\
    \ it;\r\n    return erase(it);\r\n  }\r\n\r\n public:\r\n  class iterator {\r\n\
    \   public:\r\n    using value_type = element_type;\r\n    using pointer = element_type*;\r\
    \n    using reference = const element_type&;\r\n    using difference_type = long\
    \ long;\r\n    using iterator_category = std::bidirectional_iterator_tag;\r\n\
    \   private:\r\n    node_ptr ptr_;\r\n    friend class _XFastTrie;\r\n   public:\r\
    \n    explicit iterator(node_ptr ptr) : ptr_(ptr) {}\r\n    reference operator*()\
    \ const { return *(ptr_->vptr); }\r\n    pointer operator->() const { return ptr_->vptr;\
    \ }\r\n    bool operator==(const iterator& r) const { return ptr_ == r.ptr_; }\r\
    \n    bool operator!=(const iterator& r) const { return ptr_ != r.ptr_; }\r\n\
    \    iterator& operator++() {\r\n      ptr_ = ptr_->child[1];\r\n      return\
    \ *this;\r\n    }\r\n    iterator operator++(int) {\r\n      iterator ret = *this;\r\
    \n      ++*this;\r\n      return ret;\r\n    }\r\n    iterator& operator--() {\r\
    \n      ptr_ = ptr_->child[0];\r\n      return *this;\r\n    }\r\n    iterator\
    \ operator--(int) {\r\n      iterator ret = *this;\r\n      --*this;\r\n     \
    \ return ret;\r\n    }\r\n    U key() const { return ptr_->key(); }\r\n  };\r\n\
    \  iterator begin() const { return iterator(sentinel_->child[1]); }\r\n  iterator\
    \ end() const { return iterator(sentinel_); }\r\n};\r\ntemplate<typename T, unsigned\
    \ BITS>\r\nconstexpr int _XFastTrie<T, BITS>::W;\r\ntemplate<typename T, unsigned\
    \ BITS>\r\nconstexpr bool _XFastTrie<T, BITS>::kKeyOnly;\r\ntemplate<unsigned\
    \ BITS = 64>\r\nusing XFastTrieSet = _XFastTrie<void, BITS>;\r\ntemplate<typename\
    \ T, unsigned BITS = 64>\r\nusing XFastTrieMap = _XFastTrie<T, BITS>;\r\n\r\n\r\
    \ntemplate<typename T, unsigned BITS=64>\r\nclass YFastTrie {\r\n public:\r\n\
    \  using U = uint64_t;\r\n  static constexpr int W = BITS;\r\n  static constexpr\
    \ U MAX_KEY = W == 64 ? ~uint64_t(0) : (1ull<<W)-1;\r\n  static constexpr bool\
    \ kKeyOnly = std::is_same<T, void>::value;\r\n  using element_type = typename\
    \ std::conditional<kKeyOnly, U, std::pair<U,T>>::type;\r\n  using value_type =\
    \ typename std::conditional<kKeyOnly, U, T>::type;\r\n  using _set = typename\
    \ std::conditional<kKeyOnly, TreapSet<U>, TreapMap<U,T>>::type;\r\n  using _set_iterator\
    \ = typename _set::iterator;\r\n  using _xft = XFastTrieMap<_set, W>;\r\n  class\
    \ iterator;\r\n private:\r\n  _xft xft_;\r\n  size_t size_;\r\n\r\n  bool _pibot_selected()\
    \ {\r\n    return rand() % W == 0;\r\n  }\r\n  U _key_of_elm(const element_type&\
    \ e) const {\r\n    if constexpr (kKeyOnly)\r\n      return e;\r\n    else\r\n\
    \      return e.first;\r\n  }\r\n  U _key_of_elm(element_type&& e) const {\r\n\
    \    if constexpr (kKeyOnly)\r\n      return e;\r\n    else\r\n      return e.first;\r\
    \n  }\r\n  template<typename... Args>\r\n  U _key_of_elm_args(U x, Args&&...)\
    \ const {\r\n    return x;\r\n  }\r\n  U _key_of_sub(_set_iterator it) const {\r\
    \n    return _key_of_elm(*it);\r\n  }\r\n  U _key_of(iterator it) const {\r\n\
    \    return _key_of_sub(it.sit_);\r\n  }\r\n\r\n public:\r\n  YFastTrie() : size_(0)\
    \ {\r\n    xft_.emplace(MAX_KEY, _set{});\r\n  }\r\n  template<typename It>\r\n\
    \  explicit YFastTrie(It begin, It end) : YFastTrie() {\r\n    insert(begin, end);\r\
    \n  }\r\n  YFastTrie(std::initializer_list<element_type> list) : YFastTrie(list.begin(),\
    \ list.end()) {}\r\n\r\n  size_t size() const { return size_; }\r\n  bool empty()\
    \ const { return size() == 0; }\r\n  void clear() {\r\n    *this = YFastTrie();\r\
    \n  }\r\n\r\n  iterator find(U x) const {\r\n    auto xit = xft_.lower_bound(x);\r\
    \n    assert(xit != xft_.end());\r\n    auto& s = xit->second;\r\n    auto sit\
    \ = s.find(x);\r\n    return sit != s.end() ? iterator(this, xit, sit) : end();\r\
    \n  }\r\n  size_t count(U x) const { return (size_t) (find(x) != end()); }\r\n\
    \  bool contains(U x) const { return (bool) count(x); }\r\n  iterator lower_bound(U\
    \ x) const {\r\n    auto xit = xft_.lower_bound(x);\r\n    assert(xit != xft_.end());\r\
    \n    auto& s = xit->second;\r\n    auto sit = s.lower_bound(x);\r\n    if (sit\
    \ == s.end()) {\r\n      assert(std::next(xit) == xft_.end());\r\n      return\
    \ end();\r\n    }\r\n    return iterator(this, xit, sit);\r\n  }\r\n  iterator\
    \ upper_bound(U x) const {\r\n    auto it = lower_bound(x);\r\n    return (it\
    \ != end() and _key_of(it) == x) ? ++it : it;\r\n  }\r\n  iterator successor(U\
    \ x) const {\r\n    return upper_bound(x);\r\n  }\r\n  iterator predecessor(U\
    \ x) const {\r\n    auto it = lower_bound(x);\r\n    return it != begin() ? --it\
    \ : end();\r\n  }\r\n\r\n private:\r\n  template<typename C>\r\n  iterator _insert_before(typename\
    \ _xft::iterator xit, _set_iterator sit, U x, C elm_constructor) {\r\n    auto&\
    \ s = xit->second;\r\n    // Expect 'sit = s.emplace_hint(sit, element)'\r\n \
    \   sit = elm_constructor(s, sit);\r\n    size_++;\r\n    if (_pibot_selected())\
    \ {\r\n      xit = xft_.emplace_hint(xit, x, _set());\r\n      xit->second = s.split(std::next(sit));\r\
    \n    }\r\n    return iterator(this, xit, sit);\r\n  }\r\n  template<typename\
    \ C>\r\n  iterator _insert(U x, C elm_constructor) {\r\n    auto xit = xft_.lower_bound(x);\r\
    \n    assert(xit != xft_.end());\r\n    auto& s = xit->second;\r\n    auto sit\
    \ = s.lower_bound(x);\r\n    if (sit == s.end() or _key_of_sub(sit) > x) {\r\n\
    \      return _insert_before(xit, sit, x, elm_constructor);\r\n    }\r\n    return\
    \ iterator(this, xit, sit);\r\n  }\r\n  template<typename C>\r\n  iterator _insert_hint(iterator\
    \ hint, U x, C elm_constructor) {\r\n    // Check hint isn't malicious.\r\n  \
    \  if ((hint != begin() and x <= _key_of(std::prev(hint))) or\r\n        (hint\
    \ != end() and _key_of(hint) <= x))\r\n      return _insert(x, elm_constructor);\r\
    \n\r\n    auto xit = hint.xit_;\r\n    assert(xit != xft_.end());\r\n    auto\
    \ sit = hint.sit_;\r\n    assert(sit == xit->second.end() or _key_of_sub(sit)\
    \ > x);\r\n    return _insert_before(xit, sit, x, elm_constructor);\r\n  }\r\n\
    \ public:\r\n  iterator insert(const element_type& e) {\r\n    return _insert(_key_of_elm(e),\r\
    \n                   [&e]\r\n                   (_set& s, const _set_iterator&\
    \ it) {\r\n                     return s.emplace_hint(it, e);\r\n            \
    \       });\r\n  }\r\n  iterator insert(element_type&& e) {\r\n    return _insert(_key_of_elm(e),\r\
    \n                   [e=std::move(e)](_set& s, const _set_iterator& it) {\r\n\
    \                     return s.emplace_hint(it, std::move(e));\r\n           \
    \        });\r\n  }\r\n  template<typename... Args>\r\n  iterator emplace(Args&&...\
    \ args) {\r\n    return _insert(_key_of_elm_args(args...),\r\n               \
    \    [&args...](_set& s, const _set_iterator& it) {\r\n                     return\
    \ s.emplace_hint(it, std::forward<Args>(args)...);\r\n                   });\r\
    \n  }\r\n  template<typename... Args>\r\n  iterator emplace_hint(iterator hint,\
    \ Args&&... args) {\r\n    return _insert_hint(hint,\r\n                     \
    \   _key_of_elm_args(args...),\r\n                        [&args...](_set& s,\
    \ const _set_iterator& it) {\r\n                          return s.emplace_hint(it,\
    \ std::forward<Args>(args)...);\r\n                        });\r\n  }\r\n  template<typename\
    \ It>\r\n  void insert(It begin, It end) {\r\n    using traits = std::iterator_traits<It>;\r\
    \n    static_assert(std::is_convertible<typename traits::value_type, element_type>::value,\
    \ \"\");\r\n    static_assert(std::is_base_of<std::forward_iterator_tag, typename\
    \ traits::iterator_category>::value, \"\");\r\n    for (auto it = begin; it !=\
    \ end; ++it)\r\n      insert(*it);\r\n  }\r\n  void insert(std::initializer_list<element_type>\
    \ list) {\r\n    insert(list.begin(), list.end());\r\n  }\r\n  iterator erase(iterator\
    \ it) {\r\n    if (it == end())\r\n      return it;\r\n    U x = _key_of(it);\r\
    \n    auto xit = it.xit_;\r\n    assert(xit != xft_.end());\r\n    auto& s = xit->second;\r\
    \n    auto sit = it.sit_;\r\n    size_--;\r\n    sit = s.erase(sit);\r\n    if\
    \ (x == xit.key()) {\r\n      if (x != MAX_KEY) {\r\n        if (!s.empty()) {\r\
    \n          auto& snext = std::next(xit)->second;\r\n          sit = snext.absorb(&s);\r\
    \n        }\r\n        xit = xft_.erase(xit);\r\n      } else {\r\n        return\
    \ end();\r\n      }\r\n    }\r\n    return iterator(this, xit, sit);\r\n  }\r\n\
    \  iterator erase(U x) {\r\n    auto it = lower_bound(x);\r\n    return (_key_of(it)\
    \ == x) ? erase(it) : it;\r\n  }\r\n\r\n public:\r\n  class iterator {\r\n   public:\r\
    \n    using value_type = element_type;\r\n    using pointer = element_type*;\r\
    \n    using reference = const element_type&;\r\n    using difference_type = long\
    \ long;\r\n    using iterator_category = std::bidirectional_iterator_tag;\r\n\
    \    using xiterator = typename _xft::iterator;\r\n    using siterator = _set_iterator;\r\
    \n   private:\r\n    const YFastTrie* yft_;\r\n    xiterator xit_;\r\n    siterator\
    \ sit_;\r\n    friend class YFastTrie;\r\n   public:\r\n    iterator(const YFastTrie*\
    \ yft, xiterator xit, siterator sit) : yft_(yft), xit_(xit), sit_(sit) {}\r\n\
    \    reference operator*() const { return *sit_; }\r\n    pointer operator->()\
    \ const { return &*sit_; }\r\n    bool operator==(const iterator& r) { return\
    \ sit_ == r.sit_; }\r\n    bool operator!=(const iterator& r) { return sit_ !=\
    \ r.sit_; }\r\n    iterator& operator++() {\r\n      if (++sit_ == xit_->second.end())\
    \ {\r\n        auto nxt = std::next(xit_);\r\n        if (nxt != yft_->xft_.end())\
    \ {\r\n          xit_ = nxt;\r\n          sit_ = xit_->second.begin();\r\n   \
    \     }\r\n      }\r\n      return *this;\r\n    }\r\n    iterator operator++(int)\
    \ {\r\n      iterator ret = *this;\r\n      ++this;\r\n      return ret;\r\n \
    \   }\r\n    iterator& operator--() {\r\n      if (sit_ == xit_->second.begin())\
    \ {\r\n        --xit_;\r\n        sit_ = std::prev(xit_->second.end());\r\n  \
    \    } else {\r\n        --sit_;\r\n      }\r\n      return *this;\r\n    }\r\n\
    \    iterator operator--(int) {\r\n      iterator ret = *this;\r\n      --this;\r\
    \n      return ret;\r\n    }\r\n  };\r\n  iterator begin() const {\r\n    auto\
    \ xit = xft_.begin();\r\n    return iterator(this, xit, xit->second.begin());\r\
    \n  }\r\n  iterator end() const {\r\n    auto xit = std::prev(xft_.end());\r\n\
    \    return iterator(this, xit, xit->second.end());\r\n  }\r\n};\r\ntemplate<typename\
    \ T, unsigned BITS>\r\nconstexpr int YFastTrie<T, BITS>::W;\r\ntemplate<typename\
    \ T, unsigned BITS>\r\nconstexpr typename YFastTrie<T, BITS>::U YFastTrie<T, BITS>::MAX_KEY;\r\
    \ntemplate<typename T, unsigned BITS>\r\nconstexpr bool YFastTrie<T, BITS>::kKeyOnly;\r\
    \ntemplate<unsigned BITS=64>\r\nusing YFastTrieSet = YFastTrie<void, BITS>;\r\n\
    \r\n\r\ntemplate<typename T, unsigned BITS=64>\r\nclass YFastTrieMAP : public\
    \ YFastTrie<T, BITS> {\r\n  using _base = YFastTrie<T, BITS>;\r\n public:\r\n\
    \  using typename _base::value_type;\r\n  using typename _base::U;\r\n  using\
    \ _base::kKeyOnly;\r\n  using reference = value_type&;\r\n  reference operator[](U\
    \ x) {\r\n    auto it = find(x);\r\n    if (it == _base::end())\r\n      it =\
    \ emplace(x);\r\n    if constexpr (kKeyOnly)\r\n      return *it;\r\n    else\r\
    \n      return it->second;\r\n  }\r\n};\n"
  code: "#pragma once\r\n#include \"treap.hpp\"\r\n#include \"bit_manip.hpp\"\r\n\
    #include <memory>\r\n#include <iterator>\r\n#include <cassert>\r\n#include <unordered_map>\r\
    \n#include <initializer_list>\r\n\r\ntemplate<typename T, unsigned BITS = 64>\r\
    \nclass _XFastTrie {\r\n public:\r\n  using U = uint64_t;\r\n  static constexpr\
    \ int W = BITS;\r\n  class iterator;\r\n  static constexpr bool kKeyOnly = std::is_same<T,\
    \ void>::value;\r\n  using element_type = typename std::conditional<kKeyOnly,\
    \ U, std::pair<U, T>>::type;\r\n private:\r\n  struct Node;\r\n  using node_ptr\
    \ = std::shared_ptr<Node>;\r\n  using node_weak = std::weak_ptr<Node>;\r\n  struct\
    \ Node {\r\n    // common\r\n    uint8_t cmask;\r\n    std::array<node_ptr, 2>\
    \ child;\r\n    node_weak parent;\r\n    // leaf\r\n    element_type* vptr;\r\n\
    \    Node() : cmask(0), child({nullptr, nullptr}), vptr(nullptr) {}\r\n    U key()\
    \ const {\r\n      if constexpr (kKeyOnly)\r\n        return *vptr;\r\n      else\r\
    \n        return vptr->first;\r\n    }\r\n  };\r\n private:\r\n  using _hash_table\
    \ = std::unordered_map<U, node_ptr>;\r\n  node_ptr root_, sentinel_;\r\n  size_t\
    \ size_;\r\n  std::array<_hash_table, W+1> xmap_;\r\n\r\n public:\r\n  _XFastTrie()\r\
    \n      : root_(std::make_shared<Node>()), sentinel_(std::make_shared<Node>()),\
    \ size_(0)\r\n  {\r\n    sentinel_->child[0] = sentinel_->child[1] = root_->child[0]\
    \ = root_->child[1] = sentinel_;\r\n    xmap_[0].emplace(0, root_);\r\n  }\r\n\
    \  template<typename It>\r\n  explicit _XFastTrie(It begin, It end) : _XFastTrie()\
    \ {\r\n    insert(begin, end);\r\n  }\r\n  _XFastTrie(std::initializer_list<element_type>\
    \ list) : _XFastTrie(list.begin(), list.end()) {}\r\n private:\r\n  void _del_node(node_ptr\
    \ u) {\r\n    u->parent.reset();\r\n    for (int i = 0; i < 2; i++) {\r\n    \
    \  if (u->cmask & (1<<i) and u->child[i] != u) {\r\n        _del_node(u->child[i]);\r\
    \n      }\r\n      u->child[i] = nullptr;\r\n    }\r\n    if (u->vptr) {\r\n \
    \     delete u->vptr;\r\n    }\r\n  }\r\n  void _del() {\r\n    if (root_)\r\n\
    \      _del_node(root_);\r\n    if (sentinel_)\r\n      sentinel_->child[0] =\
    \ sentinel_->child[1] = nullptr;\r\n  }\r\n public:\r\n//  TODO: Optimal copy\
    \ implementation\r\n  _XFastTrie(const _XFastTrie& r) : _XFastTrie(r.begin(),\
    \ r.end()) {}\r\n  _XFastTrie& operator=(const _XFastTrie& r) {\r\n    _del();\r\
    \n    root_ = std::make_shared<Node>();\r\n    sentinel_ = std::make_shared<Node>();\r\
    \n    size_ = 0;\r\n    sentinel_->child[0] = sentinel_->child[1] =\r\n      \
    \  root_->child[0] = root_->child[1] = sentinel_;\r\n    xmap_[0].emplace(0, root_);\r\
    \n    insert(r.begin(), r.end());\r\n    return *this;\r\n  }\r\n  _XFastTrie(_XFastTrie&&\
    \ r)\r\n    : root_(std::move(r.root_)), sentinel_(std::move(r.sentinel_)),\r\n\
    \      size_(std::move(r.size_)), xmap_(std::move(r.xmap_)) {}\r\n  _XFastTrie&\
    \ operator=(_XFastTrie&& r) noexcept {\r\n    _del();\r\n    root_ = std::move(r.root_);\r\
    \n    sentinel_ = std::move(r.sentinel_);\r\n    size_ = std::move(r.size_);\r\
    \n    xmap_ = std::move(r.xmap_);\r\n    return *this;\r\n  }\r\n  ~_XFastTrie()\
    \ {\r\n    _del();\r\n  }\r\n\r\n  size_t size() const { return size_; }\r\n \
    \ bool empty() const { return size() == 0; }\r\n  void clear() {\r\n    *this\
    \ = _XFastTrie();\r\n  }\r\n\r\n  iterator find(U x) const {\r\n    auto it =\
    \ xmap_[W].find(x);\r\n    return it != xmap_[W].end() ? iterator(it->second)\
    \ : end();\r\n  }\r\n  size_t count(U x) const { return (size_t) (find(x) != end());\
    \ }\r\n  bool contains(U x) const { return (bool) count(x); }\r\n  iterator lower_bound(U\
    \ x) const {\r\n    int l = 0, r = W+1;\r\n    auto u = root_;\r\n    while (l+1<r)\
    \ {\r\n      int c = (l+r)/2;\r\n      auto vit = xmap_[c].find(x >> (W-c));\r\
    \n      if (vit != xmap_[c].end()) {\r\n        u = vit->second;\r\n        l\
    \ = c;\r\n      } else {\r\n        r = c;\r\n      }\r\n    }\r\n    if (l ==\
    \ W) return iterator(u);\r\n    auto next = ((x>>(W-1-l))&1) == 0 ? u->child[0]\
    \ : u->child[1]->child[1];\r\n    return iterator(next);\r\n  }\r\n  iterator\
    \ upper_bound(U x) const {\r\n    auto it = lower_bound(x);\r\n    return (it\
    \ != end() and it.ptr_->key() == x) ? ++it : it;\r\n  }\r\n  iterator successor(U\
    \ x) const {\r\n    return upper_bound(x);\r\n  }\r\n  iterator predecessor(U\
    \ x) const {\r\n    auto it = lower_bound(x);\r\n    return it != begin() ? --it\
    \ : end();\r\n  }\r\n\r\n private:\r\n  U _key_of_elm(element_type& e) const {\r\
    \n    if constexpr (kKeyOnly)\r\n      return e;\r\n    else\r\n      return e.first;\r\
    \n  }\r\n  U _key_of(iterator it) const {\r\n    return _key_of_elm(*(it.ptr_->vptr));\r\
    \n  }\r\n  template<typename... Args>\r\n  U _key_of_elm_args(U x, Args&&...)\
    \ const { return x; }\r\n  iterator _insert_subtree(int depth, node_ptr u, U x,\
    \ element_type* eptr) {\r\n    int i = depth;\r\n    int c = (x >> (W-i-1)) &\
    \ 1;\r\n    auto pred = c == 1 ? u->child[1] : u->child[0]->child[0];\r\n    assert(pred);\r\
    \n    for (; i < W; i++) {\r\n      c = (x >> (W-1-i)) & 1;\r\n      u->cmask\
    \ |= 1u<<c;\r\n      u->child[c] = std::make_shared<Node>();\r\n      u->child[c]->parent\
    \ = u;\r\n      u = u->child[c];\r\n      xmap_[i+1].emplace(x>>(W-1-i), u);\r\
    \n    }\r\n    u->vptr = eptr;\r\n    u->child[0] = pred;\r\n    u->child[1] =\
    \ pred->child[1];\r\n    pred->child[1]->child[0] = u;\r\n    pred->child[1] =\
    \ u;\r\n    auto v = u->parent.lock();\r\n    for (i = W-1; i >= 0; i--) {\r\n\
    \      assert(v);\r\n      c = (x >> (W-1-i)) & 1;\r\n      int ic = c^1;\r\n\
    \      if ((v->cmask & (1u<<ic)) == 0\r\n          and (!v->child[ic] or v->child[ic]\
    \ == sentinel_\r\n              or (ic == 0 ? v->child[ic]->key() > x\r\n    \
    \                      : v->child[ic]->key() < x)))\r\n        v->child[ic] =\
    \ u;\r\n      assert(v->child[0] and v->child[1]);\r\n      v = v->parent.lock();\r\
    \n    }\r\n    size_++;\r\n    return iterator(u);\r\n  }\r\n  iterator _insert(U\
    \ x, element_type* eptr) {\r\n    auto u = root_;\r\n    int i, c;\r\n    for\
    \ (i = 0; i < W; i++) {\r\n      c = (x >> (W-1-i)) & 1;\r\n      if ((u->cmask\
    \ & (1u<<c)) == 0) break;\r\n      u = u->child[c];\r\n    }\r\n    if (i == W)\
    \ return iterator(u);\r\n    return _insert_subtree(i, u, x, eptr);\r\n  }\r\n\
    \  iterator _insert_hint(iterator hint, U x, element_type* eptr) {\r\n    // Check\
    \ hint isn't malicious.\r\n    U lx=0, rx=0;\r\n    if ((hint != begin() and x\
    \ < (lx = _key_of(std::prev(hint)))) or\r\n        (hint != end() and (rx = _key_of(hint))\
    \ < x))\r\n      return _insert(x, eptr);\r\n    // Find longest common path on\
    \ the trie.\r\n    int d = 0;\r\n    if (hint != begin())\r\n      d = std::max(d,\
    \ (int) bm::clz(x ^ lx) - (64-W));\r\n    if (hint != end())\r\n      d = std::max(d,\
    \ (int) bm::clz(x ^ rx) - (64-W));\r\n    if (d == W) return iterator(xmap_[W][x]);\r\
    \n    return _insert_subtree(d, xmap_[d][x >> (W-d)], x, eptr);\r\n  }\r\n public:\r\
    \n  iterator insert(const element_type& e) {\r\n    return _insert(_key_of_elm(e),\r\
    \n                   new element_type(e));\r\n  }\r\n  iterator insert(element_type&&\
    \ e) {\r\n    return _insert(_key_of_elm(e),\r\n                   new element_type(std::move(e)));\r\
    \n  }\r\n  template<typename... Args>\r\n  iterator emplace(Args&&... args) {\r\
    \n    return _insert(_key_of_elm_args(args...),\r\n                   new element_type(std::forward<Args>(args)...));\r\
    \n  }\r\n  template<typename... Args>\r\n  iterator emplace_hint(iterator hint,\
    \ Args&&... args) {\r\n    return _insert_hint(hint,\r\n                     \
    \   _key_of_elm_args(args...),\r\n                        new element_type(std::forward<Args>(args)...));\r\
    \n  }\r\n  template<typename It>\r\n  void insert(It begin, It end) {\r\n    using\
    \ traits = std::iterator_traits<It>;\r\n    static_assert(std::is_convertible<typename\
    \ traits::value_type, element_type>::value, \"\");\r\n    static_assert(std::is_base_of<std::forward_iterator_tag,\
    \ typename traits::iterator_category>::value, \"\");\r\n    for (auto it = begin;\
    \ it != end; ++it)\r\n      insert(*it);\r\n  }\r\n  void insert(std::initializer_list<element_type>\
    \ list) {\r\n    insert(list.begin(), list.end());\r\n  }\r\n  iterator erase(iterator\
    \ it) {\r\n    if (it == end())\r\n      return it;\r\n    auto u = it.ptr_;\r\
    \n    U x = u->key();\r\n    delete u->vptr;\r\n    auto next = u->child[0];\r\
    \n    u->child[0]->child[1] = u->child[1];\r\n    u->child[1]->child[0] = u->child[0];\r\
    \n    auto v = u;\r\n    int i, c;\r\n    for (i = W-1; i >= 0; i--) {\r\n   \
    \   c = (x >> (W-1-i)) & 1;\r\n      auto p = v->parent.lock();\r\n      v->parent.reset();\r\
    \n      p->cmask ^= 1u<<c;\r\n      p->child[c] = nullptr;\r\n      assert(xmap_[i+1].find(x>>(W-1-i))->second\
    \ == v);\r\n      xmap_[i+1].erase(x>>(W-1-i));\r\n      v = p;\r\n      if ((p->cmask\
    \ & (1u<<(c^1))) != 0) break;\r\n    }\r\n    if (i >= 0) {\r\n      c = (x >>\
    \ (W-1-i)) & 1;\r\n      v->child[c] = u->child[c^1];\r\n      v = v->parent.lock();\r\
    \n      i--;\r\n      for (; i >= 0; i--) {\r\n        c = (x >> (W-1-i)) & 1;\r\
    \n        int ic = c^1;\r\n        if (v->child[ic] == u) {\r\n          v->child[ic]\
    \ = u->child[c];\r\n        }\r\n        v = v->parent.lock();\r\n      }\r\n\
    \    }\r\n    size_--;\r\n    return iterator(next);\r\n  }\r\n  iterator erase(U\
    \ x) {\r\n    auto it = lower_bound(x);\r\n    if (it.key() != x)\r\n      return\
    \ it;\r\n    return erase(it);\r\n  }\r\n\r\n public:\r\n  class iterator {\r\n\
    \   public:\r\n    using value_type = element_type;\r\n    using pointer = element_type*;\r\
    \n    using reference = const element_type&;\r\n    using difference_type = long\
    \ long;\r\n    using iterator_category = std::bidirectional_iterator_tag;\r\n\
    \   private:\r\n    node_ptr ptr_;\r\n    friend class _XFastTrie;\r\n   public:\r\
    \n    explicit iterator(node_ptr ptr) : ptr_(ptr) {}\r\n    reference operator*()\
    \ const { return *(ptr_->vptr); }\r\n    pointer operator->() const { return ptr_->vptr;\
    \ }\r\n    bool operator==(const iterator& r) const { return ptr_ == r.ptr_; }\r\
    \n    bool operator!=(const iterator& r) const { return ptr_ != r.ptr_; }\r\n\
    \    iterator& operator++() {\r\n      ptr_ = ptr_->child[1];\r\n      return\
    \ *this;\r\n    }\r\n    iterator operator++(int) {\r\n      iterator ret = *this;\r\
    \n      ++*this;\r\n      return ret;\r\n    }\r\n    iterator& operator--() {\r\
    \n      ptr_ = ptr_->child[0];\r\n      return *this;\r\n    }\r\n    iterator\
    \ operator--(int) {\r\n      iterator ret = *this;\r\n      --*this;\r\n     \
    \ return ret;\r\n    }\r\n    U key() const { return ptr_->key(); }\r\n  };\r\n\
    \  iterator begin() const { return iterator(sentinel_->child[1]); }\r\n  iterator\
    \ end() const { return iterator(sentinel_); }\r\n};\r\ntemplate<typename T, unsigned\
    \ BITS>\r\nconstexpr int _XFastTrie<T, BITS>::W;\r\ntemplate<typename T, unsigned\
    \ BITS>\r\nconstexpr bool _XFastTrie<T, BITS>::kKeyOnly;\r\ntemplate<unsigned\
    \ BITS = 64>\r\nusing XFastTrieSet = _XFastTrie<void, BITS>;\r\ntemplate<typename\
    \ T, unsigned BITS = 64>\r\nusing XFastTrieMap = _XFastTrie<T, BITS>;\r\n\r\n\r\
    \ntemplate<typename T, unsigned BITS=64>\r\nclass YFastTrie {\r\n public:\r\n\
    \  using U = uint64_t;\r\n  static constexpr int W = BITS;\r\n  static constexpr\
    \ U MAX_KEY = W == 64 ? ~uint64_t(0) : (1ull<<W)-1;\r\n  static constexpr bool\
    \ kKeyOnly = std::is_same<T, void>::value;\r\n  using element_type = typename\
    \ std::conditional<kKeyOnly, U, std::pair<U,T>>::type;\r\n  using value_type =\
    \ typename std::conditional<kKeyOnly, U, T>::type;\r\n  using _set = typename\
    \ std::conditional<kKeyOnly, TreapSet<U>, TreapMap<U,T>>::type;\r\n  using _set_iterator\
    \ = typename _set::iterator;\r\n  using _xft = XFastTrieMap<_set, W>;\r\n  class\
    \ iterator;\r\n private:\r\n  _xft xft_;\r\n  size_t size_;\r\n\r\n  bool _pibot_selected()\
    \ {\r\n    return rand() % W == 0;\r\n  }\r\n  U _key_of_elm(const element_type&\
    \ e) const {\r\n    if constexpr (kKeyOnly)\r\n      return e;\r\n    else\r\n\
    \      return e.first;\r\n  }\r\n  U _key_of_elm(element_type&& e) const {\r\n\
    \    if constexpr (kKeyOnly)\r\n      return e;\r\n    else\r\n      return e.first;\r\
    \n  }\r\n  template<typename... Args>\r\n  U _key_of_elm_args(U x, Args&&...)\
    \ const {\r\n    return x;\r\n  }\r\n  U _key_of_sub(_set_iterator it) const {\r\
    \n    return _key_of_elm(*it);\r\n  }\r\n  U _key_of(iterator it) const {\r\n\
    \    return _key_of_sub(it.sit_);\r\n  }\r\n\r\n public:\r\n  YFastTrie() : size_(0)\
    \ {\r\n    xft_.emplace(MAX_KEY, _set{});\r\n  }\r\n  template<typename It>\r\n\
    \  explicit YFastTrie(It begin, It end) : YFastTrie() {\r\n    insert(begin, end);\r\
    \n  }\r\n  YFastTrie(std::initializer_list<element_type> list) : YFastTrie(list.begin(),\
    \ list.end()) {}\r\n\r\n  size_t size() const { return size_; }\r\n  bool empty()\
    \ const { return size() == 0; }\r\n  void clear() {\r\n    *this = YFastTrie();\r\
    \n  }\r\n\r\n  iterator find(U x) const {\r\n    auto xit = xft_.lower_bound(x);\r\
    \n    assert(xit != xft_.end());\r\n    auto& s = xit->second;\r\n    auto sit\
    \ = s.find(x);\r\n    return sit != s.end() ? iterator(this, xit, sit) : end();\r\
    \n  }\r\n  size_t count(U x) const { return (size_t) (find(x) != end()); }\r\n\
    \  bool contains(U x) const { return (bool) count(x); }\r\n  iterator lower_bound(U\
    \ x) const {\r\n    auto xit = xft_.lower_bound(x);\r\n    assert(xit != xft_.end());\r\
    \n    auto& s = xit->second;\r\n    auto sit = s.lower_bound(x);\r\n    if (sit\
    \ == s.end()) {\r\n      assert(std::next(xit) == xft_.end());\r\n      return\
    \ end();\r\n    }\r\n    return iterator(this, xit, sit);\r\n  }\r\n  iterator\
    \ upper_bound(U x) const {\r\n    auto it = lower_bound(x);\r\n    return (it\
    \ != end() and _key_of(it) == x) ? ++it : it;\r\n  }\r\n  iterator successor(U\
    \ x) const {\r\n    return upper_bound(x);\r\n  }\r\n  iterator predecessor(U\
    \ x) const {\r\n    auto it = lower_bound(x);\r\n    return it != begin() ? --it\
    \ : end();\r\n  }\r\n\r\n private:\r\n  template<typename C>\r\n  iterator _insert_before(typename\
    \ _xft::iterator xit, _set_iterator sit, U x, C elm_constructor) {\r\n    auto&\
    \ s = xit->second;\r\n    // Expect 'sit = s.emplace_hint(sit, element)'\r\n \
    \   sit = elm_constructor(s, sit);\r\n    size_++;\r\n    if (_pibot_selected())\
    \ {\r\n      xit = xft_.emplace_hint(xit, x, _set());\r\n      xit->second = s.split(std::next(sit));\r\
    \n    }\r\n    return iterator(this, xit, sit);\r\n  }\r\n  template<typename\
    \ C>\r\n  iterator _insert(U x, C elm_constructor) {\r\n    auto xit = xft_.lower_bound(x);\r\
    \n    assert(xit != xft_.end());\r\n    auto& s = xit->second;\r\n    auto sit\
    \ = s.lower_bound(x);\r\n    if (sit == s.end() or _key_of_sub(sit) > x) {\r\n\
    \      return _insert_before(xit, sit, x, elm_constructor);\r\n    }\r\n    return\
    \ iterator(this, xit, sit);\r\n  }\r\n  template<typename C>\r\n  iterator _insert_hint(iterator\
    \ hint, U x, C elm_constructor) {\r\n    // Check hint isn't malicious.\r\n  \
    \  if ((hint != begin() and x <= _key_of(std::prev(hint))) or\r\n        (hint\
    \ != end() and _key_of(hint) <= x))\r\n      return _insert(x, elm_constructor);\r\
    \n\r\n    auto xit = hint.xit_;\r\n    assert(xit != xft_.end());\r\n    auto\
    \ sit = hint.sit_;\r\n    assert(sit == xit->second.end() or _key_of_sub(sit)\
    \ > x);\r\n    return _insert_before(xit, sit, x, elm_constructor);\r\n  }\r\n\
    \ public:\r\n  iterator insert(const element_type& e) {\r\n    return _insert(_key_of_elm(e),\r\
    \n                   [&e]\r\n                   (_set& s, const _set_iterator&\
    \ it) {\r\n                     return s.emplace_hint(it, e);\r\n            \
    \       });\r\n  }\r\n  iterator insert(element_type&& e) {\r\n    return _insert(_key_of_elm(e),\r\
    \n                   [e=std::move(e)](_set& s, const _set_iterator& it) {\r\n\
    \                     return s.emplace_hint(it, std::move(e));\r\n           \
    \        });\r\n  }\r\n  template<typename... Args>\r\n  iterator emplace(Args&&...\
    \ args) {\r\n    return _insert(_key_of_elm_args(args...),\r\n               \
    \    [&args...](_set& s, const _set_iterator& it) {\r\n                     return\
    \ s.emplace_hint(it, std::forward<Args>(args)...);\r\n                   });\r\
    \n  }\r\n  template<typename... Args>\r\n  iterator emplace_hint(iterator hint,\
    \ Args&&... args) {\r\n    return _insert_hint(hint,\r\n                     \
    \   _key_of_elm_args(args...),\r\n                        [&args...](_set& s,\
    \ const _set_iterator& it) {\r\n                          return s.emplace_hint(it,\
    \ std::forward<Args>(args)...);\r\n                        });\r\n  }\r\n  template<typename\
    \ It>\r\n  void insert(It begin, It end) {\r\n    using traits = std::iterator_traits<It>;\r\
    \n    static_assert(std::is_convertible<typename traits::value_type, element_type>::value,\
    \ \"\");\r\n    static_assert(std::is_base_of<std::forward_iterator_tag, typename\
    \ traits::iterator_category>::value, \"\");\r\n    for (auto it = begin; it !=\
    \ end; ++it)\r\n      insert(*it);\r\n  }\r\n  void insert(std::initializer_list<element_type>\
    \ list) {\r\n    insert(list.begin(), list.end());\r\n  }\r\n  iterator erase(iterator\
    \ it) {\r\n    if (it == end())\r\n      return it;\r\n    U x = _key_of(it);\r\
    \n    auto xit = it.xit_;\r\n    assert(xit != xft_.end());\r\n    auto& s = xit->second;\r\
    \n    auto sit = it.sit_;\r\n    size_--;\r\n    sit = s.erase(sit);\r\n    if\
    \ (x == xit.key()) {\r\n      if (x != MAX_KEY) {\r\n        if (!s.empty()) {\r\
    \n          auto& snext = std::next(xit)->second;\r\n          sit = snext.absorb(&s);\r\
    \n        }\r\n        xit = xft_.erase(xit);\r\n      } else {\r\n        return\
    \ end();\r\n      }\r\n    }\r\n    return iterator(this, xit, sit);\r\n  }\r\n\
    \  iterator erase(U x) {\r\n    auto it = lower_bound(x);\r\n    return (_key_of(it)\
    \ == x) ? erase(it) : it;\r\n  }\r\n\r\n public:\r\n  class iterator {\r\n   public:\r\
    \n    using value_type = element_type;\r\n    using pointer = element_type*;\r\
    \n    using reference = const element_type&;\r\n    using difference_type = long\
    \ long;\r\n    using iterator_category = std::bidirectional_iterator_tag;\r\n\
    \    using xiterator = typename _xft::iterator;\r\n    using siterator = _set_iterator;\r\
    \n   private:\r\n    const YFastTrie* yft_;\r\n    xiterator xit_;\r\n    siterator\
    \ sit_;\r\n    friend class YFastTrie;\r\n   public:\r\n    iterator(const YFastTrie*\
    \ yft, xiterator xit, siterator sit) : yft_(yft), xit_(xit), sit_(sit) {}\r\n\
    \    reference operator*() const { return *sit_; }\r\n    pointer operator->()\
    \ const { return &*sit_; }\r\n    bool operator==(const iterator& r) { return\
    \ sit_ == r.sit_; }\r\n    bool operator!=(const iterator& r) { return sit_ !=\
    \ r.sit_; }\r\n    iterator& operator++() {\r\n      if (++sit_ == xit_->second.end())\
    \ {\r\n        auto nxt = std::next(xit_);\r\n        if (nxt != yft_->xft_.end())\
    \ {\r\n          xit_ = nxt;\r\n          sit_ = xit_->second.begin();\r\n   \
    \     }\r\n      }\r\n      return *this;\r\n    }\r\n    iterator operator++(int)\
    \ {\r\n      iterator ret = *this;\r\n      ++this;\r\n      return ret;\r\n \
    \   }\r\n    iterator& operator--() {\r\n      if (sit_ == xit_->second.begin())\
    \ {\r\n        --xit_;\r\n        sit_ = std::prev(xit_->second.end());\r\n  \
    \    } else {\r\n        --sit_;\r\n      }\r\n      return *this;\r\n    }\r\n\
    \    iterator operator--(int) {\r\n      iterator ret = *this;\r\n      --this;\r\
    \n      return ret;\r\n    }\r\n  };\r\n  iterator begin() const {\r\n    auto\
    \ xit = xft_.begin();\r\n    return iterator(this, xit, xit->second.begin());\r\
    \n  }\r\n  iterator end() const {\r\n    auto xit = std::prev(xft_.end());\r\n\
    \    return iterator(this, xit, xit->second.end());\r\n  }\r\n};\r\ntemplate<typename\
    \ T, unsigned BITS>\r\nconstexpr int YFastTrie<T, BITS>::W;\r\ntemplate<typename\
    \ T, unsigned BITS>\r\nconstexpr typename YFastTrie<T, BITS>::U YFastTrie<T, BITS>::MAX_KEY;\r\
    \ntemplate<typename T, unsigned BITS>\r\nconstexpr bool YFastTrie<T, BITS>::kKeyOnly;\r\
    \ntemplate<unsigned BITS=64>\r\nusing YFastTrieSet = YFastTrie<void, BITS>;\r\n\
    \r\n\r\ntemplate<typename T, unsigned BITS=64>\r\nclass YFastTrieMAP : public\
    \ YFastTrie<T, BITS> {\r\n  using _base = YFastTrie<T, BITS>;\r\n public:\r\n\
    \  using typename _base::value_type;\r\n  using typename _base::U;\r\n  using\
    \ _base::kKeyOnly;\r\n  using reference = value_type&;\r\n  reference operator[](U\
    \ x) {\r\n    auto it = find(x);\r\n    if (it == _base::end())\r\n      it =\
    \ emplace(x);\r\n    if constexpr (kKeyOnly)\r\n      return *it;\r\n    else\r\
    \n      return it->second;\r\n  }\r\n};"
  dependsOn:
  - include/mtl/treap.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/integer_set.hpp
  requiredBy: []
  timestamp: '2022-12-18 04:26:00+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/integer_set.hpp
layout: document
redirect_from:
- /library/include/mtl/integer_set.hpp
- /library/include/mtl/integer_set.hpp.html
title: include/mtl/integer_set.hpp
---
