#pragma once
#include <memory>
#include <array>

template<typename T, unsigned M=32>
class PersistentArray {
 private:
  struct Node;
  using node_ptr = std::shared_ptr<Node>;
  struct Node {
    T v;
    std::array<node_ptr, M> ch;
    Node() = default;
    Node(T v) : v(v) {}
  };
  node_ptr root_;
  PersistentArray(node_ptr ptr) : root_(ptr) {}
 public:
  PersistentArray() = default;
  bool empty() const { return root_ == nullptr; }
  T get(size_t i) const {
    auto u = root_;
    while (i > 0) {
      i--;
      u = u->ch[i % M];
      i /= M;
    }
    return u->v;
  }
 private:
  node_ptr set_rec(node_ptr u, size_t id, const T& v) const {
    auto new_node = u ? std::make_shared<Node>(*u) : std::make_shared<Node>();
    if (id == 0) {
      new_node->v = v;
    } else {
      --id;
      new_node->ch[id % M] = set_rec(new_node->ch[id % M], id / M, v);
    }
    return new_node;
  }
 public:
  [[nodiscard]] PersistentArray set(size_t i, const T& v) const {
    return PersistentArray(set_rec(root_, i, v));
  }
};
