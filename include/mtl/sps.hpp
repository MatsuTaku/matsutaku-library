#pragma once
#include "subset_convolution.hpp"
#include "convolution.hpp"
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;

// Sum_i A^i/i!, A^i is subset-convolution
template<class T, int LIM=20>
std::vector<T> SpsExp(int n, std::vector<T>& A) {
  assert(A.size()==size_t(1<<n));
  assert(A[0] == T(0));
  std::vector<T> dp(1<<n);
  dp[0] = 1;
  for (int i = 0; i < n; i++) {
    SubsetConvolution<T, LIM>(A.begin()+(1<<i), A.begin()+(2<<i), 
                              dp.begin(), dp.begin()+(1<<i), 
                              dp.begin()+(1<<i));
  }
  return dp;
}

// Sum_i F_i/i! A^i, A^i is subset-convolution
template<class T, int LIM=20>
std::vector<T> SpsCompositionEgf(int n, const std::vector<T>& F, const std::vector<T>& A) {
  assert(A.size() >= 1ull<<n);
  assert(A[0]== T(0));
  int d = std::min((size_t)n, F.size()-1);
  std::vector<T> dp(1<<n), ndp(1<<n);
  dp[0] = F[d];
  using ranked_fn_type = decltype(SubsetRankedZeta<T, LIM>(n, A.begin(), A.end()));
  std::vector<ranked_fn_type> zA(n), zDP(n);
  for (int i = 0; i < n; i++) {
    zA[i].resize(1<<i);
    zDP[i].resize(1<<i);
  }
  for (int i = d-1; i >= 0; i--) {
    ndp.assign(1<<n, 0);
    ndp[0] = F[i];
    for (int j = 0; j < n-i; j++) {
      // SubsetConvolution<T, LIM>(A.cbegin()+(1<<j), A.cbegin()+(2<<j),
      //                           dp.cbegin(), dp.cbegin()+(1<<j),
      //                           ndp.begin()+(1<<j));
      SubsetRankedZeta<T, LIM>(j, A.cbegin()+(1<<j), A.cbegin()+(2<<j), zA[j].begin());
      SubsetRankedZeta<T, LIM>(j, dp.cbegin(), dp.cbegin()+(1<<j), zDP[j].begin());
      SubsetConvolutionImpl<T, LIM>(j, zA[j], zDP[j], ndp.begin()+(1<<j));
    }
    swap(dp, ndp);
  }
  return dp;
}

// Sum_i F_i A^i, A^i is subset-convolution
template<class T, int LIM=20>
std::vector<T> SpsCompositionPoly(int n, const std::vector<T>& F, std::vector<T> A) {
  if (F.empty()) return std::vector<T>(1<<n, T(0));
  int d = std::min(F.size()-1, (size_t)n);
  std::vector<T> g(d+1);
  T c = A[0];
  A[0] = 0;
  A.resize(1<<n, 0);
  if (c==T(0)) {
    return SpsCompositionEgf<T, LIM>(n, F, A);
  }
  // (x+c)^i
  std::vector<T> pow(d+1);
  pow[0] = 1;
  for (int i = 0; i < (int)F.size(); i++) {
    for (int j = 0; j <= d; j++) {
      g[j] += F[i] * pow[j];
    }
    for (int j = d; j >= 0; j--) {
      pow[j] = pow[j] * c + (j==0 ? T(0) : pow[j-1]);
    }
  } 
  // to egf
  T factorial = 1;
  for (int j = 0; j <= d; j++) {
    g[j] *= factorial, factorial *= j+1;
  }
  return SpsCompositionEgf<T, LIM>(n, g, A);
}

template<class T, int LIM=20>
std::vector<T> SpsLog(int n, vector<T> s) {
    assert(s[0] == T(1));
    // composite log(1-x) with 1-s
    // log(1-x) = -sum_{i=1}^n x^i/i
    // to egf:  = sum_{i=1}^n -(i-1)! x^i/i!
    std::vector<T> f(n+1);
    T fact = 1;
    for (int i = 1; i <= n; i++) {
      f[i] = -fact;
      fact *= i;
    }
    s[0] = 0;
    if (s.size() != size_t(1ull<<n))
      s.resize(1<<n, 0);
    for (int i = 1; i < 1<<n; i++) 
      s[i] = -s[i];
    return SpsCompositionEgf<T, LIM>(n, f, s);
}

