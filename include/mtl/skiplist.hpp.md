---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/bit_manip.hpp\"\n#include <cstdint>\n#include\
    \ <cassert>\n\nnamespace bm {\n\ninline constexpr uint64_t popcnt_e8(uint64_t\
    \ x) {\n  x = (x & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);\n  x =\
    \ (x & 0x3333333333333333) + ((x>>2) & 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F)\
    \ + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n  return x;\n}\n// Count 1s\ninline constexpr\
    \ unsigned popcnt(uint64_t x) {\n  return (popcnt_e8(x) * 0x0101010101010101)\
    \ >> 56;\n}\n// Count trailing 0s. ...01101000 -> 3\ninline constexpr unsigned\
    \ ctz(uint64_t x) {\n  return popcnt((x & (-x)) - 1);\n}\ninline constexpr unsigned\
    \ ctz8(uint8_t x) {\n  return x == 0 ? 8 : popcnt_e8((x & (-x)) - 1);\n}\n// [00..0](8bit)\
    \ -> 0, [**..*](not only 0) -> 1\ninline constexpr uint8_t summary(uint64_t x)\
    \ {\n  constexpr uint64_t hmask = 0x8080808080808080ull;\n  constexpr uint64_t\
    \ lmask = 0x7F7F7F7F7F7F7F7Full;\n  auto a = x & hmask;\n  auto b = x & lmask;\n\
    \  b = hmask - b;\n  b = ~b;\n  auto c = (a | b) & hmask;\n  c *= 0x0002040810204081ull;\n\
    \  return uint8_t(c >> 56);\n}\n// Extract target area of bits\ninline constexpr\
    \ uint64_t bextr(uint64_t x, unsigned start, unsigned len) {\n  uint64_t mask\
    \ = len < 64 ? (1ull<<len)-1 : 0xFFFFFFFFFFFFFFFFull;\n  return (x >> start) &\
    \ mask;\n}\n// 00101101 -> 00111111 -count_1s-> 6\ninline constexpr unsigned log2p1(uint8_t\
    \ x) {\n  if (x & 0x80)\n    return 8;\n  uint64_t p = uint64_t(x) * 0x0101010101010101ull;\n\
    \  p -= 0x8040201008040201ull;\n  p = ~p & 0x8080808080808080ull;\n  p = (p >>\
    \ 7) * 0x0101010101010101ull;\n  p >>= 56;\n  return p;\n}\n// 00101100 -mask_mssb->\
    \ 00100000 -to_index-> 5\ninline constexpr unsigned mssb8(uint8_t x) {\n  assert(x\
    \ != 0);\n  return log2p1(x) - 1;\n}\n// 00101100 -mask_lssb-> 00000100 -to_index->\
    \ 2\ninline constexpr unsigned lssb8(uint8_t x) {\n  assert(x != 0);\n  return\
    \ popcnt_e8((x & -x) - 1);\n}\n// Count leading 0s. 00001011... -> 4\ninline constexpr\
    \ unsigned clz(uint64_t x) {\n  if (x == 0)\n    return 64;\n  auto i = mssb8(summary(x));\n\
    \  auto j = mssb8(bextr(x, 8 * i, 8));\n  return 63 - (8 * i + j);\n}\ninline\
    \ constexpr unsigned clz8(uint8_t x) {\n  return x == 0 ? 8 : 7 - mssb8(x);\n\
    }\ninline constexpr uint64_t bit_reverse(uint64_t x) {\n  x = ((x & 0x00000000FFFFFFFF)\
    \ << 32) | ((x & 0xFFFFFFFF00000000) >> 32);\n  x = ((x & 0x0000FFFF0000FFFF)\
    \ << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);\n  x = ((x & 0x00FF00FF00FF00FF)\
    \ << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);\n  x = ((x & 0x0F0F0F0F0F0F0F0F) <<\
    \ 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);\n  x = ((x & 0x3333333333333333) << 2)\
    \ | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);\n  x = ((x & 0x5555555555555555) << 1) |\
    \ ((x & 0xAAAAAAAAAAAAAAAA) >> 1);\n  return x;\n}\n\n} // namespace bm\n#line\
    \ 3 \"include/mtl/skiplist.hpp\"\n#include <memory>\r\n#include <vector>\r\n#include\
    \ <random>\r\n#line 7 \"include/mtl/skiplist.hpp\"\n#include <iostream>\r\n\r\n\
    template<typename T>\r\nclass Skiplist {\r\n protected:\r\n  struct Node {\r\n\
    \   private:\r\n    struct Path {\r\n      int length;\r\n      std::shared_ptr<Node>\
    \ next;\r\n      Path(int l, std::shared_ptr<Node> n) : length(l), next(n) {}\r\
    \n    };\r\n   public:\r\n    T v;\r\n    std::vector<std::shared_ptr<Node>> next;\r\
    \n    std::vector<int> length;\r\n    Node(T v, int h) : v(v), next(h+1, nullptr),\
    \ length(h+1, 0) {}\r\n  };\r\n\r\n public:\r\n  static constexpr int kMaxHeight\
    \ = 32;\r\n\r\n  class iterator {\r\n   public:\r\n    using value_type = T;\r\
    \n    using pointer = T*;\r\n    using reference = T&;\r\n    using iterator_category\
    \ = std::forward_iterator_tag;\r\n   private:\r\n    std::shared_ptr<Node> ptr_;\r\
    \n   public:\r\n    iterator(std::shared_ptr<Node> ptr) : ptr_(ptr) {}\r\n   \
    \ T& operator*() {\r\n      return ptr_->v;\r\n    }\r\n    T* operator&() {\r\
    \n      return &(ptr_->v);\r\n    }\r\n    iterator operator++() {\r\n      ptr_\
    \ = ptr_->next[0];\r\n      return *this;\r\n    }\r\n    iterator operator++(int)\
    \ {\r\n      iterator ret = *this;\r\n      operator++();\r\n      return ret;\r\
    \n    }\r\n    bool operator==(iterator r) const {\r\n      return ptr_ == r.ptr_;\r\
    \n    }\r\n    bool operator!=(iterator r) const {\r\n      return ptr_ != r.ptr_;\r\
    \n    }\r\n  };\r\n\r\n protected:\r\n  std::shared_ptr<Node> sentinel_;\r\n \
    \ int height_;\r\n  size_t size_;\r\n\r\n  std::default_random_engine rnd_gen;\r\
    \n  std::uniform_int_distribution<uint32_t> dist;\r\n\r\n public:\r\n  Skiplist()\
    \ :\r\n      sentinel_(std::make_shared<Node>(T(), kMaxHeight)),\r\n      height_(0),\r\
    \n      size_(0),\r\n      rnd_gen(std::random_device()()),\r\n      dist(0, (1ull<<kMaxHeight)-1)\
    \ {}\r\n\r\n  size_t size() const {return size_;}\r\n  bool empty() const {return\
    \ size() == 0;}\r\n\r\n  iterator begin() const {\r\n    return iterator(sentinel_->next[0]);\r\
    \n  }\r\n  iterator end() const {\r\n    return iterator(nullptr);\r\n  }\r\n\
    \  \r\n  iterator get_at(size_t i) const {\r\n    if (size() <= i)\r\n      return\
    \ end();\r\n    auto u = sentinel_;\r\n    i++;\r\n    for (int r = height_; r\
    \ >= 0; r--) {\r\n      while (u->next[r] and u->length[r] < (int) i) {\r\n  \
    \      i -= u->length[r];\r\n        u = u->next[r];\r\n      }\r\n      if (u->next[r]\
    \ and u->length[r] == (int) i)\r\n        return iterator(u->next[r]);\r\n   \
    \ }\r\n    assert(false);\r\n    return end();\r\n  }\r\n  iterator set_at(int\
    \ i, T v) {\r\n    auto u = get_at(i);\r\n    if (u != end())\r\n      u->v =\
    \ v;\r\n    return u;\r\n  }\r\n\r\n  int pick_height() {\r\n    return bm::ctz(dist(rnd_gen));\r\
    \n  }\r\n\r\n  iterator insert_at(int i, T v) {\r\n    if (i > size())\r\n   \
    \   return end();\r\n    i++;\r\n    auto u = sentinel_;\r\n    int hw = pick_height();\r\
    \n    auto w = std::make_shared<Node>(v, hw);\r\n    for (int r = height_; r >=\
    \ 0; r--) {\r\n      while (u->next[r] and u->length[r] < i) {\r\n        i -=\
    \ u->length[r];\r\n        u = u->next[r];\r\n      }\r\n      if (r <= hw) {\r\
    \n        w->length[r] = u->next[r] ? u->length[r] - (i - 1) : 0;\r\n        u->length[r]\
    \ = i;\r\n        w->next[r] = u->next[r];\r\n        u->next[r] = w;\r\n    \
    \  } else if (u->next[r]) {\r\n        u->length[r]++;\r\n      }\r\n    }\r\n\
    \    size_++;\r\n    return iterator(w);\r\n  }\r\n\r\n  iterator erase_at(int\
    \ i) {\r\n    if (size() <= i)\r\n      return end();\r\n    auto u = sentinel_;\r\
    \n    i++;\r\n    for (int r = height_; r >= 0; r--) {\r\n      while (u->next[r]\
    \ and u->length[r] < i) {\r\n        i -= u->length[r];\r\n        u = u->next[r];\r\
    \n      }\r\n      if (u->next[r] and u->length[r] == i) {\r\n        if (u->next[r]->length[r])\
    \ {\r\n          u->length[r] = u->length[r] + u->next[r]->length[r] - 1;\r\n\
    \        } else {\r\n          u->length[r] = 0;\r\n        }\r\n        u->next[r]\
    \ = u->next[r]->next[r];\r\n      } else if (u->next[r]) {\r\n        u->length[r]--;\r\
    \n      }\r\n    }\r\n    size_--;\r\n    return iterator(u->next[0]);\r\n  }\r\
    \n};\r\ntemplate<typename T>\r\nconstexpr int Skiplist<T>::kMaxHeight;\r\n\r\n\
    \r\ntemplate<typename T>\r\nclass SkiplistSet : public Skiplist<T> {\r\n private:\r\
    \n  using _base = Skiplist<T>;\r\n  using typename _base::Node;\r\n  using typename\
    \ _base::iterator;\r\n  std::vector<std::shared_ptr<Node>> stack_;\r\n  std::vector<int>\
    \ idx_;\r\n public:\r\n  SkiplistSet() : Skiplist<T>(),\r\n                  stack_(_base::kMaxHeight+1,\
    \ _base::sentinel_),\r\n                  idx_(_base::kMaxHeight+1, -1) {}\r\n\
    \r\n  void print_for_debug() const {\r\n    for (int r = _base::height_; r >=\
    \ 0; r--){\r\n      auto u = _base::sentinel_;\r\n      int l = u->length[r];\r\
    \n      std::cout << \"-\"<<u->length[r] << \"-\";\r\n      for (int i = 0; i\
    \ < l-1; i++)\r\n        std::cout<<\"    \";\r\n      u = u->next[r];\r\n   \
    \   while (u) {\r\n        l = u->length[r];\r\n        std::cout << \"|-\"<<u->length[r]\
    \ << \"-\";\r\n        for (int i = 0; i < l-1; i++)\r\n          std::cout<<\"\
    \    \";\r\n        u = u->next[r];\r\n      }\r\n      std::cout<<std::endl;\r\
    \n    }\r\n    auto u = _base::sentinel_;\r\n    u = u->next[0];\r\n    std::cout<<\"\
    \ \";\r\n    while (u) {\r\n      std::cout << \"  \"<<u->v<<' ';\r\n      u =\
    \ u->next[0];\r\n    }\r\n    std::cout<<std::endl;\r\n  }\r\n\r\n  iterator find(const\
    \ T& v) const {\r\n    auto u = _base::sentinel_;\r\n    for (int r = _base::height_;\
    \ r >= 0; r--) {\r\n      while (u->next[r] and u->next[r]->v < v)\r\n       \
    \ u = u->next[r];\r\n      if (u->next[r] and u->next[r]->v == v)\r\n        return\
    \ iterator(u->next[r]);\r\n    }\r\n    return _base::end();\r\n  }\r\n  size_t\
    \ count(const T& v) const {\r\n    return size_t(find(v) != _base::end());\r\n\
    \  }\r\n  iterator lower_bound(const T& v) const {\r\n    auto u = _base::sentinel_;\r\
    \n    for (int r = _base::height_; r >= 0; r--) {\r\n      while (u->next[r] and\
    \ u->next[r]->v < v)\r\n        u = u->next[r];\r\n      if (u->next[r] and u->next[r]->v\
    \ == v)\r\n        return iterator(u->next[r]);\r\n    }\r\n    return iterator(u->next[0]);\r\
    \n  }\r\n  iterator upper_bound(const T& v) const {\r\n    auto u = _base::sentinel_;\r\
    \n    for (int r = _base::height_; r >= 0; r--) {\r\n      while (u->next[r] and\
    \ u->next[r]->v <= v)\r\n        u = u->next[r];\r\n    }\r\n    return iterator(u->next[0]);\r\
    \n  }\r\n  iterator successor(const T& v) const {\r\n    return upper_bound(v);\r\
    \n  }\r\n  iterator predecessor(const T& v) const {\r\n    auto u = _base::sentinel_;\r\
    \n    for (int r = _base::height_; r >= 0; r--) {\r\n      while (u->next[r] and\
    \ u->next[r]->v < v)\r\n        u = u->next[r];\r\n    }\r\n    return u != _base::sentinel_\
    \ ? iterator(u) : _base::end();\r\n  }\r\n\r\n  iterator insert(const T& v) {\r\
    \n    auto u = _base::sentinel_;\r\n    int j = -1;\r\n    for (int r = _base::height_;\
    \ r >= 0; r--) {\r\n      while (u->next[r] and u->next[r]->v < v) {\r\n     \
    \   j += u->length[r];\r\n        u = u->next[r];\r\n      }\r\n      if (u->next[r]\
    \ and u->next[r]->v == v)\r\n        return iterator(u->next[r]);\r\n      stack_[r]\
    \ = u;\r\n      idx_[r] = j;\r\n    }\r\n    int hw = _base::pick_height();\r\n\
    \    if (hw > _base::height_) {\r\n      _base::height_ = hw;\r\n    }\r\n   \
    \ auto w = std::make_shared<Node>(v, hw);\r\n    if (hw > _base::height_) {\r\n\
    \      _base::height_ = hw;\r\n    }\r\n    for (int r = _base::height_; r >=\
    \ 0; r--) {\r\n      if (r <= hw) {\r\n        if (stack_[r]->next[r]) {\r\n \
    \         w->length[r] = stack_[r]->length[r] - (j - idx_[r]);\r\n        } else\
    \ {\r\n          w->length[r] = 0;\r\n        }\r\n        stack_[r]->length[r]\
    \ = j - idx_[r] + 1;\r\n        w->next[r] = stack_[r]->next[r];\r\n        stack_[r]->next[r]\
    \ = w;\r\n      } else if (stack_[r]->next[r]) {\r\n        stack_[r]->length[r]++;\r\
    \n      }\r\n    }\r\n    _base::size_++;\r\n    return iterator(w);\r\n  }\r\n\
    \r\n  iterator erase(const T& v) {\r\n    bool erased = false;\r\n    auto u =\
    \ _base::sentinel_;\r\n    for (int r = _base::height_; r >= 0; r--) {\r\n   \
    \   while (u->next[r] and u->next[r]->v < v)\r\n        u = u->next[r];\r\n  \
    \    if (u->next[r] and u->next[r]->v == v) {\r\n        erased = true;\r\n  \
    \      if (u->next[r]->next[r]) {\r\n          u->length[r] = u->length[r] + u->next[r]->length[r]\
    \ - 1;\r\n        } else {\r\n          u->length[r] = 0;\r\n        }\r\n   \
    \     u->next[r] = u->next[r]->next[r];\r\n      } else if (u->next[r]) {\r\n\
    \        u->length[r]--;\r\n      }\r\n    }\r\n    if (erased)\r\n      _base::size_--;\r\
    \n    return iterator(u->next[0]);\r\n  }\r\n};\r\n"
  code: "#pragma once\r\n#include \"bit_manip.hpp\"\r\n#include <memory>\r\n#include\
    \ <vector>\r\n#include <random>\r\n#include <cassert>\r\n#include <iostream>\r\
    \n\r\ntemplate<typename T>\r\nclass Skiplist {\r\n protected:\r\n  struct Node\
    \ {\r\n   private:\r\n    struct Path {\r\n      int length;\r\n      std::shared_ptr<Node>\
    \ next;\r\n      Path(int l, std::shared_ptr<Node> n) : length(l), next(n) {}\r\
    \n    };\r\n   public:\r\n    T v;\r\n    std::vector<std::shared_ptr<Node>> next;\r\
    \n    std::vector<int> length;\r\n    Node(T v, int h) : v(v), next(h+1, nullptr),\
    \ length(h+1, 0) {}\r\n  };\r\n\r\n public:\r\n  static constexpr int kMaxHeight\
    \ = 32;\r\n\r\n  class iterator {\r\n   public:\r\n    using value_type = T;\r\
    \n    using pointer = T*;\r\n    using reference = T&;\r\n    using iterator_category\
    \ = std::forward_iterator_tag;\r\n   private:\r\n    std::shared_ptr<Node> ptr_;\r\
    \n   public:\r\n    iterator(std::shared_ptr<Node> ptr) : ptr_(ptr) {}\r\n   \
    \ T& operator*() {\r\n      return ptr_->v;\r\n    }\r\n    T* operator&() {\r\
    \n      return &(ptr_->v);\r\n    }\r\n    iterator operator++() {\r\n      ptr_\
    \ = ptr_->next[0];\r\n      return *this;\r\n    }\r\n    iterator operator++(int)\
    \ {\r\n      iterator ret = *this;\r\n      operator++();\r\n      return ret;\r\
    \n    }\r\n    bool operator==(iterator r) const {\r\n      return ptr_ == r.ptr_;\r\
    \n    }\r\n    bool operator!=(iterator r) const {\r\n      return ptr_ != r.ptr_;\r\
    \n    }\r\n  };\r\n\r\n protected:\r\n  std::shared_ptr<Node> sentinel_;\r\n \
    \ int height_;\r\n  size_t size_;\r\n\r\n  std::default_random_engine rnd_gen;\r\
    \n  std::uniform_int_distribution<uint32_t> dist;\r\n\r\n public:\r\n  Skiplist()\
    \ :\r\n      sentinel_(std::make_shared<Node>(T(), kMaxHeight)),\r\n      height_(0),\r\
    \n      size_(0),\r\n      rnd_gen(std::random_device()()),\r\n      dist(0, (1ull<<kMaxHeight)-1)\
    \ {}\r\n\r\n  size_t size() const {return size_;}\r\n  bool empty() const {return\
    \ size() == 0;}\r\n\r\n  iterator begin() const {\r\n    return iterator(sentinel_->next[0]);\r\
    \n  }\r\n  iterator end() const {\r\n    return iterator(nullptr);\r\n  }\r\n\
    \  \r\n  iterator get_at(size_t i) const {\r\n    if (size() <= i)\r\n      return\
    \ end();\r\n    auto u = sentinel_;\r\n    i++;\r\n    for (int r = height_; r\
    \ >= 0; r--) {\r\n      while (u->next[r] and u->length[r] < (int) i) {\r\n  \
    \      i -= u->length[r];\r\n        u = u->next[r];\r\n      }\r\n      if (u->next[r]\
    \ and u->length[r] == (int) i)\r\n        return iterator(u->next[r]);\r\n   \
    \ }\r\n    assert(false);\r\n    return end();\r\n  }\r\n  iterator set_at(int\
    \ i, T v) {\r\n    auto u = get_at(i);\r\n    if (u != end())\r\n      u->v =\
    \ v;\r\n    return u;\r\n  }\r\n\r\n  int pick_height() {\r\n    return bm::ctz(dist(rnd_gen));\r\
    \n  }\r\n\r\n  iterator insert_at(int i, T v) {\r\n    if (i > size())\r\n   \
    \   return end();\r\n    i++;\r\n    auto u = sentinel_;\r\n    int hw = pick_height();\r\
    \n    auto w = std::make_shared<Node>(v, hw);\r\n    for (int r = height_; r >=\
    \ 0; r--) {\r\n      while (u->next[r] and u->length[r] < i) {\r\n        i -=\
    \ u->length[r];\r\n        u = u->next[r];\r\n      }\r\n      if (r <= hw) {\r\
    \n        w->length[r] = u->next[r] ? u->length[r] - (i - 1) : 0;\r\n        u->length[r]\
    \ = i;\r\n        w->next[r] = u->next[r];\r\n        u->next[r] = w;\r\n    \
    \  } else if (u->next[r]) {\r\n        u->length[r]++;\r\n      }\r\n    }\r\n\
    \    size_++;\r\n    return iterator(w);\r\n  }\r\n\r\n  iterator erase_at(int\
    \ i) {\r\n    if (size() <= i)\r\n      return end();\r\n    auto u = sentinel_;\r\
    \n    i++;\r\n    for (int r = height_; r >= 0; r--) {\r\n      while (u->next[r]\
    \ and u->length[r] < i) {\r\n        i -= u->length[r];\r\n        u = u->next[r];\r\
    \n      }\r\n      if (u->next[r] and u->length[r] == i) {\r\n        if (u->next[r]->length[r])\
    \ {\r\n          u->length[r] = u->length[r] + u->next[r]->length[r] - 1;\r\n\
    \        } else {\r\n          u->length[r] = 0;\r\n        }\r\n        u->next[r]\
    \ = u->next[r]->next[r];\r\n      } else if (u->next[r]) {\r\n        u->length[r]--;\r\
    \n      }\r\n    }\r\n    size_--;\r\n    return iterator(u->next[0]);\r\n  }\r\
    \n};\r\ntemplate<typename T>\r\nconstexpr int Skiplist<T>::kMaxHeight;\r\n\r\n\
    \r\ntemplate<typename T>\r\nclass SkiplistSet : public Skiplist<T> {\r\n private:\r\
    \n  using _base = Skiplist<T>;\r\n  using typename _base::Node;\r\n  using typename\
    \ _base::iterator;\r\n  std::vector<std::shared_ptr<Node>> stack_;\r\n  std::vector<int>\
    \ idx_;\r\n public:\r\n  SkiplistSet() : Skiplist<T>(),\r\n                  stack_(_base::kMaxHeight+1,\
    \ _base::sentinel_),\r\n                  idx_(_base::kMaxHeight+1, -1) {}\r\n\
    \r\n  void print_for_debug() const {\r\n    for (int r = _base::height_; r >=\
    \ 0; r--){\r\n      auto u = _base::sentinel_;\r\n      int l = u->length[r];\r\
    \n      std::cout << \"-\"<<u->length[r] << \"-\";\r\n      for (int i = 0; i\
    \ < l-1; i++)\r\n        std::cout<<\"    \";\r\n      u = u->next[r];\r\n   \
    \   while (u) {\r\n        l = u->length[r];\r\n        std::cout << \"|-\"<<u->length[r]\
    \ << \"-\";\r\n        for (int i = 0; i < l-1; i++)\r\n          std::cout<<\"\
    \    \";\r\n        u = u->next[r];\r\n      }\r\n      std::cout<<std::endl;\r\
    \n    }\r\n    auto u = _base::sentinel_;\r\n    u = u->next[0];\r\n    std::cout<<\"\
    \ \";\r\n    while (u) {\r\n      std::cout << \"  \"<<u->v<<' ';\r\n      u =\
    \ u->next[0];\r\n    }\r\n    std::cout<<std::endl;\r\n  }\r\n\r\n  iterator find(const\
    \ T& v) const {\r\n    auto u = _base::sentinel_;\r\n    for (int r = _base::height_;\
    \ r >= 0; r--) {\r\n      while (u->next[r] and u->next[r]->v < v)\r\n       \
    \ u = u->next[r];\r\n      if (u->next[r] and u->next[r]->v == v)\r\n        return\
    \ iterator(u->next[r]);\r\n    }\r\n    return _base::end();\r\n  }\r\n  size_t\
    \ count(const T& v) const {\r\n    return size_t(find(v) != _base::end());\r\n\
    \  }\r\n  iterator lower_bound(const T& v) const {\r\n    auto u = _base::sentinel_;\r\
    \n    for (int r = _base::height_; r >= 0; r--) {\r\n      while (u->next[r] and\
    \ u->next[r]->v < v)\r\n        u = u->next[r];\r\n      if (u->next[r] and u->next[r]->v\
    \ == v)\r\n        return iterator(u->next[r]);\r\n    }\r\n    return iterator(u->next[0]);\r\
    \n  }\r\n  iterator upper_bound(const T& v) const {\r\n    auto u = _base::sentinel_;\r\
    \n    for (int r = _base::height_; r >= 0; r--) {\r\n      while (u->next[r] and\
    \ u->next[r]->v <= v)\r\n        u = u->next[r];\r\n    }\r\n    return iterator(u->next[0]);\r\
    \n  }\r\n  iterator successor(const T& v) const {\r\n    return upper_bound(v);\r\
    \n  }\r\n  iterator predecessor(const T& v) const {\r\n    auto u = _base::sentinel_;\r\
    \n    for (int r = _base::height_; r >= 0; r--) {\r\n      while (u->next[r] and\
    \ u->next[r]->v < v)\r\n        u = u->next[r];\r\n    }\r\n    return u != _base::sentinel_\
    \ ? iterator(u) : _base::end();\r\n  }\r\n\r\n  iterator insert(const T& v) {\r\
    \n    auto u = _base::sentinel_;\r\n    int j = -1;\r\n    for (int r = _base::height_;\
    \ r >= 0; r--) {\r\n      while (u->next[r] and u->next[r]->v < v) {\r\n     \
    \   j += u->length[r];\r\n        u = u->next[r];\r\n      }\r\n      if (u->next[r]\
    \ and u->next[r]->v == v)\r\n        return iterator(u->next[r]);\r\n      stack_[r]\
    \ = u;\r\n      idx_[r] = j;\r\n    }\r\n    int hw = _base::pick_height();\r\n\
    \    if (hw > _base::height_) {\r\n      _base::height_ = hw;\r\n    }\r\n   \
    \ auto w = std::make_shared<Node>(v, hw);\r\n    if (hw > _base::height_) {\r\n\
    \      _base::height_ = hw;\r\n    }\r\n    for (int r = _base::height_; r >=\
    \ 0; r--) {\r\n      if (r <= hw) {\r\n        if (stack_[r]->next[r]) {\r\n \
    \         w->length[r] = stack_[r]->length[r] - (j - idx_[r]);\r\n        } else\
    \ {\r\n          w->length[r] = 0;\r\n        }\r\n        stack_[r]->length[r]\
    \ = j - idx_[r] + 1;\r\n        w->next[r] = stack_[r]->next[r];\r\n        stack_[r]->next[r]\
    \ = w;\r\n      } else if (stack_[r]->next[r]) {\r\n        stack_[r]->length[r]++;\r\
    \n      }\r\n    }\r\n    _base::size_++;\r\n    return iterator(w);\r\n  }\r\n\
    \r\n  iterator erase(const T& v) {\r\n    bool erased = false;\r\n    auto u =\
    \ _base::sentinel_;\r\n    for (int r = _base::height_; r >= 0; r--) {\r\n   \
    \   while (u->next[r] and u->next[r]->v < v)\r\n        u = u->next[r];\r\n  \
    \    if (u->next[r] and u->next[r]->v == v) {\r\n        erased = true;\r\n  \
    \      if (u->next[r]->next[r]) {\r\n          u->length[r] = u->length[r] + u->next[r]->length[r]\
    \ - 1;\r\n        } else {\r\n          u->length[r] = 0;\r\n        }\r\n   \
    \     u->next[r] = u->next[r]->next[r];\r\n      } else if (u->next[r]) {\r\n\
    \        u->length[r]--;\r\n      }\r\n    }\r\n    if (erased)\r\n      _base::size_--;\r\
    \n    return iterator(u->next[0]);\r\n  }\r\n};\r\n"
  dependsOn:
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/skiplist.hpp
  requiredBy: []
  timestamp: '2023-04-04 01:01:39+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/skiplist.hpp
layout: document
redirect_from:
- /library/include/mtl/skiplist.hpp
- /library/include/mtl/skiplist.hpp.html
title: include/mtl/skiplist.hpp
---
