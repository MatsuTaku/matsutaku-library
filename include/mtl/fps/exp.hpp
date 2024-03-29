#pragma once
#include "../fps.hpp"

/**
 * We adopt version2 of the exponential computation algorithm in [1].
 * reference:
 *  [1] Alin Bostan and Eric Schost, A simple and fast algorithm for computing exponentials of power series, Information Processing Letters 109, 13 (2009) 754-756
 *      https://doi.org/10.1016/j.ipl.2009.03.012
*/
template<int M>
Fps<M> Fps<M>::exp_dense(int n) const {
    // Newton descent
    // find g, s.t. F(g) = a
    //   g_{n+1} = g_n - (F(g_n) - a) / F'(g_n)
    // find g, s.t. g = exp f => log g = f
    //   g_{n+1} = g_n - (log g_n - f) / g_n^{-1}
    //           = g_n + g_n (f - log g_n)
    //           = g_n + g_n (f - inte(h' + (g_n' - g_nh')g_n^{-1}))
    //   g_0 = 1
    assert(this->empty() or this->operator[](0) == 0);
    if (n == -1) n = (int) this->size();
    if (n == 0) return Fps();
    Fps<M> f, g, t, q;
    // f = (exp h)_0 = {1}
    // g = (exp h)^{-1}_0 = {1}
    // q = h' = {}
    f.reserve(n*2-1);
    f.push_back(1);
    g.reserve(n-1);
    g.push_back(1); 
    t.reserve(n*2-1);
    q.reserve(n-1); 
    auto term = [&](size_t i) {
      return i < this->size() ? this->operator[](i) : 0;
    };
    ModularUtil<mint> mu;
    mu.set_inv(n);
    for (size_t i = 1; i < (size_t)n; i *= 2) {
      /*
      Exact computation
      f = f + f*(pre(i*2) - f.log(i*2));
      f.inline_pre(ni);
      */
      if (i > 1) {
        // t = (f*g*g).pre(i)
        t = g;
        t *= g;
        t *= f;
        t.inline_pre(i);
        // g = (2*g-f*g*g).pre(i)
        g.inline_pre(i);
        for (size_t j = i/2; j < i; j++)
          g[j] = -t[j];
        // q = pre(i).diff()
        q.resize(i-1);
        for (size_t j = i/2; j < i; j++)
          q[j-1] = term(j) * j;
      }
      auto ni = i*2;
      auto l = std::min(ni, (size_t) n);
      // t = fq % x^{2i-1}
      t = f;
      t *= q;
      t.inline_pre(l-1);
      // t = fqg div x^{i-1}
      t >>= i-1;
      t *= g;
      t.inline_pre(l-i+1);
      // t = (h + (t x^{i-1}).inte()) div x^m
      for (size_t j = 0; j < l-i+1; j++)
        t[j] = t[j] * mu.inv(i+j) + term(i+j);
      t *= f;
      t.inline_pre(l-i);
      f.inline_pre(l);
      for (size_t j = i; j < l; j++)
        f[j] = t[j-i];
    }
    return f.inline_pre(n);
}

#include "../sparse_fps.hpp"

template<int M>
Fps<M> exp_sparse_impl(const SparseFps<M>& td, int n) {
    // F = exp f
    // F' = Ff'
    // F'_{n-1} = sum_{i=1}^{n-1} F_i f'_{n-i-1}, F_0 = 0;
    Fps<M> ret(n);
    ret[0] = 1;
    using mint = typename Fps<M>::mint;
    ModularUtil<mint> mu;
    mu.set_inv(n-1);
    for (int i = 1; i < n; i++) {
      mint pf = 0;
      for (auto& t:td) {
        if (t.first == 0) continue;
        int d = t.first-1;
        auto c = t.second * t.first;
        if (d > i-1) break;
        pf += ret[i-1-d] * c;
      }
      ret[i] = pf * mu.inv(i);
    }
    return ret;
}

template<int M>
Fps<M> Fps<M>::exp_sparse(int n) const {
    assert(this->empty() or this->operator[](0) == 0);
    if (n == -1) n = (int) this->size();
    if (n == 0) return Fps();
    if (this->empty()) return Fps{1};
    return exp_sparse_impl(SparseFps<M>(*this, std::min(n, (int)this->size())), n);
}

template<int M>
Fps<M> SparseFps<M>::exp(int deg) const {
    assert(this->empty() or this->front().first != 0 or this->front().second == 0);
    if (deg == 0) return fps_type();
    if (this->empty()) return fps_type{1};
    return exp_sparse_impl(*this, deg);
}