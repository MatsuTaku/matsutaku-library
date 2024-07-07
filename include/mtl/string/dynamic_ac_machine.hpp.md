---
data:
  _extendedDependsOn:
  - icon: ':warning:'
    path: include/mtl/string/ac_machine.hpp
    title: include/mtl/string/ac_machine.hpp
  - icon: ':warning:'
    path: include/mtl/string/trie.hpp
    title: include/mtl/string/trie.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/string/trie.hpp\"\n#include <utility>\n#include\
    \ <vector>\n#include <map>\n#include <memory>\n\ntemplate<typename T, typename\
    \ KeyType, typename LabelType>\nclass Trie {\n public:\n  using key_type = KeyType;\n\
    \  using mapped_type = T;\n  using value_type = std::pair<key_type, mapped_type>;\n\
    \  using pointer = std::shared_ptr<value_type>;\n\n  using label_type = LabelType;\n\
    \n  /*\n   * node:\n   * - trans: (\\Sigma) -> V\n   * - parent: V\n   */\n  struct\
    \ TrieNode {\n    std::map<label_type, int> trans;\n    pointer ptr = nullptr;\n\
    \  };\n  std::vector<TrieNode> nodes;\n  using node_iterator = typename decltype(nodes)::iterator;\n\
    \n  Trie() : nodes(1) {}\n\n  node_iterator find(const key_type& key) const {\n\
    \    auto nit = nodes.begin();\n    for (auto c : key) {\n      auto next = nit->trans.find(c);\n\
    \      if (next == nit->trans.end())\n        return nodes.end();\n      nit =\
    \ next;\n    }\n    if (nit->ptr) {\n      return nit;\n    } else {\n      return\
    \ nodes.end();\n    }\n  }\n\n  node_iterator insert(const key_type& key, const\
    \ mapped_type& value) {\n    auto nit = nodes.begin();\n    for (auto c : key)\
    \ {\n      auto next = nit->trans.find(c);\n      if (next == nit->trans.end())\
    \ {\n        nit = add_edge(nit-nodes.begin(), c);\n      } else {\n        nit\
    \ = next;\n      }\n    }\n    if (!nit->ptr) {\n      nit->ptr = std::make_shared<value_type>(key,\
    \ value);\n    }\n    return nit;\n  }\n\n  node_iterator add_edge(int node, label_type\
    \ c) {\n    auto it = nodes[node].trans.find(c);\n    if (it != nodes[node].trans.end())\
    \ {\n      return nodes[node].begin() + it->second;\n    } else {\n      nodes[node].trans[c]\
    \ = nodes.size();\n      nodes.emplace_back();\n      return std::prev(nodes.end());\n\
    \    }\n  }\n\n};\n#line 3 \"include/mtl/string/ac_machine.hpp\"\n#include <iterator>\r\
    \n#include <cassert>\r\n#include <cstring>\r\n#line 7 \"include/mtl/string/ac_machine.hpp\"\
    \n#include <queue>\r\n#include <unordered_map>\r\n#line 10 \"include/mtl/string/ac_machine.hpp\"\
    \n#include <algorithm>\r\n#include <iostream>\r\n#include <typeinfo>\r\n#include\
    \ <limits>\r\n\r\ntemplate<typename T, typename = std::void_t<>>\r\nstruct AcMachineNodeTraits\
    \ {};\r\n\r\ntemplate<typename T>\r\nstruct AcMachineNodeTraits<T,\r\n    std::void_t<typename\
    \ T::id_type,\r\n                typename T::index_type,\r\n                typename\
    \ T::char_type,\r\n                typename T::edge_type>> {\r\n  using id_type\
    \ = typename T::id_type;\r\n  using index_type = typename T::index_type;\r\n \
    \ using char_type = typename T::char_type;\r\n  using edge_type = typename T::edge_type;\r\
    \n};\r\n\r\nstruct _AcMachineNode {\r\n  using id_type = int;\r\n  using index_type\
    \ = int;\r\n  using char_type = char;\r\n  using edge_type = std::unordered_map<char_type,\
    \ index_type>;\r\n  id_type id = -1;\r\n  edge_type e;\r\n  index_type fail =\
    \ -1;\r\n  void set_fail(index_type u, index_type f, _AcMachineNode& r) {\r\n\
    \    fail = f;\r\n  }\r\n};\r\n\r\ntemplate<typename T, class Node>\r\nclass _AcMachine\
    \ {\r\n public:\r\n  using key_type = std::string;\r\n  using mapped_type = T;\r\
    \n  using value_type = std::pair<key_type, mapped_type>;\r\n  using node_traits\
    \ = AcMachineNodeTraits<Node>;\r\n  using id_type = typename node_traits::id_type;\r\
    \n  using index_type = typename node_traits::index_type;\r\n  using char_type\
    \ = typename node_traits::char_type;\r\n protected:\r\n  std::vector<value_type>\
    \ container_;\r\n  std::vector<Node> nodes_;\r\n\r\n protected:\r\n  index_type\
    \ _go(index_type u, char_type c) const {\r\n    auto& node = nodes_[u];\r\n  \
    \  auto it = node.e.find(c);\r\n    if (it != node.e.end())\r\n      return it->second;\r\
    \n    else\r\n      return u == 0 ? 0 : -1;\r\n  }\r\n  void _insert_key(const\
    \ std::string& key, const mapped_type& value) {\r\n    if (nodes_.empty())\r\n\
    \      nodes_.emplace_back();\r\n    index_type u = 0;\r\n    size_t i;\r\n  \
    \  for (i = 0; i < key.size(); i++) {\r\n      auto it = nodes_[u].e.find(key[i]);\r\
    \n      if (it == nodes_[u].e.end())\r\n        break;\r\n      u = it->second;\r\
    \n    }\r\n    if (i == key.size())\r\n      return;\r\n    for (; i < key.size();\
    \ i++) {\r\n      auto next = nodes_.size();\r\n      nodes_.emplace_back();\r\
    \n      nodes_[u].e[key[i]] = next;\r\n      u = next;\r\n    }\r\n    nodes_[u].id\
    \ = (id_type) container_.size();\r\n    container_.emplace_back(key, value);\r\
    \n  }\r\n private:\r\n  template<typename It>\r\n  void _build_trie(It begin,\
    \ It end) {\r\n    for (auto it = begin; it != end; ++it) {\r\n      _insert_key(*it);\r\
    \n    }\r\n  }\r\n  void _build_fail() {\r\n    std::queue<index_type> idx;\r\n\
    \    for (auto s : nodes_[0].e) {\r\n      index_type next = s.second;\r\n   \
    \   idx.push(next);\r\n      nodes_[next].set_fail(next, 0, nodes_[0]);\r\n  \
    \  }\r\n    while (!idx.empty()) {\r\n      auto id = idx.front(); idx.pop();\r\
    \n      for (auto s : nodes_[id].e) {\r\n        char_type c = s.first;\r\n  \
    \      index_type next = s.second;\r\n        idx.push(next);\r\n        auto\
    \ state = nodes_[id].fail;\r\n        index_type target;\r\n        while ((target\
    \ = _go(state, c)) == -1)\r\n          state = nodes_[state].fail;\r\n       \
    \ nodes_[next].set_fail(next, target, nodes_[target]);\r\n      }\r\n    }\r\n\
    \  }\r\n  template<typename It>\r\n  void _build(It begin, It end) {\r\n    using\
    \ traits = std::iterator_traits<It>;\r\n    static_assert(std::is_convertible<typename\
    \ traits::value_type, std::string>::value, \"\");\r\n    static_assert(std::is_base_of<std::forward_iterator_tag,\
    \ typename traits::iterator_category>::value, \"\");\r\n\r\n    _build_trie(begin,\
    \ end);\r\n    _build_fail();\r\n  }\r\n\r\n  std::vector<id_type> _output_id(index_type\
    \ u) const {\r\n    std::vector<id_type> ret;\r\n    while (u != 0) {\r\n    \
    \  if (nodes_[u].id != -1)\r\n        ret.push_back(nodes_[u].id);\r\n      u\
    \ = nodes_[u].fail;\r\n    }\r\n    return ret;\r\n  }\r\n\r\n  std::vector<value_type>\
    \ _output(index_type u) const {\r\n    std::vector<value_type> ret;\r\n    while\
    \ (u != 0) {\r\n      auto id = nodes_[u].id;\r\n      if (id != -1)\r\n     \
    \   ret.push_back(container_[id]);\r\n      u = nodes_[u].fail;\r\n    }\r\n \
    \   return ret;\r\n  }\r\n\r\n public:\r\n  _AcMachine() = default;\r\n\r\n  void\
    \ insert(const key_type& key, const mapped_type& value) {\r\n    _insert_key(key,\
    \ value);\r\n  }\r\n  void build() {\r\n    _build_fail();\r\n  }\r\n\r\n  std::vector<std::vector<value_type>>\
    \ all_output() const {\r\n    std::vector<std::vector<value_type>> res(nodes_.size());\r\
    \n    std::queue<index_type> idx;\r\n    idx.push(0);\r\n    while (!idx.empty())\
    \ {\r\n      auto u = idx.front(); idx.pop();\r\n      res[u] = _output(u);\r\n\
    \      for (auto s : nodes_[u].e) {\r\n        idx.push(s.second);\r\n      }\r\
    \n    }\r\n    return res;\r\n  }\r\n\r\n  struct key_iterator {\r\n   public:\r\
    \n    using value_type = _AcMachine::value_type;\r\n    using reference = const\
    \ value_type&;\r\n    using pointer = const value_type*;\r\n    using iterator_category\
    \ = std::forward_iterator_tag;\r\n    using difference_type = long long;\r\n \
    \   const _AcMachine* ac_;\r\n    index_type u_;\r\n    void _forward_until_data()\
    \ {\r\n      while (u_ != 0 and ac_->nodes_[u_].id == -1) {\r\n        u_ = ac_->nodes_[u_].fail;\r\
    \n      }\r\n    }\r\n    key_iterator& to_exact() {\r\n      _forward_until_data();\r\
    \n      return *this;\r\n    }\r\n    key_iterator() = default;\r\n    explicit\
    \ key_iterator(const _AcMachine* ac, index_type u) : ac_(ac), u_(u) {}\r\n   \
    \ reference operator*() const { return ac_->container_[ac_->nodes_[u_].id]; }\r\
    \n    pointer operator->() const { return &ac_->container_[ac_->nodes_[u_].id];\
    \ }\r\n    bool operator==(const key_iterator& r) const { return u_ == r.u_; }\r\
    \n    bool operator!=(const key_iterator& r) const { return !(*this == r); }\r\
    \n    key_iterator& operator++() {\r\n      u_ = ac_->nodes_[u_].fail;\r\n   \
    \   to_exact();\r\n      return *this;\r\n    }\r\n    key_iterator operator++(int)\
    \ {\r\n      key_iterator ret = *this;\r\n      ++*this;\r\n      return ret;\r\
    \n    }\r\n    key_iterator& push(char_type c) {\r\n      index_type target;\r\
    \n      while ((target = ac_->_go(u_, c)) == -1)\r\n        u_ = ac_->nodes_[u_].fail;\r\
    \n      u_ = target;\r\n      return *this;\r\n    }\r\n    key_iterator pushed(char_type\
    \ c) const {\r\n      return key_iterator(*this).push(c);\r\n    }\r\n  };\r\n\
    \  key_iterator key_begin() const {\r\n    return key_iterator(this, 0);\r\n \
    \ }\r\n  key_iterator key_end() const {\r\n    return key_iterator(this, 0);\r\
    \n  }\r\n  std::vector<std::pair<size_t, key_iterator>> find_all(const std::string&\
    \ text) const {\r\n    std::vector<std::pair<size_t, key_iterator>> ret;\r\n \
    \   auto it = key_begin();\r\n    for (size_t i = 0; i < text.size(); i++) {\r\
    \n      it.push(text[i]);\r\n      auto c = it;\r\n      c.to_exact();\r\n   \
    \   if (c != key_end()) {\r\n        ret.emplace_back(i+1, c);\r\n      }\r\n\
    \    }\r\n    return ret;\r\n  }\r\n  std::pair<std::pair<size_t, key_iterator>,\
    \ bool> find(const std::string& text) const {\r\n    auto it = key_begin();\r\n\
    \    for (size_t i = 0; i < text.size(); i++) {\r\n      it.push(text[i]);\r\n\
    \      auto c = it;\r\n      c.to_exact();\r\n      if (c != key_end()) {\r\n\
    \        return {{i+1, c}, true};\r\n      }\r\n    }\r\n    return {{}, false};\r\
    \n  }\r\n  key_iterator find_suffix(const std::string& text) const {\r\n    auto\
    \ it = key_begin();\r\n    for (auto c : text) {\r\n      it.push(c);\r\n    }\r\
    \n    return it.to_exact();\r\n  }\r\n};\r\ntemplate<typename T>\r\nusing AcMachine\
    \ = _AcMachine<T, _AcMachineNode>;\n#line 4 \"include/mtl/string/dynamic_ac_machine.hpp\"\
    \n#include <unordered_set>\r\n\r\nstruct _DynamicAcMachineNode : public _AcMachineNode\
    \ {\r\n  using link_type = std::unordered_set<_AcMachineNode::index_type>;\r\n\
    \  link_type ifail;\r\n  void set_fail(index_type u, index_type f, _DynamicAcMachineNode&\
    \ r) {\r\n    fail = f;\r\n    r.ifail.insert(u);\r\n  }\r\n};\r\n\r\ntemplate<typename\
    \ T>\r\nclass DynamicAcMachine : public _AcMachine<T, _DynamicAcMachineNode> {\r\
    \n  using super = _AcMachine<T, _DynamicAcMachineNode>;\r\n public:\r\n  using\
    \ key_type = typename super::key_type;\r\n  using mapped_type = typename super::mapped_type;\r\
    \n  using value_type = typename super::value_type;\r\n  using id_type = typename\
    \ super::id_type;\r\n  using index_type = typename super::index_type;\r\n  using\
    \ char_type = typename super::char_type;\r\n\r\n public:\r\n  DynamicAcMachine()\
    \ : _AcMachine<T, _DynamicAcMachineNode>() {}\r\n  void insert(const std::string&\
    \ key, const mapped_type& value) {\r\n    if (super::nodes_.empty()) {\r\n   \
    \   super::nodes_.emplace_back();\r\n    }\r\n    index_type u = 0;\r\n    size_t\
    \ d;\r\n    std::vector<index_type> s(key.size());\r\n    // Traverse on the trie.\r\
    \n    size_t i;\r\n    for (i = 0; i < key.size(); i++) {\r\n      char_type c\
    \ = key[i];\r\n      auto it = super::nodes_[u].e.find(c);\r\n      if (it ==\
    \ super::nodes_[u].e.end())\r\n        break;\r\n      u = it->second;\r\n   \
    \   s[i] = u;\r\n    }\r\n    if (i == key.size())\r\n      return;\r\n    //\
    \ Add new nodes.\r\n    d = i;\r\n    for (; i < key.size(); i++) {\r\n      char_type\
    \ c = key[i];\r\n      index_type next = (index_type) super::nodes_.size();\r\n\
    \      super::nodes_.emplace_back();\r\n      super::nodes_[u].e[c] = next;\r\n\
    \      u = next;\r\n      s[i] = u;\r\n    }\r\n    super::nodes_[u].id = (id_type)\
    \ super::container_.size();\r\n    super::container_.emplace_back(key, value);\r\
    \n    // Set fail of new nodes.\r\n    for (size_t k = d; k < key.size(); k++)\
    \ {\r\n      if (k == 0) {\r\n        super::nodes_[s[k]].set_fail(s[k], 0, super::nodes_[0]);\r\
    \n      } else {\r\n        auto state = super::nodes_[s[k-1]].fail;\r\n     \
    \   index_type next;\r\n        while ((next = super::_go(state, key[k])) == -1)\r\
    \n          state = super::nodes_[state].fail;\r\n        super::nodes_[s[k]].set_fail(s[k],\
    \ next, super::nodes_[next]);\r\n      }\r\n    }\r\n    // Update fail of nodes\
    \ that 'failing' to new nodes.\r\n    std::queue<std::pair<index_type, size_t>>\
    \ qs;\r\n    for (auto v : super::nodes_[d == 0 ? 0 : s[d-1]].ifail)\r\n     \
    \ qs.emplace(v, d);\r\n    while (!qs.empty()) {\r\n      auto q = qs.front();\
    \ qs.pop();\r\n      index_type v = q.first;\r\n      size_t k = q.second;\r\n\
    \      index_type next;\r\n      while (k < key.size() and (next = super::_go(v,\
    \ key[k])) != -1) {\r\n        if (super::nodes_[next].fail != -1) {\r\n     \
    \     auto pf = super::nodes_[super::nodes_[next].fail];\r\n          pf.ifail.erase(next);\r\
    \n        }\r\n        super::nodes_[next].set_fail(next, s[k], super::nodes_[s[k]]);\r\
    \n        v = next;\r\n        ++k;\r\n      }\r\n      if (k < key.size()) {\r\
    \n        for (auto x : super::nodes_[v].ifail)\r\n          qs.emplace(x, k);\r\
    \n      }\r\n    }\r\n  }\r\n\r\n  void erase(const std::string& key) {\r\n  \
    \  index_type u = 0;\r\n    std::vector<index_type> s(key.size());\r\n    for\
    \ (size_t i = 0; i < key.size(); i++) {\r\n      char_type c = key[i];\r\n   \
    \   auto it = super::nodes_[u].e.find(c);\r\n      if (it == super::nodes_[u].e.end())\
    \ {\r\n        // key is not contained on the trie.\r\n        return;\r\n   \
    \   }\r\n      u = it->second;\r\n      s[i] = u;\r\n    }\r\n\r\n    if (super::nodes_[s[key.size()-1]].id\
    \ == -1) {\r\n      // key is not contained on the trie.\r\n      return;\r\n\
    \    }\r\n    // Pop removed nodes on fail links.\r\n    super::nodes_[s[key.size()-1]].id\
    \ = -1;\r\n    if (super::nodes_[s[key.size()-1]].e.empty()) {\r\n      size_t\
    \ d = key.size()-1;\r\n      while (d > 0 and super::nodes_[s[d-1]].id == -1 and\
    \ super::nodes_[s[d-1]].e.size() == 1)\r\n        d--;\r\n      for (size_t k\
    \ = d; k < key.size(); k++) {\r\n        index_type f = super::nodes_[s[k]].fail;\r\
    \n        super::nodes_[f].ifail.erase(s[k]);\r\n        for (auto v : super::nodes_[s[k]].ifail)\
    \ {\r\n          auto target = super::nodes_[s[k]].fail;\r\n          super::nodes_[target].ifail.erase(s[k]);\r\
    \n          super::nodes_[v].set_fail(v, target, super::nodes_[target]);\r\n \
    \       }\r\n      }\r\n      super::nodes_[d == 0 ? 0 : s[d-1]].e.erase(key[d]);\r\
    \n    }\r\n  }\r\n};\r\n"
  code: "#pragma once\r\n#include \"ac_machine.hpp\"\r\n#include <queue>\r\n#include\
    \ <unordered_set>\r\n\r\nstruct _DynamicAcMachineNode : public _AcMachineNode\
    \ {\r\n  using link_type = std::unordered_set<_AcMachineNode::index_type>;\r\n\
    \  link_type ifail;\r\n  void set_fail(index_type u, index_type f, _DynamicAcMachineNode&\
    \ r) {\r\n    fail = f;\r\n    r.ifail.insert(u);\r\n  }\r\n};\r\n\r\ntemplate<typename\
    \ T>\r\nclass DynamicAcMachine : public _AcMachine<T, _DynamicAcMachineNode> {\r\
    \n  using super = _AcMachine<T, _DynamicAcMachineNode>;\r\n public:\r\n  using\
    \ key_type = typename super::key_type;\r\n  using mapped_type = typename super::mapped_type;\r\
    \n  using value_type = typename super::value_type;\r\n  using id_type = typename\
    \ super::id_type;\r\n  using index_type = typename super::index_type;\r\n  using\
    \ char_type = typename super::char_type;\r\n\r\n public:\r\n  DynamicAcMachine()\
    \ : _AcMachine<T, _DynamicAcMachineNode>() {}\r\n  void insert(const std::string&\
    \ key, const mapped_type& value) {\r\n    if (super::nodes_.empty()) {\r\n   \
    \   super::nodes_.emplace_back();\r\n    }\r\n    index_type u = 0;\r\n    size_t\
    \ d;\r\n    std::vector<index_type> s(key.size());\r\n    // Traverse on the trie.\r\
    \n    size_t i;\r\n    for (i = 0; i < key.size(); i++) {\r\n      char_type c\
    \ = key[i];\r\n      auto it = super::nodes_[u].e.find(c);\r\n      if (it ==\
    \ super::nodes_[u].e.end())\r\n        break;\r\n      u = it->second;\r\n   \
    \   s[i] = u;\r\n    }\r\n    if (i == key.size())\r\n      return;\r\n    //\
    \ Add new nodes.\r\n    d = i;\r\n    for (; i < key.size(); i++) {\r\n      char_type\
    \ c = key[i];\r\n      index_type next = (index_type) super::nodes_.size();\r\n\
    \      super::nodes_.emplace_back();\r\n      super::nodes_[u].e[c] = next;\r\n\
    \      u = next;\r\n      s[i] = u;\r\n    }\r\n    super::nodes_[u].id = (id_type)\
    \ super::container_.size();\r\n    super::container_.emplace_back(key, value);\r\
    \n    // Set fail of new nodes.\r\n    for (size_t k = d; k < key.size(); k++)\
    \ {\r\n      if (k == 0) {\r\n        super::nodes_[s[k]].set_fail(s[k], 0, super::nodes_[0]);\r\
    \n      } else {\r\n        auto state = super::nodes_[s[k-1]].fail;\r\n     \
    \   index_type next;\r\n        while ((next = super::_go(state, key[k])) == -1)\r\
    \n          state = super::nodes_[state].fail;\r\n        super::nodes_[s[k]].set_fail(s[k],\
    \ next, super::nodes_[next]);\r\n      }\r\n    }\r\n    // Update fail of nodes\
    \ that 'failing' to new nodes.\r\n    std::queue<std::pair<index_type, size_t>>\
    \ qs;\r\n    for (auto v : super::nodes_[d == 0 ? 0 : s[d-1]].ifail)\r\n     \
    \ qs.emplace(v, d);\r\n    while (!qs.empty()) {\r\n      auto q = qs.front();\
    \ qs.pop();\r\n      index_type v = q.first;\r\n      size_t k = q.second;\r\n\
    \      index_type next;\r\n      while (k < key.size() and (next = super::_go(v,\
    \ key[k])) != -1) {\r\n        if (super::nodes_[next].fail != -1) {\r\n     \
    \     auto pf = super::nodes_[super::nodes_[next].fail];\r\n          pf.ifail.erase(next);\r\
    \n        }\r\n        super::nodes_[next].set_fail(next, s[k], super::nodes_[s[k]]);\r\
    \n        v = next;\r\n        ++k;\r\n      }\r\n      if (k < key.size()) {\r\
    \n        for (auto x : super::nodes_[v].ifail)\r\n          qs.emplace(x, k);\r\
    \n      }\r\n    }\r\n  }\r\n\r\n  void erase(const std::string& key) {\r\n  \
    \  index_type u = 0;\r\n    std::vector<index_type> s(key.size());\r\n    for\
    \ (size_t i = 0; i < key.size(); i++) {\r\n      char_type c = key[i];\r\n   \
    \   auto it = super::nodes_[u].e.find(c);\r\n      if (it == super::nodes_[u].e.end())\
    \ {\r\n        // key is not contained on the trie.\r\n        return;\r\n   \
    \   }\r\n      u = it->second;\r\n      s[i] = u;\r\n    }\r\n\r\n    if (super::nodes_[s[key.size()-1]].id\
    \ == -1) {\r\n      // key is not contained on the trie.\r\n      return;\r\n\
    \    }\r\n    // Pop removed nodes on fail links.\r\n    super::nodes_[s[key.size()-1]].id\
    \ = -1;\r\n    if (super::nodes_[s[key.size()-1]].e.empty()) {\r\n      size_t\
    \ d = key.size()-1;\r\n      while (d > 0 and super::nodes_[s[d-1]].id == -1 and\
    \ super::nodes_[s[d-1]].e.size() == 1)\r\n        d--;\r\n      for (size_t k\
    \ = d; k < key.size(); k++) {\r\n        index_type f = super::nodes_[s[k]].fail;\r\
    \n        super::nodes_[f].ifail.erase(s[k]);\r\n        for (auto v : super::nodes_[s[k]].ifail)\
    \ {\r\n          auto target = super::nodes_[s[k]].fail;\r\n          super::nodes_[target].ifail.erase(s[k]);\r\
    \n          super::nodes_[v].set_fail(v, target, super::nodes_[target]);\r\n \
    \       }\r\n      }\r\n      super::nodes_[d == 0 ? 0 : s[d-1]].e.erase(key[d]);\r\
    \n    }\r\n  }\r\n};\r\n"
  dependsOn:
  - include/mtl/string/ac_machine.hpp
  - include/mtl/string/trie.hpp
  isVerificationFile: false
  path: include/mtl/string/dynamic_ac_machine.hpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/string/dynamic_ac_machine.hpp
layout: document
redirect_from:
- /library/include/mtl/string/dynamic_ac_machine.hpp
- /library/include/mtl/string/dynamic_ac_machine.hpp.html
title: include/mtl/string/dynamic_ac_machine.hpp
---
