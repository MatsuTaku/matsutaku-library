#pragma once

#include <vector>
#include <cassert>

template <typename MODULAR>
class Enumerate {
 public:
  using mint = MODULAR;
 private:
  int max_n_ = 1;
  std::vector<mint> fact_, ifact_;

  void _set_max_n(int n);

 public:
  Enumerate() : fact_({1, 1}), ifact_({1, 1}) {}
  explicit Enumerate(int n) : fact_(std::max(2, n+1)), ifact_(std::max(2, n+1)) {
    fact_[0] = fact_[1] = ifact_[0] = ifact_[1] = 1;
    _set_max_n(n);
  }

  mint cmb(int p, int q) {
    if (p < q) return 0;
    return fact(p) * ifact(q) * ifact(p-q);
  }

  mint prm(int p, int q) {
    if (p < q) return 0;
    return fact(p) * ifact(p-q);
  }

  mint fact(int p) {
    if (p > max_n_)
      _set_max_n(p);
    return fact_[p];
  }
  mint ifact(int p) {
    if (p > max_n_)
      _set_max_n(p);
    return ifact_[p];
  }

};

template<typename MODULAR>
void Enumerate<MODULAR>::_set_max_n(int n) {
  if (n <= max_n_)
    return;
  int nxtn = std::max(max_n_*2, n);
  fact_.resize(nxtn+1);
  ifact_.resize(nxtn+1);
  for (int i = max_n_+1; i <= nxtn; i++) {
    fact_[i] = fact_[i-1] * i;
  }
  ifact_[nxtn] = mint(1) / fact_[nxtn];
  for (int i = nxtn-1; i > max_n_; i--) {
    ifact_[i] = ifact_[i+1] * (i+1);
  }
  max_n_ = nxtn;
}

