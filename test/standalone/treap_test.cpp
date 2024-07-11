#define STANDALONE
#include "include/mtl/treap.hpp"

#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include <algorithm>
#include "set_test.hpp"

void test_insert() {
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
      if ((int)s.size() != i+1) {
        std::cout << "insert " << t << "," << i << ": " << s.size() << " < " << i+1 << std::endl;
        assert(false);
        exit(EXIT_FAILURE);
      }
      int k = 0;
      for (auto x : s) {
        if (x != v[k]) {
          std::cout << "insert " << t << "," << i << ' ' << k << ": " << x << " != " << v[k] << std::endl;
          assert(false);
          exit(EXIT_FAILURE);
        }
        k++;
      }
    }
  }
}

void test_split() {
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
        exit(EXIT_FAILURE);
      }
      k++;
    }
    for (auto x : r) {
      if (x != v[k]) {
        std::cout << "split " << t << ",right," << k << ": " << x << " != " << v[k] << std::endl;
        assert(false);
        exit(EXIT_FAILURE);
      }
      k++;
    }
  }
}

void test_absorb() {
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
        exit(EXIT_FAILURE);
      }
      k++;
    }
  }
}

void unit_test() {
  test_insert();
  test_split();
  test_absorb();
}

int main() {
  unit_test();
  mtl::integer_set_test<Treap<int>>();
  std::cout << "OK" << std::endl;
}
