#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template <typename T>
T plus(T a, T b) {
  return a + b;
}

int main() {
  cin.tie(nullptr); ios::sync_with_stdio(false);

  ll a,b; cin>>a>>b;
  cout << ::plus(a,b) << endl;

  return 0;
}
