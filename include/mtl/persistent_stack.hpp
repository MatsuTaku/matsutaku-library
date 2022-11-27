#pragma once
#include <memory>

template<typename T>
class PersistentStack {
 public:
  struct Node;
  using node_ptr = std::shared_ptr<Node>;
  struct Node {
    T v;
    node_ptr next;
    Node() = default;
    Node(T v, node_ptr next) : v(v), next(next) {}
  };
 private:
  node_ptr top_;
  size_t size_;
  PersistentStack(node_ptr ptr, size_t sz) : top_(ptr), size_(sz) {}
 public:
  PersistentStack() : top_(nullptr), size_(0) {}
  size_t size() const { return size_; }
  bool empty() const { return size() == 0; }
  T top() const { return top_->v; }
  [[nodiscard]] PersistentStack push(const T& v) const {
    auto new_node = std::make_shared<Node>(v, top_);
    return PersistentStack(new_node, size()+1);
  }
  [[nodiscard]] PersistentStack pop() const {
    return PersistentStack(top_->next, size()-1);
  }
};