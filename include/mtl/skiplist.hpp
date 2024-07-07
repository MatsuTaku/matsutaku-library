#pragma once
#include "bit_manip.hpp"
#include <memory>
#include <vector>
#include <random>
#include <cassert>
#include <iostream>

template<typename T>
class Skiplist {
 protected:
  struct Node {
   private:
    struct Path {
      int length;
      std::shared_ptr<Node> next;
      Path(int l, std::shared_ptr<Node> n) : length(l), next(n) {}
    };
   public:
    T v;
    std::vector<std::shared_ptr<Node>> next;
    std::vector<int> length;
    Node(T v, int h) : v(v), next(h+1, nullptr), length(h+1, 0) {}
  };

 public:
  static constexpr int kMaxHeight = 32;

  class iterator {
   public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::forward_iterator_tag;
   private:
    std::shared_ptr<Node> ptr_;
   public:
    iterator(std::shared_ptr<Node> ptr) : ptr_(ptr) {}
    T& operator*() {
      return ptr_->v;
    }
    T* operator&() {
      return &(ptr_->v);
    }
    iterator operator++() {
      ptr_ = ptr_->next[0];
      return *this;
    }
    iterator operator++(int) {
      iterator ret = *this;
      operator++();
      return ret;
    }
    bool operator==(iterator r) const {
      return ptr_ == r.ptr_;
    }
    bool operator!=(iterator r) const {
      return ptr_ != r.ptr_;
    }
  };

 protected:
  std::shared_ptr<Node> sentinel_;
  int height_;
  size_t size_;

  std::default_random_engine rnd_gen;
  std::uniform_int_distribution<uint32_t> dist;

 public:
  Skiplist() :
      sentinel_(std::make_shared<Node>(T(), kMaxHeight)),
      height_(0),
      size_(0),
      rnd_gen(std::random_device()()),
      dist(0, (1ull<<kMaxHeight)-1) {}

  template<class InputIt>
  Skiplist(InputIt first, InputIt last) : Skiplist() {
    // TODO: Optimize
    // Howto: Handle vector that end pointers for each height.
    for (auto it = first; it != last; it++)
      insert_at(size(), *it);
  }

  size_t size() const {return size_;}
  bool empty() const {return size() == 0;}

  iterator begin() const {
    return iterator(sentinel_->next[0]);
  }
  iterator end() const {
    return iterator(nullptr);
  }
  
  iterator get_at(size_t i) const {
    if (size() <= i)
      return end();
    auto u = sentinel_;
    i++;
    for (int r = height_; r >= 0; r--) {
      while (u->next[r] and u->length[r] < (int) i) {
        i -= u->length[r];
        u = u->next[r];
      }
      if (u->next[r] and u->length[r] == (int) i)
        return iterator(u->next[r]);
    }
    assert(false);
    return end();
  }
  iterator set_at(int i, T v) {
    auto u = get_at(i);
    if (u != end())
      u->v = v;
    return u;
  }

  int pick_height() {
    return bm::ctz(dist(rnd_gen));
  }

  iterator insert_at(int i, T v) {
    if (i > size())
      return end();
    i++;
    auto u = sentinel_;
    int hw = pick_height();
    if (hw > height_)
      height_ = hw;
    auto w = std::make_shared<Node>(v, hw);
    for (int r = height_; r >= 0; r--) {
      while (u->next[r] and u->length[r] < i) {
        i -= u->length[r];
        u = u->next[r];
      }
      if (r <= hw) {
        w->length[r] = u->next[r] ? u->length[r] - (i-1) : 0;
        u->length[r] = i;
        w->next[r] = u->next[r];
        u->next[r] = w;
      } else if (u->next[r]) {
        u->length[r]++;
      }
    }
    size_++;
    return iterator(w);
  }

  iterator erase_at(int i) {
    if (size() <= i)
      return end();
    auto u = sentinel_;
    i++;
    for (int r = height_; r >= 0; r--) {
      while (u->next[r] and u->length[r] < i) {
        i -= u->length[r];
        u = u->next[r];
      }
      if (u->next[r] and u->length[r] == i) {
        if (u->next[r]->length[r]) {
          u->length[r] = u->length[r] + u->next[r]->length[r] - 1;
        } else {
          u->length[r] = 0;
        }
        u->next[r] = u->next[r]->next[r];
      } else if (u->next[r]) {
        u->length[r]--;
      }
    }
    size_--;
    return iterator(u->next[0]);
  }
};
template<typename T>
constexpr int Skiplist<T>::kMaxHeight;


template<typename T>
class SkiplistSet : public Skiplist<T> {
 private:
  using _base = Skiplist<T>;
  using typename _base::Node;
  using typename _base::iterator;
  std::vector<std::shared_ptr<Node>> stack_;
  std::vector<size_t> idx_;
 public:
  SkiplistSet() : Skiplist<T>(),
                  stack_(_base::kMaxHeight+1, _base::sentinel_),
                  idx_(_base::kMaxHeight+1, 0) {}

