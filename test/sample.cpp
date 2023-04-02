// -----------------------------------
// Author     : MatsuTaku
// Affiliation: Tokushima University
// Country    : Japan
// Date       : 03/14/2020
// -----------------------------------

#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template <typename T, T MOD>
class modint {
 private:
  T val_;

 public:
  constexpr modint() : val_(0) {}
  constexpr modint(T v) : val_(v%MOD) {}
  constexpr modint& operator=(T v) { val_ = v%MOD; return *this; }

  constexpr T val() const { return val_; }
  constexpr modint operator+(modint x) const { return (val() + x.val())%MOD; }
  constexpr modint operator+=(modint x) { return *this = *this + x; }
  constexpr modint operator++() { return *this += 1; }
  constexpr modint operator++(int) { modint c = *this; ++(*this); return c; }
  constexpr modint operator-(modint x) const { return (val() + MOD - x.val())%MOD; }
  constexpr modint operator-=(modint x) { return *this = *this - x; }
  constexpr modint operator--() { return *this -= 1; }
  constexpr modint operator--(int) { modint c = *this; --(*this); return c; }
  constexpr modint operator*(modint x) const { return (val() * x.val())%MOD; }
  constexpr modint operator*=(modint x) { return *this = *this * x; }
  friend constexpr modint pow(modint x, T p) {
    modint t = 1;
    modint u = x;
    while (p) {
      if (p & 1)
        t *= u;
      u *= u;
      p >>= 1;
    }
    return t;
  }
  constexpr modint operator/(modint x) const { return *this * pow(modint{x}, MOD-2); }
  constexpr modint operator/=(modint x) { return *this = *this / x; }

  constexpr bool operator==(modint x) const { return val() == x.val(); }
  constexpr bool operator!=(modint x) const { return val() != x.val(); }
  constexpr bool operator<(modint x) const { return val() < x.val(); };
  constexpr bool operator<=(modint x) const { return val() <= x.val(); };
  constexpr bool operator>(modint x) const { return val() > x.val(); };
  constexpr bool operator>=(modint x) const { return val() >= x.val(); };

  friend std::ostream& operator<<(std::ostream& os, modint x) {
    return os << x.val();
  }
  friend std::istream& operator>>(std::istream& is, modint& x) {
    return is >> x.val_;
  }

};

class NTT {
 public:
  using int_type = unsigned long long;
  static constexpr int_type P = 998244353;
  using mint = modint<int_type, P>;
 private:
  size_t n_;
  mint r_;
  std::vector<mint> coeff_;
  std::vector<mint> icoeff_;
  size_t log_n_;
  size_t clz_n_;

  int_type bit_reverse(int_type x) const {
    x = ((x & 0x00000000FFFFFFFF) << 32) | ((x & 0xFFFFFFFF00000000) >> 32);
    x = ((x & 0x0000FFFF0000FFFF) << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);
    x = ((x & 0x00FF00FF00FF00FF) << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);
    x = ((x & 0x0F0F0F0F0F0F0F0F) << 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);
    x = ((x & 0x3333333333333333) << 2) | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);
    x = ((x & 0x5555555555555555) << 1) | ((x & 0xAAAAAAAAAAAAAAAA) >> 1);
    return x >> (clz_n_+1);
  }

 public:
  explicit NTT(size_t size) {
    n_ = 1;
    log_n_ = 0;
    clz_n_ = 63;
    while (n_ < size) {
      n_*=2;
      log_n_++;
      clz_n_--;
    }
    assert((P-1)%n_ == 0);
    r_ = pow(mint{3}, (P-1)/n_);
    coeff_.resize(log_n_);
    icoeff_.resize(log_n_);
    mint iw = mint{1}/r_;
    mint w = r_;
    for (int i = log_n_-1; i >= 0; i--) {
      coeff_[i] = iw;
      iw *= iw;
      icoeff_[i] = w;
      w *= w;
    }
  };

  size_t n() const { return n_; }

  void fft_inline(vector<mint>& f) const {
    _fft(f);
  }

  vector<mint> fft(const vector<mint>& f) const {
    auto ff = f;
    ff.resize(n_, 0);
    _fft(ff);
    return ff;
  }

  void ifft_inline(vector<mint>& f) const {
    _ifft(f);
  }

  vector<mint> ifft(const vector<mint>& f) const {
    auto ff = f;
    ff.resize(n_, 0);
    _ifft(ff);
    return ff;
  }

 private:
  template <bool Forward>
  void _fft_impl(vector<mint>& f) const {
    // iterative bit reversal
    for (int_type i = 0; i < n_; i++) {
      auto j = bit_reverse(i);
      if (i >= j)
        continue;
      swap(f[i], f[j]);
    }
    // Cooley-Tukey FFT
    for (size_t log_m = 0; log_m < log_n_; log_m++) {
      size_t m = 1ull<<log_m;
      for (size_t chunk = 0; chunk < n_; chunk += 2*m) {
        mint w = 1;
        for (size_t i = 0; i < m; i++) {
          auto p = chunk + i;
          auto a = f[p];
          auto b = f[p + m] * w;
          f[p + 0] = a + b;
          f[p + m] = a - b;
          if (Forward) {
            w *= coeff_[log_m];
          } else {
            w *= icoeff_[log_m];
          }
        }
      }
    }

  }

  void _fft(vector<mint>& f) const {
    _fft_impl<true>(f);
  }
  void _ifft(vector<mint>& f) const {
    _fft_impl<false>(f);
    mint div_n = mint{1}/n_;
    for (auto& x : f)
      x *= div_n;
  }

};

constexpr NTT::int_type NTT::P;

class convolution {
 public:
  using porinomial = std::vector<NTT::mint>;
 private:
  NTT ntt_;
 public:
  explicit convolution(size_t size) : ntt_(size) {}

  porinomial operator()(const porinomial& g, const porinomial& h) const {
    auto fg = ntt_.fft(g);
    auto fh = ntt_.fft(h);
    for (size_t i = 0; i < ntt_.n(); i++) {
      fg[i] *= fh[i];
    }
    ntt_.ifft_inline(fg);
    return fg;
  }

};

int main() {
  cin.tie(nullptr); ios::sync_with_stdio(false);



  return 0;
}
