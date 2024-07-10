#pragma once
#include "fft.hpp"
#include "ntt.hpp"

const int CONVOLUTION_NAIVE_THRESHOLD = 60;

template<class T>
std::vector<T> convolution_naive(const std::vector<T>& f, const std::vector<T>& g) {
  auto n = f.size(), m = g.size();
  if (n == 0 or m == 0)
    return {};
  std::vector<T> h(n+m-1);
  if (n < m) {
    for (size_t j = 0; j < m; j++)
      for (size_t i = 0; i < n; i++)
        h[i+j] += f[i] * g[j];
  } else {
    for (size_t i = 0; i < n; i++)
      for (size_t j = 0; j < m; j++)
        h[i+j] += f[i] * g[j];
  }
  return h;
}

template<class T>
std::vector<T> convolution_fft(std::vector<T> f, std::vector<T> g) {
  auto n = f.size(), m = g.size();
  size_t len = n + m - 1;
  size_t l = 1 << math::ceil_pow2(len);
  if (n+m-3 < l/2) {
    auto fb = f.back(), gb = g.back();
    f.pop_back(); g.pop_back();
    std::vector<T> h(n+m-1, 0);
    h[n+m-2] = fb * gb;
    for (size_t i = 0; i < n; i++)
      h[i + m-1] += f[i] * gb;
    for (size_t i = 0; i < m; i++)
      h[i + n-1] += g[i] * fb;
    auto fg = convolution(std::move(f), std::move(g));
    for (size_t i = 0; i < n+m-3; i++)
      h[i] += fg[i];
    return h;
  }
  Fft fft(len);
  f.resize(fft.n(), 0);
  g.resize(fft.n(), 0);
  auto same = f == g;
  fft.fft_inline(f);
  if (same) {
    for (size_t i = 0; i < fft.n(); i++)
      f[i] *= f[i];
  } else {
    fft.fft_inline(g);
    for (size_t i = 0; i < fft.n(); i++)
      f[i] *= g[i];
  }
  fft.ifft_inline(f);
  std::vector<T> res(len);
  for (size_t i = 0; i < len; i++)
    res[i] = f[i].real();
  return res;
}

template<int Mod>
std::vector<Modular<Mod>> convolution_ntt(std::vector<Modular<Mod>> f, std::vector<Modular<Mod>> g) {
  auto n = f.size(), m = g.size();
  size_t l = 1 << math::ceil_pow2(n + m - 1);
  if (n+m-3 < l/2) {
    auto fb = f.back(), gb = g.back();
    f.pop_back(); g.pop_back();
    std::vector<Modular<Mod>> h(n+m-1, 0);
    h[n+m-2] = fb * gb;
    for (size_t i = 0; i < n-1; i++)
      h[i + m-1] += f[i] * gb;
    for (size_t i = 0; i < m-1; i++)
      h[i + n-1] += g[i] * fb;
    auto fg = convolution(std::move(f), std::move(g));
    for (size_t i = 0; i < fg.size(); i++)
      h[i] += fg[i];
    return h;
  }
  f.resize(l, 0);
  g.resize(l, 0);
  bool same = f == g;
  ntt_convolution_inline(f);
  if (same) {
    for (size_t i = 0; i < l; i++) 
      f[i] *= f[i];
  } else {
    ntt_convolution_inline(g);
    for (size_t i = 0; i < l; i++) 
      f[i] *= g[i];
  }
  intt_convolution_inline(f);
  return f;
}

template<int Mod>
std::vector<Modular<Mod>> convolution_fft(std::vector<Modular<Mod>> f, std::vector<Modular<Mod>> g) {
  return convolution_ntt(std::move(f), std::move(g));
}

std::vector<int> convolution_fft(const std::vector<int>& a, const std::vector<int>& b) {
  auto n = a.size(), m = b.size();
  std::vector<Modular998244353> f(n), g(m);
  for (size_t i = 0; i < n; i++)
    f[i] = a[i];
  for (size_t i = 0; i < m; i++)
    g[i] = b[i];
  auto h = convolution_fft(std::move(f), std::move(g));
  std::vector<int> res(n+m-1);
  for (size_t i = 0; i < n+m-1; i++)
    res[i] = h[i].val();
  return res;
}

template<class T>
std::vector<T> convolution(const std::vector<T>& f, const std::vector<T>& g) {
  auto n = f.size(), m = g.size();
  if (n == 0 or m == 0)
    return {};
  if (std::min(n, m) <= CONVOLUTION_NAIVE_THRESHOLD)
    return convolution_naive(f, g);
  return convolution_fft(f, g);
}

