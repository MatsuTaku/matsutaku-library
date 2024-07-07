---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':x:'
    path: include/mtl/compress_int.hpp
    title: include/mtl/compress_int.hpp
  - icon: ':question:'
    path: include/mtl/lazy_segment_tree.hpp
    title: include/mtl/lazy_segment_tree.hpp
  - icon: ':x:'
    path: include/mtl/mo.hpp
    title: 'Mo''s algorithm: solve offline segment queries on a sequence'
  - icon: ':question:'
    path: include/mtl/monoid.hpp
    title: include/mtl/monoid.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/static_range_inversions_query
    links:
    - https://judge.yosupo.jp/problem/static_range_inversions_query
  bundledCode: "#line 1 \"test/yosupo/static_range_inversions_query-mo_rollback.test.cpp\"\
    \n#define PROBLEM \"https://judge.yosupo.jp/problem/static_range_inversions_query\"\
    \r\n#line 2 \"include/mtl/compress_int.hpp\"\n#include <set>\r\n#include <unordered_map>\r\
    \n#include <vector>\r\n#include <algorithm>\r\n\r\ntemplate<class T, class MapContainer=std::unordered_map<T,\
    \ int>>\r\nclass Compressor {\r\n public:\r\n  using map_type = MapContainer;\r\
    \n private:\r\n  std::vector<T> vs;\r\n public:\r\n  Compressor() = default;\r\
    \n  template<typename It>\r\n  Compressor(It begin, It end) : vs(begin, end) {}\r\
    \n  void clear() { vs.clear(); }\r\n  void add(T x) {\r\n    vs.push_back(x);\r\
    \n  }\r\n  template<typename It>\r\n  void add(It begin, It end) {\r\n    vs.insert(vs.end(),\
    \ begin, end);\r\n  }\r\n  map_type release() {\r\n    std::sort(vs.begin(), vs.end());\r\
    \n    vs.erase(std::unique(vs.begin(), vs.end()), vs.end());\r\n    map_type mp;\r\
    \n    mp.reserve(vs.size());\r\n    int k = 0;\r\n    for (auto v : vs) mp[v]\
    \ = k++;\r\n    return mp;\r\n  }\r\n  std::pair<map_type, std::vector<T>> release_tie()\
    \ {\r\n    return std::make_pair(release(), std::move(vs));\r\n  }\r\n  template<typename\
    \ It>\r\n  static map_type compress(It begin, It end) {\r\n    return Compressor(begin,\
    \ end).release();\r\n  }\r\n};\r\n#line 3 \"include/mtl/mo.hpp\"\n#include <numeric>\r\
    \n#include <cmath>\r\n#include <tuple>\r\n#line 7 \"include/mtl/mo.hpp\"\n#include\
    \ <iostream>\r\n#include <cassert>\r\n\r\n/**\r\n * @brief Mo's algorithm: solve\
    \ offline segment queries on a sequence\r\n * @note  This implementation is optimized\
    \ by noshi's idea\r\n *          complexity of N sqrt(Q) + O(N).\r\n *       \
    \ - \u5B9A\u6570\u500D\u304C\u6700\u9069\u306A Mo's Algorithm. noshi91\u306E\u30E1\
    \u30E2. 2023/04/13.\r\n *          https://noshi91.hatenablog.com/entry/2023/04/13/224811\r\
    \n * @note  Mo's algorithm with rollback is based on snuke's idea\r\n *      \
    \  - Mo's algorithm \u3068\u305D\u306E\u4E0A\u4F4D\u4E92\u63DB\u306E\u8A71. \u3042\
    \u306A\u305F\u306F\u5618\u3064\u304D\u3067\u3059\u304B\u3068\u805E\u304B\u308C\
    \u305F\u3089\u300CYES\u300D\u3068\u7B54\u3048\u308B\u30D6\u30ED\u30B0. 2016/07/01.\r\
    \n *          https://snuke.hatenablog.com/entry/2016/07/01/000000\r\n*/\r\nstruct\
    \ MoSolver {\r\n  std::vector<std::tuple<int, int, int>> segs;\r\n  int q = 0;\r\
    \n\r\n  void add_segment(int l, int r) { \r\n    assert(l <= r);\r\n    segs.emplace_back(q++,\
    \ l, r);\r\n  }\r\n\r\n  void calc_mos_move(std::vector<int>& dst) const {\r\n\
    \    using std::get;\r\n    int n = 0;\r\n    for (auto s:segs)\r\n      n = std::max(n,\
    \ get<2>(s));\r\n    auto rtq = std::sqrt(q);\r\n    int b = std::ceil((double)n\
    \ / rtq);\r\n    auto bf = b-b/2;\r\n    auto get_bo = [&](int x) {\r\n      if\
    \ (x < bf) return 0;\r\n      return (x-bf)/b+1;\r\n    };\r\n    auto EvenComp\
    \ = [&](int u, int v) {\r\n      auto &s = segs[u], &t = segs[v];\r\n      auto\
    \ ls = get<1>(s), rs = get<2>(s), lt = get<1>(t), rt = get<2>(t);\r\n      auto\
    \ bs = ls / b, bt = lt / b;\r\n      return bs != bt ? ls < lt : (bs%2==0 ? rs\
    \ < rt : rs > rt);\r\n    };\r\n    auto OddComp = [&](int u, int v) {\r\n   \
    \   auto &s = segs[u], &t = segs[v];\r\n      auto ls = get<1>(s), rs = get<2>(s),\
    \ lt = get<1>(t), rt = get<2>(t);\r\n      auto bs = get_bo(ls), bt = get_bo(lt);\r\
    \n      return bs != bt ? ls < lt : (bs%2==0 ? rs < rt : rs > rt);\r\n    };\r\
    \n    auto& IE = dst;\r\n    IE.resize(q);\r\n    std::iota(IE.begin(), IE.end(),\
    \ 0);\r\n    std::sort(IE.begin(), IE.end(), \r\n      EvenComp);\r\n    auto\
    \ IO = IE;\r\n    std::sort(IO.begin(), IO.end(), \r\n      OddComp);\r\n    auto\
    \ move_distance = [&](const std::vector<int>& ids) {\r\n      long long d = 0;\r\
    \n      for (int i = 0; i < q-1; i++) {\r\n        int j = ids[i], k = ids[i+1];\r\
    \n        d += std::abs(get<1>(segs[j]) - get<1>(segs[k]));\r\n        d += std::abs(get<2>(segs[j])\
    \ - get<2>(segs[k]));\r\n      }\r\n      return d;\r\n    };\r\n    if (move_distance(IE)\
    \ > move_distance(IO))\r\n      dst = std::move(IO); // IE is reference of dst\r\
    \n  }\r\n\r\n  template<class PUSH_L, class PUSH_R, class POP_L, class POP_R,\
    \ class REM>\r\n  void solve(PUSH_L pushl, PUSH_R pushr, POP_L popl, POP_R popr,\
    \ REM rem) const {\r\n    if (q == 0) return;\r\n    std::vector<int> I;\r\n \
    \   calc_mos_move(I);\r\n    int _l = 0, _r = 0;\r\n    for (int i:I) {\r\n  \
    \    int t,l,r;\r\n      std::tie(t,l,r) = segs[i];\r\n      while (l < _l)\r\n\
    \        pushl(--_l);\r\n      while (_r < r)\r\n        pushr(_r++);\r\n    \
    \  while (_l < l)\r\n        popl(_l++);\r\n      while (r < _r)\r\n        popr(--_r);\r\
    \n      rem(t);\r\n    }\r\n  }\r\n\r\n  template<class Block, class Bend>\r\n\
    \  long long calc_mos_rollback_move(std::vector<int>& idx, std::vector<std::pair<int,int>>&\
    \ blocks, Block block, Bend bend) const {\r\n    std::sort(idx.begin(), idx.end(),\
    \ [&](auto a, auto b) {\r\n      auto [ta, la, ra] = segs[a];\r\n      auto [tb,\
    \ lb, rb] = segs[b];\r\n      auto ba = block(la), bb = block(lb);\r\n      return\
    \ ba != bb ? la < lb : ra < rb;\r\n    });\r\n    long long dist = 0;\r\n    int\
    \ cb = -1;\r\n    int _l = 0, _r = 0;\r\n    for (size_t i = 0; i < idx.size();\
    \ i++) {\r\n      auto [t,l,r] = segs[idx[i]];\r\n      auto bi = block(l);\r\n\
    \      auto be = bend(bi);\r\n      blocks[i] = std::make_pair(bi, be);\r\n  \
    \    if (bi != cb) {\r\n        _r = be;\r\n        cb = bi;\r\n      }\r\n  \
    \    dist += r-_r;\r\n      _r = r;\r\n      _l = be;\r\n      dist += _l-l;\r\
    \n    }\r\n    return dist;\r\n  }\r\n\r\n  template<class PUSH_L, class PUSH_R,\
    \ class REM, \r\n           class INIT, class SNAPSHOT, class ROLLBACK> \r\n \
    \ void solve_rollback(PUSH_L pushl, PUSH_R pushr, REM rem, \r\n             INIT\
    \ init, SNAPSHOT snapshot, ROLLBACK rollback) const {\r\n    if (q == 0) return;\r\
    \n    int n = 0;\r\n    for (auto s:segs)\r\n      n = std::max(n, std::get<2>(s));\r\
    \n//  * (2|xi-c|+b/2)q, |xi-c| < b/2, (mean |xi-c| = b/4) -> bq\r\n//  * min bq\
    \ + n^2/b, \r\n//    from AMGM, bq = n^2/b => b^2 = n^2 /q => b = n / sqrt(q)\r\
    \n//  * F = (bq + n^2/b)/2\r\n//      = bq\r\n//      = n sqrt(q)\r\n    const\
    \ int b = std::ceil((double)n / std::sqrt(q));\r\n    std::vector<int> J;\r\n\
    \    for (int i = 0; i < q; i++) {\r\n      auto [t,l,r] = segs[i];\r\n      if\
    \ (r-l < b) {\r\n        init();\r\n        for (int j = l; j < r; j++)\r\n  \
    \        pushr(j);\r\n        rem(t);\r\n      } else {\r\n        J.push_back(i);\r\
    \n      }\r\n    }\r\n    \r\n    std::vector<std::pair<int,int>> B(J.size());\r\
    \n    {\r\n      auto& b_even = B;\r\n      auto block_even = [&](int x) {\r\n\
    \        return x / b;\r\n      };\r\n      auto bend_even = [&](int bi) {\r\n\
    \        return (bi+1)*b;\r\n      };\r\n      auto dist_e = calc_mos_rollback_move(J,\
    \ b_even, block_even, bend_even);\r\n\r\n      auto K = J;\r\n      auto b_odd\
    \ = B;\r\n      const auto bf = b-b/2;\r\n      auto block_odd = [&](int x) {\r\
    \n        return x < bf ? 0 : (x-bf)/b+1;\r\n      };\r\n      auto bend_odd =\
    \ [&](int bi) {\r\n        return bf+bi*b;\r\n      };\r\n      auto dist_o =\
    \ calc_mos_rollback_move(K, b_odd, block_odd, bend_odd);\r\n\r\n      if (dist_e\
    \ > dist_o) {\r\n        J = std::move(K);\r\n        B = std::move(b_odd);\r\n\
    \      }\r\n    }\r\n\r\n    int cb = -1;\r\n    int _l = 0, _r = 0;\r\n    for\
    \ (auto it = J.begin(); it != J.end(); ++it) {\r\n      auto [t,l,r] = segs[*it];\r\
    \n      auto [bi,be] = B[it-J.begin()];\r\n      if (bi != cb) {\r\n        init();\r\
    \n        _r = be;\r\n        cb = bi;\r\n      }\r\n      assert(_r <= r);\r\n\
    \      while (_r < r) {\r\n        pushr(_r++);\r\n      }\r\n      snapshot();\r\
    \n      _l = be;\r\n      assert(l <= _l);\r\n      assert(_l-l <= b);\r\n   \
    \   while (l < _l) {\r\n        pushl(--_l);\r\n      }\r\n      rem(t);\r\n \
    \     rollback();\r\n    }\r\n  }\r\n};\r\n#line 2 \"include/mtl/bit_manip.hpp\"\
    \n#include <cstdint>\n#line 4 \"include/mtl/bit_manip.hpp\"\n\nnamespace bm {\n\
    \ninline constexpr uint64_t popcnt_e8(uint64_t x) {\n  x = (x & 0x5555555555555555)\
    \ + ((x>>1) & 0x5555555555555555);\n  x = (x & 0x3333333333333333) + ((x>>2) &\
    \ 0x3333333333333333);\n  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) & 0x0F0F0F0F0F0F0F0F);\n\
    \  return x;\n}\n// Count 1s\ninline constexpr unsigned popcnt(uint64_t x) {\n\
    \  return (popcnt_e8(x) * 0x0101010101010101) >> 56;\n}\n// Count trailing 0s.\
    \ s.t. *11011000 -> 3\ninline constexpr unsigned ctz(uint64_t x) {\n  return popcnt((x\
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
    \ 1);\n  return x;\n}\n\n} // namespace bm\n#line 3 \"include/mtl/lazy_segment_tree.hpp\"\
    \n// #include \"monoid.hpp\"\n#include <cstddef>\n#include <utility>\n#line 7\
    \ \"include/mtl/lazy_segment_tree.hpp\"\n#include <stack>\n#line 9 \"include/mtl/lazy_segment_tree.hpp\"\
    \n#if __cpp_concepts >= 202002L\n#include <concepts>\n\ntemplate<typename M>\n\
    concept LazySegmentTreeMonoid = requires (M m) {\n  {m * m} -> std::same_as<M>;\n\
    };\ntemplate<typename A, typename M>\nconcept LazySegmentTreeOperatorMonoid =\
    \ requires(A a, M m) {\n  {a *= a} -> std::same_as<A&>;\n  {a.act(m)} -> std::same_as<M>;\n\
    };\n#endif\n\ntemplate <typename M, typename A>\n#if __cpp_concepts >= 202002L\n\
    requires LazySegmentTreeMonoid<M> &&\n         LazySegmentTreeOperatorMonoid<A,M>\n\
    #endif\nclass LazySegmentTree {\n private:\n  size_t size_;\n  std::vector<std::pair<M,\
    \ A>> tree_;\n  std::vector<size_t> ids_;\n\n public:\n  explicit LazySegmentTree(size_t\
    \ size) :\n      size_(1ull<<(64-bm::clz(size-1))),\n      tree_(size_*2) {\n\
    \    ids_.reserve((64-bm::clz(size-1))*2);\n  }\n\n  template <typename Iter>\n\
    \  explicit LazySegmentTree(Iter begin, Iter end)\n    : LazySegmentTree(std::distance(begin,\
    \ end)) {\n    static_assert(std::is_convertible<typename std::iterator_traits<Iter>::value_type,\
    \ M>::value, \"\");\n    for (auto it = begin; it != end; ++it) {\n      tree_[size_\
    \ + it - begin].first = *it;\n    }\n    for (size_t i = size_-1; i > 0; i--)\
    \ {\n      tree_[i].first = tree_[i*2].first * tree_[i*2+1].first;\n    }\n  }\n\
    \n  inline void range_update(size_t l, size_t r, const A& e) {\n    assert(l <=\
    \ r);\n    assert(r <= size_);\n    if (l == r) return;\n    _lazy_propagation(l,\
    \ r);\n\n    for (size_t _l=l+size_, _r=r+size_; _l<_r; _l>>=1, _r>>=1) {\n  \
    \    if (_l&1) \n        tree_[_l++].second *= e;\n      if (_r&1) \n        tree_[--_r].second\
    \ *= e;\n    }\n\n    for (auto id : ids_) {\n      _propagate(id*2);\n      _propagate(id*2+1);\n\
    \      tree_[id].first = tree_[id*2].first * tree_[id*2+1].first;\n    }\n  }\n\
    \  inline void update(size_t l, size_t r, const A& e) {\n    range_update(l, r,\
    \ e);\n  }\n  inline void update(size_t i, const A& e) {\n    range_update(i,\
    \ i+1, e);\n  }\n\n  template<typename T>\n  inline void set(size_t i, T&& e)\
    \ {\n    _lazy_propagation(i, i+1);\n    int u = i+size_;\n    tree_[u].first\
    \ = M(std::forward<T>(e));\n    u >>= 1;\n    while (u > 0) {\n      tree_[u].first\
    \ = tree_[u*2].first * tree_[u*2+1].first;\n      u >>= 1;\n    }\n  }\n\n  inline\
    \ M query(size_t l, size_t r) {\n    _lazy_propagation(l, r);\n\n    M lhs,rhs;\n\
    \    for (size_t _l=l+size_, _r=r+size_; _l<_r; _l>>=1, _r>>=1) {\n      if (_l&1)\
    \ {\n        _propagate(_l);\n        lhs = lhs * tree_[_l].first;\n        ++_l;\n\
    \      }\n      if (_r&1) {\n        --_r;\n        _propagate(_r);\n        rhs\
    \ = tree_[_r].first * rhs;\n      }\n    }\n    return lhs * rhs;\n  }\n  ///\
    \ Alias for query(l, r)\n  M prod(size_t l, size_t r) {\n    return query(l, r);\n\
    \  }\n\n  inline const M& get(size_t index) {\n    _lazy_propagation(index, index+1);\n\
    \    auto l = index+size_;\n    _propagate(l);\n    return tree_[l].first;\n \
    \ }\n\n private:\n  void _set_ids(size_t l, size_t r) {\n    ids_.clear();\n \
    \   auto _l=l+size_, _r=r+size_;\n    auto lth = _l/(_l&(-_l))/2;\n    auto rth\
    \ = _r/(_r&(-_r))/2;\n    for (; _l<_r; _l>>=1, _r>>=1) {\n      if (_r <= rth)\
    \ ids_.push_back(_r);\n      if (_l <= lth) ids_.push_back(_l);\n    }\n    for\
    \ (; _l>0; _l>>=1)\n      ids_.push_back(_l);\n  }\n\n  inline void _propagate(size_t\
    \ id) {\n    A& e = tree_[id].second;\n    tree_[id].first = e.act(tree_[id].first);\n\
    \    if (id < size_) {\n      tree_[id*2].second *= e;\n      tree_[id*2+1].second\
    \ *= e;\n    }\n    tree_[id].second = A();\n  }\n\n  void _lazy_propagation(size_t\
    \ l, size_t r) {\n    if (l == r) return;\n    _set_ids(l, r);\n    for (auto\
    \ it = ids_.rbegin(); it != ids_.rend(); ++it)\n      _propagate(*it);\n  }\n\n\
    \ public:\n  template<class F>\n  size_t max_right(size_t begin, size_t end, F\
    \ f) {\n    if (begin == end) return end;\n    M p;\n    std::stack<std::pair<size_t,\
    \ M>> rps;\n    auto l = size_ + begin;\n    auto r = size_ + end;\n    _lazy_propagation(begin,\
    \ end);\n    auto access = [&](size_t i) {\n      _propagate(i);\n      return\
    \ tree_[i].first;\n    };\n    while (l < r and f(p * access(l))) {\n      if\
    \ (l&1) p = p * tree_[l++].first;\n      if (r&1) {\n        rps.emplace(r, access(r-1));\n\
    \        r--;\n      }\n      l>>=1; r>>=1;\n    }\n    if (l >= r) {\n      while\
    \ (rps.size()) {\n        auto& [r, rp] = rps.top();\n        if (!f(p * rp))\
    \ {\n          l = r-1;\n          break;\n        }\n        p = p * rp;\n  \
    \      rps.pop();\n      }\n      if (rps.empty()) return end;\n    }\n    while\
    \ (l < size_) {\n      assert(!f(p * access(l)));\n      l <<= 1;\n      auto\
    \ pl = access(l);\n      if (f(p * pl)) {\n        p = p * pl;\n        l++;\n\
    \      }\n    }\n    return l - size_;\n  }\n  template<bool (*F)(M)>\n  size_t\
    \ max_right(size_t begin, size_t end) {\n    return max_right(begin, end, [](M\
    \ x) { return F(x); });\n  }\n\n  template<class F>\n  size_t min_left(size_t\
    \ begin, size_t end, F f) {\n    if (end == begin) return begin;\n    M p;\n \
    \   std::stack<std::pair<size_t, M>> lps;\n    auto l = size_ + begin;\n    auto\
    \ r = size_ + end;\n    _lazy_propagation(begin, end);\n    auto access = [&](size_t\
    \ i) {\n      _propagate(i);\n      return tree_[i].first;\n    };\n    while\
    \ (l < r and f(access(r-1) * p)) {\n      if (l&1) {\n        lps.emplace(l, access(l));\n\
    \        l++;\n      }\n      if (r&1) p = tree_[r-1].first * p;\n      l>>=1;\
    \ r>>=1;\n    }\n    if (l >= r) {\n      while (lps.size()) {\n        auto&\
    \ [l, lp] = lps.top();\n        if (!f(lp * p)) {\n          r = l+1;\n      \
    \    break;\n        }\n        p = lp * p;\n        lps.pop();\n      }\n   \
    \   if (lps.empty()) return begin;\n    }\n    while (r <= size_) {\n      assert(!f(access(r-1)\
    \ * p));\n      r <<= 1;\n      auto pr = access(r-1);\n      if (f(pr * p)) {\n\
    \        p = pr * p;\n        --r;\n      }\n    }\n    return r - size_;\n  }\n\
    \  template<bool (*F)(M)>\n  size_t min_left(size_t begin, size_t end) {\n   \
    \ return min_left(begin, [](M x) { return F(x); });\n  }\n\n};\n\n#line 3 \"include/mtl/monoid.hpp\"\
    \n#if __cpp_concepts >= 202002L\n#include <concepts>\n#endif\n\ntemplate<class\
    \ T, T (*op)(T, T), T (*e)()>\nstruct Monoid {\n  T x;\n  Monoid() : x(e()) {}\n\
    \  template<class... Args>\n  Monoid(Args&&... args) : x(std::forward<Args>(args)...)\
    \ {}\n  Monoid operator*(const Monoid& rhs) const {\n    return Monoid(op(x, rhs.x));\n\
    \  }\n  const T& val() const {\n    return x;\n  }\n};\n\nstruct VoidMonoid {\n\
    \  VoidMonoid() {}\n  VoidMonoid operator*(const VoidMonoid& rhs) const {\n  \
    \  return VoidMonoid();\n  }\n};\n\n#if __cpp_concepts >= 202002L\ntemplate<class\
    \ T>\nconcept IsMonoid = requires (T m) {\n  { m * m } -> std::same_as<T>;\n};\n\
    #endif\n\ntemplate<class T, T (*op)(T, T), T (*e)()>\nstruct CommutativeMonoid\
    \ : public Monoid<T, op, e> {\n    using Base = Monoid<T, op, e>;\n    CommutativeMonoid(T\
    \ x=e()) : Base(x) {}\n    CommutativeMonoid operator+(const CommutativeMonoid&\
    \ rhs) const {\n        return CommutativeMonoid(*this * rhs);\n    }\n};\n\n\
    #if __cpp_concepts >= 202002L\ntemplate<class T>\nconcept IsCommutativeMonoid\
    \ = requires (T m) {\n  { m + m } -> std::same_as<T>;\n};\n#endif\n\ntemplate<class\
    \ S, class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>\nstruct OperatorMonoid\
    \ {\n    F f;\n    OperatorMonoid() : f(id()) {}\n    template<class... Args>\n\
    \    OperatorMonoid(Args&&... args) : f(std::forward<Args>(args)...) {}\n    OperatorMonoid&\
    \ operator*=(const OperatorMonoid& rhs) {\n        f = composition(rhs.f, f);\n\
    \        return *this;\n    }\n    S act(const S& s) const {\n        return mapping(f,\
    \ s);\n    }\n};\n\nstruct VoidOperatorMonoid {\n    VoidOperatorMonoid() {}\n\
    \    VoidOperatorMonoid& operator*=(const VoidOperatorMonoid& rhs) {\n       \
    \ return *this;\n    }\n    template<class T>\n    T act(const T& s) const {\n\
    \        return s;\n    }\n};\n\n#if __cpp_concepts >= 202002L\ntemplate<class\
    \ F, class S>\nconcept IsOperatorMonoid = requires (F f, S s) {\n    { f *= f\
    \ } -> std::same_as<F&>;\n    { f.act(s) } -> std::same_as<S>;\n};\n#endif\n#line\
    \ 6 \"test/yosupo/static_range_inversions_query-mo_rollback.test.cpp\"\n#include\
    \ <bits/stdc++.h>\r\nusing namespace std;\r\nusing lint = long long;\r\n\r\nconstexpr\
    \ int N = 1e5;\r\nint sum(int a, int b) { return a+b; }\r\nint e() { return 0;\
    \ }\r\nusing sm = Monoid<int,sum,e>;\r\nusing P = pair<int,int>;\r\nsm mapping(P\
    \ c, sm x) {\r\n  return x.val()*c.first + c.second;\r\n}\r\nP composition(P l,\
    \ P r) {\r\n  return {l.first*r.first, l.second*r.first + r.second};\r\n}\r\n\
    P Id() { return {1,0}; }\r\nstruct um {\r\n  P x;\r\n  um() : x(Id()) {}\r\n \
    \ um(P x) : x(x) {}\r\n  um& operator*=(const um& rhs) {\r\n    x = composition(x,\
    \ rhs.x);\r\n    return *this; \r\n  }\r\n  sm act(const sm& s) const { \r\n \
    \   return mapping(x, s);\r\n  }\r\n};\r\n\r\nint main() {\r\n  int n,q; cin>>n>>q;\r\
    \n  vector<int> A(n);\r\n  for (int i = 0; i < n; i++) {\r\n    cin>>A[i];\r\n\
    \  }\r\n  auto id = Compressor<int>::compress(A.begin(), A.end());\r\n  auto k\
    \ = id.size();\r\n  MoSolver mo;\r\n  for (int i = 0; i < q; i++) {\r\n    int\
    \ l,r; cin>>l>>r;\r\n    mo.add_segment(l, r);\r\n  }\r\n\r\n  LazySegmentTree<sm,\
    \ um> D(k);\r\n  vector<lint> ans(q, -1);\r\n  lint inv_sum = 0;\r\n  stack<int>\
    \ hist;\r\n  lint saved_inv_sum = 0;\r\n  int size = 0;\r\n  auto _pushl = [&](int\
    \ i) {\r\n    int vi = id[A[i]];\r\n    auto count = D.query(0, vi).val();\r\n\
    \    assert(count <= size);\r\n    inv_sum += count;\r\n    hist.push(vi); \r\n\
    \    D.update(vi, make_pair(1,1));\r\n    size++;   \r\n  };\r\n  auto _pushr\
    \ = [&](int i) {\r\n    int vi = id[A[i]];\r\n    auto count = D.query(vi+1, k).val();\r\
    \n    assert(count <= size);\r\n    inv_sum += count;\r\n    hist.push(vi); \r\
    \n    D.update(vi, make_pair(1,1));   \r\n    size++;\r\n  }; \r\n  // auto _popl\
    \ = [&](int i) {\r\n  //   int vi = id[A[i]];\r\n  //   inv_sum -= D.query(0,\
    \ vi).val();\r\n  //   D.update(vi, make_pair(1,-1));\r\n  // };\r\n  // auto\
    \ _popr = [&](int i) {\r\n  //   int vi = id[A[i]];\r\n  //   inv_sum -= D.query(vi+1,\
    \ k).val();\r\n  //   D.update(vi, make_pair(1,-1));\r\n  // };\r\n  auto rem\
    \ = [&](int t) {\r\n    ans[t] = inv_sum;   \r\n  }; \r\n  // mo.solve(_pushl,\
    \ _pushr, _popl, _popr, rem);\r\n\r\n  auto init = [&]() {\r\n    D.update(0,\
    \ k, make_pair(0,0));\r\n    inv_sum = 0;\r\n    size = 0;\r\n  };\r\n  auto snapshot\
    \ = [&]() {\r\n    hist = {};\r\n    saved_inv_sum = inv_sum;\r\n  };\r\n  auto\
    \ rollback = [&]() { \r\n    size -= hist.size();\r\n    while (!hist.empty())\
    \ {\r\n      int vi = hist.top(); hist.pop();\r\n      D.update(vi, make_pair(1,-1));\r\
    \n    }\r\n    inv_sum = saved_inv_sum;\r\n  }; \r\n  mo.solve_rollback(_pushl,\
    \ _pushr, rem, init, snapshot, rollback);\r\n\r\n  for (lint v : ans) cout <<\
    \ v << endl;\r\n}\r\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/static_range_inversions_query\"\
    \r\n#include \"../../include/mtl/compress_int.hpp\"\r\n#include \"../../include/mtl/mo.hpp\"\
    \r\n#include \"../../include/mtl/lazy_segment_tree.hpp\"\r\n#include \"../../include/mtl/monoid.hpp\"\
    \r\n#include <bits/stdc++.h>\r\nusing namespace std;\r\nusing lint = long long;\r\
    \n\r\nconstexpr int N = 1e5;\r\nint sum(int a, int b) { return a+b; }\r\nint e()\
    \ { return 0; }\r\nusing sm = Monoid<int,sum,e>;\r\nusing P = pair<int,int>;\r\
    \nsm mapping(P c, sm x) {\r\n  return x.val()*c.first + c.second;\r\n}\r\nP composition(P\
    \ l, P r) {\r\n  return {l.first*r.first, l.second*r.first + r.second};\r\n}\r\
    \nP Id() { return {1,0}; }\r\nstruct um {\r\n  P x;\r\n  um() : x(Id()) {}\r\n\
    \  um(P x) : x(x) {}\r\n  um& operator*=(const um& rhs) {\r\n    x = composition(x,\
    \ rhs.x);\r\n    return *this; \r\n  }\r\n  sm act(const sm& s) const { \r\n \
    \   return mapping(x, s);\r\n  }\r\n};\r\n\r\nint main() {\r\n  int n,q; cin>>n>>q;\r\
    \n  vector<int> A(n);\r\n  for (int i = 0; i < n; i++) {\r\n    cin>>A[i];\r\n\
    \  }\r\n  auto id = Compressor<int>::compress(A.begin(), A.end());\r\n  auto k\
    \ = id.size();\r\n  MoSolver mo;\r\n  for (int i = 0; i < q; i++) {\r\n    int\
    \ l,r; cin>>l>>r;\r\n    mo.add_segment(l, r);\r\n  }\r\n\r\n  LazySegmentTree<sm,\
    \ um> D(k);\r\n  vector<lint> ans(q, -1);\r\n  lint inv_sum = 0;\r\n  stack<int>\
    \ hist;\r\n  lint saved_inv_sum = 0;\r\n  int size = 0;\r\n  auto _pushl = [&](int\
    \ i) {\r\n    int vi = id[A[i]];\r\n    auto count = D.query(0, vi).val();\r\n\
    \    assert(count <= size);\r\n    inv_sum += count;\r\n    hist.push(vi); \r\n\
    \    D.update(vi, make_pair(1,1));\r\n    size++;   \r\n  };\r\n  auto _pushr\
    \ = [&](int i) {\r\n    int vi = id[A[i]];\r\n    auto count = D.query(vi+1, k).val();\r\
    \n    assert(count <= size);\r\n    inv_sum += count;\r\n    hist.push(vi); \r\
    \n    D.update(vi, make_pair(1,1));   \r\n    size++;\r\n  }; \r\n  // auto _popl\
    \ = [&](int i) {\r\n  //   int vi = id[A[i]];\r\n  //   inv_sum -= D.query(0,\
    \ vi).val();\r\n  //   D.update(vi, make_pair(1,-1));\r\n  // };\r\n  // auto\
    \ _popr = [&](int i) {\r\n  //   int vi = id[A[i]];\r\n  //   inv_sum -= D.query(vi+1,\
    \ k).val();\r\n  //   D.update(vi, make_pair(1,-1));\r\n  // };\r\n  auto rem\
    \ = [&](int t) {\r\n    ans[t] = inv_sum;   \r\n  }; \r\n  // mo.solve(_pushl,\
    \ _pushr, _popl, _popr, rem);\r\n\r\n  auto init = [&]() {\r\n    D.update(0,\
    \ k, make_pair(0,0));\r\n    inv_sum = 0;\r\n    size = 0;\r\n  };\r\n  auto snapshot\
    \ = [&]() {\r\n    hist = {};\r\n    saved_inv_sum = inv_sum;\r\n  };\r\n  auto\
    \ rollback = [&]() { \r\n    size -= hist.size();\r\n    while (!hist.empty())\
    \ {\r\n      int vi = hist.top(); hist.pop();\r\n      D.update(vi, make_pair(1,-1));\r\
    \n    }\r\n    inv_sum = saved_inv_sum;\r\n  }; \r\n  mo.solve_rollback(_pushl,\
    \ _pushr, rem, init, snapshot, rollback);\r\n\r\n  for (lint v : ans) cout <<\
    \ v << endl;\r\n}\r\n"
  dependsOn:
  - include/mtl/compress_int.hpp
  - include/mtl/mo.hpp
  - include/mtl/lazy_segment_tree.hpp
  - include/mtl/bit_manip.hpp
  - include/mtl/monoid.hpp
  isVerificationFile: true
  path: test/yosupo/static_range_inversions_query-mo_rollback.test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: test/yosupo/static_range_inversions_query-mo_rollback.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/static_range_inversions_query-mo_rollback.test.cpp
- /verify/test/yosupo/static_range_inversions_query-mo_rollback.test.cpp.html
title: test/yosupo/static_range_inversions_query-mo_rollback.test.cpp
---
