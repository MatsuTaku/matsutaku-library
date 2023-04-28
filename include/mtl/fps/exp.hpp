#pragma once
#include "../fps.hpp"

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
    f.reserve(n*2-1);
    g.reserve(n-1);
    t.reserve(n*2-1);
    q.reserve(n-1);
    f.push_back(1);
    g.push_back(1);
    auto term = [&](size_t i) {
      return i < this->size() ? this->operator[](i) : 0;
    };
    for (size_t i = 1; i < (size_t)n; i *= 2) {
      // std::cerr<<"["<<i<<"] "<<std::endl; 
      auto ni = i*2;
      // t = (f*g*g).pre(i)
      t = g;
      t *= g;
      t *= f;
      t.inline_pre(i);
    //   assert(t == (f*g*g).pre(i));
      // g = (2*g-f*g*g).pre(i)
      g *= 2;
      g -= t;
    //   assert(g == (2*g-f*g*g).pre(i));
      // q = pre(i).diff()
      q.resize(i-1);
      for (size_t j = std::max((size_t)1, i/2); j < i; j++)
        q[j-1] = term(j) * j;
    //   assert(q == pre(i).diff());
      // t = (f*q).pre(ni-1);
      t = f;
      t *= q;
      t.inline_pre(ni-1);
    //   assert(t == (f*q).pre(ni-1));
      // t = (f.diff()-f*q).pre(ni-1)
      t *= -1;
      for (size_t j = 1; j < i; j++)
        t[j-1] += f[j] * j;
    //   assert(t == (f.diff()-(f*q)).pre(ni-1));
      // t = (g*(f.diff()-f*q)).pre(ni-1)
      t *= g;
      t.inline_pre(ni-1);
    //   assert(t == (g*(f.diff()-f*q)).pre(ni-1));
      // t = (q + g*(f.diff()-f*q)).pre(ni-1).inte()
      t += q;
      t.inline_inte();
    //   assert(t == (q + g*(f.diff()-f*q)).pre(ni-1).inte());
    //   assert(t == f.log(ni));
      // t = pre(ni)-t
      for (size_t j = 0; j < ni; j++)
        t[j] = -t[j] + term(j);
      // f = f(1+pre(ni)-t).pre(ni)
      auto pf = f;
      t += 1;
      f *= t;
      f.inline_pre(ni);
    //   assert(f == (pf*t).pre(ni));
      // For exact computation
      // f = f + f*(pre(i*2) - f.log(i*2));
      // f.inline_pre(ni);
    }
    return f.inline_pre(n);
}

template<int M>
Fps<M> Fps<M>::exp_sparse(int n) const {
    assert(this->empty() or this->operator[](0) == 0);
    if (n == -1) n = (int) this->size();
    if (n == 0) return Fps();
    if (this->empty()) return Fps(n);
    // F = exp f
    // F' = Ff'
    // F'_{n-1} = sum_{i=1}^{n-1} F_i f'_{n-i-1}, F_0 = 0;
    auto td = pre(std::min(n, (int)this->size())).diff().term_ties();
    Fps<M> ret(n);
    ret[0] = 1;
    for (int i = 1; i < n; i++) {
      Fps<M>::mint pf = 0;
      for (auto& t:td) {
        if (t.first > i-1) break;
        pf += ret[i-1-t.first] * t.second;
      }
      ret[i] = pf / i;
    }
    return ret;
}