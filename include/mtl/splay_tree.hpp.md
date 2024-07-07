---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':x:'
    path: include/mtl/link_cut_tree.hpp
    title: include/mtl/link_cut_tree.hpp
  - icon: ':x:'
    path: include/mtl/splay_tree_list.hpp
    title: include/mtl/splay_tree_list.hpp
  _extendedVerifiedWith:
  - icon: ':x:'
    path: test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp
    title: test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp
  - icon: ':x:'
    path: test/yosupo/dynamic_tree_vertex_add_path_sum.test.cpp
    title: test/yosupo/dynamic_tree_vertex_add_path_sum.test.cpp
  - icon: ':x:'
    path: test/yosupo/dynamic_tree_vertex_set_path_composite.test.cpp
    title: test/yosupo/dynamic_tree_vertex_set_path_composite.test.cpp
  - icon: ':x:'
    path: test/yosupo/range_affine_range_sum-splay_tree.test.cpp
    title: test/yosupo/range_affine_range_sum-splay_tree.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':x:'
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
    \    }\n\n};\n"
  code: "#pragma once\n#include <memory>\n#include <cassert>\n\ntemplate<class NodeType>\n\
    struct SplayTreeNodeBase {\n    using node_type = NodeType;\n    using node_shared\
    \ = std::shared_ptr<node_type>;\n    using node_weak = std::weak_ptr<node_type>;\n\
    \    node_shared l,r;\n    node_weak p;\n    bool rev;\n    bool is_root() const\
    \ {\n        return p.expired() || (p.lock()->l.get() != this && p.lock()->r.get()\
    \ != this); \n    }\n};\ntemplate<class T>\nstruct SplayTreeNodeTraits {\n   \
    \ using node_type = typename T::node_type;\n    using node_shared = typename T::node_shared;\n\
    \    using node_weak = typename T::node_weak;\n};\n\ntemplate<class Node>\nstruct\
    \ SplayTreeBase {\n    using node_traits = SplayTreeNodeTraits<Node>;\n    using\
    \ node_type = typename node_traits::node_type;\n    using node_shared = typename\
    \ node_traits::node_shared;\n    using node_weak = typename node_traits::node_weak;\n\
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
    \        }\n        } while (!u->is_root());\n    }\n\n};"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/splay_tree.hpp
  requiredBy:
  - include/mtl/link_cut_tree.hpp
  - include/mtl/splay_tree_list.hpp
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_ALL_WA
  verifiedWith:
  - test/yosupo/range_affine_range_sum-splay_tree.test.cpp
  - test/yosupo/dynamic_tree_vertex_set_path_composite.test.cpp
  - test/yosupo/dynamic_tree_vertex_add_path_sum.test.cpp
  - test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp
documentation_of: include/mtl/splay_tree.hpp
layout: document
redirect_from:
- /library/include/mtl/splay_tree.hpp
- /library/include/mtl/splay_tree.hpp.html
title: include/mtl/splay_tree.hpp
---
