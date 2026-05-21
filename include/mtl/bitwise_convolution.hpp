#pragma once
#include "bit_manip.hpp"
#include <vector>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <cstddef>

/* Get \zeta A
 * defined: [x^s]\zeta A = \sum_{t \in s} A_t  
*/
template<std::contiguous_iterator ContiguousIterator>
void SubsetZetaImpl(int n, ContiguousIterator first) {
  auto A = std::to_address(first);
  /// Basic implementation
  // for (int i = 0; i < n; i++) {
  //   auto w = 1<<i;
  //   for (int p = 0; p < 1<<n; p += 2*w) {
  //     for (int s = p; s < p+w; s++) {
  //       int t = s|w;
  //       A[t] += A[s];
  //     }
  //   }
  // }
  /// Cache-oblivious order
  for (auto mask = 1u; mask < 1u<<n; mask += 2) {
    auto m = mask;
    auto w = 1u;
    while (m & 1u) {
      auto t = mask+1-w;
      for (auto t = mask+1-w; t < mask+1; t++) {
        A[t] += A[t-w];
      }
      m >>= 1;
      w <<= 1;
    }
  }
}

/* Get \zeta A
 * defined: [x^s]\zeta A = \sum_{t \in s} A_t  
*/
template<typename T, typename InputIterator>
std::vector<T> SubsetZeta(int n, InputIterator first) {
  std::vector<T> zA(1<<n);
  std::copy(first, first+(1<<n), zA.begin());
  SubsetZetaImpl<T>(n, zA.begin());
  return zA;
}

/* Get \zeta A
 * defined: [x^s]\zeta A = \sum_{t \in s} A_t  
*/
template<typename T>
void SubsetZetaInline(std::vector<T>& A) {
  if (A.empty()) return;
  int n = 64-bm::clz(A.size()-1);
  SubsetZetaImpl(n, A.begin());
}

/* Get \zeta A
 * defined: [x^s]\zeta A = \sum_{t \in s} A_t  
*/
template<typename T>
std::vector<T> SubsetZeta(const std::vector<T>& A) {
  if (A.empty()) return {};
  auto n = 64-bm::clz(A.size()-1);
  return SubsetZeta(n, A.begin());
}

/* Get \mobius A, the reverse transformation of \zeta A
*/
template<std::contiguous_iterator ContiguousIterator>
void SubsetMobiusImpl(int n, ContiguousIterator first) {
  auto zA = std::to_address(first);
  /// Basic implementation
  // for (int i = 0; i < n; i++) {
  //   auto w = 1<<i;
  //   for (int p = 0; p < 1<<n; p += 2*w) {
  //     for (int s = p; s < p+w; s++) {
  //       int t = s|w;
  //       zA[t] -= zA[s];
  //     }
  //   }
  // }  
  /// Cache-oblivious order
  for (auto mask = 1u; mask < 1u<<n; mask += 2) {
    auto m = mask;
    auto w = 1u;
    while (m & 1u) {
      auto t = mask+1-w;
      for (auto t = mask+1-w; t < mask+1; t++) {
        zA[t] -= zA[t-w];
      }
      m >>= 1;
      w <<= 1;
    }
  }
}

/* Get \mobius A, the reverse transformation of \zeta A
*/
template<std::input_iterator InputIterator, typename T = std::iter_value_t<InputIterator>>
std::vector<T> SubsetMobius(int n, InputIterator first) {
  std::vector<T> zA(1<<n);
  std::copy(first, first+(1<<n), zA.begin());
  SubsetMobiusImpl(n, zA.begin());
  return zA;
}

/* Get \mobius A, the reverse transformation of \zeta A
*/
template<typename T>
std::vector<T> SubsetMobius(const std::vector<T>& A) {
  if (A.empty()) return {};
  auto n = 64-bm::clz(A.size()-1);
  return SubsetMobius(n, A.begin());
}

/* Get \mobius A, the reverse transformation of \zeta A
*/
template<typename T>
void SubsetMobiusInline(std::vector<T>& A) {
  if (A.empty()) return;
  int n = 64-bm::clz(A.size()-1);
  SubsetMobiusImpl(n, A.begin());
}

template<typename T>
std::vector<T> BitwiseOrConvolution(const std::vector<T>& A, const std::vector<T>& B) {
  auto zA = SubsetZeta(A);
  auto zB = SubsetZeta(B);
  for (size_t i = 0; i < zA.size(); i++) {
    zA[i] *= zB[i];
  }
  SubsetMobiusInline(zA);
  return zA;
}

template<typename T>
void BitwiseOrConvolutionInline(std::vector<T>& A, std::vector<T>& B) {
  auto& zA = A;
  auto& zB = B;
  SubsetZetaInline(zA);
  SubsetZetaInline(zB);
  for (size_t i = 0; i < zA.size(); i++) {
    zA[i] *= zB[i];
  }
  SubsetMobiusInline(zA);
}

template<typename T>
std::vector<T> BitwiseAndConvolution(const std::vector<T>& A, const std::vector<T>& B) {
  int n = 64-bm::clz(std::max(A.size(), B.size())-1);
  std::vector<T> rA(1<<n), rB(1<<n);
  std::copy(A.begin(), A.end(), rA.begin());
  std::copy(B.begin(), B.end(), rB.begin());
  std::reverse(rA.begin(), rA.end());
  std::reverse(rB.begin(), rB.end());
  BitwiseOrConvolutionInline(rA, rB);
  std::reverse(rA.begin(), rA.end());
  return rA;
}