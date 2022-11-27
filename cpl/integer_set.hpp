#pragma once
#include "treap.hpp"
#include "bit_manip.hpp"
#include <memory>
#include <iterator>
#include <cassert>
#include <unordered_map>
#include <initializer_list>

template<typename T, unsigned BITS = 64>
class _XFastTrie {
 public:
  using U = uint64_t;
  static constexpr int W = BITS;
  class iterator;
  static constexpr bool kKeyOnly = std::is_same<T, void>::value;
  using element_type = typename std::conditional<kKeyOnly, U, std::pair<U, T>>::type;
 private:
  struct Node;
  using node_ptr = std::shared_ptr<Node>;
  using node_weak = std::weak_ptr<Node>;
  struct Node {
    // common
    uint8_t cmask;
    std::array<node_ptr, 2> child;
    node_weak parent;
    // leaf
    element_type* vptr;
    Node() : cmask(0), child({nullptr, nullptr}), vptr(nullptr) {}
    U key() const {
      if constexpr (kKeyOnly)
        return *vptr;
      else
        return vptr->first;
    }
  };
 private:
  using _hash_table = std::unordered_map<U, node_ptr>;
  node_ptr root_, sentinel_;
  size_t size_;
  std::array<_hash_table, W+1> xmap_;

 public:
  _XFastTrie()
      : root_(std::make_shared<Node>()), sentinel_(std::make_shared<Node>()), size_(0)
  {
    sentinel_->child[0] = sentinel_->child[1] = root_->child[0] = root_->child[1] = sentinel_;
    xmap_[0].emplace(0, root_);
  }
  template<typename It>
  explicit _XFastTrie(It begin, It end) : _XFastTrie() {
    insert(begin, end);
  }
  _XFastTrie(std::initializer_list<element_type> list) : _XFastTrie(list.begin(), list.end()) {}
 private:
  void _del_node(node_ptr u) {
    u->parent.reset();
    for (int i = 0; i < 2; i++) {
      if (u->cmask & (1<<i) and u->child[i] != u) {
        _del_node(u->child[i]);
      }
      u->child[i] = nullptr;
    }
    if (u->vptr) {
      delete u->vptr;
    }
  }
  void _del() {
    if (root_)
      _del_node(root_);
    if (sentinel_)
      sentinel_->child[0] = sentinel_->child[1] = nullptr;
  }
 public:
//  TODO: Optimal copy implementation
  _XFastTrie(const _XFastTrie& r) : _XFastTrie(r.begin(), r.end()) {}
  _XFastTrie& operator=(const _XFastTrie& r) {
    _del();
    root_ = std::make_shared<Node>();
    sentinel_ = std::make_shared<Node>();
    size_ = 0;
    sentinel_->child[0] = sentinel_->child[1] =
        root_->child[0] = root_->child[1] = sentinel_;
    xmap_[0].emplace(0, root_);
    insert(r.begin(), r.end());
    return *this;
  }
  _XFastTrie(_XFastTrie&& r)
    : root_(std::move(r.root_)), sentinel_(std::move(r.sentinel_)),
      size_(std::move(r.size_)), xmap_(std::move(r.xmap_)) {}
  _XFastTrie& operator=(_XFastTrie&& r) noexcept {
    _del();
    root_ = std::move(r.root_);
    sentinel_ = std::move(r.sentinel_);
    size_ = std::move(r.size_);
    xmap_ = std::move(r.xmap_);
    return *this;
  }
  ~_XFastTrie() {
    _del();
  }

  size_t size() const { return size_; }
  bool empty() const { return size() == 0; }
  void clear() {
    *this = _XFastTrie();
  }

