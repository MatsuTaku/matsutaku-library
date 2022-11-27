#pragma once
#include <utility>
#include <vector>
#include <map>
#include <memory>

template<typename T, typename KeyType, typename LabelType>
class Trie {
 public:
  using key_type = KeyType;
  using mapped_type = T;
  using value_type = std::pair<key_type, mapped_type>;
  using pointer = std::shared_ptr<value_type>;

  using label_type = LabelType;

  /*
   * node:
   * - trans: (\Sigma) -> V
   * - parent: V
   */
  struct TrieNode {
    std::map<label_type, int> trans;
    pointer ptr = nullptr;
  };
  std::vector<TrieNode> nodes;
  using node_iterator = typename decltype(nodes)::iterator;

  Trie() : nodes(1) {}

  node_iterator find(const key_type& key) const {
    auto nit = nodes.begin();
    for (auto c : key) {
      auto next = nit->trans.find(c);
      if (next == nit->trans.end())
        return nodes.end();
      nit = next;
    }
    if (nit->ptr) {
      return nit;
    } else {
      return nodes.end();
    }
  }

  node_iterator insert(const key_type& key, const mapped_type& value) {
    auto nit = nodes.begin();
    for (auto c : key) {
      auto next = nit->trans.find(c);
      if (next == nit->trans.end()) {
        nit = add_edge(nit-nodes.begin(), c);
      } else {
        nit = next;
      }
    }
    if (!nit->ptr) {
      nit->ptr = std::make_shared<value_type>(key, value);
    }
    return nit;
  }

  node_iterator add_edge(int node, label_type c) {
    auto it = nodes[node].trans.find(c);
    if (it != nodes[node].trans.end()) {
      return nodes[node].begin() + it->second;
    } else {
      nodes[node].trans[c] = nodes.size();
      nodes.emplace_back();
      return std::prev(nodes.end());
    }
  }

};
