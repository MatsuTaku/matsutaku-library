#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2603"
#include "../include/mtl/convex_hull_trick.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
  int s,n,m; cin>>s>>n>>m;
  vector<int> X(s);
  for (int i = 0; i < s; i++) cin>>X[i];
  vector<pair<int,int>> B(n);
  vector<int> T(n);
  for (int i = 0; i < n; i++) {
    int t,p; cin>>t>>p; p--;
    B[i] = {t, X[p]};
    T[i] = t-X[p];
  }
  sort(T.begin(), T.end());
  vector<int> TS(n+1);
  for (int i = 1; i <= n; i++)
    TS[i] = TS[i-1] + T[i-1];
  // dp[r] = min_{0<=l<=r} dp[l] + (r-l)*T[r-1] - (TS[r]-TS[l])
  //       = min           dp[l] - (l*T[r-1] - TS[l]) + r*T[r-1] - TS[r]
  constexpr int INF = 1e9;
  vector<int> dp(n+1, INF);
  dp[0] = 0;
  ConvexHullTrickDeque<int, greater<>> cht;
  for (int i = 0; i < m; i++) {
    cht.clear();
    cht.push_back(-0, dp[0] + TS[0]);
    for (int j = 1; j <= n; j++) {
      auto x = T[j-1];
      cht.push_back(-j, dp[j] + TS[j]);
      dp[j] = cht.get(x) + j * x - TS[j];
    }
  }
  cout << dp[n] << endl;
}