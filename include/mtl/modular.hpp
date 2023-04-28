#pragma once
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

  friend std::ostream& operator<<(std::ostream& os, const Modular& x) {
    return os << x.val();
  }
  friend std::istream& operator>>(std::istream& is, Modular& x) {
    return is >> x.val_;
  }

};

using Modular998244353 = Modular<998244353>;
using Modular1000000007 = Modular<(int)1e9+7>;

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
  int t = 1;
  int u = x;
  while (p) {
    if (p & 1) {
      t *= u;
      t %= m;
    }
    u *= u;
    u %= m;
    p >>= 1;
  }
  return t;
}

constexpr int primitive_root_constexpr(int m) {
  if (m == 2) return 1;
  if (m == 998244353) return 3;

  std::array<int, 20> divs{2};
  int cnt = 1;
  int x = (m-1) / 2;
  for (int d = 3; d*d <= x; d += 2) {
    if (x % d == 0) {
      divs[cnt++] = d;
      while (x % d == 0)
        x /= d;
    }
  }
  if (x > 1) divs[cnt++] = x;
  for (int g = 2; ; g++) {
    bool ok = true;
    for (int i = 0; i < cnt; i++) {
      if (mod_pow_constexpr(g, (m-1) / divs[cnt], m) == 1) {
        ok = false;
        break;
      }
    }
    if (ok) return g;
  }
}

template<int m>
constexpr int primitive_root = primitive_root_constexpr(m);

}
