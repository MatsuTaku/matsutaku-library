---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/compress_int.hpp
    title: include/mtl/compress_int.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/fenwick_tree.hpp
    title: include/mtl/fenwick_tree.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/ordinal_range_search.hpp
    title: include/mtl/ordinal_range_search.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/bit_vector.hpp
    title: include/mtl/succinct/bit_vector.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/wavelet_matrix.hpp
    title: include/mtl/succinct/wavelet_matrix.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/point_add_rectangle_sum
    links:
    - https://judge.yosupo.jp/problem/point_add_rectangle_sum
  bundledCode: "#line 1 \"test/yosupo-point_add_rectangle_sum-wm.test.cpp\"\n#define\
    \ PROBLEM \"https://judge.yosupo.jp/problem/point_add_rectangle_sum\"\r\n#line\
    \ 2 \"include/mtl/ordinal_range_search.hpp\"\n#include <cstddef>\r\n#include <vector>\r\
    \n#include <algorithm>\r\n#include <tuple>\r\n#include <queue>\r\n#line 2 \"include/mtl/succinct/wavelet_matrix.hpp\"\
    \n#include <limits>\n#include <array>\n#include <iterator>\n#line 3 \"include/mtl/succinct/bit_vector.hpp\"\
    \n#include <cstdint>\r\n#line 5 \"include/mtl/succinct/bit_vector.hpp\"\n#include\
    \ <iostream>\r\n#line 7 \"include/mtl/succinct/bit_vector.hpp\"\n#include <bitset>\r\
    \n#line 3 \"include/mtl/bit_manip.hpp\"\n#include <cassert>\n\nnamespace bm {\n\
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
    \ 1);\n  return x;\n}\n\n} // namespace bm\n#line 9 \"include/mtl/succinct/bit_vector.hpp\"\
    \n\r\n// Bitmap is likes std::vector<bool>\r\nstruct Bitmap {\r\n  using value_type\
    \ = bool;\r\n  using W = uint64_t;\r\n  std::vector<W> arr;\r\n  size_t sz;\r\n\
    \  static constexpr size_t required_word_size(size_t n) {\r\n    return (n+63)\
    \ / 64;\r\n  }\r\n  static constexpr W word_filled_by(bool bit) {\r\n    return\
    \ bit ? 0xFFFFFFFFFFFFFFFF : 0;\r\n  }\r\n  explicit Bitmap(size_t n = 0, bool\
    \ bit = false) : arr(required_word_size(n), word_filled_by(bit)), sz(n) {}\r\n\
    \  template<typename It>\r\n  Bitmap(It begin, It end) : sz(0) {\r\n    using\
    \ trait = std::iterator_traits<It>;\r\n    using iterator_category = typename\
    \ trait::iterator_category;\r\n    static_assert(std::is_base_of<std::input_iterator_tag,\
    \ iterator_category>::value, \"\");\r\n    static_assert(std::is_convertible<typename\
    \ trait::value_type, bool>::value, \"\");\r\n    if constexpr (std::is_base_of<std::random_access_iterator_tag,\
    \ iterator_category>::value) {\r\n      arr.reserve(required_word_size(std::distance(begin,\
    \ end)));\r\n    }\r\n    for (auto it = begin; it != end; ++it)\r\n      push_back((bool)*it);\r\
    \n  }\r\n\r\n  size_t size() const { return sz; }\r\n  bool empty() const { return\
    \ size() == 0; }\r\n\r\n  void push_back(bool bit) {\r\n    auto r = sz % 64;\r\
    \n    if (r == 0) {\r\n      arr.push_back((W)bit);\r\n    } else {\r\n      if\
    \ (bit)\r\n        arr.back() |= 1ull << r;\r\n      else\r\n        arr.back()\
    \ &= ~(1ull << r);\r\n    }\r\n    ++sz;\r\n  }\r\n  void pop_back() {\r\n   \
    \ --sz;\r\n  }\r\n  void resize(size_t new_size, bool bit) {\r\n    auto old_size\
    \ = size();\r\n    sz = new_size;\r\n    if (new_size < old_size) {\r\n      return;\r\
    \n    }\r\n    arr.resize(required_word_size(new_size), word_filled_by(bit));\r\
    \n    auto r = old_size % 64;\r\n    if (r) {\r\n      W mask = (1ull << r) -\
    \ 1;\r\n      if (bit)\r\n        arr[old_size / 64] |= ~mask;\r\n      else\r\
    \n        arr[old_size / 64] &= mask;\r\n    }\r\n  }\r\n  void assign(size_t\
    \ new_size, bool bit) {\r\n    sz = new_size;\r\n    arr.assign(required_word_size(new_size),\
    \ word_filled_by(bit));\r\n  }\r\n\r\n  struct const_reference;\r\n  struct reference;\r\
    \n  template<bool>\r\n  struct _iterator;\r\n  using const_iterator = _iterator<true>;\r\
    \n  using iterator = _iterator<false>;\r\n\r\n  const_reference operator[](size_t\
    \ i) const {\r\n    return const_reference(arr.data() + i / 64, 1ull << (i % 64));\r\
    \n  }\r\n  reference operator[](size_t i) {\r\n    return {arr.data() + i / 64,\
    \ 1ull << (i % 64)};\r\n  }\r\n  const_iterator begin() const { return const_iterator(arr.data(),\
    \ 0); }\r\n  iterator begin() { return iterator(arr.data(), 0); }\r\n  const_iterator\
    \ cbegin() const { return begin(); }\r\n  const_iterator end() const { return\
    \ const_iterator(arr.data() + sz / 64, sz % 64); }\r\n  iterator end() { return\
    \ iterator(arr.data() + sz / 64, sz % 64); }\r\n  const_iterator cend() const\
    \ { return end(); }\r\n\r\n  template<bool Const>\r\n  struct reference_base {\r\
    \n    using _pointer = typename std::conditional<Const, const W*, W*>::type;\r\
    \n    using _iterator = typename std::conditional<Const, const_iterator, iterator>::type;\r\
    \n    _pointer ptr;\r\n    W mask;\r\n    reference_base(_pointer ptr, W mask)\
    \ : ptr(ptr), mask(mask) {}\r\n    reference_base(const reference_base&) = delete;\r\
    \n    reference_base& operator=(const reference_base&) = delete;\r\n    reference_base(reference_base&&)\
    \ noexcept = default;\r\n    reference_base& operator=(reference_base&&) noexcept\
    \ = default;\r\n    inline operator bool() const {\r\n      return (*ptr & mask)\
    \ != 0;\r\n    }\r\n    inline bool operator==(bool r) const {\r\n      return\
    \ (bool) *this == r;\r\n    }\r\n    inline friend bool operator==(bool l, const\
    \ reference_base& r) {\r\n      return r == l;\r\n    }\r\n    inline bool operator!=(bool\
    \ r) const {\r\n      return (bool) *this != r;\r\n    }\r\n    inline friend\
    \ bool operator!=(bool l, const reference_base& r) {\r\n      return r != l;\r\
    \n    }\r\n    _iterator operator&() const {\r\n      return {ptr, bm::ctz(mask)};\r\
    \n    }\r\n    std::ostream& operator<<(std::ostream& os) const {\r\n      return\
    \ os << (bool) *this;\r\n    }\r\n  };\r\n  struct const_reference : public reference_base<true>\
    \ {\r\n    using _base = reference_base<true>;\r\n    const_reference(_base::_pointer\
    \ ptr, W mask) : _base(ptr, mask) {}\r\n    const_reference(const reference& rhs)\
    \ : _base(rhs.ptr, rhs.mask) {}\r\n  };\r\n  struct reference : public reference_base<false>\
    \ {\r\n    using _base = reference_base<false>;\r\n    reference(_base::_pointer\
    \ ptr, W mask) : _base(ptr, mask) {}\r\n    inline reference& operator=(bool bit)\
    \ {\r\n      if (bit)\r\n        *ptr |= mask;\r\n      else\r\n        *ptr &=\
    \ ~mask;\r\n      return *this;\r\n    }\r\n    inline reference& operator|=(bool\
    \ bit) {\r\n      if (bit)\r\n        *ptr |= mask;\r\n      return *this;\r\n\
    \    }\r\n    inline reference& operator&=(bool bit) {\r\n      if (!bit)\r\n\
    \        *ptr &= ~mask;\r\n      return *this;\r\n    }\r\n    template<bool C>\r\
    \n    inline reference& operator=(const reference_base<C>& rhs) {\r\n      return\
    \ *this = (bool) rhs;\r\n    }\r\n    reference(const reference& rhs) = delete;\r\
    \n    reference(reference&& rhs) noexcept = default;\r\n    reference& operator=(reference&&\
    \ rhs) noexcept = default;\r\n    std::istream& operator>>(std::istream& is) {\r\
    \n      bool b;\r\n      is >> b;\r\n      operator=(b);\r\n      return is;\r\
    \n    }\r\n  };\r\n\r\n  template<bool Const>\r\n  struct _iterator\r\n      :\
    \ public std::iterator<std::random_access_iterator_tag, bool, std::ptrdiff_t,\
    \ iterator, typename std::conditional<Const, const_reference, reference>::type>\r\
    \n  {\r\n    using _pointer = typename std::conditional<Const, const W*, W*>::type;\r\
    \n    using _reference = typename std::conditional<Const, const_reference, reference>::type;\r\
    \n    _pointer ptr;\r\n    unsigned ctz;\r\n    _iterator(_pointer ptr, unsigned\
    \ ctz) : ptr(ptr), ctz(ctz) {}\r\n    inline _reference operator*() const {\r\n\
    \      return _reference(ptr, 1ull << ctz);\r\n    }\r\n    template<bool C>\r\
    \n    inline bool operator==(const _iterator<C>& r) const {\r\n      return ptr\
    \ == r.ptr and ctz == r.ctz;\r\n    }\r\n    template<bool C>\r\n    inline bool\
    \ operator!=(const _iterator<C>& r) const {\r\n      return ptr != r.ptr or ctz\
    \ != r.ctz;\r\n    }\r\n    inline _iterator& operator++() {\r\n      if (ctz\
    \ % 64 < 63) {\r\n        ++ctz;\r\n      } else {\r\n        ++ptr;\r\n     \
    \   ctz = 0;\r\n      }\r\n      return *this;\r\n    }\r\n    inline _iterator\
    \ operator++(int) {\r\n      _iterator ret = *this;\r\n      operator++();\r\n\
    \      return ret;\r\n    }\r\n    inline _iterator& operator--() {\r\n      if\
    \ (ctz % 64 > 0) {\r\n        --ctz;\r\n      } else {\r\n        --ptr;\r\n \
    \       ctz = 63;\r\n      }\r\n      return *this;\r\n    }\r\n    inline _iterator\
    \ operator--(int) {\r\n      _iterator ret = *this;\r\n      operator--();\r\n\
    \      return ret;\r\n    }\r\n    inline _iterator& operator+=(std::ptrdiff_t\
    \ d) {\r\n      if (d < 0)\r\n        return operator-=(-d);\r\n      auto r =\
    \ d % 64;\r\n      if (ctz + r < 64) {\r\n        ptr += d / 64;\r\n        ctz\
    \ += r;\r\n      } else {\r\n        ptr += d / 64 + 1;\r\n        ctz = (ctz\
    \ + r) - 64;\r\n      }\r\n      return *this;\r\n    }\r\n    inline _iterator\
    \ operator+(std::ptrdiff_t d) const {\r\n      return _iterator(*this) += d;\r\
    \n    }\r\n    inline _iterator& operator-=(std::ptrdiff_t d) {\r\n      if (d\
    \ < 0)\r\n        return operator+=(-d);\r\n      auto r = d % 64;\r\n      if\
    \ (r <= ctz) {\r\n        ptr -= d / 64;\r\n        ctz -= r;\r\n      } else\
    \ {\r\n        ptr -= d / 64 + 1;\r\n        ctz = (ctz + 64 - r) - 64;\r\n  \
    \    }\r\n      return *this;\r\n    }\r\n    inline _iterator operator-(std::ptrdiff_t\
    \ d) const {\r\n      return _iterator(*this) -= d;\r\n    }\r\n    inline _reference\
    \ operator[](size_t i) const {\r\n      return *(*this + i);\r\n    }\r\n  };\r\
    \n};\r\n\r\nstruct BitVector {\r\n  static constexpr unsigned L = 512;\r\n  static\
    \ constexpr unsigned S = 64;\r\n  static constexpr unsigned S_PER_L = L / S;\r\
    \n  Bitmap bm;\r\n  std::vector<uint64_t> _r, _s, _zs;\r\n\r\n  std::array<std::array<uint8_t,\
    \ 9>, 1<<8> sel_tb;\r\n  constexpr void init_sel_tb() {\r\n    for (int i = 0;\
    \ i < 1<<8; i++) {\r\n      int c = 0;\r\n      int x = i;\r\n      sel_tb[i].fill(8);\r\
    \n      for (int j = 0; j < 8; j++) {\r\n        if (x & 1)\r\n          sel_tb[i][++c]\
    \ = j;\r\n        x >>= 1;\r\n      }\r\n    }\r\n  }\r\n\r\n  explicit BitVector(size_t\
    \ size = 0, bool bit = false) : bm(size, bit) {\r\n    init_sel_tb();\r\n  }\r\
    \n  template<typename It>\r\n  BitVector(It begin, It end) : bm(begin, end) {\r\
    \n    init_sel_tb();\r\n  }\r\n  size_t size() const { return bm.size(); }\r\n\
    \  bool empty() const { return bm.empty(); }\r\n  void push_back(bool bit) { bm.push_back(bit);\
    \ }\r\n  void resize(size_t new_size, bool bit) { bm.resize(new_size, bit); }\r\
    \n  void assign(size_t new_size, bool bit) { bm.assign(new_size, bit); }\r\n \
    \ Bitmap::const_reference operator[](size_t i) const { return bm[i]; }\r\n  Bitmap::reference\
    \ operator[](size_t i) { return bm[i]; }\r\n  Bitmap::const_iterator begin() const\
    \ { return bm.begin(); }\r\n  Bitmap::const_iterator end() const { return bm.end();\
    \ }\r\n  Bitmap::iterator begin() { return bm.begin(); }\r\n  Bitmap::iterator\
    \ end() { return bm.end(); }\r\n  Bitmap::const_iterator cbegin() const { return\
    \ bm.cbegin(); }\r\n  Bitmap::const_iterator cend() const { return bm.cend();\
    \ }\r\n\r\n  void build() {\r\n    const auto num_l = (bm.size() + L-1) / L;\r\
    \n    _r.assign((num_l + 1) * 2, 0);\r\n    _s.clear();\r\n    _s.push_back(0);\r\
    \n    _zs.clear();\r\n    _zs.push_back(0);\r\n    uint64_t sum = 0;\r\n    for\
    \ (size_t l = 0; l < num_l; ++l) {\r\n      auto psum = sum;\r\n      uint64_t\
    \ sum_l = 0;\r\n      for (size_t s = 0; s < S_PER_L; ++s) {\r\n        if (l\
    \ * S_PER_L + s < bm.arr.size())\r\n          sum_l += bm::popcnt(bm.arr[l * S_PER_L\
    \ + s]);\r\n        if (s < S_PER_L-1)\r\n          _r[l * 2 + 1] |= sum_l <<\
    \ ((7-(s+1)) * 9);\r\n      }\r\n      sum += sum_l;\r\n      _r[(l + 1) * 2]\
    \ = sum;\r\n      if (psum / L != sum / L) {\r\n        _s.push_back(l);\r\n \
    \     }\r\n      if ((L*l - psum) / L != (L*(l+1) - sum) / L) {\r\n        _zs.push_back(l);\r\
    \n      }\r\n    }\r\n    _s.push_back(num_l);\r\n    _zs.push_back(num_l);\r\n\
    \  }\r\n\r\n  template<bool B>\r\n  inline size_t get_l(size_t l) const {\r\n\
    \    return B ? _r[l * 2] : l * L - _r[l * 2];\r\n  }\r\n  template<bool B>\r\n\
    \  inline size_t get_s(size_t l, size_t s) const {\r\n    auto b = (_r[l*2+1]\
    \ >> ((7-s)*9)) % (1ull<<9);\r\n    return B ? b : s * S - b;\r\n  }\r\n\r\n \
    \ inline size_t rank(size_t i) const {\r\n    auto l = i / L;\r\n    auto s =\
    \ i % L / S;\r\n    auto q = i / S;\r\n    auto r = i % S;\r\n    return get_l<1>(l)\
    \ + get_s<1>(l, s) + bm::popcnt(bm.arr[q] & ((1ull<<r)-1));\r\n  }\r\n\r\n  template<bool\
    \ B>\r\n  size_t select(size_t n) const {\r\n    ++n; // to 1-indexed\r\n    if\
    \ (n > (B ? rank(size()) : size() - rank(size())))\r\n      return size();\r\n\
    \    // Find L block\r\n    auto& idx = B ? _s : _zs;\r\n    size_t l = idx[n\
    \ / L];\r\n    {\r\n      auto r = idx[n / L + 1] + 1;\r\n      while (l+1 < r)\
    \ {\r\n        auto c = l + (r-l)/2;\r\n        if (get_l<B>(c) < n)\r\n     \
    \     l = c;\r\n        else\r\n          r = c;\r\n      }\r\n    }\r\n    //\
    \ Find S block\r\n    size_t s = 0;\r\n    auto m = n - get_l<B>(l);\r\n    /*\
    \ Following bit-manipulates code is same as ... */\r\n//    {\r\n//      auto\
    \ d = 8;\r\n//      while (d > 1) {\r\n//        auto c = s + d/2;\r\n//     \
    \   if (get_s<B>(l, c) < m)\r\n//          s = c;\r\n//        d /= 2;\r\n// \
    \     }\r\n//    }\r\n    {\r\n      uint64_t x = (uint64_t) (m-1) * 0x0040201008040201ull;\r\
    \n      uint64_t a = _r[l*2+1];\r\n      if (!B)\r\n        a = 0x10100C08050301C0ull\
    \ - a; // to 0s sum\r\n      uint64_t xda = x - a;\r\n      uint64_t sm = 0x4020100804020100ull;\r\
    \n      uint64_t ok = (~x | a) & sm;\r\n      uint64_t ng = (~x & a) & sm;\r\n\
    \      uint64_t y = ((x ^ a ^ xda) & ok) | ng;\r\n      y = y * 0x0001010101010101ull\
    \ >> (64-1-7);\r\n      auto id = bm::clz8(y)-1;\r\n      auto clo = bm::clz((~xda\
    \ << 1 | 1) << (9*id));\r\n      auto ns = id + (clo ? (clo - 1) / 9 : 0);\r\n\
    \      s = ns;\r\n    }\r\n    // Find byte\r\n    uint64_t w = bm.arr[l * S_PER_L\
    \ + s];\r\n    if constexpr (!B) w = ~w;\r\n    auto _bs = (uint64_t) bm::popcnt_e8(w)\
    \ * 0x0101010101010100ull;\r\n    auto bs = (const uint8_t*) &_bs;\r\n    size_t\
    \ b = 0;\r\n    auto o = m - get_s<B>(l, s);\r\n    /* Following bit-manipulates\
    \ code is same as ... */\r\n//    {\r\n//      auto d = 8;\r\n//      while (d\
    \ > 1) {\r\n//        auto c = b + d/2;\r\n//        if (bs[c] < o)\r\n//    \
    \      b = c;\r\n//        d /= 2;\r\n//      }\r\n//    }\r\n    {\r\n      uint64_t\
    \ x = (uint64_t) o * 0x0101010101010101ull;\r\n      uint64_t bmx = (_bs | 0x8080808080808080ull)\
    \ - x;\r\n      uint64_t y = ((bmx & 0x8080808080808080ull) * 0x02040810204081ull)\
    \ >> (64-8);\r\n      size_t nb = bm::ctz8(y) - 1;\r\n//      assert(b == nb);\r\
    \n      b = nb;\r\n    }\r\n    // Calc select\r\n    auto r = o - bs[b];\r\n\
    \    uint8_t byte = ((const uint8_t*)(&w))[b];\r\n    assert(r and r <= bm::popcnt(byte));\r\
    \n    return l * L + s * S + b * 8 + sel_tb[byte][r];\r\n  }\r\n};\n#line 10 \"\
    include/mtl/succinct/wavelet_matrix.hpp\"\n\ntemplate<typename T>\nstruct WaveletMatrix\
    \ {\n  static constexpr unsigned H = 64 - bm::clz(std::numeric_limits<T>::max());\n\
    \n  size_t n,h;\n  BitVector B;\n  std::vector<size_t> RO,Z;\n\n  WaveletMatrix()\
    \ = default;\n  template<typename It>\n  WaveletMatrix(It begin, It end)\n   \
    \   : n(std::distance(begin, end)),\n      h(std::max(1u, 64 - bm::clz(*max_element(begin,\
    \ end)))),\n      B(n*h, false),\n      RO(h+1),\n      Z(h)\n  {\n    using trait\
    \ = std::iterator_traits<It>;\n    static_assert(std::is_base_of<std::input_iterator_tag,\
    \ typename trait::iterator_category>::value, \"\");\n    static_assert(std::is_convertible<typename\
    \ trait::value_type, T>::value, \"\");\n    assert(*min_element(begin, end) >=\
    \ 0);\n\n    std::vector<T> S(begin, end), z, o;\n    z.reserve(n);\n    o.reserve(n);\n\
    \    auto bit = B.begin();\n    for (int k = h-1; k >= 0; k--) {\n      T mask\
    \ = 1ull<<k;\n      for (size_t i = 0; i < n; i++) {\n        bool b = (S[i] &\
    \ mask) != 0;\n        *bit++ = b;\n        if (!b)\n          z.push_back(S[i]);\n\
    \        else\n          o.push_back(S[i]);\n      }\n      Z[k] = n*(h-1-k+1)\
    \ + z.size();\n      int j = n-1;\n      while (!o.empty()) {\n        S[j--]\
    \ = o.back();\n        o.pop_back();\n      }\n      while (!z.empty()) {\n  \
    \      S[j--] = z.back();\n        z.pop_back();\n      }\n    }\n    B.build();\n\
    \    for (size_t i = 0; i <= h; i++)\n      RO[i] = B.rank(n * i);\n  }\n\n  inline\
    \ size_t _child0(size_t level, size_t i) const {\n      return i + n + RO[level]\
    \ - B.rank(i);\n  }\n  inline size_t _child1(size_t level, size_t i) const {\n\
    \    return n*(level+2) + B.rank(i) - RO[level+1];\n  }\n  inline size_t child(size_t\
    \ level, size_t i, bool bit) const {\n    return !bit ? _child0(level, i) : _child1(level,\
    \ i);\n  }\n  std::pair<size_t, size_t> _child_tie0(size_t level, size_t l, size_t\
    \ r) const {\n    return std::make_pair(_child0(level, l), _child0(level, r));\n\
    \  }\n  std::pair<size_t, size_t> _child_tie1(size_t level, size_t l, size_t r)\
    \ const {\n    return std::make_pair(_child1(level, l), _child1(level, r));\n\
    \  }\n  std::pair<size_t, size_t> child_tie(size_t level, size_t l, size_t r,\
    \ bool bit) const {\n    return !bit ? _child_tie0(level, l, r) : _child_tie1(level,\
    \ l, r);\n  }\n\n  inline size_t _parent0(size_t level, size_t i) const {\n  \
    \  return B.select<0>(i - n - RO[level]);\n  }\n  inline size_t _parent1(size_t\
    \ level, size_t i) const {\n    return B.select<1>(i - Z[h-1-level] + RO[level]);\n\
    \  }\n  inline size_t parent(size_t level, size_t i, bool bit) const {\n    return\
    \ !bit ? _parent0(level, i) : _parent1(level, i);\n  }\n\n  T get(size_t i) const\
    \ {\n    T c = 0;\n    size_t j = i;\n    for (int k = h-1; k > 0; k--) {\n  \
    \    bool b = B[j];\n      j = child(h-1-k, j, b);\n      if (b)\n        c |=\
    \ 1ull<<k;\n    }\n    if (B[j])\n      c |= 1u;\n    return c;\n  }\n\n  size_t\
    \ range_rank(T c, size_t l, size_t r) const {\n    for (int k = h-1; k >= 0; k--)\
    \ {\n      if (l == r)\n        break;\n      std::tie(l,r) = child_tie(h-1-k,\
    \ l, r, (c >> k) & 1u);\n    }\n    return r - l;\n  }\n  size_t rank(T c, size_t\
    \ i) const {\n    return range_rank(c, 0, i);\n  }\n  std::tuple<size_t, size_t,\
    \ size_t> rank_3way(T c, size_t l, size_t r) const {\n    size_t lt = 0, gt =\
    \ 0;\n    for (int k = h-1; k >= 0; k--) {\n      size_t pr = r - l;\n      if\
    \ (pr == 0)\n        break;\n      if (((c >> k) & 1u) == 0) {\n        std::tie(l,r)\
    \ = _child_tie0(h-1-k, l, r);\n        gt += pr - (r - l);\n      } else {\n \
    \       std::tie(l,r) = _child_tie1(h-1-k, l, r);\n        lt += pr - (r - l);\n\
    \      }\n    }\n    return std::make_tuple(lt, r - l, gt);\n  }\n\n  // Get frequency\
    \ of values which (x <= value < y) in S[l,r).\n  size_t range_freq(size_t l, size_t\
    \ r, T x, T y) const {\n    size_t freq = 0;\n    std::queue<std::tuple<size_t,size_t,\
    \ T>> qs;\n    qs.emplace(l, r, T(0));\n    while (!qs.empty()) {\n      size_t\
    \ _l,_r;\n      T c;\n      std::tie(_l,_r,c) = qs.front();\n      qs.pop();\n\
    \      size_t level = _l/n;\n      if (_l == _r)\n        continue;\n      int\
    \ shift = h-1-level;\n      T clo = c, chi = c | ((1ull<<(shift+1))-1);\n    \
    \  if (chi < x or y <= clo)\n        continue;\n      if (x <= clo and chi < y)\
    \ {\n        freq += _r - _l;\n        continue;\n      }\n      assert(level\
    \ < h);\n      size_t nl,nr;\n      std::tie(nl,nr) = child_tie(level, _l, _r,\
    \ 0);\n      qs.emplace(nl, nr, c);\n      std::tie(nl,nr) = child_tie(level,\
    \ _l, _r, 1);\n      qs.emplace(nl, nr, c | (1ull << shift));\n    }\n    return\
    \ freq;\n  }\n\n  size_t range_select(T c, size_t l, size_t r, size_t i) const\
    \ {\n    if (r - l <= i)\n      return n;\n    for (int k = h-1; k >= 0; k--)\
    \ {\n      std::tie(l,r) = child_tie(h-1-k, l, r, (c >> k) & 1u);\n      if (r\
    \ - l <= i)\n        return n;\n    }\n    size_t j = l+i;\n    for (size_t k\
    \ = 0; k < h; k++) {\n      j = parent(h-1-k, j, (c >> k) & 1u);\n      assert((bool)((c>>k)&1u)\
    \ == B[j]);\n    }\n    return j;\n  }\n  size_t select(T c, size_t i) const {\n\
    \    return range_select(c, 0, n, i);\n  }\n\n  // Get kth (0-indexed) smallest\
    \ value in S[l,r).\n  T quantile(size_t l, size_t r, size_t k) const {\n    assert(r\
    \ - l > k);\n    T c = 0;\n    for (int d = h-1; d > 0; d--) {\n      auto os\
    \ = B.rank(r) - B.rank(l);\n      auto zs = r - l - os;\n      if (k < zs) {\n\
    \        std::tie(l,r) = _child_tie0(h-1-d, l, r);\n      } else {\n        c\
    \ |= 1ull << d;\n        k -= zs;\n        std::tie(l,r) = _child_tie1(h-1-d,\
    \ l, r);\n      }\n      assert(l < r);\n    }\n    auto os = B.rank(r) - B.rank(l);\n\
    \    auto zs = r - l - os;\n    if (k >= zs) {\n      c |= 1ull;\n    }\n    return\
    \ c;\n  }\n\n  // Get tuples (value, frequency) of the most k frequently occurring\
    \ values in S[l,r).\n  std::vector<std::pair<T, size_t>> top_k(size_t l, size_t\
    \ r, size_t k) const {\n    std::vector<std::pair<T, size_t>> ret;\n    std::priority_queue<std::tuple<size_t,\
    \ size_t, T>> qs;\n    qs.emplace(r-l, l, 0);\n    while (!qs.empty()) {\n   \
    \   size_t range, s;\n      T c;\n      std::tie(range, s, c) = qs.top();\n  \
    \    qs.pop();\n      auto level = s/n;\n      if (level == h) {\n        ret.emplace_back(c,\
    \ range);\n        if (ret.size() == k)\n          break;\n      } else {\n  \
    \      size_t _l, _r;\n        for (int b = 0; b < 2; b++) {\n          std::tie(_l,_r)\
    \ = child_tie(level, s, s+range, b);\n          if (_l != _r)\n            qs.emplace(_r-_l,\
    \ _l, c | (uint64_t(b) << (h-1-level)));\n        }\n      }\n    }\n    return\
    \ ret;\n  }\n  // Get sum of S[l,r) in O(min(r-l, \\sigma) log \\sigma) times.\n\
    \  template<typename U=T>\n  U sum(size_t l, size_t r) const {\n    U ret = 0;\n\
    \    for (auto p : top_k(l, r, r-l))\n      ret += (U) p.first * p.second;\n \
    \   return ret;\n  }\n\n  // Get k tuples of (value, frequency) that value satisfies\
    \ condition (x <= value < y) in S[l,r) from the smallest (or largest).\n  // The\
    \ complexity is O(k log \\sigma).\n  template<bool ASCENDING, bool VALUE_RANGE\
    \ = true>\n  std::vector<std::pair<T, size_t>>\n  range_list_k(size_t l, size_t\
    \ r, size_t k, T x, T y) const {\n    std::vector<std::pair<T, size_t>> ret;\n\
    \    std::queue<std::tuple<size_t, size_t, T>> qs;\n    qs.emplace(r-l, l, T(0));\n\
    \    size_t range,s;\n    T c;\n    while (!qs.empty()) {\n      std::tie(range,s,c)\
    \ = qs.top();\n      qs.pop();\n      auto level = s/n;\n      if (level == h)\
    \ {\n        assert(!VALUE_RANGE or (x <= c and c < y));\n        ret.emplace_back(c,\
    \ range);\n        if (ret.size() == k)\n          break;\n      } else {\n  \
    \      auto shift = (h-1-level);\n        for (int b = ASCENDING ? 0 : 1;\n  \
    \           ASCENDING ? b < 2 : b >= 0;\n             ASCENDING ? b++ : b--) {\n\
    \          T nc = (c << 1) | b;\n          if (VALUE_RANGE and (nc < (x >> shift)\
    \ or ((y-1) >> shift) < nc))\n            continue;\n          size_t nl,nr;\n\
    \          std::tie(nl,nr) = child_tie(level, s, s+range, b);\n          if (nl\
    \ != nr)\n            qs.emplace(nr-nl, nl, nc);\n        }\n      }\n    }\n\
    \    return ret;\n  }\n\n  // Get tuples of (value, frequency) that value satisfies\
    \ condition (x <= value < y) in S[l,r).\n  // The complexity is O(k log \\sigma).\n\
    \  std::vector<std::pair<T, size_t>> range_list(size_t l, size_t r, T x, T y)\
    \ const {\n    return range_list_k<true>(l, r, r - l, x, y);\n  }\n\n  // Get\
    \ k tuples of (value, frequency) that value satisfies condition (x <= value <\
    \ y) in S[l,r) from the largest.\n  // The complexity is O(k log \\sigma).\n \
    \ std::vector<std::pair<T, size_t>> range_max_k(size_t l, size_t r, size_t k)\
    \ const {\n    return range_list_k<false, false>(l, r, k, 0, 0);\n  }\n  // Get\
    \ k tuples of (value, frequency) that value satisfies condition (x <= value <\
    \ y) in S[l,r) from the smallest.\n  // The complexity is O(k log \\sigma).\n\
    \  std::vector<std::pair<T, size_t>> range_min_k(size_t l, size_t r, size_t k)\
    \ const {\n    return range_list_k<true, false>(l, r, k, 0, 0);\n  }\n\n  // Get\
    \ tuples (value, frequency of T1, frequency of T2) that commonly occur between\
    \ T1=S[l1,r1) and T2=S[l2,r2).\n  std::vector<std::tuple<T, size_t, size_t>> intersect(size_t\
    \ l1, size_t r1, size_t l2, size_t r2) const {\n    std::vector<std::tuple<T,\
    \ size_t, size_t>> ret;\n    std::queue<std::pair<std::array<size_t,4>, T>> qs;\n\
    \    qs.emplace({l1,r1,l2,r2}, 0);\n    std::array<size_t,4> nrs{};\n    while\
    \ (!qs.empty()) {\n      const auto& rs = qs.front().first;\n      T c = qs.front().second;\n\
    \      auto level = rs[0]/n;\n      if (level == h) {\n        ret.emplace_back(c,\
    \ rs[1]-rs[0], rs[3]-rs[2]);\n      } else {\n        for (int b = 0; b < 2; b++)\
    \ {\n          for (int i = 0; i < 4; i++)\n            nrs[i] = child(level,\
    \ rs[i], b);\n          if (nrs[0] != nrs[1] and nrs[2] != nrs[3])\n         \
    \   qs.emplace(nrs, c | (uint64_t(b) << (h-1-level)));\n        }\n      }\n \
    \     qs.pop();\n    }\n    return ret;\n  }\n};\n#line 2 \"include/mtl/compress_int.hpp\"\
    \n#include <set>\r\n#include <unordered_map>\r\n#line 6 \"include/mtl/compress_int.hpp\"\
    \n\r\ntemplate<typename T>\r\nstruct Compressor {\r\n  std::vector<T> vs;\r\n\
    \  Compressor() = default;\r\n  template<typename It>\r\n  Compressor(It begin,\
    \ It end) : vs(begin, end) {}\r\n  void clear() { vs.clear(); }\r\n  void add(T\
    \ x) {\r\n    vs.push_back(x);\r\n  }\r\n  template<typename It>\r\n  void add(It\
    \ begin, It end) {\r\n    vs.insert(vs.end(), begin, end);\r\n  }\r\n  using map_type\
    \ = std::unordered_map<T,int>;\r\n  map_type release() {\r\n    std::sort(vs.begin(),\
    \ vs.end());\r\n    vs.erase(std::unique(vs.begin(), vs.end()), vs.end());\r\n\
    \    std::unordered_map<T,int> mp;\r\n    mp.reserve(vs.size());\r\n    int k\
    \ = 0;\r\n    for (auto v : vs) mp[v] = k++;\r\n    return mp;\r\n  }\r\n  std::pair<map_type,\
    \ std::vector<T>> release_tie() {\r\n    return std::make_pair(release(), vs);\r\
    \n  }\r\n  template<typename It>\r\n  static std::unordered_map<T,int> compress(It\
    \ begin, It end) {\r\n    return Compressor(begin, end).release();\r\n  }\r\n\
    };\r\n#line 5 \"include/mtl/fenwick_tree.hpp\"\n\ntemplate <typename T>\nclass\
    \ FenwickTree {\n private:\n  std::vector<T> tree_;\n\n public:\n  FenwickTree()\
    \ = default;\n  explicit FenwickTree(size_t size) : tree_(size+1) {}\n\n  size_t\
    \ size() const { return tree_.size()-1; }\n\n  template <typename Iter>\n  explicit\
    \ FenwickTree(Iter begin, Iter end) : FenwickTree(end-begin) {\n    for (auto\
    \ it = begin; it != end; ++it)\n      add(it-begin, *it);\n  }\n\n  void add(size_t\
    \ index, T x) {\n    for (size_t i = index+1; i < tree_.size(); i += i&(-i))\n\
    \      tree_[i] += x;\n  }\n\n  T sum(size_t index) const {\n    T sum = 0;\n\
    \    for (size_t i = index+1; i > 0; i -= i&(-i))\n      sum += tree_[i];\n  \
    \  return sum;\n  }\n\n  T range_sum(size_t l, size_t r) const {\n    auto sl\
    \ = l > 0 ? sum(l-1) : 0;\n    auto sr = r > 0 ? sum(r-1) : 0;\n    return sr\
    \ - sl;\n  }\n\n  size_t lower_bound(T _sum) const {\n    size_t ret = 0;\n  \
    \  T s = 0;\n    for (int k = 63-bm::clz(size()); k >= 0; k--) {\n      size_t\
    \ j = ret | (1ull<<k);\n      if (j < tree_.size() and s + tree_[j] < _sum) {\n\
    \        s += tree_[j];\n        ret = j;\n      }\n    }\n    return ret;\n \
    \ }\n\n};\n\n#line 10 \"include/mtl/ordinal_range_search.hpp\"\n\r\ntemplate<typename\
    \ T, typename W = long long>\r\nstruct ORS {\r\n  std::vector<std::tuple<T,T,W>>\
    \ ps;\r\n  WaveletMatrix<int> wm;\r\n  FenwickTree<W> rsq;\r\n  std::vector<size_t>\
    \ ls;\r\n  std::vector<T> xs, ys;\r\n  std::unordered_map<T, int> xc, yc;\r\n\r\
    \n  ORS() = default;\r\n  void add(T x, T y, W w = 1) {\r\n    ps.emplace_back(x,y,w);\r\
    \n  }\r\n  void build() {\r\n    std::sort(ps.begin(), ps.end(),\r\n         \
    \     [](auto l, auto r) {\r\n      return std::get<0>(l) != std::get<0>(r) ?\
    \ std::get<0>(l) < std::get<0>(r)\r\n                                : std::get<1>(l)\
    \ < std::get<1>(r);\r\n    });\r\n\r\n    xs.clear();\r\n    std::vector<std::tuple<T,\
    \ size_t, W>> yi;\r\n    for (size_t i = 0; i < ps.size(); i++) {\r\n      T x,y;\r\
    \n      W w;\r\n      std::tie(x,y,w) = ps[i];\r\n      if (xs.empty() or xs.back()\
    \ != x)\r\n        xs.push_back(x);\r\n      yi.emplace_back(y, i, w);\r\n   \
    \ }\r\n    if (xs.empty() or xs.back() != std::numeric_limits<T>::max())\r\n \
    \     xs.push_back(std::numeric_limits<T>::max());\r\n\r\n    xc.clear();\r\n\
    \    xc.reserve(xs.size());\r\n    for (size_t i = 0; i < xs.size(); i++)\r\n\
    \      xc[xs[i]] = i;\r\n\r\n    std::sort(yi.begin(), yi.end());\r\n    ys.resize(yi.size());\r\
    \n    for (size_t i = 0; i < yi.size(); i++)\r\n      ys[i] = std::get<0>(yi[i]);\r\
    \n    ys.push_back(std::numeric_limits<T>::max());\r\n    ys.erase(std::unique(ys.begin(),\
    \ ys.end()), ys.end());\r\n\r\n    yc.clear();\r\n    yc.reserve(ys.size());\r\
    \n    for (size_t i = 0; i < ys.size(); i++)\r\n      yc[ys[i]] = i;\r\n\r\n \
    \   {\r\n      std::vector<int> S(ps.size());\r\n      std::vector<std::pair<int,W>>\
    \ SW(ps.size()),swz,swo;\r\n      for (size_t i = 0; i < ps.size(); i++) {\r\n\
    \        T x,y;\r\n        W w;\r\n        std::tie(x,y,w) = ps[i];\r\n      \
    \  S[i] = yc[y];\r\n        SW[i] = {yc[y], w};\r\n      }\r\n      wm = WaveletMatrix<int>(S.begin(),\
    \ S.end());\r\n      rsq = FenwickTree<W>(wm.n * (wm.h+1));\r\n      for (int\
    \ k = wm.h-1; k >= 0; k--) {\r\n        for (size_t i = 0; i < wm.n; i++) {\r\n\
    \          if (((SW[i].first >> k) & 1u) == 0)\r\n            swz.push_back(SW[i]);\r\
    \n          else\r\n            swo.push_back(SW[i]);\r\n          rsq.add(wm.n\
    \ * (wm.h-1-k) + i, SW[i].second);\r\n        }\r\n        size_t j = wm.n-1;\r\
    \n        while (!swo.empty()) {\r\n          SW[j--] = swo.back();\r\n      \
    \    swo.pop_back();\r\n        }\r\n        while (!swz.empty()) {\r\n      \
    \    SW[j--] = swz.back();\r\n          swz.pop_back();\r\n        }\r\n     \
    \ }\r\n      for (size_t i = 0; i < wm.n; i++) {\r\n        rsq.add(wm.n * wm.h\
    \ + i, SW[i].second);\r\n      }\r\n    }\r\n    ls.resize(xs.size());\r\n   \
    \ int k = 0;\r\n    ls[k++] = 0;\r\n    for (size_t i = 1; i < ps.size(); i++)\
    \ {\r\n      if (std::get<0>(ps[i-1]) != std::get<0>(ps[i]))\r\n        ls[k++]\
    \ = i;\r\n    }\r\n    ls[k] = ps.size();\r\n  }\r\n\r\n private:\r\n  std::tuple<int,\
    \ int, int, int>\r\n  compress_idx(T xl, T xh, T yl, T yh) const {\r\n    auto\
    \ _xl = std::lower_bound(xs.begin(), xs.end(), xl);\r\n    auto _xh = std::lower_bound(_xl,\
    \ xs.end(), xh);\r\n    auto _yl = std::lower_bound(ys.begin(), ys.end(), yl);\r\
    \n    auto _yh = std::lower_bound(_yl, ys.end(), yh);\r\n    return std::make_tuple(ls[_xl-xs.begin()],\
    \ ls[_xh-xs.begin()],\r\n                           (int)(_yl-ys.begin()), (int)(_yh-ys.begin()));\r\
    \n  }\r\n\r\n  template<typename F, typename G>\r\n  void _traverse(T xl, T xh,\
    \ T yl, T yh, F internal_fn, G leaf_fn) const {\r\n    size_t i,j;\r\n    int\
    \ a,b;\r\n    std::tie(i,j,a,b) = compress_idx(xl, xh, yl, yh);\r\n    std::queue<std::tuple<size_t,size_t,T>>\
    \ qs;\r\n    qs.emplace(i,j,T(0));\r\n    while (!qs.empty()) {\r\n      size_t\
    \ l,r;\r\n      T c;\r\n      std::tie(l,r,c) = qs.front(); qs.pop();\r\n    \
    \  size_t level = l/wm.n;\r\n      int shift = wm.h-1-level;\r\n      T clo =\
    \ c, chi = c | ((1ull<<(shift+1))-1);\r\n      if (chi < a or b <= clo)\r\n  \
    \      continue;\r\n      if (a <= clo and chi < b) {\r\n        internal_fn(l,\
    \ r, c);\r\n        continue;\r\n      }\r\n      if (level == wm.h) {\r\n   \
    \     leaf_fn(l, r, c);\r\n        continue;\r\n      }\r\n      size_t nl,nr;\r\
    \n      std::tie(nl,nr) = wm._child_tie0(level, l, r);\r\n      qs.emplace(nl,\
    \ nr, c);\r\n      std::tie(nl,nr) = wm._child_tie1(level, l, r);\r\n      qs.emplace(nl,\
    \ nr, c | (1ull<<shift));\r\n    }\r\n  }\r\n\r\n public:\r\n  size_t freq(T xl,\
    \ T xh, T yl, T yh) const {\r\n    size_t i,j;\r\n    int a,b;\r\n    std::tie(i,j,a,b)\
    \ = compress_idx(xl, xh, yl, yh);\r\n    return wm.range_freq(i, j, a, b);\r\n\
    \  }\r\n\r\n  void weight_add(T x, T y, W w) {\r\n    assert(xc.count(x));\r\n\
    \    assert(yc.count(y));\r\n    int cx = xc[x];\r\n    int l = ls[cx], r = ls[cx+1];\r\
    \n    int c = yc[y];\r\n    for (int k = wm.h-1; k >= 0; k--)\r\n      std::tie(l,r)\
    \ = wm.child_tie(wm.h-1-k, l, r, (c >> k) & 1u);\r\n    assert(l != r);\r\n  \
    \  size_t j = l;\r\n    for (size_t k = 0; k < wm.h; k++) {\r\n      rsq.add(j,\
    \ w);\r\n      j = wm.parent(wm.h-1-k, j, (c >> k) & 1u);\r\n    }\r\n    rsq.add(j,\
    \ w);\r\n  }\r\n\r\n  W sum(T xl, T xh, T yl, T yh) const {\r\n    W ret = 0;\r\
    \n    _traverse(xl, xh, yl, yh,\r\n              [&](size_t l, size_t r, auto\
    \ _) { ret += rsq.range_sum(l, r); },\r\n              [](auto,auto,auto){});\r\
    \n    return ret;\r\n  }\r\n\r\n  std::vector<std::pair<size_t, size_t>>\r\n \
    \ enumerate(T xl, T xh, T yl, T yh) const {\r\n    std::vector<std::pair<size_t,size_t>>\
    \ ret;\r\n    _traverse(xl, xh, yl, yh,\r\n              [](auto,auto,auto){},\r\
    \n              [&](size_t l, size_t r, T c) {\r\n      for (size_t idx = l; idx\
    \ < r; idx++) {\r\n        auto jdx = idx;\r\n        for (size_t k = 0; k < wm.h;\
    \ k++)\r\n          jdx = wm.parent(wm.h-1-k, jdx, (c >> k) & 1u);\r\n       \
    \ auto lit = upper_bound(ls.begin(), ls.end(), jdx);\r\n        ret.emplace_back(xs[lit-1-ls.begin()],\
    \ ys[c]);\r\n      }\r\n    });\r\n    return ret;\r\n  }\r\n};\n#line 3 \"test/yosupo-point_add_rectangle_sum-wm.test.cpp\"\
    \n#include <bits/stdc++.h>\r\nusing namespace std;\r\n\r\nint main() {\r\n  int\
    \ n,q; cin>>n>>q;\r\n  ORS<int, long long> ors;\r\n  for (int i = 0; i < n; i++)\
    \ {\r\n    int x,y,w; cin>>x>>y>>w;\r\n    ors.add(x,y,w);\r\n  }\r\n  vector<tuple<int,int,int,int,int>>\
    \ Q(q);\r\n  for (int i = 0; i < q; i++) {\r\n    int t; cin>>t;\r\n    if (t\
    \ == 0) {\r\n      int x,y,w; cin>>x>>y>>w;\r\n      Q[i] = {0,x,y,w,0};\r\n \
    \     ors.add(x,y,0);\r\n    } else {\r\n      int l,d,r,u; cin>>l>>d>>r>>u;\r\
    \n      Q[i] = {1,l,d,r,u};\r\n    }\r\n  }\r\n  ors.build();\r\n  for (int i\
    \ = 0; i < q; i++) {\r\n    int t = get<0>(Q[i]);\r\n    if (t == 0) {\r\n   \
    \   int x,y,w,_;\r\n      std::tie(t,x,y,w,_) = Q[i];\r\n      ors.weight_add(x,y,w);\r\
    \n    } else {\r\n      int l,d,r,u;\r\n      std::tie(t,l,d,r,u) = Q[i];\r\n\
    \      cout << ors.sum(l,r,d,u) << endl;\r\n    }\r\n  }\r\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/point_add_rectangle_sum\"\
    \r\n#include \"../include/mtl/ordinal_range_search.hpp\"\r\n#include <bits/stdc++.h>\r\
    \nusing namespace std;\r\n\r\nint main() {\r\n  int n,q; cin>>n>>q;\r\n  ORS<int,\
    \ long long> ors;\r\n  for (int i = 0; i < n; i++) {\r\n    int x,y,w; cin>>x>>y>>w;\r\
    \n    ors.add(x,y,w);\r\n  }\r\n  vector<tuple<int,int,int,int,int>> Q(q);\r\n\
    \  for (int i = 0; i < q; i++) {\r\n    int t; cin>>t;\r\n    if (t == 0) {\r\n\
    \      int x,y,w; cin>>x>>y>>w;\r\n      Q[i] = {0,x,y,w,0};\r\n      ors.add(x,y,0);\r\
    \n    } else {\r\n      int l,d,r,u; cin>>l>>d>>r>>u;\r\n      Q[i] = {1,l,d,r,u};\r\
    \n    }\r\n  }\r\n  ors.build();\r\n  for (int i = 0; i < q; i++) {\r\n    int\
    \ t = get<0>(Q[i]);\r\n    if (t == 0) {\r\n      int x,y,w,_;\r\n      std::tie(t,x,y,w,_)\
    \ = Q[i];\r\n      ors.weight_add(x,y,w);\r\n    } else {\r\n      int l,d,r,u;\r\
    \n      std::tie(t,l,d,r,u) = Q[i];\r\n      cout << ors.sum(l,r,d,u) << endl;\r\
    \n    }\r\n  }\r\n}"
  dependsOn:
  - include/mtl/ordinal_range_search.hpp
  - include/mtl/succinct/wavelet_matrix.hpp
  - include/mtl/succinct/bit_vector.hpp
  - include/mtl/bit_manip.hpp
  - include/mtl/compress_int.hpp
  - include/mtl/fenwick_tree.hpp
  isVerificationFile: true
  path: test/yosupo-point_add_rectangle_sum-wm.test.cpp
  requiredBy: []
  timestamp: '2023-04-04 01:01:39+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/yosupo-point_add_rectangle_sum-wm.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo-point_add_rectangle_sum-wm.test.cpp
- /verify/test/yosupo-point_add_rectangle_sum-wm.test.cpp.html
title: test/yosupo-point_add_rectangle_sum-wm.test.cpp
---
