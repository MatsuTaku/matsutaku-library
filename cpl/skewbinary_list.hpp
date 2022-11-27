#pragma once

#include <memory>
#include <vector>
#include <stack>
#include <cassert>
#include <cstddef>
#include <type_traits>
#include <iterator>
#include <iostream>
#include "bit_manip.hpp"

/*
 * Persistent purely functional random-access list
 */
template<typename T>
class PersistentSkewbinaryList {
 public:
  using value_type = T;
  class iterator;

 private:
  struct Digit;
  struct Node;
  using digit_pointer = std::shared_ptr<Digit>;
  using node_pointer = std::shared_ptr<Node>;
  struct Node {
    T v;
    uint8_t dig;
    node_pointer left, right;
    Node(const T& v, uint8_t dig = 1) : v(v), dig(dig), left(nullptr), right(nullptr) {}
    size_t size() const {
      return (1ull<<dig) - 1;
    }
    bool is_lump() const {
      return dig > 1 and left == nullptr;
    }
  };
  struct Digit {
    node_pointer ch;
    digit_pointer next;
    Digit(node_pointer root, digit_pointer right) : ch(root), next(right) {}
  };

  digit_pointer root_;
  size_t size_;

 private:
  explicit PersistentSkewbinaryList(digit_pointer root, size_t size=0) : root_(root), size_(size) {}

  void _push_node(node_pointer u) {
    root_ = std::make_shared<Digit>(u, root_);
    size_ += u->size();
  }

 public:
  PersistentSkewbinaryList() : root_(nullptr), size_(0) {}

  template<class InputIt>
  PersistentSkewbinaryList(InputIt begin, InputIt end) : PersistentSkewbinaryList() {
    using traits = std::iterator_traits<InputIt>;
    using value_type = typename traits::value_type;
    static_assert(std::is_convertible<T, value_type>::value);
    using category = typename traits::iterator_category;
    if constexpr (std::is_base_of<std::bidirectional_iterator_tag, category>::value) {
      for (auto it = std::prev(end); ; --it) {
        *this = pushed(*it);
        if (it == begin)
          break;
      }
    } else if constexpr (std::is_base_of<std::forward_iterator_tag, category>::value) {
      std::stack<T> s(begin, end);
      while (!s.empty()) {
        _push_node(std::make_shared<Node>(s.top()));
        s.pop();
      }
    }
  }
  PersistentSkewbinaryList(std::initializer_list<T> list) : PersistentSkewbinaryList(list.begin(), list.end()) {}

  size_t size() const { return size_; }
  bool empty() const { return size() == 0; }

  T front() const {
    return root_->ch->v;
  }

  [[nodiscard]] PersistentSkewbinaryList pushed(const T& v) const {
    if (root_ and root_->next and root_->ch->dig == root_->next->ch->dig) {
      auto new_u = std::make_shared<Node>(v, root_->ch->dig+1);
      new_u->left = root_->ch;
      new_u->right = root_->next->ch;
      auto new_root = std::make_shared<Digit>(new_u, root_->next->next);
      return PersistentSkewbinaryList(new_root, size()+1);
    } else {
      auto new_root = std::make_shared<Digit>(std::make_shared<Node>(v), root_);
      return PersistentSkewbinaryList(new_root, size()+1);
    }
  }

  [[nodiscard]] PersistentSkewbinaryList popped() const {
    if (root_->ch->size() > 1) {
      if (!root_->ch->is_lump()) {
        digit_pointer new_next = std::make_shared<Digit>(root_->ch->right, root_->next);
        digit_pointer new_root = std::make_shared<Digit>(root_->ch->left, new_next);
        return PersistentSkewbinaryList(new_root, size()-1);
      } else {
        node_pointer new_u = std::make_shared<Node>(root_->ch->v, root_->ch->dig-1);
        digit_pointer new_next = std::make_shared<Digit>(new_u, root_->next);
        digit_pointer new_root = std::make_shared<Digit>(new_u, new_next);
        return PersistentSkewbinaryList(new_root, size()-1);
      }
    } else {
      return PersistentSkewbinaryList(root_->next, size()-1);
    }
  }

  T get_at(int i) const {
    int j = i;
    auto d = root_;
    while (d->ch->size() <= j) {
      j -= d->ch->size();
      d = d->next;
    }
    auto u = d->ch;
    while (j > 0) {
      j--;
      if (u->is_lump()) {
        return u->v;
      }
      if (j < u->left->size()) {
        u = u->left;
      } else {
        j -= u->left->size();
        u = u->right;
      }
    }
    return u->v;
  }

