#pragma once
#include "binary_trie.hpp"
#include <array>
#include <memory>
#include <type_traits>
#include <cstdint>
#include <unordered_map>
#include <cassert>

template<class T, class M, int8_t W>
using XFastTrieHashTableMappedType = typename BinaryTrieBase<T, M, W>::node_ptr;

template<typename T, typename M,
    int8_t W = sizeof(T) * 8,
    class HashTable = std::unordered_map<T, XFastTrieHashTableMappedType<T, M, W>>>
class XFastTrieBase : public BinaryTrieBase<T, M, W> {
  static_assert(std::is_unsigned<T>::value, "");
  using Base = BinaryTrieBase<T, M, W>;
 public:
  using hash_table_type = HashTable;
  using types = typename Base::types;
  using value_type = typename types::value_type;
  using init_type = typename types::init_type;
  using typename Base::Node;
  using typename Base::Leaf;
  using typename Base::node_ptr;
  using typename Base::leaf_ptr;
  using typename Base::key_type;
 protected:
  using Base::root_;
  using Base::dummy_;
  using Base::size_;
  std::array<hash_table_type, W+1> tb_;
  inline void _init() override {
    for (auto& t:tb_) t.clear();
    Base::_init();
  }
 public:
  XFastTrieBase() : Base() {}
  XFastTrieBase(const XFastTrieBase&) = default;
  XFastTrieBase& operator=(const XFastTrieBase&) = default;
  XFastTrieBase(XFastTrieBase&&) noexcept = default;
  XFastTrieBase& operator=(XFastTrieBase&& rhs) {
    Base::operator=(std::move((Base&&)rhs));
    tb_ = std::move(rhs.tb_);
  }
  template<typename InputIt>
  explicit XFastTrieBase(InputIt begin, InputIt end) {
    Base::_insert_init(begin, end);
  }
  using iterator = typename Base::iterator;
  using Base::end;
 protected:
  inline std::pair<int, node_ptr> _traverse(const key_type& key) const override {
    key_type x = key;
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
  inline leaf_ptr create_leaf_at(const key_type& x, const init_type& value) override {
    auto l = Base::create_leaf_at(x, value);
    _store_node(W, x, std::static_pointer_cast<Node>(l));
    return l;
  }
  inline leaf_ptr create_leaf_at(const key_type& x, init_type&& value) override {
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
      Base::_erase_from_leaf(x, std::static_pointer_cast<Leaf>(it->second));
      return true;
    } else {
      return false;
    }
  }
};

#include "traits/set_traits.hpp"

template<typename T, typename V, uint8_t W = sizeof(T)*8>
using XFastTrie = traits::MapTraits<XFastTrieBase<T, V, W>>;
template<typename T, uint8_t W = sizeof(T)*8>
using XFastTrieSet = traits::SetTraits<XFastTrieBase<T, void, W>>;
template<typename T, typename V, uint8_t W = sizeof(T)*8>
using XFastTrieMap = XFastTrie<T, V, W>;