  iterator find(U x) const {
    auto it = xmap_[W].find(x);
    return it != xmap_[W].end() ? iterator(it->second) : end();
  }
  size_t count(U x) const { return (size_t) (find(x) != end()); }
  bool contains(U x) const { return (bool) count(x); }
  iterator lower_bound(U x) const {
    int l = 0, r = W+1;
    auto u = root_;
    while (l+1<r) {
      int c = (l+r)/2;
      auto vit = xmap_[c].find(x >> (W-c));
      if (vit != xmap_[c].end()) {
        u = vit->second;
        l = c;
      } else {
        r = c;
      }
    }
    if (l == W) return iterator(u);
    auto next = ((x>>(W-1-l))&1) == 0 ? u->child[0] : u->child[1]->child[1];
    return iterator(next);
  }
  iterator upper_bound(U x) const {
    auto it = lower_bound(x);
    return (it != end() and it.ptr_->key() == x) ? ++it : it;
  }
  iterator successor(U x) const {
    return upper_bound(x);
  }
  iterator predecessor(U x) const {
    auto it = lower_bound(x);
    return it != begin() ? --it : end();
  }

 private:
  U _key_of_elm(element_type& e) const {
    if constexpr (kKeyOnly)
      return e;
    else
      return e.first;
  }
  U _key_of(iterator it) const {
    return _key_of_elm(*(it.ptr_->vptr));
  }
  template<typename... Args>
  U _key_of_elm_args(U x, Args&&...) const { return x; }
  iterator _insert_subtree(int depth, node_ptr u, U x, element_type* eptr) {
    int i = depth;
    int c = (x >> (W-i-1)) & 1;
    auto pred = c == 1 ? u->child[1] : u->child[0]->child[0];
    assert(pred);
    for (; i < W; i++) {
      c = (x >> (W-1-i)) & 1;
      u->cmask |= 1u<<c;
      u->child[c] = std::make_shared<Node>();
      u->child[c]->parent = u;
      u = u->child[c];
      xmap_[i+1].emplace(x>>(W-1-i), u);
    }
    u->vptr = eptr;
    u->child[0] = pred;
    u->child[1] = pred->child[1];
    pred->child[1]->child[0] = u;
    pred->child[1] = u;
    auto v = u->parent.lock();
    for (i = W-1; i >= 0; i--) {
      assert(v);
      c = (x >> (W-1-i)) & 1;
      int ic = c^1;
      if ((v->cmask & (1u<<ic)) == 0
          and (!v->child[ic] or v->child[ic] == sentinel_
              or (ic == 0 ? v->child[ic]->key() > x
                          : v->child[ic]->key() < x)))
        v->child[ic] = u;
      assert(v->child[0] and v->child[1]);
      v = v->parent.lock();
    }
    size_++;
    return iterator(u);
  }
  iterator _insert(U x, element_type* eptr) {
    auto u = root_;
    int i, c;
    for (i = 0; i < W; i++) {
      c = (x >> (W-1-i)) & 1;
      if ((u->cmask & (1u<<c)) == 0) break;
      u = u->child[c];
    }
    if (i == W) return iterator(u);
    return _insert_subtree(i, u, x, eptr);
  }
  iterator _insert_hint(iterator hint, U x, element_type* eptr) {
    // Check hint isn't malicious.
    U lx=0, rx=0;
    if ((hint != begin() and x < (lx = _key_of(std::prev(hint)))) or
        (hint != end() and (rx = _key_of(hint)) < x))
      return _insert(x, eptr);
    // Find longest common path on the trie.
    int d = 0;
    if (hint != begin())
      d = std::max(d, (int) bm::clz(x ^ lx) - (64-W));
    if (hint != end())
      d = std::max(d, (int) bm::clz(x ^ rx) - (64-W));
    if (d == W) return iterator(xmap_[W][x]);
    return _insert_subtree(d, xmap_[d][x >> (W-d)], x, eptr);
  }
 public:
  iterator insert(const element_type& e) {
    return _insert(_key_of_elm(e),
                   new element_type(e));
  }
  iterator insert(element_type&& e) {
    return _insert(_key_of_elm(e),
                   new element_type(std::move(e)));
  }
  template<typename... Args>
  iterator emplace(Args&&... args) {
    return _insert(_key_of_elm_args(args...),
                   new element_type(std::forward<Args>(args)...));
  }
  template<typename... Args>
  iterator emplace_hint(iterator hint, Args&&... args) {
    return _insert_hint(hint,
                        _key_of_elm_args(args...),
                        new element_type(std::forward<Args>(args)...));
  }
  template<typename It>
  void insert(It begin, It end) {
    using traits = std::iterator_traits<It>;
    static_assert(std::is_convertible<typename traits::value_type, element_type>::value, "");
    static_assert(std::is_base_of<std::forward_iterator_tag, typename traits::iterator_category>::value, "");
    for (auto it = begin; it != end; ++it)
      insert(*it);
  }
  void insert(std::initializer_list<element_type> list) {
    insert(list.begin(), list.end());
  }
  iterator erase(iterator it) {
    if (it == end())
      return it;
    auto u = it.ptr_;
    U x = u->key();
    delete u->vptr;
    auto next = u->child[0];
    u->child[0]->child[1] = u->child[1];
    u->child[1]->child[0] = u->child[0];
    auto v = u;
    int i, c;
    for (i = W-1; i >= 0; i--) {
      c = (x >> (W-1-i)) & 1;
      auto p = v->parent.lock();
      v->parent.reset();
      p->cmask ^= 1u<<c;
      p->child[c] = nullptr;
      assert(xmap_[i+1].find(x>>(W-1-i))->second == v);
      xmap_[i+1].erase(x>>(W-1-i));
      v = p;
      if ((p->cmask & (1u<<(c^1))) != 0) break;
    }
    if (i >= 0) {
      c = (x >> (W-1-i)) & 1;
      v->child[c] = u->child[c^1];
      v = v->parent.lock();
      i--;
      for (; i >= 0; i--) {
        c = (x >> (W-1-i)) & 1;
        int ic = c^1;
        if (v->child[ic] == u) {
          v->child[ic] = u->child[c];
        }
        v = v->parent.lock();
      }
    }
    size_--;
    return iterator(next);
  }
  iterator erase(U x) {
    auto it = lower_bound(x);
    if (it.key() != x)
      return it;
    return erase(it);
  }

