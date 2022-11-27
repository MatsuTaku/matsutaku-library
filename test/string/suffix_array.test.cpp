#define PROBLEM "https://judge.yosupo.jp/problem/suffixarray"
#include "../../cpl/string/suffix_array.hpp"
#include <iostream>
using namespace std;

int main() {
  string s; cin>>s;
  auto sa = SuffixArray(s.begin(), s.end());
  for (auto v : sa) cout << v << ' ';
  cout << endl;
}