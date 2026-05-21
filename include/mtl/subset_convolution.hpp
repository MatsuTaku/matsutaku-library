#pragma once
#include "bit_manip.hpp"
#include <vector>
#include <array>
#include <iterator>
#include <concepts>
#include <cstddef>
#include <cassert>

template<typename T, int LIM>
using RankedVector = std::vector<std::array<T, LIM+1>>;

template<typename T, int LIM, class Iter, std::contiguous_iterator OutIter>
OutIter SubsetRankedZeta(const int n, Iter first, Iter last, OutIter out) {
  assert(n <= LIM);
  [[assume(n <= LIM)]];
  size_t _i = 0;
  auto out_addr = std::to_address(out);
  for (auto it = first; it != last; ++it, ++_i) {
    std::fill(out_addr[_i].begin(), out_addr[_i].end(), T{0});
    out_addr[_i][bm::popcnt(_i)] = *it;
  }
  /// Basic implementation
  // for (int i = 0; i < n; i++) {
  //   auto w = 1<<i;
  //   for (int p = 0; p < 1<<n; p += 2*w) {
  //     for (int s = p; s < p+w; s++) {
  //       auto t = s+w;
  //       // Using <=LIM instead <=n is to consider compiler optimization
  //       for (int d = 0; d <= LIM; d++) out_addr[t][d] += out_addr[s][d];
  //     }
  //   }
  // }
  /// Cache-oblivious order
  for (unsigned mask = 1u; mask < 1u<<n; mask += 2) {
    auto m = mask;
    unsigned w = 1;
    while (m&1u) {
      for (auto t = mask+1-w; t < mask+1; t++) {
        auto s = t-w;
        // Using <=LIM instead <=n is to consider compiler optimization
        for (int d = 0; d <= LIM; d++)
          out_addr[t][d] += out_addr[s][d];
      }
      m >>= 1;
      w <<= 1;
    }
  }
  return out+(1<<n);
}
template<typename T, int LIM, class Iter>
RankedVector<T, LIM> SubsetRankedZeta(const int n, Iter begin, Iter end) {
  RankedVector<T, LIM> zA(1<<n);
  SubsetRankedZeta<T, LIM>(n, begin, end, zA.begin());
  return zA;
}

template<typename T, int LIM, std::contiguous_iterator InputIter, typename OutIter>
OutIter SubsetRankedMobius(const int n, InputIter zAf, InputIter zAl, OutIter out) {
  assert(n <= LIM);
  [[assume(n <= LIM)]];
  assert(std::distance(zAf,zAl) == 1<<n);
  auto zA = std::to_address(zAf);
  /// Basic implementation
  // for (int i = 0; i < n; i++) {
  //   auto w = 1<<i;
  //   for (int p = 0; p < 1<<n; p += 2*w) {
  //     for (int s = p; s < p+w; s++) {
  //       auto t = s+w;
  //       // Using <=LIM instead <=n is to consider compiler optimization
  //       for (int d = 0; d <= LIM; d++) zA[t][d] -= zA[s][d];
  //     }
  //   }
  // }
  /// Cache-oblivious order
  for (auto mask = 1u; mask < 1u<<n; mask += 2) {
    auto m = mask;
    auto w = 1u;
    while (m&1u) {
      for (auto t = mask+1-w; t < mask+1; t++) {
        auto s = t-w;
        for (int d = 0; d <= LIM; d++)
          zA[t][d] -= zA[s][d];
      }
      m >>= 1;
      w <<= 1;
    }
  }
  for (int i = 0; i < 1<<n; i++) {
    *out = zA[i][bm::popcnt(i)];
    ++out;
  }
  return out;
}
template<typename T, int LIM>
std::vector<T> SubsetRankedMobius(int n, RankedVector<T, LIM>& zA) {
  std::vector<T> out(1<<n);
  SubsetRankedMobius<T, LIM>(n, zA.begin(), zA.end(), out.begin());
  return out;
}

// return zetaC(S) = sum_{s \in S} conv(zetaA(s), zetaB(s))
// ($zAf or $zBf) and $out can be same pointer
template<typename T, int LIM, 
  std::contiguous_iterator InputIter1, 
  std::contiguous_iterator InputIter2, 
  std::contiguous_iterator OutputIter>
InputIter1 SubsetPointwiseConvolution(
  const int n, 
  InputIter1 zAf, InputIter1 zAl,
  InputIter2 zBf, InputIter2 zBl,
  OutputIter out) 
  {
  assert(n <= LIM);
  [[assume(n <= LIM)]];
  assert(std::distance(zAf, zAl) == 1<<n);
  assert(std::distance(zBf, zBl) == 1<<n);
  auto zA = std::to_address(zAf);
  auto zB = std::to_address(zBf);
  auto zAzB = std::to_address(out);
  std::array<T, LIM+1> tmp{};
  for (int i = 0; i < 1<<n; i++) {
    // Greedy convolution
    for (int d = 0; d <= n; d++)
      tmp[d] = T{0};
    for (int j = 0; j <= n; j++) {
      auto a = zA[i][j];
      for (int k = 0; k <= n-j; k++) 
        tmp[j+k] += a * zB[i][k];
    }
    for (int d = 0; d <= n; d++)
      zAzB[i][d] = tmp[d];
  }
  return out + (1<<n);
}

template<typename T, int LIM, typename OutIter>
OutIter SubsetConvolutionMobius(
  const int n, 
  RankedVector<T, LIM>& zA, 
  const RankedVector<T, LIM>& zB, 
  OutIter out) {
  assert(n <= LIM);
  SubsetPointwiseConvolution<T, LIM>(n, zA.cbegin(), zA.cend(), zB.cbegin(), zB.cend(), 
    zA.begin());
  return SubsetRankedMobius<T, LIM>(n, zA.begin(), zA.end(), out);
}

template<typename T, int LIM, class LIter, class RIter, class OutIter>
OutIter SubsetConvolution(LIter ba, LIter ea, RIter bb, RIter eb, OutIter out) {
  auto n = 64-bm::clz(std::max(std::distance(ba, ea), std::distance(bb, eb))-1);
  auto zA = SubsetRankedZeta<T, LIM>(n, ba, ea);
  auto zB = SubsetRankedZeta<T, LIM>(n, bb, eb);
  return SubsetConvolutionMobius<T, LIM>(n, zA, zB, out);
}
template<typename T, int LIM=20, std::convertible_to<T> U, std::convertible_to<T> V>
std::vector<T> SubsetConvolution(const std::vector<U>& A, const std::vector<V>& B) {
  auto n = 64-bm::clz(std::max(A.size(), B.size())-1);
  std::vector<T> C(1<<n);
  SubsetConvolution<T, LIM>(A.begin(), A.end(), B.begin(), B.end(), C.begin());
  return C;
}

template<typename T, int LIM, class Iter, class OutIter>
OutIter SubsetConvolutionSquare(Iter ba, Iter ea, OutIter out) {
  auto n = 64-bm::clz(std::distance(ba, ea)-1);
  auto zA = SubsetRankedZeta<T, LIM>(n, ba, ea);
  return SubsetConvolutionMobius<T, LIM>(n, zA, zA, out);
}
template<typename T, int LIM=20>
std::vector<T> SubsetConvolutionSquare(const std::vector<T>& A) {
  auto n = 64-bm::clz(A.size()-1);
  std::vector<T> C(1<<n);
  SubsetConvolutionSquare<T, LIM>(A.begin(), A.end(), C.begin());
  return C;
}