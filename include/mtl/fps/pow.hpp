#pragma once
#include "../fps/exp.hpp"

template<int M>
Fps<M> Fps<M>::_pow_1_dense(long long n) const {
    return (log_dense() * n).exp_dense();
}

template<int M>
Fps<M> pow_1_sparse_impl(const SparseFps<M>& tf, long long n, int deg) {
    // F = f^n
    // fF' = NFf'
    // sum_i f_i F'_{n-i} = N sum_i F_{n-i} f'_i
    // F'_n = -sum_{i>0} f_i F'_{n-i} + N sum_i F_{n-i} f'_i
    // F'_n = -sum_{i>0} f_i F_{n-i+1} (n-i+1) + N sum_{i>0} F_{n-i+1} if_i
    if (tf.empty()) return Fps<M>();
    Fps<M> ret(deg);
    ret[0] = 1;
    using mint = typename Fps<M>::mint;
    ModularUtil<mint> mu;
    mu.set_inv(deg-1);
    mint N = n;
    for (size_t i = 1; i < (size_t)deg; i++) {
      mint f = 0;
      for (auto& t:tf) {
        if (i < t.first) break;
        auto j = i - t.first;
        auto c = t.second * ret[j];
        f += c * (N * t.first - j);
      }
      ret[i] = f * mu.inv(i);
    }
    return ret;
}

template<int M>
Fps<M> Fps<M>::_pow_1_sparse(long long n) const {
  return pow_1_sparse_impl(SparseFps<M>(*this), n, this->size());
}

template<int M>
Fps<M> Fps<M>::pow(long long n) const {
    // f = sum c_i x^i = c_k x^k sum c_{k+i}/c_k x^i = c_k x^k g
    // [x^0]g = 1
    // f^n = c_k^n x^{kn} g^n
    if (n == 0) 
      return Fps{1};
    size_t k = 0;
    for (; k < _base::size(); k++)
      if ((*this)[k] != 0) break;
    if (k == _base::size() or
        (k and n >= (long long)((_base::size()-1)/k+1))) 
      return Fps();
    if ((*this)[0] == 1)
      return _pow_1(n);
    auto c = (*this)[k];
    auto ic = c.inv();
    Fps g(_base::size()-k);
    for (size_t i = k; i < _base::size(); i++)
      g[i-k] = (*this)[i] * ic;
    g = g._pow_1(n);
    auto d = k * n;
    Fps res(d+g.size());
    auto cpn = c.pow(n);
    for (size_t i = 0; i < g.size(); i++)
      res[d+i] = g[i] * cpn;
    return res;
}

template<int M>
Fps<M> SparseFps<M>::pow(long long n, int degrees) const {
    auto pow_1 = [&](const SparseFps<M>& f) {
      return pow_1_sparse_impl(f, n, degrees);
    };
    // f = sum c_i x^i = c_k x^k sum c_{k+i}/c_k x^i = c_k x^k g
    // [x^0]g = 1
    // f^n = c_k^n x^{kn} g^n
    if (n == 0) 
      return Fps<M>{1};
    size_t k = 0, ki = 0;;
    for (; ki < this->size(); ki++)
      if ((*this)[ki].second != 0) {
        k = (*this)[ki].first;
        break;
      }
    if (ki == this->size() or
        (k and n >= (long long)((degrees+k-1)/k))) 
      return Fps<M>();
    auto c = (*this)[ki].second;
    if (k == 0 and c == 1)
      return pow_1(*this);
    auto ic = c.inv();
    // g = f / x^k / [x^k]f
    SparseFps g = *this;
    for (auto& x : g) {
      x.first -= k;
      x.second *= ic;
    }
    auto gpn = pow_1(g);
    auto d = k * n;
    fps_type res(d+gpn.size());
    auto cpn = c.pow(n);
    for (size_t i = 0; i < gpn.size(); i++)
      res[d+i] = gpn[i] * cpn;
    return res.inline_pre(degrees);
}