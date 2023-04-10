#include <succinct/rrr.hpp>
#include <bits/stdc++.h>

static constexpr int u = 1e9;
static constexpr int n = 1e6;

int main() {
  std::vector<bool> B(u);
  int c = 0;
  std::vector<int> select(n);
  std::map<int,int> _rank;
  auto rank = [&](int i) { return *_rank.lower_bound(i); };
  auto pop_rate = u/n;
  for (int i = 0; i < u; i++) {
    if (rand()%pop_rate == 0) {
      B[i] = 1;
      _rank[i] = c;
      select[c] = i;
      ++c;
    }
  }
  _rank[u] = c;

  RRR<> bm;
  for (auto [i,_]:_rank)
    bm.set(i, 1);
  bm.build();
  // rank
  for (int i = 0; i < u; i++) {
    int v = bm.rank(i);
    if (v != rank(i)) {
      std::cout << "Failed rank: " << i << " bm.rank " << v << " != rank " << rank(i) << std::endl;
      return 1;
    }
  }
  std::cout << "OK" << std::endl;

}
