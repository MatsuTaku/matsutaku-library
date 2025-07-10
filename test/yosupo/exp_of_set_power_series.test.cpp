#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_set_power_series"
#include "include/mtl/sps.hpp"
#include "include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;
using mint = Modular998244353;

int main() {
  int n; cin>>n;
  vector<mint> B(1<<n);
  for (int i = 0; i < 1<<n; i++) cin>>B[i];
  auto C = SpsExp<mint, 20>(n, B);
  for (auto c:C) cout<<c<<' ';
  cout<<endl;
}