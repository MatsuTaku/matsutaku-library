#define STANDALONE
#define IGNORE
#include "include/mtl/succinct/rrr.hpp"
#include "include/mtl/succinct/traits.hpp"
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
  // rank1
  size_t i = 0;
  size_t r = 0;
  _rank.emplace(u, c);
  for (auto [j,_]:_rank) {
    while (i <= j) {
      auto v = rs.rank1(i);
      if (v != r) {
        std::cout << "Failed rank1: " << i << " rs.rank1 " << v << " != rank1 " << r << std::endl;
        return 1;
      }
      ++i;
    }
    ++r;
  }
  // rank0;
  i = 0;
  r = 0;
  for (auto [j,_]:_rank) {
    while (i <= j) {
      auto v = rs.rank0(i);
      if (v != i-r) {
        std::cout << "Failed rank0: " << i << " rs.rank0 " << v << " != rank0 " << i-r << std::endl;
        return 1;
      }
      ++i;
    }
    ++r;
  }
  std::cout << "OK" << std::endl;

}
