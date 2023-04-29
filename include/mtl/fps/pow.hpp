#pragma once
#include "../fps/exp.hpp"

template<int M>
Fps<M> Fps<M>::_pow_1_dense(long long n) const {
    return (log_dense() * n).exp_dense();
}

template<int M>
Fps<M> Fps<M>::_pow_1_sparse(long long n) const {
    // F = f^n
    // fF' = NFf'
    // sum_i f_i F'_{n-i} = N sum_i F_{n-i} f'_i
    // F'_n = -sum_{i>0} f_i F'_{n-i} + N sum_i F_{n-i} f'_i
    // F'_n = -sum_{i>0} f_i F_{n-i+1} (n-i+1) + N sum_{i>0} F_{n-i+1} if_i
    auto tf = term_ties(1, _base::size());
    if (tf.empty()) return Fps{1};
    Fps ret(_base::size());
    ret[0] = 1;
    ModularUtil<mint> mu;
    mu.set_inv(_base::size()-1);
    mint N = n;
    for (size_t i = 1; i < _base::size(); i++) {
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