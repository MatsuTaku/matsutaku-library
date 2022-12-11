#pragma once
#include <memory>
#include <utility>
#include <array>

template<typename T, unsigned Degree=4>
class PersistentArray {
  static constexpr unsigned M = Degree;
 private:
  struct Node;
  using node_ptr = std::shared_ptr<Node>;
  struct Node {
    T v;
    std::array<node_ptr, M> ch;
    Node() = default;
    explicit Node(const T& v) : v(v) {}
    explicit Node(T&& v) : v(std::move(v)) {}
  };
  node_ptr root_;
  T init_v_;
  explicit PersistentArray(node_ptr ptr, T init_v)
    : root_(ptr), init_v_(init_v) {}

 public:
  explicit PersistentArray(T init_v = T())
    : root_(nullptr), init_v_(init_v) {}

  bool empty() const { return root_ == nullptr; }
  T get(size_t i) const {
    auto u = root_;
    while (u and i > 0) {
      i--;
      u = u->ch[i % M];
      i /= M;
    }
    return u ? u->v : init_v_;
  }

 private:
  template<typename V>
  node_ptr set_rec(node_ptr u, size_t id, V&& v) const {
    if (id == 0) {
      if (u) {
        auto new_node = std::make_shared<Node>(*u);
        new_node->v = std::forward<V>(v);
        return new_node;
      } else {
        return std::make_shared<Node>(std::forward<V>(v));
      }
    } else {
      auto new_node = u ? std::make_shared<Node>(*u) : std::make_shared<Node>(init_v_);
      --id;
      new_node->ch[id % M] = set_rec(new_node->ch[id % M], id / M, std::forward<V>(v));
      return new_node;
    }
  }
 public:
  template<typename V>
  [[nodiscard]] PersistentArray set(size_t i, V&& v) const {
    static_assert(std::is_convertible<V, T>::value, "");
    return PersistentArray(set_rec(root_, i, std::forward<V>(v)), init_v_);
  }
  [[nodiscard]] PersistentArray set(size_t i, const T& v) const {
    return PersistentArray(set_rec(root_, i, v), init_v_);
  }
  [[nodiscard]] PersistentArray set(size_t i, T&& v) const {
    return PersistentArray(set_rec(root_, i, std::move(v)), init_v_);
  }
};
