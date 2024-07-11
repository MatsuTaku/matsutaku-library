#define STANDALONE
#include "include/mtl/succinct/rrr.hpp"
#include <bits/stdc++.h>

static constexpr int u = 1e9;
static constexpr int n = 1e6;

int main() {
  int c = 0;
  std::vector<int> select(n);
  std::map<int,int> _rank;
  auto rank = [&](int i) { return _rank.lower_bound(i)->second; };
  auto pop_rate = u/n;
  for (int i = 0; i < u; i++) {
    if (rand()%pop_rate == 0) {
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
  typename RankSelectTraits<decltype(bm)>::rank_select_type rs(&bm);
  // select
  for (int i = 0; i < n; i++) {
    int v = rs.select1(i);
    if (v != select[i]) {
      std::cout << "Failed select1: " << i << " bm.select " << v << " != select1 " << select[i] << std::endl;
      return 1;
    }
  }
  // select0
  // for (int i = 0; i < u-n; i++) {
  //   int v = rs.select0(i);
  //   if (v != select0[i]) {
  //     std::cout << "Failed select0: " << i << " bm.select0 " << v << " != select0 " << select0[i] << std::endl;
  //     return 1;
  //   }
  // }
  std::cout << "OK" << std::endl;

}
