#include <iostream>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <cassert>
#include <set>

namespace mtl {

using std::cout;
using std::cerr;
using std::endl;

template<class Map>
void map_emplace_test() {
  using key_type = typename Map::key_type;
  using mapped_type = typename Map::mapped_type;
  Map s;
  s.emplace(std::make_pair(key_type(), mapped_type()));
  s.emplace(key_type(), mapped_type());
}

template<class Set, int Max = (int)4e5, bool Shuffle = true>
void integer_set_test() {
  std::vector<int> values;
  while (values.empty()) {
    for (int i = 0; i < Max; i++)
      if (rand()%4 == 0)
        values.push_back(i);
  }
  int n = values.size();
  auto insertions = values;
  if constexpr (Shuffle)
    std::random_shuffle(insertions.begin(), insertions.end());

  Set S(insertions.begin(), insertions.end());

  if (values != std::vector<int>(S.begin(), S.end())) {
    cout << "after insert order broken" << endl;
    exit(EXIT_FAILURE);
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
      if ((int)*fit != i) {
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
      if ((int)*sucit != succ) {
        std::cout << "succ: " << *sucit << std::endl;
        log();
        exit(EXIT_FAILURE);
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
      if ((int)*predit != pred) {
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

  for (int v : insertions) {
    auto f = S.find(v);
    assert(f != S.end());
    auto p = f;
    auto m = std::next(f);
    for (int i = 0; i < 2 and p != S.begin(); i++)
      --p;
    for (int i = 0; i < 2 and m != S.end(); i++)
      ++m;
    std::vector<unsigned> o(p,m);
    o.erase(find(o.begin(), o.end(), v));
    S.erase(v);
    size--;

    {
      auto lb = S.lower_bound(v);
      auto p = lb, m = lb;
      for (int i = 0; i < 2 and p != S.begin(); i++)
        --p;
      for (int i = 0; i < 2 and m != S.end(); i++)
        ++m;
      if (o != std::vector<unsigned>(p,m)) {
        std::cout << n-size<<" after erase "<<v<<" order broken " << endl;
        for (auto v:o)
          cerr<<v<<' ';
        cerr<<endl;
        for (auto it = p; it != m; ++it) {
          cerr<<*it<<' ';
        }
        cerr<<endl;
        exit(EXIT_FAILURE);
      }
    }
    if ((int) S.size() != size) {
      std::cout << S.size() << ' ' << size<< std::endl;
      exit(EXIT_FAILURE);
    }
  }
  cerr<<"integer_set_test ok"<<endl;
}

}
