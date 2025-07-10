#pragma once
#include "subset_convolution.hpp"

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