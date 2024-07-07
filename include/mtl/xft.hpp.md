---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/binary_trie.hpp
    title: include/mtl/binary_trie.hpp
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':question:'
    path: include/mtl/traits/set_traits.hpp
    title: include/mtl/traits/set_traits.hpp
  _extendedRequiredBy:
  - icon: ':question:'
    path: include/mtl/yft.hpp
    title: include/mtl/yft.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/yosupo/associative_array-yft.test.cpp
    title: test/yosupo/associative_array-yft.test.cpp
  - icon: ':x:'
    path: test/yosupo/predecessor_problem-yft.test.cpp
    title: test/yosupo/predecessor_problem-yft.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':question:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/traits/set_traits.hpp\"\n#include <cstddef>\r\
    \n#include <initializer_list>\r\n#include <type_traits>\r\n#include <iterator>\r\
    \n\r\nnamespace traits {\r\n\r\ntemplate<typename T, typename M>\r\nstruct AssociativeArrayDefinition\
    \ {\r\n  using key_type = T;\r\n  using mapped_type = M;\r\n  using value_type\
    \ = std::pair<T const, M>;\r\n  using raw_key_type = typename std::remove_const<T>::type;\r\
    \n  using raw_mapped_type = typename std::remove_const<M>::type;\r\n  using init_type\
    \ = std::pair<raw_key_type, raw_mapped_type>;\r\n  using moved_type = std::pair<raw_key_type&&,\
    \ raw_mapped_type&&>;\r\n  template<class K, class V>\r\n  static key_type const&\
    \ key_of(std::pair<K,V> const& kv) {\r\n    return kv.first;\r\n  }\r\n};\r\n\
    template<typename T>\r\nstruct AssociativeArrayDefinition<T, void> {\r\n  using\
    \ key_type = T;\r\n  using value_type = T;\r\n  using init_type = T;\r\n  static\
    \ key_type const& key_of(value_type const& k) { return k; }\r\n};\r\n\r\ntemplate<class\
    \ T, typename = std::void_t<>>\r\nstruct get_const_iterator {\r\n  using base\
    \ = typename T::iterator;\r\n  struct type : base {\r\n    type(const base& r)\
    \ : base(r) {}\r\n    type(base&& r) : base(std::move(r)) {}\r\n  };\r\n};\r\n\
    template<class T>\r\nstruct get_const_iterator<T, std::void_t<typename T::const_iterator>>\
    \ {\r\n  using type = typename T::const_iterator;\r\n};\r\n\r\n#if __cplusplus\
    \ >= 202002L\r\n#include <concepts>\r\ntemplate<class M>\r\nconcept IsAssociativeArray\
    \ = requires (M m) {\r\n  typename M::key_type;\r\n  typename M::value_type;\r\
    \n  typename M::iterator;\r\n  {m.size()} -> std::convertible_to<size_t>;\r\n\
    \  {m.empty()} -> std::same_as<bool>;\r\n  {m.clear()};\r\n  {m.begin()} -> std::same_as<typename\
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
    \ const { return size() == 0; }\r\n  using Base::clear;\r\n  using const_iterator\
    \ = typename get_const_iterator<Base>::type;\r\n  iterator begin() {\r\n    return\
    \ Base::begin();\r\n  }\r\n  iterator end() {\r\n    return Base::end();\r\n \
    \ }\r\n  const_iterator begin() const {\r\n    return const_iterator(Base::begin());\r\
    \n  }\r\n  const_iterator end() const {\r\n    return const_iterator(Base::end());\r\
    \n  }\r\n  const_iterator cbegin() const {\r\n    return begin();\r\n  }\r\n \
    \ const_iterator cend() const {\r\n    return end();\r\n  }\r\n  using reverse_iterator\
    \ = std::reverse_iterator<iterator>;\r\n  using reverse_const_iterator = std::reverse_iterator<const_iterator>;\r\
    \n  reverse_iterator rbegin() {\r\n    return std::make_reverse_iterator(end());\r\
    \n  }\r\n  reverse_iterator rend() {\r\n    return std::make_reverse_iterator(begin());\r\
    \n  }\r\n  reverse_const_iterator rbegin() const {\r\n    return std::make_reverse_iterator(end());\r\
    \n  }\r\n  reverse_const_iterator rend() const {\r\n    return std::make_reverse_iterator(begin());\r\
    \n  }\r\n  reverse_const_iterator crbegin() const {\r\n    return rbegin();\r\n\
    \  }\r\n  reverse_const_iterator crend() const {\r\n    return rend();\r\n  }\r\
    \n  template<class Key>\r\n  iterator lower_bound(const Key& x) const {\r\n  \
    \  return Base::_lower_bound(x);\r\n  }\r\n  iterator lower_bound(const key_type&\
    \ x) const {\r\n    return Base::_lower_bound(x);\r\n  }\r\n  template<class Key>\r\
    \n  iterator upper_bound(const Key& x) const {\r\n    return Base::_upper_bound(x);\r\
    \n  }\r\n  iterator upper_bound(const key_type& x) const {\r\n    return Base::_upper_bound(x);\r\
    \n  }\r\n  template<class Key>\r\n  iterator find(const Key& x) {\r\n    return\
    \ Base::_find(x);\r\n  }\r\n  iterator find(const key_type& x) {\r\n    return\
    \ Base::_find(x);\r\n  }\r\n  template<class Key>\r\n  const_iterator find(const\
    \ Key& x) const {\r\n    return Base::_find(x);\r\n  }\r\n  const_iterator find(const\
    \ key_type& x) const {\r\n    return Base::_find(x);\r\n  }\r\n  template<class\
    \ Key>\r\n  size_t count(const Key& x) const {\r\n    return find(x) != end();\r\
    \n  }\r\n  size_t count(const key_type& x) const {\r\n    return find(x) != end();\r\
    \n  }\r\n  std::pair<iterator, bool> insert(const init_type& v) {\r\n    return\
    \ Base::_insert(v);\r\n  }\r\n  std::pair<iterator, bool> insert(init_type&& v)\
    \ {\r\n    return Base::_insert(std::move(v));\r\n  }\r\n  template<typename=void>\r\
    \n  std::pair<iterator, bool> insert(const value_type& v) {\r\n    return Base::_insert(v);\r\
    \n  }\r\n  template<typename=void>\r\n  std::pair<iterator, bool> insert(value_type&&\
    \ v) {\r\n    return Base::_insert(std::move(v));\r\n  }\r\n  template<class...\
    \ Args>\r\n  std::pair<iterator, bool> emplace(Args&&... args) {\r\n    using\
    \ emplace_type = typename std::conditional<\r\n        std::is_constructible<init_type,\
    \ Args...>::value,\r\n        init_type,\r\n        value_type\r\n    >::type;\r\
    \n    return Base::_insert(emplace_type(std::forward<Args>(args)...));\r\n  }\r\
    \n  template<class... Args>\r\n  iterator emplace_hint(const_iterator hint, Args&&...\
    \ args) {\r\n    using emplace_type = typename std::conditional<\r\n        std::is_constructible<init_type,\
    \ Args...>::value,\r\n        init_type,\r\n        value_type\r\n    >::type;\r\
    \n    return Base::_emplace_hint(hint, emplace_type(std::forward<Args>(args)...));\r\
    \n  }\r\n  size_t erase(const key_type& x) {\r\n    return Base::_erase(x);\r\n\
    \  }\r\n  iterator erase(iterator it) {\r\n    return Base::_erase(it);\r\n  }\r\
    \n  iterator erase(const_iterator it) {\r\n    return Base::_erase(it);\r\n  }\r\
    \n};\r\n\r\ntemplate<typename Base>\r\nusing SetTraits = SetTraitsBase<Base>;\r\
    \n\r\ntemplate<typename Base>\r\nclass MapTraits : public SetTraitsBase<Base>\
    \ {\r\n  using SBase = SetTraitsBase<Base>;\r\n public:\r\n  using typename SBase::key_type;\r\
    \n  using typename SBase::mapped_type;\r\n  using typename SBase::value_type;\r\
    \n  using reference = mapped_type&;\r\n  MapTraits() = default;\r\n  template<typename\
    \ InputIt>\r\n  explicit MapTraits(InputIt begin, InputIt end) : SBase(begin,\
    \ end) {}\r\n  MapTraits(std::initializer_list<value_type> init) : SBase(init)\
    \ {}\r\n  template<typename Key>\r\n  reference operator[](Key&& x) {\r\n    auto\
    \ i = SBase::lower_bound(x);\r\n    if (i == SBase::end() || x < i->first) {\r\
    \n      i = SBase::emplace_hint(i, std::forward<Key>(x), mapped_type());\r\n \
    \   }\r\n    return i->second;\r\n  }\r\n  reference operator[](const key_type&\
    \ x) {\r\n    auto i = SBase::lower_bound(x);\r\n    if (i == SBase::end() ||\
    \ x < i->first) {\r\n      i = SBase::emplace_hint(i, x, mapped_type());\r\n \
    \   }\r\n    return i->second;\r\n  }\r\n  reference operator[](key_type&& x)\
    \ {\r\n    auto i = SBase::lower_bound(x);\r\n    if (i == SBase::end() || x <\
    \ i->first) {\r\n      i = SBase::emplace_hint(i, std::move(x), mapped_type());\r\
    \n    }\r\n    return i->second;\r\n  }\r\n};\r\n\r\n} // namespace traits\n#line\
    \ 2 \"include/mtl/bit_manip.hpp\"\n#include <cstdint>\n#include <cassert>\n\n\
    namespace bm {\n\ninline constexpr uint64_t popcnt_e8(uint64_t x) {\n  x = (x\
    \ & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);\n  x = (x & 0x3333333333333333)\
    \ + ((x>>2) & 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) &\
    \ 0x0F0F0F0F0F0F0F0F);\n  return x;\n}\n// Count 1s\ninline constexpr unsigned\
    \ popcnt(uint64_t x) {\n  return (popcnt_e8(x) * 0x0101010101010101) >> 56;\n\
    }\n// Count trailing 0s. s.t. *11011000 -> 3\ninline constexpr unsigned ctz(uint64_t\
    \ x) {\n  return popcnt((x & (-x)) - 1);\n}\ninline constexpr unsigned ctz8(uint8_t\
    \ x) {\n  return x == 0 ? 8 : popcnt_e8((x & (-x)) - 1);\n}\n// [00..0](8bit)\
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
    \ 4 \"include/mtl/binary_trie.hpp\"\n#include <array>\r\n#include <memory>\r\n\
    #line 9 \"include/mtl/binary_trie.hpp\"\n#include <algorithm>\r\n#line 11 \"include/mtl/binary_trie.hpp\"\
    \n\r\ntemplate<typename T, typename M,\r\n    int8_t W = sizeof(T) * 8>\r\nclass\
    \ BinaryTrieBase : public traits::AssociativeArrayDefinition<T, M> {\r\n  static_assert(std::is_unsigned<T>::value,\
    \ \"\");\r\n public:\r\n  using types = traits::AssociativeArrayDefinition<T,\
    \ M>;\r\n  using key_type = typename types::key_type;\r\n  using value_type =\
    \ typename types::value_type;\r\n  using init_type = typename types::init_type;\r\
    \n  struct Node;\r\n  using node_ptr = std::shared_ptr<Node>;\r\n  using node_weak_ptr\
    \ = std::weak_ptr<Node>;\r\n  struct Leaf;\r\n  using leaf_ptr = std::shared_ptr<Leaf>;\r\
    \n  struct Node {\r\n    std::array<node_ptr, 2> c;\r\n    leaf_ptr jump;\r\n\
    \    node_weak_ptr parent;\r\n    Node() = default;\r\n    node_ptr& left() {\
    \ return c[0]; }\r\n    node_ptr& right()  { return c[1]; }\r\n  };\r\n  struct\
    \ Leaf : Node {\r\n    value_type v;\r\n    Leaf() = default;\r\n    Leaf(const\
    \ value_type& v) : Node(), v(v) {}\r\n    Leaf(value_type&& v) : Node(), v(std::forward<value_type>(v))\
    \ {}\r\n    key_type key() const {\r\n      return types::key_of(v);\r\n    }\r\
    \n    using Node::c;\r\n    leaf_ptr prev() const {\r\n      return std::static_pointer_cast<Leaf>(c[0]);\r\
    \n    }\r\n    leaf_ptr next() const {\r\n      return std::static_pointer_cast<Leaf>(c[1]);\r\
    \n    }\r\n    void set_prev(leaf_ptr l) {\r\n      c[0] = std::static_pointer_cast<Node>(l);\r\
    \n    }\r\n    void set_next(leaf_ptr l) {\r\n      c[1] = std::static_pointer_cast<Node>(l);\r\
    \n    }\r\n  };\r\n protected:\r\n  node_ptr root_;\r\n  leaf_ptr dummy_;\r\n\
    \  size_t size_;\r\n  virtual void _init() {\r\n    root_ = create_node_at(0,\
    \ 0);\r\n    dummy_ = std::make_shared<Leaf>();\r\n    root_->jump = dummy_;\r\
    \n    dummy_->set_next(dummy_);\r\n    dummy_->set_prev(dummy_);\r\n    size_\
    \ = 0;\r\n  }\r\n  void _deinit() {\r\n    root_ = nullptr;\r\n    auto u = dummy_->next();\r\
    \n    dummy_->set_next(nullptr);\r\n    u->set_prev(nullptr);\r\n    while (u\
    \ != dummy_) {\r\n      auto n = u->next();\r\n      u->set_next(nullptr);\r\n\
    \      n->set_prev(nullptr);\r\n      u = n;\r\n    }\r\n    dummy_ = nullptr;\r\
    \n  }\r\n public:\r\n  BinaryTrieBase() {\r\n    _init();\r\n  }\r\n  BinaryTrieBase(const\
    \ BinaryTrieBase& rhs) {\r\n    _insert_init(rhs.begin(), rhs.end());\r\n  }\r\
    \n  virtual BinaryTrieBase& operator=(const BinaryTrieBase& rhs) {\r\n    _deinit();\r\
    \n    _insert_init(rhs.begin(), rhs.end());\r\n    return *this;\r\n  }\r\n  BinaryTrieBase(BinaryTrieBase&&)\
    \ noexcept = default;\r\n  virtual BinaryTrieBase& operator=(BinaryTrieBase&&\
    \ rhs) noexcept {\r\n    _deinit();\r\n    root_ = std::move(rhs.root_);\r\n \
    \   dummy_ = std::move(rhs.dummy_);\r\n    size_ = std::move(rhs.size_);\r\n \
    \   return *this;\r\n  }\r\n  virtual ~BinaryTrieBase() {\r\n    _deinit();\r\n\
    \  }\r\n protected:\r\n  template<class InputIt>\r\n  void _insert_init(InputIt\
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
    \ end);\r\n  }\r\n  size_t size() const {\r\n    return size_;\r\n  }\r\n  bool\
    \ empty() const { return size() == 0; }\r\n  void clear() {\r\n    _deinit();\r\
    \n    _init();\r\n  }\r\n protected:\r\n  template<bool> struct iterator_base;\r\
    \n public:\r\n  using iterator = iterator_base<false>;\r\n  using const_iterator\
    \ = iterator_base<true>;\r\n  iterator begin() {\r\n    return iterator(dummy_->next());\r\
    \n  }\r\n  iterator end() {\r\n    return iterator(dummy_);\r\n  }\r\n  const_iterator\
    \ begin() const {\r\n    return const_iterator(dummy_->next());\r\n  }\r\n  const_iterator\
    \ end() const {\r\n    return const_iterator(dummy_);\r\n  }\r\n  template<class\
    \ Rule>\r\n  const_iterator traverse(Rule rule) const {\r\n    auto u = root_;\r\
    \n    for (int i = 0; i < W; i++) {\r\n      auto l = (bool)u->c[0];\r\n     \
    \ auto r = (bool)u->c[1];\r\n      auto c = rule(W-1-i, l, r);\r\n      u = u->c[c];\r\
    \n    }\r\n    return const_iterator(std::static_pointer_cast<Leaf>(u));\r\n \
    \ }\r\n protected:\r\n  virtual std::pair<int, node_ptr> _traverse(const key_type&\
    \ key, \r\n                                             int depth = 0, \r\n  \
    \                                           node_ptr root = nullptr) const {\r\
    \n    int i, c;\r\n    key_type x = key;\r\n    auto u = !root ? root_ : root;\r\
    \n    for (i = depth; i < W; i++) {\r\n      c = (x >> (W-i-1)) & 1;\r\n     \
    \ if (!u->c[c]) break;\r\n      u = u->c[c];\r\n    }\r\n    return std::make_pair(i,\
    \ u);\r\n  }\r\n  iterator _lower_bound(const key_type& x) const {\r\n    auto\
    \ reached = _traverse(x);\r\n    int i = reached.first;\r\n    node_ptr u = reached.second;\r\
    \n    if (i == W) return iterator(std::static_pointer_cast<Leaf>(u));\r\n    auto\
    \ l = (((x >> (W-i-1)) & 1) == 0) ? u->jump : u->jump->next();\r\n    return iterator(l);\r\
    \n  }\r\n  iterator _upper_bound(const key_type& x) const {\r\n    auto it = _lower_bound(x);\r\
    \n    if (types::key_of(*it) == x)\r\n      ++it;\r\n    return it;\r\n  }\r\n\
    \  virtual iterator _find(const key_type& x) const {\r\n    auto reached = _traverse(x);\r\
    \n    int i = reached.first;\r\n    node_ptr u = reached.second;\r\n    if (i\
    \ == W)\r\n      return iterator(std::static_pointer_cast<Leaf>(u));\r\n    else\r\
    \n      return end();\r\n  }\r\n  virtual node_ptr create_node_at(const key_type&,\
    \ int) {\r\n    return std::make_shared<Node>();\r\n  }\r\n  virtual leaf_ptr\
    \ create_leaf_at(const key_type&, const init_type& value) {\r\n    return std::make_shared<Leaf>(value);\r\
    \n  }\r\n  virtual leaf_ptr create_leaf_at(const key_type&, init_type&& value)\
    \ {\r\n    return std::make_shared<Leaf>(std::move(value));\r\n  }\r\n  template<typename\
    \ Value>\r\n  iterator _emplace_impl(key_type x, int height, node_ptr forked,\
    \ Value&& value) {\r\n    assert(height < W);\r\n    int i = height;\r\n    node_ptr\
    \ u = forked;\r\n    auto f = u;\r\n    int c = (x >> (W-i-1)) & 1;\r\n    auto\
    \ fc = c;\r\n    auto fi = i;\r\n    auto pred = c == 1 ? u->jump : u->jump->prev();\r\
    \n    u->jump = nullptr;\r\n    auto l = create_leaf_at(x, std::forward<Value>(value));\r\
    \n    l->set_prev(pred);\r\n    l->set_next(pred->next());\r\n    l->prev()->set_next(l);\r\
    \n    l->next()->set_prev(l);\r\n    for (; i < W-1; i++) {\r\n      c = (x >>\
    \ (W-i-1)) & 1;\r\n      assert(!u->c[c]);\r\n      u->c[c] = create_node_at(x,\
    \ i+1);\r\n      u->c[c]->parent = u;\r\n      u->c[c]->jump = l;\r\n      u =\
    \ u->c[c];\r\n    }\r\n    {\r\n      c = (x >> (W-i-1)) & 1;\r\n      u->c[c]\
    \ = l;\r\n      u->c[c]->parent = u;\r\n    }\r\n    if (f == root_) [[unlikely]]\
    \ {\r\n      f->jump = l;\r\n    } else [[likely]] {\r\n      auto v = f->parent.lock();\r\
    \n      fi--;\r\n      while (v) {\r\n        c = x >> (W-fi-1) & 1;\r\n     \
    \   if (c != fc and !v->jump)\r\n          break;\r\n        if (!v->c[fc])\r\n\
    \          v->jump = l;\r\n        v = v->parent.lock();\r\n        fi--;\r\n\
    \      }\r\n    }\r\n    size_++;\r\n    return iterator(l);\r\n  }\r\n  template<typename\
    \ Value>\r\n  std::pair<iterator, bool> _insert(Value&& value) {\r\n    static_assert(std::is_convertible<Value,\
    \ value_type>::value, \"\");\r\n    key_type x = types::key_of(value);\r\n   \
    \ auto reached = _traverse(x);\r\n    int i = reached.first;\r\n    node_ptr u\
    \ = reached.second;\r\n    if (i == W)\r\n      return std::make_pair(iterator(std::static_pointer_cast<Leaf>(u)),\
    \ false);\r\n    return std::make_pair(_emplace_impl(x, i, u, std::forward<Value>(value)),\
    \ true);\r\n  }\r\n  virtual std::pair<int, node_ptr> climb_to_lca(leaf_ptr l,\
    \ key_type x) {\r\n    key_type m = x ^ types::key_of(l->v);\r\n    if (m == 0)\r\
    \n      return std::make_pair(W, std::static_pointer_cast<Node>(l));\r\n    int\
    \ h = bm::clz(m) - (64 - W);\r\n    node_ptr f = std::static_pointer_cast<Node>(l);\r\
    \n    for (int i = W; i > h; i--)\r\n      f = f->parent.lock();\r\n    return\
    \ std::make_pair(h, f);\r\n  }\r\n  template<class Value>\r\n  iterator _emplace_hint(const_iterator\
    \ hint, Value&& value) {\r\n    key_type x = types::key_of(value);\r\n    if (empty())\r\
    \n      return _emplace_impl(x, 0, root_, std::forward<Value>(value));\r\n   \
    \ if (hint == end())\r\n      --hint;\r\n    int h;\r\n    node_ptr f;\r\n   \
    \ std::tie(h, f) = climb_to_lca(hint.ptr_, x);\r\n    std::tie(h, f) = _traverse(x,\
    \ h, f);\r\n    if (h == W)\r\n      return iterator(std::static_pointer_cast<Leaf>(f));\r\
    \n    return _emplace_impl(x, h, f, std::forward<Value>(value));\r\n  }\r\n\r\n\
    \  virtual void erase_node_at(const key_type&, int, node_ptr) {}\r\n  virtual\
    \ bool _erase(const key_type& key) {\r\n    auto it = _find(key);\r\n    if (it\
    \ != end()) {\r\n      _erase_from_leaf(types::key_of(*it), it.ptr_);\r\n    \
    \  return true;\r\n    } else {\r\n      return false;\r\n    }\r\n  }\r\n  template<typename\
    \ Key>\r\n  iterator _erase_from_leaf(Key&& key, leaf_ptr l) {\r\n    static_assert(std::is_convertible<Key,\
    \ key_type>::value, \"\");\r\n    key_type x = std::forward<Key>(key);\r\n   \
    \ assert(x == l->key());\r\n    l->prev()->set_next(l->next());\r\n    l->next()->set_prev(l->prev());\r\
    \n    int i,c;\r\n    auto v = std::static_pointer_cast<Node>(l);\r\n    for (i\
    \ = W-1; i >= 0; i--) {\r\n      erase_node_at(x, i+1, v);\r\n      v = v->parent.lock();\r\
    \n      c = (x >> (W-i-1)) & 1;\r\n      v->c[c] = nullptr;\r\n      if (v->c[c^1])\
    \ break;\r\n    }\r\n    auto nj = c ? l->prev() : l->next();\r\n    auto fc =\
    \ c;\r\n    v->jump = nj;\r\n    v = v->parent.lock();\r\n    i--;\r\n    for\
    \ (; i >= 0; i--) {\r\n      assert(v);\r\n      c = (x >> (W-i-1)) & 1;\r\n \
    \     if (c != fc) {\r\n        if (!v->jump) break;\r\n        v->jump = nj;\r\
    \n      }\r\n      v = v->parent.lock();\r\n    }\r\n    size_--;\r\n    return\
    \ iterator(l->next());\r\n  }\r\n  iterator iterator_remove_const(const const_iterator&\
    \ it) {\r\n    return iterator(it.ptr_);\r\n  }\r\n  iterator iterator_remove_const(const_iterator&&\
    \ it) {\r\n    return iterator(std::move(it.ptr_));\r\n  }\r\n  iterator _erase(iterator\
    \ it) {\r\n    if (it == end()) return it;\r\n    return _erase_from_leaf(types::key_of(*it),\
    \ it.ptr_);\r\n  }\r\n  iterator _erase(const_iterator it) {\r\n    if (it ==\
    \ end()) return iterator_remove_const(it);\r\n    return _erase_from_leaf(types::key_of(*it),\
    \ it.ptr_);\r\n  }\r\n protected:\r\n  template<bool Const>\r\n  struct iterator_base\
    \ {\r\n    using difference_type = ptrdiff_t;\r\n    using value_type = BinaryTrieBase::value_type;\r\
    \n    using pointer = typename std::conditional<Const,\r\n                   \
    \                           const value_type*,\r\n                           \
    \                   value_type*>::type;\r\n    using reference = typename std::conditional<Const,\r\
    \n                                                const value_type&,\r\n     \
    \                                           value_type&>::type;\r\n    using iterator_category\
    \ = std::bidirectional_iterator_tag;\r\n    leaf_ptr ptr_;\r\n    iterator_base(leaf_ptr\
    \ p) : ptr_(p) {}\r\n    template<bool C>\r\n    iterator_base(const iterator_base<C>&\
    \ rhs) : ptr_(rhs.ptr_) {}\r\n    template<bool C>\r\n    iterator_base& operator=(const\
    \ iterator_base<C>& rhs) {\r\n      ptr_ = rhs.ptr_;\r\n    }\r\n    template<bool\
    \ C>\r\n    iterator_base(iterator_base<C>&& rhs) : ptr_(std::move(rhs.ptr_))\
    \ {}\r\n    template<bool C>\r\n    iterator_base& operator=(iterator_base<C>&&\
    \ rhs) {\r\n      ptr_ = std::move(rhs.ptr_);\r\n    }\r\n    reference operator*()\
    \ {\r\n      return ptr_->v;\r\n    }\r\n    pointer operator->() {\r\n      return\
    \ &(ptr_->v);\r\n    }\r\n    template<bool C>\r\n    bool operator==(const iterator_base<C>&\
    \ rhs) const {\r\n      return ptr_ == rhs.ptr_;\r\n    }\r\n    template<bool\
    \ C>\r\n    bool operator!=(const iterator_base<C>& rhs) const {\r\n      return\
    \ !operator==(rhs);\r\n    }\r\n    iterator_base& operator++() {\r\n      ptr_\
    \ = ptr_->next();\r\n      return *this;\r\n    }\r\n    iterator_base operator++(int)\
    \ const {\r\n      iterator_base ret = *this;\r\n      operator++();\r\n     \
    \ return ret;\r\n    }\r\n    iterator_base& operator--() {\r\n      ptr_ = ptr_->prev();\r\
    \n      return *this;\r\n    }\r\n    iterator_base operator--(int) const {\r\n\
    \      iterator_base ret = *this;\r\n      operator--();\r\n      return ret;\r\
    \n    }\r\n  };\r\n};\r\n\r\ntemplate<typename T, typename V, uint8_t W = sizeof(T)*8>\r\
    \nusing BinaryTrie = traits::MapTraits<BinaryTrieBase<T, V, W>>;\r\ntemplate<typename\
    \ T, uint8_t W = sizeof(T)*8>\r\nusing BinaryTrieSet = traits::SetTraits<BinaryTrieBase<T,\
    \ void, W>>;\r\ntemplate<typename T, typename V, uint8_t W = sizeof(T)*8>\r\n\
    using BinaryTrieMap = BinaryTrie<T, V, W>;\r\n#line 7 \"include/mtl/xft.hpp\"\n\
    #include <unordered_map>\r\n#line 9 \"include/mtl/xft.hpp\"\n\r\ntemplate<class\
    \ T, class M, int8_t W>\r\nusing XFastTrieHashTableMappedType = typename BinaryTrieBase<T,\
    \ M, W>::node_ptr;\r\n#define XFT_DEFAULT_HASH_TABLE        std::unordered_map\r\
    \n#define XFT_HASH_TABLE_TYPE(HT,T,M,W) HT<T, XFastTrieHashTableMappedType<T,\
    \ M, W>>\r\n\r\ntemplate<typename T, typename M,\r\n    int8_t W = sizeof(T) *\
    \ 8,\r\n    class HashTable = XFT_HASH_TABLE_TYPE(XFT_DEFAULT_HASH_TABLE, T, M,\
    \ W)>\r\nclass XFastTrieBase : public BinaryTrieBase<T, M, W> {\r\n  static_assert(std::is_unsigned<T>::value,\
    \ \"\");\r\n  using Base = BinaryTrieBase<T, M, W>;\r\n public:\r\n  using hash_table_type\
    \ = HashTable;\r\n  using types = typename Base::types;\r\n  using value_type\
    \ = typename types::value_type;\r\n  using init_type = typename types::init_type;\r\
    \n  using typename Base::Node;\r\n  using typename Base::Leaf;\r\n  using typename\
    \ Base::node_ptr;\r\n  using typename Base::leaf_ptr;\r\n  using typename Base::key_type;\r\
    \n protected:\r\n  using Base::root_;\r\n  using Base::dummy_;\r\n  using Base::size_;\r\
    \n  std::array<hash_table_type, W+1> tb_;\r\n  void _store_node(const int i, const\
    \ key_type& x, node_ptr u) {\r\n    tb_[i].emplace(W-i < (int)sizeof(key_type)*8\
    \ ? (x >> (W-i)) : 0, u);\r\n  }\r\n  void _init() override {\r\n    for (auto&\
    \ t:tb_) t.clear();\r\n    Base::_init();\r\n  }\r\n public:\r\n  XFastTrieBase()\
    \ : Base() {}\r\n  XFastTrieBase(const XFastTrieBase& rhs) {\r\n    Base::operator=(rhs);\r\
    \n  }\r\n  XFastTrieBase& operator=(const XFastTrieBase& rhs) {\r\n    Base::operator=(rhs);\r\
    \n  }\r\n  XFastTrieBase(XFastTrieBase&& rhs) noexcept {\r\n    Base::operator=(std::move(rhs));\r\
    \n  }\r\n  XFastTrieBase& operator=(XFastTrieBase&& rhs) noexcept {\r\n    Base::operator=(std::move(rhs));\r\
    \n  }\r\n  template<typename InputIt>\r\n  explicit XFastTrieBase(InputIt begin,\
    \ InputIt end) {\r\n    Base::_insert_init(begin, end);\r\n  }\r\n  using iterator\
    \ = typename Base::iterator;\r\n  using Base::end;\r\n protected:\r\n  node_ptr\
    \ create_node_at(const key_type& x, int i) override {\r\n    auto u = Base::create_node_at(x,\
    \ i);\r\n    _store_node(i, x, u);\r\n    return u;\r\n  }\r\n  leaf_ptr create_leaf_at(const\
    \ key_type& x, const init_type& value) override {\r\n    auto l = Base::create_leaf_at(x,\
    \ value);\r\n    _store_node(W, x, std::static_pointer_cast<Node>(l));\r\n   \
    \ return l;\r\n  }\r\n  leaf_ptr create_leaf_at(const key_type& x, init_type&&\
    \ value) override {\r\n    auto l = Base::create_leaf_at(x, std::move(value));\r\
    \n    _store_node(W, x, std::static_pointer_cast<Node>(l));\r\n    return l;\r\
    \n  }\r\n  void erase_node_at(const key_type& x, int i, node_ptr u) override {\r\
    \n    Base::erase_node_at(x, i, u);\r\n    auto it = tb_[i].find(W-i < (int)sizeof(key_type)*8\
    \ ? (x >> (W-i)) : 0);\r\n    assert(it != tb_[i].end());\r\n    assert(it->second\
    \ == u);\r\n    tb_[i].erase(it);\r\n  }\r\n  std::pair<int, node_ptr> _traverse(const\
    \ key_type& key, \r\n                                     int depth = 0, \r\n\
    \                                     node_ptr root = nullptr) const override\
    \ {\r\n    key_type x = key;\r\n    int l = depth, h = W+1;\r\n    node_ptr u\
    \ = !root ? root_ : root;\r\n    while (l+1 < h) {\r\n      int i = l+(h-l)/2;\r\
    \n      auto p = W-i < (int)sizeof(key_type)*8 ? (x >> (W-i)) : 0;\r\n      auto\
    \ it = tb_[i].find(p);\r\n      if (it != tb_[i].end()) {\r\n        l = i;\r\n\
    \        u = it->second;\r\n      } else {\r\n        h = i;\r\n      }\r\n  \
    \  }\r\n    return std::make_pair(l, u);\r\n  }\r\n  iterator _find(const key_type&\
    \ x) const override {\r\n    auto it = tb_[W].find(x);\r\n    if (it != tb_[W].end())\r\
    \n      return iterator(std::static_pointer_cast<Leaf>(it->second));\r\n    else\r\
    \n      return end();\r\n  }\r\n  using Base::_insert;\r\n  std::pair<int, node_ptr>\
    \ climb_to_lca(leaf_ptr l, key_type x) override {\r\n    key_type m = x ^ types::key_of(l->v);\r\
    \n    if (m == 0)\r\n      return std::make_pair(W, std::static_pointer_cast<Node>(l));\r\
    \n    int h = bm::clz(m) - (64 - W);\r\n    key_type y = W-h < (int)sizeof(key_type)*8\
    \ ? (x >> (W-h)) : 0;\r\n    assert(tb_[h].count(y));\r\n    node_ptr f = tb_[h][y];\r\
    \n    return std::make_pair(h, f);\r\n  }\r\n  using Base::_emplace_hint;\r\n\
    \  using Base::_erase;\r\n  bool _erase(const key_type& key) override {\r\n  \
    \  auto it = tb_[W].find(key);\r\n    if (it != tb_[W].end()) {\r\n      Base::_erase_from_leaf(key,\
    \ std::static_pointer_cast<Leaf>(it->second));\r\n      return true;\r\n    }\
    \ else {\r\n      return false;\r\n    }\r\n  }\r\n};\r\n\r\n#line 137 \"include/mtl/xft.hpp\"\
    \n\r\ntemplate<typename T, typename V, uint8_t W = sizeof(T)*8,\r\n    class HashTable\
    \ = XFT_HASH_TABLE_TYPE(XFT_DEFAULT_HASH_TABLE, T, V, W)>\r\nusing XFastTrie =\
    \ traits::MapTraits<XFastTrieBase<T, V, W, HashTable>>;\r\ntemplate<typename T,\
    \ uint8_t W = sizeof(T)*8,\r\n    class HashTable = XFT_HASH_TABLE_TYPE(XFT_DEFAULT_HASH_TABLE,\
    \ T, void, W)>\r\nusing XFastTrieSet = traits::SetTraits<XFastTrieBase<T, void,\
    \ W, HashTable>>;\r\ntemplate<typename T, typename V, uint8_t W = sizeof(T)*8,\r\
    \n    class HashTable = XFT_HASH_TABLE_TYPE(XFT_DEFAULT_HASH_TABLE, T, V, W)>\r\
    \nusing XFastTrieMap = XFastTrie<T, V, W, HashTable>;\r\n"
  code: "#pragma once\r\n#include \"binary_trie.hpp\"\r\n#include <array>\r\n#include\
    \ <memory>\r\n#include <type_traits>\r\n#include <cstdint>\r\n#include <unordered_map>\r\
    \n#include <cassert>\r\n\r\ntemplate<class T, class M, int8_t W>\r\nusing XFastTrieHashTableMappedType\
    \ = typename BinaryTrieBase<T, M, W>::node_ptr;\r\n#define XFT_DEFAULT_HASH_TABLE\
    \        std::unordered_map\r\n#define XFT_HASH_TABLE_TYPE(HT,T,M,W) HT<T, XFastTrieHashTableMappedType<T,\
    \ M, W>>\r\n\r\ntemplate<typename T, typename M,\r\n    int8_t W = sizeof(T) *\
    \ 8,\r\n    class HashTable = XFT_HASH_TABLE_TYPE(XFT_DEFAULT_HASH_TABLE, T, M,\
    \ W)>\r\nclass XFastTrieBase : public BinaryTrieBase<T, M, W> {\r\n  static_assert(std::is_unsigned<T>::value,\
    \ \"\");\r\n  using Base = BinaryTrieBase<T, M, W>;\r\n public:\r\n  using hash_table_type\
    \ = HashTable;\r\n  using types = typename Base::types;\r\n  using value_type\
    \ = typename types::value_type;\r\n  using init_type = typename types::init_type;\r\
    \n  using typename Base::Node;\r\n  using typename Base::Leaf;\r\n  using typename\
    \ Base::node_ptr;\r\n  using typename Base::leaf_ptr;\r\n  using typename Base::key_type;\r\
    \n protected:\r\n  using Base::root_;\r\n  using Base::dummy_;\r\n  using Base::size_;\r\
    \n  std::array<hash_table_type, W+1> tb_;\r\n  void _store_node(const int i, const\
    \ key_type& x, node_ptr u) {\r\n    tb_[i].emplace(W-i < (int)sizeof(key_type)*8\
    \ ? (x >> (W-i)) : 0, u);\r\n  }\r\n  void _init() override {\r\n    for (auto&\
    \ t:tb_) t.clear();\r\n    Base::_init();\r\n  }\r\n public:\r\n  XFastTrieBase()\
    \ : Base() {}\r\n  XFastTrieBase(const XFastTrieBase& rhs) {\r\n    Base::operator=(rhs);\r\
    \n  }\r\n  XFastTrieBase& operator=(const XFastTrieBase& rhs) {\r\n    Base::operator=(rhs);\r\
    \n  }\r\n  XFastTrieBase(XFastTrieBase&& rhs) noexcept {\r\n    Base::operator=(std::move(rhs));\r\
    \n  }\r\n  XFastTrieBase& operator=(XFastTrieBase&& rhs) noexcept {\r\n    Base::operator=(std::move(rhs));\r\
    \n  }\r\n  template<typename InputIt>\r\n  explicit XFastTrieBase(InputIt begin,\
    \ InputIt end) {\r\n    Base::_insert_init(begin, end);\r\n  }\r\n  using iterator\
    \ = typename Base::iterator;\r\n  using Base::end;\r\n protected:\r\n  node_ptr\
    \ create_node_at(const key_type& x, int i) override {\r\n    auto u = Base::create_node_at(x,\
    \ i);\r\n    _store_node(i, x, u);\r\n    return u;\r\n  }\r\n  leaf_ptr create_leaf_at(const\
    \ key_type& x, const init_type& value) override {\r\n    auto l = Base::create_leaf_at(x,\
    \ value);\r\n    _store_node(W, x, std::static_pointer_cast<Node>(l));\r\n   \
    \ return l;\r\n  }\r\n  leaf_ptr create_leaf_at(const key_type& x, init_type&&\
    \ value) override {\r\n    auto l = Base::create_leaf_at(x, std::move(value));\r\
    \n    _store_node(W, x, std::static_pointer_cast<Node>(l));\r\n    return l;\r\
    \n  }\r\n  void erase_node_at(const key_type& x, int i, node_ptr u) override {\r\
    \n    Base::erase_node_at(x, i, u);\r\n    auto it = tb_[i].find(W-i < (int)sizeof(key_type)*8\
    \ ? (x >> (W-i)) : 0);\r\n    assert(it != tb_[i].end());\r\n    assert(it->second\
    \ == u);\r\n    tb_[i].erase(it);\r\n  }\r\n  std::pair<int, node_ptr> _traverse(const\
    \ key_type& key, \r\n                                     int depth = 0, \r\n\
    \                                     node_ptr root = nullptr) const override\
    \ {\r\n    key_type x = key;\r\n    int l = depth, h = W+1;\r\n    node_ptr u\
    \ = !root ? root_ : root;\r\n    while (l+1 < h) {\r\n      int i = l+(h-l)/2;\r\
    \n      auto p = W-i < (int)sizeof(key_type)*8 ? (x >> (W-i)) : 0;\r\n      auto\
    \ it = tb_[i].find(p);\r\n      if (it != tb_[i].end()) {\r\n        l = i;\r\n\
    \        u = it->second;\r\n      } else {\r\n        h = i;\r\n      }\r\n  \
    \  }\r\n    return std::make_pair(l, u);\r\n  }\r\n  iterator _find(const key_type&\
    \ x) const override {\r\n    auto it = tb_[W].find(x);\r\n    if (it != tb_[W].end())\r\
    \n      return iterator(std::static_pointer_cast<Leaf>(it->second));\r\n    else\r\
    \n      return end();\r\n  }\r\n  using Base::_insert;\r\n  std::pair<int, node_ptr>\
    \ climb_to_lca(leaf_ptr l, key_type x) override {\r\n    key_type m = x ^ types::key_of(l->v);\r\
    \n    if (m == 0)\r\n      return std::make_pair(W, std::static_pointer_cast<Node>(l));\r\
    \n    int h = bm::clz(m) - (64 - W);\r\n    key_type y = W-h < (int)sizeof(key_type)*8\
    \ ? (x >> (W-h)) : 0;\r\n    assert(tb_[h].count(y));\r\n    node_ptr f = tb_[h][y];\r\
    \n    return std::make_pair(h, f);\r\n  }\r\n  using Base::_emplace_hint;\r\n\
    \  using Base::_erase;\r\n  bool _erase(const key_type& key) override {\r\n  \
    \  auto it = tb_[W].find(key);\r\n    if (it != tb_[W].end()) {\r\n      Base::_erase_from_leaf(key,\
    \ std::static_pointer_cast<Leaf>(it->second));\r\n      return true;\r\n    }\
    \ else {\r\n      return false;\r\n    }\r\n  }\r\n};\r\n\r\n#include \"traits/set_traits.hpp\"\
    \r\n\r\ntemplate<typename T, typename V, uint8_t W = sizeof(T)*8,\r\n    class\
    \ HashTable = XFT_HASH_TABLE_TYPE(XFT_DEFAULT_HASH_TABLE, T, V, W)>\r\nusing XFastTrie\
    \ = traits::MapTraits<XFastTrieBase<T, V, W, HashTable>>;\r\ntemplate<typename\
    \ T, uint8_t W = sizeof(T)*8,\r\n    class HashTable = XFT_HASH_TABLE_TYPE(XFT_DEFAULT_HASH_TABLE,\
    \ T, void, W)>\r\nusing XFastTrieSet = traits::SetTraits<XFastTrieBase<T, void,\
    \ W, HashTable>>;\r\ntemplate<typename T, typename V, uint8_t W = sizeof(T)*8,\r\
    \n    class HashTable = XFT_HASH_TABLE_TYPE(XFT_DEFAULT_HASH_TABLE, T, V, W)>\r\
    \nusing XFastTrieMap = XFastTrie<T, V, W, HashTable>;\r\n"
  dependsOn:
  - include/mtl/binary_trie.hpp
  - include/mtl/traits/set_traits.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/xft.hpp
  requiredBy:
  - include/mtl/yft.hpp
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_SOME_WA
  verifiedWith:
  - test/yosupo/associative_array-yft.test.cpp
  - test/yosupo/predecessor_problem-yft.test.cpp
documentation_of: include/mtl/xft.hpp
layout: document
redirect_from:
- /library/include/mtl/xft.hpp
- /library/include/mtl/xft.hpp.html
title: include/mtl/xft.hpp
---
