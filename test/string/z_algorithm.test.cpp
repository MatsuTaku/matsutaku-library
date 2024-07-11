#define PROBLEM "https://judge.yosupo.jp/problem/zalgorithm"
#include "include/mtl/string/z_algorithm.hpp"
#include <iostream>
using namespace std;

int main() {
  string s; cin>>s;
  auto z = z_algorithm(s.begin(), s.end());
  for (int i = 0; i < (int) s.size(); i++)
    cout << z[i] << ' ';
  cout << endl;
}