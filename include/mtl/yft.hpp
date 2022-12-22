#pragma once
#include "traits/set_traits.hpp"
#include "binary_trie.hpp"
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
    class XFT = XFastTrieMap<T, TREAP>>
class YFastTrieBase : public traits::AssociativeArrayDefinition<T, M> {
  static_assert(std::is_unsigned<T>::value, "");
  using Def = traits::AssociativeArrayDefinition<T, M>;
 public:
  using typename Def::key_type;
  using typename Def::value_type;
  using treap_type = TREAP;
  using xft_type = XFT;
  static constexpr key_type const kKeyMax = std::numeric_limits<T>::max() >> (sizeof(T)*8-W);
  struct iterator;
 protected:
  xft_type xft_;
  iterator end_;
  size_t size_;
  std::default_random_engine eng_;
  std::uniform_int_distribution<uint8_t> dist_;
  inline void _init() {
    xft_.clear();
    auto xit = xft_.emplace(kKeyMax, treap_type()).first; // TODO
//    auto xit = xft_.insert({kKeyMax, treap_type()}).first;
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
  }
  YFastTrieBase(YFastTrieBase&&) noexcept = default;
  YFastTrieBase& operator=(YFastTrieBase&&) noexcept = default;
  template<typename InputIt>
  explicit YFastTrieBase(InputIt begin, InputIt end) : YFastTrieBase() {
    static_assert(std::is_convertible<typename std::iterator_traits<InputIt>::value_type, value_type>::value, "");
    // TODO: needs test
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
//        xft_.emplace_hint(xft_.end(), x, treap_type(b, e)); // TODO: best
        xft_.emplace(x, treap_type(b, e)); // TODO: better
//        xft_.insert({x, treap_type(b, e)});
        b = e;
      } else {
//        auto xe = xft_.emplace_hint(xft_.end(), kKeyMax, treap_type(b, e)); // TODO: best
        auto xe = xft_.emplace(kKeyMax, treap_type(b, e)).first; // TODO: better
//        auto xe = xft_.insert({kKeyMax, treap_type(b, e)}).first;
        end_ = iterator(&xft_, xe, xe->second.end());
        break;
      }
    }
    size_ = std::distance(begin, end);
  }
  inline size_t size() const { return size_; }
  inline bool empty() const { return size() == 0; }
  inline void clear() {
    _init();
  }
  inline iterator begin() const {
    return iterator(&xft_, xft_.begin(), xft_.begin()->second.begin());
  }
  inline iterator end() const {
    return end_;
  }
 protected:
  template<class Key>
  inline iterator _lower_bound(Key&& key) const {
    key_type x = std::forward<Key>(key);
    auto tit = xft_.lower_bound(x);
    assert(tit != xft_.end());
    auto tres = tit->second.lower_bound(x);
    return iterator(&xft_, tit, tres);
  }
  template<class Key>
  inline iterator _upper_bound(Key&& key) const {
    key_type x = std::forward<Key>(key);
    auto tit = xft_.upper_bound(x);
    if (tit == xft_.end()) [[unlikely]]
      return end();
    assert(tit != xft_.end());
    auto tres = tit->second.upper_bound(x);
    return iterator(&xft_, tit, tres);
  }
  template<class Key>
  inline iterator _find(Key&& key) const {
    key_type x = std::forward<Key>(key);
    auto tit = xft_.lower_bound(x);
    assert(tit != xft_.end());
    auto tres = tit->second.find(x);
    if (tres != tit->second.end())
      return iterator(&xft_, tit, tres);
    else
      return end();
  }
  inline bool _pivot_selected() {
    return dist_(eng_) == 0;
  }
  template<class Value>
  inline std::pair<iterator, bool> _insert(Value&& value) {
    key_type x = Def::key_of(value);
    auto xlb = xft_.lower_bound(x);
    assert(xlb != xft_.end());
    auto& t = xlb->second;
    auto tins = t.insert(std::forward<Value>(value));
    if (tins.second) {
      size_++;
      if (_pivot_selected()) [[unlikely]] {
        auto lt = std::move(t.split(std::next(tins.first)));
//        xlb = xft_.emplace_hint(xlb, x, std::move(lt)); // TODO
        xlb = xft_.emplace(x, std::move(lt)).first;
//        xlb = xft_.insert({x, std::move(lt)}).first;
      }
      return std::make_pair(iterator(&xft_, xlb, tins.first), true);
    }
    return std::make_pair(iterator(&xft_, xlb, tins.first), false);
  }
  template<class Key>
  bool _erase(Key&& key) {
    key_type x = std::forward<Key>(key);
    auto xlb = xft_.lower_bound(x);
    assert(xlb != xft_.end());
    auto& t = xlb->second;
    if (t.erase(x)) {
      size_--;
      auto nxlb = std::next(xlb);
      assert(nxlb != xlb);
      if (xlb->first == x and nxlb != xft_.end()) [[unlikely]] {
        nxlb->second.absorb(&t);
        xft_.erase(xlb);
      }
      return true;
    }
    return false;
  }
 public:
  static inline key_type key_of(iterator it) {
    return Def::key_of(*(it.tit_));
  }
 protected:
  inline iterator _erase(iterator it) {
    if (it == end()) return it;
    auto next = std::next(it);
    auto xlb = it.xit_;
    auto x = key_of(it);
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
 public:
  struct iterator {
    using difference_type = ptrdiff_t;
    using value_type = typename YFastTrieBase::value_type;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::bidirectional_iterator_tag;
    using xiterator = typename xft_type::iterator;
    using titerator = typename treap_type::iterator;
    const xft_type* xft_;
    xiterator xit_;
    titerator tit_;
    iterator(const xft_type* xft, xiterator xit, titerator tit)
      : xft_(xft), xit_(xit), tit_(tit) {}
    inline reference operator*() const {
      return *tit_;
    }
    inline pointer operator->() const {
      return tit_.operator->();
    }
    inline bool operator==(const iterator& rhs) const {
      return xit_ == rhs.xit_ and tit_ == rhs.tit_;
    }
    inline bool operator!=(const iterator& rhs) const {
      return !operator==(rhs);
    }
    inline iterator& operator++() {
      ++tit_;
      if (tit_ == xit_->second.end() and std::next(xit_) != xft_->end()) {
        ++xit_;
        tit_ = xit_->second.begin();
      }
      return *this;
    }
    inline iterator operator++(int) {
      iterator ret = *this;
      operator++();
      return ret;
    }
    inline iterator& operator--() {
      if (tit_ == xit_->second.begin()) {
        --xit_;
        tit_ = std::prev(xit_->second.end());
      } else {
        --tit_;
      }
      return *this;
    }
    inline iterator operator--(int) {
      iterator ret = *this;
      operator--();
      return ret;
    }
  };
};

template<typename T, typename V, uint8_t W = sizeof(T)*8>
using YFastTrie = traits::MapTraits<YFastTrieBase<T, V, W>>;
template<typename T, uint8_t W = sizeof(T)*8>
using YFastTrieSet = traits::SetTraits<YFastTrieBase<T, void, W>>;
template<typename T, typename V, uint8_t W = sizeof(T)*8>
using YFastTrieMap = YFastTrie<T, V, W>;
