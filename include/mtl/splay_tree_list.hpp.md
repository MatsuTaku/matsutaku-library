---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: include/mtl/monoid.hpp
    title: include/mtl/monoid.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/splay_tree.hpp
    title: include/mtl/splay_tree.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp
    title: test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/splay_tree.hpp\"\n#include <memory>\n#include\
    \ <cassert>\n\ntemplate<class NodeType>\nstruct SplayTreeNodeBase {\n    using\
    \ node_type = NodeType;\n    using node_shared = std::shared_ptr<node_type>;\n\
    \    using node_weak = std::weak_ptr<node_type>;\n    node_shared l,r;\n    node_weak\
    \ p;\n    bool rev;\n    bool is_root() const {\n        return p.expired() ||\
    \ (p.lock()->l.get() != this && p.lock()->r.get() != this); \n    }\n};\ntemplate<class\
    \ T>\nstruct SplayTreeNodeTraits {\n    using node_type = typename T::node_type;\n\
    \    using node_shared = typename T::node_shared;\n    using node_weak = typename\
    \ T::node_weak;\n};\n\ntemplate<class Node>\nstruct SplayTreeBase {\n    using\
    \ node_traits = SplayTreeNodeTraits<Node>;\n    using node_type = typename node_traits::node_type;\n\
    \    using node_shared = typename node_traits::node_shared;\n    using node_weak\
    \ = typename node_traits::node_weak;\n    SplayTreeBase() = default;\n\n    void\
    \ rotate_left(const node_shared& u) const {\n        auto p = u->p.lock(), q =\
    \ p->p.lock();\n        p->r = u->l;\n        if (p->r)\n            p->r->p =\
    \ p;\n        u->l = p;\n        p->p = u;\n        u->p = q;\n        if (q)\
    \ {\n            if (q->l == p)\n                q->l = u;\n            else if\
    \ (q->r == p)\n                q->r = u;\n        }\n    }\n    void rotate_right(const\
    \ node_shared& u) const {\n        auto p = u->p.lock(), q = p->p.lock();\n  \
    \      p->l = u->r;\n        if (p->l)\n            p->l->p = p;\n        u->r\
    \ = p;\n        p->p = u;\n        u->p = q;\n        if (q) {\n            if\
    \ (q->l == p)\n                q->l = u;\n            else if (q->r == p)\n  \
    \              q->r = u;\n        }\n    }\n    virtual void reverse_prod(const\
    \ node_shared& u) const {}\n    virtual void propagate(const node_shared& u) const\
    \ {}\n    virtual void aggregate(const node_shared& u) const {}\n    void splay(const\
    \ node_shared& u) const {\n        if (u->is_root()) {\n            this->propagate(u);\n\
    \            this->aggregate(u);\n            return;\n        }\n        do {\n\
    \            assert(u);\n            auto p = u->p.lock();\n            assert(p);\n\
    \            if (p->is_root()) {\n                this->propagate(p);\n      \
    \          this->propagate(u);\n                if (p->l == u)\n             \
    \       rotate_right(u);\n                else if (p->r == u)\n              \
    \      rotate_left(u);\n                else throw \"invalid tree\";\n       \
    \         this->aggregate(p);\n                this->aggregate(u);\n         \
    \   } else {\n                auto q = p->p.lock();\n                this->propagate(q);\n\
    \                this->propagate(p);\n                this->propagate(u);\n  \
    \              if (q->l == p) {\n                    if (p->l == u) { // zig-zig\n\
    \                        rotate_right(p);\n                        rotate_right(u);\n\
    \                        this->aggregate(q);\n                        this->aggregate(p);\n\
    \                    } else if (p->r == u) { // zig-zag\n                    \
    \    rotate_left(u);\n                        rotate_right(u);\n             \
    \           this->aggregate(p);\n                        this->aggregate(q);\n\
    \                    } else throw \"invalid tree\";\n                } else if\
    \ (q->r == p) { \n                    if (p->r == u) { // zig-zig\n          \
    \              rotate_left(p);\n                        rotate_left(u);\n    \
    \                    this->aggregate(q);\n                        this->aggregate(p);\n\
    \                    } else if (p->l == u) { // zig-zag\n                    \
    \    rotate_right(u);\n                        rotate_left(u);\n             \
    \           this->aggregate(p);\n                        this->aggregate(q);\n\
    \                    } else throw \"invalid tree\";\n                }\n     \
    \           this->aggregate(u);\n            }\n        } while (!u->is_root());\n\
    \    }\n\n};\n#line 2 \"include/mtl/monoid.hpp\"\n#if __cpp_concepts >= 202002L\n\
    #include <concepts>\n#endif\n\ntemplate<class T, T (*op)(T, T), T (*e)()>\nstruct\
    \ Monoid {\n  T x;\n  Monoid() : x(e()) {}\n  template<class... Args>\n  Monoid(Args&&...\
    \ args) : x(std::forward<Args>(args)...) {}\n  Monoid operator*(const Monoid&\
    \ rhs) const {\n    return Monoid(op(x, rhs.x));\n  }\n};\n\nstruct VoidMonoid\
    \ {\n  VoidMonoid() {}\n  VoidMonoid operator*(const VoidMonoid& rhs) const {\n\
    \    return VoidMonoid();\n  }\n};\n\n#if __cpp_concepts >= 202002L\ntemplate<class\
    \ T>\nconcept IsMonoid = requires (T m) {\n  { m * m } -> std::same_as<T>;\n};\n\
    #endif\n\ntemplate<class T, T (*op)(T, T), T (*e)()>\nstruct CommutativeMonoid\
    \ : public Monoid<T, op, e> {\n    using Base = Monoid<T, op, e>;\n    CommutativeMonoid(T\
    \ x=e()) : Base(x) {}\n    CommutativeMonoid operator+(const CommutativeMonoid&\
    \ rhs) const {\n        return CommutativeMonoid(*this * rhs);\n    }\n};\n\n\
    #if __cpp_concepts >= 202002L\ntemplate<class T>\nconcept IsCommutativeMonoid\
    \ = requires (T m) {\n  { m + m } -> std::same_as<T>;\n};\n#endif\n\ntemplate<class\
    \ S, class F, S (*mapping)(F, S), S (*composition)(F, S), F (*id)()>\nstruct OperatorMonoid\
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
    \ 4 \"include/mtl/splay_tree_list.hpp\"\n\ntemplate<class M, class O>\nstruct\
    \ SplayTreeListNode : SplayTreeNodeBase<SplayTreeListNode<M,O>> {\n    size_t\
    \ sum;\n    M m, prod, rprod;\n    O f;\n    using Base = SplayTreeNodeBase<SplayTreeListNode<M,O>>;\n\
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
    \    u->f *= f;\n        base::splay(u);\n        root = u;\n    }\n};\n"
  code: "#pragma once\n#include \"splay_tree.hpp\"\n#include \"monoid.hpp\"\n\ntemplate<class\
    \ M, class O>\nstruct SplayTreeListNode : SplayTreeNodeBase<SplayTreeListNode<M,O>>\
    \ {\n    size_t sum;\n    M m, prod, rprod;\n    O f;\n    using Base = SplayTreeNodeBase<SplayTreeListNode<M,O>>;\n\
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
    \    u->f *= f;\n        base::splay(u);\n        root = u;\n    }\n};"
  dependsOn:
  - include/mtl/splay_tree.hpp
  - include/mtl/monoid.hpp
  isVerificationFile: false
  path: include/mtl/splay_tree_list.hpp
  requiredBy: []
  timestamp: '2023-04-20 09:14:32+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp
documentation_of: include/mtl/splay_tree_list.hpp
layout: document
redirect_from:
- /library/include/mtl/splay_tree_list.hpp
- /library/include/mtl/splay_tree_list.hpp.html
title: include/mtl/splay_tree_list.hpp
---
