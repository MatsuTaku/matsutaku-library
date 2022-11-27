#define PROBLEM "https://atcoder.jp/contests/tenka1-2016-final-open/tasks/tenka1_2016_final_e"
#include "../include/mtl/convex_hull_trick.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
  int n,l; cin>>n>>l;
  vector<vector<int>> A(n, vector<int>(l));
  vector<long long> dp(l);
  // Cost of skewer at offset k by position j
  // = (j-k)^2 + A[i][k]
  // = j^2 - 2jk + k^2 + A[i][k]
  for (int i = 0; i < n; i++) {
    ConvexHullTrick<long long, greater<>> cht;
    for (int j = 0; j < l; j++) {
      cin>>A[i][j];
      cht.add(-2*j, j*j + A[i][j]);
    }
    for (int j = l-1; j >= 0; j--)
      dp[j] += cht.get(j) + j*j;
  }
  long long ans = *min_element(dp.begin(), dp.end());
  cout << ans << endl;
}