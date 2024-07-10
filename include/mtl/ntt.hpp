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

#ifndef MTL_ARRAY_SET_CONSTEXPR
#if __cplusplus >= 201703L
#define MTL_ARRAY_SET_CONSTEXPR constexpr
#else
#define MTL_ARRAY_SET_CONSTEXPR
#endif
#endif

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
  static MTL_ARRAY_SET_CONSTEXPR
  std::array<Modular<mod>, log_n()> coeff(bool forward) {
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

template<class T>
void _iterative_bit_reverse(std::vector<T>& f) {
  int log_n = math::ceil_pow2(f.size());
  int n = 1 << log_n;
  for (int i = 0, j = 1; j < n-1; j++) {
    for (int k = n >> 1; k > (i ^= k); k >>= 1);
    if (i < j) std::swap(f[i], f[j]);
  }
}

template <bool Forward, bool BitReverse, int mod>
void _fft_impl(std::vector<Modular<mod>>& f) {
  using mint = Modular<mod>;
  int log_n = math::ceil_pow2(f.size());
  using info = ntt_info<mod>;
  assert(info::log_n() >= log_n);
  int n = 1 << log_n;
  f.resize(n, 0);
  if constexpr (!Forward and BitReverse)
    _iterative_bit_reverse(f);
  // Cooley-Tukey FFT
  static MTL_ARRAY_SET_CONSTEXPR auto coeff = info::coeff(Forward);
  if constexpr (Forward) {
    for (int log_m = log_n-1; log_m >= 0; log_m -= 2) {
      int m = 1<<log_m;
      mint w0 = coeff[log_m];
      if (log_m == 0) {
        for (int chunk = 0; chunk < n; chunk += 2*m) {
          mint w = 1;
          for (int i = 0; i < m; i++) {
            auto p = chunk + i;
            auto a = f[p + 0];
            auto b = f[p + m];
            f[p + 0] = (a + b);
            f[p + m] = (a - b) * w;
            w *= w0;
          }
        }
      } else { // 4-base
        auto w1 = coeff[log_m-1];
        auto wh = w0.pow(m/2).val();
        for (int chunk = 0; chunk < n; chunk += 2*m) {
          mint w = 1, x = 1;
          for (int i = 0; i < m/2; i++) {
            auto p = chunk + i;
            auto ia = p + 0*m/2;
            auto ib = p + 1*m/2;
            auto ic = p + 2*m/2;
            auto id = p + 3*m/2;
            long long a = f[ia].val(), b = f[ib].val(), c = f[ic].val(), d = f[id].val();
            auto s = a + c, t = b + d, u = (a - c + mod) * w.val() % mod, v = (b - d + mod) * w.val() % mod * wh % mod;
            f[ia] = (s + t);
            f[ib] = (s - t) * x.val();
            f[ic] = (u + v);
            f[id] = (u - v) * x.val();
            w *= w0;
            x *= w1;
          }
        }
      }
    }
  } else {
    int _log_m = 0;
    if (log_n % 2 == 1) {
      for (int chunk = 0; chunk < n; chunk += 2) {
        auto p = chunk;
        auto a = f[p + 0];
        auto b = f[p + 1];
        f[p + 0] = a + b;
        f[p + 1] = a - b;
      }
      _log_m = 1;
    }
    // 4-base
    for (int log_m = _log_m; log_m < log_n; log_m += 2) {
      int m = 1<<(log_m+1);
      mint w0 = coeff[log_m];
      auto w1 = coeff[log_m+1];
      auto wh = w1.pow(m/2).val();
      for (int chunk = 0; chunk < n; chunk += 2*m) {
        mint w = 1, x = 1;
        for (int i = 0; i < m/2; i++) {
          auto p = chunk + i;
          auto ia = p + 0*m/2;
          auto ib = p + 1*m/2;
          auto ic = p + 2*m/2;
          auto id = p + 3*m/2;
          long long a = f[ia].val(), b = (long long)f[ib].val() * w.val() % mod, 
                    c = f[ic].val(), d = (long long)f[id].val() * w.val() % mod;
          auto s = a + b, t = a - b + mod, u = (c + d) * x.val() % mod, v = (c - d + mod) * x.val() % mod * wh % mod;
          f[ia] = s + u;
          f[ib] = t + v;
          f[ic] = s - u; 
          f[id] = t - v;
          w *= w0;
          x *= w1;
        }
      }
    }
  }
  if constexpr (Forward and BitReverse)
    _iterative_bit_reverse(f);
  if constexpr (!Forward) {
    mint inv = mint(n).inv();
    for (int i = 0; i < n; i++) f[i] *= inv;
  }
}
template <int mod>
void _fft(std::vector<Modular<mod>>& f) { 
  _fft_impl<true, true>(f); 
}
template <int mod>
void _ifft(std::vector<Modular<mod>>& f) { 
  _fft_impl<false, true>(f); 
}
template <int mod>
void _convolution_fft(std::vector<Modular<mod>>& f) { 
  _fft_impl<true, false>(f); 
}
template <int mod>
void _convolution_ifft(std::vector<Modular<mod>>& f) { 
  _fft_impl<false, false>(f); 
}

} // namespace _ntt

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
void ntt_convolution_inline(std::vector<mint>& f) {
  _ntt::_convolution_fft(f);
}
template<class mint>
std::vector<mint> ntt_convolution(std::vector<mint> f) {
  _ntt::_convolution_fft(f);
  return f;
}
template<class mint>
void intt_convolution_inline(std::vector<mint>& f) {
  _ntt::_convolution_ifft(f);
}
template<class mint>
std::vector<mint> intt_convolution(std::vector<mint> f) {
  _ntt::_convolution_ifft(f);
  return f;
}
