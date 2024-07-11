#define STANDALONE
#include "include/mtl/persistent_array.hpp"

#include <vector>
#include <stack>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>

constexpr int N = 1e5;
int main() {
  std::vector<std::pair<int,int>> U(N);
  for (int i = 0; i < N; i++) {
    U[i] = {rand()%N, rand()%N};
  }
  std::vector<std::pair<int,int>> Q(N);
  for (int i = 0; i < N; i++) {
    Q[i] = {rand()%N+1, rand()%N};
  }
  std::vector<int> expected(N+1, -1);
  std::vector<int> st(N);
  int k = 0;
  std::vector<int> qi(N);
  std::iota(qi.begin(), qi.end(), 0);
  std::sort(qi.begin(), qi.end(),
            [&](int l, int r) { return Q[l].first < Q[r].first; });
  for (int i = 0; i < (int)U.size(); i++) {
    st[U[i].first] = U[i].second;
    while (k < N and Q[qi[k]].first == i+1) {
      expected[qi[k]] = st[Q[qi[k]].second];
      k++;
    }
  }

  std::vector<PersistentArray<int>> ps(N+1);
  for (int i = 0; i < N; i++)
    ps[0] = ps[0].set(i, 0);
  for (int i = 0; i < N; i++) {
    ps[i+1] = ps[i].set(U[i].first, U[i].second);
  }

  for (int i = 0; i < (int)Q.size(); i++) {
    int v = ps[Q[i].first].get(Q[i].second);
    if (expected[i] != v) {
      std::cout << i<<", "<<Q[i].first<<", "<<Q[i].second<<" expected[i] != ps[Q[i].first].get(Q[i].second): " << expected[i] << ' ' << v << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  std::cout << "OK" << std::endl;
}
