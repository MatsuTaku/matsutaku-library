#pragma once
#include "binary_trie.hpp"
#include <array>
#include <memory>
#include <type_traits>
#include <cstdint>
#include <unordered_map>
#include <cassert>
#include <vector>
#include <bitset>
#include <algorithm>

template<typename T, typename M,
    int8_t W = sizeof(T) * 8,
    typename HashTable = std::unordered_map<T, typename BinaryTrieDefinition<T, M>::node_ptr>>
class XFastTrieBase : public BinaryTrieBase<T, M, W> {
  static_assert(std::is_unsigned<T>::value, "");
  using Base = BinaryTrieBase<T, M, W>;
 public:
  using Node = typename Base::Node;
  using Leaf = typename Base::Leaf;
  using node_ptr = typename Base::node_ptr;
  using leaf_ptr = typename Base::leaf_ptr;
 protected:
  using key_type = typename Base::key_type;
  static constexpr bool kPairValue = Base::kPairValue;
  using value_type = typename Base::value_type;
  using mapped_type = typename Base::mapped_type;
  using hash_table_type = HashTable;
  using Base::root_;
  using Base::dummy_;
  using Base::size_;
  std::array<hash_table_type, W+1> tb_;
  void _init() {
    Base::_init();
    tb_[0].emplace(0, std::static_pointer_cast<Node>(dummy_));
  }
  XFastTrieBase() : Base() {}
 public:
  using iterator = typename Base::iterator;
  using Base::begin;
  using Base::end;
  using Base::size;
 protected:
  iterator _lower_bound(key_type x) const {
    int l = 0, h = W+1;
    auto u = root_;
    while (l+1 < h) {
      int i = l+(h-l)/2;
      auto p = x >> (W-i);
      auto it = tb_[i].find(p);
      if (it != tb_[i].end()) {
        l = i;
        u = it->second;
      } else {
        h = i;
      }
    }
    if (l == W) return iterator(std::static_pointer_cast<Leaf>(u));
    auto lb = ((x >> (W-l-1)) & 1) == 0 ? u->jump : u->jump->next();
    return lb == dummy_ ? end() : iterator(lb);
  }
  iterator _upper_bound(key_type x) const {
    auto it = _lower_bound(x);
    if (Base::key_of(it) == x)
      ++it;
    return it;
  }
  iterator _find(key_type x) const {
    auto it = tb_[W].find(x);
    if (it != tb_[W].end())
      return iterator(std::static_pointer_cast<Leaf>(it->second));
    else
      return end();
  }
  inline node_ptr create_node_at(key_type x, int i) override {
    auto u = Base::create_node_at(x, i);
    tb_[i].emplace(x >> (W-i), u);
    return u;
  }
  inline node_ptr create_leaf_at(key_type x, value_type value) override {
    auto u = Base::create_leaf_at(x, std::forward<value_type>(value));
    tb_[W].emplace(x, u);
    return u;
  }
  using Base::_insert;
  inline void erase_node_at(key_type x, int i, node_ptr u) override {
    Base::erase_node_at(x, i, u);
    auto it = tb_[i].find(x >> (W-i));
    assert(it != tb_[i].end());
    assert(it->second == u);
    tb_[i].erase(it);
  }
  using Base::_erase;
};

template<typename T, typename V>
class XFastTrie : public XFastTrieBase<T, V> {
  using Base = XFastTrieBase<T, V>;
 public:
  using key_type = typename Base::key_type;
  using mapped_type = typename Base::mapped_type;
  using value_type = typename Base::value_type;
  using iterator = typename Base::iterator;
  XFastTrie() = default;
  iterator lower_bound(const key_type& x) const {
    return Base::_lower_bound(x);
  }
  iterator lower_bound(key_type&& x) const {
    return Base::_lower_bound(std::forward<key_type>(x));
  }
  iterator upper_bound(const key_type& x) const {
    return Base::_upper_bound(x);
  }
  iterator upper_bound(key_type&& x) const {
    return Base::_upper_bound(std::forward<key_type>(x));
  }
  iterator find(const key_type& x) const {
    return Base::_find(x);
  }
  iterator find(key_type&& x) const {
    return Base::_find(std::forward<key_type>(x));
  }
  std::pair<iterator, bool> insert(const value_type& v) {
    return Base::_insert(v);
  }
  std::pair<iterator, bool> insert(value_type&& v) {
    return Base::_insert(std::forward<value_type>(v));
  }
  bool erase(const key_type& x) {
    return Base::_erase(x);
  }
  bool erase(key_type&& x) {
    return Base::_erase(x);
  }
};

template<typename T>
using XFastTrieSet = XFastTrie<T, void>;
template<typename T, typename V>
using XFastTrieMap = XFastTrie<T, V>;
