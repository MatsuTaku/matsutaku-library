#define PROBLEM "https://judge.yosupo.jp/problem/bitwise_and_convolution"
#include "include/mtl/bitwise_convolution.hpp"
#include "include/mtl/modular.hpp"
#include <bits/stdc++.h>

using namespace std;
using mint = Modular998244353;

int main() {
  int n; cin>>n;
  vector<mint> A(1<<n), B(1<<n);
  for(int i=0; i < 1<<n; i++) cin>>A[i];
  for(int i=0; i < 1<<n; i++) cin>>B[i];
  auto C = BitwiseAndConvolution(A,B);
  for (int i = 0; i < 1<<n; i++) cout<<C[i]<<' ';
  cout<<endl;
}