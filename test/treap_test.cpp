#include <treap.hpp>

#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include <algorithm>

int test_insert() {
  constexpr int n = 300;
  for (int t = 0; t < 300; t++) {
    std::set<int> _v{0,
                     std::numeric_limits<int>::max(),
                     std::numeric_limits<int>::min()};
    while (_v.size() < n)
      _v.insert(rand());
    std::vector<int> v(_v.begin(), _v.end());
    TreapSet<int> s;
    for (int i = 0; i < n; i++) {
      s.insert(v[i]);
      if (s.size() != i+1) {
        std::cout << "insert " << t << "," << i << ": " << s.size() << " < " << i+1 << std::endl;
        assert(false);
        return 1;
      }
      int k = 0;
      for (auto x : s) {
        if (x != v[k]) {
          std::cout << "insert " << t << "," << i << ' ' << k << ": " << x << " != " << v[k] << std::endl;
          assert(false);
          return 1;
        }
        k++;
      }
    }
  }
  return 0;
}

int test_split() {
  constexpr int n = 1000;
  for (int t = 0; t < 1000; t++) {
    std::set<int> _v;
    while (_v.size() < n)
      _v.insert(rand());
    std::vector<int> v(_v.begin(), _v.end());
    TreapSet<int> r(v.begin(), v.end());
    auto l = r.split(std::next(r.begin(), n/2));
    int k = 0;
    for (auto x : l) {
      if (x != v[k]) {
        std::cout << "split " << t << ",left," << k << ": " << x << " != " << v[k] << std::endl;
        assert(false);
        return 1;
      }
      k++;
    }
    for (auto x : r) {
      if (x != v[k]) {
        std::cout << "split " << t << ",right," << k << ": " << x << " != " << v[k] << std::endl;
        assert(false);
        return 1;
      }
      k++;
    }
  }
  return 0;
}

int test_absorb() {
  constexpr int n = 1000;
  for (int t = 0; t < 1000; t++) {
    std::set<int> _v;
    while (_v.size() < n)
      _v.insert(rand());
    std::vector<int> v(_v.begin(), _v.end());
    TreapSet<int> l(v.begin(), v.begin()+n/2);
    TreapSet<int> r(v.begin()+n/2, v.end());
    r.absorb(&l);
    int k = 0;
    for (auto x : r) {
      if (x != v[k]) {
        std::cout << t << ", " << k << ": " << x << " != " << v[k] << std::endl;
        assert(false);
        return 1;
      }
      k++;
    }
  }
  return 0;
}

int unit_test() {
  if (test_insert())
    return 1;
  if (test_split())
    return 1;
  if (test_absorb())
    return 1;
  return 0;
}

template<typename Set>
int test_set() {
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

  Set S;
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
      if ((int) *fit != i) {
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
      if ((int) *sucit != succ) {
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
      if ((int) *predit != pred) {
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
  return 0;
}

int main() {
  if (unit_test() != 0)
    return 1;
  if (test_set<Treap<int>>() != 0)
    return 1;
  std::cout << "OK" << std::endl;
}
