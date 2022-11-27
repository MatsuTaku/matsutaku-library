#pragma once
#include "string/ac_machine.hpp"
#include <queue>
#include <unordered_set>

struct _DynamicAcMachineNode : public _AcMachineNode {
  using link_type = std::unordered_set<_AcMachineNode::index_type>;
  link_type ifail;
  void set_fail(index_type u, index_type f, _DynamicAcMachineNode& r) {
    fail = f;
    r.ifail.insert(u);
  }
};

template<typename T>
class DynamicAcMachine : public _AcMachine<T, _DynamicAcMachineNode> {
  using super = _AcMachine<T, _DynamicAcMachineNode>;
 public:
  using key_type = typename super::key_type;
  using mapped_type = typename super::mapped_type;
  using value_type = typename super::value_type;
  using id_type = typename super::id_type;
  using index_type = typename super::index_type;
  using char_type = typename super::char_type;

 public:
  DynamicAcMachine() : _AcMachine<T, _DynamicAcMachineNode>() {}
  void insert(const std::string& key, const mapped_type& value) {
    if (super::nodes_.empty()) {
      super::nodes_.emplace_back();
    }
    index_type u = 0;
    size_t d;
    std::vector<index_type> s(key.size());
    // Traverse on the trie.
    size_t i;
    for (i = 0; i < key.size(); i++) {
      char_type c = key[i];
      auto it = super::nodes_[u].e.find(c);
      if (it == super::nodes_[u].e.end())
        break;
      u = it->second;
      s[i] = u;
    }
    if (i == key.size())
      return;
    // Add new nodes.
    d = i;
    for (; i < key.size(); i++) {
      char_type c = key[i];
      index_type next = (index_type) super::nodes_.size();
      super::nodes_.emplace_back();
      super::nodes_[u].e[c] = next;
      u = next;
      s[i] = u;
    }
    super::nodes_[u].id = (id_type) super::container_.size();
    super::container_.emplace_back(key, value);
    // Set fail of new nodes.
    for (size_t k = d; k < key.size(); k++) {
      if (k == 0) {
        super::nodes_[s[k]].set_fail(s[k], 0, super::nodes_[0]);
      } else {
        auto state = super::nodes_[s[k-1]].fail;
        index_type next;
        while ((next = super::_go(state, key[k])) == -1)
          state = super::nodes_[state].fail;
        super::nodes_[s[k]].set_fail(s[k], next, super::nodes_[next]);
      }
    }
    // Update fail of nodes that 'failing' to new nodes.
    std::queue<std::pair<index_type, size_t>> qs;
    for (auto v : super::nodes_[d == 0 ? 0 : s[d-1]].ifail)
      qs.emplace(v, d);
    while (!qs.empty()) {
      auto q = qs.front(); qs.pop();
      index_type v = q.first;
      size_t k = q.second;
      index_type next;
      while (k < key.size() and (next = super::_go(v, key[k])) != -1) {
        if (super::nodes_[next].fail != -1) {
          auto pf = super::nodes_[super::nodes_[next].fail];
          pf.ifail.erase(next);
        }
        super::nodes_[next].set_fail(next, s[k], super::nodes_[s[k]]);
        v = next;
        ++k;
      }
      if (k < key.size()) {
        for (auto x : super::nodes_[v].ifail)
          qs.emplace(x, k);
      }
    }
  }

  void erase(const std::string& key) {
    index_type u = 0;
    std::vector<index_type> s(key.size());
    for (size_t i = 0; i < key.size(); i++) {
      char_type c = key[i];
      auto it = super::nodes_[u].e.find(c);
      if (it == super::nodes_[u].e.end()) {
        // key is not contained on the trie.
        return;
      }
      u = it->second;
      s[i] = u;
    }

    if (super::nodes_[s[key.size()-1]].id == -1) {
      // key is not contained on the trie.
      return;
    }
    // Pop removed nodes on fail links.
    super::nodes_[s[key.size()-1]].id = -1;
    if (super::nodes_[s[key.size()-1]].e.empty()) {
      size_t d = key.size()-1;
      while (d > 0 and super::nodes_[s[d-1]].id == -1 and super::nodes_[s[d-1]].e.size() == 1)
        d--;
      for (size_t k = d; k < key.size(); k++) {
        index_type f = super::nodes_[s[k]].fail;
        super::nodes_[f].ifail.erase(s[k]);
        for (auto v : super::nodes_[s[k]].ifail) {
          auto target = super::nodes_[s[k]].fail;
          super::nodes_[target].ifail.erase(s[k]);
          super::nodes_[v].set_fail(v, target, super::nodes_[target]);
        }
      }
      super::nodes_[d == 0 ? 0 : s[d-1]].e.erase(key[d]);
    }
  }
};