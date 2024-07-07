#pragma once
#if __cplusplus >= 202002L
#ifndef MTL_CPP20
#define MTL_CPP20
#endif
#include <compare>
#endif
#include <numeric>
#include <cmath>
#include <cassert>

template<typename I>
struct FractionBase {
  I u,v;
  FractionBase(I _u, I _v) : u(_u), v(_v) {
    assert(v != 0);
    if (u == 0) {
      v = 1;
    } else if (v < 0) {
      u = -u;
      v = -v;
    }
  }
  FractionBase(I _u = 0) : FractionBase(_u, 1) {}
  FractionBase reducted() const {
    int g = std::gcd(u,v);
    return {u/g, v/g};
  }
  FractionBase& reduct() {
    return *this = reducted();
  }
  template<typename T>
  inline T get() const {
    return (T) u / (T) v;
  }
  
};

struct FractionOrdering : public FractionBase<long long int> {
  FractionOrdering(long long int _u, long long int _v) : 
    FractionBase<long long int>((_v >= 0 ? _u : -_u), std::abs(_v)) {}
  FractionOrdering(long long int u = 0) : FractionOrdering(u, 1) {}

  inline bool operator==(const FractionOrdering& rhs) const {
    return u * rhs.v == v * rhs.u;
  }
#ifdef MTL_CPP20
  auto operator<=>(const FractionOrdering& rhs) const -> std::strong_ordering {
    auto lv = u * rhs.v;
    auto rv = v * rhs.u;
    return lv <=> rv;
  }
  auto operator<=>(long long int rhs) const -> std::strong_ordering {
    return u <=> v * rhs;
  }
  friend auto operator==(long long int lhs, const FractionOrdering& rhs) -> std::strong_ordering {
    return lhs * rhs.v <=> rhs.u;
  }
#else
  inline bool operator!=(const FractionOrdering& rhs) const {
    return u * rhs.v != v * rhs.u;
  }
  inline bool operator<(const FractionOrdering& rhs) const {
    return u * rhs.v < v * rhs.u;
  }
  inline bool operator<=(const FractionOrdering& rhs) const {
    return u * rhs.v <= v * rhs.u;
  }
  inline bool operator>(const FractionOrdering& rhs) const {
    return u * rhs.v > v * rhs.u;
  }
  inline bool operator>=(const FractionOrdering& rhs) const {
    return u * rhs.v >= v * rhs.u;
  }
  inline bool operator==(long long int rhs) const {
    return u == v * rhs;
  }
  inline bool operator!=(long long int rhs) const {
    return u != v * rhs;
  }
  inline bool operator<(long long int rhs) const {
    return u < v * rhs;
  }
  inline bool operator<=(long long int rhs) const {
    return u <= v * rhs;
  }
  inline bool operator>(long long int rhs) const {
    return u > v * rhs;
  }
  inline bool operator>=(long long int rhs) const {
    return u >= v * rhs;
  }
  inline friend bool operator==(long long int lhs, const FractionOrdering& rhs) {
    return rhs == lhs;
  }
  inline friend bool operator!=(long long int lhs, const FractionOrdering& rhs) {
    return rhs != lhs;
  }
  inline friend bool operator<(long long int lhs, const FractionOrdering& rhs) {
    return rhs > lhs;
  }
  inline friend bool operator<=(long long int lhs, const FractionOrdering& rhs) {
    return rhs >= lhs;
  }
  inline friend bool operator>(long long int lhs, const FractionOrdering& rhs) {
    return rhs < lhs;
  }
  inline friend bool operator>=(long long int lhs, const FractionOrdering& rhs) {
    return rhs <= lhs;
  }
#endif

};

struct Fraction : public FractionBase<long long> {
  Fraction(long long u, long long v) : FractionBase<long long>(u,v) {}
  Fraction(long long u = 0) : Fraction(u, 1) {}
  Fraction(const FractionOrdering& fo) : Fraction(fo.u,fo.v) {}

  Fraction& reduct() {
    FractionBase::reduct();
    return *this;
  }

  Fraction& operator+=(const Fraction& r) {
    auto g = std::gcd(v, r.v);
    auto rvbg = r.v / g;
    auto nv = v * rvbg;
    auto nu = u * rvbg + r.u * (v / g);
    u = nu;
    v = nv;
    return *this;
  }
  Fraction operator+(const Fraction& r) const {
    return Fraction(*this) += r;
  }
  Fraction& operator-=(const Fraction& r) {
    return operator+=(-r);
  }
  Fraction operator-(const Fraction& r) const {
    return Fraction(*this) -= r;
  }
  Fraction& operator*=(const Fraction& r) {
    auto nv = v * r.v;
    auto nu = u * r.u;
    auto g = std::gcd(nu, nv);
    u = nu / g;
    v = nv / g;
    return *this;
  }
  Fraction operator*(const Fraction& r) const {
    return Fraction(*this) *= r;
  }
  Fraction inv() const {
    assert(u != 0);
    return Fraction(v,u);
  }
  Fraction& operator/=(const Fraction& r) {
    return *this *= r.inv();
  }
  Fraction operator/(const Fraction& r) const {
    return Fraction(*this) /= r;
  }
  Fraction& operator+=(long long x) {
    u += x * v;
    return reduct();
  }
  Fraction operator+(long long x) const {
    return Fraction(*this) += x;
  }
  Fraction& operator-=(long long x) {
    u -= x * v;
    return reduct();
  }
  Fraction operator-(long long x) const {
    return Fraction(*this) -= x;
  }
  Fraction operator-() const {
    return Fraction(-u, v);
  }
  Fraction& operator*=(long long x) {
    u *= x;
    return reduct();
  }
  Fraction operator*(long long x) const {
    return Fraction(*this) *= x;
  }
  Fraction& operator/=(long long x) {
    v *= x;
    return reduct();
  }
  Fraction operator/(long long x) const {
    return Fraction(*this) /= x;
  }
  friend Fraction operator+(long long x, const Fraction& r) {
    return r + x;
  }
  friend Fraction operator-(long long x, const Fraction& r) {
    return -r + x;
  }
  friend Fraction operator*(long long x, const Fraction& r) {
    return r * x;
  }
  friend Fraction operator/(long long x, const Fraction& r) {
    return x * r.inv();
  }

  FractionOrdering to_cmp() const {
    return FractionOrdering(u,v);
  }

  bool operator==(const Fraction& rhs) const {
    return u * rhs.v == v * rhs.u;
  }

#ifdef MTL_CPP20
  auto operator<=>(const Fraction& rhs) const -> decltype(to_cmp() <=> rhs.to_cmp()) {
    return to_cmp() <=> rhs.to_cmp();
  }
#endif

};

