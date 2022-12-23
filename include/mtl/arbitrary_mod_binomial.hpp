#pragma once
#include <vector>
#include <array>
#include <type_traits>

namespace math {

template<class T>
constexpr bool is_coprime(T a, T b) {
  using type = typename std::remove_const<typename std::remove_reference<T>::type>::type;
  type c = a;
  type d = b;
  while (d) {
    std::swap(c,d);
    d %= c;
  }
  return c == 1;
}

} // namespace

/* \binom(N, m) mod M
 * @param N, M
 * @complexity
 *   - preprocess: O(N \log M / \log\log M)
 *   - query:      O(1)
 * @example
 *   int binom_N_m_mod_M = ArbitraryModBinomial(N,M)(m);
 */
struct ArbitraryModBinomial {
  static constexpr int MAX_PRIME_FACTOR_NUM = 9;
  using buffer_type = std::array<int, MAX_PRIME_FACTOR_NUM+1>;
  bool enabled_;
  int n_, mod_;
  constexpr bool enabled() const { return enabled_; }
  constexpr int mod() { return mod_; }
  std::vector<int> st_;
  constexpr ArbitraryModBinomial(int n, int mod)
    : enabled_(math::is_coprime(n, mod)), n_(n), mod_(mod), st_((n+1)/2) {
    if (!enabled) return;
    std::vector<bool> isn_p(n+1);
    buffer_type P{};
    std::vector<int> d(n+1), inv(n+1);
    int k = 0;
    for (int i = 2; i <= n; i++) {
      if (isn_p[i]) continue;
      for (int j = i+i; j <= n; j += i)
        isn_p[j] = 1;
      if (mod() % i) continue;
      P[++k] = prime;
      for (int v = prime; v <= n; v += prime)
        d[v] = k;
    }
    using mint = Modular<mod()>;
    int mid = (n+1)/2;
    std::vector<int> inv(mid+1);
    inv[mid] = mint(mid).inv().val();
    for (int i = mid-1; i > 0; i--)
      inv[i] = (long long) inv[i+1] * (i+1) % mod();
    int h = k+1;
    int w = n/(P[1]-1) + 1;
    std::vector<int> ptb(h * w);
    for (int i = 1; i <= MAX_PRIME_FACTOR_NUM; i++) {
      int t = n / (P[i]-1) + 1;
      int v = 1;
      ptb[i*h] = v;
      for(int j = 1; j < t; j++) {
        v = v * P[i] % mod();
        ptb[i*h+j] = v;
      }
    }
    int s = 1, ns = 1;
    buffer_type t{}, nt{};
    S[0] = T[0] = 1;
    /* important equation
     * \binom(n,k) = (n-k+1) / k * \binom(n,k-1)
     */
    auto make_st = [&](int k, buffer_type& q) {
      std::fill(q.begin(), q.end(), 0);
      if (k == 0)
        return 1;
      int x = k;
      while (d[x]) {
        q[d[x]]++;
        x /= P[d[x]];
      }
      return x;
    };
    for (int k = 1; k <= mid; k++) {
      s = (long long) s * ns % mod();
      for (int i = 1; i <= MAX_PRIME_FACTOR_NUM; i++)
        t[i] += nt[i];
      ns = make_st(k, nt);
      s = (long long) s * inv[ns] % mod();
      for (int i = 1; i <= MAX_PRIME_FACTOR_NUM; i++)
        t[i] -= nt[i];
      st_[i] = s;
      for (int i = 0; i <= MAX_PRIME_FACTOR_NUM; i++)
        st_[i] = (long long) st_[i] * ptb[i][t[i]] % mod();
    }
  }
  constexpr inline int operator(int m) const {
    assert(enabled());
    if (m*2 > n_)
      return operator()(n_-m);
    return st_[m];
  }
};
