#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include "include/mtl/range_minimum_query.hpp"
#include <iostream>
using namespace std;

int main() {
  int n,q; cin>>n>>q;
  vector<int> A(n);
  for (int i = 0; i < n; i++) cin>>A[i];
  Rmq<int> st(A.begin(), A.end());
  while (q--) {
    int l,r; cin>>l>>r;
    auto min_idx = st.rmq(l,r);
    // if (!(l <= min_idx and min_idx < r)) {
    //   std::cerr<<"l r min_idx "<<l<<' '<<r<<' '<<min_idx<<std::endl;
    // }
    // assert(l <= min_idx and min_idx < r);
    cout<<A[min_idx]<<endl;
  }
}