 public:
  class iterator {
   public:
    using value_type = element_type;
    using pointer = element_type*;
    using reference = const element_type&;
    using difference_type = long long;
    using iterator_category = std::bidirectional_iterator_tag;
   private:
    node_ptr ptr_;
    friend class _XFastTrie;
   public:
    explicit iterator(node_ptr ptr) : ptr_(ptr) {}
    reference operator*() const { return *(ptr_->vptr); }
    pointer operator->() const { return ptr_->vptr; }
    bool operator==(const iterator& r) const { return ptr_ == r.ptr_; }
    bool operator!=(const iterator& r) const { return ptr_ != r.ptr_; }
    iterator& operator++() {
      ptr_ = ptr_->child[1];
      return *this;
    }
    iterator operator++(int) {
      iterator ret = *this;
      ++*this;
      return ret;
    }
    iterator& operator--() {
      ptr_ = ptr_->child[0];
      return *this;
    }
    iterator operator--(int) {
      iterator ret = *this;
      --*this;
      return ret;
    }
    U key() const { return ptr_->key(); }
  };
  iterator begin() const { return iterator(sentinel_->child[1]); }
  iterator end() const { return iterator(sentinel_); }
};
template<typename T, unsigned BITS>
constexpr int _XFastTrie<T, BITS>::W;
template<typename T, unsigned BITS>
constexpr bool _XFastTrie<T, BITS>::kKeyOnly;
template<unsigned BITS = 64>
using XFastTrieSet = _XFastTrie<void, BITS>;
template<typename T, unsigned BITS = 64>
using XFastTrieMap = _XFastTrie<T, BITS>;


template<typename T, unsigned BITS=64>
class YFastTrie {
 public:
  using U = uint64_t;
  static constexpr int W = BITS;
  static constexpr U MAX_KEY = W == 64 ? ~uint64_t(0) : (1ull<<W)-1;
  static constexpr bool kKeyOnly = std::is_same<T, void>::value;
  using element_type = typename std::conditional<kKeyOnly, U, std::pair<U,T>>::type;
  using value_type = typename std::conditional<kKeyOnly, U, T>::type;
  using _set = typename std::conditional<kKeyOnly, TreapSet<U>, TreapMap<U,T>>::type;
  using _set_iterator = typename _set::iterator;
  using _xft = XFastTrieMap<_set, W>;
  class iterator;
 private:
  _xft xft_;
  size_t size_;