template<class T>
std::vector<T> convolution(std::vector<T>&& f, std::vector<T>&& g) {
  auto n = f.size(), m = g.size();
  if (n == 0 or m == 0)
    return {};
  if (std::min(n, m) <= CONVOLUTION_NAIVE_THRESHOLD)
    return convolution_naive(f, g);
  return convolution_fft(std::move(f), std::move(g));
}

template<class mint>
std::vector<mint> convolution_garner(const std::vector<mint>& f, 
                                     const std::vector<mint>& g) {
  auto n = f.size();
  auto m = g.size();
  if (n == 0 or m == 0)
    return {};
  constexpr long long nttprimes[] = {754974721, 167772161, 469762049};
  using mint0 = Modular<754974721>;
  using mint1 = Modular<167772161>;
  using mint2 = Modular<469762049>;
  std::vector<mint0> a0(n), b0(m);
  std::vector<mint1> a1(n), b1(m);
  std::vector<mint2> a2(n), b2(m);
  for(size_t i = 0; i < n; i++) a0[i] = f[i].val(), a1[i] = f[i].val(), a2[i] = f[i].val();
  for(size_t i = 0; i < m; i++) b0[i] = g[i].val(), b1[i] = g[i].val(), b2[i] = g[i].val();
  auto c0 = convolution_ntt(a0, b0);
  auto c1 = convolution_ntt(a1, b1);
  auto c2 = convolution_ntt(a2, b2);
  constexpr long long m01 = (long long) nttprimes[0] * nttprimes[1];
  constexpr long long m0_inv_m1 = mint1(nttprimes[0]).inv().val();
  constexpr long long m01_inv_m2 = mint2(m01).inv().val();
  const int mod = mint::mod();
  auto garner = [&](mint0 x0, mint1 x1, mint2 x2) -> mint {
    int r0 = x0.val(), r1 = x1.val(), r2 = x2.val();
    int v1 = (m0_inv_m1 * (r1 + nttprimes[1] - r0)) % nttprimes[1];
    auto v2 = (mint2(r2) - r0 - mint2(nttprimes[0]) * v1) * mint2(m01_inv_m2);
    return mint(r0 + 1LL * nttprimes[0] * v1 + m01 % mod * v2.val());
  };
  std::vector<mint> c(c0.size());
  for(size_t i = 0; i < c.size(); i++) c[i] = garner(c0[i], c1[i], c2[i]);
  return c;
}

template<int Mod>
std::vector<Modular<Mod>> convolution_large(const std::vector<Modular<Mod>>& f, 
                                            const std::vector<Modular<Mod>>& g) {
  auto n = f.size();
  auto m = g.size();
  if (n == 0 or m == 0)
    return {};
  constexpr auto h = bm::ctz(Mod-1);
  if (n+m-1 < 1u<<h)
    return convolution(f ,g);
  size_t l = h-1;
  int cs = ((n-1)>>l)+1;
  int ds = ((m-1)>>l)+1;
  using result_type = std::vector<Modular<Mod>>;
  std::vector<result_type> c(cs, result_type(2<<l));
  std::vector<result_type> d(ds, result_type(2<<l));
  size_t mask = (1<<l)-1;
  for (size_t i = 0; i < n; i++)
    c[i>>l][i&mask] = f[i];
  for (size_t i = 0; i < m; i++)
    d[i>>l][i&mask] = g[i];
  for (auto& v: c) ntt_convolution_inline(v);
  for (auto& v: d) ntt_convolution_inline(v);
  result_type res(n+m-1);
  result_type e(2<<l);
  for (int i = 0; i < cs+ds-1; i++) {
    e.assign(2<<l, 0);
    for (int j = std::max(i-ds+1, 0); j < std::min(cs, i+1); j++) {
      int k = i-j;
      assert(k >= 0 and k < ds);
      for (int x = 0; x < 2<<l; x++)
        e[x] += c[j][x] * d[k][x];
    }
    intt_convolution_inline(e);
    auto len = std::min((size_t)2<<l, n+m-1-(i<<l));
    for (size_t x = 0; x < len; x++)
      res[(i<<l) + x] += e[x];
  }
  return res;
}

size_t complexity_of_convolution(size_t fsize, size_t gsize) {
  int lg = math::ceil_pow2(fsize + gsize - 1);
  return lg << lg;
}