  [[nodiscard]] PersistentSkewbinaryList updated_at(int i, const T& v) const {
    auto d = root_;
    digit_pointer pd = std::make_shared<Digit>(nullptr, nullptr);
    PersistentSkewbinaryList dst(pd, size());
    int j = i;
    while (j >= d->ch->size()) {
      auto new_d = std::make_shared<Digit>(nullptr, nullptr);
      pd->ch = d->ch;
      pd->next = new_d;
      pd = new_d;

      j -= d->ch->size();
      d = d->next;
    }

    auto u = d->ch;
    node_pointer pu = std::make_shared<Node>(d->ch->v, d->ch->dig);
    pd->ch = pu;
    pd->next = d->next;
    while (j > 0 and !u->is_lump()) {
      j--;
      if (j < u->left->size()) {
        auto new_u = std::make_shared<Node>(u->left->v, u->left->dig);
        pu->left = new_u;
        pu->right = u->right;
        pu = new_u;

        u = u->left;
      } else {
        auto new_u = std::make_shared<Node>(u->right->v, u->right->dig);
        pu->left = u->left;
        pu->right = new_u;
        pu = new_u;

        j -= u->left->size();
        u = u->right;
      }
    }
    if (!u->is_lump()) {
      pu->v = v;
      pu->left = u->left;
      pu->right = u->right;
    } else {
      int s = u->size() >> 1;
      int dig = u->dig-1;
      while (j > 0) {
        j--;
        pu->left = std::make_shared<Node>(u->v, dig);
        pu->right = std::make_shared<Node>(u->v, dig);
        if (j < s) {
          pu = pu->left;
        } else {
          j -= s;
          pu = pu->right;
        }
        s >>= 1;
        dig--;
      }
      pu->v = v;
      pu->left = pu->right = std::make_shared<Node>(u->v, dig);
    }
    return dst;
  }

  [[nodiscard]] PersistentSkewbinaryList dropped(const int i) const {
    int j = i;
    auto d = root_;
    while (d and d->ch->size() <= j) {
      j -= d->ch->size();
      d = d->next;
    }
    if (!d) {
      return PersistentSkewbinaryList();
    }
    PersistentSkewbinaryList dst(d->next); // dst.size_ is not correct value.
    auto u = d->ch;
    while (j > 0) {
      j--;
      if (j < u->left->size()) {
        dst._push_node(u->right);
        u = u->left;
      } else {
        j -= u->left->size();
        u = u->right;
      }
    }
    dst._push_node(u);
    dst.size_ = size() - i;
    return dst;
  }

  uint64_t sbdig_to_num(int dig) const {
    return (1ull<<dig) - 1;
  }

  [[nodiscard]] PersistentSkewbinaryList extended(size_t n, const T& v) const {
    PersistentSkewbinaryList dst(root_, size());
    auto m = n;
    while (m > 0) {
      if (dst.root_ and dst.root_->next and dst.root_->ch->size() == dst.root_->next->ch->size()) {
        auto new_u = std::make_shared<Node>(v, dst.root_->ch->dig+1);
        new_u->left = dst.root_->ch;
        new_u->right = dst.root_->next->ch;
        dst.root_ = std::make_shared<Digit>(new_u, dst.root_->next->next);
        dst.size_++;
        m--;
      } else if (m >= dst.root_->ch->size()) {
        m -= dst.root_->ch->size();
        dst._push_node(std::make_shared<Node>(v, dst.root_->ch->dig));
      } else {
        break;
      }
    }
    while (m > 0) {
      int dig = 63-bm::clz(m+1);
      dst._push_node(std::make_shared<Node>(v, dig));
      size_t s = (1ull<<dig) - 1;
      assert(m >= s);
      m -= s;
    }
    assert(dst.size_ == size() + n);
    return dst;
  }

 public:
  T operator[](size_t i) const {
    return get_at(i);
  }

  class iterator {
   public:
    using value_type = T;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = unsigned long long;
   private:
    digit_pointer root_;
   public:
    iterator(digit_pointer root) : root_(root) {}
    value_type operator*() const { return root_->ch->v; }
    bool operator==(iterator r) { return root_ == r.root_; }
    bool operator!=(iterator r) { return root_ != r.root_; }
    iterator& operator++() {
      root_ = PersistentSkewbinaryList(root_).popped().root_;
      return *this;
    }
    iterator operator++(int) {
      auto dst = *this;
      root_ = PersistentSkewbinaryList(root_).popped().root_;
      return dst;
    }
    iterator operator+(difference_type x) const {
      return iterator(PersistentSkewbinaryList(root_).dropped(x).root_);
    }
    iterator operator+=(difference_type x) {
      root_ = PersistentSkewbinaryList(root_).dropped(x).root_;
      return *this;
    }
  };

  iterator begin() const {
    return iterator(root_);
  }
  iterator end() const {
    return iterator(nullptr);
  }

};