  bool _pibot_selected() {
    return rand() % W == 0;
  }
  U _key_of_elm(const element_type& e) const {
    if constexpr (kKeyOnly)
      return e;
    else
      return e.first;
  }
  U _key_of_elm(element_type&& e) const {
    if constexpr (kKeyOnly)
      return e;
    else
      return e.first;
  }
  template<typename... Args>
  U _key_of_elm_args(U x, Args&&...) const {
    return x;
  }
  U _key_of_sub(_set_iterator it) const {
    return _key_of_elm(*it);
  }
  U _key_of(iterator it) const {
    return _key_of_sub(it.sit_);
  }

 public:
  YFastTrie() : size_(0) {
    xft_.emplace(MAX_KEY, _set{});
  }
  template<typename It>
  explicit YFastTrie(It begin, It end) : YFastTrie() {
    insert(begin, end);
  }
  YFastTrie(std::initializer_list<element_type> list) : YFastTrie(list.begin(), list.end()) {}

  size_t size() const { return size_; }
  bool empty() const { return size() == 0; }
  void clear() {
    *this = YFastTrie();
  }

  iterator find(U x) const {
    auto xit = xft_.lower_bound(x);
    assert(xit != xft_.end());
    auto& s = xit->second;
    auto sit = s.find(x);
    return sit != s.end() ? iterator(this, xit, sit) : end();
  }
  size_t count(U x) const { return (size_t) (find(x) != end()); }
  bool contains(U x) const { return (bool) count(x); }
  iterator lower_bound(U x) const {
    auto xit = xft_.lower_bound(x);
    assert(xit != xft_.end());
    auto& s = xit->second;
    auto sit = s.lower_bound(x);
    if (sit == s.end()) {
      assert(std::next(xit) == xft_.end());
      return end();
    }
    return iterator(this, xit, sit);
  }
  iterator upper_bound(U x) const {
    auto it = lower_bound(x);
    return (it != end() and _key_of(it) == x) ? ++it : it;
  }
  iterator successor(U x) const {
    return upper_bound(x);
  }
  iterator predecessor(U x) const {
    auto it = lower_bound(x);
    return it != begin() ? --it : end();
  }

 private:
  template<typename C>
  iterator _insert_before(typename _xft::iterator xit, _set_iterator sit, U x, C elm_constructor) {
    auto& s = xit->second;
    // Expect 'sit = s.emplace_hint(sit, element)'
    sit = elm_constructor(s, sit);
    size_++;
    if (_pibot_selected()) {
      xit = xft_.emplace_hint(xit, x, _set());
      xit->second = s.split(std::next(sit));
    }
    return iterator(this, xit, sit);
  }
  template<typename C>
  iterator _insert(U x, C elm_constructor) {
    auto xit = xft_.lower_bound(x);
    assert(xit != xft_.end());
    auto& s = xit->second;
    auto sit = s.lower_bound(x);
    if (sit == s.end() or _key_of_sub(sit) > x) {
      return _insert_before(xit, sit, x, elm_constructor);
    }
    return iterator(this, xit, sit);
  }
  template<typename C>
  iterator _insert_hint(iterator hint, U x, C elm_constructor) {
    // Check hint isn't malicious.
    if ((hint != begin() and x <= _key_of(std::prev(hint))) or
        (hint != end() and _key_of(hint) <= x))
      return _insert(x, elm_constructor);

    auto xit = hint.xit_;
    assert(xit != xft_.end());
    auto sit = hint.sit_;
    assert(sit == xit->second.end() or _key_of_sub(sit) > x);
    return _insert_before(xit, sit, x, elm_constructor);
  }
 public:
  iterator insert(const element_type& e) {
    return _insert(_key_of_elm(e),
                   [&e]
                   (_set& s, const _set_iterator& it) {
                     return s.emplace_hint(it, e);
                   });
  }
  iterator insert(element_type&& e) {
    return _insert(_key_of_elm(e),
                   [e=std::move(e)](_set& s, const _set_iterator& it) {
                     return s.emplace_hint(it, std::move(e));
                   });
  }
  template<typename... Args>
  iterator emplace(Args&&... args) {
    return _insert(_key_of_elm_args(args...),
                   [&args...](_set& s, const _set_iterator& it) {
                     return s.emplace_hint(it, std::forward<Args>(args)...);
                   });
  }
  template<typename... Args>
  iterator emplace_hint(iterator hint, Args&&... args) {
    return _insert_hint(hint,
                        _key_of_elm_args(args...),
                        [&args...](_set& s, const _set_iterator& it) {
                          return s.emplace_hint(it, std::forward<Args>(args)...);
                        });
  }
  template<typename It>
  void insert(It begin, It end) {
    using traits = std::iterator_traits<It>;
    static_assert(std::is_convertible<typename traits::value_type, element_type>::value, "");
    static_assert(std::is_base_of<std::forward_iterator_tag, typename traits::iterator_category>::value, "");
    for (auto it = begin; it != end; ++it)
      insert(*it);
  }
  void insert(std::initializer_list<element_type> list) {
    insert(list.begin(), list.end());
  }
  iterator erase(iterator it) {
    if (it == end())
      return it;
    U x = _key_of(it);
    auto xit = it.xit_;
    assert(xit != xft_.end());
    auto& s = xit->second;
    auto sit = it.sit_;
    size_--;
    sit = s.erase(sit);
    if (x == xit.key()) {
      if (x != MAX_KEY) {
        if (!s.empty()) {
          auto& snext = std::next(xit)->second;
          sit = snext.absorb(&s);
        }
        xit = xft_.erase(xit);
      } else {
        return end();
      }
    }
    return iterator(this, xit, sit);
  }
  iterator erase(U x) {
    auto it = lower_bound(x);
    return (_key_of(it) == x) ? erase(it) : it;
  }

