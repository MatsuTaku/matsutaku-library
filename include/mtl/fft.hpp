#pragma once
#include <vector>
#include <complex>

class Fft {
 public:
  using complex_t = std::complex<double>;
  using value_type = complex_t;
 private:
  size_t n_;

 public:
  explicit Fft(size_t size) {
    n_ = 1;
    while (n_ < size)
      n_ *= 2;
  };

  size_t n() const { return n_; }

  void fft_inline(std::vector<complex_t>& f) const {
    _fft(f);
  }

  std::vector<complex_t> fft(const std::vector<complex_t>& f) const {
    auto ff = f;
    ff.resize(n_, 0);
    _fft(ff);
    return ff;
  }

  void ifft_inline(std::vector<complex_t>& f) const {
    _ifft(f);
  }

  std::vector<complex_t> ifft(const std::vector<complex_t>& f) const {
    auto ff = f;
    ff.resize(n_, 0);
    _ifft(ff);
    return ff;
  }

 private:
  template <bool Forward>
  void _fft_impl(std::vector<complex_t>& f) const {
    // iterative bit reversal
    for (size_t i = 0, j = 1; j < n_-1; j++) {
      for (size_t k = n_ >> 1; k > (i ^= k); k >>= 1);
      if (i < j) std::swap(f[i], f[j]);
    }
    // Cooley-Tukey FFT
    for (size_t m = 1; m < n_; m *= 2) {
      double theta0 = Forward ? M_PI/m : -M_PI/m;
      for (size_t chunk = 0; chunk < n_; chunk += 2*m) {
        for (size_t i = 0; i < m; i++) {
          complex_t w = {cos(theta0*i), -sin(theta0*i)};
          auto p = chunk + i;
          auto a = f[p + 0];
          auto b = f[p + m] * w;
          f[p + 0] = a + b;
          f[p + m] = a - b;
        }
      }
    }
  }

  void _fft(std::vector<complex_t>& f) const {
    _fft_impl<true>(f);
  }
  void _ifft(std::vector<complex_t>& f) const {
    _fft_impl<false>(f);
    for (auto& x : f) {
      x /= n_;
    }
  }

};

