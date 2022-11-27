#include <fibonacci_heap.hpp>

#include <vector>
#include <numeric>
#include <random>
#include <iostream>
#include <queue>
#include <set>

using P = std::pair<int,int>;
using Fh = FibonacciHeap<int, std::greater<>>;

int main() {
  int size = 1<<8;
  std::vector<P> num(size);
  std::vector<int> V(size);
  for (int i = 0; i < size; i++) {
    num[i] = {i, i};
    V[i] = i;
  }
  for (int i = 0; i < size; i++) {
    std::swap(num[i], num[rand()%size]);
  }

  std::priority_queue<P, std::vector<P>, std::greater<>> pq;
  Fh fh(size);
  for (auto p : num) {
    pq.push(p);
    fh.push(p.first, p.second);
  }
  std::set<int> mex;
  std::vector<bool> ex(size, 1);
  for (int i = 0; i < size; i++) {
    if (!mex.empty() and rand()%8==0) {
      int cnt = rand()%mex.size();
      for (int c = 0; c < cnt; c++) {
        auto j = rand()%size;
        if (ex[j]) {
          auto v = *mex.begin();
          if (v < V[j]) {
            mex.erase(mex.begin());
            pq.push({v, j});
            fh.update(j, v);
            V[j] = v;
          }
        }
      }
    }
    auto pqtop = pq.top(); pq.pop();
    while (pqtop.first != V[pqtop.second]) {
      pqtop = pq.top(); pq.pop();
    }
    auto fhtop = fh.top(); fh.pop();
    if (V[fhtop.first] != fhtop.second or
        std::make_pair(fhtop.second, fhtop.first) != pqtop) {
      std::cout << pqtop.first << ", " << pqtop.second << " != " << fhtop.second << ", " << fhtop.first << std::endl;
      return 1;
    }
    mex.insert(fhtop.second);
    ex[fhtop.first] = false;
  }

  std::cout << "OK" << std::endl;
}
