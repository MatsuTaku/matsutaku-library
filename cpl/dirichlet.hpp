#pragma once
#include <vector>
#include <cmath>
#include <cassert>

std::vector<int> EratosthenesSieve(const int n) {
  std::vector<int> p(n+1);
  if (n == 0)
    return p;
  p[1] = 1;
  for (int i = 2; i <= n; i++) {
    if (p[i] == 0) {
      p[i] = i;
      for (int j = i*2; j <= n; j += i) {
        if (p[j] == 0)
          p[j] = i;
      }
    }
  }
  return p;
}

std::vector<int> Primes(const int n) {
  std::vector<int> ps;
  auto era = EratosthenesSieve(n);
  for (int i = 2; i <= n; i++) {
    if (era[i] == i) {
      ps.push_back(i);
    }
  }
  return ps;
}

/* PseudoCode:
 *   D_c(s) = sum c(n) n^{-s} = sum_n sum_{ij=n} a(i)b(j) (ij)^{-s}
 * complexity: O(n log n)
 */
template<typename T>
std::vector<T> DirichletConvolution(const std::vector<T>& a, const std::vector<T>& b) {
  int n = (a.size()-1);
  std::vector<T> c(n+1);
  for (int i = 1; i <= n; i++) {
    int m = n / i;
    for (int j = 1; j <= m; j++)
      c[i * j] += a[i] * b[j];
  }
  return c;
}

/* PseudoCode:
 *   for p in primes:
 *     D_{a,p}(s) = sum_k a(p^k) p^{-ks} (p-part of D_a)
 *     D_b(s) <- D_b(s) D_{a,p}(s)
 * requirements:
 *   - Sequence a should be multinomial.
 *     D_a(s) = prod_p sum_k a(p^k) p^{-ks}
 * complexity: O(n log log n)
 */
template<typename T>
std::vector<T> DirichletMultinomialConvolution(const std::vector<T>& a, const std::vector<T>& b) {
  int n = (a.size()-1);
  auto c = b;
  c.resize(n+1);
  for (int p : Primes(n)) {
    int m = n/p;
    for (int i = m; i >= 1; i--) {
      int u = p * i;
      int q = p, j = i;
      while (true) {
        c[u] += a[q] * c[j];
        if (j % p != 0)
          break;
        q *= p;
        j /= p;
      }
    }
  }
  return c;
}

template<typename T>
std::pair<std::vector<T>, std::vector<T>> Identity(int n) {
  int k = std::pow(n, (double) 2 / 3);
  int l = (n + k - 1) / k;
  std::vector<T> a(k+1, 0), A(l+1, 1);
  a[1] = 1;
  A[0] = 0;
  return make_pair(a, A);
}

template<typename T>
std::pair<std::vector<T>, std::vector<T>> Zeta(int n) {
  int k = std::pow(n, (double) 2 / 3);
  int l = (n + k - 1) / k;
  std::vector<T> a(k+1, 1), A(l+1);
  a[0] = 0;
  for (int i = 1; i <= l; i++)
    A[i] = n / i;
  return make_pair(a, A);
}

template<typename T>
std::pair<std::vector<T>, std::vector<T>> DirichletConvolveOptimal(int N, const std::pair<std::vector<T>, std::vector<T>>& _a, const std::pair<std::vector<T>, std::vector<T>>& _b) {
  const auto &a = _a.first, &A = _a.second, &b = _b.first, &B = _b.second;
  int k = a.size()-1, l = A.size()-1;
  assert(k * l >= N);
  auto Alow = a;
  auto Blow = b;
  for (int i = 1; i <= k; i++)
    Alow[i] += Alow[i-1];
  auto getA = [&](int i) {
    return i <= k ? Alow[i] : A[N / i];
  };
  for (int i = 1; i <= k; i++)
    Blow[i] += Blow[i-1];
  auto getB = [&](int i) {
    return i <= k ? Blow[i] : B[N / i];
  };

  auto c = DirichletConvolution(a, b);

  std::vector<T> C(l+1);
  for (int j = 1; j <= l; j++) {
    int n = N / j;
    int m = sqrt(n);
    for (int i = 1; i <= m; i++) {
      C[j] += a[i] * getB(n / i);
      C[j] += (getA(n / i) - getA(m)) * b[i];
    }
  }
  return std::make_pair(c, C);
}

template<typename T>
std::pair<std::vector<T>, std::vector<T>> DirichletConvolveZeta(int N, const std::vector<int>& primes, const std::pair<std::vector<T>, std::vector<T>>& _a) {
  const auto &a = _a.first, &A = _a.second;
  int k = a.size()-1, l = A.size()-1;
  auto Alow = a;
  for (int i = 1; i <= k; i++)
    Alow[i] += Alow[i-1];
  auto getA = [&](int i) {
    return i <= k ? Alow[i] : A[N / i];
  };

  auto c = a;
  for (int p : primes) {
    int m = k / p;
    for (int i = 1; i <= m; i++) {
      c[p * i] += c[i];
    }
  }

  std::vector<T> C(l+1);
  for (int j = 1; j <= l; j++) {
    int n = N / j;
    int m = std::sqrt(n);
    for (int i = 1; i <= m; i++) {
      C[j] += a[i] * (n / i);
      C[j] += getA(n / i) - getA(m);
    }
  }
  return std::make_pair(c, C);
}
