#pragma once
#include "modular.hpp"
#include "bit_manip.hpp"
#include <vector>
#include <cassert>

namespace _ntt {

static constexpr int P = 998244353;
static constexpr int MAX_DIV = 23;
using mint = Modular<P>;
static const mint PRIMITIVE_ROOT = 3;

bool _ntt_initialized = false;
std::vector<mint> _coeff;
std::vector<mint> _icoeff;

void _ntt_initialize() {
  if (_ntt_initialized)
    return;
  _ntt_initialized = true;
  int log_n_ = MAX_DIV;
  mint r_ = pow(PRIMITIVE_ROOT, (P-1) >> MAX_DIV);
  _coeff.resize(log_n_);
  _icoeff.resize(log_n_);
  mint w = r_;
  mint iw = w.inv();
  for (int i = log_n_-1; i >= 0; i--) {
    _coeff[i] = iw;
    iw *= iw;
    _icoeff[i] = w;
    w *= w;
  }
}

template <bool Forward>
void _fft_impl(std::vector<mint>& f) {
  _ntt_initialize();
  int log_n = 64-bm::clz(f.size()-1);
  int n = 1 << log_n;
  f.resize(n, 0);
  // iterative bit reversal
  for (int i = 0, j = 1; j < n-1; j++) {
    for (int k = n >> 1; k > (i ^= k); k >>= 1);
    if (i < j) std::swap(f[i], f[j]);
  }
  // Cooley-Tukey FFT
  for (int log_m = 0; log_m < log_n; log_m++) {
    int m = 1<<log_m;
    mint w0 = Forward ? _coeff[log_m] : _icoeff[log_m];
    for (int chunk = 0; chunk < n; chunk += 2*m) {
      mint w = 1;
      for (int i = 0; i < m; i++) {
        auto p = chunk + i;
        auto a = f[p + 0];
        auto b = f[p + m] * w;
        f[p + 0] = a + b;
        f[p + m] = a - b;
        w *= w0;
      }
    }
  }
}
void _fft(std::vector<mint>& f) { _fft_impl<true>(f); }
void _ifft(std::vector<mint>& f) { _fft_impl<false>(f); }

}

std::vector<_ntt::mint>& ntt_inline(std::vector<_ntt::mint>& f) {
  _ntt::_fft(f);
  return f;
}

std::vector<_ntt::mint> ntt(std::vector<_ntt::mint> f) {
  _ntt::_fft(f);
  return f;
}

std::vector<_ntt::mint>& intt_inline(std::vector<_ntt::mint>& f) {
  _ntt::_ifft(f);
  return f;
}

std::vector<_ntt::mint> intt(std::vector<_ntt::mint> f) {
  _ntt::_ifft(f);
  return f;
}

std::vector<_ntt::mint> convolution(std::vector<_ntt::mint> f, std::vector<_ntt::mint> g) {
  int l = f.size() + g.size() - 1;
  f.resize(l);
  ntt_inline(f);
  g.resize(l);
  ntt_inline(g);
  int n = f.size();
  for (int i = 0; i < n; i++)
    f[i] *= g[i];
  intt_inline(f);
  auto n_inv = _ntt::mint(n).inv();
  for (int i = 0; i < n; i++)
    f[i] *= n_inv;
  return f;
}
