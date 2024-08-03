#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include "include/mtl/disjoint_sparse_table.hpp"
#include <bits/stdc++.h>
using namespace std;

int fmin(int a, int b) {return min(a, b);}

int main() {
  int n,q; cin>>n>>q;
  vector<int> A(n);
  for (int i = 0; i < n; i++) cin>>A[i];
  DisjointSparseTable<int, fmin> st(A.begin(), A.end());
  while (q--) {
    int l,r; cin>>l>>r;
    cout<<st.query(l,r)<<endl;
  }
}