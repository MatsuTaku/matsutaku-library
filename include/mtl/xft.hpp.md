---
data:
  _extendedDependsOn:
  - icon: ':warning:'
    path: include/mtl/binary_trie.hpp
    title: include/mtl/binary_trie.hpp
  - icon: ':warning:'
    path: include/mtl/traits/set_traits.hpp
    title: include/mtl/traits/set_traits.hpp
  _extendedRequiredBy:
  - icon: ':warning:'
    path: include/mtl/yft.hpp
    title: include/mtl/yft.hpp
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/traits/set_traits.hpp\"\n#include <cstddef>\r\
    \n#include <initializer_list>\r\n#include <type_traits>\r\n#include <iterator>\r\
    \n\r\nnamespace traits {\r\n\r\ntemplate<typename T, typename M>\r\nstruct AssociativeArrayDefinition\
    \ {\r\n  using key_type = T;\r\n  using mapped_type = M;\r\n  using value_type\
    \ = std::pair<T const, M>;\r\n  using raw_key_type = typename std::remove_const<T>::type;\r\
    \n  using raw_mapped_type = typename std::remove_const<M>::type;\r\n  using init_type\
    \ = std::pair<raw_key_type, raw_mapped_type>;\r\n  using moved_type = std::pair<raw_key_type&&,\
    \ raw_mapped_type&&>;\r\n  template<class K, class V>\r\n  static inline key_type\
    \ const& key_of(std::pair<K,V> const& kv) {\r\n    return kv.first;\r\n  }\r\n\
    };\r\ntemplate<typename T>\r\nstruct AssociativeArrayDefinition<T, void> {\r\n\
    \  using key_type = T;\r\n  using value_type = T;\r\n  using init_type = T;\r\n\
    \  static inline key_type const& key_of(value_type const& k) { return k; }\r\n\
    };\r\n\r\n#if __cplusplus >= 202002L\r\n#include <concepts>\r\ntemplate<class\
    \ M>\r\nconcept IsAssociativeArray = requires (M m) {\r\n  typename M::key_type;\r\
    \n  typename M::value_type;\r\n  typename M::iterator;\r\n  {m.size()} -> std::convertible_to<size_t>;\r\
    \n  {m.empty()} -> std::same_as<bool>;\r\n  {m.clear()};\r\n  {m.begin()} -> std::same_as<typename\
    \ M::iterator>;\r\n  {m.end()} -> std::same_as<typename M::iterator>;\r\n};\r\n\
    #endif\r\n\r\ntemplate<class Base>\r\n#if __cplusplus >= 202002L\r\nrequires IsAssociativeArray<Base>\r\
    \n#endif\r\nclass SetTraitsBase : public Base {\r\n public:\r\n  using key_type\
    \ = typename Base::key_type;\r\n  using value_type = typename Base::value_type;\r\
    \n  using init_type = typename Base::init_type;\r\n  using iterator = typename\
    \ Base::iterator;\r\n  SetTraitsBase() = default;\r\n  template<typename InputIt>\r\
    \n  explicit SetTraitsBase(InputIt begin, InputIt end) : Base(begin, end) {\r\n\
    \    static_assert(std::is_convertible<typename std::iterator_traits<InputIt>::value_type,\
    \ value_type>::value, \"\");\r\n  }\r\n  SetTraitsBase(std::initializer_list<value_type>\
    \ init) : Base(init.begin(), init.end()) {}\r\n  using Base::size;\r\n  bool empty()\
    \ const { return size() == 0; }\r\n  using Base::clear;\r\n  using Base::begin;\r\
    \n  using Base::end;\r\n  using reverse_iterator = std::reverse_iterator<iterator>;\r\
    \n  inline reverse_iterator rbegin() const {\r\n    return reverse_iterator(begin());\r\
    \n  }\r\n  inline reverse_iterator rend() const {\r\n    return reverse_iterator(end());\r\
    \n  }\r\n  template<class Key>\r\n  inline iterator lower_bound(Key&& x) const\
    \ {\r\n    return Base::_lower_bound(std::forward<Key>(x));\r\n  }\r\n  inline\
    \ iterator lower_bound(const key_type& x) const {\r\n    return Base::_lower_bound(x);\r\
    \n  }\r\n  inline iterator lower_bound(key_type&& x) const {\r\n    return Base::_lower_bound(std::move(x));\r\
    \n  }\r\n  template<class Key>\r\n  inline iterator upper_bound(Key&& x) const\
    \ {\r\n    return Base::_upper_bound(std::forward<Key>(x));\r\n  }\r\n  inline\
    \ iterator upper_bound(const key_type& x) const {\r\n    return Base::_upper_bound(x);\r\
    \n  }\r\n  inline iterator upper_bound(key_type&& x) const {\r\n    return Base::_upper_bound(std::move(x));\r\
    \n  }\r\n  template<class Key>\r\n  inline iterator find(Key&& x) const {\r\n\
    \    return Base::_find(std::forward<Key>(x));\r\n  }\r\n  inline iterator find(const\
    \ key_type& x) const {\r\n    return Base::_find(x);\r\n  }\r\n  inline iterator\
    \ find(key_type&& x) const {\r\n    return Base::_find(std::move(x));\r\n  }\r\
    \n  template<class Key>\r\n  inline size_t count(Key&& x) const {\r\n    return\
    \ find(std::forward<Key>(x)) != end();\r\n  }\r\n  inline size_t count(const key_type&\
    \ x) const {\r\n    return find(x) != end();\r\n  }\r\n  inline size_t count(key_type&&\
    \ x) const {\r\n    return find(std::move(x)) != end();\r\n  }\r\n  inline std::pair<iterator,\
    \ bool> insert(const init_type& v) {\r\n    return Base::_insert(v);\r\n  }\r\n\
    \  inline std::pair<iterator, bool> insert(init_type&& v) {\r\n    return Base::_insert(std::move(v));\r\
    \n  }\r\n  template<typename=void>\r\n  inline std::pair<iterator, bool> insert(const\
    \ value_type& v) {\r\n    return Base::_insert(v);\r\n  }\r\n  template<typename=void>\r\
    \n  inline std::pair<iterator, bool> insert(value_type&& v) {\r\n    return Base::_insert(std::move(v));\r\
    \n  }\r\n  // TODO\r\n  template<class... Args>\r\n  inline std::pair<iterator,\
    \ bool> emplace(Args&&... args) {\r\n    using emplace_type = std::conditional<\r\
    \n        std::is_constructible<init_type, Args...>::value,\r\n            init_type,\r\
    \n            value_type\r\n        >::type;\r\n    return Base::_insert(emplace_type(std::forward<Args>(args)...));\r\
    \n  }\r\n  template<class Key>\r\n  inline bool erase(Key&& x) {\r\n    return\
    \ Base::_erase(std::forward<Key>(x));\r\n  }\r\n  inline bool erase(const key_type&\
    \ x) {\r\n    return Base::_erase(x);\r\n  }\r\n  inline bool erase(key_type&&\
    \ x) {\r\n    return Base::_erase(std::move(x));\r\n  }\r\n  inline iterator erase(iterator\
    \ it) {\r\n    return Base::_erase(it);\r\n  }\r\n};\r\n\r\ntemplate<typename\
    \ Base>\r\nusing SetTraits = SetTraitsBase<Base>;\r\n\r\ntemplate<typename Base>\r\
    \nclass MapTraits : public SetTraitsBase<Base> {\r\n  using SBase = SetTraitsBase<Base>;\r\
    \n public:\r\n  using typename SBase::key_type;\r\n  using typename SBase::mapped_type;\r\
    \n  using typename SBase::value_type;\r\n  using reference = mapped_type&;\r\n\
    \  MapTraits() = default;\r\n  template<typename InputIt>\r\n  explicit MapTraits(InputIt\
    \ begin, InputIt end) : SBase(begin, end) {}\r\n  MapTraits(std::initializer_list<value_type>\
    \ init) : SBase(init) {}\r\n  inline reference operator[](const key_type& x) {\r\
    \n    // TODO\r\n//    return SBase::try_emplace(x).first->second;\r\n    auto\
    \ it = SBase::insert({x, mapped_type()}).first;\r\n    return it->second;\r\n\
    \  }\r\n  inline reference operator[](key_type&& x) {\r\n    // TODO\r\n//   \
    \ return SBase::try_emplace(std::move(x)).first->second;\r\n    auto it = SBase::insert({std::move(x),\
    \ mapped_type()}).first;\r\n    return it->second;\r\n  }\r\n};\r\n\r\n} // namespace\
    \ traits\n#line 3 \"include/mtl/binary_trie.hpp\"\n#include <array>\r\n#include\
    \ <memory>\r\n#line 6 \"include/mtl/binary_trie.hpp\"\n#include <cstdint>\r\n\
    #line 8 \"include/mtl/binary_trie.hpp\"\n#include <algorithm>\r\n#include <cassert>\r\
    \n#include <bitset>\r\n#include <iostream>\r\nusing std::cerr;\r\nusing std::endl;\r\
    \n\r\ntemplate<typename T, typename M,\r\n    int8_t W = sizeof(T) * 8>\r\nclass\
    \ BinaryTrieBase : public traits::AssociativeArrayDefinition<T, M> {\r\n  static_assert(std::is_unsigned<T>::value,\
    \ \"\");\r\n public:\r\n  using types = traits::AssociativeArrayDefinition<T,\
    \ M>;\r\n  using key_type = typename types::key_type;\r\n  using value_type =\
    \ typename types::value_type;\r\n  using init_type = typename types::init_type;\r\
    \n  struct Node;\r\n  using node_ptr = std::shared_ptr<Node>;\r\n  using node_weak_ptr\
    \ = std::weak_ptr<Node>;\r\n  struct Leaf;\r\n  using leaf_ptr = std::shared_ptr<Leaf>;\r\
    \n  struct Node {\r\n    std::array<node_ptr, 2> c;\r\n    leaf_ptr jump;\r\n\
    \    node_weak_ptr parent;\r\n    Node() = default;\r\n    inline node_ptr& left()\
    \ { return c[0]; }\r\n    inline node_ptr& right()  { return c[1]; }\r\n  };\r\
    \n  struct Leaf : Node {\r\n    value_type v;\r\n    Leaf() = default;\r\n   \
    \ Leaf(const value_type& v) : Node(), v(v) {}\r\n    Leaf(value_type&& v) : Node(),\
    \ v(std::forward<value_type>(v)) {}\r\n    inline key_type key() const {\r\n \
    \     return types::key_of(v);\r\n    }\r\n    using Node::c;\r\n    inline leaf_ptr\
    \ prev() const {\r\n      return std::static_pointer_cast<Leaf>(c[0]);\r\n   \
    \ }\r\n    inline leaf_ptr next() const {\r\n      return std::static_pointer_cast<Leaf>(c[1]);\r\
    \n    }\r\n    inline void set_prev(leaf_ptr l) {\r\n      c[0] = std::static_pointer_cast<Node>(l);\r\
    \n    }\r\n    inline void set_next(leaf_ptr l) {\r\n      c[1] = std::static_pointer_cast<Node>(l);\r\
    \n    }\r\n  };\r\n protected:\r\n  node_ptr root_;\r\n  leaf_ptr dummy_;\r\n\
    \  size_t size_;\r\n  virtual inline void _init() {\r\n    root_ = create_node_at(0,\
    \ 0);\r\n    dummy_ = std::make_shared<Leaf>();\r\n    root_->jump = dummy_;\r\
    \n    dummy_->set_next(dummy_);\r\n    dummy_->set_prev(dummy_);\r\n    size_\
    \ = 0;\r\n  }\r\n  inline void _deinit() {\r\n    root_ = nullptr;\r\n    auto\
    \ u = dummy_->next();\r\n    dummy_->set_next(nullptr);\r\n    u->set_prev(nullptr);\r\
    \n    while (u != dummy_) {\r\n      auto n = u->next();\r\n      u->set_next(nullptr);\r\
    \n      n->set_prev(nullptr);\r\n      u = n;\r\n    }\r\n    dummy_ = nullptr;\r\
    \n  }\r\n public:\r\n  BinaryTrieBase() {\r\n    _init();\r\n  }\r\n  BinaryTrieBase(const\
    \ BinaryTrieBase& rhs) {\r\n    _insert_init(rhs.begin(), rhs.end());\r\n  }\r\
    \n  BinaryTrieBase& operator=(const BinaryTrieBase& rhs) {\r\n    _deinit();\r\
    \n    _insert_init(rhs.begin(), rhs.end());\r\n  }\r\n  BinaryTrieBase(BinaryTrieBase&&)\
    \ noexcept = default;\r\n  BinaryTrieBase& operator=(BinaryTrieBase&& rhs) noexcept\
    \ {\r\n    _deinit();\r\n    root_ = std::move(rhs.root_);\r\n    dummy_ = std::move(rhs.dummy_);\r\
    \n    size_ = std::move(rhs.size_);\r\n  }\r\n  ~BinaryTrieBase() {\r\n    _deinit();\r\
    \n  }\r\n protected:\r\n  template<class InputIt>\r\n  void _insert_init(InputIt\
    \ begin, InputIt end) {\r\n    static_assert(std::is_convertible<typename std::iterator_traits<InputIt>::value_type,\
    \ value_type>::value, \"\");\r\n    _init();\r\n    if (begin == end) return;\r\
    \n    if (!std::is_sorted(begin, end, [](auto& l, auto& r) {\r\n      return types::key_of(l)\
    \ < types::key_of(r);\r\n    })) {\r\n      for (auto it = begin; it != end; ++it)\r\
    \n        _insert(*it);\r\n      return;\r\n    }\r\n    auto push_link = [&](leaf_ptr\
    \ l) {\r\n      auto b = dummy_->prev();\r\n      l->set_prev(b);\r\n      l->set_next(dummy_);\r\
    \n      l->prev()->set_next(l);\r\n      l->next()->set_prev(l);\r\n    };\r\n\
    \    std::array<node_ptr, W> us{};\r\n    auto grow = [&](key_type x, int k, leaf_ptr\
    \ l) {\r\n      for (int i = k; i < W-1; i++) {\r\n        us[i+1] = create_node_at(x,\
    \ i+1);\r\n        int c = (x >> (W-i-1)) & 1;\r\n        us[i]->c[c] = us[i+1];\r\
    \n        us[i+1]->parent = us[i];\r\n        us[i+1]->jump = l;\r\n      }\r\n\
    \      int c = x & 1;\r\n      us[W-1]->c[c] = l;\r\n      l->parent = us[W-1];\r\
    \n    };\r\n    us[0] = root_;\r\n    key_type x = types::key_of(*begin);\r\n\
    \    auto l = create_leaf_at(x, *begin);\r\n    push_link(l);\r\n    us[0]->jump\
    \ = l;\r\n    grow(x, 0, l);\r\n    size_t sz = 1;\r\n    for (auto it = std::next(begin);\
    \ it != end; ++it) {\r\n      key_type px = x;\r\n      x = types::key_of(*it);\r\
    \n      auto m = x ^ px;\r\n      if (m == 0) continue;\r\n//      [[assume(m\
    \ != 0)]]\r\n      int k = W-1;\r\n      while (m > 1) {\r\n        m >>= 1;\r\
    \n        --k;\r\n      }\r\n      l = create_leaf_at(x, *it);\r\n      push_link(l);\r\
    \n      for (int i = 0; i < k; i++)\r\n        if (!us[i]->c[1]) us[i]->jump =\
    \ l;\r\n      us[k]->jump = nullptr;\r\n      grow(x, k, l);\r\n      ++sz;\r\n\
    \    }\r\n    size_ = sz;\r\n  }\r\n public:\r\n  template<typename InputIt>\r\
    \n  explicit BinaryTrieBase(InputIt begin, InputIt end) {\r\n    _insert_init(begin,\
    \ end);\r\n  }\r\n  inline size_t size() const {\r\n    return size_;\r\n  }\r\
    \n  inline bool empty() const { return size() == 0; }\r\n  inline void clear()\
    \ {\r\n    _deinit();\r\n    _init();\r\n  }\r\n  struct iterator;\r\n  inline\
    \ iterator begin() const {\r\n    return iterator(dummy_->next());\r\n  }\r\n\
    \  inline iterator end() const {\r\n    return iterator(dummy_);\r\n  }\r\n protected:\r\
    \n  virtual inline std::pair<int, node_ptr> _traverse(const key_type& key) const\
    \ {\r\n    int i, c;\r\n    key_type x = key;\r\n    auto u = root_;\r\n    for\
    \ (i = 0; i < W; i++) {\r\n      c = (x >> (W-i-1)) & 1;\r\n      if (!u->c[c])\
    \ break;\r\n      u = u->c[c];\r\n    }\r\n    return std::make_pair(i, u);\r\n\
    \  }\r\n  template<typename Key>\r\n  inline iterator _lower_bound(Key&& key)\
    \ const {\r\n    static_assert(std::is_convertible<Key, key_type>::value, \"\"\
    );\r\n    key_type x = std::forward<Key>(key);\r\n    auto reached = _traverse(x);\r\
    \n    int i = reached.first;\r\n    node_ptr u = reached.second;\r\n    if (i\
    \ == W) return iterator(std::static_pointer_cast<Leaf>(u));\r\n    auto l = (((x\
    \ >> (W-i-1)) & 1) == 0) ? u->jump : u->jump->next();\r\n    return iterator(l);\r\
    \n  }\r\n protected:\r\n  template<typename Key>\r\n  inline iterator _upper_bound(Key&&\
    \ key) const {\r\n    static_assert(std::is_convertible<Key, key_type>::value,\
    \ \"\");\r\n    key_type x = std::forward<Key>(key);\r\n    auto it = _lower_bound(x);\r\
    \n    if (types::key_of(*it) == x)\r\n      ++it;\r\n    return it;\r\n  }\r\n\
    \  template<typename Key>\r\n  inline iterator _find(Key&& key) const {\r\n  \
    \  static_assert(std::is_convertible<Key, key_type>::value, \"\");\r\n    key_type\
    \ x = std::forward<Key>(key);\r\n    auto reached = _traverse(x);\r\n    int i\
    \ = reached.first;\r\n    node_ptr u = reached.second;\r\n    if (i == W)\r\n\
    \      return iterator(std::static_pointer_cast<Leaf>(u));\r\n    else\r\n   \
    \   return end();\r\n  }\r\n protected:\r\n  virtual inline node_ptr create_node_at(const\
    \ key_type&, int) {\r\n    return std::make_shared<Node>();\r\n  }\r\n  virtual\
    \ inline leaf_ptr create_leaf_at(const key_type&, const init_type& value) {\r\n\
    \    return std::make_shared<Leaf>(value);\r\n  }\r\n  virtual inline leaf_ptr\
    \ create_leaf_at(const key_type&, init_type&& value) {\r\n    return std::make_shared<Leaf>(std::move(value));\r\
    \n  }\r\n  template<typename Value>\r\n  inline std::pair<iterator, bool> _insert(Value&&\
    \ value) {\r\n    static_assert(std::is_convertible<Value, value_type>::value,\
    \ \"\");\r\n    key_type x = types::key_of(value);\r\n    auto reached = _traverse(x);\r\
    \n    int i = reached.first;\r\n    node_ptr u = reached.second;\r\n    if (i\
    \ == W)\r\n      return std::make_pair(iterator(std::static_pointer_cast<Leaf>(u)),\
    \ false);\r\n    auto f = u;\r\n    int c = (x >> (W-i-1)) & 1;\r\n    auto fc\
    \ = c;\r\n    auto fi = i;\r\n    auto pred = c == 1 ? u->jump : u->jump->prev();\r\
    \n    u->jump = nullptr;\r\n    auto l = create_leaf_at(x, std::forward<Value>(value));\r\
    \n    l->set_prev(pred);\r\n    l->set_next(pred->next());\r\n    l->prev()->set_next(l);\r\
    \n    l->next()->set_prev(l);\r\n    for (; i < W-1; i++) {\r\n      c = (x >>\
    \ (W-i-1)) & 1;\r\n      u->c[c] = create_node_at(x, i+1);\r\n      u->c[c]->parent\
    \ = u;\r\n      u->c[c]->jump = l;\r\n      u = u->c[c];\r\n    }\r\n    {\r\n\
    \      c = (x >> (W-i-1)) & 1;\r\n      u->c[c] = l;\r\n      u->c[c]->parent\
    \ = u;\r\n    }\r\n    if (f == root_) [[unlikely]] {\r\n      f->jump = l;\r\n\
    \    } else [[likely]] {\r\n      auto v = f->parent.lock();\r\n      fi--;\r\n\
    \      while (v) {\r\n        c = x >> (W-fi-1) & 1;\r\n        if (c != fc and\
    \ !v->jump)\r\n          break;\r\n        if (!v->c[fc])\r\n          v->jump\
    \ = l;\r\n        v = v->parent.lock();\r\n        fi--;\r\n      }\r\n    }\r\
    \n    size_++;\r\n    return std::make_pair(iterator(l), true);\r\n  }\r\n\r\n\
    \  virtual void erase_node_at(const key_type&, int, node_ptr) {}\r\n  template<typename\
    \ Key>\r\n  bool _erase(Key&& x) {\r\n    static_assert(std::is_convertible<Key,\
    \ key_type>::value, \"\");\r\n    auto it = _find(x);\r\n    if (it != end())\
    \ {\r\n      _erase(it);\r\n      return true;\r\n    } else {\r\n      return\
    \ false;\r\n    }\r\n  }\r\n  template<typename Key>\r\n  inline iterator _erase_from_leaf(Key&&\
    \ key, leaf_ptr l) {\r\n    static_assert(std::is_convertible<Key, key_type>::value,\
    \ \"\");\r\n    key_type x = std::forward<Key>(key);\r\n    assert(x == l->key());\r\
    \n    l->prev()->set_next(l->next());\r\n    l->next()->set_prev(l->prev());\r\
    \n    int i,c;\r\n    auto v = std::static_pointer_cast<Node>(l);\r\n    for (i\
    \ = W-1; i >= 0; i--) {\r\n      erase_node_at(x, i+1, v);\r\n      v = v->parent.lock();\r\
    \n      c = (x >> (W-i-1)) & 1;\r\n      v->c[c] = nullptr;\r\n      if (v->c[c^1])\
    \ break;\r\n    }\r\n    auto nj = c ? l->prev() : l->next();\r\n    auto fc =\
    \ c;\r\n    v->jump = nj;\r\n    v = v->parent.lock();\r\n    i--;\r\n    for\
    \ (; i >= 0; i--) {\r\n      assert(v);\r\n      c = (x >> (W-i-1)) & 1;\r\n \
    \     if (c != fc) {\r\n        if (!v->jump) break;\r\n        v->jump = nj;\r\
    \n      }\r\n      v = v->parent.lock();\r\n    }\r\n    size_--;\r\n    return\
    \ iterator(l->next());\r\n  }\r\n  inline iterator _erase(iterator it) {\r\n \
    \   if (it == end()) return it;\r\n    return _erase_from_leaf(types::key_of(*it),\
    \ it.ptr_);\r\n  }\r\n};\r\ntemplate<typename T, typename M, int8_t W>\r\nstruct\
    \ BinaryTrieBase<T,M,W>::iterator {\r\n  using difference_type = ptrdiff_t;\r\n\
    \  using value_type = BinaryTrieBase::value_type;\r\n  using pointer = value_type*;\r\
    \n  using reference = value_type&;\r\n  using iterator_category = std::bidirectional_iterator_tag;\r\
    \n  leaf_ptr ptr_;\r\n  iterator(leaf_ptr p) : ptr_(p) {}\r\n  inline reference\
    \ operator*() {\r\n    return ptr_->v;\r\n  }\r\n  inline pointer operator->()\
    \ {\r\n    return &(ptr_->v);\r\n  }\r\n  inline bool operator==(const iterator&\
    \ rhs) const {\r\n    return ptr_ == rhs.ptr_;\r\n  }\r\n  inline bool operator!=(const\
    \ iterator& rhs) const {\r\n    return !operator==(rhs);\r\n  }\r\n  inline iterator&\
    \ operator++() {\r\n    ptr_ = ptr_->next();\r\n    return *this;\r\n  }\r\n \
    \ inline iterator operator++(int) const {\r\n    iterator ret = *this;\r\n   \
    \ operator++();\r\n    return ret;\r\n  }\r\n  inline iterator& operator--() {\r\
    \n    ptr_ = ptr_->prev();\r\n    return *this;\r\n  }\r\n  inline iterator operator--(int)\
    \ const {\r\n    iterator ret = *this;\r\n    operator--();\r\n    return ret;\r\
    \n  }\r\n};\r\n\r\ntemplate<typename T, typename V, uint8_t W = sizeof(T)*8>\r\
    \nusing BinaryTrie = traits::MapTraits<BinaryTrieBase<T, V, W>>;\r\ntemplate<typename\
    \ T, uint8_t W = sizeof(T)*8>\r\nusing BinaryTrieSet = traits::SetTraits<BinaryTrieBase<T,\
    \ void, W>>;\r\ntemplate<typename T, typename V, uint8_t W = sizeof(T)*8>\r\n\
    using BinaryTrieMap = BinaryTrie<T, V, W>;\r\n#line 7 \"include/mtl/xft.hpp\"\n\
    #include <unordered_map>\r\n#line 9 \"include/mtl/xft.hpp\"\n\r\ntemplate<class\
    \ T, class M, int8_t W>\r\nusing XFastTrieHashTableMappedType = typename BinaryTrieBase<T,\
    \ M, W>::node_ptr;\r\n\r\ntemplate<typename T, typename M,\r\n    int8_t W = sizeof(T)\
    \ * 8,\r\n    class HashTable = std::unordered_map<T, XFastTrieHashTableMappedType<T,\
    \ M, W>>>\r\nclass XFastTrieBase : public BinaryTrieBase<T, M, W> {\r\n  static_assert(std::is_unsigned<T>::value,\
    \ \"\");\r\n  using Base = BinaryTrieBase<T, M, W>;\r\n public:\r\n  using hash_table_type\
    \ = HashTable;\r\n  using types = typename Base::types;\r\n  using value_type\
    \ = typename types::value_type;\r\n  using init_type = typename types::init_type;\r\
    \n  using typename Base::Node;\r\n  using typename Base::Leaf;\r\n  using typename\
    \ Base::node_ptr;\r\n  using typename Base::leaf_ptr;\r\n  using typename Base::key_type;\r\
    \n protected:\r\n  using Base::root_;\r\n  using Base::dummy_;\r\n  using Base::size_;\r\
    \n  std::array<hash_table_type, W+1> tb_;\r\n  inline void _init() override {\r\
    \n    for (auto& t:tb_) t.clear();\r\n    Base::_init();\r\n  }\r\n public:\r\n\
    \  XFastTrieBase() : Base() {}\r\n  XFastTrieBase(const XFastTrieBase&) = default;\r\
    \n  XFastTrieBase& operator=(const XFastTrieBase&) = default;\r\n  XFastTrieBase(XFastTrieBase&&)\
    \ noexcept = default;\r\n  XFastTrieBase& operator=(XFastTrieBase&& rhs) {\r\n\
    \    Base::operator=(std::move((Base&&)rhs));\r\n    tb_ = std::move(rhs.tb_);\r\
    \n  }\r\n  template<typename InputIt>\r\n  explicit XFastTrieBase(InputIt begin,\
    \ InputIt end) {\r\n    Base::_insert_init(begin, end);\r\n  }\r\n  using iterator\
    \ = typename Base::iterator;\r\n  using Base::end;\r\n protected:\r\n  inline\
    \ std::pair<int, node_ptr> _traverse(const key_type& key) const override {\r\n\
    \    key_type x = key;\r\n    int l = 0, h = W+1;\r\n    auto u = root_;\r\n \
    \   while (l+1 < h) {\r\n      int i = l+(h-l)/2;\r\n      auto p = x >> (W-i);\r\
    \n      auto it = tb_[i].find(p);\r\n      if (it != tb_[i].end()) {\r\n     \
    \   l = i;\r\n        u = it->second;\r\n      } else {\r\n        h = i;\r\n\
    \      }\r\n    }\r\n    return std::make_pair(l, u);\r\n  }\r\n  template<class\
    \ Key>\r\n  inline iterator _lower_bound(Key&& key) const {\r\n    auto reached\
    \ = _traverse(std::forward<Key>(key));\r\n    int& l = reached.first;\r\n    node_ptr&\
    \ u = reached.second;\r\n    if (l == W) return iterator(std::static_pointer_cast<Leaf>(u));\r\
    \n    key_type x = std::forward<Key>(key);\r\n    auto lb = ((x >> (W-l-1)) &\
    \ 1) == 0 ? u->jump : u->jump->next();\r\n    return lb == dummy_ ? end() : iterator(lb);\r\
    \n  }\r\n  template<class Key>\r\n  inline iterator _upper_bound(Key&& key) const\
    \ {\r\n    auto reached = _traverse(std::forward<Key>(key));\r\n    int& l = reached.first;\r\
    \n    node_ptr& u = reached.second;\r\n    if (l == W) return iterator(std::static_pointer_cast<Leaf>(u)->next());\r\
    \n    key_type x = std::forward<Key>(key);\r\n    auto lb = ((x >> (W-l-1)) &\
    \ 1) == 0 ? u->jump : u->jump->next();\r\n    return lb == dummy_ ? end() : iterator(lb);\r\
    \n  }\r\n  template<class Key>\r\n  inline iterator _find(Key&& key) const {\r\
    \n    key_type x = std::forward<Key>(key);\r\n    auto it = tb_[W].find(x);\r\n\
    \    if (it != tb_[W].end())\r\n      return iterator(std::static_pointer_cast<Leaf>(it->second));\r\
    \n    else\r\n      return end();\r\n  }\r\n  inline void _store_node(const int\
    \ i, const key_type& x, node_ptr u) {\r\n    tb_[i].emplace(x >> (W-i), u);\r\n\
    \  }\r\n  inline node_ptr create_node_at(const key_type& x, int i) override {\r\
    \n    auto u = Base::create_node_at(x, i);\r\n    _store_node(i, x, u);\r\n  \
    \  return u;\r\n  }\r\n  inline leaf_ptr create_leaf_at(const key_type& x, const\
    \ init_type& value) override {\r\n    auto l = Base::create_leaf_at(x, value);\r\
    \n    _store_node(W, x, std::static_pointer_cast<Node>(l));\r\n    return l;\r\
    \n  }\r\n  inline leaf_ptr create_leaf_at(const key_type& x, init_type&& value)\
    \ override {\r\n    auto l = Base::create_leaf_at(x, std::move(value));\r\n  \
    \  _store_node(W, x, std::static_pointer_cast<Node>(l));\r\n    return l;\r\n\
    \  }\r\n  using Base::_insert;\r\n  inline void erase_node_at(const key_type&\
    \ x, int i, node_ptr u) override {\r\n    Base::erase_node_at(x, i, u);\r\n  \
    \  auto it = tb_[i].find(x >> (W-i));\r\n    assert(it != tb_[i].end());\r\n \
    \   assert(it->second == u);\r\n    tb_[i].erase(it);\r\n  }\r\n  using Base::_erase;\r\
    \n  template<class Key>\r\n  inline bool _erase(Key&& key) {\r\n    key_type x\
    \ = std::forward<Key>(key);\r\n    auto it = tb_[W].find(x);\r\n    if (it !=\
    \ tb_[W].end()) {\r\n      Base::_erase_from_leaf(x, std::static_pointer_cast<Leaf>(it->second));\r\
    \n      return true;\r\n    } else {\r\n      return false;\r\n    }\r\n  }\r\n\
    };\r\n\r\n#line 141 \"include/mtl/xft.hpp\"\n\r\ntemplate<typename T, typename\
    \ V, uint8_t W = sizeof(T)*8>\r\nusing XFastTrie = traits::MapTraits<XFastTrieBase<T,\
    \ V, W>>;\r\ntemplate<typename T, uint8_t W = sizeof(T)*8>\r\nusing XFastTrieSet\
    \ = traits::SetTraits<XFastTrieBase<T, void, W>>;\r\ntemplate<typename T, typename\
    \ V, uint8_t W = sizeof(T)*8>\r\nusing XFastTrieMap = XFastTrie<T, V, W>;\r\n"
  code: "#pragma once\r\n#include \"binary_trie.hpp\"\r\n#include <array>\r\n#include\
    \ <memory>\r\n#include <type_traits>\r\n#include <cstdint>\r\n#include <unordered_map>\r\
    \n#include <cassert>\r\n\r\ntemplate<class T, class M, int8_t W>\r\nusing XFastTrieHashTableMappedType\
    \ = typename BinaryTrieBase<T, M, W>::node_ptr;\r\n\r\ntemplate<typename T, typename\
    \ M,\r\n    int8_t W = sizeof(T) * 8,\r\n    class HashTable = std::unordered_map<T,\
    \ XFastTrieHashTableMappedType<T, M, W>>>\r\nclass XFastTrieBase : public BinaryTrieBase<T,\
    \ M, W> {\r\n  static_assert(std::is_unsigned<T>::value, \"\");\r\n  using Base\
    \ = BinaryTrieBase<T, M, W>;\r\n public:\r\n  using hash_table_type = HashTable;\r\
    \n  using types = typename Base::types;\r\n  using value_type = typename types::value_type;\r\
    \n  using init_type = typename types::init_type;\r\n  using typename Base::Node;\r\
    \n  using typename Base::Leaf;\r\n  using typename Base::node_ptr;\r\n  using\
    \ typename Base::leaf_ptr;\r\n  using typename Base::key_type;\r\n protected:\r\
    \n  using Base::root_;\r\n  using Base::dummy_;\r\n  using Base::size_;\r\n  std::array<hash_table_type,\
    \ W+1> tb_;\r\n  inline void _init() override {\r\n    for (auto& t:tb_) t.clear();\r\
    \n    Base::_init();\r\n  }\r\n public:\r\n  XFastTrieBase() : Base() {}\r\n \
    \ XFastTrieBase(const XFastTrieBase&) = default;\r\n  XFastTrieBase& operator=(const\
    \ XFastTrieBase&) = default;\r\n  XFastTrieBase(XFastTrieBase&&) noexcept = default;\r\
    \n  XFastTrieBase& operator=(XFastTrieBase&& rhs) {\r\n    Base::operator=(std::move((Base&&)rhs));\r\
    \n    tb_ = std::move(rhs.tb_);\r\n  }\r\n  template<typename InputIt>\r\n  explicit\
    \ XFastTrieBase(InputIt begin, InputIt end) {\r\n    Base::_insert_init(begin,\
    \ end);\r\n  }\r\n  using iterator = typename Base::iterator;\r\n  using Base::end;\r\
    \n protected:\r\n  inline std::pair<int, node_ptr> _traverse(const key_type& key)\
    \ const override {\r\n    key_type x = key;\r\n    int l = 0, h = W+1;\r\n   \
    \ auto u = root_;\r\n    while (l+1 < h) {\r\n      int i = l+(h-l)/2;\r\n   \
    \   auto p = x >> (W-i);\r\n      auto it = tb_[i].find(p);\r\n      if (it !=\
    \ tb_[i].end()) {\r\n        l = i;\r\n        u = it->second;\r\n      } else\
    \ {\r\n        h = i;\r\n      }\r\n    }\r\n    return std::make_pair(l, u);\r\
    \n  }\r\n  template<class Key>\r\n  inline iterator _lower_bound(Key&& key) const\
    \ {\r\n    auto reached = _traverse(std::forward<Key>(key));\r\n    int& l = reached.first;\r\
    \n    node_ptr& u = reached.second;\r\n    if (l == W) return iterator(std::static_pointer_cast<Leaf>(u));\r\
    \n    key_type x = std::forward<Key>(key);\r\n    auto lb = ((x >> (W-l-1)) &\
    \ 1) == 0 ? u->jump : u->jump->next();\r\n    return lb == dummy_ ? end() : iterator(lb);\r\
    \n  }\r\n  template<class Key>\r\n  inline iterator _upper_bound(Key&& key) const\
    \ {\r\n    auto reached = _traverse(std::forward<Key>(key));\r\n    int& l = reached.first;\r\
    \n    node_ptr& u = reached.second;\r\n    if (l == W) return iterator(std::static_pointer_cast<Leaf>(u)->next());\r\
    \n    key_type x = std::forward<Key>(key);\r\n    auto lb = ((x >> (W-l-1)) &\
    \ 1) == 0 ? u->jump : u->jump->next();\r\n    return lb == dummy_ ? end() : iterator(lb);\r\
    \n  }\r\n  template<class Key>\r\n  inline iterator _find(Key&& key) const {\r\
    \n    key_type x = std::forward<Key>(key);\r\n    auto it = tb_[W].find(x);\r\n\
    \    if (it != tb_[W].end())\r\n      return iterator(std::static_pointer_cast<Leaf>(it->second));\r\
    \n    else\r\n      return end();\r\n  }\r\n  inline void _store_node(const int\
    \ i, const key_type& x, node_ptr u) {\r\n    tb_[i].emplace(x >> (W-i), u);\r\n\
    \  }\r\n  inline node_ptr create_node_at(const key_type& x, int i) override {\r\
    \n    auto u = Base::create_node_at(x, i);\r\n    _store_node(i, x, u);\r\n  \
    \  return u;\r\n  }\r\n  inline leaf_ptr create_leaf_at(const key_type& x, const\
    \ init_type& value) override {\r\n    auto l = Base::create_leaf_at(x, value);\r\
    \n    _store_node(W, x, std::static_pointer_cast<Node>(l));\r\n    return l;\r\
    \n  }\r\n  inline leaf_ptr create_leaf_at(const key_type& x, init_type&& value)\
    \ override {\r\n    auto l = Base::create_leaf_at(x, std::move(value));\r\n  \
    \  _store_node(W, x, std::static_pointer_cast<Node>(l));\r\n    return l;\r\n\
    \  }\r\n  using Base::_insert;\r\n  inline void erase_node_at(const key_type&\
    \ x, int i, node_ptr u) override {\r\n    Base::erase_node_at(x, i, u);\r\n  \
    \  auto it = tb_[i].find(x >> (W-i));\r\n    assert(it != tb_[i].end());\r\n \
    \   assert(it->second == u);\r\n    tb_[i].erase(it);\r\n  }\r\n  using Base::_erase;\r\
    \n  template<class Key>\r\n  inline bool _erase(Key&& key) {\r\n    key_type x\
    \ = std::forward<Key>(key);\r\n    auto it = tb_[W].find(x);\r\n    if (it !=\
    \ tb_[W].end()) {\r\n      Base::_erase_from_leaf(x, std::static_pointer_cast<Leaf>(it->second));\r\
    \n      return true;\r\n    } else {\r\n      return false;\r\n    }\r\n  }\r\n\
    };\r\n\r\n#include \"traits/set_traits.hpp\"\r\n\r\ntemplate<typename T, typename\
    \ V, uint8_t W = sizeof(T)*8>\r\nusing XFastTrie = traits::MapTraits<XFastTrieBase<T,\
    \ V, W>>;\r\ntemplate<typename T, uint8_t W = sizeof(T)*8>\r\nusing XFastTrieSet\
    \ = traits::SetTraits<XFastTrieBase<T, void, W>>;\r\ntemplate<typename T, typename\
    \ V, uint8_t W = sizeof(T)*8>\r\nusing XFastTrieMap = XFastTrie<T, V, W>;\r\n"
  dependsOn:
  - include/mtl/binary_trie.hpp
  - include/mtl/traits/set_traits.hpp
  isVerificationFile: false
  path: include/mtl/xft.hpp
  requiredBy:
  - include/mtl/yft.hpp
  timestamp: '2022-12-22 12:22:12+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/xft.hpp
layout: document
redirect_from:
- /library/include/mtl/xft.hpp
- /library/include/mtl/xft.hpp.html
title: include/mtl/xft.hpp
---
