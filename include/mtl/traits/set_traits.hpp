#pragma once
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <iterator>

namespace traits {

template<typename T, typename M>
struct AssociativeArrayDefinition {
  using key_type = T;
  using mapped_type = M;
  using value_type = std::pair<T const, M>;
  using raw_key_type = typename std::remove_const<T>::type;
  using raw_mapped_type = typename std::remove_const<M>::type;
  using init_type = std::pair<raw_key_type, raw_mapped_type>;
  using moved_type = std::pair<raw_key_type&&, raw_mapped_type&&>;
  template<class K, class V>
  static key_type const& key_of(std::pair<K,V> const& kv) {
    return kv.first;
  }
};
template<typename T>
struct AssociativeArrayDefinition<T, void> {
  using key_type = T;
  using value_type = T;
  using init_type = T;
  static key_type const& key_of(value_type const& k) { return k; }
};

template<class T, typename = std::void_t<>>
struct get_const_iterator {
  using base = typename T::iterator;
  struct type : base {
    type(const base& r) : base(r) {}
    type(base&& r) : base(std::move(r)) {}
  };
};
template<class T>
struct get_const_iterator<T, std::void_t<typename T::const_iterator>> {
  using type = typename T::const_iterator;
};

#if __cplusplus >= 202002L
#include <concepts>
template<class M>
concept IsAssociativeArray = requires (M m) {
  typename M::key_type;
  typename M::value_type;
  typename M::iterator;
  {m.size()} -> std::convertible_to<size_t>;
  {m.empty()} -> std::same_as<bool>;
  {m.clear()};
  {m.begin()} -> std::same_as<typename M::iterator>;
  {m.end()} -> std::same_as<typename M::iterator>;
};
#endif

template<class Base>
#if __cplusplus >= 202002L
requires IsAssociativeArray<Base>
#endif
class SetTraitsBase : public Base {
 public:
  using key_type = typename Base::key_type;
  using value_type = typename Base::value_type;
  using init_type = typename Base::init_type;
  using iterator = typename Base::iterator;
  SetTraitsBase() = default;
  template<typename InputIt>
  explicit SetTraitsBase(InputIt begin, InputIt end) : Base(begin, end) {
    static_assert(std::is_convertible<typename std::iterator_traits<InputIt>::value_type, value_type>::value, "");
  }
  SetTraitsBase(std::initializer_list<value_type> init) : Base(init.begin(), init.end()) {}
  using Base::size;
  bool empty() const { return size() == 0; }
  using Base::clear;
  using const_iterator = typename get_const_iterator<Base>::type;
  iterator begin() {
    return Base::begin();
  }
  iterator end() {
    return Base::end();
  }
  const_iterator begin() const {
    return const_iterator(Base::begin());
  }
  const_iterator end() const {
    return const_iterator(Base::end());
  }
  const_iterator cbegin() const {
    return begin();
  }
  const_iterator cend() const {
    return end();
  }
  using reverse_iterator = std::reverse_iterator<iterator>;
  using reverse_const_iterator = std::reverse_iterator<const_iterator>;
  reverse_iterator rbegin() {
    return std::make_reverse_iterator(end());
  }
  reverse_iterator rend() {
    return std::make_reverse_iterator(begin());
  }
  reverse_const_iterator rbegin() const {
    return std::make_reverse_iterator(end());
  }
  reverse_const_iterator rend() const {
    return std::make_reverse_iterator(begin());
  }
  reverse_const_iterator crbegin() const {
    return rbegin();
  }
  reverse_const_iterator crend() const {
    return rend();
  }
  template<class Key>
  iterator lower_bound(const Key& x) const {
    return Base::_lower_bound(x);
  }
  iterator lower_bound(const key_type& x) const {
    return Base::_lower_bound(x);
  }
  template<class Key>
  iterator upper_bound(const Key& x) const {
    return Base::_upper_bound(x);
  }
  iterator upper_bound(const key_type& x) const {
    return Base::_upper_bound(x);
  }
  template<class Key>
  iterator find(const Key& x) {
    return Base::_find(x);
  }
  iterator find(const key_type& x) {
    return Base::_find(x);
  }
  template<class Key>
  const_iterator find(const Key& x) const {
    return Base::_find(x);
  }
  const_iterator find(const key_type& x) const {
    return Base::_find(x);
  }
  template<class Key>
  size_t count(const Key& x) const {
    return find(x) != end();
  }
  size_t count(const key_type& x) const {
    return find(x) != end();
  }
  std::pair<iterator, bool> insert(const init_type& v) {
    return Base::_insert(v);
  }
  std::pair<iterator, bool> insert(init_type&& v) {
    return Base::_insert(std::move(v));
  }
  template<typename=void>
  std::pair<iterator, bool> insert(const value_type& v) {
    return Base::_insert(v);
  }
  template<typename=void>
  std::pair<iterator, bool> insert(value_type&& v) {
    return Base::_insert(std::move(v));
  }
  template<class... Args>
  std::pair<iterator, bool> emplace(Args&&... args) {
    using emplace_type = typename std::conditional<
        std::is_constructible<init_type, Args...>::value,
        init_type,
        value_type
    >::type;
    return Base::_insert(emplace_type(std::forward<Args>(args)...));
  }
  template<class... Args>
  iterator emplace_hint(const_iterator hint, Args&&... args) {
    using emplace_type = typename std::conditional<
        std::is_constructible<init_type, Args...>::value,
        init_type,
        value_type
    >::type;
    return Base::_emplace_hint(hint, emplace_type(std::forward<Args>(args)...));
  }
  size_t erase(const key_type& x) {
    return Base::_erase(x);
  }
  iterator erase(iterator it) {
    return Base::_erase(it);
  }
  iterator erase(const_iterator it) {
    return Base::_erase(it);
  }
};

template<typename Base>
using SetTraits = SetTraitsBase<Base>;

template<typename Base>
class MapTraits : public SetTraitsBase<Base> {
  using SBase = SetTraitsBase<Base>;
 public:
  using typename SBase::key_type;
  using typename SBase::mapped_type;
  using typename SBase::value_type;
  using reference = mapped_type&;
  MapTraits() = default;
  template<typename InputIt>
  explicit MapTraits(InputIt begin, InputIt end) : SBase(begin, end) {}
  MapTraits(std::initializer_list<value_type> init) : SBase(init) {}
  template<typename Key>
  reference operator[](Key&& x) {
    auto i = SBase::lower_bound(x);
    if (i == SBase::end() || x < i->first) {
      i = SBase::emplace_hint(i, std::forward<Key>(x), mapped_type());
    }
    return i->second;
  }
  reference operator[](const key_type& x) {
    auto i = SBase::lower_bound(x);
    if (i == SBase::end() || x < i->first) {
      i = SBase::emplace_hint(i, x, mapped_type());
    }
    return i->second;
  }
  reference operator[](key_type&& x) {
    auto i = SBase::lower_bound(x);
    if (i == SBase::end() || x < i->first) {
      i = SBase::emplace_hint(i, std::move(x), mapped_type());
    }
    return i->second;
  }
};

} // namespace traits