#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"
#include "../include/mtl/fps.hpp"
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int main() {
  cin.tie(nullptr); ios::sync_with_stdio(false);

  int N,M; cin>>N>>M;
  Fps A(N), B(M);
  for (auto& a : A) cin>>a;
  for (auto& b : B) cin>>b;
  auto C = A * B;
  for (int i = 0; i < N+M-1; i++)
    cout << C[i] << " ";
  cout << endl;

  return 0;
}
