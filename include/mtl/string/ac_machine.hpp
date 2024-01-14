#pragma once
#include "trie.hpp"
#include <iterator>
#include <cassert>
#include <cstring>
#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <limits>

template<typename T, typename = std::void_t<>>
struct AcMachineNodeTraits {};

template<typename T>
struct AcMachineNodeTraits<T,
    std::void_t<typename T::id_type,
                typename T::index_type,
                typename T::char_type,
                typename T::edge_type>> {
  using id_type = typename T::id_type;
  using index_type = typename T::index_type;
  using char_type = typename T::char_type;
  using edge_type = typename T::edge_type;
};

struct _AcMachineNode {
  using id_type = int;
  using index_type = int;
  using char_type = char;
  using edge_type = std::unordered_map<char_type, index_type>;
  id_type id = -1;
  edge_type e;
  index_type fail = -1;
  void set_fail(index_type u, index_type f, _AcMachineNode& r) {
    fail = f;
  }
};

template<typename T, class Node>
class _AcMachine {
 public:
  using key_type = std::string;
  using mapped_type = T;
  using value_type = std::pair<key_type, mapped_type>;
  using node_traits = AcMachineNodeTraits<Node>;
  using id_type = typename node_traits::id_type;
  using index_type = typename node_traits::index_type;
  using char_type = typename node_traits::char_type;
 protected:
  std::vector<value_type> container_;
  std::vector<Node> nodes_;

 protected:
  index_type _go(index_type u, char_type c) const {
    auto& node = nodes_[u];
    auto it = node.e.find(c);
    if (it != node.e.end())
      return it->second;
    else
      return u == 0 ? 0 : -1;
  }
  void _insert_key(const std::string& key, const mapped_type& value) {
    if (nodes_.empty())
      nodes_.emplace_back();
    index_type u = 0;
    size_t i;
    for (i = 0; i < key.size(); i++) {
      auto it = nodes_[u].e.find(key[i]);
      if (it == nodes_[u].e.end())
        break;
      u = it->second;
    }
    if (i == key.size())
      return;
    for (; i < key.size(); i++) {
      auto next = nodes_.size();
      nodes_.emplace_back();
      nodes_[u].e[key[i]] = next;
      u = next;
    }
    nodes_[u].id = (id_type) container_.size();
    container_.emplace_back(key, value);
  }
 private:
  template<typename It>
  void _build_trie(It begin, It end) {
    for (auto it = begin; it != end; ++it) {
      _insert_key(*it);
    }
  }
  void _build_fail() {
    std::queue<index_type> idx;
    for (auto s : nodes_[0].e) {
      index_type next = s.second;
      idx.push(next);
      nodes_[next].set_fail(next, 0, nodes_[0]);
    }
    while (!idx.empty()) {
      auto id = idx.front(); idx.pop();
      for (auto s : nodes_[id].e) {
        char_type c = s.first;
        index_type next = s.second;
        idx.push(next);
        auto state = nodes_[id].fail;
        index_type target;
        while ((target = _go(state, c)) == -1)
          state = nodes_[state].fail;
        nodes_[next].set_fail(next, target, nodes_[target]);
      }
    }
  }
  template<typename It>
  void _build(It begin, It end) {
    using traits = std::iterator_traits<It>;
    static_assert(std::is_convertible<typename traits::value_type, std::string>::value, "");
    static_assert(std::is_base_of<std::forward_iterator_tag, typename traits::iterator_category>::value, "");

    _build_trie(begin, end);
    _build_fail();
  }

  std::vector<id_type> _output_id(index_type u) const {
    std::vector<id_type> ret;
    while (u != 0) {
      if (nodes_[u].id != -1)
        ret.push_back(nodes_[u].id);
      u = nodes_[u].fail;
    }
    return ret;
  }

  std::vector<value_type> _output(index_type u) const {
    std::vector<value_type> ret;
    while (u != 0) {
      auto id = nodes_[u].id;
      if (id != -1)
        ret.push_back(container_[id]);
      u = nodes_[u].fail;
    }
    return ret;
  }

 public:
  _AcMachine() = default;

  void insert(const key_type& key, const mapped_type& value) {
    _insert_key(key, value);
  }
  void build() {
    _build_fail();
  }

  std::vector<std::vector<value_type>> all_output() const {
    std::vector<std::vector<value_type>> res(nodes_.size());
    std::queue<index_type> idx;
    idx.push(0);
    while (!idx.empty()) {
      auto u = idx.front(); idx.pop();
      res[u] = _output(u);
      for (auto s : nodes_[u].e) {
        idx.push(s.second);
      }
    }
    return res;
  }

  struct key_iterator {
   public:
    using value_type = _AcMachine::value_type;
    using reference = const value_type&;
    using pointer = const value_type*;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = long long;
    const _AcMachine* ac_;
    index_type u_;
    void _forward_until_data() {
      while (u_ != 0 and ac_->nodes_[u_].id == -1) {
        u_ = ac_->nodes_[u_].fail;
      }
    }
    key_iterator& to_exact() {
      _forward_until_data();
      return *this;
    }
    key_iterator() = default;
    explicit key_iterator(const _AcMachine* ac, index_type u) : ac_(ac), u_(u) {}
    reference operator*() const { return ac_->container_[ac_->nodes_[u_].id]; }
    pointer operator->() const { return &ac_->container_[ac_->nodes_[u_].id]; }
    bool operator==(const key_iterator& r) const { return u_ == r.u_; }
    bool operator!=(const key_iterator& r) const { return !(*this == r); }
    key_iterator& operator++() {
      u_ = ac_->nodes_[u_].fail;
      to_exact();
      return *this;
    }
    key_iterator operator++(int) {
      key_iterator ret = *this;
      ++*this;
      return ret;
    }
    key_iterator& push(char_type c) {
      index_type target;
      while ((target = ac_->_go(u_, c)) == -1)
        u_ = ac_->nodes_[u_].fail;
      u_ = target;
      return *this;
    }
    key_iterator pushed(char_type c) const {
      return key_iterator(*this).push(c);
    }
  };
  key_iterator key_begin() const {
    return key_iterator(this, 0);
  }
  key_iterator key_end() const {
    return key_iterator(this, 0);
  }
  std::vector<std::pair<size_t, key_iterator>> find_all(const std::string& text) const {
    std::vector<std::pair<size_t, key_iterator>> ret;
    auto it = key_begin();
    for (size_t i = 0; i < text.size(); i++) {
      it.push(text[i]);
      auto c = it;
      c.to_exact();
      if (c != key_end()) {
        ret.emplace_back(i+1, c);
      }
    }
    return ret;
  }
  std::pair<std::pair<size_t, key_iterator>, bool> find(const std::string& text) const {
    auto it = key_begin();
    for (size_t i = 0; i < text.size(); i++) {
      it.push(text[i]);
      auto c = it;
      c.to_exact();
      if (c != key_end()) {
        return {{i+1, c}, true};
      }
    }
    return {{}, false};
  }
  key_iterator find_suffix(const std::string& text) const {
    auto it = key_begin();
    for (auto c : text) {
      it.push(c);
    }
    return it.to_exact();
  }
};
template<typename T>
using AcMachine = _AcMachine<T, _AcMachineNode>;