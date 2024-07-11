#define STANDALONE
#include "../../include/mtl/enumerate.hpp"
#include "../../include/mtl/modular.hpp"

#include <iostream>

using namespace std;
using ll = long long;

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  Enumerate<Modular<int(1e9) + 7>> enm(1e5);
  auto a = enm.cmb(4, 2);
  if (a != 6) {
    cerr << "Error cmb(4,2) is not equal to " << a << endl;
    assert(false);
  }
  auto b = enm.cmb(5, 3);
  if (b != 10) {
    cerr << "Error cmb(5,3) is not equal to " << b << endl;
    assert(false);
  }
  auto c = enm.cmb(6, 3);
  if (c != 20) {
    cerr << "Error cmb(6,3) is not equal to " << c << endl;
    assert(false);
  }

  return 0;
}
