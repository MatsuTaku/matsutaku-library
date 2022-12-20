---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/skewbinary_list.hpp\"\n\n#include <memory>\n\
    #include <vector>\n#include <stack>\n#include <cassert>\n#include <cstddef>\n\
    #include <type_traits>\n#include <iterator>\n#include <iostream>\n#line 2 \"include/mtl/bit_manip.hpp\"\
    \n#include <cstdint>\n#line 4 \"include/mtl/bit_manip.hpp\"\n\nnamespace bm {\n\
    \ninline constexpr uint64_t popcnt_e8(uint64_t x) {\n  x = (x & 0x5555555555555555)\
    \ + ((x>>1) & 0x5555555555555555);\n  x = (x & 0x3333333333333333) + ((x>>2) &\
    \ 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n\
    \  return x;\n}\n// Count 1s\ninline constexpr unsigned popcnt(uint64_t x) {\n\
    \  return (popcnt_e8(x) * 0x0101010101010101) >> 56;\n}\n// Count trailing 0s.\
    \ ...01101000 -> 3\ninline constexpr unsigned ctz(uint64_t x) {\n  return popcnt((x\
    \ & (-x)) - 1);\n}\ninline constexpr unsigned ctz8(uint8_t x) {\n  return x ==\
    \ 0 ? 8 : popcnt_e8((x & (-x)) - 1);\n}\n// [00..0](8bit) -> 0, [**..*](not only\
    \ 0) -> 1\ninline constexpr uint8_t summary(uint64_t x) {\n  constexpr uint64_t\
    \ hmask = 0x8080808080808080ull;\n  constexpr uint64_t lmask = 0x7F7F7F7F7F7F7F7Full;\n\
    \  auto a = x & hmask;\n  auto b = x & lmask;\n  b = hmask - b;\n  b = ~b;\n \
    \ auto c = (a | b) & hmask;\n  c *= 0x0002040810204081ull;\n  return uint8_t(c\
    \ >> 56);\n}\n// Extract target area of bits\ninline constexpr uint64_t bextr(uint64_t\
    \ x, unsigned start, unsigned len) {\n  uint64_t mask = len < 64 ? (1ull<<len)-1\
    \ : 0xFFFFFFFFFFFFFFFFull;\n  return (x >> start) & mask;\n}\n// 00101101 -> 00111111\
    \ -count_1s-> 6\ninline constexpr unsigned log2p1(uint8_t x) {\n  if (x & 0x80)\n\
    \    return 8;\n  uint64_t p = uint64_t(x) * 0x0101010101010101ull;\n  p -= 0x8040201008040201ull;\n\
    \  p = ~p & 0x8080808080808080ull;\n  p = (p >> 7) * 0x0101010101010101ull;\n\
    \  p >>= 56;\n  return p;\n}\n// 00101100 -mask_mssb-> 00100000 -to_index-> 5\n\
    inline constexpr unsigned mssb8(uint8_t x) {\n  assert(x != 0);\n  return log2p1(x)\
    \ - 1;\n}\n// 00101100 -mask_lssb-> 00000100 -to_index-> 2\ninline constexpr unsigned\
    \ lssb8(uint8_t x) {\n  assert(x != 0);\n  return popcnt_e8((x & -x) - 1);\n}\n\
    // Count leading 0s. 00001011... -> 4\ninline constexpr unsigned clz(uint64_t\
    \ x) {\n  if (x == 0)\n    return 64;\n  auto i = mssb8(summary(x));\n  auto j\
    \ = mssb8(bextr(x, 8 * i, 8));\n  return 63 - (8 * i + j);\n}\ninline constexpr\
    \ unsigned clz8(uint8_t x) {\n  return x == 0 ? 8 : 7 - mssb8(x);\n}\ninline constexpr\
    \ uint64_t bit_reverse(uint64_t x) {\n  x = ((x & 0x00000000FFFFFFFF) << 32) |\
    \ ((x & 0xFFFFFFFF00000000) >> 32);\n  x = ((x & 0x0000FFFF0000FFFF) << 16) |\
    \ ((x & 0xFFFF0000FFFF0000) >> 16);\n  x = ((x & 0x00FF00FF00FF00FF) << 8) | ((x\
    \ & 0xFF00FF00FF00FF00) >> 8);\n  x = ((x & 0x0F0F0F0F0F0F0F0F) << 4) | ((x &\
    \ 0xF0F0F0F0F0F0F0F0) >> 4);\n  x = ((x & 0x3333333333333333) << 2) | ((x & 0xCCCCCCCCCCCCCCCC)\
    \ >> 2);\n  x = ((x & 0x5555555555555555) << 1) | ((x & 0xAAAAAAAAAAAAAAAA) >>\
    \ 1);\n  return x;\n}\n\n} // namespace bm\n#line 12 \"include/mtl/skewbinary_list.hpp\"\
    \n\n/*\n * Persistent purely functional random-access list\n */\ntemplate<typename\
    \ T>\nclass PersistentSkewbinaryList {\n public:\n  using value_type = T;\n  class\
    \ iterator;\n\n private:\n  struct Digit;\n  struct Node;\n  using digit_pointer\
    \ = std::shared_ptr<Digit>;\n  using node_pointer = std::shared_ptr<Node>;\n \
    \ struct Node {\n    T v;\n    uint8_t dig;\n    node_pointer left, right;\n \
    \   Node(const T& v, uint8_t dig = 1) : v(v), dig(dig), left(nullptr), right(nullptr)\
    \ {}\n    size_t size() const {\n      return (1ull<<dig) - 1;\n    }\n    bool\
    \ is_lump() const {\n      return dig > 1 and left == nullptr;\n    }\n  };\n\
    \  struct Digit {\n    node_pointer ch;\n    digit_pointer next;\n    Digit(node_pointer\
    \ root, digit_pointer right) : ch(root), next(right) {}\n  };\n\n  digit_pointer\
    \ root_;\n  size_t size_;\n\n private:\n  explicit PersistentSkewbinaryList(digit_pointer\
    \ root, size_t size=0) : root_(root), size_(size) {}\n\n  void _push_node(node_pointer\
    \ u) {\n    root_ = std::make_shared<Digit>(u, root_);\n    size_ += u->size();\n\
    \  }\n\n public:\n  PersistentSkewbinaryList() : root_(nullptr), size_(0) {}\n\
    \n  template<class InputIt>\n  PersistentSkewbinaryList(InputIt begin, InputIt\
    \ end) : PersistentSkewbinaryList() {\n    using traits = std::iterator_traits<InputIt>;\n\
    \    using value_type = typename traits::value_type;\n    static_assert(std::is_convertible<T,\
    \ value_type>::value);\n    using category = typename traits::iterator_category;\n\
    \    if constexpr (std::is_base_of<std::bidirectional_iterator_tag, category>::value)\
    \ {\n      for (auto it = std::prev(end); ; --it) {\n        *this = pushed(*it);\n\
    \        if (it == begin)\n          break;\n      }\n    } else if constexpr\
    \ (std::is_base_of<std::forward_iterator_tag, category>::value) {\n      std::stack<T>\
    \ s(begin, end);\n      while (!s.empty()) {\n        _push_node(std::make_shared<Node>(s.top()));\n\
    \        s.pop();\n      }\n    }\n  }\n  PersistentSkewbinaryList(std::initializer_list<T>\
    \ list) : PersistentSkewbinaryList(list.begin(), list.end()) {}\n\n  size_t size()\
    \ const { return size_; }\n  bool empty() const { return size() == 0; }\n\n  T\
    \ front() const {\n    return root_->ch->v;\n  }\n\n  [[nodiscard]] PersistentSkewbinaryList\
    \ pushed(const T& v) const {\n    if (root_ and root_->next and root_->ch->dig\
    \ == root_->next->ch->dig) {\n      auto new_u = std::make_shared<Node>(v, root_->ch->dig+1);\n\
    \      new_u->left = root_->ch;\n      new_u->right = root_->next->ch;\n     \
    \ auto new_root = std::make_shared<Digit>(new_u, root_->next->next);\n      return\
    \ PersistentSkewbinaryList(new_root, size()+1);\n    } else {\n      auto new_root\
    \ = std::make_shared<Digit>(std::make_shared<Node>(v), root_);\n      return PersistentSkewbinaryList(new_root,\
    \ size()+1);\n    }\n  }\n\n  [[nodiscard]] PersistentSkewbinaryList popped()\
    \ const {\n    if (root_->ch->size() > 1) {\n      if (!root_->ch->is_lump())\
    \ {\n        digit_pointer new_next = std::make_shared<Digit>(root_->ch->right,\
    \ root_->next);\n        digit_pointer new_root = std::make_shared<Digit>(root_->ch->left,\
    \ new_next);\n        return PersistentSkewbinaryList(new_root, size()-1);\n \
    \     } else {\n        node_pointer new_u = std::make_shared<Node>(root_->ch->v,\
    \ root_->ch->dig-1);\n        digit_pointer new_next = std::make_shared<Digit>(new_u,\
    \ root_->next);\n        digit_pointer new_root = std::make_shared<Digit>(new_u,\
    \ new_next);\n        return PersistentSkewbinaryList(new_root, size()-1);\n \
    \     }\n    } else {\n      return PersistentSkewbinaryList(root_->next, size()-1);\n\
    \    }\n  }\n\n  T get_at(int i) const {\n    int j = i;\n    auto d = root_;\n\
    \    while (d->ch->size() <= j) {\n      j -= d->ch->size();\n      d = d->next;\n\
    \    }\n    auto u = d->ch;\n    while (j > 0) {\n      j--;\n      if (u->is_lump())\
    \ {\n        return u->v;\n      }\n      if (j < u->left->size()) {\n       \
    \ u = u->left;\n      } else {\n        j -= u->left->size();\n        u = u->right;\n\
    \      }\n    }\n    return u->v;\n  }\n\n  [[nodiscard]] PersistentSkewbinaryList\
    \ updated_at(int i, const T& v) const {\n    auto d = root_;\n    digit_pointer\
    \ pd = std::make_shared<Digit>(nullptr, nullptr);\n    PersistentSkewbinaryList\
    \ dst(pd, size());\n    int j = i;\n    while (j >= d->ch->size()) {\n      auto\
    \ new_d = std::make_shared<Digit>(nullptr, nullptr);\n      pd->ch = d->ch;\n\
    \      pd->next = new_d;\n      pd = new_d;\n\n      j -= d->ch->size();\n   \
    \   d = d->next;\n    }\n\n    auto u = d->ch;\n    node_pointer pu = std::make_shared<Node>(d->ch->v,\
    \ d->ch->dig);\n    pd->ch = pu;\n    pd->next = d->next;\n    while (j > 0 and\
    \ !u->is_lump()) {\n      j--;\n      if (j < u->left->size()) {\n        auto\
    \ new_u = std::make_shared<Node>(u->left->v, u->left->dig);\n        pu->left\
    \ = new_u;\n        pu->right = u->right;\n        pu = new_u;\n\n        u =\
    \ u->left;\n      } else {\n        auto new_u = std::make_shared<Node>(u->right->v,\
    \ u->right->dig);\n        pu->left = u->left;\n        pu->right = new_u;\n \
    \       pu = new_u;\n\n        j -= u->left->size();\n        u = u->right;\n\
    \      }\n    }\n    if (!u->is_lump()) {\n      pu->v = v;\n      pu->left =\
    \ u->left;\n      pu->right = u->right;\n    } else {\n      int s = u->size()\
    \ >> 1;\n      int dig = u->dig-1;\n      while (j > 0) {\n        j--;\n    \
    \    pu->left = std::make_shared<Node>(u->v, dig);\n        pu->right = std::make_shared<Node>(u->v,\
    \ dig);\n        if (j < s) {\n          pu = pu->left;\n        } else {\n  \
    \        j -= s;\n          pu = pu->right;\n        }\n        s >>= 1;\n   \
    \     dig--;\n      }\n      pu->v = v;\n      pu->left = pu->right = std::make_shared<Node>(u->v,\
    \ dig);\n    }\n    return dst;\n  }\n\n  [[nodiscard]] PersistentSkewbinaryList\
    \ dropped(const int i) const {\n    int j = i;\n    auto d = root_;\n    while\
    \ (d and d->ch->size() <= j) {\n      j -= d->ch->size();\n      d = d->next;\n\
    \    }\n    if (!d) {\n      return PersistentSkewbinaryList();\n    }\n    PersistentSkewbinaryList\
    \ dst(d->next); // dst.size_ is not correct value.\n    auto u = d->ch;\n    while\
    \ (j > 0) {\n      j--;\n      if (j < u->left->size()) {\n        dst._push_node(u->right);\n\
    \        u = u->left;\n      } else {\n        j -= u->left->size();\n       \
    \ u = u->right;\n      }\n    }\n    dst._push_node(u);\n    dst.size_ = size()\
    \ - i;\n    return dst;\n  }\n\n  uint64_t sbdig_to_num(int dig) const {\n   \
    \ return (1ull<<dig) - 1;\n  }\n\n  [[nodiscard]] PersistentSkewbinaryList extended(size_t\
    \ n, const T& v) const {\n    PersistentSkewbinaryList dst(root_, size());\n \
    \   auto m = n;\n    while (m > 0) {\n      if (dst.root_ and dst.root_->next\
    \ and dst.root_->ch->size() == dst.root_->next->ch->size()) {\n        auto new_u\
    \ = std::make_shared<Node>(v, dst.root_->ch->dig+1);\n        new_u->left = dst.root_->ch;\n\
    \        new_u->right = dst.root_->next->ch;\n        dst.root_ = std::make_shared<Digit>(new_u,\
    \ dst.root_->next->next);\n        dst.size_++;\n        m--;\n      } else if\
    \ (m >= dst.root_->ch->size()) {\n        m -= dst.root_->ch->size();\n      \
    \  dst._push_node(std::make_shared<Node>(v, dst.root_->ch->dig));\n      } else\
    \ {\n        break;\n      }\n    }\n    while (m > 0) {\n      int dig = 63-bm::clz(m+1);\n\
    \      dst._push_node(std::make_shared<Node>(v, dig));\n      size_t s = (1ull<<dig)\
    \ - 1;\n      assert(m >= s);\n      m -= s;\n    }\n    assert(dst.size_ == size()\
    \ + n);\n    return dst;\n  }\n\n public:\n  T operator[](size_t i) const {\n\
    \    return get_at(i);\n  }\n\n  class iterator {\n   public:\n    using value_type\
    \ = T;\n    using iterator_category = std::forward_iterator_tag;\n    using difference_type\
    \ = unsigned long long;\n   private:\n    digit_pointer root_;\n   public:\n \
    \   iterator(digit_pointer root) : root_(root) {}\n    value_type operator*()\
    \ const { return root_->ch->v; }\n    bool operator==(iterator r) { return root_\
    \ == r.root_; }\n    bool operator!=(iterator r) { return root_ != r.root_; }\n\
    \    iterator& operator++() {\n      root_ = PersistentSkewbinaryList(root_).popped().root_;\n\
    \      return *this;\n    }\n    iterator operator++(int) {\n      auto dst =\
    \ *this;\n      root_ = PersistentSkewbinaryList(root_).popped().root_;\n    \
    \  return dst;\n    }\n    iterator operator+(difference_type x) const {\n   \
    \   return iterator(PersistentSkewbinaryList(root_).dropped(x).root_);\n    }\n\
    \    iterator operator+=(difference_type x) {\n      root_ = PersistentSkewbinaryList(root_).dropped(x).root_;\n\
    \      return *this;\n    }\n  };\n\n  iterator begin() const {\n    return iterator(root_);\n\
    \  }\n  iterator end() const {\n    return iterator(nullptr);\n  }\n\n};\n"
  code: "#pragma once\n\n#include <memory>\n#include <vector>\n#include <stack>\n\
    #include <cassert>\n#include <cstddef>\n#include <type_traits>\n#include <iterator>\n\
    #include <iostream>\n#include \"bit_manip.hpp\"\n\n/*\n * Persistent purely functional\
    \ random-access list\n */\ntemplate<typename T>\nclass PersistentSkewbinaryList\
    \ {\n public:\n  using value_type = T;\n  class iterator;\n\n private:\n  struct\
    \ Digit;\n  struct Node;\n  using digit_pointer = std::shared_ptr<Digit>;\n  using\
    \ node_pointer = std::shared_ptr<Node>;\n  struct Node {\n    T v;\n    uint8_t\
    \ dig;\n    node_pointer left, right;\n    Node(const T& v, uint8_t dig = 1) :\
    \ v(v), dig(dig), left(nullptr), right(nullptr) {}\n    size_t size() const {\n\
    \      return (1ull<<dig) - 1;\n    }\n    bool is_lump() const {\n      return\
    \ dig > 1 and left == nullptr;\n    }\n  };\n  struct Digit {\n    node_pointer\
    \ ch;\n    digit_pointer next;\n    Digit(node_pointer root, digit_pointer right)\
    \ : ch(root), next(right) {}\n  };\n\n  digit_pointer root_;\n  size_t size_;\n\
    \n private:\n  explicit PersistentSkewbinaryList(digit_pointer root, size_t size=0)\
    \ : root_(root), size_(size) {}\n\n  void _push_node(node_pointer u) {\n    root_\
    \ = std::make_shared<Digit>(u, root_);\n    size_ += u->size();\n  }\n\n public:\n\
    \  PersistentSkewbinaryList() : root_(nullptr), size_(0) {}\n\n  template<class\
    \ InputIt>\n  PersistentSkewbinaryList(InputIt begin, InputIt end) : PersistentSkewbinaryList()\
    \ {\n    using traits = std::iterator_traits<InputIt>;\n    using value_type =\
    \ typename traits::value_type;\n    static_assert(std::is_convertible<T, value_type>::value);\n\
    \    using category = typename traits::iterator_category;\n    if constexpr (std::is_base_of<std::bidirectional_iterator_tag,\
    \ category>::value) {\n      for (auto it = std::prev(end); ; --it) {\n      \
    \  *this = pushed(*it);\n        if (it == begin)\n          break;\n      }\n\
    \    } else if constexpr (std::is_base_of<std::forward_iterator_tag, category>::value)\
    \ {\n      std::stack<T> s(begin, end);\n      while (!s.empty()) {\n        _push_node(std::make_shared<Node>(s.top()));\n\
    \        s.pop();\n      }\n    }\n  }\n  PersistentSkewbinaryList(std::initializer_list<T>\
    \ list) : PersistentSkewbinaryList(list.begin(), list.end()) {}\n\n  size_t size()\
    \ const { return size_; }\n  bool empty() const { return size() == 0; }\n\n  T\
    \ front() const {\n    return root_->ch->v;\n  }\n\n  [[nodiscard]] PersistentSkewbinaryList\
    \ pushed(const T& v) const {\n    if (root_ and root_->next and root_->ch->dig\
    \ == root_->next->ch->dig) {\n      auto new_u = std::make_shared<Node>(v, root_->ch->dig+1);\n\
    \      new_u->left = root_->ch;\n      new_u->right = root_->next->ch;\n     \
    \ auto new_root = std::make_shared<Digit>(new_u, root_->next->next);\n      return\
    \ PersistentSkewbinaryList(new_root, size()+1);\n    } else {\n      auto new_root\
    \ = std::make_shared<Digit>(std::make_shared<Node>(v), root_);\n      return PersistentSkewbinaryList(new_root,\
    \ size()+1);\n    }\n  }\n\n  [[nodiscard]] PersistentSkewbinaryList popped()\
    \ const {\n    if (root_->ch->size() > 1) {\n      if (!root_->ch->is_lump())\
    \ {\n        digit_pointer new_next = std::make_shared<Digit>(root_->ch->right,\
    \ root_->next);\n        digit_pointer new_root = std::make_shared<Digit>(root_->ch->left,\
    \ new_next);\n        return PersistentSkewbinaryList(new_root, size()-1);\n \
    \     } else {\n        node_pointer new_u = std::make_shared<Node>(root_->ch->v,\
    \ root_->ch->dig-1);\n        digit_pointer new_next = std::make_shared<Digit>(new_u,\
    \ root_->next);\n        digit_pointer new_root = std::make_shared<Digit>(new_u,\
    \ new_next);\n        return PersistentSkewbinaryList(new_root, size()-1);\n \
    \     }\n    } else {\n      return PersistentSkewbinaryList(root_->next, size()-1);\n\
    \    }\n  }\n\n  T get_at(int i) const {\n    int j = i;\n    auto d = root_;\n\
    \    while (d->ch->size() <= j) {\n      j -= d->ch->size();\n      d = d->next;\n\
    \    }\n    auto u = d->ch;\n    while (j > 0) {\n      j--;\n      if (u->is_lump())\
    \ {\n        return u->v;\n      }\n      if (j < u->left->size()) {\n       \
    \ u = u->left;\n      } else {\n        j -= u->left->size();\n        u = u->right;\n\
    \      }\n    }\n    return u->v;\n  }\n\n  [[nodiscard]] PersistentSkewbinaryList\
    \ updated_at(int i, const T& v) const {\n    auto d = root_;\n    digit_pointer\
    \ pd = std::make_shared<Digit>(nullptr, nullptr);\n    PersistentSkewbinaryList\
    \ dst(pd, size());\n    int j = i;\n    while (j >= d->ch->size()) {\n      auto\
    \ new_d = std::make_shared<Digit>(nullptr, nullptr);\n      pd->ch = d->ch;\n\
    \      pd->next = new_d;\n      pd = new_d;\n\n      j -= d->ch->size();\n   \
    \   d = d->next;\n    }\n\n    auto u = d->ch;\n    node_pointer pu = std::make_shared<Node>(d->ch->v,\
    \ d->ch->dig);\n    pd->ch = pu;\n    pd->next = d->next;\n    while (j > 0 and\
    \ !u->is_lump()) {\n      j--;\n      if (j < u->left->size()) {\n        auto\
    \ new_u = std::make_shared<Node>(u->left->v, u->left->dig);\n        pu->left\
    \ = new_u;\n        pu->right = u->right;\n        pu = new_u;\n\n        u =\
    \ u->left;\n      } else {\n        auto new_u = std::make_shared<Node>(u->right->v,\
    \ u->right->dig);\n        pu->left = u->left;\n        pu->right = new_u;\n \
    \       pu = new_u;\n\n        j -= u->left->size();\n        u = u->right;\n\
    \      }\n    }\n    if (!u->is_lump()) {\n      pu->v = v;\n      pu->left =\
    \ u->left;\n      pu->right = u->right;\n    } else {\n      int s = u->size()\
    \ >> 1;\n      int dig = u->dig-1;\n      while (j > 0) {\n        j--;\n    \
    \    pu->left = std::make_shared<Node>(u->v, dig);\n        pu->right = std::make_shared<Node>(u->v,\
    \ dig);\n        if (j < s) {\n          pu = pu->left;\n        } else {\n  \
    \        j -= s;\n          pu = pu->right;\n        }\n        s >>= 1;\n   \
    \     dig--;\n      }\n      pu->v = v;\n      pu->left = pu->right = std::make_shared<Node>(u->v,\
    \ dig);\n    }\n    return dst;\n  }\n\n  [[nodiscard]] PersistentSkewbinaryList\
    \ dropped(const int i) const {\n    int j = i;\n    auto d = root_;\n    while\
    \ (d and d->ch->size() <= j) {\n      j -= d->ch->size();\n      d = d->next;\n\
    \    }\n    if (!d) {\n      return PersistentSkewbinaryList();\n    }\n    PersistentSkewbinaryList\
    \ dst(d->next); // dst.size_ is not correct value.\n    auto u = d->ch;\n    while\
    \ (j > 0) {\n      j--;\n      if (j < u->left->size()) {\n        dst._push_node(u->right);\n\
    \        u = u->left;\n      } else {\n        j -= u->left->size();\n       \
    \ u = u->right;\n      }\n    }\n    dst._push_node(u);\n    dst.size_ = size()\
    \ - i;\n    return dst;\n  }\n\n  uint64_t sbdig_to_num(int dig) const {\n   \
    \ return (1ull<<dig) - 1;\n  }\n\n  [[nodiscard]] PersistentSkewbinaryList extended(size_t\
    \ n, const T& v) const {\n    PersistentSkewbinaryList dst(root_, size());\n \
    \   auto m = n;\n    while (m > 0) {\n      if (dst.root_ and dst.root_->next\
    \ and dst.root_->ch->size() == dst.root_->next->ch->size()) {\n        auto new_u\
    \ = std::make_shared<Node>(v, dst.root_->ch->dig+1);\n        new_u->left = dst.root_->ch;\n\
    \        new_u->right = dst.root_->next->ch;\n        dst.root_ = std::make_shared<Digit>(new_u,\
    \ dst.root_->next->next);\n        dst.size_++;\n        m--;\n      } else if\
    \ (m >= dst.root_->ch->size()) {\n        m -= dst.root_->ch->size();\n      \
    \  dst._push_node(std::make_shared<Node>(v, dst.root_->ch->dig));\n      } else\
    \ {\n        break;\n      }\n    }\n    while (m > 0) {\n      int dig = 63-bm::clz(m+1);\n\
    \      dst._push_node(std::make_shared<Node>(v, dig));\n      size_t s = (1ull<<dig)\
    \ - 1;\n      assert(m >= s);\n      m -= s;\n    }\n    assert(dst.size_ == size()\
    \ + n);\n    return dst;\n  }\n\n public:\n  T operator[](size_t i) const {\n\
    \    return get_at(i);\n  }\n\n  class iterator {\n   public:\n    using value_type\
    \ = T;\n    using iterator_category = std::forward_iterator_tag;\n    using difference_type\
    \ = unsigned long long;\n   private:\n    digit_pointer root_;\n   public:\n \
    \   iterator(digit_pointer root) : root_(root) {}\n    value_type operator*()\
    \ const { return root_->ch->v; }\n    bool operator==(iterator r) { return root_\
    \ == r.root_; }\n    bool operator!=(iterator r) { return root_ != r.root_; }\n\
    \    iterator& operator++() {\n      root_ = PersistentSkewbinaryList(root_).popped().root_;\n\
    \      return *this;\n    }\n    iterator operator++(int) {\n      auto dst =\
    \ *this;\n      root_ = PersistentSkewbinaryList(root_).popped().root_;\n    \
    \  return dst;\n    }\n    iterator operator+(difference_type x) const {\n   \
    \   return iterator(PersistentSkewbinaryList(root_).dropped(x).root_);\n    }\n\
    \    iterator operator+=(difference_type x) {\n      root_ = PersistentSkewbinaryList(root_).dropped(x).root_;\n\
    \      return *this;\n    }\n  };\n\n  iterator begin() const {\n    return iterator(root_);\n\
    \  }\n  iterator end() const {\n    return iterator(nullptr);\n  }\n\n};\n"
  dependsOn:
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/skewbinary_list.hpp
  requiredBy: []
  timestamp: '2022-12-18 04:26:00+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/skewbinary_list.hpp
layout: document
redirect_from:
- /library/include/mtl/skewbinary_list.hpp
- /library/include/mtl/skewbinary_list.hpp.html
title: include/mtl/skewbinary_list.hpp
---
