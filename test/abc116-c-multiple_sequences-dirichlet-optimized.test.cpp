#define PROBLEM "https://atcoder.jp/contests/arc116/tasks/arc116_c"
#include "include/mtl/dirichlet.hpp"
#include "include/mtl/modular.hpp"
#include "include/mtl/enumerate.hpp"
#include <bits/stdc++.h>
using namespace std;
using mint = Modular998244353;

int main() {
  int n,m; cin>>n>>m;
  constexpr int max_logn = 30;
  auto primes = Primes(m);
  // Sum of zeta^k for m
  auto f = Identity<mint>(m);
  vector<mint> A(max_logn);
  for (int k = 0; k < max_logn; k++) {
    A[k] = f.second[1];
    f = DirichletConvolveZeta(m, primes, f);
  }
  // Sum of (zeta-1)^k for m
  vector<mint> B(max_logn);
  Enumerate<mint> enm;
  for (int k = 0; k < max_logn; k++) {
    for (int i = 0; i <= k; i++) {
      auto coef = enm.cmb(k, i);
      if ((k-i)%2) coef = -coef;
      B[k] += A[i] * coef;
    }
  }
  // ans = sum_k B[k] binom(n, k)
  mint binom = 1;
  mint ans = 0;
  for (int k = 0; k < max_logn; k++) {
    ans += binom * B[k];
    binom *= mint(n-k) * mint(k+1).inv();
  }
  cout << ans << endl;
}