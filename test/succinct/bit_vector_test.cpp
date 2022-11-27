#include <succinct/bit_vector.hpp>
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
  // get
  for (int i = 0; i < n; i++) {
    auto v = bm[i];
    if (v != B[i]) {
      std::cout << "Failed get: " << i << " bm.get " << v << " != B " << B[i] << std::endl;
      return 1;
    }
  }
  // rank
  for (int i = 0; i < n; i++) {
    int v = bm.rank(i);
    if (v != rank[i]) {
      std::cout << "Failed rank: " << i << " bm.rank " << v << " != rank " << rank[i] << std::endl;
      return 1;
    }
  }
  // select
  for (int i = 0; i < k; i++) {
    int v = bm.select<1>(i);
    if (v != select[i]) {
      std::cout << "Failed select: " << i << " bm.select " << v << " != select " << select[i] << std::endl;
      return 1;
    }
  }
  // select0
  for (int i = 0; i < n-k; i++) {
    int v = bm.select<0>(i);
    if (v != select0[i]) {
      std::cout << "Failed select0: " << i << " bm.select0 " << v << " != select0 " << select0[i] << std::endl;
      return 1;
    }
  }
  std::cout << "OK" << std::endl;

}
