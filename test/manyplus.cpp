#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template <typename T>
T plus(T a, T b) {
  return a + b;
}

int main() {
  cin.tie(nullptr); ios::sync_with_stdio(false);

  int T; cin >> T;
  for (int t = 0; t < T; t++) {
    ll A,B; cin>>A>>B;
    cout << ::plus(A,B) << endl;
  }

  return 0;
}
