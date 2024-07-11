#define STANDALONE
#include "../../include/mtl/skiplist.hpp"

#include <iostream>
#include <vector>
#include <cassert>

int main() {
  const int Max = 4e5;
  std::vector<int> values;
  while (values.empty()) {
    for (int i = 0; i < Max; i++)
      if (rand()%4 == 0)
        values.push_back(i);
  }
  int n = values.size();
  std::vector<int> shuffled = values;
  for (int i = 0; i < n; i++) {
    std::swap(shuffled[i], shuffled[rand()%n]);
  }

  SkiplistSet<int> S;
  for (int v : shuffled) {
    S.insert(v);
  }
//  S.print_for_debug();
  for (int i = 0; i < n; i++) {
    if (*S.get_at(i) != values[i]) {
      std::cout << "get " << i << " " << *S.get_at(i) << " != " << values[i] << std::endl;
      assert(false);
      return 1;
    }
  }
  int target = -1;
  int pred = -1;
  int succ = values[0];
  int k = 0;
  auto log = [&]() {
    std::cout << pred << ' ' << target << ' ' << succ << std::endl;
    assert(false);
  };
  for (int i = 0; i < Max; i++) {
    if (k < n and values[k] == i) {
      target = values[k];
      pred = k-1 >= 0 ? values[k-1] : -1;
      succ = k+1 < n ? values[k+1] : -1;
      k++;
    } else {
      pred = k-1 >= 0 ? values[k-1] : -1;
      target = -1;
    }

    auto fit = S.find(i);
    if (fit != S.end()) {
      if (*fit != i) {
        std::cout << "find: " << i << std::endl;
        log();
        return 1;
      }
    } else {
      if (target != -1) {
        log();
        return 1;
      }
    }

    auto sucit = S.successor(i);
    if (sucit != S.end()) {
      if (*sucit != succ) {
        std::cout << "succ: " << *sucit << std::endl;
        log();
        return 1;
      }
    } else {
      if (succ != -1) {
        log();
        return 1;
      }
    }

    auto predit = S.predecessor(i);
    if (predit != S.end()) {
      if (*predit != pred) {
        std::cout << "pred: " << *predit << std::endl;
        log();
        return 1;
      }
    } else {
      if (pred != -1) {
        log();
        return 1;
      }
    }
  }

  int size = n;
  if ((int) S.size() != size) {
    std::cout << S.size() << ' ' << size<< std::endl;
    log();
    return 1;
  }
  for (int v : shuffled) {
    S.erase(v);
    size--;
    if ((int) S.size() != size) {
      std::cout << S.size() << ' ' << size<< std::endl;
      log();
      return 1;
    }
  }

  std::cout << "OK" << std::endl;
}
