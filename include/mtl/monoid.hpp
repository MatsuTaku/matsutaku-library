#pragma once
#if __cpp_concepts >= 202002L
#include <concepts>
#endif

template<class T, T (*op)(T, T), T (*e)()>
struct Monoid {
  T x;
  Monoid(T x=e()) : x(x) {}
  Monoid operator*(const Monoid& rhs) const {
    return Monoid(op(x, rhs.x));
  }
};

#if __cpp_concepts >= 202002L
template<class T>
concept IsMonoid = requires (T m) {
  { m * m } -> std::same_as<T>;
};
#endif

template<class T, T (*op)(T, T), T (*e)()>
struct CommutativeMonoid : public Monoid<T, op, e> {
    using Base = Monoid<T, op, e>;
    CommutativeMonoid(T x=e()) : Base(x) {}
    CommutativeMonoid operator+(const CommutativeMonoid& rhs) const {
        return CommutativeMonoid(*this * rhs);
    }
};

#if __cpp_concepts >= 202002L
template<class T>
concept IsCommutativeMonoid = requires (T m) {
  { m + m } -> std::same_as<T>;
};
#endif

template<class S, class F, S (*mapping)(F, S), S (*composition)(F, S), F (*id)()>
struct OperatorMonoid {
    F f;
    OperatorMonoid() : f(id()) {}
    template<class... Args>
    OperatorMonoid(Args&&... args) : f(std::forward<Args>(args)...) {}
    OperatorMonoid& operator*=(const OperatorMonoid& rhs) {
        f = composition(rhs.f, f);
        return *this;
    }
    S act(const S& s) const {
        return mapping(f, s);
    }
};

#if __cpp_concepts >= 202002L
template<class F, class S>
concept IsOperatorMonoid = requires (F f, S s) {
    { f *= f } -> std::same_as<F&>;
    { f.act(s) } -> std::same_as<S>;
};
#endif