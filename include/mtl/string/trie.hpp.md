---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':warning:'
    path: include/mtl/string/ac_machine.hpp
    title: include/mtl/string/ac_machine.hpp
  - icon: ':warning:'
    path: include/mtl/string/dynamic_ac_machine.hpp
    title: include/mtl/string/dynamic_ac_machine.hpp
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
    \    }\n  }\n\n};\n"
  code: "#pragma once\n#include <utility>\n#include <vector>\n#include <map>\n#include\
    \ <memory>\n\ntemplate<typename T, typename KeyType, typename LabelType>\nclass\
    \ Trie {\n public:\n  using key_type = KeyType;\n  using mapped_type = T;\n  using\
    \ value_type = std::pair<key_type, mapped_type>;\n  using pointer = std::shared_ptr<value_type>;\n\
    \n  using label_type = LabelType;\n\n  /*\n   * node:\n   * - trans: (\\Sigma)\
    \ -> V\n   * - parent: V\n   */\n  struct TrieNode {\n    std::map<label_type,\
    \ int> trans;\n    pointer ptr = nullptr;\n  };\n  std::vector<TrieNode> nodes;\n\
    \  using node_iterator = typename decltype(nodes)::iterator;\n\n  Trie() : nodes(1)\
    \ {}\n\n  node_iterator find(const key_type& key) const {\n    auto nit = nodes.begin();\n\
    \    for (auto c : key) {\n      auto next = nit->trans.find(c);\n      if (next\
    \ == nit->trans.end())\n        return nodes.end();\n      nit = next;\n    }\n\
    \    if (nit->ptr) {\n      return nit;\n    } else {\n      return nodes.end();\n\
    \    }\n  }\n\n  node_iterator insert(const key_type& key, const mapped_type&\
    \ value) {\n    auto nit = nodes.begin();\n    for (auto c : key) {\n      auto\
    \ next = nit->trans.find(c);\n      if (next == nit->trans.end()) {\n        nit\
    \ = add_edge(nit-nodes.begin(), c);\n      } else {\n        nit = next;\n   \
    \   }\n    }\n    if (!nit->ptr) {\n      nit->ptr = std::make_shared<value_type>(key,\
    \ value);\n    }\n    return nit;\n  }\n\n  node_iterator add_edge(int node, label_type\
    \ c) {\n    auto it = nodes[node].trans.find(c);\n    if (it != nodes[node].trans.end())\
    \ {\n      return nodes[node].begin() + it->second;\n    } else {\n      nodes[node].trans[c]\
    \ = nodes.size();\n      nodes.emplace_back();\n      return std::prev(nodes.end());\n\
    \    }\n  }\n\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/string/trie.hpp
  requiredBy:
  - include/mtl/string/ac_machine.hpp
  - include/mtl/string/dynamic_ac_machine.hpp
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/string/trie.hpp
layout: document
redirect_from:
- /library/include/mtl/string/trie.hpp
- /library/include/mtl/string/trie.hpp.html
title: include/mtl/string/trie.hpp
---
