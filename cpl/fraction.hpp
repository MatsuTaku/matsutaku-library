#pragma once
#if __cplusplus >= 202002L
#define CPL_CPP20
#include <compare>
#endif
#include <numeric>
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
  FractionBase& reduct() {
    int g = std::gcd(u,v);
    u /= g;
    v /= g;
    return *this;
  }
  template<typename T>
  inline T get() const {
    return (T) u / (T) v;
  }
  
};

struct FractionOrdering : public FractionBase<int> {
  FractionOrdering(int _u, int _v) : FractionBase<int>(_u,_v) {}
  FractionOrdering(int u = 0) : FractionOrdering(u, 1) {}

  inline bool operator==(const FractionOrdering& rhs) const {
    return (long long int) u * rhs.v == (long long int) v * rhs.u;
  }
#ifdef CPL_CPP20
  auto operator<=>(const FractionOrdering&) const {
    auto lv = (long long int) u * rhs.v;
    auto rv = (long long int) v * rhs.u;
    if (lv != rv) {
      return lv <=> rv;
    } else {
      return std::string_ordering::equal;
    }
  }
#else
  inline bool operator!=(const FractionOrdering& rhs) const {
    return (long long int) u * rhs.v != (long long int) v * rhs.u;
  }
  inline bool operator<(const FractionOrdering& rhs) const {
    return (long long int) u * rhs.v < (long long int) v * rhs.u;
  }
  inline bool operator<=(const FractionOrdering& rhs) const {
    return (long long int) u * rhs.v <= (long long int) v * rhs.u;
  }
  inline bool operator>(const FractionOrdering& rhs) const {
    return (long long int) u * rhs.v > (long long int) v * rhs.u;
  }
  inline bool operator>=(const FractionOrdering& rhs) const {
    return (long long int) u * rhs.v >= (long long int) v * rhs.u;
  }
#endif
  inline bool operator==(int rhs) const {
    return u == (long long int) v * rhs;
  }
  inline bool operator!=(int rhs) const {
    return u != (long long int) v * rhs;
  }
  inline bool operator<(int rhs) const {
    return u < (long long int) v * rhs;
  }
  inline bool operator<=(int rhs) const {
    return u <= (long long int) v * rhs;
  }
  inline bool operator>(int rhs) const {
    return u > (long long int) v * rhs;
  }
  inline bool operator>=(int rhs) const {
    return u >= (long long int) v * rhs;
  }
  inline friend bool operator==(int lhs, const FractionOrdering& rhs) {
    return rhs == lhs;
  }
  inline friend bool operator!=(int lhs, const FractionOrdering& rhs) {
    return rhs != lhs;
  }
  inline friend bool operator<(int lhs, const FractionOrdering& rhs) {
    return rhs > lhs;
  }
  inline friend bool operator<=(int lhs, const FractionOrdering& rhs) {
    return rhs >= lhs;
  }
  inline friend bool operator>(int lhs, const FractionOrdering& rhs) {
    return rhs < lhs;
  }
  inline friend bool operator>=(int lhs, const FractionOrdering& rhs) {
    return rhs <= lhs;
  }

};

struct Fraction : public FractionBase<long long> {
  Fraction(long long u, long long v) : FractionBase<long long>(u,v) {}
  Fraction(long long u = 0) : Fraction(u, 1) {}
  Fraction(const FractionOrdering& fo) : Fraction(fo.u,fo.v) {}

  Fraction& reduct() {
    FractionBase<long long>::reduct();
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

};

