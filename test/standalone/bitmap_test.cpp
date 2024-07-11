#define STANDALONE
#include "include/mtl/bitmap.hpp"
#include <iostream>

int main() {
  const int n = 1e6;
  std::vector<bool> B(n);
  int c = 0;
  int k = 0;
  std::vector<int> rank(n), select(n);
  for (int i = 0; i < n; i++) {
    rank[i] = c;
    B[i] = rand()%2;
    c += B[i];
    if (B[i]) select[k++] = i;
  }

  Bitmap bm(B.begin(), B.end());
  // get
  for (int i = 0; i < n; i++) {
    auto v = bm.get(i);
    if (v != B[i]) {
      std::cout << "Failed get: " << i << " bm.get " << v << " != B " << B[i] << std::endl;
      return 1;
    }
  }
  // rank
  for (int i = 0; i < n; i++) {
    auto v = bm.rank(i);
    if (v != rank[i]) {
      std::cout << "Failed rank: " << i << " bm.rank " << v << " != rank " << rank[i] << std::endl;
      return 1;
    }
  }
  // select
  for (int i = 1; i < k; i++) {
    auto v = bm.select(i);
    if (v != select[i]) {
      std::cout << "Failed select: " << i << " bm.select " << v << " != select " << select[i] << std::endl;
      return 1;
    }
  }
  std::cout << "OK" << std::endl;

}
