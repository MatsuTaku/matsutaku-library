#pragma once
#include "ntt.hpp"
#include <vector>
#include <initializer_list>
#include <cassert>
#include <algorithm>

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
  Fps& inline_pre(size_t m) {
    _base::resize(m, 0);
    return *this;
  }
  /// f mod x^m
  Fps pre(size_t m) const {
    if (m <= _base::size())
      return Fps(_base::begin(), _base::begin()+m);
    else 
      return Fps(*this).inline_pre(m);
  }
  bool operator==(const Fps& rhs) const {
    for (size_t i = 0, n = std::max(_base::size(), rhs.size()); i < n; i++) {
      auto l = i < _base::size() ? (*this)[i] : 0;
      auto r = i < rhs.size() ? rhs[i] : 0;
      if (l != r) return false;
    }
    return true;
  }
  bool operator!=(const Fps& rhs) const {
    return !(*this == rhs);
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
  friend Fps operator+(mint x, const Fps& y) { return y+x; }
  friend Fps operator-(mint x, const Fps& y) { return -y+x; }
  friend Fps operator*(mint x, const Fps& y) { return y*x; }
  friend Fps operator/(mint x, const Fps& y) { return y.inv()*x; }
  Fps& operator+=(const Fps& r) {
    if (r.size() > _base::size())
      _base::resize(r.size());
    for (size_t i = 0; i < r.size(); i++)
      (*this)[i] += r[i];
    return *this;
  }
  Fps& operator-=(const Fps& r) {
    if (r.size() > _base::size())
      _base::resize(r.size());
    for (size_t i = 0; i < r.size(); i++)
      (*this)[i] -= r[i];
    return *this;
  }
  Fps& dot(const Fps& r) {
    if (_base::size() > r.size())
      _base::resize(r.size());
    for (size_t i = 0; i < _base::size(); i++)
      (*this)[i] *= r[i];
    return *this;
  }
  size_t count_terms(int n = -1) const {
    if (n == -1) n = (int)_base::size();
    n = std::min(n, (int)_base::size());
    return std::count_if(_base::begin(), _base::begin()+n, [](mint x) { return x != 0; });
  }
  std::vector<std::pair<size_t, mint>> term_ties() const {
    return term_ties(0, _base::size());
  }
  std::vector<std::pair<size_t, mint>> term_ties(size_t n) const {
    return term_ties(0, n);
  }
  std::vector<std::pair<size_t, mint>> term_ties(size_t front, size_t back) const {
    std::vector<std::pair<size_t, mint>> ret;
    for (size_t i = front; i < back; i++)
      if ((*this)[i] != 0)
        ret.emplace_back(i, (*this)[i]);
    return ret;
  }
 private:
  template<class F>
  Fps& _mul_set_dense(F&& r) {
    return *this = Fps(convolution(std::move(*this), std::forward<F>(r)));
  }
  /** 
   * Complexity: O(NK) 
   *             where N is count of non-zero terms of self and K is count of non-zero terms of r
  */
  Fps& _mul_set_sparse(const Fps& r) {
    auto ri = r.term_ties();
    if (ri.empty()) return *this = Fps();
    Fps ret(_base::size() + ri.back().first);
    for (size_t i = 0; i < _base::size(); i++) if ((*this)[i] != 0) for (auto j:ri) {
      ret[i + j.first] += (*this)[i] * j.second;
    }
    return *this = ret;
  }
  template<class F>
  Fps& _mul_set(F&& r) {
    return
      r.count_terms() < 100 ?
      _mul_set_sparse(std::forward<F>(r)) :
      _mul_set_dense(std::forward<F>(r));
  }
 public:
  Fps& operator*=(const Fps& r) {
    return _mul_set(r);
  }
  Fps& operator*=(Fps&& r) {
    return _mul_set(std::move(r));
  }
  Fps inv_dense(int n = -1) const {
    assert(!_base::empty() and (*this)[0] != 0);
    if (n == -1) n = (int) _base::size();
    if (n == 0) return Fps();
    // Newton descent
    // find g, s.t. F(g) = a
    //   g_{n+1} = g_n - (F(g_n) - a) / F'(g_n)
    // find g, s.t. g^{-1} = f
    //   g_{n+1} = g_n - (g_n^{-1} - f) / -g_n^{-2}
    //           = 2g_n - g_n^2 f
    //   g_0 = f_0^{-1}
    Fps g,fm,fgg;
    g.reserve(n); fm.reserve(n); fgg.reserve(n*2-1);
    g.push_back((*this)[0].inv());
    fm.push_back((*this)[0]);
    for (int m = 1; m < n; m <<= 1) {
      int nm = std::min(m*2, n);
      fm.resize(nm);
      for (int i = m; i < std::min(nm, (int)_base::size()); i++)
        fm[i] = (*this)[i];
      fgg = g;
      fgg *= g;
      fgg *= fm;
      fgg.inline_pre(nm);
      g.resize(nm);
      for (int i = m; i < nm; i++)
        g[i] = -fgg[i];
    }
    // assert((pre(n)*g).pre(n) == Fps{1});
    return g;
  }
  Fps inv_sparse(int n = -1) const {
    assert(!_base::empty() and (*this)[0] != 0);
    if (n == -1) n = (int) _base::size();
    if (n == 0) return Fps();
    // fg = 1 => (n>0) sum_i f_i g_{n-i} = 0
    // f_0 g_n = - sum_{i=1}^n f_i g_{n-i}
    auto tf = term_ties(n);
    Fps g(n);
    auto ifz = (*this)[0].inv();
    g[0] = ifz;
    for (size_t i = 1; i < (size_t) n; i++) {
      mint s = 0;
      for (size_t j = 1; j < tf.size(); j++) {
        if (tf[j].first > i) break;
        s += tf[j].second * g[i-tf[j].first];
      }
      g[i] = -s * ifz;
    }
    // assert((pre(n)*g).pre(n) == Fps{1});
    return g;
  }
  Fps inv(int n = -1) const {
    if (n == -1) n = (int) _base::size();
    return count_terms(n) < 100 ? inv_sparse(n) : inv_dense(n);
  }
  Fps& operator/=(const Fps& r) {
    return *this *= r.inv();
  }
  // f x^m = sum f_i x^{i+m}
  Fps operator<<(size_t m) const {
    Fps ret(m + _base::size());
    std::copy(_base::begin(), _base::end(), ret.begin()+m);
    return ret;
  }
  // sum f_{i+m} x^i
  Fps operator>>(size_t m) const {
    if (m >= _base::size()) return Fps();
    return Fps(_base::begin()+m, _base::end());
  }
  Fps& operator<<=(size_t m) {
    size_t s = _base::size();
    _base::resize(m + s);
    for (size_t i = 0; i < s; i++)
      (*this)[m+s-1-i] = (*this)[s-1-i];
    std::fill(_base::begin(), _base::begin()+m, 0);
    return *this;
  }
  Fps& operator>>=(size_t m) {
    if (m >= _base::size()) return *this = Fps();
    _base::erase(_base::begin(), _base::begin()+m);
    return *this;
  }
  Fps& inline_diff() {
    if (_base::empty()) return *this;
    for (size_t i = 1; i < _base::size(); i++)
      (*this)[i-1] = (*this)[i] * i;
    _base::pop_back();
    return *this;
  }
  Fps diff() const {
    return Fps(*this).inline_diff();
  }
  Fps& inline_inte() {
    if (_base::empty()) return *this;
    _base::push_back(0);
    ModularUtil<mint> mu;
    mu.set_inv(_base::size());
    for (int i = (int) _base::size()-1; i > 0; i--)
      (*this)[i] = (*this)[i-1] * mu.inv(i);
    (*this)[0] = 0;
    return *this;
  }
  Fps inte() const {
    return Fps(*this).inline_inte();
  }
  Fps log_dense(int n = -1) const {
    assert(!_base::empty() and _base::operator[](0) == 1);
    if (n == -1) n = (int) _base::size();
    // integral(f' / f)
    return (diff() * inv_dense(n-1)).inline_pre(n-1).inline_inte().inline_pre(n);
  }
  Fps log_sparse(int n = -1) const {
    assert(!_base::empty() and _base::operator[](0) == 1);
    if (n == -1) n = (int) _base::size();
    // integral(f' / f)
    return (diff() * inv_sparse(n-1)).inline_pre(n-1).inline_inte().inline_pre(n);
  }
  /**
   * define log (1-f) = -sum_n f^n / n
   * satisfy log(f)' = f'/f, log(fg) = log f + log g
  */
  Fps log(int n = -1) const {
    assert(!_base::empty() and _base::operator[](0) == 1);
    if (n == -1) n = (int) _base::size();
    // integral(f' / f)
    return (diff() * inv(n-1)).inline_pre(n-1).inline_inte().inline_pre(n);
  }
  Fps exp_dense(int n = -1) const;
  Fps exp_sparse(int n = -1) const;
  /**
   * define exp f = sum_n f^n / n!
   * satisfy (exp f)' = (exp f)f', exp(f + g) = exp(f)exp(g)
  */
  Fps exp(int n = -1) const {
    assert(_base::empty() or _base::operator[](0) == 0);
    return count_terms() < 100 ? exp_sparse(n) : exp_dense(n);
  }
 private:
  Fps _pow_1_dense(long long n) const;
  Fps _pow_1_sparse(long long n) const;
  Fps _pow_1(long long n) const {
    assert(!_base::empty() and _base::operator[](0) == 1);
    return count_terms() < 100 ? _pow_1_sparse(n) : _pow_1_dense(n);
  }
 public:
  /**
   * f^n = exp(n log f)
   * f^{ab} = (f^a)^b, f^{a+b} = f^a f^b
  */
  Fps pow(long long n) const;
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
