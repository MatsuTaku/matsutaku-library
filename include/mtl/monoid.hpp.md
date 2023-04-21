---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: include/mtl/link_cut_tree.hpp
    title: include/mtl/link_cut_tree.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/segment_hld.hpp
    title: include/mtl/segment_hld.hpp
  - icon: ':heavy_check_mark:'
    path: include/mtl/splay_tree_list.hpp
    title: include/mtl/splay_tree_list.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
    title: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp
    title: test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/dynamic_tree_vertex_add_path_sum.test.cpp
    title: test/yosupo/dynamic_tree_vertex_add_path_sum.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/dynamic_tree_vertex_set_path_composite.test.cpp
    title: test/yosupo/dynamic_tree_vertex_set_path_composite.test.cpp
  - icon: ':heavy_check_mark:'
    path: test/yosupo/vertex_set_path_composite.test.cpp
    title: test/yosupo/vertex_set_path_composite.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/monoid.hpp\"\n#if __cpp_concepts >= 202002L\n\
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
    \ } -> std::same_as<F&>;\n    { f.act(s) } -> std::same_as<S>;\n};\n#endif\n"
  code: "#pragma once\n#if __cpp_concepts >= 202002L\n#include <concepts>\n#endif\n\
    \ntemplate<class T, T (*op)(T, T), T (*e)()>\nstruct Monoid {\n  T x;\n  Monoid()\
    \ : x(e()) {}\n  template<class... Args>\n  Monoid(Args&&... args) : x(std::forward<Args>(args)...)\
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
    #endif"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/monoid.hpp
  requiredBy:
  - include/mtl/link_cut_tree.hpp
  - include/mtl/segment_hld.hpp
  - include/mtl/splay_tree_list.hpp
  timestamp: '2023-04-20 08:14:31+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/yosupo/dynamic_sequence_range_affine_range_sum.test.cpp
  - test/yosupo/dynamic_tree_vertex_set_path_composite.test.cpp
  - test/yosupo/vertex_set_path_composite.test.cpp
  - test/yosupo/dynamic_tree_vertex_add_path_sum.test.cpp
  - test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
documentation_of: include/mtl/monoid.hpp
layout: document
redirect_from:
- /library/include/mtl/monoid.hpp
- /library/include/mtl/monoid.hpp.html
title: include/mtl/monoid.hpp
---
