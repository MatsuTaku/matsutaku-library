#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/DSL_5_A"
#include "../../include/mtl/dual_disjoint_sparse_table.hpp"
#include <bits/stdc++.h>
using namespace std;

int sum(int a, int b) {
    return a+b;
}
int e() {return 0;}

int main() {
    int n,t; cin>>n>>t;
    DualDisjointSparseTable<int, sum, e> st(t);
    for (int i = 0; i < n; i++) {
        int l,r; cin>>l>>r;
        st.apply(l, r, 1);
    }
    st.build();
    int ans = 0;
    for (int i = 0; i < t; i++)
        ans = max(ans, st.get(i));
    cout << ans << endl;
}