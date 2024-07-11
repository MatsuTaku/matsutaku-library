#define PROBLEM "https://atcoder.jp/contests/arc116/tasks/arc116_c"
#define IGNORE
#include "include/mtl/dirichlet.hpp"
#include "include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;
using mint = Modular998244353;

int main() {
  int n,m; cin>>n>>m;
  auto z = Zeta<mint>(m);
  auto t = Identity<mint>(m);
  while (n) {
    if (n&1) {
      t = DirichletConvolveOptimal(m, z, t);
    }
    z = DirichletConvolveOptimal(m, z, z);
    n >>= 1;
  }
  cout << t.second[1] << endl;
}