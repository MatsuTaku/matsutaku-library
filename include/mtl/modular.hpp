#pragma once
#include <iostream>

template <long long MOD>
class Modular {
 private:
  long long val_;

 public:
  Modular() : val_(0) {}
  Modular(long long v) : val_(v%MOD) {
    if (val_ < 0) val_ += MOD;
  }

  long long val() const { return val_; }
  Modular& operator+=(Modular x) {
    val_ += x.val();
    if (val_ >= MOD) val_ %= MOD;
    return *this;
  }
  Modular operator-() const { return {MOD - val_}; }
  Modular& operator-=(Modular x) {
    val_ -= x.val();
    if (val_ < 0) val_ += MOD;
    return *this;
  }
  Modular& operator*=(Modular x) {
    val_ *= x.val();
    if (val_ >= MOD) val_ %= MOD;
    return *this;
  }
  Modular pow(long long p) const {
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
  friend Modular pow(Modular x, long long p) {
    return x.pow(p);
  }
  Modular inv() const { return pow(MOD-2); }
  Modular& operator/=(Modular x) { return *this *= x.inv(); }
  Modular operator+(Modular x) const { return Modular(*this) += x; }
  Modular operator-(Modular x) const { return Modular(*this) -= x; }
  Modular operator*(Modular x) const { return Modular(*this) *= x; }
  Modular operator/(Modular x) const { return Modular(*this) /= x; }
  Modular& operator++() { return *this += 1; }
  Modular operator++(int) { Modular c = *this; ++(*this); return c; }
  Modular& operator--() { return *this -= 1; }
  Modular operator--(int) { Modular c = *this; --(*this); return c; }

  bool operator==(Modular x) const { return val() == x.val(); }
  bool operator!=(Modular x) const { return val() != x.val(); }
  bool operator<(Modular x) const { return val() < x.val(); };
  bool operator<=(Modular x) const { return val() <= x.val(); };
  bool operator>(Modular x) const { return val() > x.val(); };
  bool operator>=(Modular x) const { return val() >= x.val(); };

  friend std::ostream& operator<<(std::ostream& os, const Modular& x) {
    return os << x.val();
  }
  friend std::istream& operator>>(std::istream& is, Modular& x) {
    return is >> x.val_;
  }

};

using Modular998244353 = Modular<998244353>;
using Modular1000000007 = Modular<(int)1e9+7>;
