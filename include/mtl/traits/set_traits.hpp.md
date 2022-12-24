---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':warning:'
    path: include/mtl/binary_trie.hpp
    title: include/mtl/binary_trie.hpp
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
    \ traits\n"
  code: "#pragma once\r\n#include <cstddef>\r\n#include <initializer_list>\r\n#include\
    \ <type_traits>\r\n#include <iterator>\r\n\r\nnamespace traits {\r\n\r\ntemplate<typename\
    \ T, typename M>\r\nstruct AssociativeArrayDefinition {\r\n  using key_type =\
    \ T;\r\n  using mapped_type = M;\r\n  using value_type = std::pair<T const, M>;\r\
    \n  using raw_key_type = typename std::remove_const<T>::type;\r\n  using raw_mapped_type\
    \ = typename std::remove_const<M>::type;\r\n  using init_type = std::pair<raw_key_type,\
    \ raw_mapped_type>;\r\n  using moved_type = std::pair<raw_key_type&&, raw_mapped_type&&>;\r\
    \n  template<class K, class V>\r\n  static inline key_type const& key_of(std::pair<K,V>\
    \ const& kv) {\r\n    return kv.first;\r\n  }\r\n};\r\ntemplate<typename T>\r\n\
    struct AssociativeArrayDefinition<T, void> {\r\n  using key_type = T;\r\n  using\
    \ value_type = T;\r\n  using init_type = T;\r\n  static inline key_type const&\
    \ key_of(value_type const& k) { return k; }\r\n};\r\n\r\n#if __cplusplus >= 202002L\r\
    \n#include <concepts>\r\ntemplate<class M>\r\nconcept IsAssociativeArray = requires\
    \ (M m) {\r\n  typename M::key_type;\r\n  typename M::value_type;\r\n  typename\
    \ M::iterator;\r\n  {m.size()} -> std::convertible_to<size_t>;\r\n  {m.empty()}\
    \ -> std::same_as<bool>;\r\n  {m.clear()};\r\n  {m.begin()} -> std::same_as<typename\
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
    \ traits"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/traits/set_traits.hpp
  requiredBy:
  - include/mtl/xft.hpp
  - include/mtl/yft.hpp
  - include/mtl/binary_trie.hpp
  timestamp: '2022-12-22 12:22:12+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/traits/set_traits.hpp
layout: document
redirect_from:
- /library/include/mtl/traits/set_traits.hpp
- /library/include/mtl/traits/set_traits.hpp.html
title: include/mtl/traits/set_traits.hpp
---
