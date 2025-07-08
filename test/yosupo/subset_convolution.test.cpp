#define PROBLEM "https://judge.yosupo.jp/problem/subset_convolution"
#include "include/mtl/subset_convolution.hpp"
#include "include/mtl/modular.hpp"
using namespace std;
using mint = Modular998244353;

int main() {
  int n; cin>>n;
  vector<mint> A(1<<n), B(1<<n);
  for (int i = 0; i < 1<<n; i++) cin>>A[i];
  for (int i = 0; i < 1<<n; i++) cin>>B[i];
  auto C = SubsetConvolution(A,B);
  for (int i = 0; i < 1<<n; i++) cout<<C[i]<<' ';
  cerr<<endl;
} 