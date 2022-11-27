#include <succinct/wavelet_tree.hpp>
#include <bits/stdc++.h>
using namespace std;

int main() {
  const int n = 1e6;
  std::vector<char> S(n);
  std::array<int, 26> cnt{};
  std::array<std::vector<int>, 26> rank, select, rank_c;
  for (int i = 0; i < 26; i++) {
    rank[i].resize(n+1);
    rank_c[i].resize(n+1);
  }
  for (int i = 0; i < n; i++) {
    int c = rand()%26;
    S[i] = c;
    for (int j = 0; j < 26; j++) {
      rank[j][i] = cnt[j];
      if (j) rank_c[j][i] = rank_c[j-1][i] + cnt[j-1];
    }
    cnt[c]++;
    select[c].push_back(i);
  }
  for (int i = 0; i < 26; i++) {
    rank[i][n] = cnt[i];
    if (i) rank_c[i][n] = rank_c[i-1][n] + cnt[i-1];
    select[i].push_back(n);
  }

  WaveletTree<char> wm(S.begin(), S.end());
  // access
  for (int i = 0; i < n; i++) {
    char v = wm.get(i);
    if (v != S[i]) {
      cout << "Failed access "<<i<<" wm.get: "<<char(v+'a')<<" != S: "<<char(S[i]+'a')<<endl;
      return 1;
    }
  }
  // rank
  for (int i = 0; i <= n; i++) {
    for (int c = 0; c < 26; c++) {
      int v = wm.rank(c, i);
      if (v != rank[c][i]) {
        cout << "Failed rank "<<char(c+'a')<<", "<<i<<" wm.rank: "<<v<<" != rank: "<<rank[c][i]<<endl;
        return 1;
      }
    }
  }
  // select
  std::array<int, 26> selected{};
  for (int i = 0; i < n; i++) {
    int c = S[i];
    int k = selected[c]++;
    int v = wm.select(c, k);
    if (v != select[c][k] or v != i) {
      cout << "Failed select "<<char(S[i]+'a')<<", "<<k<<" wm.select: "<<v<<" != select: "<<select[c][k]<<endl;
      return 1;
    }
  }
  for (int c = 0; c < 26; c++) {
    int k = selected[c];
    int v = wm.select(c, k);
    if (v != select[c][k] or v != n) {
      cout << "Failed select "<<char(c + 'a')<<", "<<k<<" wm.select: "<<v<<" != select: "<<select[c][k]<<endl;
      return 1;
    }
  }
  // rank_c
  for (int i = 0; i <= n; i++) {
    for (int c = 0; c < 26; c++) {
      int v = wm.rank_lower(c, i);
      if (v != rank_c[c][i]) {
        cout << "Failed rank_c "<<char(c+'a')<<", "<<i<<" wm.rank_lower: "<<v<<" != rank_c: "<<rank_c[c][i]<<endl;
        return 1;
      }
    }
  }
  // enumerate
  const int m = std::min(n, (int)cbrt(n*6));
  for (int l = 0; l < m; l++) {
    for (int r = l+1; r <= m; r++) {
      std::array<int, 26> enm{};
      for (int c = 0; c < 26; c++) enm[c] = rank[c][r] - rank[c][l];
      auto _v = wm.enumerate(l, r);
      std::array<int, 26> v{};
      for (auto p : _v) v[p.first] = p.second;
      for (int c = 0; c < 26; c++) {
        if (v[c] != enm[c]) {
          cout << "Failed enumerate "<<l<<", "<<r<<", "<<char(c+'a')<<" wm.enumerate: "<<v[c]<<" != "<<enm[c]<<endl;
          return 1;
        }
      }
    }
  }
  std::cout << "OK" << std::endl;
}