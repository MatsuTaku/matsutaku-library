#pragma once
#include <vector>
#include <iterator>
#include "bit_manip.hpp"

template<class T, int LIM, class Iter>
std::vector<std::array<T,LIM+1>> SubsetRankedZeta(int n, Iter begin, Iter end) {
  std::vector<std::array<T,LIM+1>> zA(1<<n);
  size_t _i = 0;
  for (auto it = begin; it != end; ++it, _i++) {
    zA[_i][bm::popcnt(_i)] = *it;
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

template<class T, int LIM, class OutIter>
OutIter SubsetRankedMobius(int n, std::vector<std::array<T,LIM+1>>& zA, OutIter out) {
  for (int i = 0; i < n; i++) {
    auto w = 1<<i;
    for (int p = 0; p < 1<<n; p += 2*w) {
      for (int s = p; s < p+w; s++) {
        auto t = s+w;
        for (int d = 0; d <= n; d++) zA[t][d] -= zA[s][d];
      }
    }
  }
  for (int i = 0; i < 1<<n; i++) {
    *out = zA[i][bm::popcnt(i)];
    ++out;
  }
  return out;
}

template<class T, int LIM, class Iter, class OutIter>
Iter SubsetConvolution(Iter ba, Iter ea, Iter bb, Iter eb, OutIter out) {
  auto n = 64-bm::clz(std::max(std::distance(ba, ea), std::distance(bb, eb))-1);
  auto zA = SubsetRankedZeta<T, LIM>(n, ba, ea);
  auto zB = SubsetRankedZeta<T, LIM>(n, bb, eb);
  for (int i = 0; i < 1<<n; i++) {
    for (int d = n; d >= 0; d--) {
      T x = 0;
      for (int k = 0; k <= d; k++) {
        x += zA[i][k] * zB[i][d-k];
      }
      zA[i][d] = x;
    }
  }
  return SubsetRankedMobius<T, LIM>(n, zA, out);
}

template<class T, int LIM=20>
std::vector<T> SubsetConvolution(const std::vector<T>& A, const std::vector<T>& B) {
  auto n = 64-bm::clz(std::max(A.size(), B.size())-1);
  std::vector<T> C(1<<n);
  SubsetConvolution<T, LIM>(A.begin(), A.end(), B.begin(), B.end(), C.begin());
  return C;
}