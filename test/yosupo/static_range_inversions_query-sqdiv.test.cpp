#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"
#include <bits/stdc++.h>
using namespace std;
using lint = long long;

constexpr int N = 1e5;

int main() {
  int n,q; cin>>n>>q;
  vector<int> A(n);
  for (int i = 0; i < n; i++) {
    cin>>A[i];
  }
  vector<int> I(n);
  iota(I.begin(), I.end(), 0);
  stable_sort(I.begin(), I.end(), [&](int l, int r) {return A[l]<A[r];});
  int b = ceil(sqrt(n));
  int m = n/b+1;
  vector<vector<int>> v(m, vector<int>(b,n));
  vector<vector<int>> sorted_idx(m);
  for (int i = 0; i < n; i++) {
    v[I[i]/b][I[i]%b] = i;
    sorted_idx[I[i]/b].push_back(I[i]%b);
  }
  vector<vector<int>> pref_inv(m);
  vector<vector<lint>> block_inv(m, vector<lint>(m,0ll));
  vector<int> freq(n+1);
  vector<vector<int>> rank(m, vector<int>(n,0));
  for (int i = 0; i < m; i++) {
    lint inv = 0;
    pref_inv[i].push_back(0);
    for (int j = 0; j < b; j++) {
      for (int k = 0; k < j; k++) {
        if (v[i][k] > v[i][j])
          inv++;
      }
      pref_inv[i].push_back(inv);
    }
    if (i+1 < m) {
      block_inv[i+1][i] = inv;
      for (int j = i-1; j >= 0; j--) {
        auto ri = sorted_idx[i].begin();
        for (int k:sorted_idx[j]) {
          while (ri != sorted_idx[i].end() and v[i][*ri] < v[j][k]) {
            ++ri;
          }
          inv += ri-sorted_idx[i].begin();
        }
        block_inv[i+1][j] = block_inv[i][j] + inv;
      }
    }
    for (int j = 0; j < n; j++)
      rank[i][j+1] = rank[i][j] + freq[j];
    for (int j = 0; j < b; j++) {
      freq[v[i][j]]++;
    }
  }
  auto range_inversions_query = [&](int l, int r) {
    int lq = l/b, rq = r/b;
    int lr = l%b, rr = r%b;
    lint ans = 0;
    if (lq == rq) {
      ans = pref_inv[lq][rr] - pref_inv[lq][lr];
      int less_sum = 0;
      for (int i : sorted_idx[lq]) {
        if (i < lr) 
          ans -= less_sum;
        else if (i < rr)        
          less_sum++;
      }
    } else {
      ans = block_inv[rq][lq];
      for (int i = 0; i < lr; i++) {
        auto vli = v[lq][i];
        ans -= rank[rq][vli] - rank[lq][vli];
      }
      ans += lr*(lr-1)/2 - pref_inv[lq][lr];
      ans += pref_inv[rq][rr];
      ans += (rq-lq)*b*rr;
      for (int i = 0; i < rr; i++) {
        auto vri = v[rq][i];
        ans -= rank[rq][vri]-rank[lq][vri];
      }
      auto ri = sorted_idx[rq].begin();
      int less_sum = 0;
      for (int i:sorted_idx[lq]) if (i<lr) {
        while (ri != sorted_idx[rq].end() and v[rq][*ri] < v[lq][i]) {
          less_sum += *ri < rr;
          ++ri;
        }
        if (i < lr)
          ans -= less_sum;
      }
    }
    return ans;
  };
  for (int i = 0; i < q; i++) {
    int l,r; cin>>l>>r;
    cout << range_inversions_query(l, r) << endl;
  }
}
