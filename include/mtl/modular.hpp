#pragma once
#include "bit_manip.hpp"
#include <iostream>
#include <cassert>

template <int MOD>
class Modular {
 private:
  unsigned int val_;

 public:
  static constexpr unsigned int mod() { return MOD; }
  template<class T>
  static constexpr unsigned int safe_mod(T v) {
    auto x = (long long)(v%(long long)mod());
    if (x < 0) x += mod();
    return (unsigned int) x;
  }

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
  constexpr Modular(T v) : val_(safe_mod(v)) {}

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

  constexpr bool is_square() const {
    return pow((mod()-1)/2) == 1;
  }
  /**
   * Return x s.t. x * x = a mod p
   * reference: https://zenn.dev/peria/articles/c6afc72b6b003c
  */
  constexpr Modular sqrt() const {
    if (!is_square()) 
      throw std::runtime_error("not square");
    auto mod_eight = mod() % 8;
    if (mod_eight == 3 || mod_eight == 7) {
      return pow((mod()+1)/4);
    } else if (mod_eight == 5) {
      auto x = pow((mod()+3)/8);
      if (x * x != *this)
        x *= Modular(2).pow((mod()-1)/4);
      return x;
    } else {
      Modular d = 2;
      while (d.is_square())
        d += 1;
      auto t = mod()-1;
      int s = bm::ctz(t);
      t >>= s;
      auto a = pow(t);
      auto D = d.pow(t);
      int m = 0;
      Modular dt = 1;
      Modular du = D;
      for (int i = 0; i < s; i++) {
        if ((a*dt).pow(1u<<(s-1-i)) == -1) {
          m |= 1u << i;
          dt *= du;
        }
        du *= du;
      }
      return pow((t+1)/2) * D.pow(m/2);
    }
  }

  friend std::ostream& operator<<(std::ostream& os, const Modular& x) {
    return os << x.val();
  }
  friend std::istream& operator>>(std::istream& is, Modular& x) {
    return is >> x.val_;
  }

};

using Modular998244353 = Modular<998244353>;
using Modular1000000007 = Modular<(int)1e9+7>;

template<int Id=0>
class DynamicModular {
 private:
  static unsigned int mod_;
  unsigned int val_;

 public:
  static unsigned int mod() { return mod_; }
  static void set_mod(unsigned int m) { mod_ = m; }
  template<class T>
  static constexpr unsigned int safe_mod(T v) {
    auto x = (long long)(v%(long long)mod());
    if (x < 0) x += mod();
    return (unsigned int) x;
  }

  constexpr DynamicModular() : val_(0) {}
  template<class T,
      std::enable_if_t<
          std::is_integral<T>::value && std::is_unsigned<T>::value
      > * = nullptr>
  constexpr DynamicModular(T v) : val_(v%mod()) {}
  template<class T,
      std::enable_if_t<
          std::is_integral<T>::value && !std::is_unsigned<T>::value
      > * = nullptr>
  constexpr DynamicModular(T v) : val_(safe_mod(v)) {}

  constexpr unsigned int val() const { return val_; }
  constexpr DynamicModular& operator+=(DynamicModular x) {
    val_ += x.val();
    if (val_ >= mod()) val_ -= mod();
    return *this;
  }
  constexpr DynamicModular operator-() const { return {mod() - val_}; }
  constexpr DynamicModular& operator-=(DynamicModular x) {
    val_ += mod() - x.val();
    if (val_ >= mod()) val_ -= mod();
    return *this;
  }
  constexpr DynamicModular& operator*=(DynamicModular x) {
    auto v = (long long) val_ * x.val();
    if (v >= mod()) v %= mod();
    val_ = v;
    return *this;
  }
  constexpr DynamicModular pow(long long p) const {
    assert(p >= 0);
    DynamicModular t = 1;
    DynamicModular u = *this;
    while (p) {
      if (p & 1)
        t *= u;
      u *= u;
      p >>= 1;
    }
    return t;
  }
  friend constexpr DynamicModular pow(DynamicModular x, long long p) {
    return x.pow(p);
  }
  // TODO: implement when mod is not prime
  constexpr DynamicModular inv() const { return pow(mod()-2); }
  constexpr DynamicModular& operator/=(DynamicModular x) { return *this *= x.inv(); }
  constexpr DynamicModular operator+(DynamicModular x) const { return DynamicModular(*this) += x; }
  constexpr DynamicModular operator-(DynamicModular x) const { return DynamicModular(*this) -= x; }
  constexpr DynamicModular operator*(DynamicModular x) const { return DynamicModular(*this) *= x; }
  constexpr DynamicModular operator/(DynamicModular x) const { return DynamicModular(*this) /= x; }
  constexpr DynamicModular& operator++() { return *this += 1; }
  constexpr DynamicModular operator++(int) { DynamicModular c = *this; ++(*this); return c; }
  constexpr DynamicModular& operator--() { return *this -= 1; }
  constexpr DynamicModular operator--(int) { DynamicModular c = *this; --(*this); return c; }

