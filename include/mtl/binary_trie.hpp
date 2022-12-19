#pragma once
#include <array>
#include <memory>
#include <type_traits>
#include <cstdint>
#include <iostream>
using std::cerr;
using std::endl;

template<typename T, typename M>
struct BinaryTrieDefinition {
  static_assert(std::is_unsigned<T>::value, "");
 public:
  using key_type = T;
  static constexpr bool kPairValue = !std::is_same<M, void>::value;
  using value_type = typename std::conditional<kPairValue, std::pair<T,M>, T>::type;
  using mapped_type = typename std::conditional<kPairValue, M, T>::type;
  struct Node;
  using node_ptr = std::shared_ptr<Node>;
  using node_weak_ptr = std::weak_ptr<Node>;
  struct Leaf;
  using leaf_ptr = std::shared_ptr<Leaf>;
  struct Node {
    std::array<node_ptr, 2> c;
    leaf_ptr jump;
    node_weak_ptr parent;
    Node() = default;
    inline node_ptr& left() { return c[0]; }
    inline node_ptr& right()  { return c[1]; }
  };
  struct Leaf : Node {
    union {
      key_type x;
      value_type v;
    };
    Leaf() = default;
    Leaf(const value_type& v) : Node(), v(v) {}
    Leaf(value_type&& v) : Node(), v(std::forward<value_type>(v)) {}
    using Node::c;
    inline leaf_ptr prev() const { return std::static_pointer_cast<Leaf>(c[0]); }
    inline leaf_ptr next() const { return std::static_pointer_cast<Leaf>(c[1]); }
    inline void set_prev(leaf_ptr l) {
      c[0] = std::static_pointer_cast<Node>(l);
    }
    inline void set_next(leaf_ptr l) {
      c[1] = std::static_pointer_cast<Node>(l);
    }
  };
};

template<typename T, typename M,
    int8_t W = sizeof(T) * 8>
class BinaryTrieBase : public BinaryTrieDefinition<T, M> {
  static_assert(std::is_unsigned<T>::value, "");
  using Def = BinaryTrieDefinition<T, M>;
 public:
  using key_type = typename Def::key_type;
  static constexpr bool kPairValue = Def::kPairValue;
  using value_type = typename Def::value_type;
  using mapped_type = typename Def::mapped_type;
  using Node = typename Def::Node;
  using node_ptr = typename Def::node_ptr;
  using node_weak_ptr = typename Def::node_weak_ptr;
  using Leaf = typename Def::Leaf;
  using leaf_ptr = typename Def::leaf_ptr;
 protected:
  node_ptr root_;
  leaf_ptr dummy_;
  size_t size_;
  BinaryTrieBase() : root_(nullptr), dummy_(nullptr), size_(0) {}
 public:
  struct iterator;
  inline iterator begin() const {
    return iterator(dummy_->next());
  }
  inline iterator end() const {
    return iterator(dummy_);
  }
  inline size_t size() const {
    return size_;
  }
 protected:
  iterator _lower_bound(key_type x) const {
    int i, c;
    auto u = root_;
    for (i = 0; i < W; i++) {
      c = (x >> (W-i-1)) & 1;
      if (!u->c[c]) break;
      u = u->c[c];
    }
    if (i == W) return iterator(std::static_pointer_cast<Leaf>(u));
    auto l = (c == 0) ? u->jump : u->jump->next();
    return l == dummy_ ? end() : iterator(l);
  }
  iterator _upper_bound(key_type x) const {
    auto it = _lower_bound(x);
    if (key_of(it) == x)
      ++it;
    return it;
  }
  iterator _find(key_type x) const {
    int i, c;
    auto u = root_;
    for (i = 0; i < W; i++) {
      c = (x >> (W-i-1)) & 1;
      if (!u->c[c]) return end();
      u = u->c[c];
    }
    return iterator(std::static_pointer_cast<Leaf>(u));
  }
 public:
  void clear() {
    root_ = nullptr;
    dummy_ = nullptr;
    size_ = 0;
  }
 protected:
  void _init() {
    root_ = std::make_shared<Node>();
    dummy_ = std::make_shared<Leaf>();
    root_->jump = dummy_;
    dummy_->set_prev(dummy_);
    dummy_->set_next(dummy_);
    size_ = 0;
  }
  virtual inline node_ptr create_node_at(key_type x [[maybe_unused]], int i [[maybe_unused]]) {
    return std::make_shared<Node>();
  }
  virtual inline node_ptr create_leaf_at(key_type x [[maybe_unused]], value_type value) {
    return std::static_pointer_cast<Node>(std::make_shared<Leaf>(std::forward<value_type>(value)));
  }
  std::pair<iterator, bool> _insert(value_type value) {
    int i, c;
    key_type x = key_of(value);
    if (!root_) {
      this->_init();
    }
    auto u = root_;
    for (i = 0; i < W; i++) {
      c = (x >> (W-i-1)) & 1;
      if (!u->c[c]) break;
      u = u->c[c];
    }
    if (i == W) return std::make_pair(iterator(std::static_pointer_cast<Leaf>(u)), false);
    auto pred = c == 1 ? u->jump : u->jump->prev();
    u->jump = nullptr;
    for (; i < W-1; i++) {
      c = (x >> (W-i-1)) & 1;
      u->c[c] = create_node_at(x, i+1);
      u->c[c]->parent = u;
      u = u->c[c];
    }
    {
      c = (x >> (W-i-1)) & 1;
      u->c[c] = create_leaf_at(x, std::forward<value_type>(value));
      u->c[c]->parent = u;
      u = u->c[c];
      i++;
    }
    auto l = std::static_pointer_cast<Leaf>(u);
    l->set_prev(pred);
    l->set_next(pred->next());
    l->prev()->set_next(l);
    l->next()->set_prev(l);
    auto v = u->parent.lock();
    while (v) {
      if ((!v->left() && (!v->jump || std::static_pointer_cast<Leaf>(v->jump)->x > x)) ||
          (!v->right() && (!v->jump || std::static_pointer_cast<Leaf>(v->jump)->x < x)))
        v->jump = l;
      v = v->parent.lock();
    }
    size_++;
    return std::make_pair(iterator(l), true);
  }

