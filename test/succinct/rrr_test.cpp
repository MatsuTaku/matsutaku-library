#define STANDALONE
#define IGNORE
#include "include/mtl/succinct/rrr.hpp"
#include <bits/stdc++.h>

static constexpr int u = 1e9;
static constexpr int n = 1e6;

int main() {
  std::vector<bool> B(u);
  int c = 0;
  std::vector<int> select(n);
  std::map<int,int> _rank;
  auto rank = [&](int i) { return _rank.lower_bound(i)->second; };
  auto pop_rate = u/n;
  for (int i = 0; i < u; i++) {
    if (rand()%pop_rate == 0) {
      B[i] = 1;
      _rank.emplace_hint(_rank.end(), i, c);
      select[c] = i;
      ++c;
    }
  }
  _rank[u] = c;

  RRR<> bm;
  for (auto [i,_]:_rank)
    bm.set(i, 1);
  bm.build();
  // get
  int k = 0;
  for (int i = 0; i < u; i++) {
    auto v = bm.get(i);
    if (v != B[i]) {
      std::cout << "Failed get: " << i << " bm.get " << v << " != B " << B[i] << std::endl;
      return 1;
    }
  }
  std::cout << "OK" << std::endl;

}
