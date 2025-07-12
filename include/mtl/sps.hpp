#pragma once
#include "subset_convolution.hpp"
#include <iostream>
using namespace std;

// Sum_i A^i/i!, A^i is subset-comvolution
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

// Sum_i F_i/i! A^i, A^i is subset-comvolution
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

// Sum_i F_i A^i, A^i is subset-comvolution
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