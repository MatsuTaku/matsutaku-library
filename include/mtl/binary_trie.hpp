#pragma once
#include <array>
#include <memory>
#include <type_traits>
#include <cstdint>
#include <initializer_list>
#include <cassert>
#include <bitset>
#include <iostream>
using std::cerr;
using std::endl;

template<typename T, typename M>
struct AssociativeArrayDefinition {
  static constexpr bool kPairValue = true;
  using key_type = T;
  using mapped_type = M;
  using value_type = std::pair<T const, M>;
  using raw_key_type = typename std::remove_reference<T>::type;
  using raw_mapped_type = typename std::remove_reference<M>::type;
  using init_type = std::pair<raw_key_type, raw_mapped_type>;
  using moved_type = std::pair<raw_key_type&&, raw_mapped_type&&>;
  template<class K, class V>
  static inline key_type const& key_of(std::pair<K,V> const& kv) {
    return kv.first;
  }
};
template<typename T>
struct AssociativeArrayDefinition<T, void> {
  static constexpr bool kPairValue = false;
  using key_type = T;
  using value_type = T;
  using init_type = T;
  static inline key_type const& key_of(value_type const& k) { return k; }
};

template<typename T, typename M>
struct BinaryTrieDefinition : AssociativeArrayDefinition<T, M> {
  static_assert(std::is_unsigned<T>::value, "");
  using Base = AssociativeArrayDefinition<T, M>;
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
  using typename Base::key_type;
  using typename Base::value_type;
  struct Leaf : Node {
    value_type v;
    Leaf() = default;
    Leaf(const value_type& v) : Node(), v(v) {}
    Leaf(value_type&& v) : Node(), v(std::forward<value_type>(v)) {}
    inline key_type key() const {
      return Base::key_of(v);
    }
    using Node::c;
    inline leaf_ptr prev() const {
      return std::static_pointer_cast<Leaf>(c[0]);
    }
    inline leaf_ptr next() const {
      return std::static_pointer_cast<Leaf>(c[1]);
    }
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
  using Node = typename Def::Node;
  using node_ptr = typename Def::node_ptr;
  using node_weak_ptr = typename Def::node_weak_ptr;
  using Leaf = typename Def::Leaf;
  using leaf_ptr = typename Def::leaf_ptr;
 protected:
  node_ptr root_;
  leaf_ptr dummy_;
  size_t size_;
  virtual void _init() {
    root_ = std::make_shared<Node>();
    dummy_ = std::make_shared<Leaf>();
    root_->jump = dummy_;
    dummy_->set_next(dummy_);
    dummy_->set_prev(dummy_);
    size_ = 0;
  }
 public:
  BinaryTrieBase() {
    _init();
  }
  ~BinaryTrieBase() {
    root_ = nullptr;
    dummy_->set_next(nullptr);
    dummy_->set_prev(nullptr);
    dummy_ = nullptr;
  }
  template<typename InputIt>
  explicit BinaryTrieBase(InputIt begin, InputIt end) : BinaryTrieBase() {
    static_assert(std::is_convertible<typename std::iterator_traits<InputIt>::value_type, value_type>::value, "");
    // TODO: optimize
    for (auto it = begin; it != end; ++it)
      _insert(*it);
  }
  BinaryTrieBase(std::initializer_list<value_type> init) : BinaryTrieBase(init.begin(), init.end()) {}
  inline size_t size() const {
    return size_;
  }
  inline bool empty() const { return size() == 0; }
  struct iterator;
  inline iterator begin() const {
    return iterator(dummy_->next());
  }
  inline iterator end() const {
    return iterator(dummy_);
  }
 protected:
  template<typename Key>
  inline iterator _lower_bound(Key&& key) const {
    static_assert(std::is_convertible<Key, key_type>::value, "");
    key_type x = std::forward<Key>(key);
    int i, c;
    auto u = root_;
    for (i = 0; i < W; i++) {
      c = (x >> (W-i-1)) & 1;
      if (!u->c[c]) break;
      u = u->c[c];
    }
    if (i == W) return iterator(std::static_pointer_cast<Leaf>(u));
    auto l = (c == 0) ? u->jump : u->jump->next();
    return iterator(l);
  }
 protected:
  template<typename Key>
  inline iterator _upper_bound(Key&& key) const {
    static_assert(std::is_convertible<Key, key_type>::value, "");
    key_type x = std::forward<Key>(key);
    auto it = _lower_bound(x);
    if (Def::key_of(*it) == x)
      ++it;
    return it;
  }
  template<typename Key>
  inline iterator _find(Key&& key) const {
    static_assert(std::is_convertible<Key, key_type>::value, "");
    key_type x = std::forward<Key>(key);
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
  inline void clear() {
    _init();
  }
 protected:
  virtual inline node_ptr create_node_at(const key_type& x [[maybe_unused]], int i [[maybe_unused]]) {
    return std::make_shared<Node>();
  }
  virtual inline leaf_ptr create_leaf_at(const key_type& x [[maybe_unused]], const value_type& value) {
    return std::make_shared<Leaf>(value);
  }
  virtual inline leaf_ptr create_leaf_at(const key_type& x [[maybe_unused]], value_type&& value) {
    return std::make_shared<Leaf>(std::move(value));
  }
  template<typename Value>
  inline std::pair<iterator, bool> _insert(Value&& value) {
    static_assert(std::is_convertible<Value, value_type>::value, "");
    int i, c;
    key_type x = Def::key_of(value);
    auto u = root_;
    for (i = 0; i < W; i++) {
      c = (x >> (W-i-1)) & 1;
      if (!u->c[c]) break;
      u = u->c[c];
    }
    if (i == W)
      return std::make_pair(iterator(std::static_pointer_cast<Leaf>(u)), false);
    auto f = u;
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
    return std::make_pair(iterator(l), true);
  }

  virtual void erase_node_at(const key_type& x [[maybe_unused]], int i [[maybe_unused]], node_ptr u [[maybe_unused]]) {}
  template<typename Key>
  bool _erase(Key&& x) {
    static_assert(std::is_convertible<Key, key_type>::value, "");
    auto it = _find(x);
    if (it != end()) {
      _erase(it);
      return true;
    } else {
      return false;
    }
  }
  template<typename Key>
  inline iterator _erase(Key&& key, leaf_ptr l) {
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
  inline iterator _erase(iterator it) {
    if (it == end()) return it;
    return _erase(Def::key_of(*it), it.ptr_);
  }
};
template<typename T, typename M, int8_t W>
struct BinaryTrieBase<T,M,W>::iterator {
  using difference_type = ptrdiff_t;
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

#if __cplusplus >= 202002L
#include <concepts>
template<typename M>
concept IsAssociativeArray = requires (M m) {
  typename M::key_type;
  typename M::value_type;
  typename M::iterator;
  {m.size()} -> std::convertible_to<size_t>;
  {m.empty()} -> std::same_as<bool>;
  {m.clear()};
  {m.begin()} -> std::same_as<typename M::iterator>;
  {m.end()} -> std::same_as<typename M::iterator>;
};
#endif

template<class Base>
#if __cplusplus >= 202002L
requires IsAssociativeArray<Base>
#endif
class SetInterfaceBase : public Base {
 public:
  using key_type = typename Base::key_type;
  using value_type = typename Base::value_type;
  using iterator = typename Base::iterator;
  SetInterfaceBase() = default;
  template<typename InputIt>
  explicit SetInterfaceBase(InputIt begin, InputIt end) : Base(begin, end) {}
  SetInterfaceBase(std::initializer_list<value_type> init) : Base(init) {}
  using Base::begin;
  using Base::end;
  using reverse_iterator = std::reverse_iterator<iterator>;
  inline reverse_iterator rbegin() const {
    return reverse_iterator(begin());
  }
  inline reverse_iterator rend() const {
    return reverse_iterator(end());
  }
  template<class Key>
  inline iterator lower_bound(Key&& x) const {
    return Base::_lower_bound(std::forward<Key>(x));
  }
  inline iterator lower_bound(const key_type& x) const {
    return Base::_lower_bound(x);
  }
  inline iterator lower_bound(key_type&& x) const {
    return Base::_lower_bound(std::move(x));
  }
  template<class Key>
  inline iterator upper_bound(Key&& x) const {
    return Base::_upper_bound(std::forward<Key>(x));
  }
  inline iterator upper_bound(const key_type& x) const {
    return Base::_upper_bound(x);
  }
  inline iterator upper_bound(key_type&& x) const {
    return Base::_upper_bound(std::move(x));
  }
  template<class Key>
  inline iterator find(Key&& x) const {
    return Base::_find(std::forward<Key>(x));
  }
  inline iterator find(const key_type& x) const {
    return Base::_find(x);
  }
  inline iterator find(key_type&& x) const {
    return Base::_find(std::move(x));
  }
  template<class Key>
  inline size_t count(Key&& x) const {
    return find(std::forward<Key>(x)) != end();
  }
  inline size_t count(const key_type& x) const {
    return find(x) != end();
  }
  inline size_t count(key_type&& x) const {
    return find(std::move(x)) != end();
  }
  template<class Value>
  inline std::pair<iterator, bool> insert(Value&& v) {
    return Base::_insert(std::forward<Value>(v));
  }
  inline std::pair<iterator, bool> insert(const value_type& v) {
    return Base::_insert(v);
  }
  inline std::pair<iterator, bool> insert(value_type&& v) {
    return Base::_insert(std::move(v));
  }
  // TODO
//  template<class... Args>
//  inline std::pair<iterator, bool> emplace(Args&&... args) {
//    return Base::_emplace(std::forward<Args>(args)...);
//  }
  template<class Key>
  inline bool erase(Key&& x) {
    return Base::_erase(std::forward<Key>(x));
  }
  inline bool erase(const key_type& x) {
    return Base::_erase(x);
  }
  inline bool erase(key_type&& x) {
    return Base::_erase(std::move(x));
  }
  inline iterator erase(iterator it) {
    return Base::_erase(it);
  }
};

template<typename Base>
using SetInterface = SetInterfaceBase<Base>;

template<typename Base>
class MapInterface : public SetInterfaceBase<Base> {
  using SBase = SetInterfaceBase<Base>;
 public:
  using typename SBase::key_type;
  using typename SBase::mapped_type;
  using typename SBase::value_type;
  using reference = mapped_type&;
  MapInterface() = default;
  template<typename InputIt>
  explicit MapInterface(InputIt begin, InputIt end) : SBase(begin, end) {}
  MapInterface(std::initializer_list<value_type> init) : SBase(init) {}
  inline reference operator[](const key_type& x) {
    // TODO
//    return SBase::try_emplace(x).first->second;
    auto it = SBase::insert({x, mapped_type()}).first;
    return it->second;
  }
  inline reference operator[](key_type&& x) {
    // TODO
//    return SBase::try_emplace(std::move(x)).first->second;
    auto it = SBase::insert({std::move(x), mapped_type()}).first;
    return it->second;
  }
};

template<typename T, typename V, uint8_t W = sizeof(T)*8>
using BinaryTrie = MapInterface<BinaryTrieBase<T, V, W>>;
template<typename T, uint8_t W = sizeof(T)*8>
using BinaryTrieSet = SetInterface<BinaryTrieBase<T, void, W>>;
template<typename T, typename V, uint8_t W = sizeof(T)*8>
using BinaryTrieMap = BinaryTrie<T, V, W>;
