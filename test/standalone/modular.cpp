#include "../include/mtl/modular.hpp"

#include <iostream>

using namespace std;
using ll = long long;

int main() {
  cin.tie(nullptr); ios::sync_with_stdio(false);

  constexpr int MOD = 1e9+7;
  using mint = Modular<MOD>;
  mint x = 34567;
  x += 4132;
  x -= 8952;
  x *= 1785;
  x /= 52;

  return 0;
}
