#pragma once
#include "traits/set_traits.hpp"
#include "treap.hpp"
#include "xft.hpp"
#include <array>
#include <memory>
#include <type_traits>
#include <iterator>
#include <cstdint>
#include <random>
#include <cassert>
#include <vector>
#include <bitset>
#include <algorithm>

template<class T, class M,
    int8_t W = sizeof(T) * 8,
    class TREAP = Treap<T, M>,
    class HashTable = XFT_HASH_TABLE_TYPE(XFT_DEFAULT_HASH_TABLE,T,TREAP,W),
    class XFT = XFastTrieMap<T, TREAP, W, HashTable>>
class YFastTrieBase : public traits::AssociativeArrayDefinition<T, M> {
  static_assert(std::is_unsigned<T>::value, "");
  using Def = traits::AssociativeArrayDefinition<T, M>;
 public:
  using typename Def::key_type;
  using typename Def::value_type;
  using treap_type = TREAP;
  using xft_type = XFT;
  static constexpr key_type const kKeyMax = std::numeric_limits<T>::max() >> (sizeof(T)*8-W);
 protected:
  template<bool> struct iterator_base;
 public:
  using iterator = iterator_base<false>;
  using const_iterator = iterator_base<true>;
 protected:
  xft_type xft_;
  iterator end_;
  size_t size_;
  std::default_random_engine eng_;
  std::uniform_int_distribution<uint8_t> dist_;
  void _init() {
    xft_.clear();
    auto xit = xft_.emplace(kKeyMax, treap_type()).first;
    end_ = iterator(&xft_, xit, xit->second.end());
    size_ = 0;
  }
 public:
  YFastTrieBase()
    : xft_({{kKeyMax, treap_type()}}),
      end_(&xft_, std::prev(xft_.end()), std::prev(xft_.end())->second.end()),
      size_(0),
      dist_(0, W-1) {}
  YFastTrieBase(const YFastTrieBase& rhs)
    : xft_(rhs.xft_),
      end_(&xft_, std::prev(xft_.end()), std::prev(xft_.end())->second.end()),
      size_(rhs.size_),
      dist_(0, W-1) {}
  YFastTrieBase& operator=(const YFastTrieBase& rhs) {
    xft_ = rhs.xft_;
    end_ = iterator(&xft_, std::prev(xft_.end()), std::prev(xft_.end())->second.end());
    size_ = rhs.size_;
    eng_ = rhs.eng_;
    dist_ = rhs.dist_;
    return *this;
  }
  YFastTrieBase(YFastTrieBase&&) noexcept = default;
  YFastTrieBase& operator=(YFastTrieBase&&) noexcept = default;
  template<typename InputIt>
  explicit YFastTrieBase(InputIt begin, InputIt end) : YFastTrieBase() {
    static_assert(std::is_convertible<typename std::iterator_traits<InputIt>::value_type, value_type>::value, "");
    if (begin == end) return;
    if (!std::is_sorted(begin, end, [](auto& l, auto& r) {
      return Def::key_of(l) < Def::key_of(r);
    })) {
      for (auto it = begin; it != end; ++it)
        _insert(*it);
      return;
    }
    xft_.clear();
    auto b = begin;
    while (b != end) {
      auto e = std::next(b);
      key_type px = Def::key_of(*b);
      while (e != end and (px == Def::key_of(*e) or !_pivot_selected())) {
        px = Def::key_of(*(e++));
      }
      if (e != end) {
        key_type x = Def::key_of(*e);
        ++e;
        xft_.emplace_hint(xft_.end(), x, treap_type(b, e));
        b = e;
      } else {
        auto xe = xft_.emplace_hint(xft_.end(), kKeyMax, treap_type(b, e));
        end_ = iterator(&xft_, xe, xe->second.end());
        break;
      }
    }
    size_ = std::distance(begin, end);
  }
  size_t size() const { return size_; }
  bool empty() const { return size() == 0; }
  void clear() {
    _init();
  }
  iterator begin() const {
    return make_raw_iterator(&xft_, xft_.begin(), xft_.begin()->second.begin());
  }
  iterator end() const {
    return end_;
  }
 protected:
  template<class Key>
  iterator _lower_bound(const Key& key) const {
    key_type x = key;
    auto tit = xft_.lower_bound(x);
    assert(tit != xft_.end());
    auto tres = tit->second.lower_bound(x);
    return make_raw_iterator(&xft_, tit, tres);
  }
  template<class Key>
  iterator _upper_bound(const Key& key) const {
    key_type x = key;
    auto tit = xft_.upper_bound(x);
    if (tit == xft_.end()) [[unlikely]]
      return end();
    assert(tit != xft_.end());
    auto tres = tit->second.upper_bound(x);
    return make_raw_iterator(&xft_, tit, tres);
  }
  template<class Key>
  iterator _find(const Key& key) const {
    key_type x = key;
    auto tit = xft_.lower_bound(x);
    assert(tit != xft_.end());
    auto tres = tit->second.find(x);
    if (tres != tit->second.end())
      return make_raw_iterator(&xft_, tit, tres);
    else
      return end();
  }
  bool _pivot_selected() {
    return dist_(eng_) == 0;
  }
  iterator activate_new_treap_node(const key_type& x,
                                   typename xft_type::iterator xlb,
                                   typename treap_type::iterator new_tit) {
    size_++;
    if (_pivot_selected()) [[unlikely]] {
      auto lt = std::move(xlb->second.split(std::next(new_tit)));
      xlb = xft_.emplace_hint(xlb, x, std::move(lt));
    }
    return iterator(&xft_, xlb, new_tit);
  }
  template<class Value>
  std::pair<iterator, bool> _insert(Value&& value) {
    key_type x = Def::key_of(value);
    auto xlb = xft_.lower_bound(x);
    assert(xlb != xft_.end());
    auto& t = xlb->second;
    auto tins = t.insert(std::forward<Value>(value));
    if (tins.second) {
      return std::make_pair(activate_new_treap_node(x, xlb, tins.first), true);
    }
    return std::make_pair(iterator(&xft_, xlb, tins.first), false);
  }
  template<class Value>
  iterator _emplace_hint_unique(const_iterator hint, Value&& value) {
    assert(hint == end() || Def::key_of(value) < hint->first);
    auto xit = hint.xit_;
    auto tins = xit->second.emplace_hint(hint.tit_, std::forward<Value>(value));
    return activate_new_treap_node(tins->first, xit, tins);
  }
  template<class Value>
  iterator _emplace_hint(const_iterator hint, Value&& value) {
    key_type x = Def::key_of(value);
    if (hint != end() and x == hint->first) {
      return hint;
    }
    return _emplace_hint_unique(hint, std::forward<Value>(value));
  }
  bool _erase(const key_type& key) {
    auto xlb = xft_.lower_bound(key);
    assert(xlb != xft_.end());
    auto& t = xlb->second;
    if (t.erase(key)) {
      size_--;
      auto nxlb = std::next(xlb);
      assert(nxlb != xlb);
      if (xlb->first == key and nxlb != xft_.end()) [[unlikely]] {
        nxlb->second.absorb(&t);
        xft_.erase(xlb);
      }
      return true;
    }
    return false;
  }
  iterator _erase(const_iterator it) {
    if (it == end()) return it;
    auto next = std::next(it);
    auto xlb = it.xit_;
    auto x = Def::key_of(*it);
    auto* t = &xlb->second;
    t->erase(it.tit_);
    size_--;
    if (xlb->first == x and xlb != std::prev(xft_.end())) {
      auto& rt = std::next(xlb)->second;
      rt.absorb(t);
      xft_.erase(xlb);
    }
    return next;
  }
 protected:
  template<bool Const>
  struct iterator_base {
    using difference_type = ptrdiff_t;
    using value_type = typename YFastTrieBase::value_type;
    using pointer = typename std::conditional<Const,
        const value_type*,
        value_type*>::type;
    using reference = typename std::conditional<Const,
        const value_type&,
        value_type&>::type;
    using iterator_category = std::bidirectional_iterator_tag;
    using xft_pointer = xft_type*;
    using xiterator = typename xft_type::iterator;
    using titerator = typename treap_type::iterator;
    xft_pointer xft_;
    xiterator xit_;
    titerator tit_;
    iterator_base(xft_pointer xft, xiterator xit, titerator tit) : 
        xft_(xft), xit_(xit), tit_(tit) {}
    template<bool C>
    iterator_base(const iterator_base<C>& rhs) : 
        xft_(rhs.xft_), xit_(rhs.xit_), tit_(rhs.tit_) {}
    template<bool C>
    iterator_base& operator=(const iterator_base<C>& rhs) {
      xft_ = rhs.xft_;
      xit_ = rhs.xit_;
      tit_ = rhs.tit_;
      return *this;
    }
    template<bool C>
    iterator_base(iterator_base<C>&& rhs) : 
        xft_(std::move(rhs.xft_)), 
        xit_(std::move(rhs.xit_)), 
        tit_(std::move(rhs.tit_)) {}
    template<bool C>
    iterator_base& operator=(iterator_base<C>&& rhs) {
      xft_ = std::move(rhs.xft_);
      xit_ = std::move(rhs.xit_);
      tit_ = std::move(rhs.tit_);
      return *this;
    }
    reference operator*() const {
      return *tit_;
    }
    pointer operator->() const {
      return tit_.operator->();
    }
    template<bool C>
    bool operator==(const iterator_base<C>& rhs) const {
      return xit_ == rhs.xit_ and tit_ == rhs.tit_;
    }
    template<bool C>
    bool operator!=(const iterator_base<C>& rhs) const {
      return !operator==(rhs);
    }
    iterator_base& operator++() {
      ++tit_;
      if (tit_ == xit_->second.end() and std::next(xit_) != xft_->end()) {
        ++xit_;
        tit_ = xit_->second.begin();
      }
      return *this;
    }
    iterator_base operator++(int) {
      iterator_base ret = *this;
      operator++();
      return ret;
    }
    iterator_base& operator--() {
      if (tit_ == xit_->second.begin()) {
        --xit_;
        tit_ = std::prev(xit_->second.end());
      } else {
        --tit_;
      }
      return *this;
    }
    iterator_base operator--(int) {
      iterator_base ret = *this;
      operator--();
      return ret;
    }
  };
 protected:
  using xft_pointer = xft_type*;
  using xft_iterator = typename xft_type::iterator;
  using treap_iterator = typename treap_type::iterator;
  using const_xft_pointer = const xft_type*;
  using const_xft_iterator = typename xft_type::const_iterator;
  using const_treap_iterator = typename treap_type::const_iterator;
  static iterator make_raw_iterator(const_xft_pointer xft,
                                    const_xft_iterator xit,
                                    const_treap_iterator tit) {
    return iterator(const_cast<xft_pointer>(xft), xit, tit);
  }
};

template<typename Key, typename T, uint8_t W = sizeof(Key)*8>
using YFastTrie = traits::MapTraits<YFastTrieBase<Key, T, W>>;
template<typename T, uint8_t W = sizeof(T)*8>
using YFastTrieSet = traits::SetTraits<YFastTrieBase<T, void, W>>;
template<typename Key, typename T, uint8_t W = sizeof(Key)*8>
using YFastTrieMap = YFastTrie<Key, T, W>;
