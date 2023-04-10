#include <succinct/rrr.hpp>
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
  typename decltype(bm)::rs_type rs(bm);
  // rank
  for (auto it = _rank.begin(); it != _rank.end();) {
    auto [i,r] = *it;
    auto v = rs.rank1(i);
    if (v != r) {
      std::cout << "Failed rank1: " << i << " rs.rank1 " << v << " != rank1 " << r << std::endl;
      return 1;
    }
    ++it;
    if (it != _rank.end()) {
      auto [_,r] = *it;
      auto v = rs.rank1(i+1);
      if (v != r) {
        std::cout << "Failed rank1: " << i+1 << " rs.rank1 " << v << " != rank1 " << r << std::endl;
        return 1;
      }
    }
  }
  std::cout << "OK" << std::endl;

}
