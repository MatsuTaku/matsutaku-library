#define STANDALONE
#include "../../include/mtl/skewbinary_list.hpp"

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <random>

int main() {
  const int Max = 1e5;
  int n = Max;
  std::vector<int> values(n);
  for (int& v : values) v = rand();

//  PersistentSkewbinaryList<int> S;
//  for (int v : values) {
//    S = S.pushed(v);
//  }
//  std::reverse(values.begin(), values.end());
  PersistentSkewbinaryList<int> S(values.begin(), values.end());

//  S.print_for_debug();

  { // Test random-access
//    std::cout << "random-access" << std::endl;
    for (int i = 0; i < S.size(); i++) {
      int get = S.get_at(i);
      if (get != values[i]) {
        std::cout << "get " << i << " " << get << " != " << values[i] << std::endl;
        assert(false);
        return 1;
      }
    }
  }

  { // Test pop
//    std::cout << "pop" << std::endl;
    auto P = S;
    int popcount = 0;
    while (!P.empty()) {
      auto get = P.front();
      if (get != values[popcount]) {
        std::cout << "pop " << popcount << " " << get << " != " << values[popcount] << std::endl;
        assert(false);
        return 1;
      }
      P = P.popped();
      popcount++;
    }
  }

  { // Test drop
//    std::cout << "drop" << std::endl;
    auto O = S;
    int dropcount = 0;
    std::uniform_real_distribution<double> dist(0,1);
    std::default_random_engine eng;
    while (!O.empty()) {
      auto get = O.front();
      if (get != values[dropcount]) {
        std::cout << "drop " << dropcount << " " << get << " != " << values[dropcount] << std::endl;
        assert(false);
        return 1;
      }
      int off = std::round((std::exp(dist(eng))-1) * O.size());
      off = std::min(std::max(off, 1), (int)O.size());
      O = O.dropped(off);
      dropcount += off;
    }
  }

  { // Test update
    for (int t = 0; t < 100; t++) {
      int idx = rand() % n;
      values[idx] = rand();
      S = S.updated_at(idx, values[idx]);
      for (int i = 0; i < n; i++) {
        auto get = S.get_at(i);
        if (get != values[i]) {
          std::cout << "update " << idx << " " << get << " != " << values[i] << std::endl;
          assert(false);
          return 1;
        }
      }
    }
  }

  { // Test iterator
//    std::cout << "iter" << std::endl;
    int i = 0;
    for (auto v : S) {
      if (v != values[i]) {
        std::cout << "iter get " << i << " " << v << " != " << values[i] << std::endl;
        assert(false);
        return 1;
      }
      ++i;
    }
  }

  std::cout << "OK" << std::endl;
}
