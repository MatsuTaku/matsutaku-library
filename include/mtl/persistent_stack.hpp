#pragma once
#include <algorithm>
#include <cstddef>
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
    Node(const T& v, node_ptr next) : v(v), next(next) {}
    Node(T&& v, node_ptr next) : v(std::move(v)), next(next) {}
  };
 private:
  node_ptr top_;
  size_t size_;
  PersistentStack(node_ptr ptr, size_t sz) : top_(ptr), size_(sz) {}
 public:
  PersistentStack() : top_(nullptr), size_(0) {}
  size_t size() const { return size_; }
  bool empty() const { return size() == 0; }
  const T& top() const { return top_->v; }
 private:
  template<typename V>
  PersistentStack _push(V&& v) const {
    auto new_node = std::make_shared<Node>(std::forward<V>(v), top_);
    return PersistentStack(new_node, size()+1);
  }
 public:
  template<typename V>
  [[nodiscard]] PersistentStack push(V&& v) const {
    static_assert(std::is_convertible<V,T>::value, "");
    return _push(std::forward<V>(v));
  }
  [[nodiscard]] PersistentStack push(const T& v) const {
    return _push(v);
  }
  [[nodiscard]] PersistentStack push(T&& v) const {
    return _push(std::move(v));
  }
  [[nodiscard]] PersistentStack pop() const {
    return PersistentStack(top_->next, size()-1);
  }
  [[nodiscard]] PersistentStack concat(const PersistentStack& other) const {
    if (empty()) 
      return other;
    else 
      return pop().concat(other).push(top());
  }
  [[nodiscard]] PersistentStack reverse() const {
    PersistentStack ret;
    for (auto t = *this; !t.empty(); t = t.pop())
      ret = ret.push(t.top());
    return ret;
  }
};