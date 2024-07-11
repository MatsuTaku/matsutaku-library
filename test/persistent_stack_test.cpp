#define STANDALONE
#include "include/mtl/persistent_stack.hpp"

#include <vector>
#include <stack>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>

constexpr int N = 1e5;
int main() {
  std::vector<std::pair<int,int>> U(N);
  int sz = 0;
  for (int i = 0; i < N; i++) {
    int t = sz > 0 ? (rand()%3 != 0) : 1;
    if (t == 0) {
      U[i] = {t, 0};
      sz--;
    } else {
      U[i] = {t, rand()};
      sz++;
    }
  }
  std::vector<int> Q(N);
  for (int i = 0; i < N; i++) {
    Q[i] = rand() % N + 1;
  }
  std::vector<int> expected(N+1, -1);
  std::stack<int> st;
  for (int i = 0; i < (int)U.size(); i++) {
    if (U[i].first == 0) {
      st.pop();
    } else {
      st.push(U[i].second);
    }
    expected[i+1] = st.empty() ? -1 : st.top();
  }

  std::vector<PersistentStack<int>> ps(N+1);
  for (int i = 0; i < N; i++) {
    if (U[i].first == 0) {
      ps[i+1] = ps[i].pop();
    } else {
      ps[i+1] = ps[i].push(U[i].second);
    }
  }

  for (auto q : Q) {
    int v = ps[q].empty() ? -1 : ps[q].top();
    if (expected[q] != v) {
      std::cout << "expected[q] != ps[q].top(): " << expected[q] << ' ' << v << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  std::cout << "OK" << std::endl;
}