 public:
  class iterator {
   public:
    using value_type = element_type;
    using pointer = element_type*;
    using reference = const element_type&;
    using difference_type = long long;
    using iterator_category = std::bidirectional_iterator_tag;
    using xiterator = typename _xft::iterator;
    using siterator = _set_iterator;
   private:
    const YFastTrie* yft_;
    xiterator xit_;
    siterator sit_;
    friend class YFastTrie;
   public:
    iterator(const YFastTrie* yft, xiterator xit, siterator sit) : yft_(yft), xit_(xit), sit_(sit) {}
    reference operator*() const { return *sit_; }
    pointer operator->() const { return &*sit_; }
    bool operator==(const iterator& r) { return sit_ == r.sit_; }
    bool operator!=(const iterator& r) { return sit_ != r.sit_; }
    iterator& operator++() {
      if (++sit_ == xit_->second.end()) {
        auto nxt = std::next(xit_);
        if (nxt != yft_->xft_.end()) {
          xit_ = nxt;
          sit_ = xit_->second.begin();
        }
      }
      return *this;
    }
    iterator operator++(int) {
      iterator ret = *this;
      ++this;
      return ret;
    }
    iterator& operator--() {
      if (sit_ == xit_->second.begin()) {
        --xit_;
        sit_ = std::prev(xit_->second.end());
      } else {
        --sit_;
      }
      return *this;
    }
    iterator operator--(int) {
      iterator ret = *this;
      --this;
      return ret;
    }
  };
  iterator begin() const {
    auto xit = xft_.begin();
    return iterator(this, xit, xit->second.begin());
  }
  iterator end() const {
    auto xit = std::prev(xft_.end());
    return iterator(this, xit, xit->second.end());
  }
};
template<typename T, unsigned BITS>
constexpr int YFastTrie<T, BITS>::W;
template<typename T, unsigned BITS>
constexpr typename YFastTrie<T, BITS>::U YFastTrie<T, BITS>::MAX_KEY;
template<typename T, unsigned BITS>
constexpr bool YFastTrie<T, BITS>::kKeyOnly;
template<unsigned BITS=64>
using YFastTrieSet = YFastTrie<void, BITS>;


template<typename T, unsigned BITS=64>
class YFastTrieMAP : public YFastTrie<T, BITS> {
  using _base = YFastTrie<T, BITS>;
 public:
  using typename _base::value_type;
  using typename _base::U;
  using _base::kKeyOnly;
  using reference = value_type&;
  reference operator[](U x) {
    auto it = find(x);
    if (it == _base::end())
      it = emplace(x);
    if constexpr (kKeyOnly)
      return *it;
    else
      return it->second;
  }
};