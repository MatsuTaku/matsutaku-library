#pragma once
#include "subset_convolution.hpp"
#include "convolution.hpp"
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;

template<typename T, int LIM, std::contiguous_iterator TempIter>
void SpsEgfZetaMerge(int n, TempIter zbf) {
  assert(n <= LIM);
  [[assume(n <= LIM)]];
  if (n==0) return;
  auto zB = std::to_address(zbf);
  auto w = 1ull<<(n-1);
  for (auto s = 0; s < w; s++) {
    auto t = s+w;
    // Shift rank+1
    for (int d = LIM; d > 0; d--)
      zB[t][d] = zB[t][d-1];
    zB[t][0] = 0;
    // Final addition of zeta
    for (int d = 0; d <= LIM; d++)
      zB[t][d] += zB[s][d];
  }
}
/// Reverse of SpsEgfZetaMerge($n, $zbf)
template<typename T, int LIM, std::contiguous_iterator TempIter>
void SpsEgfMobiusSplit(int n, TempIter zbf) {
  assert(n <= LIM);
  [[assume(n <= LIM)]];
  if (n==0) return;
  auto zB = std::to_address(zbf);
  auto w = 1<<(n-1);
  for (int s = 0; s < w; s++) {
    auto t = s+w;
    // Shift rank-1
    for (int d = 0; d <= LIM; d++) 
      zB[t][d] -= zB[s][d];
    // Final sub of mobius
    for (int d = 0; d < LIM; d++)
      zB[t][d] = zB[t][d+1];
    zB[t][n] = 0;
  }
}

// Sum_i F_i/i! A^i, A^i is subset-convolution
template<typename T, int LIM=20>
std::vector<T> SpsCompositionEgf(int n, const std::vector<T>& F, const std::vector<T>& A) {
  assert(A.size() >= 1ull<<n);
  assert(A[0]== T(0));
  int d = std::min((size_t)n, F.size()-1);
  // std::vector<T> dp(1<<n), ndp(1<<n);
  // dp[0] = F[d];
  // using ranked_fn_type = decltype(SubsetRankedZeta<T, LIM>(n, A.begin(), A.end()));
  // std::vector<ranked_fn_type> zA(n), zDP(n);
  // for (int i = 0; i < n; i++) {
  //   zA[i].resize(1<<i);
  //   zDP[i].resize(1<<i);
  // }
  // for (int i = d-1; i >= 0; i--) {
  //   ndp.assign(1<<n, 0);
  //   ndp[0] = F[i];
  //   for (int j = 0; j < n-i; j++) {
  //     // A:
  //     // SubsetConvolution<T, LIM>(A.cbegin()+(1<<j), A.cbegin()+(2<<j),
  //     //                           dp.cbegin(), dp.cbegin()+(1<<j),
  //     //                           ndp.begin()+(1<<j));
  //     // B:
  //     SubsetRankedZeta<T, LIM>(j, A.cbegin()+(1<<j), A.cbegin()+(2<<j), zA[j].begin());
  //     SubsetRankedZeta<T, LIM>(j, dp.cbegin(), dp.cbegin()+(1<<j), zDP[j].begin());
  //     SubsetConvolutionMobius<T, LIM>(j, zA[j], zDP[j], ndp.begin()+(1<<j));
  //   }
  //   swap(dp, ndp);
  // }
  // return dp;

  RankedVector<T,LIM> zA(1<<n),zB(1<<n),zC(1<<n);
  for (int i = 0; i < n; i++)
    SubsetRankedZeta<T,LIM>(i, A.begin(), A.end(), zA.begin()+(1<<i));
  zB[0][0] = F[d];
  for (int k = d-1; d >= 0; k--) {
    zC[0][0] = F[k];
    for (int i = 0; i < n-k; i++) {
      if (i>0) {
        SpsEgfZetaMerge<T,LIM>(i, zB.begin());
      }
      SubsetPointwiseConvolution<T,LIM>(i,
        zA.cbegin()+(1<<i), zA.cbegin()+(2<<i),
        zB.cbegin(), zB.cbegin()+(1<<i),
        zC.begin()+(1<<i));
    }
    swap(zB,zC);
  }
  std::vector<T> ret(1<<n);
  for (int i = n-1; i >= 0; i--) {
    SubsetRankedMobius<T,LIM>(i, zB.begin()+(1<<i), zB.begin()+(2<<i), ret.begin()+(1<<i));
    if (i>0) {
      SpsEgfMobiusSplit<T,LIM>(i, zB.begin());
    }
  }
  assert(F[0] == zB[0][0]);
  ret[0] = zB[0][0];
  return ret;
}

// Sum_i F_i A^i, A^i is subset-convolution
template<typename T, int LIM=20>
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