  constexpr bool operator==(DynamicModular x) const { return val() == x.val(); }
  constexpr bool operator!=(DynamicModular x) const { return val() != x.val(); }

  constexpr bool is_square() const {
    return val() == 0 or pow((mod()-1)/2) == 1;
  }
  /**
   * Return x s.t. x * x = a mod p
   * reference: https://zenn.dev/peria/articles/c6afc72b6b003c
  */
  constexpr DynamicModular sqrt() const {
    // assert mod is prime
    if (!is_square()) 
      throw std::runtime_error("not square");
    if (val() < 2)
      return val();
    auto mod_eight = mod() % 8;
    if (mod_eight == 3 || mod_eight == 7) {
      return pow((mod()+1)/4);
    } else if (mod_eight == 5) {
      auto x = pow((mod()+3)/8);
      if (x * x != *this)
        x *= DynamicModular(2).pow((mod()-1)/4);
      return x;
    } else {
      DynamicModular d = 2;
      while (d.is_square())
        ++d;
      auto t = mod()-1;
      int s = bm::ctz(t);
      t >>= s;
      auto a = pow(t);
      auto D = d.pow(t);
      int m = 0;
      DynamicModular dt = 1;
      DynamicModular du = D;
      for (int i = 0; i < s; i++) {
        if ((a*dt).pow(1u<<(s-1-i)) == -1) {
          m |= 1u << i;
          dt *= du;
        }
        du *= du;
      }
      return pow((t+1)/2) * D.pow(m/2);
    }
  }

  friend std::ostream& operator<<(std::ostream& os, const DynamicModular& x) {
    return os << x.val();
  }
  friend std::istream& operator>>(std::istream& is, DynamicModular& x) {
    return is >> x.val_;
  }

};
template<int Id>
unsigned int DynamicModular<Id>::mod_;

#include <vector>

template<class ModInt>
struct ModularUtil {
  static constexpr int mod = ModInt::mod();
  static struct inv_table {
    std::vector<ModInt> tb{0,1};
    inv_table() : tb({0,1}) {}
  } inv_;
  void set_inv(int n) {
    int m = inv_.tb.size();
    if (m > n) return;
    inv_.tb.resize(n+1);
    for (int i = m; i < n+1; i++)
      inv_.tb[i] = -inv_.tb[mod % i] * (mod / i);
  }
  ModInt& inv(int i) {
    set_inv(i);
    return inv_.tb[i];
  }
};
template<class ModInt>
typename ModularUtil<ModInt>::inv_table ModularUtil<ModInt>::inv_;

#include <array>

namespace math {

constexpr int mod_pow_constexpr(int x, int p, int m) {
  long long t = 1;
  long long u = x;
  while (p) {
    if (p & 1) {
      t *= u;
      t %= m;
    }
    u *= u;
    u %= m;
    p >>= 1;
  }
  return (int) t;
}

constexpr int primitive_root_constexpr(int m) {
  if (m == 2) return 1;
  if (m == 167772161) return 3;
  if (m == 469762049) return 3;
  if (m == 754974721) return 11;
  if (m == 880803841) return 26;
  if (m == 998244353) return 3;

  std::array<int, 20> divs{};
  int cnt = 0;
  int x = m-1;
  if (x % 2 == 0) {
    divs[cnt++] = 2;
    x >>= bm::ctz(x);
  }
  for (int d = 3; d*d <= x; d += 2) {
    if (x % d == 0) {
      divs[cnt++] = d;
      while (x % d == 0)
        x /= d;
    }
  }
  if (x > 1) divs[cnt++] = x;
  for (int g = 2; g < m; g++) {
    bool ok = true;
    for (int i = 0; i < cnt; i++) {
      if (mod_pow_constexpr(g, (m-1) / divs[i], m) == 1) {
        ok = false;
        break;
      }
    }
    if (ok) return g;
  }
  return -1;
}

template<int m>
constexpr int primitive_root = primitive_root_constexpr(m);

}
