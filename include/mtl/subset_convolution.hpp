#pragma once
#include <vector>
#include "bit_manip.hpp"

#include <bits/stdc++.h>
template<class T, int LIM=20>
std::vector<std::array<T,LIM+1>> SubsetRankedZeta(const std::vector<T>& A, int n=-1) {
  if (A.empty()) return {};
  if (n==-1) n = 64-bm::clz(A.size()-1);
  std::vector<std::array<T,LIM+1>> zA(1<<n);
  for (size_t i = 0; i < A.size(); i++) {
    zA[i][bm::popcnt(i)] = A[i];
  }
  for (int i = 0; i < n; i++) {
    auto w = 1<<i;
    for (int p = 0; p < 1<<n; p += 2*w) {
      for (int s = p; s < p+w; s++) {
        auto t = s+w;
        for (int d = 0; d <= n; d++) zA[t][d] += zA[s][d];
      }
    }
  }
  return zA;
} 

template<class T, int LIM=20>
std::vector<T> SubsetRankedMobius(std::vector<std::array<T,LIM+1>>& zA, int n=-1) {
  if (zA.empty()) return {};
  if (n==-1) n = 64-bm::clz(zA.size()-1);
  auto& _A = zA;
  for (int i = 0; i < n; i++) {
    auto w = 1<<i;
    for (int p = 0; p < 1<<n; p += 2*w) {
      for (int s = p; s < p+w; s++) {
        auto t = s+w;
        for (int d = 0; d <= n; d++) _A[t][d] -= _A[s][d];
      }
    }
  }
  std::vector<T> A(1<<n);
  for (int i = 0; i < 1<<n; i++) {
    A[i] = _A[i][bm::popcnt(i)];
  }
  return A;
} 

template<class T, int LIM=20>
std::vector<T> SubsetConvolution(const std::vector<T>& A, const std::vector<T>& B) {
  if (A.empty() or B.empty()) return {};
  auto n = 64-bm::clz(std::max(A.size(), B.size())-1);
  auto zA = SubsetRankedZeta<T, LIM>(A, n);
  auto zB = SubsetRankedZeta<T, LIM>(B, n);
  for (int i = 0; i < 1<<n; i++) {
    for (int d = n; d >= 0; d--) {
      T x = 0;
      for (int k = 0; k <= d; k++) {
        x += zA[i][k] * zB[i][d-k];
      }
      zA[i][d] = x;
    }
  }
  return SubsetRankedMobius<T, LIM>(zA, n);
}