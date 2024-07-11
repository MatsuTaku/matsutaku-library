#define STANDALONE
#include "../../include/mtl/succinct/bit_vector.hpp"
#include <bits/stdc++.h>

int main() {
  const int n = 1e6;
  std::vector<bool> B(n);
  int c = 0;
  int k = 0;
  std::vector<int> rank(n), select(n), select0(n);
  for (int i = 0; i < n; i++) {
    rank[i] = c;
    B[i] = rand()%2;
    c += B[i];
    if (B[i]) select[k++] = i;
    else select0[i-k] = i;
  }

  BitVector bm(B.begin(), B.end());
  bm.build();
  // rank
  for (int i = 0; i < n; i++) {
    int v = bm.rank(i);
    if (v != rank[i]) {
      std::cout << "Failed rank: " << i << " bm.rank " << v << " != rank " << rank[i] << std::endl;
      return 1;
    }
  }
  std::cout << "OK" << std::endl;

}
