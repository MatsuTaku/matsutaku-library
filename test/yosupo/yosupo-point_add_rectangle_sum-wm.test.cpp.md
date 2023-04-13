---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':question:'
    path: include/mtl/fenwick_tree.hpp
    title: include/mtl/fenwick_tree.hpp
  - icon: ':question:'
    path: include/mtl/ordinal_range_search.hpp
    title: Ordinal Range Search
  - icon: ':question:'
    path: include/mtl/succinct/binary_set.hpp
    title: include/mtl/succinct/binary_set.hpp
  - icon: ':question:'
    path: include/mtl/succinct/bit_vector.hpp
    title: include/mtl/succinct/bit_vector.hpp
  - icon: ':question:'
    path: include/mtl/succinct/bv.hpp
    title: include/mtl/succinct/bv.hpp
  - icon: ':question:'
    path: include/mtl/succinct/rrr.hpp
    title: Succinct bit vector in memory of B(n, u) + O(u log log n / log n) bits
  - icon: ':question:'
    path: include/mtl/succinct/select.hpp
    title: include/mtl/succinct/select.hpp
  - icon: ':question:'
    path: include/mtl/succinct/ty.hpp
    title: 'TY: Store increasing sequence of integers.'
  - icon: ':question:'
    path: include/mtl/succinct/wavelet_matrix.hpp
    title: include/mtl/succinct/wavelet_matrix.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/point_add_rectangle_sum
    links:
    - https://judge.yosupo.jp/problem/point_add_rectangle_sum
  bundledCode: "#line 1 \"test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp\"\
    \n#define PROBLEM \"https://judge.yosupo.jp/problem/point_add_rectangle_sum\"\r\
    \n#line 2 \"include/mtl/bit_manip.hpp\"\n#include <cstdint>\n#include <cassert>\n\
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
    \ 3 \"include/mtl/fenwick_tree.hpp\"\n#include <cstddef>\n#include <vector>\n\n\
    template <class T>\nclass FenwickTree {\n private:\n  std::vector<T> tree_;\n\n\
    \ public:\n  FenwickTree() = default;\n  explicit FenwickTree(size_t size) : tree_(size+1)\
    \ {}\n\n  size_t size() const { return tree_.size()-1; }\n\n  template <class\
    \ Iter>\n  explicit FenwickTree(Iter begin, Iter end) : FenwickTree(std::distance(begin,\
    \ end)) {\n    size_t i = 1;\n    for (auto it = begin; it != end; ++it) {\n \
    \     tree_[i] = tree_[i] + *it;\n      auto j = i + (i&(-i));\n      if (j <\
    \ tree_.size())\n        tree_[j] = tree_[j] + tree_[i];\n      ++i;\n    }\n\
    \  }\n\n  template<class V>\n  void add(size_t index, const V& x) {\n    for (size_t\
    \ i = index+1; i < tree_.size(); i += i&(-i))\n      tree_[i] = tree_[i] + x;\n\
    \  }\n\n  T sum(size_t index) const {\n    T sum = 0;\n    for (size_t i = index+1;\
    \ i > 0; i -= i&(-i))\n      sum = sum + tree_[i];\n    return sum;\n  }\n\n \
    \ T range_sum(size_t l, size_t r) const {\n    auto sl = l > 0 ? sum(l-1) : 0;\n\
    \    auto sr = r > 0 ? sum(r-1) : 0;\n    return sr - sl;\n  }\n\n  template<class\
    \ V>\n  size_t lower_bound(const V& _sum) const {\n    size_t ret = 0;\n    T\
    \ s = 0;\n    for (int k = 63-bm::clz(size()); k >= 0; k--) {\n      size_t j\
    \ = ret | (1ull<<k);\n      if (j < tree_.size() and s + tree_[j] < _sum) {\n\
    \        s = s + tree_[j];\n        ret = j;\n      }\n    }\n    return ret;\n\
    \  }\n\n};\n\n#line 3 \"include/mtl/succinct/select.hpp\"\n#include <array>\n\n\
    struct select64 {\n    using size_type = uint8_t;\n    static struct make_select_table\
    \ {\n        using bitmap_type = uint8_t;\n        std::array<std::array<size_type,\
    \ 9>, 1<<8> tb;\n        make_select_table() {\n            for (int i = 0; i\
    \ < 1<<8; i++) {\n                int c = 0;\n                int x = i;\n   \
    \             tb[i].fill(8);\n                for (int j = 0; j < 8; j++) {\n\
    \                    if (x & 1)\n                    tb[i][++c] = j;\n       \
    \             x >>= 1;\n                }\n            }\n        }\n        size_type\
    \ operator()(bitmap_type bitmap, size_type ith) const {\n            return tb[bitmap][ith];\n\
    \        }\n    } sel_tb;\n    template<bool B>\n    static constexpr size_type\
    \ select(size_type ith, uint64_t bitmap) { // 0-indexed\n        assert(ith <\
    \ 64);\n        ith++; // to 1-index\n        // Find byte\n        uint64_t w\
    \ = bitmap;\n        if constexpr (!B) w = ~w;\n        auto _bs = (uint64_t)\
    \ bm::popcnt_e8(w) * 0x0101010101010100ull;\n        auto bs = (const uint8_t*)\
    \ &_bs;\n        size_type b = 0;\n        auto o = ith;\n        /* Following\
    \ bit-manipulates code is same as ... */\n        // {\n        //     auto d\
    \ = 8;\n        //     while (d > 1) {\n        //     auto c = b + d/2;\n   \
    \     //     if (bs[c] < o)\n        //         b = c;\n        //     d /= 2;\n\
    \        //     }\n        // }\n        {\n            uint64_t x = (uint64_t)\
    \ o * 0x0101010101010101ull;\n            uint64_t bmx = (_bs | 0x8080808080808080ull)\
    \ - x;\n            uint64_t y = ((bmx & 0x8080808080808080ull) * 0x02040810204081ull)\
    \ >> (64-8);\n            size_type nb = bm::ctz8(y) - 1;\n            // assert(b\
    \ == nb);\n            b = nb;\n        }\n        // Calc select\n        auto\
    \ r = o - bs[b];\n        uint8_t byte = ((const uint8_t*)(&w))[b];\n        assert(r\
    \ and r <= (size_type)bm::popcnt(byte));\n        return b * 8 + sel_tb(byte,\
    \ r);\n    }\n    static constexpr size_type select1(size_type ith, uint64_t bitmap)\
    \ {\n        return select<1>(ith, bitmap);\n    }\n    static constexpr size_type\
    \ select0(size_type ith, uint64_t bitmap) {\n        return select<0>(ith, bitmap);\n\
    \    }\n};\n\ntypename select64::make_select_table select64::sel_tb;\n#line 5\
    \ \"include/mtl/succinct/bv.hpp\"\n#include <bitset>\n#include <iostream>\n\n\
    #if __cpp_concepts >= 202002L\n#include <concepts>\ntemplate<class T>\nconcept\
    \ ConstructableBV = requires(T t, size_t s) {\n  { t.size() } -> std::same_as<size_t>;\n\
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
    \ rs_support.select<B>(n);\r\n  }\r\n};\n#line 4 \"include/mtl/succinct/wavelet_matrix.hpp\"\
    \n#include <limits>\n#line 7 \"include/mtl/succinct/wavelet_matrix.hpp\"\n#include\
    \ <iterator>\n#include <algorithm>\n#include <queue>\n#include <tuple>\n\ntemplate<class\
    \ T, class BitmapType = Bitmap>\nstruct WaveletMatrix {\n  static constexpr unsigned\
    \ H = 64 - bm::clz(std::numeric_limits<T>::max());\n\n  size_t n,h;\n  using bitmap_type\
    \ = BitmapType;\n  bitmap_type B;\n  using rs_type = typename RankSelectTraits<bitmap_type>::rank_select_type;\n\
    \  rs_type rs_b;\n  std::vector<size_t> RO,Z;\n\n  WaveletMatrix() = default;\n\
    \  template<typename It>\n  WaveletMatrix(It begin, It end)\n      : n(std::distance(begin,\
    \ end)),\n      h(std::max(1u, 64 - bm::clz(*max_element(begin, end)))),\n   \
    \   B(n*h, false),\n      rs_b(),\n      RO(h+1),\n      Z(h)\n  {\n    using\
    \ trait = std::iterator_traits<It>;\n    static_assert(std::is_base_of<std::input_iterator_tag,\
    \ typename trait::iterator_category>::value, \"\");\n    static_assert(std::is_convertible<typename\
    \ trait::value_type, T>::value, \"\");\n    assert(*min_element(begin, end) >=\
    \ 0);\n\n    std::vector<T> S(begin, end), z, o;\n    z.reserve(n);\n    o.reserve(n);\n\
    \    auto bit = B.begin();\n    for (int k = h-1; k >= 0; k--) {\n      for (size_t\
    \ i = 0; i < n; i++) {\n        bool b = S[i] >> k & 1;\n        *bit++ = b;\n\
    \        if (!b)\n          z.push_back(S[i]);\n        else\n          o.push_back(S[i]);\n\
    \      }\n      Z[k] = n*(h-1-k+1) + z.size();\n      auto j = n;\n      while\
    \ (!o.empty()) {\n        S[--j] = o.back();\n        o.pop_back();\n      }\n\
    \      while (!z.empty()) {\n        S[--j] = z.back();\n        z.pop_back();\n\
    \      }\n      assert(j == 0);\n    }\n    B.build();\n    rs_b.build(&B);\n\
    \    for (size_t i = 0; i <= h; i++)\n      RO[i] = rs_b.rank1(n * i);\n  }\n\
    \  WaveletMatrix(const WaveletMatrix& rhs) noexcept :\n    n(rhs.n),\n    h(rhs.h),\n\
    \    B(rhs.B),\n    rs_b(rhs.rs_b),\n    RO(rhs.RO),\n    Z(rhs.Z) \n  {\n   \
    \ rs_b.set_ptr(&B);\n  }\n  WaveletMatrix& operator=(const WaveletMatrix& rhs)\
    \ noexcept {\n    n = rhs.n;\n    h = rhs.h;\n    B = rhs.B;\n    rs_b = rhs.rs_b;\n\
    \    rs_b.set_ptr(&B);\n    RO = rhs.RO;\n    Z = rhs.Z;\n    return *this;\n\
    \  }\n  WaveletMatrix(WaveletMatrix&& rhs) noexcept :\n    n(std::move(rhs.n)),\n\
    \    h(std::move(rhs.h)),\n    B(std::move(rhs.B)),\n    rs_b(std::move(rhs.rs_b)),\n\
    \    RO(std::move(rhs.RO)),\n    Z(std::move(rhs.Z)) \n  {\n    rs_b.set_ptr(&B);\n\
    \  }\n  WaveletMatrix& operator=(WaveletMatrix&& rhs) noexcept {\n    n = std::move(rhs.n);\n\
    \    h = std::move(rhs.h);\n    B = std::move(rhs.B);\n    rs_b = std::move(rhs.rs_b);\n\
    \    rs_b.set_ptr(&B);\n    RO = std::move(rhs.RO);\n    Z = std::move(rhs.Z);\n\
    \    return *this;\n  }\n\n  inline size_t _child0(size_t level, size_t i) const\
    \ {\n      return i + n + RO[level] - rs_b.rank1(i);\n  }\n  inline size_t _child1(size_t\
    \ level, size_t i) const {\n    return n*(level+2) + rs_b.rank1(i) - RO[level+1];\n\
    \  }\n  inline size_t child(size_t level, size_t i, bool bit) const {\n    return\
    \ !bit ? _child0(level, i) : _child1(level, i);\n  }\n  std::pair<size_t, size_t>\
    \ _child_tie0(size_t level, size_t l, size_t r) const {\n    return std::make_pair(_child0(level,\
    \ l), _child0(level, r));\n  }\n  std::pair<size_t, size_t> _child_tie1(size_t\
    \ level, size_t l, size_t r) const {\n    return std::make_pair(_child1(level,\
    \ l), _child1(level, r));\n  }\n  std::pair<size_t, size_t> child_tie(size_t level,\
    \ size_t l, size_t r, bool bit) const {\n    return !bit ? _child_tie0(level,\
    \ l, r) : _child_tie1(level, l, r);\n  }\n\n  inline size_t _parent0(size_t level,\
    \ size_t i) const {\n    return rs_b.select0(i - n - RO[level]);\n  }\n  inline\
    \ size_t _parent1(size_t level, size_t i) const {\n    return rs_b.select1(i -\
    \ Z[h-1-level] + RO[level]);\n  }\n  inline size_t parent(size_t level, size_t\
    \ i, bool bit) const {\n    return !bit ? _parent0(level, i) : _parent1(level,\
    \ i);\n  }\n\n  T get(size_t i) const {\n    T c = 0;\n    size_t j = i;\n   \
    \ for (int k = h-1; k > 0; k--) {\n      bool b = B[j];\n      j = child(h-1-k,\
    \ j, b);\n      if (b)\n        c |= 1ull<<k;\n    }\n    if (B[j])\n      c |=\
    \ 1u;\n    return c;\n  }\n\n  size_t range_rank(T c, size_t l, size_t r) const\
    \ {\n    for (int k = h-1; k >= 0; k--) {\n      if (l == r)\n        break;\n\
    \      std::tie(l,r) = child_tie(h-1-k, l, r, (c >> k) & 1u);\n    }\n    return\
    \ r - l;\n  }\n  size_t rank(T c, size_t i) const {\n    return range_rank(c,\
    \ 0, i);\n  }\n  std::tuple<size_t, size_t, size_t> rank_3way(T c, size_t l, size_t\
    \ r) const {\n    size_t lt = 0, gt = 0;\n    for (int k = h-1; k >= 0; k--) {\n\
    \      size_t pr = r - l;\n      if (pr == 0)\n        break;\n      if (((c >>\
    \ k) & 1u) == 0) {\n        std::tie(l,r) = _child_tie0(h-1-k, l, r);\n      \
    \  gt += pr - (r - l);\n      } else {\n        std::tie(l,r) = _child_tie1(h-1-k,\
    \ l, r);\n        lt += pr - (r - l);\n      }\n    }\n    return std::make_tuple(lt,\
    \ r - l, gt);\n  }\n\n  /// Get frequency of values which (x <= value < y) in\
    \ S[l,r).\n  size_t range_freq(size_t l, size_t r, T x, T y) const {\n    size_t\
    \ freq = 0;\n    std::queue<std::tuple<size_t,size_t, T>> qs;\n    qs.emplace(l,\
    \ r, T(0));\n    while (!qs.empty()) {\n      size_t _l,_r;\n      T c;\n    \
    \  std::tie(_l,_r,c) = qs.front();\n      qs.pop();\n      size_t level = _l/n;\n\
    \      if (_l == _r)\n        continue;\n      int shift = h-1-level;\n      T\
    \ clo = c, chi = c | ((1ull<<(shift+1))-1);\n      if (chi < x or y <= clo)\n\
    \        continue;\n      if (x <= clo and chi < y) {\n        freq += _r - _l;\n\
    \        continue;\n      }\n      assert(level < h);\n      size_t nl,nr;\n \
    \     std::tie(nl,nr) = child_tie(level, _l, _r, 0);\n      qs.emplace(nl, nr,\
    \ c);\n      std::tie(nl,nr) = child_tie(level, _l, _r, 1);\n      qs.emplace(nl,\
    \ nr, c | (1ull << shift));\n    }\n    return freq;\n  }\n\n  size_t range_select(T\
    \ c, size_t l, size_t r, size_t i) const {\n    if (r - l <= i)\n      return\
    \ n;\n    for (int k = h-1; k >= 0; k--) {\n      std::tie(l,r) = child_tie(h-1-k,\
    \ l, r, (c >> k) & 1u);\n      if (r - l <= i)\n        return n;\n    }\n   \
    \ size_t j = l+i;\n    for (size_t k = 0; k < h; k++) {\n      j = parent(h-1-k,\
    \ j, (c >> k) & 1u);\n      assert((bool)((c>>k)&1u) == B[j]);\n    }\n    return\
    \ j;\n  }\n  size_t select(T c, size_t i) const {\n    return range_select(c,\
    \ 0, n, i);\n  }\n\n  /// Get kth (0-indexed) smallest value in S[l,r).\n  T quantile(size_t\
    \ l, size_t r, size_t k) const {\n    assert(r - l > k);\n    T c = 0;\n    for\
    \ (int d = h-1; d > 0; d--) {\n      auto os = rs_b.rank1(r) - rs_b.rank1(l);\n\
    \      auto zs = r - l - os;\n      if (k < zs) {\n        std::tie(l,r) = _child_tie0(h-1-d,\
    \ l, r);\n      } else {\n        c |= 1ull << d;\n        k -= zs;\n        std::tie(l,r)\
    \ = _child_tie1(h-1-d, l, r);\n      }\n      assert(l < r);\n    }\n    auto\
    \ os = rs_b.rank1(r) - rs_b.rank1(l);\n    auto zs = r - l - os;\n    if (k >=\
    \ zs) {\n      c |= 1ull;\n    }\n    return c;\n  }\n\n  /// Get tuples (value,\
    \ frequency) of the most k frequently occurring values in S[l,r).\n  std::vector<std::pair<T,\
    \ size_t>> top_k(size_t l, size_t r, size_t k) const {\n    std::vector<std::pair<T,\
    \ size_t>> ret;\n    std::priority_queue<std::tuple<size_t, size_t, T>> qs;\n\
    \    qs.emplace(r-l, l, 0);\n    while (!qs.empty()) {\n      size_t range, s;\n\
    \      T c;\n      std::tie(range, s, c) = qs.top();\n      qs.pop();\n      auto\
    \ level = s/n;\n      if (level == h) {\n        ret.emplace_back(c, range);\n\
    \        if (ret.size() == k)\n          break;\n      } else {\n        size_t\
    \ _l, _r;\n        for (int b = 0; b < 2; b++) {\n          std::tie(_l,_r) =\
    \ child_tie(level, s, s+range, b);\n          if (_l != _r)\n            qs.emplace(_r-_l,\
    \ _l, c | (uint64_t(b) << (h-1-level)));\n        }\n      }\n    }\n    return\
    \ ret;\n  }\n  /// Get sum of S[l,r) in O(min(r-l, \\sigma) log \\sigma) times.\n\
    \  template<typename U=T>\n  U sum(size_t l, size_t r) const {\n    U ret = 0;\n\
    \    for (auto p : top_k(l, r, r-l))\n      ret += (U) p.first * p.second;\n \
    \   return ret;\n  }\n\n  /// Get k tuples of (value, frequency) that value satisfies\
    \ condition (x <= value < y) in S[l,r) from the smallest (or largest).\n  ///\
    \ The complexity is O(k log \\sigma).\n  template<bool ASCENDING, bool VALUE_RANGE\
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
    \    return ret;\n  }\n\n  /// Get tuples of (value, frequency) that value satisfies\
    \ condition (x <= value < y) in S[l,r).\n  /// The complexity is O(k log \\sigma).\n\
    \  std::vector<std::pair<T, size_t>> range_list(size_t l, size_t r, T x, T y)\
    \ const {\n    return range_list_k<true>(l, r, r - l, x, y);\n  }\n\n  /// Get\
    \ k tuples of (value, frequency) that value satisfies condition (x <= value <\
    \ y) in S[l,r) from the largest.\n  /// The complexity is O(k log \\sigma).\n\
    \  std::vector<std::pair<T, size_t>> range_max_k(size_t l, size_t r, size_t k)\
    \ const {\n    return range_list_k<false, false>(l, r, k, 0, 0);\n  }\n  /// Get\
    \ k tuples of (value, frequency) that value satisfies condition (x <= value <\
    \ y) in S[l,r) from the smallest.\n  // The complexity is O(k log \\sigma).\n\
    \  std::vector<std::pair<T, size_t>> range_min_k(size_t l, size_t r, size_t k)\
    \ const {\n    return range_list_k<true, false>(l, r, k, 0, 0);\n  }\n\n  ///\
    \ Get tuples (value, frequency of T1, frequency of T2) that commonly occur between\
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
    \     qs.pop();\n    }\n    return ret;\n  }\n};\n#line 7 \"include/mtl/succinct/ty.hpp\"\
    \n\n/**\n * @brief TY: Store increasing sequence of integers.\n *            Memory\
    \ needs for store nth integers O(n log d) bits \n *            which d is max\
    \ diff of consecutive elements.\n*/\ntemplate<class T, class DiffType = uint16_t>\n\
    struct TY {\n    using value_type = T;\n    static constexpr auto block_size =\
    \ sizeof(value_type) * 8;\n    using diff_value_type = DiffType;\n    static constexpr\
    \ unsigned max_diff = std::numeric_limits<diff_value_type>::max();\n    std::vector<value_type>\
    \ head;\n    std::vector<diff_value_type> diff;\n\n    TY() = default;\n    size_t\
    \ size() const {\n        return head.size() + diff.size();\n    }\n    void reserve(size_t\
    \ n) {\n        head.reserve((n + block_size - 1) / block_size);\n        diff.reserve(n\
    \ / block_size * (block_size - 1) + n % block_size);\n    }\n    bool empty()\
    \ const { return size() == 0; }\n    value_type raw_element(const value_type&\
    \ v) {\n        return v;\n    }\n    value_type diff_element(const value_type&\
    \ v) {\n        return v;\n    }\n    void push_back(const value_type& v) {\n\
    \        assert(head.empty() or size() % block_size == 0 or v - head.back() <=\
    \ (value_type)max_diff);\n        if (size() % block_size == 0) {\n          \
    \  head.push_back(raw_element(v));\n        } else {\n            diff.push_back(diff_element(v\
    \ - head.back()));\n        }\n    }\n    value_type get(size_t i) const {\n \
    \       return (i % block_size == 0) ? \n            head[i / block_size] : \n\
    \            head[i / block_size] + diff[i / block_size * (block_size-1) + i %\
    \ block_size - 1];\n    }\n    value_type operator[](size_t i) const { return\
    \ get(i); }\n    value_type front() const { return get(0); }\n    value_type back()\
    \ const { return get(size()-1); }\n};\n#line 6 \"include/mtl/succinct/rrr.hpp\"\
    \n#include <map>\n#line 13 \"include/mtl/succinct/rrr.hpp\"\n\nconstexpr unsigned\
    \ need_bits(uint64_t n) {\n    return 64-bm::clz(n);\n}\n\ntemplate<unsigned N>\n\
    struct BinomialTable {\n    static_assert(N < 64, \n        \"Too large N for\
    \ BinomialTable. N must be less than 64\");\n    using number_type = uint64_t;\n\
    \    static struct impl {\n        std::array<std::array<number_type, N+1>, N+1>\
    \ tb;\n        constexpr impl() {\n            tb[0][0] = 1;\n            for\
    \ (size_t i = 1; i <= N; i++) {\n                tb[i][0] = tb[i-1][0];\n    \
    \            for (size_t j = 1; j <= i; j++) \n                    tb[i][j] =\
    \ tb[i-1][j-1] + tb[i-1][j];\n            }\n        }\n    } data;\n    static\
    \ constexpr number_type binom(size_t n, size_t k) {\n        assert(n <= N and\
    \ k <= N);\n        return data.tb[n][k];\n    }\n};\n\ntemplate<unsigned N>\n\
    typename BinomialTable<N>::impl BinomialTable<N>::data;\n\ntemplate<class Def>\n\
    struct RRRTable {\n    using def = Def;\n    static constexpr unsigned s_size\
    \ = def::s_size;\n    using s_type = typename def::s_type;\n    static constexpr\
    \ unsigned n_bits = def::n_bits;\n    using binomial_table_type = BinomialTable<s_size>;\n\
    \    using number_type = typename binomial_table_type::number_type;\n    static\
    \ constexpr s_type get_int(unsigned n, number_type k, unsigned bits = s_size)\
    \ {\n        s_type res = 0;\n        const auto offset = bits;\n        s_type\
    \ mask = ((s_type(1)<<bits)-1);\n        auto nn = n;\n        unsigned i = 0;\n\
    \        /*\n        Binary search time B = ceil(\\log_2 w)\n        Expected\
    \ length of consecutive zeros Ez = \\sum j binom(w-j, nn) / binom(w, nn), j=1..w-nn\n\
    \        Expected length of consecutive ones  Eo = \\sum j binom(w-j, nn-j) /\
    \ binom(w, nn), j=1..nn\n        Approximate simple function from Ez > B to be\
    \ nn <= min(20, w-1)\n        Approximate simple function from Eo > B to be nn\
    \ > min(40, w)\n        */\n        // TODO: When nn > 40, use binary search to\
    \ find length of consecutive ones\n        for (; i < offset and nn > 20; i++)\
    \ {\n            auto w = s_size - i;\n            if (nn == w) {\n          \
    \      res |= ((s_type(1)<<nn)-1) << i;\n                return res & mask;\n\
    \            }\n            if (nn == w-1) {\n                res |= (((s_type(1)<<w)-1)\
    \ ^ (s_type(1) << k)) << i;\n                return res & mask;\n            }\n\
    \            // Linear search\n            auto binom = binomial_table_type::binom(w-1,\
    \ nn);\n            if (k >= binom) {\n                res |= s_type(1) << i;\n\
    \                k -= binom;\n                nn--;\n            }\n        }\n\
    \        for (; i < offset and nn > 1; i++) {\n            auto w = s_size - i;\n\
    \            if (nn == w) {\n                res |= ((s_type(1)<<nn)-1) << i;\n\
    \                return res & mask;\n            }\n            if (nn == w-1)\
    \ {\n                res |= (((s_type(1)<<w)-1) ^ (s_type(1) << k)) << i;\n  \
    \              return res & mask;\n            }\n            // Binary search\
    \ to find length of consecutive zeros\n            auto l = i, r = offset+1;\n\
    \            while (l+1<r) {\n                auto c = l+(r-l)/2;\n          \
    \      if (k < binomial_table_type::binom(s_size-c, nn))\n                   \
    \ l = c;\n                else \n                    r = c;\n            }\n \
    \           if (l < offset) {\n                res |= s_type(1) << l;\n      \
    \          k -= binomial_table_type::binom(s_size-l-1, nn);\n                nn--;\n\
    \            }\n            i = l;\n        }\n        if (nn == 1) {\n      \
    \      res |= s_type(1) << (s_size-1-k);\n            return res & mask;\n   \
    \     }\n        if (nn == 0) \n            return res;\n        if (k >= binomial_table_type::binom(s_size-offset-1,\
    \ nn))\n            res |= s_type(1) << offset;\n        return res & ((s_type(1)<<bits)-1);\n\
    \    }\n    static constexpr bool get_bit(unsigned n, number_type k, unsigned\
    \ offset) {\n        auto nn = n;\n        unsigned i = 0;\n        /*\n     \
    \   Binary search time B = ceil(\\log_2 w)\n        Expected length of consecutive\
    \ zeros Ez = \\sum j binom(w-j, nn) / binom(w, nn), j=1..w-nn\n        Expected\
    \ length of consecutive ones  Eo = \\sum j binom(w-j, nn-j) / binom(w, nn), j=1..nn\n\
    \        Approximate simple function from Ez > B to be nn <= min(20, w-1)\n  \
    \      Approximate simple function from Eo > B to be nn > min(40, w)\n       \
    \ */\n        // TODO: When nn > 40, use binary search to find length of consecutive\
    \ ones\n        for (; i < offset and nn > 20; i++) {\n            auto w = s_size\
    \ - i;\n            if (nn == w) {\n                return 1;\n            }\n\
    \            if (nn == w-1) {\n                return offset != i+k;\n       \
    \     }\n            // linear search\n            auto binom = binomial_table_type::binom(w-1,\
    \ nn);\n            if (k >= binom) {\n                k -= binom;\n         \
    \       nn--;\n            }\n        }\n        for (; i < offset and nn > 1;\
    \ i++) {\n            auto w = s_size - i;\n            if (nn == w) {\n     \
    \           return 1;\n            }\n            if (nn == w-1) {\n         \
    \       return offset != i+k;\n            }\n            // binary search\n \
    \           auto l = i, r = offset+1;\n            while (l+1<r) {\n         \
    \       auto c = l+(r-l)/2;\n                if (k < binomial_table_type::binom(s_size-c,\
    \ nn))\n                    l = c;\n                else \n                  \
    \  r = c;\n            }\n            if (l < offset) {\n                k -=\
    \ binomial_table_type::binom(s_size-l-1, nn);\n                nn--;\n       \
    \     }\n            i = l;\n        }\n        if (nn == 1)\n            return\
    \ offset == s_size-1-k;\n        if (nn == 0) \n            return 0;\n      \
    \  return k >= binomial_table_type::binom(s_size-offset-1, nn);\n    }\n    static\
    \ number_type get_number_for_popcnt(s_type bitmap, unsigned pc) {\n        number_type\
    \ number = 0;\n        auto m = bitmap;\n        auto n = pc;\n        while (m)\
    \ {\n            auto i = bm::ctz(m);\n            number += binomial_table_type::binom(s_size-i-1,\
    \ n);\n            n--;\n            m ^= (s_type(1)<<i);\n        }\n       \
    \ return number;\n    }\n    static std::pair<unsigned, number_type> get_pc_and_number(s_type\
    \ bitmap) {\n        unsigned pc = bm::popcnt(bitmap);\n        auto number =\
    \ pc <= s_size-pc ? get_number_for_popcnt(bitmap, pc) \n                     \
    \                 : (number_size(pc)-1-get_number_for_popcnt(\n              \
    \                              ~bitmap & ((s_type(1)<<s_size)-1), s_size-pc));\n\
    \        return std::make_pair(pc, number);\n    }\n    static number_type number_size(unsigned\
    \ n) {\n        return binomial_table_type::binom(s_size, n);\n    }\n    static\
    \ struct number_bits_table {\n        std::array<unsigned, s_size+1> tb;\n   \
    \     number_bits_table() {\n            for (unsigned i = 0; i <= s_size; i++)\n\
    \                tb[i] = need_bits(number_size(i)-1);\n        }\n    } n_len;\n\
    \    static unsigned number_bits(unsigned n) {\n        return n_len.tb[n];\n\
    \    }\n\n};\n\ntemplate<class Def>\ntypename RRRTable<Def>::number_bits_table\
    \ RRRTable<Def>::n_len;\n\ntemplate<unsigned SSize, class SType>\nstruct RRRDefinition\
    \ {\n    static constexpr unsigned s_size = SSize;\n    using s_type = SType;\n\
    \    static constexpr unsigned n_bits = need_bits(s_size);\n};\n\n/** \n * @brief\
    \ Succinct bit vector in memory of B(n, u) + O(u log log n / log n) bits \n *\
    \        where u is number of bits and n is number of 1s\n*/\ntemplate<\n    unsigned\
    \ SSize = 63,\n    class SType = uint64_t,\n    class MapType = std::map<size_t,\
    \ SType>\n    >\nstruct RRR {\n    using def = RRRDefinition<SSize, SType>;\n\
    \    using s_type = typename def::s_type;\n    using rrr_table_type = RRRTable<def>;\n\
    \    using map_type = MapType;\n    using ty_type = TY<size_t>;\n\n    map_type\
    \ s_map;\n    ty_type heads;\n    Bitmap bm;\n\n    RRR() = default;\n    void\
    \ set(size_t i, bool b) {\n        if (b)\n            s_map[i/def::s_size] |=\
    \ (s_type)1<<(i%def::s_size);\n        else\n            s_map[i/def::s_size]\
    \ &= ~((s_type)1<<(i%def::s_size));\n    }\n    void build() {\n        size_t\
    \ h = 0;\n        size_t pq = 0;\n        for (auto qm : s_map) {\n          \
    \  auto qidx = qm.first;\n            auto mask = qm.second;\n            while\
    \ (pq < qidx) {\n                heads.push_back(h);\n                auto w =\
    \ def::n_bits;\n                bm.resize(h+w);\n                bm.range_set(h,\
    \ h+w, 0);\n                h += w;\n                pq++;\n            }\n  \
    \          heads.push_back(h);\n            auto np = rrr_table_type::get_pc_and_number(mask);\n\
    \            auto n = np.first;\n            auto p = np.second;\n           \
    \ assert(rrr_table_type::get_int(n, p) == mask);\n            auto w = def::n_bits\
    \ + rrr_table_type::number_bits(n);\n            bm.resize(h+w);\n           \
    \ bm.range_set(h, h+def::n_bits, n);\n            bm.range_set(h+def::n_bits,\
    \ h+w, p);\n            assert(rrr_table_type::get_int(\n                bm.range_get(h,\
    \ h+def::n_bits), bm.range_get(h+def::n_bits, h+w)) == mask);\n            h +=\
    \ w;\n            pq++;\n        }\n        s_map.clear();\n    }\n    bool get_bit(size_t\
    \ si, unsigned off) const {\n        if (si >= heads.size())\n            return\
    \ false;\n        auto a = heads.get(si);\n        auto b = a+def::n_bits;\n \
    \       auto n = bm.range_get(a, b);\n        auto p = bm.range_get(b, b+rrr_table_type::number_bits(n));\n\
    \        return rrr_table_type::get_bit(n, p, off);\n    }\n    s_type get_mask(size_t\
    \ si) const {\n        if (si >= heads.size())\n            return 0;\n      \
    \  auto a = heads.get(si);\n        auto b = a+def::n_bits;\n        auto n =\
    \ bm.range_get(a, b);\n        auto p = bm.range_get(b, b+rrr_table_type::number_bits(n));\n\
    \        return rrr_table_type::get_int(n, p);\n    }\n    uint64_t get_word(size_t\
    \ si) const {\n        return get_mask(si);\n    }\n    size_t word_size() const\
    \ {\n        return heads.size();\n    }\n    size_t size() const {\n        return\
    \ heads.size() * def::s_size;\n    }\n    bool empty() const {\n        return\
    \ size() == 0;\n    }\n\n    bool get(size_t i) const {\n        return get_bit(i/def::s_size,\
    \ i%def::s_size);\n    }\n\n};\n\ntemplate<unsigned SSize, class SType, class\
    \ MapType>\nstruct RankSelectTraits<RRR<SSize, SType, MapType>> {\n    using rank_select_type\
    \ = BV<RRR<SSize, SType, MapType>, SSize>;\n};\n#line 7 \"include/mtl/succinct/binary_set.hpp\"\
    \n\ntemplate<class BitmapType = RRR<>>\nclass BinarySet {\npublic:\n    using\
    \ value_type = uint32_t;\nprivate:\n    using bitmap_type = BitmapType;\n    using\
    \ rs_type = typename RankSelectTraits<bitmap_type>::rank_select_type;\n    bitmap_type\
    \ bm_;\n    rs_type rs_;\npublic:\n    BinarySet() = default;\n    BinarySet(const\
    \ BinarySet& rhs) : bm_(rhs.bm_), rs_(rhs.rs_) {\n        rs_.set_ptr(&bm_);\n\
    \    }\n    BinarySet& operator=(const BinarySet& rhs) {\n        bm_ = rhs.bm_;\n\
    \        rs_ = rhs.rs_;\n        rs_.set_ptr(&bm_);\n        return *this;\n \
    \   }\n    BinarySet(BinarySet&& rhs) : bm_(std::move(rhs.bm_)), rs_(std::move(rhs.rs_))\
    \ {\n        rs_.set_ptr(&bm_);\n    }\n    BinarySet& operator=(BinarySet&& rhs)\
    \ {\n        bm_ = std::move(rhs.bm_);\n        rs_ = std::move(rhs.rs_);\n  \
    \      rs_.set_ptr(&bm_);\n        return *this;\n    }\n    ~BinarySet() = default;\n\
    \n    void insert(value_type x) {\n        bm_.set(x, 1);\n    }\n    void build()\
    \ {\n        bm_.build();\n        rs_.build(&bm_);\n    }\n    size_t size()\
    \ const { return rs_.rank1(bm_.size()); }\n    bool empty() const { return size()\
    \ == 0;}\n    bool contains(value_type x) const {\n        return bm_.get(x);\n\
    \    }\n    value_type lower_bound(value_type x) const {\n        if (contains(x))\
    \ return x;\n        return select(rank(x));\n    }\n    size_t rank(value_type\
    \ x) const {\n        return rs_.rank1(x);\n    }\n    value_type select(size_t\
    \ i) const {\n        return rs_.select1(i);\n    }\n};\n\ntemplate<class BitmapType\
    \ = RRR<>>\nclass BinaryMultiset {\npublic:\n    using value_type = uint32_t;\n\
    \    static constexpr value_type ValueMax = std::numeric_limits<value_type>::max();\n\
    private:\n    using bitmap_type = BitmapType;\n    using rs_type = typename RankSelectTraits<bitmap_type>::rank_select_type;\n\
    \    bitmap_type bm_;\n    rs_type rs_;\n    std::vector<value_type> values_;\n\
    public:\n    BinaryMultiset() = default;\n    BinaryMultiset(const BinaryMultiset&\
    \ rhs) : bm_(rhs.bm_), rs_(rhs.rs_), values_(rhs.values_) {\n        rs_.set_ptr(&bm_);\n\
    \    }\n    BinaryMultiset& operator=(const BinaryMultiset& rhs) {\n        bm_\
    \ = rhs.bm_;\n        rs_ = rhs.rs_;\n        values_ = rhs.values_;\n       \
    \ rs_.set_ptr(&bm_);\n        return *this;\n    }\n    BinaryMultiset(BinaryMultiset&&\
    \ rhs) noexcept \n        : bm_(std::move(rhs.bm_)), rs_(std::move(rhs.rs_)),\
    \ values_(std::move(rhs.values_)) {\n        rs_.set_ptr(&bm_);\n    }\n    BinaryMultiset&\
    \ operator=(BinaryMultiset&& rhs) noexcept {\n        bm_ = std::move(rhs.bm_);\n\
    \        rs_ = std::move(rhs.rs_);\n        values_ = std::move(rhs.values_);\n\
    \        rs_.set_ptr(&bm_);\n        return *this;\n    }\n    ~BinaryMultiset()\
    \ = default;\n\n    void insert(value_type x) {\n        values_.push_back(x);\n\
    \    }\n    void build() {\n        std::sort(values_.begin(), values_.end());\n\
    \        for (size_t i = 0; i < values_.size(); i++)\n            bm_.set((unsigned\
    \ long long)values_[i] + i, 1);\n        values_.clear();\n        values_.shrink_to_fit();\n\
    \        bm_.build();\n        rs_.build(&bm_);\n    }\n    size_t size() const\
    \ { return rs_.rank1(bm_.size()); }\n    bool empty() const { return size() ==\
    \ 0;}\n    size_t count(value_type x) const {\n        auto s = rs_.select0(x);\n\
    \        return s - (x == 0 ? 0 : (rs_.select0(x-1)+1));\n    }\n    bool contains(value_type\
    \ x) const {\n        auto s = rs_.select0(x);\n        return s and bm_.get(s-1);\n\
    \    }\n    value_type lower_bound(value_type x) const {\n        return select(rank(x));\n\
    \    }\n    size_t rank(value_type x) const {\n        return x == 0 ? 0 : (rs_.select0(x-1)\
    \ - (x-1));\n    }\n    value_type select(size_t i) const {\n        return rs_.select1(i)\
    \ - i;\n    }\n};\n#line 10 \"include/mtl/ordinal_range_search.hpp\"\n\r\n/**\r\
    \n * @brief Ordinal Range Search\r\n *   Maintain 2-dimential points (x,y) and\
    \ their weights.\r\n*/\r\ntemplate<typename T, typename W = long long,\r\n  class\
    \ BitmapType = RRR<>\r\n  >\r\nstruct ORS {\r\n  using index_type = T;\r\n  using\
    \ weight_type = W;\r\n  static constexpr index_type IndexLimit = std::numeric_limits<index_type>::max();\r\
    \n  std::vector<std::tuple<index_type,index_type,weight_type>> ps;\r\n  using\
    \ x_index_multiset = BinaryMultiset<BitmapType>;\r\n  using y_index_set = BinarySet<BitmapType>;\r\
    \n  x_index_multiset X;\r\n  y_index_set Y;\r\n  using wm_type = WaveletMatrix<int>;\r\
    \n  wm_type wm;\r\n  using range_sum_type = FenwickTree<W>;\r\n  range_sum_type\
    \ rsq;\r\n  index_type value_of_ith_x(size_t i) const {\r\n    return X.select(i);\r\
    \n  }\r\n  size_t index_of_lower_bound_x(const T& x) const {\r\n    return X.rank(x);\r\
    \n  }\r\n  std::pair<size_t, size_t> range_on_wm_lower_bound_x(const T& x) const\
    \ {\r\n    auto l = index_of_lower_bound_x(x);\r\n    auto lb = value_of_ith_x(l);\r\
    \n    auto c = X.count(lb);\r\n    return std::make_pair(l, l+c);\r\n  }\r\n \
    \ index_type value_of_ith_y(size_t i) const {\r\n    return Y.select(i);\r\n \
    \ }\r\n  size_t index_of_lower_bound_y(const T& y) const {\r\n    return Y.rank(y);\r\
    \n  }\r\n\r\n  ORS() = default;\r\n\r\n  void add(index_type x, index_type y,\
    \ W w = 1) {\r\n    ps.emplace_back(x,y,w);\r\n  }\r\n  void build(index_type\
    \ max_index = IndexLimit) {\r\n    std::sort(ps.begin(), ps.end(),\r\n       \
    \       [](auto l, auto r) {\r\n      return std::get<0>(l) != std::get<0>(r)\
    \ ? std::get<0>(l) < std::get<0>(r)\r\n                                      \
    \        : std::get<1>(l) < std::get<1>(r);\r\n    });\r\n\r\n    for (size_t\
    \ i = 0; i < ps.size(); i++) {\r\n      auto x = std::get<0>(ps[i]);\r\n     \
    \ auto y = std::get<1>(ps[i]);\r\n      X.insert(x);\r\n      Y.insert(y);\r\n\
    \    }\r\n    X.insert(max_index);\r\n    Y.insert(max_index);\r\n    X.build();\r\
    \n    Y.build();\r\n\r\n    {\r\n      std::vector<int> S(ps.size());\r\n    \
    \  std::vector<std::pair<int, W>> SW(ps.size()), swz, swo;\r\n      for (size_t\
    \ i = 0; i < ps.size(); i++) {\r\n        auto y = std::get<1>(ps[i]);\r\n   \
    \     auto w = std::get<2>(ps[i]);\r\n        S[i] = index_of_lower_bound_y(y);\r\
    \n        SW[i] = {S[i], w};\r\n      }\r\n      ps.clear();\r\n      ps.shrink_to_fit();\r\
    \n      wm = wm_type(S.begin(), S.end());\r\n      rsq = range_sum_type(wm.n *\
    \ (wm.h+1));\r\n      for (int k = wm.h-1; k >= 0; k--) {\r\n        for (size_t\
    \ i = 0; i < wm.n; i++) {\r\n          if (((SW[i].first >> k) & 1u) == 0)\r\n\
    \            swz.push_back(SW[i]);\r\n          else\r\n            swo.push_back(SW[i]);\r\
    \n          rsq.add(wm.n * (wm.h-1-k) + i, SW[i].second);\r\n        }\r\n   \
    \     size_t j = wm.n-1;\r\n        while (!swo.empty()) {\r\n          SW[j--]\
    \ = swo.back();\r\n          swo.pop_back();\r\n        }\r\n        while (!swz.empty())\
    \ {\r\n          SW[j--] = swz.back();\r\n          swz.pop_back();\r\n      \
    \  }\r\n      }\r\n      for (size_t i = 0; i < wm.n; i++) {\r\n        rsq.add(wm.n\
    \ * wm.h + i, SW[i].second);\r\n      }\r\n    }\r\n  }\r\n\r\n private:\r\n \
    \ std::tuple<int, int, int, int>\r\n  compress_idx(const index_type& xl, \r\n\
    \               const index_type& xh, \r\n               const index_type& yl,\
    \ \r\n               const index_type& yh) const {\r\n    return std::make_tuple(\r\
    \n      index_of_lower_bound_x(xl),\r\n      index_of_lower_bound_x(xh),\r\n \
    \     index_of_lower_bound_y(yl),\r\n      index_of_lower_bound_y(yh)\r\n    );\r\
    \n  }\r\n\r\n  template<typename F, typename G>\r\n  void _traverse(const index_type&\
    \ xl, \r\n                 const index_type& xh, \r\n                 const index_type&\
    \ yl, \r\n                 const index_type& yh, \r\n                 F internal_fn,\
    \ \r\n                 G leaf_fn) const {\r\n    size_t i,j;\r\n    int a,b;\r\
    \n    std::tie(i,j,a,b) = compress_idx(xl, xh, yl, yh);\r\n    std::queue<std::tuple<size_t,size_t,T>>\
    \ qs;\r\n    qs.emplace(i,j,T(0));\r\n    while (!qs.empty()) {\r\n      size_t\
    \ l,r;\r\n      T c;\r\n      std::tie(l,r,c) = qs.front(); qs.pop();\r\n    \
    \  size_t level = l/wm.n;\r\n      int shift = wm.h-1-level;\r\n      T clo =\
    \ c, chi = c | ((1ull<<(shift+1))-1);\r\n      if (chi < a or b <= clo)\r\n  \
    \      continue;\r\n      if (a <= clo and chi < b) {\r\n        internal_fn(l,\
    \ r, c);\r\n        continue;\r\n      }\r\n      if (level == wm.h) {\r\n   \
    \     leaf_fn(l, r, c);\r\n        continue;\r\n      }\r\n      size_t nl,nr;\r\
    \n      std::tie(nl,nr) = wm._child_tie0(level, l, r);\r\n      qs.emplace(nl,\
    \ nr, c);\r\n      std::tie(nl,nr) = wm._child_tie1(level, l, r);\r\n      qs.emplace(nl,\
    \ nr, c | (1ull<<shift));\r\n    }\r\n  }\r\n\r\n public:\r\n  size_t freq(const\
    \ index_type& xl, \r\n              const index_type& xh, \r\n              const\
    \ index_type& yl, \r\n              const index_type& yh) const {\r\n    size_t\
    \ i,j;\r\n    int a,b;\r\n    std::tie(i,j,a,b) = compress_idx(xl, xh, yl, yh);\r\
    \n    return wm.range_freq(i, j, a, b);\r\n  }\r\n\r\n  void weight_add(const\
    \ index_type& x, const index_type& y, const weight_type& w) {\r\n    size_t l,\
    \ r;\r\n    std::tie(l,r) = range_on_wm_lower_bound_x(x);\r\n    auto c = index_of_lower_bound_y(y);\r\
    \n    for (int k = wm.h-1; k >= 0; k--) {\r\n      std::tie(l,r) = wm.child_tie(wm.h-1-k,\
    \ l, r, (c >> k) & 1u);\r\n    }\r\n    assert(l != r);\r\n    size_t j = l;\r\
    \n    for (size_t k = 0; k < wm.h; k++) {\r\n      rsq.add(j, w);\r\n      j =\
    \ wm.parent(wm.h-1-k, j, (c >> k) & 1u);\r\n    }\r\n    rsq.add(j, w);\r\n  }\r\
    \n\r\n  W sum(const index_type& xl, \r\n        const index_type& xh, \r\n   \
    \     const index_type& yl, \r\n        const index_type& yh) const {\r\n    W\
    \ ret = 0;\r\n    _traverse(xl, xh, yl, yh,\r\n              [&](size_t l, size_t\
    \ r, auto _) { ret += rsq.range_sum(l, r); },\r\n              [](auto,auto,auto){});\r\
    \n    return ret;\r\n  }\r\n\r\n  std::vector<std::pair<size_t, size_t>>\r\n \
    \ enumerate(const index_type& xl, \r\n            const index_type& xh, \r\n \
    \           const index_type& yl, \r\n            const index_type& yh) const\
    \ {\r\n    std::vector<std::pair<size_t,size_t>> ret;\r\n    _traverse(xl, xh,\
    \ yl, yh,\r\n              [](auto,auto,auto){},\r\n              [&](size_t l,\
    \ size_t r, T c) {\r\n      for (size_t idx = l; idx < r; idx++) {\r\n       \
    \ auto jdx = idx;\r\n        for (size_t k = 0; k < wm.h; k++)\r\n          jdx\
    \ = wm.parent(wm.h-1-k, jdx, (c >> k) & 1u);\r\n        ret.emplace_back(value_of_ith_x(jdx),\
    \ value_of_ith_y(c));\r\n      }\r\n    });\r\n    return ret;\r\n  }\r\n};\n\
    #line 3 \"test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp\"\n#include <bits/stdc++.h>\r\
    \nusing namespace std;\r\n\r\nint main() {\r\n  int n,q; cin>>n>>q;\r\n  int index_max\
    \ = 0;\r\n  auto chmax = [&](int& a, int b) { a = max(a, b); };\r\n  ORS<int,\
    \ long long> ors;\r\n  for (int i = 0; i < n; i++) {\r\n    int x,y,w; cin>>x>>y>>w;\r\
    \n    ors.add(x,y,w);\r\n    chmax(index_max, max(x, y));\r\n  }\r\n  vector<tuple<int,int,int,int,int>>\
    \ Q(q);\r\n  for (int i = 0; i < q; i++) {\r\n    int t; cin>>t;\r\n    if (t\
    \ == 0) {\r\n      int x,y,w; cin>>x>>y>>w;\r\n      Q[i] = {0,x,y,w,0};\r\n \
    \     ors.add(x,y,0);\r\n      chmax(index_max, max(x, y));\r\n    } else {\r\n\
    \      int l,d,r,u; cin>>l>>d>>r>>u;\r\n      Q[i] = {1,l,d,r,u};\r\n      chmax(index_max,\
    \ max(r, u));\r\n    }\r\n  }\r\n  ors.build(index_max);\r\n  for (int i = 0;\
    \ i < q; i++) {\r\n    int t = get<0>(Q[i]);\r\n    if (t == 0) {\r\n      int\
    \ x,y,w,_;\r\n      std::tie(t,x,y,w,_) = Q[i];\r\n      ors.weight_add(x,y,w);\r\
    \n    } else {\r\n      int l,d,r,u;\r\n      std::tie(t,l,d,r,u) = Q[i];\r\n\
    \      cout << ors.sum(l,r,d,u) << endl;\r\n    }\r\n  }\r\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/point_add_rectangle_sum\"\
    \r\n#include \"../../include/mtl/ordinal_range_search.hpp\"\r\n#include <bits/stdc++.h>\r\
    \nusing namespace std;\r\n\r\nint main() {\r\n  int n,q; cin>>n>>q;\r\n  int index_max\
    \ = 0;\r\n  auto chmax = [&](int& a, int b) { a = max(a, b); };\r\n  ORS<int,\
    \ long long> ors;\r\n  for (int i = 0; i < n; i++) {\r\n    int x,y,w; cin>>x>>y>>w;\r\
    \n    ors.add(x,y,w);\r\n    chmax(index_max, max(x, y));\r\n  }\r\n  vector<tuple<int,int,int,int,int>>\
    \ Q(q);\r\n  for (int i = 0; i < q; i++) {\r\n    int t; cin>>t;\r\n    if (t\
    \ == 0) {\r\n      int x,y,w; cin>>x>>y>>w;\r\n      Q[i] = {0,x,y,w,0};\r\n \
    \     ors.add(x,y,0);\r\n      chmax(index_max, max(x, y));\r\n    } else {\r\n\
    \      int l,d,r,u; cin>>l>>d>>r>>u;\r\n      Q[i] = {1,l,d,r,u};\r\n      chmax(index_max,\
    \ max(r, u));\r\n    }\r\n  }\r\n  ors.build(index_max);\r\n  for (int i = 0;\
    \ i < q; i++) {\r\n    int t = get<0>(Q[i]);\r\n    if (t == 0) {\r\n      int\
    \ x,y,w,_;\r\n      std::tie(t,x,y,w,_) = Q[i];\r\n      ors.weight_add(x,y,w);\r\
    \n    } else {\r\n      int l,d,r,u;\r\n      std::tie(t,l,d,r,u) = Q[i];\r\n\
    \      cout << ors.sum(l,r,d,u) << endl;\r\n    }\r\n  }\r\n}"
  dependsOn:
  - include/mtl/ordinal_range_search.hpp
  - include/mtl/fenwick_tree.hpp
  - include/mtl/bit_manip.hpp
  - include/mtl/succinct/wavelet_matrix.hpp
  - include/mtl/succinct/bit_vector.hpp
  - include/mtl/succinct/select.hpp
  - include/mtl/succinct/bv.hpp
  - include/mtl/succinct/rrr.hpp
  - include/mtl/succinct/ty.hpp
  - include/mtl/succinct/binary_set.hpp
  isVerificationFile: true
  path: test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
  requiredBy: []
  timestamp: '2023-04-14 01:09:23+09:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
- /verify/test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp.html
title: test/yosupo/yosupo-point_add_rectangle_sum-wm.test.cpp
---