  void print_for_debug() const {
    for (int r = _base::height_; r >= 0; r--){
      auto u = _base::sentinel_;
      int l = u->length[r];
      std::cout << "-"<<u->length[r] << "-";
      for (int i = 0; i < l-1; i++)
        std::cout<<"    ";
      u = u->next[r];
      while (u) {
        l = u->length[r];
        std::cout << "|-"<<u->length[r] << "-";
        for (int i = 0; i < l-1; i++)
          std::cout<<"    ";
        u = u->next[r];
      }
      std::cout<<std::endl;
    }
    auto u = _base::sentinel_;
    u = u->next[0];
    std::cout<<" ";
    while (u) {
      std::cout << "  "<<u->v<<' ';
      u = u->next[0];
    }
    std::cout<<std::endl;
  }

  iterator find(const T& v) const {
    auto u = _base::sentinel_;
    for (int r = _base::height_; r >= 0; r--) {
      while (u->next[r] and u->next[r]->v < v)
        u = u->next[r];
      if (u->next[r] and u->next[r]->v == v)
        return iterator(u->next[r]);
    }
    return _base::end();
  }
  size_t count(const T& v) const {
    return size_t(find(v) != _base::end());
  }
  iterator lower_bound(const T& v) const {
    auto u = _base::sentinel_;
    for (int r = _base::height_; r >= 0; r--) {
      while (u->next[r] and u->next[r]->v < v)
        u = u->next[r];
      if (u->next[r] and u->next[r]->v == v)
        return iterator(u->next[r]);
    }
    return iterator(u->next[0]);
  }
  iterator upper_bound(const T& v) const {
    auto u = _base::sentinel_;
    for (int r = _base::height_; r >= 0; r--) {
      while (u->next[r] and u->next[r]->v <= v)
        u = u->next[r];
    }
    return iterator(u->next[0]);
  }
  iterator successor(const T& v) const {
    return upper_bound(v);
  }
  iterator predecessor(const T& v) const {
    auto u = _base::sentinel_;
    for (int r = _base::height_; r >= 0; r--) {
      while (u->next[r] and u->next[r]->v < v)
        u = u->next[r];
    }
    return u != _base::sentinel_ ? iterator(u) : _base::end();
  }

  template<class... Args>
  std::pair<iterator, bool> emplace(Args&&... args) {
    auto u = _base::sentinel_;
    size_t j = 1;
    T v(std::forward<Args>(args)...);
    for (int r = _base::height_; r >= 0; r--) {
      while (u->next[r] and u->next[r]->v < v) {
        j += u->length[r];
        u = u->next[r];
      }
      if (u->next[r] and u->next[r]->v == v)
        return std::make_pair(iterator(u->next[r]), false);
      stack_[r] = u;
      idx_[r] = j-1;
    }
    int hw = _base::pick_height();
    if (_base::height_ < hw) {
      _base::height_ = hw;
    }
    auto w = std::make_shared<Node>(std::move(v), hw);
    for (int r = _base::height_; r >= 0; r--) {
      if (r <= hw) {
        assert(idx_[r] < j);
        if (stack_[r]->next[r]) {
          w->length[r] = stack_[r]->length[r] - (j - idx_[r]) + 1;
        } else {
          w->length[r] = 0;
        }
        stack_[r]->length[r] = j - idx_[r];
        w->next[r] = stack_[r]->next[r];
        stack_[r]->next[r] = w;
      } else if (stack_[r]->next[r]) {
        if (stack_[r]->next[r])
          stack_[r]->length[r]++;
      }
    }
    _base::size_++;
    return std::make_pair(iterator(w), true);
  }
  std::pair<iterator, bool> insert(const T& v) {
    return emplace(v);
  }
  std::pair<iterator, bool> insert(T&& v) {
    return emplace(std::move(v));
  }

  iterator erase(const T& v) {
    bool erased = false;
    auto u = _base::sentinel_;
    int r = _base::height_;
    for (; r >= 0; r--) {
      while (u->next[r] and u->next[r]->v < v)
        u = u->next[r];
      if (u->next[r] and u->next[r]->v == v) {
        erased = true;
        break;
      }
      stack_[r] = u;
    }
    if (erased) {
      _base::size_--;
      for (int i = _base::height_; i > r; i--)
        if (stack_[i]->next[i])
          stack_[i]->length[i]--;
      for(; r >= 0; r--) {
        if (u->next[r] and u->next[r]->v == v) {
          if (u->next[r]->next[r]) {
            u->length[r] = u->length[r] + u->next[r]->length[r] - 1;
          } else {
            u->length[r] = 0;
          }
          u->next[r] = u->next[r]->next[r];
        }
      }
    }
    return iterator(u->next[0]);
  }
};
