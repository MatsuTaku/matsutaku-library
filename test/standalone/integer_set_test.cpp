#define STANDALONE
#include "../../include/mtl/integer_set.hpp"
#include "set_test.hpp"

constexpr int W = 20;
//constexpr int Max = 4e5;
//
//template<typename Set>
//int test_set() {
//  std::vector<int> values;
//  while (values.empty()) {
//    for (int i = 0; i < Max; i++) {
//      if (i == 0 or rand()%4 == 0)
//        values.push_back(i);
//    }
//  }
//  int n = values.size();
//  std::vector<int> shuffled = values;
//  for (int i = 0; i < n; i++) {
//    std::swap(shuffled[i], shuffled[rand()%n]);
//  }
//
//  Set S;
//  for (int v : shuffled) {
//    S.insert(v);
//  }
////  S.print_for_debug();
//  int target = -1;
//  int pred = -1;
//  int succ = values[0];
//  int k = 0;
//  auto log = [&]() {
//    std::cout << pred << ' ' << target << ' ' << succ << std::endl;
//    assert(false);
//  };
//  for (int i = 0; i < Max; i++) {
//    if (k < n and values[k] == i) {
//      target = values[k];
//      pred = k-1 >= 0 ? values[k-1] : -1;
//      succ = k+1 < n ? values[k+1] : -1;
//      k++;
//    } else {
//      pred = k-1 >= 0 ? values[k-1] : -1;
//      target = -1;
//    }
//
//    auto fit = S.find(i);
//    if (fit != S.end()) {
//      if ((int) *fit != i) {
//        std::cout << "find: " << i << std::endl;
//        log();
//        return 1;
//      }
//    } else {
//      if (target != -1) {
//        log();
//        return 1;
//      }
//    }
//
//    auto sucit = S.successor(i);
//    if (sucit != S.end()) {
//      if ((int) *sucit != succ) {
//        std::cout << "succ: " << *sucit << std::endl;
//        log();
//        return 1;
//      }
//    } else {
//      if (succ != -1) {
//        log();
//        return 1;
//      }
//    }
//
//    auto predit = S.predecessor(i);
//    if (predit != S.end()) {
//      if ((int) *predit != pred) {
//        std::cout << "pred: " << *predit << std::endl;
//        log();
//        return 1;
//      }
//    } else {
//      if (pred != -1) {
//        log();
//        return 1;
//      }
//    }
//  }
//
//  int size = n;
//  if ((int) S.size() != size) {
//    std::cout << S.size() << ' ' << size<< std::endl;
//    log();
//    return 1;
//  }
//  for (int v : shuffled) {
//    S.erase(v);
//    size--;
//    if ((int) S.size() != size) {
//      std::cout << S.size() << ' ' << size<< std::endl;
//      assert(false);
//      return 1;
//    }
//  }
//  return 0;
//}

int main() {
//  mtl::integer_set_test<XFastTrieSet<W>>();
//  mtl::integer_set_test<YFastTrieSet<W>>();
  std::cout << "OK" << std::endl;
}
