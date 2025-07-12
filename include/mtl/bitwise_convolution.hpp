#pragma once
#include <vector>
#include <cstddef>
#include "bit_manip.hpp"

/* Get \zeta A
 * defined: [x^s]\zeta A = \sum_{t \in s} A_t  
*/
template<class T>
std::vector<T> SubsetZeta(const std::vector<T>& A) {
  if (A.empty()) return {};
  auto n = 64-bm::clz(A.size()-1);
  std::vector<T> zA(1<<n);
  std::copy(A.begin(), A.end(), zA.begin());
  for (int i = 0; i < n; i++) {
    auto w = 1<<i;
    for (int p = 0; p < 1<<n; p += 2*w) {
      for (int s = p; s < p+w; s++) {
        int t = s|w;
        zA[t] += zA[s];
      }
    }
  }
  return zA;
}

/* Get \zeta A
 * defined: [x^s]\zeta A = \sum_{t \in s} A_t  
*/
template<class T>
void SubsetZetaInline(std::vector<T>& A) {
  if (A.empty()) return;
  int n = 64-bm::clz(A.size()-1);
  for (int i = 0; i < n; i++) {
    auto w = 1<<i;
    for (int p = 0; p < 1<<n; p += 2*w) {
      for (int s = p; s < p+w; s++) {
        int t = s|w;
        A[t] += A[s];
      }
    }
  }
}

/* Get \mobius A, the reverse transformation of \zeta A
*/
template<class T>
std::vector<T> SubsetMobius(const std::vector<T>& A) {
  if (A.empty()) return {};
  auto n = 64-bm::clz(A.size()-1);
  std::vector<T> zA(1<<n);
  std::copy(A.begin(), A.end(), zA.begin());
  for (int i = 0; i < n; i++) {
    auto w = 1<<i;
    for (int p = 0; p < 1<<n; p += 2*w) {
      for (int s = p; s < p+w; s++) {
        int t = s|w;
        zA[t] -= zA[s];
      }
    }
  }
  return zA;
}

/* Get \mobius A, the reverse transformation of \zeta A
*/
template<class T>
void SubsetMobiusInline(std::vector<T>& A) {
  if (A.empty()) return;
  int n = 64-bm::clz(A.size()-1);
  for (int i = 0; i < n; i++) {
    auto w = 1<<i;
    for (int p = 0; p < 1<<n; p += 2*w) {
      for (int s = p; s < p+w; s++) {
        int t = s|w;
        A[t] -= A[s];
      }
    }
  }
}

template<class T>
std::vector<T> BitwiseOrConvolution(const std::vector<T>& A, const std::vector<T>& B) {
  auto zA = SubsetZeta(A);
  auto zB = SubsetZeta(B);
  for (size_t i = 0; i < zA.size(); i++) {
    zA[i] *= zB[i];
  }
  return SubsetMobius(zA);
}

template<class T>
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

template<class T>
void BitFlippedRearrangeInline(std::vector<T>& A) {
  int mask = A.size()-1;
  for (size_t i = 0; i < A.size()/2; i++) {
    std::swap(A[i], A[(~i)&mask]);
  }
}

template<class T>
std::vector<T> BitwiseAndConvolution(const std::vector<T>& A, const std::vector<T>& B) {
  int n = 64-bm::clz(std::max(A.size(), B.size())-1);
  std::vector<T> rA(1<<n), rB(1<<n);
  std::copy(A.begin(), A.end(), rA.begin());
  std::copy(B.begin(), B.end(), rB.begin());
  BitFlippedRearrangeInline(rA);
  BitFlippedRearrangeInline(rB);
  BitwiseOrConvolutionInline(rA, rB);
  BitFlippedRearrangeInline(rA);
  return rA;
}