// For given sps s, consider linear map F:a->b = subset-conv(a,s)
// Given x, calculate transpose(F)(x)
template<class T, int LIM, class LIter, class RIter, class OutIter>
OutIter TransposedSubsetConvolution(LIter bs, LIter es, RIter bx, RIter ex, OutIter out) {
  /* 
  sum_{j}x_jb_j = sum_{i subset j}x_ja_is_{j-i} = sum_{i}y_ia_i
  y_i = sum_{j subset i} x_js_{j-i}
  (rev y)_i = sum_{j subset i}(rev x)_js{i-j}
  y = rev(conv(rev x), s)
  */
  auto out_end = SubsetConvolution<T, LIM>(
    bs, es, 
    std::make_reverse_iterator(ex), std::make_reverse_iterator(bx), 
    out);
  std::reverse(out, out_end);
  return out_end;
}

// For given sps s, consider linear map F:a->b = subset-conv(a,s)
// Given x, calculate transpose(F)(x)
template<class T, int LIM=20>
vector<T> TransposedSubsetConvolution(const vector<T>& s, vector<T> x) {
  /* 
  sum_{j}x_jb_j = sum_{i subset j}x_ja_is_{j-i} = sum_{i}y_ia_i
  y_i = sum_{j subset i} x_js_{j-i}
  (rev y)_i = sum_{j subset i}(rev x)_js{i-j}
  y = rev(conv(rev x), s)
  */
  std::reverse(x.begin(), x.end());
  x = SubsetConvolution<T, LIM>(x, s);
  std::reverse(x.begin(), x.end());
  return x;
}

// For fixed sps s s.t. s[0]==0.
// Consider Linear map F:f->t=f(s) for egf f.
// Given x, calculate transpose(F)(x)
// Equivalent: Calculate sum_i x_i(s^k/k!)_i for k=0,1,...,M-1
template<class T, int LIM=20>
std::vector<T> TransposedSpsCompositionEgf(const std::vector<T>& s, std::vector<T> x) {
  int n = 64-bm::clz(s.size()-1);
  std::vector<T> y(n+1);
  y[0] = x[0];
  if (n==0) return y;
  auto& dp = x;
  decltype(SubsetRankedZeta<T,LIM>(LIM,s.begin(),s.end())) zs, zdp;
  zs.reserve(1<<(n-1));
  zdp.reserve(1<<(n-1));
  for (int i = 0; i < n; i++) {
    std::vector<T> newdp(1<<(n-1-i));
    for (int j = 0; j < n-i; j++) {
      // std::vector<T> a(s.begin()+(1<<j), s.begin()+(2<<j));
      // std::vector<T> b(dp.begin()+(1<<j), dp.begin()+(2<<j));
      // b = TransposedSubsetConvolution<T, LIM>(a, b);
      // TransposedSubsetConvolution<T, LIM>(
      //   s.cbegin()+(1<<j), s.cbegin()+(2<<j),
      //   dp.cbegin()+(1<<j), dp.cbegin()+(2<<j),
      //   dp.begin()+(1<<j)
      // );
      zs.resize(1<<j);
      zdp.resize(1<<j);
      SubsetRankedZeta<T,LIM>(j, s.begin()+(1<<j), s.begin()+(2<<j), zs.begin());
      SubsetRankedZeta<T,LIM>(j, 
                              std::make_reverse_iterator(dp.begin()+(2<<j)), 
                              std::make_reverse_iterator(dp.begin()+(1<<j)),
                              zdp.begin());
      SubsetConvolutionImpl<T,LIM>(j, zs, zdp, 
                                   std::make_reverse_iterator(dp.begin()+(2<<j)));
      for (int k = 0; k < 1<<j; k++) newdp[k] += dp[(1<<j)+k];
    }
    swap(dp, newdp);
    y[i+1] = dp[0];
  }
  return y;
}

// For fixed sps s.
// Consider linear map F:f->t=f(s) for polynomial f.
// Given x, calcuate transpose(F)(x)
// Equivalent: Calculate sum_i x_i(s^k)_i for k=0,1,...,M-1
template<class T, int LIM=20>
std::vector<T> TransposedSpsCompositionPoly(std::vector<T> s, std::vector<T> x, int m) {
  if (m==0) return {};
  T c = s[0];
  s[0] -= c;
  x = TransposedSpsCompositionEgf<T, LIM>(s, x);
  std::vector<T> fact(m), fact_inv(m);
  fact[0] = 1;
  for (int i = 1; i < m; i++) {
    fact[i] = fact[i-1] * i;
  }
  fact_inv[m-1] = fact[m-1].inv();
  for (int i = m-2; i >= 0; i--) {
    fact_inv[i] = fact_inv[i+1] * (i+1);
  }
  std::vector<T> g(m);
  T pow = 1;
  for (int i = 0; i < m; i++) {
    g[i] = pow * fact_inv[i];
    pow *= c;
  }
  x = convolution_fft(x, g);
  x.resize(m);
  for (int i = 0; i < m; i++) x[i] *= fact[i];
  return x;
}