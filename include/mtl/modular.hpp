#pragma once
#include <iostream>
#include <cassert>

template <int MOD>
class Modular {
 private:
  unsigned int val_;

 public:
  static constexpr unsigned int mod() { return MOD; }

  constexpr Modular() : val_(0) {}
  template<class T,
      std::enable_if_t<
          std::is_integral<T>::value && std::is_unsigned<T>::value
      > * = nullptr>
  constexpr Modular(T v) : val_(v%mod()) {}
  template<class T,
      std::enable_if_t<
          std::is_integral<T>::value && !std::is_unsigned<T>::value
      > * = nullptr>
  constexpr Modular(T v) {
    auto x = (long long)(v%(long long)mod());
    if (x < 0) x += mod();
    val_ = (unsigned int)x;
  }

  constexpr unsigned int val() const { return val_; }
  constexpr Modular& operator+=(Modular x) {
    val_ += x.val();
    if (val_ >= mod()) val_ -= mod();
    return *this;
  }
  constexpr Modular operator-() const { return {mod() - val_}; }
  constexpr Modular& operator-=(Modular x) {
    val_ += mod() - x.val();
    if (val_ >= mod()) val_ -= mod();
    return *this;
  }
  constexpr Modular& operator*=(Modular x) {
    auto v = (long long) val_ * x.val();
    if (v >= mod()) v %= mod();
    val_ = v;
    return *this;
  }
  constexpr Modular pow(long long p) const {
    assert(p >= 0);
    Modular t = 1;
    Modular u = *this;
    while (p) {
      if (p & 1)
        t *= u;
      u *= u;
      p >>= 1;
    }
    return t;
  }
  friend constexpr Modular pow(Modular x, long long p) {
    return x.pow(p);
  }
  constexpr Modular inv() const { return pow(mod()-2); }
  constexpr Modular& operator/=(Modular x) { return *this *= x.inv(); }
  constexpr Modular operator+(Modular x) const { return Modular(*this) += x; }
  constexpr Modular operator-(Modular x) const { return Modular(*this) -= x; }
  constexpr Modular operator*(Modular x) const { return Modular(*this) *= x; }
  constexpr Modular operator/(Modular x) const { return Modular(*this) /= x; }
  constexpr Modular& operator++() { return *this += 1; }
  constexpr Modular operator++(int) { Modular c = *this; ++(*this); return c; }
  constexpr Modular& operator--() { return *this -= 1; }
  constexpr Modular operator--(int) { Modular c = *this; --(*this); return c; }

  constexpr bool operator==(Modular x) const { return val() == x.val(); }
  constexpr bool operator!=(Modular x) const { return val() != x.val(); }

  friend std::ostream& operator<<(std::ostream& os, const Modular& x) {
    return os << x.val();
  }
  friend std::istream& operator>>(std::istream& is, Modular& x) {
    return is >> x.val_;
  }

};

using Modular998244353 = Modular<998244353>;
using Modular1000000007 = Modular<(int)1e9+7>;
