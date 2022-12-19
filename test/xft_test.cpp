#include <xft.hpp>

#include <iostream>
#include <vector>
#include <algorithm>
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
  std::random_shuffle(shuffled.begin(), shuffled.end());

  XFastTrieSet<unsigned> S;
  for (int v : shuffled) {
    S.insert(v);
  }
//  S.print_for_debug();
  int target = -1;
  int pred = -1;
  int succ = values[0];
  int k = 0;
  auto log = [&]() {
    std::cout << pred << ' ' << target << ' ' << succ << std::endl;
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
        exit(EXIT_FAILURE);
      }
    } else {
      if (target != -1) {
        log();
        exit(EXIT_FAILURE);
      }
    }

    auto sucit = S.upper_bound(i);
    if (sucit != S.end()) {
      if (*sucit != succ) {
        std::cout << "succ: " << *sucit << std::endl;
        log();
        return 1;
      }
    } else {
      if (succ != -1) {
        log();
        exit(EXIT_FAILURE);
      }
    }

    auto predit = S.lower_bound(i);
    if (predit != S.begin()) {
      --predit;
      if (*predit != pred) {
        std::cout << "pred: " << *predit << std::endl;
        log();
        exit(EXIT_FAILURE);
      }
    } else {
      if (pred != -1) {
        log();
        exit(EXIT_FAILURE);
      }
    }
  }

  int size = n;
  if ((int) S.size() != size) {
    std::cout << S.size() << ' ' << size<< std::endl;
    log();
    exit(EXIT_FAILURE);
  }
  for (int v : shuffled) {
    S.erase(v);
    size--;
    if ((int) S.size() != size) {
      std::cout << S.size() << ' ' << size<< std::endl;
      log();
      exit(EXIT_FAILURE);
    }
  }

  std::cout << "OK" << std::endl;
}
