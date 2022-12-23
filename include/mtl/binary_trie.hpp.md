---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':warning:'
    path: include/mtl/xft.hpp
    title: include/mtl/xft.hpp
  - icon: ':warning:'
    path: include/mtl/yft.hpp
    title: include/mtl/yft.hpp
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/binary_trie.hpp\"\n#include <array>\r\n#include\
    \ <memory>\r\n#include <type_traits>\r\n#include <cstdint>\r\n#include <initializer_list>\r\
    \n#include <cassert>\r\n#include <bitset>\r\n#include <iostream>\r\nusing std::cerr;\r\
    \nusing std::endl;\r\n\r\ntemplate<typename T, typename M>\r\nstruct AssociativeArrayDefinition\
    \ {\r\n  static constexpr bool kPairValue = true;\r\n  using key_type = T;\r\n\
    \  using mapped_type = M;\r\n  using value_type = std::pair<T const, M>;\r\n \
    \ using raw_key_type = typename std::remove_reference<T>::type;\r\n  using raw_mapped_type\
    \ = typename std::remove_reference<M>::type;\r\n  using init_type = std::pair<raw_key_type,\
    \ raw_mapped_type>;\r\n  using moved_type = std::pair<raw_key_type&&, raw_mapped_type&&>;\r\
    \n  template<class K, class V>\r\n  static inline key_type const& key_of(std::pair<K,V>\
    \ const& kv) {\r\n    return kv.first;\r\n  }\r\n};\r\ntemplate<typename T>\r\n\
    struct AssociativeArrayDefinition<T, void> {\r\n  static constexpr bool kPairValue\
    \ = false;\r\n  using key_type = T;\r\n  using value_type = T;\r\n  using init_type\
    \ = T;\r\n  static inline key_type const& key_of(value_type const& k) { return\
    \ k; }\r\n};\r\n\r\ntemplate<typename T, typename M>\r\nstruct BinaryTrieDefinition\
    \ : AssociativeArrayDefinition<T, M> {\r\n  static_assert(std::is_unsigned<T>::value,\
    \ \"\");\r\n  using Base = AssociativeArrayDefinition<T, M>;\r\n  struct Node;\r\
    \n  using node_ptr = std::shared_ptr<Node>;\r\n  using node_weak_ptr = std::weak_ptr<Node>;\r\
    \n  struct Leaf;\r\n  using leaf_ptr = std::shared_ptr<Leaf>;\r\n  struct Node\
    \ {\r\n    std::array<node_ptr, 2> c;\r\n    leaf_ptr jump;\r\n    node_weak_ptr\
    \ parent;\r\n    Node() = default;\r\n    inline node_ptr& left() { return c[0];\
    \ }\r\n    inline node_ptr& right()  { return c[1]; }\r\n  };\r\n  using typename\
    \ Base::key_type;\r\n  using typename Base::value_type;\r\n  struct Leaf : Node\
    \ {\r\n    value_type v;\r\n    Leaf() = default;\r\n    Leaf(const value_type&\
    \ v) : Node(), v(v) {}\r\n    Leaf(value_type&& v) : Node(), v(std::forward<value_type>(v))\
    \ {}\r\n    inline key_type key() const {\r\n      return Base::key_of(v);\r\n\
    \    }\r\n    using Node::c;\r\n    inline leaf_ptr prev() const {\r\n      return\
    \ std::static_pointer_cast<Leaf>(c[0]);\r\n    }\r\n    inline leaf_ptr next()\
    \ const {\r\n      return std::static_pointer_cast<Leaf>(c[1]);\r\n    }\r\n \
    \   inline void set_prev(leaf_ptr l) {\r\n      c[0] = std::static_pointer_cast<Node>(l);\r\
    \n    }\r\n    inline void set_next(leaf_ptr l) {\r\n      c[1] = std::static_pointer_cast<Node>(l);\r\
    \n    }\r\n  };\r\n};\r\n\r\ntemplate<typename T, typename M,\r\n    int8_t W\
    \ = sizeof(T) * 8>\r\nclass BinaryTrieBase : public BinaryTrieDefinition<T, M>\
    \ {\r\n  static_assert(std::is_unsigned<T>::value, \"\");\r\n  using Def = BinaryTrieDefinition<T,\
    \ M>;\r\n public:\r\n  using key_type = typename Def::key_type;\r\n  static constexpr\
    \ bool kPairValue = Def::kPairValue;\r\n  using value_type = typename Def::value_type;\r\
    \n  using Node = typename Def::Node;\r\n  using node_ptr = typename Def::node_ptr;\r\
    \n  using node_weak_ptr = typename Def::node_weak_ptr;\r\n  using Leaf = typename\
    \ Def::Leaf;\r\n  using leaf_ptr = typename Def::leaf_ptr;\r\n protected:\r\n\
    \  node_ptr root_;\r\n  leaf_ptr dummy_;\r\n  size_t size_;\r\n  virtual void\
    \ _init() {\r\n    root_ = std::make_shared<Node>();\r\n    dummy_ = std::make_shared<Leaf>();\r\
    \n    root_->jump = dummy_;\r\n    dummy_->set_next(dummy_);\r\n    dummy_->set_prev(dummy_);\r\
    \n    size_ = 0;\r\n  }\r\n public:\r\n  BinaryTrieBase() {\r\n    _init();\r\n\
    \  }\r\n  ~BinaryTrieBase() {\r\n    root_ = nullptr;\r\n    dummy_->set_next(nullptr);\r\
    \n    dummy_->set_prev(nullptr);\r\n    dummy_ = nullptr;\r\n  }\r\n  template<typename\
    \ InputIt>\r\n  explicit BinaryTrieBase(InputIt begin, InputIt end) : BinaryTrieBase()\
    \ {\r\n    static_assert(std::is_convertible<typename std::iterator_traits<InputIt>::value_type,\
    \ value_type>::value, \"\");\r\n    // TODO: optimize\r\n    for (auto it = begin;\
    \ it != end; ++it)\r\n      _insert(*it);\r\n  }\r\n  BinaryTrieBase(std::initializer_list<value_type>\
    \ init) : BinaryTrieBase(init.begin(), init.end()) {}\r\n  inline size_t size()\
    \ const {\r\n    return size_;\r\n  }\r\n  inline bool empty() const { return\
    \ size() == 0; }\r\n  struct iterator;\r\n  inline iterator begin() const {\r\n\
    \    return iterator(dummy_->next());\r\n  }\r\n  inline iterator end() const\
    \ {\r\n    return iterator(dummy_);\r\n  }\r\n protected:\r\n  template<typename\
    \ Key>\r\n  inline iterator _lower_bound(Key&& key) const {\r\n    static_assert(std::is_convertible<Key,\
    \ key_type>::value, \"\");\r\n    key_type x = std::forward<Key>(key);\r\n   \
    \ int i, c;\r\n    auto u = root_;\r\n    for (i = 0; i < W; i++) {\r\n      c\
    \ = (x >> (W-i-1)) & 1;\r\n      if (!u->c[c]) break;\r\n      u = u->c[c];\r\n\
    \    }\r\n    if (i == W) return iterator(std::static_pointer_cast<Leaf>(u));\r\
    \n    auto l = (c == 0) ? u->jump : u->jump->next();\r\n    return iterator(l);\r\
    \n  }\r\n protected:\r\n  template<typename Key>\r\n  inline iterator _upper_bound(Key&&\
    \ key) const {\r\n    static_assert(std::is_convertible<Key, key_type>::value,\
    \ \"\");\r\n    key_type x = std::forward<Key>(key);\r\n    auto it = _lower_bound(x);\r\
    \n    if (Def::key_of(*it) == x)\r\n      ++it;\r\n    return it;\r\n  }\r\n \
    \ template<typename Key>\r\n  inline iterator _find(Key&& key) const {\r\n   \
    \ static_assert(std::is_convertible<Key, key_type>::value, \"\");\r\n    key_type\
    \ x = std::forward<Key>(key);\r\n    int i, c;\r\n    auto u = root_;\r\n    for\
    \ (i = 0; i < W; i++) {\r\n      c = (x >> (W-i-1)) & 1;\r\n      if (!u->c[c])\
    \ return end();\r\n      u = u->c[c];\r\n    }\r\n    return iterator(std::static_pointer_cast<Leaf>(u));\r\
    \n  }\r\n public:\r\n  inline void clear() {\r\n    _init();\r\n  }\r\n protected:\r\
    \n  virtual inline node_ptr create_node_at(const key_type& x [[maybe_unused]],\
    \ int i [[maybe_unused]]) {\r\n    return std::make_shared<Node>();\r\n  }\r\n\
    \  virtual inline leaf_ptr create_leaf_at(const key_type& x [[maybe_unused]],\
    \ const value_type& value) {\r\n    return std::make_shared<Leaf>(value);\r\n\
    \  }\r\n  virtual inline leaf_ptr create_leaf_at(const key_type& x [[maybe_unused]],\
    \ value_type&& value) {\r\n    return std::make_shared<Leaf>(std::move(value));\r\
    \n  }\r\n  template<typename Value>\r\n  inline std::pair<iterator, bool> _insert(Value&&\
    \ value) {\r\n    static_assert(std::is_convertible<Value, value_type>::value,\
    \ \"\");\r\n    int i, c;\r\n    key_type x = Def::key_of(value);\r\n    auto\
    \ u = root_;\r\n    for (i = 0; i < W; i++) {\r\n      c = (x >> (W-i-1)) & 1;\r\
    \n      if (!u->c[c]) break;\r\n      u = u->c[c];\r\n    }\r\n    if (i == W)\r\
    \n      return std::make_pair(iterator(std::static_pointer_cast<Leaf>(u)), false);\r\
    \n    auto f = u;\r\n    auto fc = c;\r\n    auto fi = i;\r\n    auto pred = c\
    \ == 1 ? u->jump : u->jump->prev();\r\n    u->jump = nullptr;\r\n    auto l =\
    \ create_leaf_at(x, std::forward<Value>(value));\r\n    l->set_prev(pred);\r\n\
    \    l->set_next(pred->next());\r\n    l->prev()->set_next(l);\r\n    l->next()->set_prev(l);\r\
    \n    for (; i < W-1; i++) {\r\n      c = (x >> (W-i-1)) & 1;\r\n      u->c[c]\
    \ = create_node_at(x, i+1);\r\n      u->c[c]->parent = u;\r\n      u->c[c]->jump\
    \ = l;\r\n      u = u->c[c];\r\n    }\r\n    {\r\n      c = (x >> (W-i-1)) & 1;\r\
    \n      u->c[c] = l;\r\n      u->c[c]->parent = u;\r\n    }\r\n    if (f == root_)\
    \ [[unlikely]] {\r\n      f->jump = l;\r\n    } else [[likely]] {\r\n      auto\
    \ v = f->parent.lock();\r\n      fi--;\r\n      while (v) {\r\n        c = x >>\
    \ (W-fi-1) & 1;\r\n        if (c != fc and !v->jump)\r\n          break;\r\n \
    \       if (!v->c[fc])\r\n          v->jump = l;\r\n        v = v->parent.lock();\r\
    \n        fi--;\r\n      }\r\n    }\r\n    size_++;\r\n    return std::make_pair(iterator(l),\
    \ true);\r\n  }\r\n\r\n  virtual void erase_node_at(const key_type& x [[maybe_unused]],\
    \ int i [[maybe_unused]], node_ptr u [[maybe_unused]]) {}\r\n  template<typename\
    \ Key>\r\n  bool _erase(Key&& x) {\r\n    static_assert(std::is_convertible<Key,\
    \ key_type>::value, \"\");\r\n    auto it = _find(x);\r\n    if (it != end())\
    \ {\r\n      _erase(it);\r\n      return true;\r\n    } else {\r\n      return\
    \ false;\r\n    }\r\n  }\r\n  template<typename Key>\r\n  inline iterator _erase(Key&&\
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
    \   if (it == end()) return it;\r\n    return _erase(Def::key_of(*it), it.ptr_);\r\
    \n  }\r\n};\r\ntemplate<typename T, typename M, int8_t W>\r\nstruct BinaryTrieBase<T,M,W>::iterator\
    \ {\r\n  using difference_type = ptrdiff_t;\r\n  using value_type = BinaryTrieBase::value_type;\r\
    \n  using pointer = value_type*;\r\n  using reference = value_type&;\r\n  using\
    \ iterator_category = std::bidirectional_iterator_tag;\r\n  leaf_ptr ptr_;\r\n\
    \  iterator(leaf_ptr p) : ptr_(p) {}\r\n  inline reference operator*() {\r\n \
    \   return ptr_->v;\r\n  }\r\n  inline pointer operator->() {\r\n    return &(ptr_->v);\r\
    \n  }\r\n  inline bool operator==(const iterator& rhs) const {\r\n    return ptr_\
    \ == rhs.ptr_;\r\n  }\r\n  inline bool operator!=(const iterator& rhs) const {\r\
    \n    return !operator==(rhs);\r\n  }\r\n  inline iterator& operator++() {\r\n\
    \    ptr_ = ptr_->next();\r\n    return *this;\r\n  }\r\n  inline iterator operator++(int)\
    \ const {\r\n    iterator ret = *this;\r\n    operator++();\r\n    return ret;\r\
    \n  }\r\n  inline iterator& operator--() {\r\n    ptr_ = ptr_->prev();\r\n   \
    \ return *this;\r\n  }\r\n  inline iterator operator--(int) const {\r\n    iterator\
    \ ret = *this;\r\n    operator--();\r\n    return ret;\r\n  }\r\n};\r\n\r\n#if\
    \ __cplusplus >= 202002L\r\n#include <concepts>\r\ntemplate<typename M>\r\nconcept\
    \ IsAssociativeArray = requires (M m) {\r\n  typename M::key_type;\r\n  typename\
    \ M::value_type;\r\n  typename M::iterator;\r\n  {m.size()} -> std::convertible_to<size_t>;\r\
    \n  {m.empty()} -> std::same_as<bool>;\r\n  {m.clear()};\r\n  {m.begin()} -> std::same_as<typename\
    \ M::iterator>;\r\n  {m.end()} -> std::same_as<typename M::iterator>;\r\n};\r\n\
    #endif\r\n\r\ntemplate<class Base>\r\n#if __cplusplus >= 202002L\r\nrequires IsAssociativeArray<Base>\r\
    \n#endif\r\nclass SetInterfaceBase : public Base {\r\n public:\r\n  using key_type\
    \ = typename Base::key_type;\r\n  using value_type = typename Base::value_type;\r\
    \n  using iterator = typename Base::iterator;\r\n  SetInterfaceBase() = default;\r\
    \n  template<typename InputIt>\r\n  explicit SetInterfaceBase(InputIt begin, InputIt\
    \ end) : Base(begin, end) {}\r\n  SetInterfaceBase(std::initializer_list<value_type>\
    \ init) : Base(init) {}\r\n  using Base::begin;\r\n  using Base::end;\r\n  using\
    \ reverse_iterator = std::reverse_iterator<iterator>;\r\n  inline reverse_iterator\
    \ rbegin() const {\r\n    return reverse_iterator(begin());\r\n  }\r\n  inline\
    \ reverse_iterator rend() const {\r\n    return reverse_iterator(end());\r\n \
    \ }\r\n  template<class Key>\r\n  inline iterator lower_bound(Key&& x) const {\r\
    \n    return Base::_lower_bound(std::forward<Key>(x));\r\n  }\r\n  inline iterator\
    \ lower_bound(const key_type& x) const {\r\n    return Base::_lower_bound(x);\r\
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
    \ x) const {\r\n    return find(std::move(x)) != end();\r\n  }\r\n  template<class\
    \ Value>\r\n  inline std::pair<iterator, bool> insert(Value&& v) {\r\n    return\
    \ Base::_insert(std::forward<Value>(v));\r\n  }\r\n  inline std::pair<iterator,\
    \ bool> insert(const value_type& v) {\r\n    return Base::_insert(v);\r\n  }\r\
    \n  inline std::pair<iterator, bool> insert(value_type&& v) {\r\n    return Base::_insert(std::move(v));\r\
    \n  }\r\n  // TODO\r\n//  template<class... Args>\r\n//  inline std::pair<iterator,\
    \ bool> emplace(Args&&... args) {\r\n//    return Base::_emplace(std::forward<Args>(args)...);\r\
    \n//  }\r\n  template<class Key>\r\n  inline bool erase(Key&& x) {\r\n    return\
    \ Base::_erase(std::forward<Key>(x));\r\n  }\r\n  inline bool erase(const key_type&\
    \ x) {\r\n    return Base::_erase(x);\r\n  }\r\n  inline bool erase(key_type&&\
    \ x) {\r\n    return Base::_erase(std::move(x));\r\n  }\r\n  inline iterator erase(iterator\
    \ it) {\r\n    return Base::_erase(it);\r\n  }\r\n};\r\n\r\ntemplate<typename\
    \ Base>\r\nusing SetInterface = SetInterfaceBase<Base>;\r\n\r\ntemplate<typename\
    \ Base>\r\nclass MapInterface : public SetInterfaceBase<Base> {\r\n  using SBase\
    \ = SetInterfaceBase<Base>;\r\n public:\r\n  using typename SBase::key_type;\r\
    \n  using typename SBase::mapped_type;\r\n  using typename SBase::value_type;\r\
    \n  using reference = mapped_type&;\r\n  MapInterface() = default;\r\n  template<typename\
    \ InputIt>\r\n  explicit MapInterface(InputIt begin, InputIt end) : SBase(begin,\
    \ end) {}\r\n  MapInterface(std::initializer_list<value_type> init) : SBase(init)\
    \ {}\r\n  inline reference operator[](const key_type& x) {\r\n    // TODO\r\n\
    //    return SBase::try_emplace(x).first->second;\r\n    auto it = SBase::insert({x,\
    \ mapped_type()}).first;\r\n    return it->second;\r\n  }\r\n  inline reference\
    \ operator[](key_type&& x) {\r\n    // TODO\r\n//    return SBase::try_emplace(std::move(x)).first->second;\r\
    \n    auto it = SBase::insert({std::move(x), mapped_type()}).first;\r\n    return\
    \ it->second;\r\n  }\r\n};\r\n\r\ntemplate<typename T, typename V, uint8_t W =\
    \ sizeof(T)*8>\r\nusing BinaryTrie = MapInterface<BinaryTrieBase<T, V, W>>;\r\n\
    template<typename T, uint8_t W = sizeof(T)*8>\r\nusing BinaryTrieSet = SetInterface<BinaryTrieBase<T,\
    \ void, W>>;\r\ntemplate<typename T, typename V, uint8_t W = sizeof(T)*8>\r\n\
    using BinaryTrieMap = BinaryTrie<T, V, W>;\r\n"
  code: "#pragma once\r\n#include <array>\r\n#include <memory>\r\n#include <type_traits>\r\
    \n#include <cstdint>\r\n#include <initializer_list>\r\n#include <cassert>\r\n\
    #include <bitset>\r\n#include <iostream>\r\nusing std::cerr;\r\nusing std::endl;\r\
    \n\r\ntemplate<typename T, typename M>\r\nstruct AssociativeArrayDefinition {\r\
    \n  static constexpr bool kPairValue = true;\r\n  using key_type = T;\r\n  using\
    \ mapped_type = M;\r\n  using value_type = std::pair<T const, M>;\r\n  using raw_key_type\
    \ = typename std::remove_reference<T>::type;\r\n  using raw_mapped_type = typename\
    \ std::remove_reference<M>::type;\r\n  using init_type = std::pair<raw_key_type,\
    \ raw_mapped_type>;\r\n  using moved_type = std::pair<raw_key_type&&, raw_mapped_type&&>;\r\
    \n  template<class K, class V>\r\n  static inline key_type const& key_of(std::pair<K,V>\
    \ const& kv) {\r\n    return kv.first;\r\n  }\r\n};\r\ntemplate<typename T>\r\n\
    struct AssociativeArrayDefinition<T, void> {\r\n  static constexpr bool kPairValue\
    \ = false;\r\n  using key_type = T;\r\n  using value_type = T;\r\n  using init_type\
    \ = T;\r\n  static inline key_type const& key_of(value_type const& k) { return\
    \ k; }\r\n};\r\n\r\ntemplate<typename T, typename M>\r\nstruct BinaryTrieDefinition\
    \ : AssociativeArrayDefinition<T, M> {\r\n  static_assert(std::is_unsigned<T>::value,\
    \ \"\");\r\n  using Base = AssociativeArrayDefinition<T, M>;\r\n  struct Node;\r\
    \n  using node_ptr = std::shared_ptr<Node>;\r\n  using node_weak_ptr = std::weak_ptr<Node>;\r\
    \n  struct Leaf;\r\n  using leaf_ptr = std::shared_ptr<Leaf>;\r\n  struct Node\
    \ {\r\n    std::array<node_ptr, 2> c;\r\n    leaf_ptr jump;\r\n    node_weak_ptr\
    \ parent;\r\n    Node() = default;\r\n    inline node_ptr& left() { return c[0];\
    \ }\r\n    inline node_ptr& right()  { return c[1]; }\r\n  };\r\n  using typename\
    \ Base::key_type;\r\n  using typename Base::value_type;\r\n  struct Leaf : Node\
    \ {\r\n    value_type v;\r\n    Leaf() = default;\r\n    Leaf(const value_type&\
    \ v) : Node(), v(v) {}\r\n    Leaf(value_type&& v) : Node(), v(std::forward<value_type>(v))\
    \ {}\r\n    inline key_type key() const {\r\n      return Base::key_of(v);\r\n\
    \    }\r\n    using Node::c;\r\n    inline leaf_ptr prev() const {\r\n      return\
    \ std::static_pointer_cast<Leaf>(c[0]);\r\n    }\r\n    inline leaf_ptr next()\
    \ const {\r\n      return std::static_pointer_cast<Leaf>(c[1]);\r\n    }\r\n \
    \   inline void set_prev(leaf_ptr l) {\r\n      c[0] = std::static_pointer_cast<Node>(l);\r\
    \n    }\r\n    inline void set_next(leaf_ptr l) {\r\n      c[1] = std::static_pointer_cast<Node>(l);\r\
    \n    }\r\n  };\r\n};\r\n\r\ntemplate<typename T, typename M,\r\n    int8_t W\
    \ = sizeof(T) * 8>\r\nclass BinaryTrieBase : public BinaryTrieDefinition<T, M>\
    \ {\r\n  static_assert(std::is_unsigned<T>::value, \"\");\r\n  using Def = BinaryTrieDefinition<T,\
    \ M>;\r\n public:\r\n  using key_type = typename Def::key_type;\r\n  static constexpr\
    \ bool kPairValue = Def::kPairValue;\r\n  using value_type = typename Def::value_type;\r\
    \n  using Node = typename Def::Node;\r\n  using node_ptr = typename Def::node_ptr;\r\
    \n  using node_weak_ptr = typename Def::node_weak_ptr;\r\n  using Leaf = typename\
    \ Def::Leaf;\r\n  using leaf_ptr = typename Def::leaf_ptr;\r\n protected:\r\n\
    \  node_ptr root_;\r\n  leaf_ptr dummy_;\r\n  size_t size_;\r\n  virtual void\
    \ _init() {\r\n    root_ = std::make_shared<Node>();\r\n    dummy_ = std::make_shared<Leaf>();\r\
    \n    root_->jump = dummy_;\r\n    dummy_->set_next(dummy_);\r\n    dummy_->set_prev(dummy_);\r\
    \n    size_ = 0;\r\n  }\r\n public:\r\n  BinaryTrieBase() {\r\n    _init();\r\n\
    \  }\r\n  ~BinaryTrieBase() {\r\n    root_ = nullptr;\r\n    dummy_->set_next(nullptr);\r\
    \n    dummy_->set_prev(nullptr);\r\n    dummy_ = nullptr;\r\n  }\r\n  template<typename\
    \ InputIt>\r\n  explicit BinaryTrieBase(InputIt begin, InputIt end) : BinaryTrieBase()\
    \ {\r\n    static_assert(std::is_convertible<typename std::iterator_traits<InputIt>::value_type,\
    \ value_type>::value, \"\");\r\n    // TODO: optimize\r\n    for (auto it = begin;\
    \ it != end; ++it)\r\n      _insert(*it);\r\n  }\r\n  BinaryTrieBase(std::initializer_list<value_type>\
    \ init) : BinaryTrieBase(init.begin(), init.end()) {}\r\n  inline size_t size()\
    \ const {\r\n    return size_;\r\n  }\r\n  inline bool empty() const { return\
    \ size() == 0; }\r\n  struct iterator;\r\n  inline iterator begin() const {\r\n\
    \    return iterator(dummy_->next());\r\n  }\r\n  inline iterator end() const\
    \ {\r\n    return iterator(dummy_);\r\n  }\r\n protected:\r\n  template<typename\
    \ Key>\r\n  inline iterator _lower_bound(Key&& key) const {\r\n    static_assert(std::is_convertible<Key,\
    \ key_type>::value, \"\");\r\n    key_type x = std::forward<Key>(key);\r\n   \
    \ int i, c;\r\n    auto u = root_;\r\n    for (i = 0; i < W; i++) {\r\n      c\
    \ = (x >> (W-i-1)) & 1;\r\n      if (!u->c[c]) break;\r\n      u = u->c[c];\r\n\
    \    }\r\n    if (i == W) return iterator(std::static_pointer_cast<Leaf>(u));\r\
    \n    auto l = (c == 0) ? u->jump : u->jump->next();\r\n    return iterator(l);\r\
    \n  }\r\n protected:\r\n  template<typename Key>\r\n  inline iterator _upper_bound(Key&&\
    \ key) const {\r\n    static_assert(std::is_convertible<Key, key_type>::value,\
    \ \"\");\r\n    key_type x = std::forward<Key>(key);\r\n    auto it = _lower_bound(x);\r\
    \n    if (Def::key_of(*it) == x)\r\n      ++it;\r\n    return it;\r\n  }\r\n \
    \ template<typename Key>\r\n  inline iterator _find(Key&& key) const {\r\n   \
    \ static_assert(std::is_convertible<Key, key_type>::value, \"\");\r\n    key_type\
    \ x = std::forward<Key>(key);\r\n    int i, c;\r\n    auto u = root_;\r\n    for\
    \ (i = 0; i < W; i++) {\r\n      c = (x >> (W-i-1)) & 1;\r\n      if (!u->c[c])\
    \ return end();\r\n      u = u->c[c];\r\n    }\r\n    return iterator(std::static_pointer_cast<Leaf>(u));\r\
    \n  }\r\n public:\r\n  inline void clear() {\r\n    _init();\r\n  }\r\n protected:\r\
    \n  virtual inline node_ptr create_node_at(const key_type& x [[maybe_unused]],\
    \ int i [[maybe_unused]]) {\r\n    return std::make_shared<Node>();\r\n  }\r\n\
    \  virtual inline leaf_ptr create_leaf_at(const key_type& x [[maybe_unused]],\
    \ const value_type& value) {\r\n    return std::make_shared<Leaf>(value);\r\n\
    \  }\r\n  virtual inline leaf_ptr create_leaf_at(const key_type& x [[maybe_unused]],\
    \ value_type&& value) {\r\n    return std::make_shared<Leaf>(std::move(value));\r\
    \n  }\r\n  template<typename Value>\r\n  inline std::pair<iterator, bool> _insert(Value&&\
    \ value) {\r\n    static_assert(std::is_convertible<Value, value_type>::value,\
    \ \"\");\r\n    int i, c;\r\n    key_type x = Def::key_of(value);\r\n    auto\
    \ u = root_;\r\n    for (i = 0; i < W; i++) {\r\n      c = (x >> (W-i-1)) & 1;\r\
    \n      if (!u->c[c]) break;\r\n      u = u->c[c];\r\n    }\r\n    if (i == W)\r\
    \n      return std::make_pair(iterator(std::static_pointer_cast<Leaf>(u)), false);\r\
    \n    auto f = u;\r\n    auto fc = c;\r\n    auto fi = i;\r\n    auto pred = c\
    \ == 1 ? u->jump : u->jump->prev();\r\n    u->jump = nullptr;\r\n    auto l =\
    \ create_leaf_at(x, std::forward<Value>(value));\r\n    l->set_prev(pred);\r\n\
    \    l->set_next(pred->next());\r\n    l->prev()->set_next(l);\r\n    l->next()->set_prev(l);\r\
    \n    for (; i < W-1; i++) {\r\n      c = (x >> (W-i-1)) & 1;\r\n      u->c[c]\
    \ = create_node_at(x, i+1);\r\n      u->c[c]->parent = u;\r\n      u->c[c]->jump\
    \ = l;\r\n      u = u->c[c];\r\n    }\r\n    {\r\n      c = (x >> (W-i-1)) & 1;\r\
    \n      u->c[c] = l;\r\n      u->c[c]->parent = u;\r\n    }\r\n    if (f == root_)\
    \ [[unlikely]] {\r\n      f->jump = l;\r\n    } else [[likely]] {\r\n      auto\
    \ v = f->parent.lock();\r\n      fi--;\r\n      while (v) {\r\n        c = x >>\
    \ (W-fi-1) & 1;\r\n        if (c != fc and !v->jump)\r\n          break;\r\n \
    \       if (!v->c[fc])\r\n          v->jump = l;\r\n        v = v->parent.lock();\r\
    \n        fi--;\r\n      }\r\n    }\r\n    size_++;\r\n    return std::make_pair(iterator(l),\
    \ true);\r\n  }\r\n\r\n  virtual void erase_node_at(const key_type& x [[maybe_unused]],\
    \ int i [[maybe_unused]], node_ptr u [[maybe_unused]]) {}\r\n  template<typename\
    \ Key>\r\n  bool _erase(Key&& x) {\r\n    static_assert(std::is_convertible<Key,\
    \ key_type>::value, \"\");\r\n    auto it = _find(x);\r\n    if (it != end())\
    \ {\r\n      _erase(it);\r\n      return true;\r\n    } else {\r\n      return\
    \ false;\r\n    }\r\n  }\r\n  template<typename Key>\r\n  inline iterator _erase(Key&&\
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
    \   if (it == end()) return it;\r\n    return _erase(Def::key_of(*it), it.ptr_);\r\
    \n  }\r\n};\r\ntemplate<typename T, typename M, int8_t W>\r\nstruct BinaryTrieBase<T,M,W>::iterator\
    \ {\r\n  using difference_type = ptrdiff_t;\r\n  using value_type = BinaryTrieBase::value_type;\r\
    \n  using pointer = value_type*;\r\n  using reference = value_type&;\r\n  using\
    \ iterator_category = std::bidirectional_iterator_tag;\r\n  leaf_ptr ptr_;\r\n\
    \  iterator(leaf_ptr p) : ptr_(p) {}\r\n  inline reference operator*() {\r\n \
    \   return ptr_->v;\r\n  }\r\n  inline pointer operator->() {\r\n    return &(ptr_->v);\r\
    \n  }\r\n  inline bool operator==(const iterator& rhs) const {\r\n    return ptr_\
    \ == rhs.ptr_;\r\n  }\r\n  inline bool operator!=(const iterator& rhs) const {\r\
    \n    return !operator==(rhs);\r\n  }\r\n  inline iterator& operator++() {\r\n\
    \    ptr_ = ptr_->next();\r\n    return *this;\r\n  }\r\n  inline iterator operator++(int)\
    \ const {\r\n    iterator ret = *this;\r\n    operator++();\r\n    return ret;\r\
    \n  }\r\n  inline iterator& operator--() {\r\n    ptr_ = ptr_->prev();\r\n   \
    \ return *this;\r\n  }\r\n  inline iterator operator--(int) const {\r\n    iterator\
    \ ret = *this;\r\n    operator--();\r\n    return ret;\r\n  }\r\n};\r\n\r\n#if\
    \ __cplusplus >= 202002L\r\n#include <concepts>\r\ntemplate<typename M>\r\nconcept\
    \ IsAssociativeArray = requires (M m) {\r\n  typename M::key_type;\r\n  typename\
    \ M::value_type;\r\n  typename M::iterator;\r\n  {m.size()} -> std::convertible_to<size_t>;\r\
    \n  {m.empty()} -> std::same_as<bool>;\r\n  {m.clear()};\r\n  {m.begin()} -> std::same_as<typename\
    \ M::iterator>;\r\n  {m.end()} -> std::same_as<typename M::iterator>;\r\n};\r\n\
    #endif\r\n\r\ntemplate<class Base>\r\n#if __cplusplus >= 202002L\r\nrequires IsAssociativeArray<Base>\r\
    \n#endif\r\nclass SetInterfaceBase : public Base {\r\n public:\r\n  using key_type\
    \ = typename Base::key_type;\r\n  using value_type = typename Base::value_type;\r\
    \n  using iterator = typename Base::iterator;\r\n  SetInterfaceBase() = default;\r\
    \n  template<typename InputIt>\r\n  explicit SetInterfaceBase(InputIt begin, InputIt\
    \ end) : Base(begin, end) {}\r\n  SetInterfaceBase(std::initializer_list<value_type>\
    \ init) : Base(init) {}\r\n  using Base::begin;\r\n  using Base::end;\r\n  using\
    \ reverse_iterator = std::reverse_iterator<iterator>;\r\n  inline reverse_iterator\
    \ rbegin() const {\r\n    return reverse_iterator(begin());\r\n  }\r\n  inline\
    \ reverse_iterator rend() const {\r\n    return reverse_iterator(end());\r\n \
    \ }\r\n  template<class Key>\r\n  inline iterator lower_bound(Key&& x) const {\r\
    \n    return Base::_lower_bound(std::forward<Key>(x));\r\n  }\r\n  inline iterator\
    \ lower_bound(const key_type& x) const {\r\n    return Base::_lower_bound(x);\r\
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
    \ x) const {\r\n    return find(std::move(x)) != end();\r\n  }\r\n  template<class\
    \ Value>\r\n  inline std::pair<iterator, bool> insert(Value&& v) {\r\n    return\
    \ Base::_insert(std::forward<Value>(v));\r\n  }\r\n  inline std::pair<iterator,\
    \ bool> insert(const value_type& v) {\r\n    return Base::_insert(v);\r\n  }\r\
    \n  inline std::pair<iterator, bool> insert(value_type&& v) {\r\n    return Base::_insert(std::move(v));\r\
    \n  }\r\n  // TODO\r\n//  template<class... Args>\r\n//  inline std::pair<iterator,\
    \ bool> emplace(Args&&... args) {\r\n//    return Base::_emplace(std::forward<Args>(args)...);\r\
    \n//  }\r\n  template<class Key>\r\n  inline bool erase(Key&& x) {\r\n    return\
    \ Base::_erase(std::forward<Key>(x));\r\n  }\r\n  inline bool erase(const key_type&\
    \ x) {\r\n    return Base::_erase(x);\r\n  }\r\n  inline bool erase(key_type&&\
    \ x) {\r\n    return Base::_erase(std::move(x));\r\n  }\r\n  inline iterator erase(iterator\
    \ it) {\r\n    return Base::_erase(it);\r\n  }\r\n};\r\n\r\ntemplate<typename\
    \ Base>\r\nusing SetInterface = SetInterfaceBase<Base>;\r\n\r\ntemplate<typename\
    \ Base>\r\nclass MapInterface : public SetInterfaceBase<Base> {\r\n  using SBase\
    \ = SetInterfaceBase<Base>;\r\n public:\r\n  using typename SBase::key_type;\r\
    \n  using typename SBase::mapped_type;\r\n  using typename SBase::value_type;\r\
    \n  using reference = mapped_type&;\r\n  MapInterface() = default;\r\n  template<typename\
    \ InputIt>\r\n  explicit MapInterface(InputIt begin, InputIt end) : SBase(begin,\
    \ end) {}\r\n  MapInterface(std::initializer_list<value_type> init) : SBase(init)\
    \ {}\r\n  inline reference operator[](const key_type& x) {\r\n    // TODO\r\n\
    //    return SBase::try_emplace(x).first->second;\r\n    auto it = SBase::insert({x,\
    \ mapped_type()}).first;\r\n    return it->second;\r\n  }\r\n  inline reference\
    \ operator[](key_type&& x) {\r\n    // TODO\r\n//    return SBase::try_emplace(std::move(x)).first->second;\r\
    \n    auto it = SBase::insert({std::move(x), mapped_type()}).first;\r\n    return\
    \ it->second;\r\n  }\r\n};\r\n\r\ntemplate<typename T, typename V, uint8_t W =\
    \ sizeof(T)*8>\r\nusing BinaryTrie = MapInterface<BinaryTrieBase<T, V, W>>;\r\n\
    template<typename T, uint8_t W = sizeof(T)*8>\r\nusing BinaryTrieSet = SetInterface<BinaryTrieBase<T,\
    \ void, W>>;\r\ntemplate<typename T, typename V, uint8_t W = sizeof(T)*8>\r\n\
    using BinaryTrieMap = BinaryTrie<T, V, W>;\r\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/binary_trie.hpp
  requiredBy:
  - include/mtl/xft.hpp
  - include/mtl/yft.hpp
  timestamp: '2022-12-21 13:24:57+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/binary_trie.hpp
layout: document
redirect_from:
- /library/include/mtl/binary_trie.hpp
- /library/include/mtl/binary_trie.hpp.html
title: include/mtl/binary_trie.hpp
---
