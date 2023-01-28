#pragma once
#include "modular.hpp"
#include "bit_manip.hpp"
#include <vector>
#include <cassert>

namespace math {

constexpr int ceil_pow2(unsigned long long x) {
  return x == 0 ? 0 : 64-bm::clz(x-1);
}

}

namespace _ntt {

template<int mod>
struct ntt_info {
  using mint = Modular<mod>;
  static constexpr int primitive_root() {
    return math::primitive_root<mod>;
  }
  static constexpr int log_n() {
    return bm::ctz(mod-1);
  }
  static constexpr std::array<Modular<mod>, log_n()> coeff(bool forward) {
    mint r = mint(primitive_root()).pow((mod-1) >> log_n());
    std::array<mint, log_n()> coeff{};
    mint iw = forward ? r.inv() : r;
    for (int i = log_n()-1; i >= 0; i--) {
      coeff[i] = iw;
      iw *= iw;
    }
    return coeff;
  }
};

template <bool Forward, int mod>
void _fft_impl(std::vector<Modular<mod>>& f) {
  using mint = Modular<mod>;
  int log_n = math::ceil_pow2(f.size());
  using info = ntt_info<mod>;
  assert(info::log_n() >= log_n);
  int n = 1 << log_n;
  f.resize(n, 0);
  // iterative bit reversal
  for (int i = 0, j = 1; j < n-1; j++) {
    for (int k = n >> 1; k > (i ^= k); k >>= 1);
    if (i < j) std::swap(f[i], f[j]);
  }
  // Cooley-Tukey FFT
  static constexpr auto coeff = info::coeff(Forward);
  for (int log_m = 0; log_m < log_n; log_m++) {
    int m = 1<<log_m;
    mint w0 = coeff[log_m];
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
template<class mint>
void _fft(std::vector<mint>& f) { _fft_impl<true>(f); }
template<class mint>
void _ifft(std::vector<mint>& f) { _fft_impl<false>(f); }

}

template<class mint>
void ntt_inline(std::vector<mint>& f) {
  _ntt::_fft(f);
}

template<class mint>
std::vector<mint> ntt(std::vector<mint> f) {
  _ntt::_fft(f);
  return f;
}

template<class mint>
void intt_inline(std::vector<mint>& f) {
  _ntt::_ifft(f);
}

template<class mint>
std::vector<mint> intt(std::vector<mint> f) {
  _ntt::_ifft(f);
  return f;
}

template<class mint>
std::vector<mint> convolution(std::vector<mint> f, std::vector<mint> g) {
  if (f.empty() or g.empty())
    return {};
  int l = 1 << math::ceil_pow2(f.size() + g.size() - 1);
  f.resize(l);
  ntt_inline(f);
  g.resize(l);
  ntt_inline(g);
  int n = f.size();
  for (int i = 0; i < n; i++)
    f[i] *= g[i];
  intt_inline(f);
  auto n_inv = mint(n).inv();
  for (int i = 0; i < n; i++)
    f[i] *= n_inv;
  return f;
}
