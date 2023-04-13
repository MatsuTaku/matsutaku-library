---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':question:'
    path: include/mtl/segment_hld.hpp
    title: include/mtl/segment_hld.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
    title: test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
  - icon: ':x:'
    path: test/yosupo/vertex_set_path_composite.test.cpp
    title: test/yosupo/vertex_set_path_composite.test.cpp
  _isVerificationFailed: true
  _pathExtension: hpp
  _verificationStatusIcon: ':question:'
  attributes:
    links: []
  bundledCode: "#line 2 \"include/mtl/monoid.hpp\"\n#if __cpp_concepts >= 202002L\n\
    #include <concepts>\n#endif\n\ntemplate<class T, T (*op)(T, T), T E>\nstruct Monoid\
    \ {\n  T x;\n  Monoid(T x=E) : x(x) {}\n  Monoid operator*(const Monoid& rhs)\
    \ const {\n    return Monoid(op(x, rhs.x));\n  }\n};\n\n#if __cpp_concepts >=\
    \ 202002L\ntemplate<class T>\nconcept IsMonoid = requires (T m) {\n  { m * m }\
    \ -> std::same_as<T>;\n};\n#endif\n\ntemplate<class T, T (*op)(T, T), T E>\nstruct\
    \ CommutativeMonoid : public Monoid<T, op, E> {\n    using Base = Monoid<T, op,\
    \ E>;\n    CommutativeMonoid(T x=E) : Base(x) {}\n    CommutativeMonoid operator+(const\
    \ CommutativeMonoid& rhs) const {\n        return CommutativeMonoid(*this * rhs);\n\
    \    }\n};\n\n#if __cpp_concepts >= 202002L\ntemplate<class T>\nconcept IsCommutativeMonoid\
    \ = requires (T m) {\n  { m + m } -> std::same_as<T>;\n};\n#endif\n\ntemplate<class\
    \ S, class F, S (*mapping)(F, S), S (*composition)(F, S), F (*id)()>\nstruct OperatorMonoid\
    \ {\n    F f;\n    OperatorMonoid() : f(id()) {}\n    template<class... Args>\n\
    \    OperatorMonoid(Args&&... args) : f(std::forward<Args>(args)...) {}\n    OperatorMonoid&\
    \ operator*=(const OperatorMonoid& rhs) {\n        f = composition(rhs.f, f);\n\
    \        return *this;\n    }\n    S act(const S& s) const {\n        return mapping(f,\
    \ s);\n    }\n};\n\n#if __cpp_concepts >= 202002L\ntemplate<class F, class S>\n\
    concept IsOperatorMonoid = requires (F f, S s) {\n    { f *= f } -> std::same_as<F&>;\n\
    \    { f.act(s) } -> std::same_as<S>;\n};\n#endif\n"
  code: "#pragma once\n#if __cpp_concepts >= 202002L\n#include <concepts>\n#endif\n\
    \ntemplate<class T, T (*op)(T, T), T E>\nstruct Monoid {\n  T x;\n  Monoid(T x=E)\
    \ : x(x) {}\n  Monoid operator*(const Monoid& rhs) const {\n    return Monoid(op(x,\
    \ rhs.x));\n  }\n};\n\n#if __cpp_concepts >= 202002L\ntemplate<class T>\nconcept\
    \ IsMonoid = requires (T m) {\n  { m * m } -> std::same_as<T>;\n};\n#endif\n\n\
    template<class T, T (*op)(T, T), T E>\nstruct CommutativeMonoid : public Monoid<T,\
    \ op, E> {\n    using Base = Monoid<T, op, E>;\n    CommutativeMonoid(T x=E) :\
    \ Base(x) {}\n    CommutativeMonoid operator+(const CommutativeMonoid& rhs) const\
    \ {\n        return CommutativeMonoid(*this * rhs);\n    }\n};\n\n#if __cpp_concepts\
    \ >= 202002L\ntemplate<class T>\nconcept IsCommutativeMonoid = requires (T m)\
    \ {\n  { m + m } -> std::same_as<T>;\n};\n#endif\n\ntemplate<class S, class F,\
    \ S (*mapping)(F, S), S (*composition)(F, S), F (*id)()>\nstruct OperatorMonoid\
    \ {\n    F f;\n    OperatorMonoid() : f(id()) {}\n    template<class... Args>\n\
    \    OperatorMonoid(Args&&... args) : f(std::forward<Args>(args)...) {}\n    OperatorMonoid&\
    \ operator*=(const OperatorMonoid& rhs) {\n        f = composition(rhs.f, f);\n\
    \        return *this;\n    }\n    S act(const S& s) const {\n        return mapping(f,\
    \ s);\n    }\n};\n\n#if __cpp_concepts >= 202002L\ntemplate<class F, class S>\n\
    concept IsOperatorMonoid = requires (F f, S s) {\n    { f *= f } -> std::same_as<F&>;\n\
    \    { f.act(s) } -> std::same_as<S>;\n};\n#endif"
  dependsOn: []
  isVerificationFile: false
  path: include/mtl/monoid.hpp
  requiredBy:
  - include/mtl/segment_hld.hpp
  timestamp: '2023-04-13 21:51:40+09:00'
  verificationStatus: LIBRARY_SOME_WA
  verifiedWith:
  - test/yosupo/vertex_set_path_composite.test.cpp
  - test/aoj/aoj-do_use_segment_tree-balanced_tree.test.cpp
documentation_of: include/mtl/monoid.hpp
layout: document
redirect_from:
- /library/include/mtl/monoid.hpp
- /library/include/mtl/monoid.hpp.html
title: include/mtl/monoid.hpp
---
