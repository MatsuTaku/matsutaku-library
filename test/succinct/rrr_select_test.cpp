#include <succinct/rrr.hpp>
#include <bits/stdc++.h>

static constexpr int u = 1e9;
static constexpr int n = 1e6;

int main() {
  std::vector<bool> B(u);
  int c = 0
  int k = 0;
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
  // select
  for (int i = 0; i < k; i++) {
    int v = bm.select<1>(i);
    if (v != select[i]) {
      std::cout << "Failed select: " << i << " bm.select " << v << " != select " << select[i] << std::endl;
      return 1;
    }
  }
  // // select0
  // for (int i = 0; i < n-k; i++) {
  //   int v = bm.select<0>(i);
  //   if (v != select0[i]) {
  //     std::cout << "Failed select0: " << i << " bm.select0 " << v << " != select0 " << select0[i] << std::endl;
  //     return 1;
  //   }
  // }
  std::cout << "OK" << std::endl;

}