  virtual void erase_node_at(key_type x [[maybe_unused]], int i [[maybe_unused]], node_ptr u [[maybe_unused]]) {}
  bool _erase(key_type x) {
    int i, c;
    auto u = root_;
    for (i = 0; i < W; i++) {
      c = (x >> (W-i-1)) & 1;
      if (!u->c[c]) return false;
      u = u->c[c];
    }
    auto l = std::static_pointer_cast<Leaf>(u);
    l->prev()->set_next(l->next());
    l->next()->set_prev(l->prev());
    auto v = u;
    for(i = W-1; i >= 0; i--) {
      c = (x >> (W-i-1)) & 1;
      v = v->parent.lock();
      erase_node_at(x, i+1, v->c[c]);
      v->c[c] = nullptr;
      if (v->c[c^1]) break;
    }
    c = (x >> (W-i-1)) & 1;
    v->jump = std::static_pointer_cast<Leaf>(u->c[c^1]);
    v = v->parent.lock();
    i--;
    for (; i >= 0; i--) {
      c = (x >> (W-i-1)) & 1;
      if (v->jump == l)
        v->jump = std::static_pointer_cast<Leaf>(l->c[c^1]);
      v = v->parent.lock();
    }
    size_--;
    return true;
  }

  key_type key_of(const iterator& it) const {
    return it.ptr_->x;
  }
  key_type key_of(iterator&& it) const {
    return it.ptr_->x;
  }
  key_type key_of(const value_type& v) const {
    return kPairValue ? *reinterpret_cast<const key_type*>(reinterpret_cast<const void*>(&v)) : v;
  }
  key_type key_of(value_type&& v) const {
    return kPairValue ? *reinterpret_cast<key_type*>(reinterpret_cast<void*>(&v)) : v;
  }
};
template<typename T, typename M, int8_t W>
struct BinaryTrieBase<T,M,W>::iterator {
  using value_type = BinaryTrieBase::value_type;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::bidirectional_iterator_tag;
  leaf_ptr ptr_;
  iterator(leaf_ptr p) : ptr_(p) {}
  inline reference operator*() {
    return ptr_->v;
  }
  inline pointer operator->() {
    return &(ptr_->v);
  }
  inline bool operator==(const iterator& rhs) const {
    return ptr_ == rhs.ptr_;
  }
  inline bool operator!=(const iterator& rhs) const {
    return !operator==(rhs);
  }
  inline iterator& operator++() {
    ptr_ = ptr_->next();
    return *this;
  }
  inline iterator operator++(int) const {
    iterator ret = *this;
    operator++();
    return ret;
  }
  inline iterator& operator--() {
    ptr_ = ptr_->prev();
    return *this;
  }
  inline iterator operator--(int) const {
    iterator ret = *this;
    operator--();
    return ret;
  }
};
//template<typename T, typename M, int8_t W>
//constexpr bool BinaryTrieBase<T,M,W>::kPairValue;

template<typename T, typename V>
class BinaryTrie : public BinaryTrieBase<T, V> {
  using Base = BinaryTrieBase<T, V>;
 public:
  using key_type = typename Base::key_type;
  using mapped_type = typename Base::mapped_type;
  using value_type = typename Base::value_type;
  using iterator = typename Base::iterator;
  BinaryTrie() = default;
  iterator lower_bound(const key_type& x) const {
    return Base::_lower_bound(x);
  }
  iterator lower_bound(key_type&& x) const {
    return Base::_lower_bound(std::forward<key_type>(x));
  }
  iterator upper_bound(const key_type& x) const {
    return Base::_upper_bound(x);
  }
  iterator upper_bound(key_type&& x) const {
    return Base::_upper_bound(std::forward<key_type>(x));
  }
  iterator find(const key_type& x) const {
    return Base::_find(x);
  }
  iterator find(key_type&& x) const {
    return Base::_find(std::forward<key_type>(x));
  }
  std::pair<iterator, bool> insert(const value_type& v) {
    return Base::_insert(v);
  }
  std::pair<iterator, bool> insert(value_type&& v) {
    return Base::_insert(std::forward<value_type>(v));
  }
  bool erase(const key_type& x) {
    return Base::_erase(x);
  }
  bool erase(key_type&& x) {
    return Base::_erase(x);
  }
};

template<typename T>
using BinaryTrieSet = BinaryTrie<T, void>;
template<typename T, typename V>
using BinaryTrieMap = BinaryTrie<T, V>;
