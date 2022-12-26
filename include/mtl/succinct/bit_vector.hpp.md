---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: include/mtl/ordinal_range_search.hpp
    title: include/mtl/ordinal_range_search.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/wavelet_matrix.hpp
    title: include/mtl/succinct/wavelet_matrix.hpp
  - icon: ':warning:'
    path: include/mtl/succinct/wavelet_tree.hpp
    title: include/mtl/succinct/wavelet_tree.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/succinct/range_kth_smallest.test.cpp
    title: test/succinct/range_kth_smallest.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo-point_add_rectangle_sum-wm.test.cpp
    title: test/yosupo-point_add_rectangle_sum-wm.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/succinct/bit_vector.hpp\"\n#include <cstddef>\r\
    \n#include <cstdint>\r\n#include <vector>\r\n#include <iostream>\r\n#include <array>\r\
    \n#include <bitset>\r\n#line 3 \"include/mtl/bit_manip.hpp\"\n#include <cassert>\n\
    \nnamespace bm {\n\ninline constexpr uint64_t popcnt_e8(uint64_t x) {\n  x = (x\
    \ & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);\n  x = (x & 0x3333333333333333)\
    \ + ((x>>2) & 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) &\
    \ 0x0F0F0F0F0F0F0F0F);\n  return x;\n}\n// Count 1s\ninline constexpr unsigned\
    \ popcnt(uint64_t x) {\n  return (popcnt_e8(x) * 0x0101010101010101) >> 56;\n\
    }\n// Count trailing 0s. ...01101000 -> 3\ninline constexpr unsigned ctz(uint64_t\
    \ x) {\n  return popcnt((x & (-x)) - 1);\n}\ninline constexpr unsigned ctz8(uint8_t\
    \ x) {\n  return x == 0 ? 8 : popcnt_e8((x & (-x)) - 1);\n}\n// [00..0](8bit)\
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
    \ 9 \"include/mtl/succinct/bit_vector.hpp\"\n\r\n// Bitmap is likes std::vector<bool>\r\
    \nstruct Bitmap {\r\n  using value_type = bool;\r\n  using W = uint64_t;\r\n \
    \ std::vector<W> arr;\r\n  size_t sz;\r\n  static constexpr size_t required_word_size(size_t\
    \ n) {\r\n    return (n+63) / 64;\r\n  }\r\n  static constexpr W word_filled_by(bool\
    \ bit) {\r\n    return bit ? 0xFFFFFFFFFFFFFFFF : 0;\r\n  }\r\n  explicit Bitmap(size_t\
    \ n = 0, bool bit = false) : arr(required_word_size(n), word_filled_by(bit)),\
    \ sz(n) {}\r\n  template<typename It>\r\n  Bitmap(It begin, It end) : sz(0) {\r\
    \n    using trait = std::iterator_traits<It>;\r\n    using iterator_category =\
    \ typename trait::iterator_category;\r\n    static_assert(std::is_base_of<std::input_iterator_tag,\
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
    \n    return l * L + s * S + b * 8 + sel_tb[byte][r];\r\n  }\r\n};\n"
  code: "#pragma once\r\n#include <cstddef>\r\n#include <cstdint>\r\n#include <vector>\r\
    \n#include <iostream>\r\n#include <array>\r\n#include <bitset>\r\n#include \"\
    ../bit_manip.hpp\"\r\n\r\n// Bitmap is likes std::vector<bool>\r\nstruct Bitmap\
    \ {\r\n  using value_type = bool;\r\n  using W = uint64_t;\r\n  std::vector<W>\
    \ arr;\r\n  size_t sz;\r\n  static constexpr size_t required_word_size(size_t\
    \ n) {\r\n    return (n+63) / 64;\r\n  }\r\n  static constexpr W word_filled_by(bool\
    \ bit) {\r\n    return bit ? 0xFFFFFFFFFFFFFFFF : 0;\r\n  }\r\n  explicit Bitmap(size_t\
    \ n = 0, bool bit = false) : arr(required_word_size(n), word_filled_by(bit)),\
    \ sz(n) {}\r\n  template<typename It>\r\n  Bitmap(It begin, It end) : sz(0) {\r\
    \n    using trait = std::iterator_traits<It>;\r\n    using iterator_category =\
    \ typename trait::iterator_category;\r\n    static_assert(std::is_base_of<std::input_iterator_tag,\
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
    \n    return l * L + s * S + b * 8 + sel_tb[byte][r];\r\n  }\r\n};"
  dependsOn:
  - include/mtl/bit_manip.hpp
  isVerificationFile: false
  path: include/mtl/succinct/bit_vector.hpp
  requiredBy:
  - include/mtl/succinct/wavelet_matrix.hpp
  - include/mtl/succinct/wavelet_tree.hpp
  - include/mtl/ordinal_range_search.hpp
  timestamp: '2022-12-18 04:26:00+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/succinct/range_kth_smallest.test.cpp
  - test/yosupo-point_add_rectangle_sum-wm.test.cpp
documentation_of: include/mtl/succinct/bit_vector.hpp
layout: document
redirect_from:
- /library/include/mtl/succinct/bit_vector.hpp
- /library/include/mtl/succinct/bit_vector.hpp.html
title: include/mtl/succinct/bit_vector.hpp
---
