---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: include/mtl/link_cut_tree.hpp
    title: include/mtl/link_cut_tree.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/monoid.hpp
    title: include/mtl/monoid.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/splay_tree.hpp
    title: include/mtl/splay_tree.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum
    links:
    - https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum
  bundledCode: "#line 1 \"test/yosupo/dynamic_tree_vertex_add_path_sum.test.cpp\"\n\
    #define PROBLEM \"https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum\"\
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
    #endif\n#line 3 \"include/mtl/link_cut_tree.hpp\"\n#include <vector>\n#include\
    \ <iostream>\n\ntemplate<class NodeType>\nstruct LinkCutTreeBase : public SplayTreeBase<NodeType>\
    \ {\n    using Base = SplayTreeBase<NodeType>;\n    using node_traits = SplayTreeNodeTraits<NodeType>;\n\
    \    using node_shared = typename node_traits::node_shared;\n    void expose(const\
    \ node_shared& x) const {\n        node_shared r = nullptr;\n        for (node_shared\
    \ p = x; p; p = p->p.lock()) {\n            Base::splay(p);\n            p->r\
    \ = r;\n            r = p;\n            this->aggregate(p);\n        }\n     \
    \   Base::splay(x);\n    }\n    void evert(const node_shared& v) const {\n   \
    \     expose(v);\n        v->rev ^= true;\n        this->reverse_prod(v);\n  \
    \      this->propagate(v);\n    }\n    void cut(const node_shared& c) const {\n\
    \        expose(c);\n        auto l = c->l;\n        c->l = nullptr;\n       \
    \ l->p.reset();\n        this->aggregate(c);\n    }\n    void link(const node_shared&\
    \ c, const node_shared& p) const {\n        evert(c);\n        expose(p);\n  \
    \      p->r = c;\n        c->p = p;\n        this->aggregate(p);\n    }\n    void\
    \ print_tree(const node_shared& u) const {\n        if (!u) return;\n        if\
    \ (u->l and u->l->p.lock() == u) {\n            print_tree(u->l);\n        }\n\
    \        std::cerr<<u->m.x<<' ';\n        if (u->r and u->r->p.lock() == u) {\n\
    \            print_tree(u->r);\n        }\n    }\n};\n\ntemplate<class M, class\
    \ O>\nstruct LinkCutTreeNode : SplayTreeNodeBase<LinkCutTreeNode<M, O>> {\n  \
    \  M m, prod, rprod;\n    O f;\n    using Base = SplayTreeNodeBase<LinkCutTreeNode<M,\
    \ O>>;\n    LinkCutTreeNode() = default;\n    template<class... Args>\n    LinkCutTreeNode(Args&&...\
    \ args) \n        : Base(), m(std::forward<Args>(args)...), prod(m), rprod(m),\
    \ f() {}\n};\n\ntemplate<class M, class O=VoidOperatorMonoid>\n#if __cpp_concepts\
    \ >= 202002L\nrequires IsMonoid<M> && IsOperatorMonoid<O, M>\n#endif\nstruct LinkCutTree\
    \ : LinkCutTreeBase<LinkCutTreeNode<M, O>> {\n    using node_type = LinkCutTreeNode<M,\
    \ O>;\n    using Base = LinkCutTreeBase<LinkCutTreeNode<M, O>>;\n    using monoid_type\
    \ = M;\n    using operator_monoid_type = O;\n    using node_shared = typename\
    \ SplayTreeNodeTraits<LinkCutTreeNode<M, O>>::node_shared;\n    std::vector<node_shared>\
    \ nodes;\n    LinkCutTree(size_t n) : Base(), nodes(n) {\n        for (size_t\
    \ i = 0; i < n; ++i)\n            nodes[i] = std::make_shared<node_type>();\n\
    \    }\n    template<class InputIt>\n    LinkCutTree(InputIt first, InputIt last)\
    \ : Base(), nodes(std::distance(first, last)) {\n        size_t i = 0;\n     \
    \   for (auto it = first; it != last; ++it)\n            nodes[i++] = std::make_shared<node_type>(*it);\n\
    \    }\n    void reverse_prod(const node_shared& u) const override {\n       \
    \ std::swap(u->prod, u->rprod);\n    }\n    void propagate(const node_shared&\
    \ u) const override {\n        bool cl = u->l and u->l->p.lock() == u;\n     \
    \   bool cr = u->r and u->r->p.lock() == u;\n        if (cl) {\n            u->l->m\
    \ = u->f.act(u->l->m);\n            u->l->prod = u->f.act(u->l->prod);\n     \
    \       u->l->rprod = u->f.act(u->l->rprod);\n            u->l->f *= u->f;\n \
    \       }\n        if (cr) {\n            u->r->m = u->f.act(u->r->m);\n     \
    \       u->r->prod = u->f.act(u->r->prod);\n            u->r->rprod = u->f.act(u->r->rprod);\n\
    \            u->r->f *= u->f;\n        }\n        if (u->rev) {\n            std::swap(u->l,\
    \ u->r);\n            if (cr) {\n                u->l->rev ^= true;\n        \
    \        reverse_prod(u->l);\n            }\n            if (cl) {\n         \
    \       u->r->rev ^= true;\n                reverse_prod(u->r);\n            }\n\
    \            u->rev = false;\n        }\n        u->f = operator_monoid_type();\n\
    \    }\n    void aggregate(const node_shared& u) const override {\n        u->prod\
    \ = u->m;\n        u->rprod = u->m;\n        if (u->l and u->l->p.lock() == u)\
    \ {\n            u->prod = u->l->prod * u->prod;\n            u->rprod = u->rprod\
    \ * u->l->rprod;\n        }\n        if (u->r and u->r->p.lock() == u) {\n   \
    \         u->prod = u->prod * u->r->prod;\n            u->rprod = u->r->rprod\
    \ * u->rprod;\n        }\n    }\n    void cut(size_t u, size_t v) const {\n  \
    \      Base::evert(nodes[u]);\n        Base::expose(nodes[v]);\n        auto l\
    \ = nodes[v]->l;\n        nodes[v]->l = nullptr;\n        l->p.reset();\n    \
    \    this->aggregate(nodes[v]);\n    }\n    void link(size_t u, size_t v) const\
    \ {\n        Base::link(nodes[v], nodes[u]);\n    }\n    monoid_type prod(size_t\
    \ u, size_t v) const {\n        Base::evert(nodes[u]);\n        Base::expose(nodes[v]);\n\
    \        return nodes[v]->prod;\n    }\n    template<class... Args>\n    void\
    \ set(size_t i, Args&&... args) const {\n        auto u = nodes[i];\n        Base::splay(u);\n\
    \        u->m = monoid_type(std::forward<Args>(args)...);\n        this->aggregate(u);\n\
    \    }\n    void update(size_t i, const operator_monoid_type& f) const {\n   \
    \     auto u = nodes[i];\n        Base::splay(u);\n        u->m = f.act(u->m);\n\
    \        this->aggregate(u);\n    }\n    void update(size_t u, size_t v, const\
    \ operator_monoid_type& f) const {\n        Base::evert(nodes[u]);\n        auto\
    \ nv = nodes[v];\n        Base::expose(nv);\n        nv->m = f.act(nv->m);\n \
    \       nv->prod = f.act(nv->prod);\n        nv->rprod = f.act(nv->rprod);\n \
    \       nv->f *= f;\n        Base::splay(nv);\n    }\n};\n#line 3 \"test/yosupo/dynamic_tree_vertex_add_path_sum.test.cpp\"\
    \n#include <bits/stdc++.h>\nusing namespace std;\n\nstruct Sum {\n    long long\
    \ x;\n    Sum(long long x = 0) : x(x) {}\n    Sum operator*(const Sum& rhs) const\
    \ {\n        return Sum(x + rhs.x);\n    }\n    Sum& operator*=(const Sum& rhs)\
    \ {\n        return *this = *this * rhs;\n    }\n    Sum act(const Sum& rhs) const\
    \ {\n        return Sum(x + rhs.x);\n    }\n};\n\nint main() {\n    int n,q; cin>>n>>q;\n\
    \    vector<int> A(n);\n    for (int i = 0; i < n; i++)\n        cin>>A[i];\n\
    \    LinkCutTree<Sum, Sum> T(A.begin(), A.end());\n    for (int i = 0; i < n-1;\
    \ i++) {\n        int u,v; cin>>u>>v; \n        T.link(u,v);\n    }\n    for (int\
    \ i = 0; i < q; i++) {\n        int t; cin>>t;\n        if (t == 0) {\n      \
    \      int u,v,w,x; cin>>u>>v>>w>>x;\n            T.cut(u,v);\n            T.link(w,x);\n\
    \        } else if (t == 1) {\n            int p,x; cin>>p>>x;\n            T.update(p,\
    \ x);\n        } else {\n            int u,v; cin>>u>>v;\n            cout <<\
    \ T.prod(u,v).x << endl;\n        }\n    }\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum\"\
    \n#include \"../../include/mtl/link_cut_tree.hpp\"\n#include <bits/stdc++.h>\n\
    using namespace std;\n\nstruct Sum {\n    long long x;\n    Sum(long long x =\
    \ 0) : x(x) {}\n    Sum operator*(const Sum& rhs) const {\n        return Sum(x\
    \ + rhs.x);\n    }\n    Sum& operator*=(const Sum& rhs) {\n        return *this\
    \ = *this * rhs;\n    }\n    Sum act(const Sum& rhs) const {\n        return Sum(x\
    \ + rhs.x);\n    }\n};\n\nint main() {\n    int n,q; cin>>n>>q;\n    vector<int>\
    \ A(n);\n    for (int i = 0; i < n; i++)\n        cin>>A[i];\n    LinkCutTree<Sum,\
    \ Sum> T(A.begin(), A.end());\n    for (int i = 0; i < n-1; i++) {\n        int\
    \ u,v; cin>>u>>v; \n        T.link(u,v);\n    }\n    for (int i = 0; i < q; i++)\
    \ {\n        int t; cin>>t;\n        if (t == 0) {\n            int u,v,w,x; cin>>u>>v>>w>>x;\n\
    \            T.cut(u,v);\n            T.link(w,x);\n        } else if (t == 1)\
    \ {\n            int p,x; cin>>p>>x;\n            T.update(p, x);\n        } else\
    \ {\n            int u,v; cin>>u>>v;\n            cout << T.prod(u,v).x << endl;\n\
    \        }\n    }\n}"
  dependsOn:
  - include/mtl/link_cut_tree.hpp
  - include/mtl/splay_tree.hpp
  - include/mtl/monoid.hpp
  isVerificationFile: true
  path: test/yosupo/dynamic_tree_vertex_add_path_sum.test.cpp
  requiredBy: []
  timestamp: '2023-04-20 09:14:32+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: test/yosupo/dynamic_tree_vertex_add_path_sum.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/dynamic_tree_vertex_add_path_sum.test.cpp
- /verify/test/yosupo/dynamic_tree_vertex_add_path_sum.test.cpp.html
title: test/yosupo/dynamic_tree_vertex_add_path_sum.test.cpp
---