// exp(A) = Sum_i A^i/i!, A^i is subset-convolution
// Equal to composite f=1+x+x^2+... with A
// In this case, f(a) = f^d(a)
// So this is special case of sps-composition
// $first and $out can be same pointer
template<typename T, int LIM, typename InputIter, typename OutputIter>
OutputIter SpsExp(int n, InputIter first, InputIter last, OutputIter out) {
  assert(n <= LIM);
  [[assume(n <= LIM)]];
  assert(std::distance(first, last) == 1<<n);
  assert(*first == T(0));
  // Same as below:
  // out[0] = 1;
  // for (int i = 0; i < n; i++) {
  //   SubsetConvolution<T, LIM>(first+(1<<i), first+(2<<i), 
  //                             out, out+(1<<i), 
  //                             out+(1<<i));
  // }

  RankedVector<T, LIM> zA(1<<n), zB(1<<n);
  zB[0][0] = 1;
  for (int i = 0; i < n; i++) {
    SubsetRankedZeta<T, LIM>(i, first+(1<<i), first+(2<<i), zA.begin()+(1<<i));
    // Remaining addition of zeta(out[0,1<<i))
    if (i>0) {
      SpsEgfZetaMerge<T, LIM>(i, zB.begin());
    }
    SubsetPointwiseConvolution<T, LIM>(i, 
                                       zA.cbegin()+(1<<i), zA.cbegin()+(2<<i),
                                       zB.cbegin(), zB.cbegin()+(1<<i),
                                       zB.begin()+(1<<i));
  }
  for (int i = n-1; i >= 0; i--) {
    SubsetRankedMobius<T, LIM>(i, zB.begin()+(1<<i), zB.begin()+(2<<i), out+(1<<i));
    // Revert addition of zeta(out[0,1<<i))
    if (i>0) {
      SpsEgfMobiusSplit<T, LIM>(i, zB.begin());
    }
  }
  *(out+0) = 1;
  return out+(1<<n);
}
// Sum_i A^i/i!, A^i is subset-convolution
// Equal to composite f=1+x+x^2+... with A
// So this is special case of sps-composition
template<typename T, int LIM=20>
std::vector<T> SpsExp(int n, const std::vector<T>& A) {
  std::vector<T> dp(1<<n);
  SpsExp(n, A.cbegin(), A.cend(), dp.begin());
  return dp;
}

template<typename T, int LIM=20>
std::vector<T> SpsLog(int n, std::vector<T> s) {
  assert(n <= LIM);
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
template<typename T, int LIM, typename LIter, typename RIter, typename OutIter>
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
template<typename T, int LIM=20>
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
template<typename T, int LIM, typename InputIterator1, typename InputIterator2>
std::vector<T> TransposedSpsCompositionEgf(int n, InputIterator1 sf, InputIterator2 xf) {
  assert(*sf == T{0});
  assert(n <= LIM);
  [[assume(n <= LIM)]];
  std::vector<T> y(n+1);
  y[0] = *xf;
  if (n==0) return y;
  std::vector<T> f(1<<n);
  std::copy(xf, xf+(1<<n), f.begin());
  RankedVector<T, LIM> zs(1<<n),zf(1<<n);
  for (int k = 0; k < n; k++)
    SubsetRankedZeta<T, LIM>(k, sf+(1<<k), sf+(2<<k), zs.begin()+(1<<k));
  for (int k = 0; k < n; k++) {
    auto& g = f; // Can calc inline
    g[0] = 0;
    for (int i = 0; i < n-k; i++) {
      // TransposedSubsetConvolution<T, LIM>(i, 
      //   f.cbegin()+(1<<i), f.cbegin()+(2<<i),
      //   s.cbegin()+(1<<i), s.cbegin()+(2<<i),
      //   f.begin()+(1<<i));
      SubsetRankedZeta<T, LIM>(i,
        std::make_reverse_iterator(f.cbegin()+(2<<i)),
        std::make_reverse_iterator(f.cbegin()+(1<<i)),
        zf.begin()+(1<<i));
      SubsetPointwiseConvolution<T, LIM>(i,
        zs.cbegin()+(1<<i), zs.cbegin()+(2<<i),
        zf.cbegin()+(1<<i), zf.cbegin()+(2<<i),
        zf.begin()+(1<<i));
      SubsetRankedMobius<T, LIM>(i, zf.begin()+(1<<i), zf.begin()+(2<<i), 
        std::make_reverse_iterator(g.begin()+(2<<i)));
    }
    for (int i = 0; i < n-k; i++) {
      auto w = 1u<<i;
      for (int j = 0; j < w; j++) {
        g[j] += g[j+w];
        g[j+w] = 0;
      }
    }
    y[k+1] = g[0];
  }
  return y;
}

// For fixed sps s s.t. s[0]==0.
// Consider Linear map F:f->t=f(s) for egf f.
// Given x, calculate transpose(F)(x)
// Equivalent: Calculate sum_i x_i(s^k/k!)_i for k=0,1,...,M-1
template<typename T, int LIM=20, std::convertible_to<T> U, std::convertible_to<T> V>
std::vector<T> TransposedSpsCompositionEgf(const std::vector<U>& s, const std::vector<V>& x) {
  assert(!s.empty());
  int n = 64-bm::clz(s.size()-1);
  return TransposedSpsCompositionEgf<T, LIM>(n, s.begin(), x.begin());
}

// For fixed sps s.
// Consider linear map F:f->t=f(s) for polynomial f.
// Given x, calcuate transpose(F)(x)
// Equivalent: Calculate sum_i x_i(s^k)_i for k=0,1,...,M-1
template<typename T, int LIM=20>
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

template<typename T, int LIM=20, std::convertible_to<T> U>
std::vector<T> SpsInv(int n, const std::vector<U>& s) {
  auto zs = SubsetRankedZeta<T,LIM>(n, s.begin(), s.end());
  // Pointwise inv
  std::array<T, LIM+1> g{};
  for (int i = 0; i < 1<<n; i++) {
    g[0] = 1;
    for (int j = 1; j <= n; j++) {
      g[j] = 0;
      for (int k = 0; k < j; k++) {
        g[j] -= g[k] * zs[i][j-k];
      }
    }
    zs[i] = g;
  }
  return SubsetRankedMobius<T,LIM>(n, zs);
}