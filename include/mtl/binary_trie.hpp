#pragma once
#include "traits/set_traits.hpp"
#include "bit_manip.hpp"
#include <array>
#include <memory>
#include <type_traits>
#include <cstdint>
#include <initializer_list>
#include <algorithm>
#include <cassert>

template<typename T, typename M,
    int8_t W = sizeof(T) * 8>
class BinaryTrieBase : public traits::AssociativeArrayDefinition<T, M> {
  static_assert(std::is_unsigned<T>::value, "");
 public:
  using types = traits::AssociativeArrayDefinition<T, M>;
  using key_type = typename types::key_type;
  using value_type = typename types::value_type;
  using init_type = typename types::init_type;
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
    node_ptr& left() { return c[0]; }
    node_ptr& right()  { return c[1]; }
  };
  struct Leaf : Node {
    value_type v;
    Leaf() = default;
    Leaf(const value_type& v) : Node(), v(v) {}
    Leaf(value_type&& v) : Node(), v(std::forward<value_type>(v)) {}
    key_type key() const {
      return types::key_of(v);
    }
    using Node::c;
    leaf_ptr prev() const {
      return std::static_pointer_cast<Leaf>(c[0]);
    }
    leaf_ptr next() const {
      return std::static_pointer_cast<Leaf>(c[1]);
    }
    void set_prev(leaf_ptr l) {
      c[0] = std::static_pointer_cast<Node>(l);
    }
    void set_next(leaf_ptr l) {
      c[1] = std::static_pointer_cast<Node>(l);
    }
  };
 protected:
  node_ptr root_;
  leaf_ptr dummy_;
  size_t size_;
  virtual void _init() {
    root_ = create_node_at(0, 0);
    dummy_ = std::make_shared<Leaf>();
    root_->jump = dummy_;
    dummy_->set_next(dummy_);
    dummy_->set_prev(dummy_);
    size_ = 0;
  }
  void _deinit() {
    root_ = nullptr;
    auto u = dummy_->next();
    dummy_->set_next(nullptr);
    u->set_prev(nullptr);
    while (u != dummy_) {
      auto n = u->next();
      u->set_next(nullptr);
      n->set_prev(nullptr);
      u = n;
    }
    dummy_ = nullptr;
  }
 public:
  BinaryTrieBase() {
    _init();
  }
  BinaryTrieBase(const BinaryTrieBase& rhs) {
    _insert_init(rhs.begin(), rhs.end());
  }
  virtual BinaryTrieBase& operator=(const BinaryTrieBase& rhs) {
    _deinit();
    _insert_init(rhs.begin(), rhs.end());
    return *this;
  }
  BinaryTrieBase(BinaryTrieBase&&) noexcept = default;
  virtual BinaryTrieBase& operator=(BinaryTrieBase&& rhs) noexcept {
    _deinit();
    root_ = std::move(rhs.root_);
    dummy_ = std::move(rhs.dummy_);
    size_ = std::move(rhs.size_);
    return *this;
  }
  virtual ~BinaryTrieBase() {
    _deinit();
  }
 protected:
  template<class InputIt>
  void _insert_init(InputIt begin, InputIt end) {
    static_assert(std::is_convertible<typename std::iterator_traits<InputIt>::value_type, value_type>::value, "");
    _init();
    if (begin == end) return;
    if (!std::is_sorted(begin, end, [](auto& l, auto& r) {
      return types::key_of(l) < types::key_of(r);
    })) {
      for (auto it = begin; it != end; ++it)
        _insert(*it);
      return;
    }
    auto push_link = [&](leaf_ptr l) {
      auto b = dummy_->prev();
      l->set_prev(b);
      l->set_next(dummy_);
      l->prev()->set_next(l);
      l->next()->set_prev(l);
    };
    std::array<node_ptr, W> us{};
    auto grow = [&](key_type x, int k, leaf_ptr l) {
      for (int i = k; i < W-1; i++) {
        us[i+1] = create_node_at(x, i+1);
        int c = (x >> (W-i-1)) & 1;
        us[i]->c[c] = us[i+1];
        us[i+1]->parent = us[i];
        us[i+1]->jump = l;
      }
      int c = x & 1;
      us[W-1]->c[c] = l;
      l->parent = us[W-1];
    };
    us[0] = root_;
    key_type x = types::key_of(*begin);
    auto l = create_leaf_at(x, *begin);
    push_link(l);
    us[0]->jump = l;
    grow(x, 0, l);
    size_t sz = 1;
    for (auto it = std::next(begin); it != end; ++it) {
      key_type px = x;
      x = types::key_of(*it);
      auto m = x ^ px;
      if (m == 0) continue;
//      [[assume(m != 0)]]
      int k = W-1;
      while (m > 1) {
        m >>= 1;
        --k;
      }
      l = create_leaf_at(x, *it);
      push_link(l);
      for (int i = 0; i < k; i++)
        if (!us[i]->c[1]) us[i]->jump = l;
      us[k]->jump = nullptr;
      grow(x, k, l);
      ++sz;
    }
    size_ = sz;
  }
 public:
  template<typename InputIt>
  explicit BinaryTrieBase(InputIt begin, InputIt end) {
    _insert_init(begin, end);
  }
  size_t size() const {
    return size_;
  }
  bool empty() const { return size() == 0; }
  void clear() {
    _deinit();
    _init();
  }
 protected:
  template<bool> struct iterator_base;
 public:
  using iterator = iterator_base<false>;
  using const_iterator = iterator_base<true>;
  iterator begin() {
    return iterator(dummy_->next());
  }
  iterator end() {
    return iterator(dummy_);
  }
  const_iterator begin() const {
    return const_iterator(dummy_->next());
  }
  const_iterator end() const {
    return const_iterator(dummy_);
  }
 protected:
  virtual std::pair<int, node_ptr> _traverse(const key_type& key, 
                                             int depth = 0, 
                                             node_ptr root = nullptr) const {
    int i, c;
    key_type x = key;
    auto u = !root ? root_ : root;
    for (i = depth; i < W; i++) {
      c = (x >> (W-i-1)) & 1;
      if (!u->c[c]) break;
      u = u->c[c];
    }
    return std::make_pair(i, u);
  }
  iterator _lower_bound(const key_type& x) const {
    auto reached = _traverse(x);
    int i = reached.first;
    node_ptr u = reached.second;
    if (i == W) return iterator(std::static_pointer_cast<Leaf>(u));
    auto l = (((x >> (W-i-1)) & 1) == 0) ? u->jump : u->jump->next();
    return iterator(l);
  }
  iterator _upper_bound(const key_type& x) const {
    auto it = _lower_bound(x);
    if (types::key_of(*it) == x)
      ++it;
    return it;
  }
  virtual iterator _find(const key_type& x) const {
    auto reached = _traverse(x);
    int i = reached.first;
    node_ptr u = reached.second;
    if (i == W)
      return iterator(std::static_pointer_cast<Leaf>(u));
    else
      return end();
  }
  virtual node_ptr create_node_at(const key_type&, int) {
    return std::make_shared<Node>();
  }
  virtual leaf_ptr create_leaf_at(const key_type&, const init_type& value) {
    return std::make_shared<Leaf>(value);
  }
  virtual leaf_ptr create_leaf_at(const key_type&, init_type&& value) {
    return std::make_shared<Leaf>(std::move(value));
  }
  template<typename Value>
  iterator _emplace_impl(key_type x, int height, node_ptr forked, Value&& value) {
    assert(height < W);
    int i = height;
    node_ptr u = forked;
    auto f = u;
    int c = (x >> (W-i-1)) & 1;
    auto fc = c;
    auto fi = i;
    auto pred = c == 1 ? u->jump : u->jump->prev();
    u->jump = nullptr;
    auto l = create_leaf_at(x, std::forward<Value>(value));
    l->set_prev(pred);
    l->set_next(pred->next());
    l->prev()->set_next(l);
    l->next()->set_prev(l);
    for (; i < W-1; i++) {
      c = (x >> (W-i-1)) & 1;
      assert(!u->c[c]);
      u->c[c] = create_node_at(x, i+1);
      u->c[c]->parent = u;
      u->c[c]->jump = l;
      u = u->c[c];
    }
    {
      c = (x >> (W-i-1)) & 1;
      u->c[c] = l;
      u->c[c]->parent = u;
    }
    if (f == root_) [[unlikely]] {
      f->jump = l;
    } else [[likely]] {
      auto v = f->parent.lock();
      fi--;
      while (v) {
        c = x >> (W-fi-1) & 1;
        if (c != fc and !v->jump)
          break;
        if (!v->c[fc])
          v->jump = l;
        v = v->parent.lock();
        fi--;
      }
    }
    size_++;
    return iterator(l);
  }
  template<typename Value>
  std::pair<iterator, bool> _insert(Value&& value) {
    static_assert(std::is_convertible<Value, value_type>::value, "");
    key_type x = types::key_of(value);
    auto reached = _traverse(x);
    int i = reached.first;
    node_ptr u = reached.second;
    if (i == W)
      return std::make_pair(iterator(std::static_pointer_cast<Leaf>(u)), false);
    return std::make_pair(_emplace_impl(x, i, u, std::forward<Value>(value)), true);
  }
  virtual std::pair<int, node_ptr> climb_to_lca(leaf_ptr l, key_type x) {
    key_type m = x ^ types::key_of(l->v);
    if (m == 0)
      return std::make_pair(W, std::static_pointer_cast<Node>(l));
    int h = bm::clz(m) - (64 - W);
    node_ptr f = std::static_pointer_cast<Node>(l);
    for (int i = W; i > h; i--)
      f = f->parent.lock();
    return std::make_pair(h, f);
  }
  template<class Value>
  iterator _emplace_hint(const_iterator hint, Value&& value) {
    key_type x = types::key_of(value);
    if (empty())
      return _emplace_impl(x, 0, root_, std::forward<Value>(value));
    if (hint == end())
      --hint;
    int h;
    node_ptr f;
    std::tie(h, f) = climb_to_lca(hint.ptr_, x);
    std::tie(h, f) = _traverse(x, h, f);
    if (h == W)
      return iterator(std::static_pointer_cast<Leaf>(f));
    return _emplace_impl(x, h, f, std::forward<Value>(value));
  }

  virtual void erase_node_at(const key_type&, int, node_ptr) {}
  virtual bool _erase(const key_type& key) {
    auto it = _find(key);
    if (it != end()) {
      _erase_from_leaf(types::key_of(*it), it.ptr_);
      return true;
    } else {
      return false;
    }
  }
  template<typename Key>
  iterator _erase_from_leaf(Key&& key, leaf_ptr l) {
    static_assert(std::is_convertible<Key, key_type>::value, "");
    key_type x = std::forward<Key>(key);
    assert(x == l->key());
    l->prev()->set_next(l->next());
    l->next()->set_prev(l->prev());
    int i,c;
    auto v = std::static_pointer_cast<Node>(l);
    for (i = W-1; i >= 0; i--) {
      erase_node_at(x, i+1, v);
      v = v->parent.lock();
      c = (x >> (W-i-1)) & 1;
      v->c[c] = nullptr;
      if (v->c[c^1]) break;
    }
    auto nj = c ? l->prev() : l->next();
    auto fc = c;
    v->jump = nj;
    v = v->parent.lock();
    i--;
    for (; i >= 0; i--) {
      assert(v);
      c = (x >> (W-i-1)) & 1;
      if (c != fc) {
        if (!v->jump) break;
        v->jump = nj;
      }
      v = v->parent.lock();
    }
    size_--;
    return iterator(l->next());
  }
  iterator iterator_remove_const(const const_iterator& it) {
    return iterator(it.ptr_);
  }
  iterator iterator_remove_const(const_iterator&& it) {
    return iterator(std::move(it.ptr_));
  }
  iterator _erase(iterator it) {
    if (it == end()) return it;
    return _erase_from_leaf(types::key_of(*it), it.ptr_);
  }
  iterator _erase(const_iterator it) {
    if (it == end()) return iterator_remove_const(it);
    return _erase_from_leaf(types::key_of(*it), it.ptr_);
  }
 protected:
  template<bool Const>
  struct iterator_base {
    using difference_type = ptrdiff_t;
    using value_type = BinaryTrieBase::value_type;
    using pointer = typename std::conditional<Const,
                                              const value_type*,
                                              value_type*>::type;
    using reference = typename std::conditional<Const,
                                                const value_type&,
                                                value_type&>::type;
    using iterator_category = std::bidirectional_iterator_tag;
    leaf_ptr ptr_;
    iterator_base(leaf_ptr p) : ptr_(p) {}
    template<bool C>
    iterator_base(const iterator_base<C>& rhs) : ptr_(rhs.ptr_) {}
    template<bool C>
    iterator_base& operator=(const iterator_base<C>& rhs) {
      ptr_ = rhs.ptr_;
    }
    template<bool C>
    iterator_base(iterator_base<C>&& rhs) : ptr_(std::move(rhs.ptr_)) {}
    template<bool C>
    iterator_base& operator=(iterator_base<C>&& rhs) {
      ptr_ = std::move(rhs.ptr_);
    }
    reference operator*() {
      return ptr_->v;
    }
    pointer operator->() {
      return &(ptr_->v);
    }
    template<bool C>
    bool operator==(const iterator_base<C>& rhs) const {
      return ptr_ == rhs.ptr_;
    }
    template<bool C>
    bool operator!=(const iterator_base<C>& rhs) const {
      return !operator==(rhs);
    }
    iterator_base& operator++() {
      ptr_ = ptr_->next();
      return *this;
    }
    iterator_base operator++(int) const {
      iterator_base ret = *this;
      operator++();
      return ret;
    }
    iterator_base& operator--() {
      ptr_ = ptr_->prev();
      return *this;
    }
    iterator_base operator--(int) const {
      iterator_base ret = *this;
      operator--();
      return ret;
    }
  };
};

template<typename T, typename V, uint8_t W = sizeof(T)*8>
using BinaryTrie = traits::MapTraits<BinaryTrieBase<T, V, W>>;
template<typename T, uint8_t W = sizeof(T)*8>
using BinaryTrieSet = traits::SetTraits<BinaryTrieBase<T, void, W>>;
template<typename T, typename V, uint8_t W = sizeof(T)*8>
using BinaryTrieMap = BinaryTrie<T, V, W>;
