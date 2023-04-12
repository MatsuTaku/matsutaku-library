---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/bit_vector.hpp
    title: include/mtl/succinct/bit_vector.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/bv.hpp
    title: include/mtl/succinct/bv.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/succinct/select.hpp
    title: include/mtl/succinct/select.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/succinct/wavelet_tree.hpp\"\n#include <limits>\r\
    \n#include <array>\r\n#include <iterator>\r\n#include <algorithm>\r\n#include\
    \ <queue>\r\n#include <tuple>\r\n#line 2 \"include/mtl/bit_manip.hpp\"\n#include\
    \ <cstdint>\n#include <cassert>\n\nnamespace bm {\n\ninline constexpr uint64_t\
    \ popcnt_e8(uint64_t x) {\n  x = (x & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);\n\
    \  x = (x & 0x3333333333333333) + ((x>>2) & 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F)\
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
    \ 4 \"include/mtl/succinct/select.hpp\"\n\nstruct select64 {\n    using size_type\
    \ = uint8_t;\n    static struct make_select_table {\n        using bitmap_type\
    \ = uint8_t;\n        std::array<std::array<size_type, 9>, 1<<8> tb;\n       \
    \ make_select_table() {\n            for (int i = 0; i < 1<<8; i++) {\n      \
    \          int c = 0;\n                int x = i;\n                tb[i].fill(8);\n\
    \                for (int j = 0; j < 8; j++) {\n                    if (x & 1)\n\
    \                    tb[i][++c] = j;\n                    x >>= 1;\n         \
    \       }\n            }\n        }\n        size_type operator()(bitmap_type\
    \ bitmap, size_type ith) const {\n            return tb[bitmap][ith];\n      \
    \  }\n    } sel_tb;\n    template<bool B>\n    static constexpr size_type select(size_type\
    \ ith, uint64_t bitmap) { // 0-indexed\n        assert(ith < 64);\n        ith++;\
    \ // to 1-index\n        // Find byte\n        uint64_t w = bitmap;\n        if\
    \ constexpr (!B) w = ~w;\n        auto _bs = (uint64_t) bm::popcnt_e8(w) * 0x0101010101010100ull;\n\
    \        auto bs = (const uint8_t*) &_bs;\n        size_type b = 0;\n        auto\
    \ o = ith;\n        /* Following bit-manipulates code is same as ... */\n    \
    \    // {\n        //     auto d = 8;\n        //     while (d > 1) {\n      \
    \  //     auto c = b + d/2;\n        //     if (bs[c] < o)\n        //       \
    \  b = c;\n        //     d /= 2;\n        //     }\n        // }\n        {\n\
    \            uint64_t x = (uint64_t) o * 0x0101010101010101ull;\n            uint64_t\
    \ bmx = (_bs | 0x8080808080808080ull) - x;\n            uint64_t y = ((bmx & 0x8080808080808080ull)\
    \ * 0x02040810204081ull) >> (64-8);\n            size_type nb = bm::ctz8(y) -\
    \ 1;\n            // assert(b == nb);\n            b = nb;\n        }\n      \
    \  // Calc select\n        auto r = o - bs[b];\n        uint8_t byte = ((const\
    \ uint8_t*)(&w))[b];\n        assert(r and r <= (size_type)bm::popcnt(byte));\n\
    \        return b * 8 + sel_tb(byte, r);\n    }\n    static constexpr size_type\
    \ select1(size_type ith, uint64_t bitmap) {\n        return select<1>(ith, bitmap);\n\
    \    }\n    static constexpr size_type select0(size_type ith, uint64_t bitmap)\
    \ {\n        return select<0>(ith, bitmap);\n    }\n};\n\ntypename select64::make_select_table\
    \ select64::sel_tb;\n#line 2 \"include/mtl/succinct/bv.hpp\"\n#include <vector>\n\
    #include <cstddef>\n#line 5 \"include/mtl/succinct/bv.hpp\"\n#include <bitset>\n\
    #include <iostream>\n\n#if __cpp_concepts >= 202002L\n#include <concepts>\ntemplate<class\
    \ T>\nconcept ConstructableBV = requires(T t, size_t s) {\n  { t.size() } -> std::same_as<size_t>;\n\
    \  { t.word_size() } -> std::same_as<size_t>;\n  { t.get_word(s) } -> std::convertible_to<uint64_t>;\n\
    };\n#endif\n\ntemplate<\n#if __cpp_concepts >= 202002L\n  ConstructableBV\n#else\n\
    \  class \n#endif\n    BitVec, unsigned WordSize>\nstruct BV {\n  static_assert(WordSize\
    \ <= 64, \"WordSize must be <= 64\");\n  static constexpr unsigned S = WordSize;\n\
    \  static constexpr unsigned S_PER_L = 8;\n  static constexpr unsigned L = S*S_PER_L;\n\
    \  using bitvec_type = BitVec;\n  const bitvec_type* bm_;\n  std::vector<uint64_t>\
    \ _r, _s, _zs;\n  \n  BV() = default;\n  BV(const bitvec_type* bm) {\n    build(bm);\n\
    \  }\n  void set_ptr(const bitvec_type* bm) {\n    bm_ = bm;\n  }\n  void build(const\
    \ bitvec_type* bm) {\n    set_ptr(bm);\n    const auto num_l = (bm->size() + L-1)\
    \ / L;\n    _r.assign((num_l + 1) * 2, 0);\n    _s.clear();\n    _s.push_back(0);\n\
    \    _zs.clear();\n    _zs.push_back(0);\n    uint64_t sum = 0;\n    for (size_t\
    \ l = 0; l < num_l; ++l) {\n      auto psum = sum;\n      uint64_t sum_l = 0;\n\
    \      for (size_t s = 0; s < S_PER_L; ++s) {\n        if (l * S_PER_L + s < bm->word_size())\n\
    \          sum_l += bm::popcnt(bm->get_word(l * S_PER_L + s));\n        if (s\
    \ < S_PER_L-1)\n          _r[l * 2 + 1] |= sum_l << ((7-(s+1)) * 9);\n      }\n\
    \      sum += sum_l;\n      _r[(l + 1) * 2] = sum;\n      if (psum / L != sum\
    \ / L) {\n        _s.push_back(l);\n      }\n      if ((L*l - psum) / L != (L*(l+1)\
    \ - sum) / L) {\n        _zs.push_back(l);\n      }\n    }\n    _s.push_back(num_l);\n\
    \    _zs.push_back(num_l);\n  }\n\n  template<bool B>\n  size_t get_l(size_t l)\
    \ const {\n    auto b = _r[l*2];\n    return B ? b : l * L - b;\n  }\n  static\
    \ size_t s_off(size_t s) {\n    return (7-s) * 9;\n  }\n  template<bool B>\n \
    \ size_t get_s(size_t l, size_t s) const {\n    auto b = (_r[l*2+1] >> s_off(s))\
    \ % (1ull<<9);\n    return B ? b : s * S - b;\n  }\n  uint64_t mask(size_t width)\
    \ const {\n    return width == 64 ? ~0ull : (1ull << width) - 1;\n  }\n  size_t\
    \ rank1(size_t i) const {\n    auto l = i / L;\n    auto s = i % L / S;\n    auto\
    \ q = i / S;\n    auto r = i % S;\n    auto w = bm_->get_word(q) & mask(r);\n\
    \    return get_l<1>(l) + \n           get_s<1>(l, s) + \n           bm::popcnt(w);\n\
    \  }\n  size_t rank0(size_t i) const { \n    return i - rank1(i);\n  }\n  template<bool\
    \ B>\n  size_t rank(size_t i) const {\n    if constexpr (B)\n      return rank1(i);\n\
    \    else\n      return rank0(i);\n  }\n\n  static struct l_block_cap_mask {\n\
    \    uint64_t mask;\n    constexpr l_block_cap_mask() {\n      mask = 0;\n   \
    \   for (unsigned i = 0; i < S_PER_L; i++) {\n        uint64_t cap = i * S;\n\
    \        mask |= cap << s_off(i);\n      }\n    }\n  } l_block_cap;\n\n  template<bool\
    \ B>\n  size_t select(size_t ith) const {\n    auto n = ith+1; // to 1-indexed\n\
    \    if (n > rank<B>(bm_->size()))\n      return bm_->size();\n    // Find L block\n\
    \    auto& idx = B ? _s : _zs;\n    size_t l = idx[n / L];\n    {\n      auto\
    \ r = idx[n / L + 1] + 1;\n      while (l+1 < r) {\n        auto c = l + (r-l)/2;\n\
    \        if (get_l<B>(c) < n)\n          l = c;\n        else\n          r = c;\n\
    \      }\n    }\n    // Find S block\n    size_t s = 0;\n    auto m = n - get_l<B>(l);\n\
    \    /* Following bit-manipulates code is same as ... */\n//    {\n//      auto\
    \ d = 8;\n//      while (d > 1) {\n//        auto c = s + d/2;\n//        if (get_s<B>(l,\
    \ c) < m)\n//          s = c;\n//        d /= 2;\n//      }\n//    }\n    {\n\
    \      uint64_t x = (uint64_t) (m-1) * 0x0040201008040201ull;\n      uint64_t\
    \ a = _r[l*2+1];\n      if constexpr (!B)\n        a = l_block_cap.mask - a; //\
    \ to 0s sum\n      uint64_t xda = x - a;\n      uint64_t sm = 0x4020100804020100ull;\n\
    \      uint64_t ok = (~x | a) & sm;\n      uint64_t ng = (~x & a) & sm;\n    \
    \  uint64_t y = ((x ^ a ^ xda) & ok) | ng;\n      y = y * 0x0001010101010101ull\
    \ >> (64-1-7);\n      auto id = bm::clz8(y)-1;\n      auto clo = bm::clz((~xda\
    \ << 1 | 1) << (9*id));\n      auto ns = id + (clo ? (clo - 1) / 9 : 0);\n   \
    \   s = ns;\n    }\n    // Calc select\n    auto o = m - get_s<B>(l, s);\n   \
    \ uint64_t w = bm_->get_word(l * S_PER_L + s);\n    return l * L + \n        \
    \   s * S + \n           select64::select<B>(o-1, w);\n  }\n  size_t select1(size_t\
    \ ith) const {\n    return select<1>(ith);\n  }\n  size_t select0(size_t ith)\
    \ const {\n    return select<0>(ith);\n  }\n};\n\ntemplate<class BitVec, unsigned\
    \ WordSize>\ntypename BV<BitVec, WordSize>::l_block_cap_mask BV<BitVec, WordSize>::l_block_cap;\n\
    \ntemplate<class T>\nstruct RankSelectTraits : std::false_type {};\n#line 11 \"\
    include/mtl/succinct/bit_vector.hpp\"\n\r\n// Bitmap is likes std::vector<bool>\r\
    \nstruct Bitmap {\r\n  using value_type = bool;\r\n  using W = uint64_t;\r\n \
    \ std::vector<W> arr;\r\n  size_t sz;\r\n  static constexpr size_t required_word_size(size_t\
    \ n) {\r\n    return (n+63) / 64;\r\n  }\r\n  static constexpr W word_filled_by(bool\
    \ bit) {\r\n    return bit ? 0xFFFFFFFFFFFFFFFF : 0;\r\n  }\r\n  explicit Bitmap(size_t\
    \ n = 0, bool bit = false) \r\n    : arr(required_word_size(n), word_filled_by(bit)),\
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
    \ --sz;\r\n  }\r\n  void resize(size_t new_size, bool bit=false) { // TODO: fix\
    \ when bit = true\r\n    auto old_size = size();\r\n    sz = new_size;\r\n   \
    \ if (new_size < old_size) {\r\n      return;\r\n    }\r\n    arr.resize(required_word_size(new_size),\
    \ word_filled_by(bit));\r\n    auto r = old_size % 64;\r\n    if (r) {\r\n   \
    \   W mask = (1ull << r) - 1;\r\n      if (bit)\r\n        arr[old_size / 64]\
    \ |= ~mask;\r\n      else\r\n        arr[old_size / 64] &= mask;\r\n    }\r\n\
    \  }\r\n  void assign(size_t new_size, bool bit) {\r\n    sz = new_size;\r\n \
    \   arr.assign(required_word_size(new_size), word_filled_by(bit));\r\n  }\r\n\
    \  void reserve(size_t reserved_size) {\r\n    arr.reserve(required_word_size(reserved_size));\r\
    \n  }\r\n\r\n  struct const_reference;\r\n  struct reference;\r\n  template<bool>\r\
    \n  struct _iterator;\r\n  using const_iterator = _iterator<true>;\r\n  using\
    \ iterator = _iterator<false>;\r\n\r\n  const_reference operator[](size_t i) const\
    \ {\r\n    return const_reference(arr.data() + i / 64, 1ull << (i % 64));\r\n\
    \  }\r\n  reference operator[](size_t i) {\r\n    return {arr.data() + i / 64,\
    \ 1ull << (i % 64)};\r\n  }\r\n  const_reference get(size_t i) const {\r\n   \
    \ return operator[](i);\r\n  }\r\n  /**\r\n   * Usable without pre-set required\
    \ size\r\n  */\r\n  void set(size_t i, bool b) {\r\n    if (i >= size())\r\n \
    \     resize(i + 1);\r\n    operator[](i) = b;\r\n  }\r\n  /**\r\n   * No build\
    \ process is needed\r\n  */\r\n  void build() const {}\r\n  const_iterator begin()\
    \ const { return const_iterator(arr.data(), 0); }\r\n  iterator begin() { return\
    \ iterator(arr.data(), 0); }\r\n  const_iterator cbegin() const { return begin();\
    \ }\r\n  const_iterator end() const { return const_iterator(arr.data() + sz /\
    \ 64, sz % 64); }\r\n  iterator end() { return iterator(arr.data() + sz / 64,\
    \ sz % 64); }\r\n  const_iterator cend() const { return end(); }\r\n\r\n  template<bool\
    \ Const>\r\n  struct reference_base {\r\n    using _pointer = typename std::conditional<Const,\
    \ const W*, W*>::type;\r\n    using _iterator = typename std::conditional<Const,\
    \ const_iterator, iterator>::type;\r\n    _pointer ptr;\r\n    W mask;\r\n   \
    \ reference_base(_pointer ptr, W mask) : ptr(ptr), mask(mask) {}\r\n    reference_base(const\
    \ reference_base&) = delete;\r\n    reference_base& operator=(const reference_base&)\
    \ = delete;\r\n    reference_base(reference_base&&) noexcept = default;\r\n  \
    \  reference_base& operator=(reference_base&&) noexcept = default;\r\n    inline\
    \ operator bool() const {\r\n      return (*ptr & mask) != 0;\r\n    }\r\n   \
    \ inline bool operator==(bool r) const {\r\n      return (bool) *this == r;\r\n\
    \    }\r\n    inline friend bool operator==(bool l, const reference_base& r) {\r\
    \n      return r == l;\r\n    }\r\n    inline bool operator!=(bool r) const {\r\
    \n      return (bool) *this != r;\r\n    }\r\n    inline friend bool operator!=(bool\
    \ l, const reference_base& r) {\r\n      return r != l;\r\n    }\r\n    _iterator\
    \ operator&() const {\r\n      return {ptr, bm::ctz(mask)};\r\n    }\r\n    std::ostream&\
    \ operator<<(std::ostream& os) const {\r\n      return os << (bool) *this;\r\n\
    \    }\r\n  };\r\n  struct const_reference : public reference_base<true> {\r\n\
    \    using _base = reference_base<true>;\r\n    const_reference(_base::_pointer\
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
    \n\r\n  void range_set(size_t b, size_t e, uint64_t x) {\r\n    if (b >= e) return;\r\
    \n    auto r = b % 64;\r\n    auto w = e-b;\r\n    auto mask = w < 64 ? (1ull\
    \ << w) - 1 : ~0ull;\r\n    assert(x <= mask);\r\n    arr[b/64] = (arr[b/64] &\
    \ ~(mask << r)) | x << r;\r\n    if (mask + r > 64) {\r\n      arr[b/64+1] = (arr[b/64+1]\
    \ & ~(mask >> (64-r))) | x >> (64-r);\r\n    }\r\n  }\r\n  uint64_t range_get(size_t\
    \ b, size_t e) const {\r\n    if (b >= e) return 0;\r\n    assert(e-b <= 64);\r\
    \n    auto r = b % 64;\r\n    auto w = e-b;\r\n    auto mask = w < 64 ? (1ull\
    \ << w) - 1 : ~0ull;\r\n    auto x = arr[b/64] >> r;\r\n    if (w + r > 64) \r\
    \n      x |= arr[b/64+1] << (64-r);\r\n    return x & mask;\r\n  }\r\n  const\
    \ uint64_t& get_word(size_t wi) const {\r\n    return arr[wi];\r\n  }\r\n  size_t\
    \ word_size() const {\r\n    return arr.size();\r\n  }\r\n  // using rank_select_type\
    \ = BV<Bitmap, 64>;\r\n};\r\n\r\ntemplate<>\r\nstruct RankSelectTraits<Bitmap>\
    \ {\r\n  using rank_select_type = BV<Bitmap, 64>;\r\n};\r\n\r\nstruct BitVector\
    \ {\r\n  Bitmap bm;\r\n  using rs_type = typename RankSelectTraits<Bitmap>::rank_select_type;\r\
    \n  rs_type rs_support;\r\n  // std::vector<uint64_t> _r, _s, _zs;\r\n\r\n  BitVector(size_t\
    \ size = 0, bool bit = false) : bm(size, bit) {}\r\n  BitVector(const BitVector&\
    \ rhs) : bm(rhs.bm), rs_support(rhs.rs_support) {\r\n    rs_support.set_ptr(&bm);\r\
    \n  }\r\n  BitVector& operator=(const BitVector& rhs) {\r\n    bm = rhs.bm;\r\n\
    \    rs_support = rhs.rs_support;\r\n    rs_support.set_ptr(&bm);\r\n    return\
    \ *this;\r\n  }\r\n  BitVector(BitVector&& rhs) noexcept : \r\n    bm(std::move(rhs.bm)),\
    \ \r\n    rs_support(std::move(rhs.rs_support)) {\r\n    rs_support.set_ptr(&bm);\r\
    \n  }\r\n  BitVector& operator=(BitVector&& rhs) noexcept {\r\n    bm = std::move(rhs.bm);\r\
    \n    rs_support = std::move(rhs.rs_support);\r\n    rs_support.set_ptr(&bm);\r\
    \n    return *this;\r\n  }\r\n  template<typename It>\r\n  BitVector(It begin,\
    \ It end) : bm(begin, end) {\r\n    build();\r\n  }\r\n  size_t size() const {\
    \ return bm.size(); }\r\n  bool empty() const { return bm.empty(); }\r\n  void\
    \ push_back(bool bit) { bm.push_back(bit); }\r\n  void resize(size_t new_size,\
    \ bool bit) { bm.resize(new_size, bit); }\r\n  void assign(size_t new_size, bool\
    \ bit) { bm.assign(new_size, bit); }\r\n  void reserve(size_t reserved_size) {\
    \ bm.reserve(reserved_size); }\r\n  Bitmap::const_reference operator[](size_t\
    \ i) const { return bm[i]; }\r\n  Bitmap::reference operator[](size_t i) { return\
    \ bm[i]; }\r\n  Bitmap::const_iterator begin() const { return bm.begin(); }\r\n\
    \  Bitmap::const_iterator end() const { return bm.end(); }\r\n  Bitmap::iterator\
    \ begin() { return bm.begin(); }\r\n  Bitmap::iterator end() { return bm.end();\
    \ }\r\n  Bitmap::const_iterator cbegin() const { return bm.cbegin(); }\r\n  Bitmap::const_iterator\
    \ cend() const { return bm.cend(); }\r\n\r\n  void build() {\r\n    rs_support.build(&bm);\r\
    \n  }\r\n\r\n  inline size_t rank(size_t i) const {\r\n    return rs_support.rank1(i);\r\
    \n  }\r\n\r\n  template<bool B>\r\n  size_t select(size_t n) const {\r\n    return\
    \ rs_support.select<B>(n);\r\n  }\r\n};\n#line 10 \"include/mtl/succinct/wavelet_tree.hpp\"\
    \n\r\ntemplate<typename T>\r\nstruct WaveletTree {\r\n  static constexpr unsigned\
    \ H = 64 - bm::clz(std::numeric_limits<T>::max());\r\n  size_t n,h;\r\n  BitVector\
    \ B;\r\n  WaveletTree() = default;\r\n  template<typename It>\r\n  WaveletTree(It\
    \ begin, It end)\r\n    : n(std::distance(begin, end)), h(64 - bm::clz(*max_element(begin,\
    \ end)))\r\n  {\r\n    using trait = std::iterator_traits<It>;\r\n    static_assert(std::is_base_of<std::input_iterator_tag,\
    \ typename trait::iterator_category>::value, \"\");\r\n    static_assert(std::is_convertible<typename\
    \ trait::value_type, T>::value, \"\");\r\n    assert(*min_element(begin, end)\
    \ >= 0);\r\n\r\n    B.assign(h * n, false);\r\n    std::vector<T> S(begin, end);\r\
    \n    std::vector<size_t> D = {0,n}, nd;\r\n    auto bit = B.begin();\r\n    std::queue<T>\
    \ sz,so;\r\n    for (int k = h-1; k > 0; k--) {\r\n      for (size_t i = 0; i\
    \ < n; i++, ++bit)\r\n        *bit = (S[i] & (1ull<<k)) != 0;\r\n      nd.clear();\r\
    \n      nd.push_back(0);\r\n      for (size_t d = 0; d < D.size()-1; ++d) {\r\n\
    \        size_t l = D[d], r = D[d+1];\r\n        for (size_t i = l; i < r; ++i)\r\
    \n          if ((S[i] & (1ull<<k)) == 0)\r\n            sz.push(S[i]);\r\n   \
    \       else\r\n            so.push(S[i]);\r\n        int zs = sz.size();\r\n\
    \        {\r\n          auto i = l;\r\n          while (!sz.empty()) {\r\n   \
    \         S[i++] = sz.front();\r\n            sz.pop();\r\n          }\r\n   \
    \       while (!so.empty()) {\r\n            S[i++] = so.front();\r\n        \
    \    so.pop();\r\n          }\r\n          assert(i == r);\r\n        }\r\n  \
    \      if (zs)\r\n          nd.push_back(l + zs);\r\n        if (l + zs < r)\r\
    \n          nd.push_back(r);\r\n      }\r\n      std::swap(D, nd);\r\n    }\r\n\
    \    for (size_t i = 0; i < n; i++, ++bit)\r\n      *bit = (S[i] & 1ull) != 0;\r\
    \n    B.build();\r\n  }\r\n\r\n  std::pair<size_t,size_t> child(bool b, size_t\
    \ l, size_t r) {\r\n    auto os = B.rank(r) - B.rank(l);\r\n    auto zs = r-l\
    \ - os;\r\n    return !b ? std::make_pair(l + n, l + zs + n)\r\n             \
    \ : std::make_pair(l + zs + n, r + n);\r\n  }\r\n\r\n  T get(size_t i) const {\r\
    \n    size_t l = 0, r = n;\r\n    T ret = 0;\r\n    auto j = i;\r\n    for (int\
    \ k = h-1; k > 0; k--) {\r\n      auto rl = B.rank(l),\r\n          rj = B.rank(l\
    \ + j),\r\n          rr = B.rank(r);\r\n      auto zs = r - l - (rr - rl);\r\n\
    \      if (B[l + j]) {\r\n        ret |= 1ull<<k;\r\n        j = rj - rl;\r\n\
    \        l += n + zs;\r\n        r += n;\r\n      } else {\r\n        j = (l+j\
    \ - rj) - (l - rl);\r\n        l += n;\r\n        r = l + zs;\r\n      }\r\n \
    \   }\r\n    if (B[l + j])\r\n      ret |= 1ull;\r\n    return ret;\r\n  }\r\n\
    \r\n  size_t rank(T c, size_t i) const {\r\n    size_t l = 0, r = n;\r\n    auto\
    \ j = i;\r\n    for (int k = h-1; k >= 0; k--) {\r\n      auto rl = B.rank(l),\
    \ rj = B.rank(l + j), rr = B.rank(r);\r\n      auto zs = r - l - (rr - rl);\r\n\
    \      if (c & (1ull<<k)) {\r\n        j = rj - rl;\r\n        l += n + zs;\r\n\
    \        r += n;\r\n      } else {\r\n        j = (l+j - rj) - (l - rl);\r\n \
    \       l += n;\r\n        r = l + zs;\r\n      }\r\n    }\r\n    return j;\r\n\
    \  }\r\n\r\n  size_t rank_lower(T c, size_t i) const {\r\n    size_t l = 0, r\
    \ = n;\r\n    auto j = i;\r\n    size_t ret = 0;\r\n    for (int k = h-1; k >=\
    \ 0; k--) {\r\n      auto rl = B.rank(l), rj = B.rank(l+j), rr = B.rank(r);\r\n\
    \      auto zs = r - l - (rr - rl);\r\n      if (c & (1ull << k)) {\r\n      \
    \  ret += (l+j - rj) - (l - rl);\r\n        j = rj - rl;\r\n        l += n + zs;\r\
    \n        r += n;\r\n      } else {\r\n        j = (l+j - rj) - (l - rl);\r\n\
    \        l += n;\r\n        r = l + zs;\r\n      }\r\n    }\r\n    return ret;\r\
    \n  }\r\n\r\n  std::tuple<size_t, size_t, size_t> rank_3way(T c, size_t i) const\
    \ {\r\n    size_t lt = 0, gt = 0;\r\n    size_t l = 0, r = n, j = i;\r\n    for\
    \ (int k = h-1; k >= 0; k--) {\r\n      auto rl = B.rank(l), rj = B.rank(l+j),\
    \ rr = B.rank(r);\r\n      auto zs = r - l - (rr - rl);\r\n      if (c & (1ull\
    \ << k)) {\r\n        lt += (l+j - rj) - (l - rl);\r\n        j = rj - rl;\r\n\
    \        l += n + zs;\r\n        r += n;\r\n      } else {\r\n        gt += rj\
    \ - rl;\r\n        j = (l+j - rj) - (l - rl);\r\n        l += n;\r\n        r\
    \ = l + zs;\r\n      }\r\n    }\r\n    return std::make_tuple(lt, j, gt);\r\n\
    \  }\r\n\r\n  std::array<int, H> ls;\r\n  size_t select(T c, size_t i) { // 0-indexed\r\
    \n    size_t l = 0, r = n;\r\n    for (int k = h-1; k >= 0; k--) {\r\n      ls[k]\
    \ = l;\r\n      auto rl = B.rank(l), rr = B.rank(r);\r\n      auto zs = r - l\
    \ - (rr - rl);\r\n      if (c & (1ull<<k)) {\r\n        l += n + zs;\r\n     \
    \   r += n;\r\n      } else {\r\n        l += n;\r\n        r = l + zs;\r\n  \
    \    }\r\n    }\r\n    if (i >= r - l)\r\n      return n; // Not found\r\n   \
    \ auto j = i;\r\n    for (int k = 0; k < (int)h; k++) {\r\n      auto rl = B.rank(ls[k]);\r\
    \n      if (c & (1ull<<k)) {\r\n        r = rl;\r\n        j = B.select<1>(j +\
    \ r) - ls[k];\r\n      } else {\r\n        r = ls[k] - rl;\r\n        j = B.select<0>(j\
    \ + r) - ls[k];\r\n      }\r\n    }\r\n    return j;\r\n  }\r\n\r\n private:\r\
    \n  void _enumerate(size_t l, size_t r, size_t a, size_t b, T c,\r\n         \
    \         std::vector<std::pair<T, size_t>>& enm) const {\r\n    if (b-a == 0)\r\
    \n      return;\r\n    if (l >= n*h) {\r\n      enm.emplace_back(c, b-a);\r\n\
    \      return;\r\n    }\r\n    auto rl = B.rank(l),\r\n        rr = B.rank(r),\r\
    \n        ra = B.rank(l+a) - rl,\r\n        rb = B.rank(l+b) - rl;\r\n    auto\
    \ zs = (r - rr) - (l - rl);\r\n    _enumerate(l + n, l + n + zs, a - ra, b - rb,\
    \ c, enm);\r\n    auto k = h-1 - l/n;\r\n    _enumerate(l + n + zs, r + n, ra,\
    \ rb, c | (1ull << k), enm);\r\n  }\r\n public:\r\n  std::vector<std::pair<T,\
    \ size_t>> enumerate(size_t l, size_t r) const {\r\n    std::vector<std::pair<T,\
    \ size_t>> enm;\r\n    _enumerate(0, n, l, r, T{0}, enm);\r\n    return enm;\r\
    \n  }\r\n\r\n private:\r\n  std::pair<size_t, size_t> child_tie(bool bit, size_t\
    \ l, size_t r) const {\r\n    auto zs = (r-l) - (B.rank(r) - B.rank(l));\r\n \
    \   if (!bit)\r\n      return std::make_pair(l+n, l+n+zs);\r\n    else\r\n   \
    \   return std::make_pair(l+n+zs, r+n);\r\n  }\r\n\r\n public:\r\n  // Get frequency\
    \ of values which (x <= value < y) in S[l,r).\r\n  size_t range_freq(size_t l,\
    \ size_t r, T x, T y) const {\r\n    size_t freq = 0;\r\n    std::queue<std::tuple<size_t,\
    \ size_t, T>> qs;\r\n    qs.emplace(l,r,0,n,T(0));\r\n    while (!qs.empty(0))\
    \ {\r\n      size_t i,j,_l,_r;\r\n      T c;\r\n      std::tie(i,j,_l,_r,c) =\
    \ qs.front(); qs.pop();\r\n      if (i == j)\r\n        continue;\r\n      size_t\
    \ level = i/n;\r\n      int shift = h-1-level;\r\n      T clo = c, chi = c | ((1ull<<(shift+1))-1);\r\
    \n      if (chi < x or y <= clo)\r\n        continue;\r\n      if (x <= clo and\
    \ chi < y) {\r\n        freq += j - i;\r\n        continue;\r\n      }\r\n   \
    \   assert(level < h);\r\n      size_t rl = B.rank(_l), ri = B.rank(_l+i), rj\
    \ = B.rank(_l+j), rr = B.rank(_r);\r\n      size_t zs = (_r - rr) - (_l - rl);\r\
    \n      qs.emplace((_l+i - ri) - (_l - rl), (_l+j - rj) - (_l - rl),\r\n     \
    \            _l+n, _l+n+zs, c);\r\n      qs.emplace(ri - rl, rj - rl,\r\n    \
    \             _l+n+zs, _r+n, c|(1ull<<shift));\r\n    }\r\n    return freq;\r\n\
    \  }\r\n};\n"
  code: "#pragma once\r\n#include <limits>\r\n#include <array>\r\n#include <iterator>\r\
    \n#include <algorithm>\r\n#include <queue>\r\n#include <tuple>\r\n#include \"\
    bit_vector.hpp\"\r\n#include \"../bit_manip.hpp\"\r\n\r\ntemplate<typename T>\r\
    \nstruct WaveletTree {\r\n  static constexpr unsigned H = 64 - bm::clz(std::numeric_limits<T>::max());\r\
    \n  size_t n,h;\r\n  BitVector B;\r\n  WaveletTree() = default;\r\n  template<typename\
    \ It>\r\n  WaveletTree(It begin, It end)\r\n    : n(std::distance(begin, end)),\
    \ h(64 - bm::clz(*max_element(begin, end)))\r\n  {\r\n    using trait = std::iterator_traits<It>;\r\
    \n    static_assert(std::is_base_of<std::input_iterator_tag, typename trait::iterator_category>::value,\
    \ \"\");\r\n    static_assert(std::is_convertible<typename trait::value_type,\
    \ T>::value, \"\");\r\n    assert(*min_element(begin, end) >= 0);\r\n\r\n    B.assign(h\
    \ * n, false);\r\n    std::vector<T> S(begin, end);\r\n    std::vector<size_t>\
    \ D = {0,n}, nd;\r\n    auto bit = B.begin();\r\n    std::queue<T> sz,so;\r\n\
    \    for (int k = h-1; k > 0; k--) {\r\n      for (size_t i = 0; i < n; i++, ++bit)\r\
    \n        *bit = (S[i] & (1ull<<k)) != 0;\r\n      nd.clear();\r\n      nd.push_back(0);\r\
    \n      for (size_t d = 0; d < D.size()-1; ++d) {\r\n        size_t l = D[d],\
    \ r = D[d+1];\r\n        for (size_t i = l; i < r; ++i)\r\n          if ((S[i]\
    \ & (1ull<<k)) == 0)\r\n            sz.push(S[i]);\r\n          else\r\n     \
    \       so.push(S[i]);\r\n        int zs = sz.size();\r\n        {\r\n       \
    \   auto i = l;\r\n          while (!sz.empty()) {\r\n            S[i++] = sz.front();\r\
    \n            sz.pop();\r\n          }\r\n          while (!so.empty()) {\r\n\
    \            S[i++] = so.front();\r\n            so.pop();\r\n          }\r\n\
    \          assert(i == r);\r\n        }\r\n        if (zs)\r\n          nd.push_back(l\
    \ + zs);\r\n        if (l + zs < r)\r\n          nd.push_back(r);\r\n      }\r\
    \n      std::swap(D, nd);\r\n    }\r\n    for (size_t i = 0; i < n; i++, ++bit)\r\
    \n      *bit = (S[i] & 1ull) != 0;\r\n    B.build();\r\n  }\r\n\r\n  std::pair<size_t,size_t>\
    \ child(bool b, size_t l, size_t r) {\r\n    auto os = B.rank(r) - B.rank(l);\r\
    \n    auto zs = r-l - os;\r\n    return !b ? std::make_pair(l + n, l + zs + n)\r\
    \n              : std::make_pair(l + zs + n, r + n);\r\n  }\r\n\r\n  T get(size_t\
    \ i) const {\r\n    size_t l = 0, r = n;\r\n    T ret = 0;\r\n    auto j = i;\r\
    \n    for (int k = h-1; k > 0; k--) {\r\n      auto rl = B.rank(l),\r\n      \
    \    rj = B.rank(l + j),\r\n          rr = B.rank(r);\r\n      auto zs = r - l\
    \ - (rr - rl);\r\n      if (B[l + j]) {\r\n        ret |= 1ull<<k;\r\n       \
    \ j = rj - rl;\r\n        l += n + zs;\r\n        r += n;\r\n      } else {\r\n\
    \        j = (l+j - rj) - (l - rl);\r\n        l += n;\r\n        r = l + zs;\r\
    \n      }\r\n    }\r\n    if (B[l + j])\r\n      ret |= 1ull;\r\n    return ret;\r\
    \n  }\r\n\r\n  size_t rank(T c, size_t i) const {\r\n    size_t l = 0, r = n;\r\
    \n    auto j = i;\r\n    for (int k = h-1; k >= 0; k--) {\r\n      auto rl = B.rank(l),\
    \ rj = B.rank(l + j), rr = B.rank(r);\r\n      auto zs = r - l - (rr - rl);\r\n\
    \      if (c & (1ull<<k)) {\r\n        j = rj - rl;\r\n        l += n + zs;\r\n\
    \        r += n;\r\n      } else {\r\n        j = (l+j - rj) - (l - rl);\r\n \
    \       l += n;\r\n        r = l + zs;\r\n      }\r\n    }\r\n    return j;\r\n\
    \  }\r\n\r\n  size_t rank_lower(T c, size_t i) const {\r\n    size_t l = 0, r\
    \ = n;\r\n    auto j = i;\r\n    size_t ret = 0;\r\n    for (int k = h-1; k >=\
    \ 0; k--) {\r\n      auto rl = B.rank(l), rj = B.rank(l+j), rr = B.rank(r);\r\n\
    \      auto zs = r - l - (rr - rl);\r\n      if (c & (1ull << k)) {\r\n      \
    \  ret += (l+j - rj) - (l - rl);\r\n        j = rj - rl;\r\n        l += n + zs;\r\
    \n        r += n;\r\n      } else {\r\n        j = (l+j - rj) - (l - rl);\r\n\
    \        l += n;\r\n        r = l + zs;\r\n      }\r\n    }\r\n    return ret;\r\
    \n  }\r\n\r\n  std::tuple<size_t, size_t, size_t> rank_3way(T c, size_t i) const\
    \ {\r\n    size_t lt = 0, gt = 0;\r\n    size_t l = 0, r = n, j = i;\r\n    for\
    \ (int k = h-1; k >= 0; k--) {\r\n      auto rl = B.rank(l), rj = B.rank(l+j),\
    \ rr = B.rank(r);\r\n      auto zs = r - l - (rr - rl);\r\n      if (c & (1ull\
    \ << k)) {\r\n        lt += (l+j - rj) - (l - rl);\r\n        j = rj - rl;\r\n\
    \        l += n + zs;\r\n        r += n;\r\n      } else {\r\n        gt += rj\
    \ - rl;\r\n        j = (l+j - rj) - (l - rl);\r\n        l += n;\r\n        r\
    \ = l + zs;\r\n      }\r\n    }\r\n    return std::make_tuple(lt, j, gt);\r\n\
    \  }\r\n\r\n  std::array<int, H> ls;\r\n  size_t select(T c, size_t i) { // 0-indexed\r\
    \n    size_t l = 0, r = n;\r\n    for (int k = h-1; k >= 0; k--) {\r\n      ls[k]\
    \ = l;\r\n      auto rl = B.rank(l), rr = B.rank(r);\r\n      auto zs = r - l\
    \ - (rr - rl);\r\n      if (c & (1ull<<k)) {\r\n        l += n + zs;\r\n     \
    \   r += n;\r\n      } else {\r\n        l += n;\r\n        r = l + zs;\r\n  \
    \    }\r\n    }\r\n    if (i >= r - l)\r\n      return n; // Not found\r\n   \
    \ auto j = i;\r\n    for (int k = 0; k < (int)h; k++) {\r\n      auto rl = B.rank(ls[k]);\r\
    \n      if (c & (1ull<<k)) {\r\n        r = rl;\r\n        j = B.select<1>(j +\
    \ r) - ls[k];\r\n      } else {\r\n        r = ls[k] - rl;\r\n        j = B.select<0>(j\
    \ + r) - ls[k];\r\n      }\r\n    }\r\n    return j;\r\n  }\r\n\r\n private:\r\
    \n  void _enumerate(size_t l, size_t r, size_t a, size_t b, T c,\r\n         \
    \         std::vector<std::pair<T, size_t>>& enm) const {\r\n    if (b-a == 0)\r\
    \n      return;\r\n    if (l >= n*h) {\r\n      enm.emplace_back(c, b-a);\r\n\
    \      return;\r\n    }\r\n    auto rl = B.rank(l),\r\n        rr = B.rank(r),\r\
    \n        ra = B.rank(l+a) - rl,\r\n        rb = B.rank(l+b) - rl;\r\n    auto\
    \ zs = (r - rr) - (l - rl);\r\n    _enumerate(l + n, l + n + zs, a - ra, b - rb,\
    \ c, enm);\r\n    auto k = h-1 - l/n;\r\n    _enumerate(l + n + zs, r + n, ra,\
    \ rb, c | (1ull << k), enm);\r\n  }\r\n public:\r\n  std::vector<std::pair<T,\
    \ size_t>> enumerate(size_t l, size_t r) const {\r\n    std::vector<std::pair<T,\
    \ size_t>> enm;\r\n    _enumerate(0, n, l, r, T{0}, enm);\r\n    return enm;\r\
    \n  }\r\n\r\n private:\r\n  std::pair<size_t, size_t> child_tie(bool bit, size_t\
    \ l, size_t r) const {\r\n    auto zs = (r-l) - (B.rank(r) - B.rank(l));\r\n \
    \   if (!bit)\r\n      return std::make_pair(l+n, l+n+zs);\r\n    else\r\n   \
    \   return std::make_pair(l+n+zs, r+n);\r\n  }\r\n\r\n public:\r\n  // Get frequency\
    \ of values which (x <= value < y) in S[l,r).\r\n  size_t range_freq(size_t l,\
    \ size_t r, T x, T y) const {\r\n    size_t freq = 0;\r\n    std::queue<std::tuple<size_t,\
    \ size_t, T>> qs;\r\n    qs.emplace(l,r,0,n,T(0));\r\n    while (!qs.empty(0))\
    \ {\r\n      size_t i,j,_l,_r;\r\n      T c;\r\n      std::tie(i,j,_l,_r,c) =\
    \ qs.front(); qs.pop();\r\n      if (i == j)\r\n        continue;\r\n      size_t\
    \ level = i/n;\r\n      int shift = h-1-level;\r\n      T clo = c, chi = c | ((1ull<<(shift+1))-1);\r\
    \n      if (chi < x or y <= clo)\r\n        continue;\r\n      if (x <= clo and\
    \ chi < y) {\r\n        freq += j - i;\r\n        continue;\r\n      }\r\n   \
    \   assert(level < h);\r\n      size_t rl = B.rank(_l), ri = B.rank(_l+i), rj\
    \ = B.rank(_l+j), rr = B.rank(_r);\r\n      size_t zs = (_r - rr) - (_l - rl);\r\
    \n      qs.emplace((_l+i - ri) - (_l - rl), (_l+j - rj) - (_l - rl),\r\n     \
    \            _l+n, _l+n+zs, c);\r\n      qs.emplace(ri - rl, rj - rl,\r\n    \
    \             _l+n+zs, _r+n, c|(1ull<<shift));\r\n    }\r\n    return freq;\r\n\
    \  }\r\n};"
  dependsOn:
  - include/mtl/succinct/bit_vector.hpp
  - include/mtl/bit_manip.hpp
  - include/mtl/succinct/select.hpp
  - include/mtl/succinct/bv.hpp
  isVerificationFile: false
  path: include/mtl/succinct/wavelet_tree.hpp
  requiredBy: []
  timestamp: '2023-04-12 22:25:51+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/mtl/succinct/wavelet_tree.hpp
layout: document
redirect_from:
- /library/include/mtl/succinct/wavelet_tree.hpp
- /library/include/mtl/succinct/wavelet_tree.hpp.html
title: include/mtl/succinct/wavelet_tree.hpp
---
