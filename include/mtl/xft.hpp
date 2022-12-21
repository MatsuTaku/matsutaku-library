#pragma once
#include "binary_trie.hpp"
#include <array>
#include <memory>
#include <type_traits>
#include <cstdint>
#include <unordered_map>
#include <cassert>

template<typename T, typename M,
    int8_t W = sizeof(T) * 8,
    typename HashTable = std::unordered_map<T, typename BinaryTrieDefinition<T, M>::node_ptr>>
class XFastTrieBase : public BinaryTrieBase<T, M, W> {
  static_assert(std::is_unsigned<T>::value, "");
  using Base = BinaryTrieBase<T, M, W>;
  using Def = AssociativeArrayDefinition<T, M>;
 public:
  using typename Base::Node;
  using typename Base::Leaf;
  using typename Base::node_ptr;
  using typename Base::leaf_ptr;
  using typename Base::key_type;
  using typename Base::value_type;
  using hash_table_type = HashTable;
 protected:
  using Base::root_;
  using Base::dummy_;
  using Base::size_;
  std::array<hash_table_type, W+1> tb_;
  void _init() override {
    Base::_init();
    tb_[0].emplace(0, std::static_pointer_cast<Node>(root_));
  }
 public:
  XFastTrieBase() : Base() {}
  template<typename InputIt>
  explicit XFastTrieBase(InputIt begin, InputIt end) : XFastTrieBase() {
    static_assert(std::is_convertible<typename std::iterator_traits<InputIt>::value_type, value_type>::value, "");
    // TODO: optimize
    for (auto it = begin; it != end; ++it)
      _insert(*it);
  }
  XFastTrieBase(std::initializer_list<value_type> init) : XFastTrieBase(init.begin(), init.end()) {}
  using iterator = typename Base::iterator;
  using Base::end;
 protected:
  template<class Key>
  inline std::pair<int, node_ptr> _traverse(Key&& key) const {
    key_type x = std::forward<Key>(key);
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
    return std::make_pair(l, u);
  }
  template<class Key>
  inline iterator _lower_bound(Key&& key) const {
    auto reached = _traverse(std::forward<Key>(key));
    int& l = reached.first;
    node_ptr& u = reached.second;
    if (l == W) return iterator(std::static_pointer_cast<Leaf>(u));
    key_type x = std::forward<Key>(key);
    auto lb = ((x >> (W-l-1)) & 1) == 0 ? u->jump : u->jump->next();
    return lb == dummy_ ? end() : iterator(lb);
  }
  template<class Key>
  inline iterator _upper_bound(Key&& key) const {
    auto reached = _traverse(std::forward<Key>(key));
    int& l = reached.first;
    node_ptr& u = reached.second;
    if (l == W) return iterator(std::static_pointer_cast<Leaf>(u)->next());
    key_type x = std::forward<Key>(key);
    auto lb = ((x >> (W-l-1)) & 1) == 0 ? u->jump : u->jump->next();
    return lb == dummy_ ? end() : iterator(lb);
  }
  template<class Key>
  inline iterator _find(Key&& key) const {
    key_type x = std::forward<Key>(key);
    auto it = tb_[W].find(x);
    if (it != tb_[W].end())
      return iterator(std::static_pointer_cast<Leaf>(it->second));
    else
      return end();
  }
  inline void _store_node(const int i, const key_type& x, node_ptr u) {
    tb_[i].emplace(x >> (W-i), u);
  }
  inline node_ptr create_node_at(const key_type& x, int i) override {
    auto u = Base::create_node_at(x, i);
    _store_node(i, x, u);
    return u;
  }
  inline leaf_ptr create_leaf_at(const key_type& x, const value_type& value) override {
    auto l = Base::create_leaf_at(x, value);
    _store_node(W, x, std::static_pointer_cast<Node>(l));
    return l;
  }
  inline leaf_ptr create_leaf_at(const key_type& x, value_type&& value) override {
    auto l = Base::create_leaf_at(x, std::move(value));
    _store_node(W, x, std::static_pointer_cast<Node>(l));
    return l;
  }
  using Base::_insert;
  inline void erase_node_at(const key_type& x, int i, node_ptr u) override {
    Base::erase_node_at(x, i, u);
    auto it = tb_[i].find(x >> (W-i));
    assert(it != tb_[i].end());
    assert(it->second == u);
    tb_[i].erase(it);
  }
  using Base::_erase;
  template<class Key>
  inline bool _erase(Key&& key) {
    key_type x = std::forward<Key>(key);
    auto it = tb_[W].find(x);
    if (it != tb_[W].end()) {
      Base::_erase(x, std::static_pointer_cast<Leaf>(it->second));
      return true;
    } else {
      return false;
    }
  }
};

template<typename T, typename V, uint8_t W = sizeof(T)*8>
using XFastTrie = MapInterface<XFastTrieBase<T, V, W>>;
template<typename T, uint8_t W = sizeof(T)*8>
using XFastTrieSet = SetInterface<XFastTrieBase<T, void, W>>;
template<typename T, typename V, uint8_t W = sizeof(T)*8>
using XFastTrieMap = XFastTrie<T, V, W>;
