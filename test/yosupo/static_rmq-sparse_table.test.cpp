#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include "include/mtl/sparse_table.hpp"
#include <iostream>
using namespace std;

int main() {
  int n,q; cin>>n>>q;
  vector<int> A(n);
  for (int i = 0; i < n; i++) cin>>A[i];
  min_SparseTable<int> st(A.begin(), A.end());
  while (q--) {
    int l,r; cin>>l>>r;
    cout<<st.query(l,r)<<endl;
  }
}