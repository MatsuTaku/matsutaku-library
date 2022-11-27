#pragma once
#include <memory>
#include <cassert>
#include <vector>
#include <list>
#include <iostream>

template<typename T, typename Cond = std::less<>>
class FibonacciHeap {
  Cond cond_;

 public:
  struct Node;
  using node_ptr = Node*;
  using const_node_ptr = const Node*;
  struct Node {
    node_ptr next = nullptr;
    node_ptr prev = nullptr;
    node_ptr child = nullptr;
    node_ptr parent = nullptr;
    int deg = 0;
    bool marked = false;
    bool enabled = false;
    std::pair<int, T> value;

    Node() = default;
    void init(int k, T v) {
      next = prev = this;
      child = parent = nullptr;
      deg = 0;
      marked = false;
      enabled = true;
      value = {k, v};
    }
    T priority() const { return value.second; }
    void free() {
      enabled = false;
    }
  };

 private:
  node_ptr root_ = nullptr;
  size_t sz_ = 0;
  std::vector<node_ptr> map_;
  std::vector<Node> container_;
  std::vector<node_ptr> deg_table_;

 public:
  explicit FibonacciHeap(size_t size) : map_(size) {
    container_.reserve(size);
    std::array<size_t,2> tb{1,1};
    int k = 2;
    while (tb[1] < size) {
      auto x = tb[0]+tb[1];
      tb[0] = tb[1];
      tb[1] = x;
      ++k;
    }
    deg_table_.resize(k);
  }

  inline std::pair<int, T> top() const {
    assert(root_ and root_->enabled);
    return root_->value;
  }

  inline std::pair<int, T> get(int key) const {
    assert(map_[key] and map_[key]->enabled);
    return map_[key]->value;
  }

  // Time complexity: O(1)
  void push(int key, T value) {
    if (map_[key] and map_[key]->enabled) {
      update(key, value);
      return;
    }
    if (!map_[key]) {
      container_.emplace_back();
      map_[key] = &container_.back();
    }
    node_ptr node = map_[key];
    node->init(key, value);
    _PushLink(node, &deg_table_[0]);
    if (!root_ or _CompareNodePriority(root_, node))
      root_ = node;
    ++sz_;
  }

  // Time complexity: O(log n)
  void update(int key, T updated_value) {
    if (!map_[key] or !map_[key]->enabled) {
      push(key, updated_value);
      return;
    }
    auto node = map_[key];
    if (!cond_(node->priority(), updated_value)) {
      return;
    }
    node->value.second = updated_value;
    if (node->parent and _CompareNodePriority(node->parent, node)) {
      _Consolidate(node);
    }
    assert(root_);
    if (_CompareNodePriority(root_, node)) {
      root_ = node;
    }
  }

  // Time complexity: O(log n)
  void pop() {
    assert(root_);
    auto r = root_;
    root_ = nullptr;
    _PopLink(r, &deg_table_[r->deg]);
    _ExpandChildren(r);
    _ExtractTop();
    r->free();
    --sz_;
  }

  // Time complexity: O(log n)
  void erase(int key) {
    assert(map_[key] and map_[key]->enabled);
    auto node = map_[key];
    bool needs_extraction = root_ == node;
    if (node->parent) {
      _Consolidate(node);
    }
    assert(!node->parent);
    _PopLink(node, &deg_table_[node->deg]);
    _ExpandChildren(node);
    if (needs_extraction)
      _ExtractTop();
    node->free();
    --sz_;
  }

  size_t size() const { return sz_; }
  bool empty() const { return size() == 0; }
  void clear() {
    root_ = nullptr;
    sz_ = 0;
    map_.fill(nullptr);
    container_.clear();
  }

  void print_for_debug() const {
      for (int k = 0; k < (int)deg_table_.size(); k++) {
        std::cout<<"d="<<k<<std::endl;
        if (deg_table_[k]) {
          std::cout<<"forward"<<std::endl;
          auto r = deg_table_[k];
          do {
            std::cout << r->priority() << ' ' << std::flush;
            r = r->next;
          } while (r != deg_table_[k]);
          std::cout<<std::endl;
          std::cout<<"deg"<<std::endl;
          r = deg_table_[k];
          do {
            std::cout<<r->deg<<' '<<std::flush;
            r = r->prev;
          } while (r != deg_table_[k]);
          std::cout<<std::endl;
        } else {
          std::cout<<"empty"<<std::endl;
        }
      }
  }

 private:
  bool _CompareNodePriority(const_node_ptr l, const_node_ptr r) const {
    return cond_(l->priority(), r->priority());
  }

  void _PushLink(node_ptr node, node_ptr* link_head) {
    if (!*link_head) {
      *link_head = node;
      node->next = node;
      node->prev = node;
    } else {
      auto b = (*link_head)->prev;
      node->next = *link_head;
      node->prev = b;
      b->next = node;
      (*link_head)->prev = node;
    }
  }

  void _PopLink(node_ptr node, node_ptr* link_head) {
    assert(*link_head);
    if (node->next == node) {
      assert(node->prev == node);
      assert(*link_head == node);
      *link_head = nullptr;
    } else {
      auto n = node->next;
      auto p = node->prev;
      p->next = n;
      n->prev = p;
      if (*link_head == node)
        *link_head = n;
      node->next = node;
      node->prev = node;
    }
  }

  node_ptr _Meld(node_ptr l, node_ptr r) {
    assert(l->deg == r->deg);
    if (_CompareNodePriority(l, r)) {
      std::swap(l, r);
    }
    _PushLink(r, &l->child);
    r->parent = l;
    l->deg++;
    return l;
  }

  void _ExpandChildren(node_ptr node) {
    while (node->child) {
      auto c = node->child;
      _PopLink(c, &node->child);
      --node->deg;
      c->parent = nullptr;
      c->marked = false;
      _PushLink(c, &deg_table_[c->deg]);
    }
    assert(node->deg == 0);
  }

  void _ExtractTop() {
    int k = 0;
    while (k < (int)deg_table_.size()) {
      while (k < (int)deg_table_.size() and !deg_table_[k]) ++k;
      if (k == (int)deg_table_.size())
        break;
      if (deg_table_[k]->next != deg_table_[k]) { // Multiple trees at current degree
        auto node = deg_table_[k];
        _PopLink(node, &deg_table_[k]);
        assert(node->deg == k);
        while (deg_table_[node->deg]) {
          auto d = node->deg;
          auto u = deg_table_[d];
          _PopLink(u, &deg_table_[d]);
          node = _Meld(node, u);
          assert(node->deg == d+1);
        }
        _PushLink(node, &deg_table_[node->deg]);
      } else { // Single tree at current degree
        if (!root_ or _CompareNodePriority(root_, deg_table_[k]))
          root_ = deg_table_[k];
        ++k;
      }
    }
  }

  void _Consolidate(node_ptr node) {
    assert(node->parent);
    auto p = node->parent;
    if (p->marked) {
      assert(p->parent); // Parent is not one of the roots.
      _Consolidate(p);
    }
    assert(!p->marked);
    if (p->parent) {
      p->marked = true;
    } else {
      _PopLink(p, &deg_table_[p->deg]);
      _PushLink(p, &deg_table_[p->deg-1]);
    }
    _PopLink(node, &p->child);
    p->deg--;
    node->parent = nullptr;
    node->marked = false;
    _PushLink(node, &deg_table_[node->deg]);
  }

};
