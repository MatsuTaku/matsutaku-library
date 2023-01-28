#pragma once
#include "ntt.hpp"
#include <vector>
#include <initializer_list>

template<int Mod = 998244353>
class Fps : public std::vector<Modular<Mod>> {
  using _base = std::vector<Modular<Mod>>;
 public:
  using mint = Modular<Mod>;
  using f = std::vector<mint>;
  explicit Fps(size_t n=0, mint v=0) : f(n, v) {}
  template<typename It> Fps(It begin, It end) : f(begin, end) {}
  Fps(std::initializer_list<mint> l) : f(l) {}
  explicit Fps(const std::vector<mint>& l) : f(l) {}
  explicit Fps(std::vector<mint>&& l) : f(std::move(l)) {}

  Fps& normalize() {
    int n = (int) _base::size();
    while (n-1 >= 0 and (*this)[n-1] == 0) n--;
    _base::resize(n);
    return *this;
  }
  Fps& operator+=(mint x) {
    if (_base::empty())
      _base::resize(1);
    (*this)[0] += x;
    return *this;
  }
  Fps& operator-=(mint x) { return *this += -x; }
  Fps& operator*=(mint x) {
    for (auto& v : *this) v *= x;
    return *this;
  }
  Fps& operator/=(mint x) { return *this *= x.inv(); }
  Fps operator+(mint x) const { return Fps(*this) += x; }
  Fps operator-(mint x) const { return Fps(*this) -= x; }
  Fps operator*(mint x) const { return Fps(*this) *= x; }
  Fps operator/(mint x) const { return Fps(*this) /= x; }
  Fps operator-() const {
    auto g = *this;
    for (auto& v : g) v = -v;
    return g;
  }
  Fps& operator+=(const Fps& r) {
    if (r.size() > _base::size())
      _base::resize(r.size());
    for (int i = 0; i < (int) r.size(); i++)
      (*this)[i] += r[i];
    return *this;
  }
  Fps& operator+=(Fps&& r) {
    if (r.size() > _base::size())
      std::swap(*this, r);
    for (int i = 0; i < (int) r.size(); i++)
      (*this)[i] += r[i];
    return *this;
  }
  Fps& operator-=(const Fps& r) {
    if (r.size() > _base::size())
      resize(r.size());
    for (int i = 0; i < (int) r.size(); i++)
      (*this)[i] -= r[i];
    return *this;
  }
  Fps& operator-=(Fps&& r) {
    if (r.size() > _base::size())
      std::swap(*this, r);
    for (int i = 0; i < (int) r.size(); i++)
      (*this)[i] -= r[i];
    return *this;
  }
  Fps& dot(const Fps& r) {
    if (_base::size() > r.size())
      _base::resize(r.size());
    for (int i = 0; i < (int) _base::size(); i++)
      (*this)[i] *= r[i];
    return *this;
  }
  Fps& dot(Fps&& r) {
    if (_base::size() > r.size())
      std::swap(*this, r);
    for (int i = 0; i < (int) _base::size(); i++)
      (*this)[i] *= r[i];
    return *this;
  }
 private:
  template<class F>
  Fps& _mul_set(F&& r) {
    return *this = Fps(convolution(std::move(*this), std::forward<F>(r)));
  }
 public:
  Fps& operator*=(const Fps& r) {
    return _mul_set(r);
  }
  Fps& operator*=(Fps&& r) {
    return _mul_set(std::move(r));
  }
  Fps inv(int n = -1) const {
    if (n == -1)
      n = (int) _base::size();
    assert(!_base::empty() and (*this)[0] != 0);
    Fps g,fm;
    g.reserve(n); fm.reserve(n);
    g.push_back((*this)[0].inv());
    fm.push_back((*this)[0]);
    for (int m = 1; m < n; m <<= 1) {
      int nm = std::min(m*2, n);
      for (int i = m; i < nm; i++)
        fm.push_back(i < (int) _base::size() ? (*this)[i] : 0);
      auto fgg = g * g * fm;
      for (int i = m; i < nm; i++)
        g.push_back(-fgg[i]);
    }
    return g;
  }
  Fps& operator/=(const Fps& r) {
    return *this *= r.inv();
  }
 private:
  template<class F>
  Fps& _mod_set(F&& r) {
    normalize();
    Fps q = *this / r;
    return *this -= q * std::forward<F>(r);
  }
 public:
  Fps& operator%=(const Fps& r) {
    return _mod_set(r);
  }
  Fps& operator%=(Fps&& r) {
    return _mod_set(std::move(r));
  }
  Fps operator+(const Fps& r) const { return Fps(*this) += r; }
  Fps operator+(Fps&& r) const { return Fps(*this) += std::move(r); }
  Fps operator-(const Fps& r) const { return Fps(*this) -= r; }
  Fps operator-(Fps&& r) const { return Fps(*this) -= std::move(r); }
  Fps operator*(const Fps& r) const { return Fps(*this) *= r; }
  Fps operator*(Fps&& r) const { return Fps(*this) *= std::move(r); }
  Fps operator/(const Fps& r) const { return Fps(*this) /= r; }
  Fps operator/(Fps&& r) const { return Fps(*this) /= std::move(r); }
  Fps operator%(const Fps& r) const { return Fps(*this) %= r; }
  Fps operator%(Fps&& r) const { return Fps(*this) %= std::move(r); }
};
