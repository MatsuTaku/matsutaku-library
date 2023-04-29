---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/bit_manip.hpp
    title: include/mtl/bit_manip.hpp
  - icon: ':question:'
    path: include/mtl/modular.hpp
    title: include/mtl/modular.hpp
  - icon: ':question:'
    path: include/mtl/monoid.hpp
    title: include/mtl/monoid.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/splay_tree.hpp
    title: include/mtl/splay_tree.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/splay_tree_list.hpp
    title: include/mtl/splay_tree_list.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum
    links:
    - https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum
  bundledCode: "#line 1 \"test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp\"\
    \n#define PROBLEM \"https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum\"\
    \n#line 2 \"include/mtl/splay_tree.hpp\"\n#include <memory>\n#include <cassert>\n\
    \ntemplate<class NodeType>\nstruct SplayTreeNodeBase {\n    using node_type =\
    \ NodeType;\n    using node_shared = std::shared_ptr<node_type>;\n    using node_weak\
    \ = std::weak_ptr<node_type>;\n    node_shared l,r;\n    node_weak p;\n    bool\
    \ rev;\n    bool is_root() const {\n        return p.expired() || (p.lock()->l.get()\
    \ != this && p.lock()->r.get() != this); \n    }\n};\ntemplate<class T>\nstruct\
    \ SplayTreeNodeTraits {\n    using node_type = typename T::node_type;\n    using\
    \ node_shared = typename T::node_shared;\n    using node_weak = typename T::node_weak;\n\
    };\n\ntemplate<class Node>\nstruct SplayTreeBase {\n    using node_traits = SplayTreeNodeTraits<Node>;\n\
    \    using node_type = typename node_traits::node_type;\n    using node_shared\
    \ = typename node_traits::node_shared;\n    using node_weak = typename node_traits::node_weak;\n\
    \    SplayTreeBase() = default;\n\n    void rotate_left(const node_shared& u)\
    \ const {\n        auto p = u->p.lock(), q = p->p.lock();\n        p->r = u->l;\n\
    \        if (p->r)\n            p->r->p = p;\n        u->l = p;\n        p->p\
    \ = u;\n        u->p = q;\n        if (q) {\n            if (q->l == p)\n    \
    \            q->l = u;\n            else if (q->r == p)\n                q->r\
    \ = u;\n        }\n    }\n    void rotate_right(const node_shared& u) const {\n\
    \        auto p = u->p.lock(), q = p->p.lock();\n        p->l = u->r;\n      \
    \  if (p->l)\n            p->l->p = p;\n        u->r = p;\n        p->p = u;\n\
    \        u->p = q;\n        if (q) {\n            if (q->l == p)\n           \
    \     q->l = u;\n            else if (q->r == p)\n                q->r = u;\n\
    \        }\n    }\n    virtual void reverse_prod(const node_shared& u) const {}\n\
    \    virtual void propagate(const node_shared& u) const {}\n    virtual void aggregate(const\
    \ node_shared& u) const {}\n    void splay(const node_shared& u) const {\n   \
    \     if (u->is_root()) {\n            this->propagate(u);\n            this->aggregate(u);\n\
    \            return;\n        }\n        do {\n            assert(u);\n      \
    \      auto p = u->p.lock();\n            assert(p);\n            if (p->is_root())\
    \ {\n                this->propagate(p);\n                this->propagate(u);\n\
    \                if (p->l == u)\n                    rotate_right(u);\n      \
    \          else if (p->r == u)\n                    rotate_left(u);\n        \
    \        else throw \"invalid tree\";\n                this->aggregate(p);\n \
    \               this->aggregate(u);\n            } else {\n                auto\
    \ q = p->p.lock();\n                this->propagate(q);\n                this->propagate(p);\n\
    \                this->propagate(u);\n                if (q->l == p) {\n     \
    \               if (p->l == u) { // zig-zig\n                        rotate_right(p);\n\
    \                        rotate_right(u);\n                        this->aggregate(q);\n\
    \                        this->aggregate(p);\n                    } else if (p->r\
    \ == u) { // zig-zag\n                        rotate_left(u);\n              \
    \          rotate_right(u);\n                        this->aggregate(p);\n   \
    \                     this->aggregate(q);\n                    } else throw \"\
    invalid tree\";\n                } else if (q->r == p) { \n                  \
    \  if (p->r == u) { // zig-zig\n                        rotate_left(p);\n    \
    \                    rotate_left(u);\n                        this->aggregate(q);\n\
    \                        this->aggregate(p);\n                    } else if (p->l\
    \ == u) { // zig-zag\n                        rotate_right(u);\n             \
    \           rotate_left(u);\n                        this->aggregate(p);\n   \
    \                     this->aggregate(q);\n                    } else throw \"\
    invalid tree\";\n                }\n                this->aggregate(u);\n    \
    \        }\n        } while (!u->is_root());\n    }\n\n};\n#line 2 \"include/mtl/monoid.hpp\"\
    \n#if __cpp_concepts >= 202002L\n#include <concepts>\n#endif\n\ntemplate<class\
    \ T, T (*op)(T, T), T (*e)()>\nstruct Monoid {\n  T x;\n  Monoid() : x(e()) {}\n\
    \  template<class... Args>\n  Monoid(Args&&... args) : x(std::forward<Args>(args)...)\
    \ {}\n  Monoid operator*(const Monoid& rhs) const {\n    return Monoid(op(x, rhs.x));\n\
    \  }\n};\n\nstruct VoidMonoid {\n  VoidMonoid() {}\n  VoidMonoid operator*(const\
    \ VoidMonoid& rhs) const {\n    return VoidMonoid();\n  }\n};\n\n#if __cpp_concepts\
    \ >= 202002L\ntemplate<class T>\nconcept IsMonoid = requires (T m) {\n  { m *\
    \ m } -> std::same_as<T>;\n};\n#endif\n\ntemplate<class T, T (*op)(T, T), T (*e)()>\n\
    struct CommutativeMonoid : public Monoid<T, op, e> {\n    using Base = Monoid<T,\
    \ op, e>;\n    CommutativeMonoid(T x=e()) : Base(x) {}\n    CommutativeMonoid\
    \ operator+(const CommutativeMonoid& rhs) const {\n        return CommutativeMonoid(*this\
    \ * rhs);\n    }\n};\n\n#if __cpp_concepts >= 202002L\ntemplate<class T>\nconcept\
    \ IsCommutativeMonoid = requires (T m) {\n  { m + m } -> std::same_as<T>;\n};\n\
    #endif\n\ntemplate<class S, class F, S (*mapping)(F, S), S (*composition)(F, S),\
    \ F (*id)()>\nstruct OperatorMonoid {\n    F f;\n    OperatorMonoid() : f(id())\
    \ {}\n    template<class... Args>\n    OperatorMonoid(Args&&... args) : f(std::forward<Args>(args)...)\
    \ {}\n    OperatorMonoid& operator*=(const OperatorMonoid& rhs) {\n        f =\
    \ composition(rhs.f, f);\n        return *this;\n    }\n    S act(const S& s)\
    \ const {\n        return mapping(f, s);\n    }\n};\n\nstruct VoidOperatorMonoid\
    \ {\n    VoidOperatorMonoid() {}\n    VoidOperatorMonoid& operator*=(const VoidOperatorMonoid&\
    \ rhs) {\n        return *this;\n    }\n    template<class T>\n    T act(const\
    \ T& s) const {\n        return s;\n    }\n};\n\n#if __cpp_concepts >= 202002L\n\
    template<class F, class S>\nconcept IsOperatorMonoid = requires (F f, S s) {\n\
    \    { f *= f } -> std::same_as<F&>;\n    { f.act(s) } -> std::same_as<S>;\n};\n\
    #endif\n#line 4 \"include/mtl/splay_tree_list.hpp\"\n\ntemplate<class M, class\
    \ O>\nstruct SplayTreeListNode : SplayTreeNodeBase<SplayTreeListNode<M,O>> {\n\
    \    size_t sum;\n    M m, prod, rprod;\n    O f;\n    using Base = SplayTreeNodeBase<SplayTreeListNode<M,O>>;\n\
    \    template<class... Args>\n    SplayTreeListNode(Args&&... args) \n       \
    \ : Base(), sum(1), m(std::forward<Args>(args)...), prod(m), rprod(m), f() {}\n\
    };\ntemplate<class M, class O=VoidOperatorMonoid>\n#if __cpp_concepts >= 202002L\n\
    requires IsMonoid<M> && IsOperatorMonoid<O, M>\n#endif\nstruct SplayTreeList :\
    \ SplayTreeBase<SplayTreeListNode<M, O>> {\n    using node_type = SplayTreeListNode<M,\
    \ O>;\n    using base = SplayTreeBase<node_type>;\n    using monoid_type = M;\n\
    \    using operator_monoid_type = O;\n    using node_shared = typename SplayTreeNodeTraits<node_type>::node_shared;\n\
    \    node_shared root;\n\n    SplayTreeList() : base(), root(nullptr) {}\n   \
    \ template<class InputIt>\n    node_shared _dfs_init(InputIt first, InputIt last)\
    \ {\n        if (first == last) return nullptr;\n        auto n = std::distance(first,\
    \ last);\n        auto mid = std::next(first, n / 2);\n        auto u = std::make_shared<node_type>(*mid);\n\
    \        u->l = _dfs_init(first, mid);\n        if (u->l) u->l->p = u;\n     \
    \   u->r = _dfs_init(std::next(mid), last);\n        if (u->r) u->r->p = u;\n\
    \        this->aggregate(u);\n        return u;\n    }\n    template<class InputIt>\n\
    \    SplayTreeList(InputIt first, InputIt last) : SplayTreeList() {\n        if\
    \ (first == last) return;\n        using iterator_category = typename std::iterator_traits<InputIt>::iterator_category;\n\
    \        if constexpr (std::is_base_of_v<iterator_category, std::random_access_iterator_tag>)\
    \ {\n            root = _dfs_init(first, last);\n        } else {\n          \
    \  auto it = first;\n            root = std::make_shared<node_type>(*it);\n  \
    \          ++it;\n            for (; it != last; ++it) {\n                auto\
    \ u = std::make_shared<node_type>(*it);\n                u->l = root;\n      \
    \          root->p = u;\n                root = u;\n                this->aggregate(u);\n\
    \            }\n        }\n    }\n    void reverse_prod(const node_shared& u)\
    \ const override {\n        std::swap(u->prod, u->rprod);\n    }\n    void propagate(const\
    \ node_shared& u) const override {\n        if (u->l) {\n            u->l->m =\
    \ u->f.act(u->l->m);\n            u->l->prod = u->f.act(u->l->prod);\n       \
    \     u->l->rprod = u->f.act(u->l->rprod);\n            u->l->f *= u->f;\n   \
    \     }\n        if (u->r) {\n            u->r->m = u->f.act(u->r->m);\n     \
    \       u->r->prod = u->f.act(u->r->prod);\n            u->r->rprod = u->f.act(u->r->rprod);\n\
    \            u->r->f *= u->f;\n        }\n        if (u->rev) {\n            std::swap(u->l,\
    \ u->r);\n            if (u->l) {\n                u->l->rev ^= true;\n      \
    \          reverse_prod(u->l);\n            }\n            if (u->r) {\n     \
    \           u->r->rev ^= true;\n                reverse_prod(u->r);\n        \
    \    }\n            u->rev = false;\n        }\n        u->f = operator_monoid_type();\n\
    \    }\n    void aggregate(const node_shared& u) const override {\n        u->sum\
    \ = 1;\n        u->prod = u->m;\n        u->rprod = u->m;\n        if (u->l) {\n\
    \            u->sum += u->l->sum;\n            u->prod = u->l->prod * u->prod;\n\
    \            u->rprod = u->rprod * u->l->rprod;\n        }\n        if (u->r)\
    \ {\n            u->sum += u->r->sum;\n            u->prod = u->prod * u->r->prod;\n\
    \            u->rprod = u->r->rprod * u->rprod;\n        }\n    }\n    node_shared\
    \ kth_element(size_t k) {\n        assert(k < root->sum);\n        auto u = root;\n\
    \        while (true) {\n            assert(u);\n            auto lp = u->l ?\
    \ u->l->sum : 0;\n            auto rp = u->r ? u->r->sum : 0;\n            //\
    \ cerr<<u->m.first<<' '<<u->sum<<' '<<lp<<' '<<rp<<endl;\n            assert(u->sum\
    \ == lp+rp+1);\n            propagate(u);\n            if (!u->l) {\n        \
    \        if (k == 0)\n                    break;\n                k--;\n     \
    \           u = u->r;\n            } else if (u->l->sum == k) {\n            \
    \    break;\n            } else if (u->l->sum > k) {\n                u = u->l;\n\
    \            } else {\n                k -= u->l->sum + 1;\n                u\
    \ = u->r;\n            }\n        }\n        assert(u);\n        base::splay(u);\n\
    \        root = u;\n        return u;\n    }\n    template<class... Args>\n  \
    \  void insert(size_t i, Args&&... args) {\n        auto u = std::make_shared<node_type>(std::forward<Args>(args)...);\n\
    \        if (i == 0) {\n            u->r = root;\n            if (root) root->p\
    \ = u;\n            root = u;\n            aggregate(u);\n            return;\n\
    \        }\n        if (i == root->sum) {\n            u->l = root;\n        \
    \    if (root) root->p = u;\n            root = u;\n            aggregate(u);\n\
    \            return;\n        }\n        auto p = kth_element(i);\n        u->l\
    \ = p->l;\n        u->r = p;\n        if (u->l)\n            u->l->p = u;\n  \
    \      u->r->p = u;\n        p->l = nullptr;\n        root = u;\n        aggregate(p);\n\
    \        aggregate(u);\n    }\n    void erase(size_t i) {\n        assert(i <\
    \ root->sum);\n        auto p = kth_element(i);\n        if (i == 0) {\n     \
    \       root = p->r;\n            if (root)\n                root->p.reset();\n\
    \            return;\n        }\n        if (i == root->sum-1) {\n           \
    \ root = p->l;\n            if (root)\n                root->p.reset();\n    \
    \        return;\n        }\n        auto r = p->r;\n        auto l = p->l;\n\
    \        root = r;\n        root->p.reset();\n        r = kth_element(0);\n  \
    \      r->l = l;\n        l->p = r;\n        aggregate(r);\n        // p has no\
    \ referry so p is deleted outomatically.\n    }\n    node_shared between(size_t\
    \ l, size_t r) {\n        assert(r <= root->sum);\n        if (l == 0) {\n   \
    \         if (r == root->sum)\n                return root;\n            else\n\
    \                return kth_element(r)->l;\n        }\n        if (r == root->sum)\
    \ {\n            return kth_element(l-1)->r;\n        }\n        auto rp = kth_element(r);\n\
    \        root = rp->l;\n        root->p.reset();\n        auto lp = kth_element(l-1);\n\
    \        rp->l = lp;\n        lp->p = rp;\n        root = rp;\n        aggregate(rp);\n\
    \        return lp->r;\n    }\n    monoid_type prod(size_t l, size_t r) {\n  \
    \      return between(l, r)->prod;\n    }\n    void reverse(size_t l, size_t r)\
    \ {\n        auto u = between(l, r);\n        u->rev ^= true;\n        reverse_prod(u);\n\
    \        base::splay(u);\n        root = u;\n    }\n    template<class... Args>\n\
    \    void set(size_t i, Args&&... args) {\n        auto u = kth_element(i);\n\
    \        u->m = monoid_type(std::forward<Args>(args)...);\n        this->aggregate(u);\n\
    \    }\n    void update(size_t i, const operator_monoid_type& f) {\n        auto\
    \ u = kth_element(i);\n        u->m = f.act(u->m);\n        this->aggregate(u);\n\
    \    }\n    void update(size_t l, size_t r, const operator_monoid_type& f) {\n\
    \        assert(l < r);\n        auto u = between(l, r);\n        u->m = f.act(u->m);\n\
    \        u->prod = f.act(u->prod);\n        u->rprod = f.act(u->rprod);\n    \
    \    u->f *= f;\n        base::splay(u);\n        root = u;\n    }\n};\n#line\
    \ 2 \"include/mtl/bit_manip.hpp\"\n#include <cstdint>\n#line 4 \"include/mtl/bit_manip.hpp\"\
    \n\nnamespace bm {\n\ninline constexpr uint64_t popcnt_e8(uint64_t x) {\n  x =\
    \ (x & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);\n  x = (x & 0x3333333333333333)\
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
    \ 3 \"include/mtl/modular.hpp\"\n#include <iostream>\n#line 5 \"include/mtl/modular.hpp\"\
    \n\ntemplate <int MOD>\nclass Modular {\n private:\n  unsigned int val_;\n\n public:\n\
    \  static constexpr unsigned int mod() { return MOD; }\n  template<class T>\n\
    \  static constexpr unsigned int safe_mod(T v) {\n    auto x = (long long)(v%(long\
    \ long)mod());\n    if (x < 0) x += mod();\n    return (unsigned int) x;\n  }\n\
    \n  constexpr Modular() : val_(0) {}\n  template<class T,\n      std::enable_if_t<\n\
    \          std::is_integral<T>::value && std::is_unsigned<T>::value\n      > *\
    \ = nullptr>\n  constexpr Modular(T v) : val_(v%mod()) {}\n  template<class T,\n\
    \      std::enable_if_t<\n          std::is_integral<T>::value && !std::is_unsigned<T>::value\n\
    \      > * = nullptr>\n  constexpr Modular(T v) : val_(safe_mod(v)) {}\n\n  constexpr\
    \ unsigned int val() const { return val_; }\n  constexpr Modular& operator+=(Modular\
    \ x) {\n    val_ += x.val();\n    if (val_ >= mod()) val_ -= mod();\n    return\
    \ *this;\n  }\n  constexpr Modular operator-() const { return {mod() - val_};\
    \ }\n  constexpr Modular& operator-=(Modular x) {\n    val_ += mod() - x.val();\n\
    \    if (val_ >= mod()) val_ -= mod();\n    return *this;\n  }\n  constexpr Modular&\
    \ operator*=(Modular x) {\n    auto v = (long long) val_ * x.val();\n    if (v\
    \ >= mod()) v %= mod();\n    val_ = v;\n    return *this;\n  }\n  constexpr Modular\
    \ pow(long long p) const {\n    assert(p >= 0);\n    Modular t = 1;\n    Modular\
    \ u = *this;\n    while (p) {\n      if (p & 1)\n        t *= u;\n      u *= u;\n\
    \      p >>= 1;\n    }\n    return t;\n  }\n  friend constexpr Modular pow(Modular\
    \ x, long long p) {\n    return x.pow(p);\n  }\n  constexpr Modular inv() const\
    \ { return pow(mod()-2); }\n  constexpr Modular& operator/=(Modular x) { return\
    \ *this *= x.inv(); }\n  constexpr Modular operator+(Modular x) const { return\
    \ Modular(*this) += x; }\n  constexpr Modular operator-(Modular x) const { return\
    \ Modular(*this) -= x; }\n  constexpr Modular operator*(Modular x) const { return\
    \ Modular(*this) *= x; }\n  constexpr Modular operator/(Modular x) const { return\
    \ Modular(*this) /= x; }\n  constexpr Modular& operator++() { return *this +=\
    \ 1; }\n  constexpr Modular operator++(int) { Modular c = *this; ++(*this); return\
    \ c; }\n  constexpr Modular& operator--() { return *this -= 1; }\n  constexpr\
    \ Modular operator--(int) { Modular c = *this; --(*this); return c; }\n\n  constexpr\
    \ bool operator==(Modular x) const { return val() == x.val(); }\n  constexpr bool\
    \ operator!=(Modular x) const { return val() != x.val(); }\n\n  constexpr bool\
    \ is_square() const {\n    return pow((mod()-1)/2) == 1;\n  }\n  /**\n   * Return\
    \ x s.t. x * x = a mod p\n   * reference: https://zenn.dev/peria/articles/c6afc72b6b003c\n\
    \  */\n  constexpr Modular sqrt() const {\n    if (!is_square()) \n      throw\
    \ std::runtime_error(\"not square\");\n    auto mod_eight = mod() % 8;\n    if\
    \ (mod_eight == 3 || mod_eight == 7) {\n      return pow((mod()+1)/4);\n    }\
    \ else if (mod_eight == 5) {\n      auto x = pow((mod()+3)/8);\n      if (x *\
    \ x != *this)\n        x *= Modular(2).pow((mod()-1)/4);\n      return x;\n  \
    \  } else {\n      Modular d = 2;\n      while (d.is_square())\n        d += 1;\n\
    \      auto t = mod()-1;\n      int s = bm::ctz(t);\n      t >>= s;\n      auto\
    \ a = pow(t);\n      auto D = d.pow(t);\n      int m = 0;\n      Modular dt =\
    \ 1;\n      Modular du = D;\n      for (int i = 0; i < s; i++) {\n        if ((a*dt).pow(1u<<(s-1-i))\
    \ == -1) {\n          m |= 1u << i;\n          dt *= du;\n        }\n        du\
    \ *= du;\n      }\n      return pow((t+1)/2) * D.pow(m/2);\n    }\n  }\n\n  friend\
    \ std::ostream& operator<<(std::ostream& os, const Modular& x) {\n    return os\
    \ << x.val();\n  }\n  friend std::istream& operator>>(std::istream& is, Modular&\
    \ x) {\n    return is >> x.val_;\n  }\n\n};\n\nusing Modular998244353 = Modular<998244353>;\n\
    using Modular1000000007 = Modular<(int)1e9+7>;\n\ntemplate<int Id=0>\nclass DynamicModular\
    \ {\n private:\n  static unsigned int mod_;\n  unsigned int val_;\n\n public:\n\
    \  static unsigned int mod() { return mod_; }\n  static void set_mod(unsigned\
    \ int m) { mod_ = m; }\n  template<class T>\n  static constexpr unsigned int safe_mod(T\
    \ v) {\n    auto x = (long long)(v%(long long)mod());\n    if (x < 0) x += mod();\n\
    \    return (unsigned int) x;\n  }\n\n  constexpr DynamicModular() : val_(0) {}\n\
    \  template<class T,\n      std::enable_if_t<\n          std::is_integral<T>::value\
    \ && std::is_unsigned<T>::value\n      > * = nullptr>\n  constexpr DynamicModular(T\
    \ v) : val_(v%mod()) {}\n  template<class T,\n      std::enable_if_t<\n      \
    \    std::is_integral<T>::value && !std::is_unsigned<T>::value\n      > * = nullptr>\n\
    \  constexpr DynamicModular(T v) : val_(safe_mod(v)) {}\n\n  constexpr unsigned\
    \ int val() const { return val_; }\n  constexpr DynamicModular& operator+=(DynamicModular\
    \ x) {\n    val_ += x.val();\n    if (val_ >= mod()) val_ -= mod();\n    return\
    \ *this;\n  }\n  constexpr DynamicModular operator-() const { return {mod() -\
    \ val_}; }\n  constexpr DynamicModular& operator-=(DynamicModular x) {\n    val_\
    \ += mod() - x.val();\n    if (val_ >= mod()) val_ -= mod();\n    return *this;\n\
    \  }\n  constexpr DynamicModular& operator*=(DynamicModular x) {\n    auto v =\
    \ (long long) val_ * x.val();\n    if (v >= mod()) v %= mod();\n    val_ = v;\n\
    \    return *this;\n  }\n  constexpr DynamicModular pow(long long p) const {\n\
    \    assert(p >= 0);\n    DynamicModular t = 1;\n    DynamicModular u = *this;\n\
    \    while (p) {\n      if (p & 1)\n        t *= u;\n      u *= u;\n      p >>=\
    \ 1;\n    }\n    return t;\n  }\n  friend constexpr DynamicModular pow(DynamicModular\
    \ x, long long p) {\n    return x.pow(p);\n  }\n  // TODO: implement when mod\
    \ is not prime\n  constexpr DynamicModular inv() const { return pow(mod()-2);\
    \ }\n  constexpr DynamicModular& operator/=(DynamicModular x) { return *this *=\
    \ x.inv(); }\n  constexpr DynamicModular operator+(DynamicModular x) const { return\
    \ DynamicModular(*this) += x; }\n  constexpr DynamicModular operator-(DynamicModular\
    \ x) const { return DynamicModular(*this) -= x; }\n  constexpr DynamicModular\
    \ operator*(DynamicModular x) const { return DynamicModular(*this) *= x; }\n \
    \ constexpr DynamicModular operator/(DynamicModular x) const { return DynamicModular(*this)\
    \ /= x; }\n  constexpr DynamicModular& operator++() { return *this += 1; }\n \
    \ constexpr DynamicModular operator++(int) { DynamicModular c = *this; ++(*this);\
    \ return c; }\n  constexpr DynamicModular& operator--() { return *this -= 1; }\n\
    \  constexpr DynamicModular operator--(int) { DynamicModular c = *this; --(*this);\
    \ return c; }\n\n  constexpr bool operator==(DynamicModular x) const { return\
    \ val() == x.val(); }\n  constexpr bool operator!=(DynamicModular x) const { return\
    \ val() != x.val(); }\n\n  constexpr bool is_square() const {\n    return val()\
    \ == 0 or pow((mod()-1)/2) == 1;\n  }\n  /**\n   * Return x s.t. x * x = a mod\
    \ p\n   * reference: https://zenn.dev/peria/articles/c6afc72b6b003c\n  */\n  constexpr\
    \ DynamicModular sqrt() const {\n    // assert mod is prime\n    if (!is_square())\
    \ \n      throw std::runtime_error(\"not square\");\n    if (val() < 2)\n    \
    \  return val();\n    auto mod_eight = mod() % 8;\n    if (mod_eight == 3 || mod_eight\
    \ == 7) {\n      return pow((mod()+1)/4);\n    } else if (mod_eight == 5) {\n\
    \      auto x = pow((mod()+3)/8);\n      if (x * x != *this)\n        x *= DynamicModular(2).pow((mod()-1)/4);\n\
    \      return x;\n    } else {\n      DynamicModular d = 2;\n      while (d.is_square())\n\
    \        ++d;\n      auto t = mod()-1;\n      int s = bm::ctz(t);\n      t >>=\
    \ s;\n      auto a = pow(t);\n      auto D = d.pow(t);\n      int m = 0;\n   \
    \   DynamicModular dt = 1;\n      DynamicModular du = D;\n      for (int i = 0;\
    \ i < s; i++) {\n        if ((a*dt).pow(1u<<(s-1-i)) == -1) {\n          m |=\
    \ 1u << i;\n          dt *= du;\n        }\n        du *= du;\n      }\n     \
    \ return pow((t+1)/2) * D.pow(m/2);\n    }\n  }\n\n  friend std::ostream& operator<<(std::ostream&\
    \ os, const DynamicModular& x) {\n    return os << x.val();\n  }\n  friend std::istream&\
    \ operator>>(std::istream& is, DynamicModular& x) {\n    return is >> x.val_;\n\
    \  }\n\n};\ntemplate<int Id>\nunsigned int DynamicModular<Id>::mod_;\n\n#include\
    \ <vector>\n\ntemplate<class ModInt>\nstruct ModularUtil {\n  static constexpr\
    \ int mod = ModInt::mod();\n  static struct inv_table {\n    std::vector<ModInt>\
    \ tb{0,1};\n    inv_table() : tb({0,1}) {}\n  } inv_;\n  void set_inv(int n) {\n\
    \    int m = inv_.tb.size();\n    if (m > n) return;\n    inv_.tb.resize(n+1);\n\
    \    for (int i = m; i < n+1; i++)\n      inv_.tb[i] = -inv_.tb[mod % i] * (mod\
    \ / i);\n  }\n  ModInt& inv(int i) {\n    set_inv(i);\n    return inv_.tb[i];\n\
    \  }\n};\ntemplate<class ModInt>\ntypename ModularUtil<ModInt>::inv_table ModularUtil<ModInt>::inv_;\n\
    \n#include <array>\n\nnamespace math {\n\nconstexpr int mod_pow_constexpr(int\
    \ x, int p, int m) {\n  int t = 1;\n  int u = x;\n  while (p) {\n    if (p & 1)\
    \ {\n      t *= u;\n      t %= m;\n    }\n    u *= u;\n    u %= m;\n    p >>=\
    \ 1;\n  }\n  return t;\n}\n\nconstexpr int primitive_root_constexpr(int m) {\n\
    \  if (m == 2) return 1;\n  if (m == 998244353) return 3;\n\n  std::array<int,\
    \ 20> divs{2};\n  int cnt = 1;\n  int x = (m-1) / 2;\n  for (int d = 3; d*d <=\
    \ x; d += 2) {\n    if (x % d == 0) {\n      divs[cnt++] = d;\n      while (x\
    \ % d == 0)\n        x /= d;\n    }\n  }\n  if (x > 1) divs[cnt++] = x;\n  for\
    \ (int g = 2; ; g++) {\n    bool ok = true;\n    for (int i = 0; i < cnt; i++)\
    \ {\n      if (mod_pow_constexpr(g, (m-1) / divs[cnt], m) == 1) {\n        ok\
    \ = false;\n        break;\n      }\n    }\n    if (ok) return g;\n  }\n}\n\n\
    template<int m>\nconstexpr int primitive_root = primitive_root_constexpr(m);\n\
    \n}\n#line 5 \"test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp\"\n\
    #include <bits/stdc++.h>\nusing namespace std;\n\nusing mint = Modular998244353;\n\
    struct Sum {\n    mint a;\n    size_t sz;\n    Sum(int a=0) : a(a), sz(1) {}\n\
    \    Sum(mint a, size_t sz) : a(a), sz(sz) {}\n    Sum operator*(const Sum& rhs)\
    \ const {\n        return Sum(a+rhs.a, sz+rhs.sz);\n    }\n};\nstruct Affine {\n\
    \    mint b,c;\n    Affine(mint b=1, mint c=0) : b(b), c(c) {}\n    Affine& operator*=(const\
    \ Affine& rhs) {\n        b *= rhs.b;\n        c = c*rhs.b + rhs.c;\n        return\
    \ *this;\n    }\n    Sum act(const Sum& a) const {\n        return {b * a.a +\
    \ c * a.sz, a.sz};\n    }\n};\nusing DynamicSequenceRangeAffineRangeSum = SplayTreeList<Sum,\
    \ Affine>;\n\nint main() {\n    int n,q; cin>>n>>q;\n    vector<int> A(n);\n \
    \   for (int i = 0; i < n; i++) cin>>A[i];\n    DynamicSequenceRangeAffineRangeSum\
    \ st(A.begin(), A.end());\n    for (int i = 0; i < q; i++) {\n        int t; cin>>t;\n\
    \        switch (t) {\n            case 0: {\n                int i,x; cin>>i>>x;\n\
    \                st.insert(i, x);\n            }; break;\n            case 1:\
    \ {\n                int i; cin>>i;\n                st.erase(i);\n          \
    \  } break;\n            case 2: {\n                int l,r; cin>>l>>r;\n    \
    \            st.reverse(l,r);\n            } break;\n            case 3: {\n \
    \               int l,r,b,c; cin>>l>>r>>b>>c;\n                st.update(l,r,{b,c});\n\
    \            } break;\n            case 4: {   \n                int l,r; cin>>l>>r;\n\
    \                cout << st.prod(l,r).a << endl;\n            } break;\n     \
    \       default: break;\n        }\n    }\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum\"\
    \n#include \"../../include/mtl/splay_tree_list.hpp\"\n#include \"../../include/mtl/modular.hpp\"\
    \n#include \"../../include/mtl/monoid.hpp\"\n#include <bits/stdc++.h>\nusing namespace\
    \ std;\n\nusing mint = Modular998244353;\nstruct Sum {\n    mint a;\n    size_t\
    \ sz;\n    Sum(int a=0) : a(a), sz(1) {}\n    Sum(mint a, size_t sz) : a(a), sz(sz)\
    \ {}\n    Sum operator*(const Sum& rhs) const {\n        return Sum(a+rhs.a, sz+rhs.sz);\n\
    \    }\n};\nstruct Affine {\n    mint b,c;\n    Affine(mint b=1, mint c=0) : b(b),\
    \ c(c) {}\n    Affine& operator*=(const Affine& rhs) {\n        b *= rhs.b;\n\
    \        c = c*rhs.b + rhs.c;\n        return *this;\n    }\n    Sum act(const\
    \ Sum& a) const {\n        return {b * a.a + c * a.sz, a.sz};\n    }\n};\nusing\
    \ DynamicSequenceRangeAffineRangeSum = SplayTreeList<Sum, Affine>;\n\nint main()\
    \ {\n    int n,q; cin>>n>>q;\n    vector<int> A(n);\n    for (int i = 0; i < n;\
    \ i++) cin>>A[i];\n    DynamicSequenceRangeAffineRangeSum st(A.begin(), A.end());\n\
    \    for (int i = 0; i < q; i++) {\n        int t; cin>>t;\n        switch (t)\
    \ {\n            case 0: {\n                int i,x; cin>>i>>x;\n            \
    \    st.insert(i, x);\n            }; break;\n            case 1: {\n        \
    \        int i; cin>>i;\n                st.erase(i);\n            } break;\n\
    \            case 2: {\n                int l,r; cin>>l>>r;\n                st.reverse(l,r);\n\
    \            } break;\n            case 3: {\n                int l,r,b,c; cin>>l>>r>>b>>c;\n\
    \                st.update(l,r,{b,c});\n            } break;\n            case\
    \ 4: {   \n                int l,r; cin>>l>>r;\n                cout << st.prod(l,r).a\
    \ << endl;\n            } break;\n            default: break;\n        }\n   \
    \ }\n}"
  dependsOn:
  - include/mtl/splay_tree_list.hpp
  - include/mtl/splay_tree.hpp
  - include/mtl/monoid.hpp
  - include/mtl/modular.hpp
  - include/mtl/bit_manip.hpp
  isVerificationFile: true
  path: test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp
  requiredBy: []
  timestamp: '2023-04-29 13:51:09+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp
- /verify/test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp.html
title: test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp
---