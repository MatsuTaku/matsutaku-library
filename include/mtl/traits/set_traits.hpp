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
  using raw_key_type = typename std::remove_reference<T>::type;
  using raw_mapped_type = typename std::remove_reference<M>::type;
  using init_type = std::pair<raw_key_type, raw_mapped_type>;
  using moved_type = std::pair<raw_key_type&&, raw_mapped_type&&>;
  template<class K, class V>
  static inline key_type const& key_of(std::pair<K,V> const& kv) {
    return kv.first;
  }
};
template<typename T>
struct AssociativeArrayDefinition<T, void> {
  using key_type = T;
  using value_type = T;
  using init_type = T;
  static inline key_type const& key_of(value_type const& k) { return k; }
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
  using Base::begin;
  using Base::end;
  using reverse_iterator = std::reverse_iterator<iterator>;
  inline reverse_iterator rbegin() const {
    return reverse_iterator(begin());
  }
  inline reverse_iterator rend() const {
    return reverse_iterator(end());
  }
  template<class Key>
  inline iterator lower_bound(Key&& x) const {
    return Base::_lower_bound(std::forward<Key>(x));
  }
  inline iterator lower_bound(const key_type& x) const {
    return Base::_lower_bound(x);
  }
  inline iterator lower_bound(key_type&& x) const {
    return Base::_lower_bound(std::move(x));
  }
  template<class Key>
  inline iterator upper_bound(Key&& x) const {
    return Base::_upper_bound(std::forward<Key>(x));
  }
  inline iterator upper_bound(const key_type& x) const {
    return Base::_upper_bound(x);
  }
  inline iterator upper_bound(key_type&& x) const {
    return Base::_upper_bound(std::move(x));
  }
  template<class Key>
  inline iterator find(Key&& x) const {
    return Base::_find(std::forward<Key>(x));
  }
  inline iterator find(const key_type& x) const {
    return Base::_find(x);
  }
  inline iterator find(key_type&& x) const {
    return Base::_find(std::move(x));
  }
  template<class Key>
  inline size_t count(Key&& x) const {
    return find(std::forward<Key>(x)) != end();
  }
  inline size_t count(const key_type& x) const {
    return find(x) != end();
  }
  inline size_t count(key_type&& x) const {
    return find(std::move(x)) != end();
  }
  template<class Value>
  inline std::pair<iterator, bool> insert(Value&& v) {
    return Base::_insert(std::forward<Value>(v));
  }
  inline std::pair<iterator, bool> insert(const value_type& v) {
    return Base::_insert(v);
  }
  inline std::pair<iterator, bool> insert(value_type&& v) {
    return Base::_insert(std::move(v));
  }
  // TODO
//  template<class... Args>
//  inline std::pair<iterator, bool> emplace(Args&&... args) {
//    return Base::_emplace(std::forward<Args>(args)...);
//  }
  template<class Key>
  inline bool erase(Key&& x) {
    return Base::_erase(std::forward<Key>(x));
  }
  inline bool erase(const key_type& x) {
    return Base::_erase(x);
  }
  inline bool erase(key_type&& x) {
    return Base::_erase(std::move(x));
  }
  inline iterator erase(iterator it) {
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
  inline reference operator[](const key_type& x) {
    // TODO
//    return SBase::try_emplace(x).first->second;
    auto it = SBase::insert({x, mapped_type()}).first;
    return it->second;
  }
  inline reference operator[](key_type&& x) {
    // TODO
//    return SBase::try_emplace(std::move(x)).first->second;
    auto it = SBase::insert({std::move(x), mapped_type()}).first;
    return it->second;
  }
};

} // namespace traits