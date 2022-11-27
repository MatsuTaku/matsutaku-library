#pragma once
#include "ntt.hpp"
#include <vector>
#include <initializer_list>

class Fps : public std::vector<_ntt::mint> {
 public:
  using mint = _ntt::mint;
  using f = std::vector<mint>;
  explicit Fps(size_t n=0, mint v=0) : f(n, v) {}
  template<typename It> Fps(It begin, It end) : f(begin, end) {}
  Fps(std::initializer_list<mint> l) : f(l) {}
  explicit Fps(const std::vector<mint>& l) : f(l) {}

  Fps& normalize() {
    int n = (int) size();
    while (n-1 >= 0 and (*this)[n-1] == 0) n--;
    resize(n);
    return *this;
  }
  Fps& operator+=(mint x) {
    if (empty())
      resize(1);
    (*this)[0] += x;
    return *this;
  }
  Fps& operator-=(mint x) { return *this += -x; }
  Fps& operator*=(mint x) {
    for (int i = 0; i < (int) size(); i++)
      (*this)[i] *= x;
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
    if (r.size() > size())
      resize(r.size());
    for (int i = 0; i < (int) r.size(); i++)
      (*this)[i] += r[i];
    return *this;
  }
  Fps& operator-=(const Fps& r) {
    if (r.size() > size())
      resize(r.size());
    for (int i = 0; i < (int) r.size(); i++)
      (*this)[i] -= r[i];
    return *this;
  }
  Fps& dot(const Fps& r) {
    if (size() > r.size())
      resize(r.size());
    for (int i = 0; i < (int) size(); i++)
      (*this)[i] *= r[i];
    return *this;
  }
  Fps& operator*=(const Fps& r) {
    int n = (int) (size() + r.size()) - 1;
    resize(n);
    ntt_inline(*this);
    Fps Fr = r;
    Fr.resize(n);
    ntt_inline(Fr);
    dot(Fr);
    intt_inline(*this);
    return *this /= (int) size();
  }
  Fps& mod(int k) {
    resize(k);
    return *this;
  }
  Fps inv(int n = -1) const {
    if (n == -1)
      n = (int) size();
    assert(!empty() and (*this)[0] != 0);
    Fps g,fm;
    g.reserve(n); fm.reserve(n);
    g.push_back((*this)[0].inv());
    fm.push_back((*this)[0]);
    for (int m = 1; m < n; m <<= 1) {
      int nm = std::min(m*2, n);
      for (int i = m; i < nm; i++)
        fm.push_back(i < (int) size() ? (*this)[i] : 0);
      auto fgg = g * g * fm;
      for (int i = m; i < nm; i++)
        g.push_back(-fgg[i]);
    }
    return g;
  }
  Fps& operator/=(const Fps& r) {
    return *this *= r.inv();
  }
  Fps& operator%=(const Fps& r) {
    normalize();
    Fps q = *this / r;
    return *this -= q * r;
  }
  Fps operator+(const Fps& r) const { return Fps(*this) += r; }
  Fps operator-(const Fps& r) const { return Fps(*this) -= r; }
  Fps operator*(const Fps& r) const { return Fps(*this) *= r; }
  Fps operator/(const Fps& r) const { return Fps(*this) /= r; }
  Fps operator%(const Fps& r) const { return Fps(*this) %= r; }
};
