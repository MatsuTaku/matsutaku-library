#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum"
#include "include/mtl/splay_tree_list.hpp"
#include "include/mtl/modular.hpp"
#include "include/mtl/monoid.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;
struct Sum {
    mint a;
    size_t sz;
    Sum(int a=0) : a(a), sz(1) {}
    Sum(mint a, size_t sz) : a(a), sz(sz) {}
    Sum operator*(const Sum& rhs) const {
        return Sum(a+rhs.a, sz+rhs.sz);
    }
};
struct Affine {
    mint b,c;
    Affine(mint b=1, mint c=0) : b(b), c(c) {}
    Affine& operator*=(const Affine& rhs) {
        b *= rhs.b;
        c = c*rhs.b + rhs.c;
        return *this;
    }
    Sum act(const Sum& a) const {
        return {b * a.a + c * a.sz, a.sz};
    }
};
using DynamicSequenceRangeAffineRangeSum = SplayTreeList<Sum, Affine>;

int main() {
    int n,q; cin>>n>>q;
    vector<int> A(n);
    for (int i = 0; i < n; i++) cin>>A[i];
    DynamicSequenceRangeAffineRangeSum st(A.begin(), A.end());
    for (int i = 0; i < q; i++) {
        int t; cin>>t;
        switch (t) {
            case 0: {
                int i,x; cin>>i>>x;
                st.insert(i, x);
            }; break;
            case 1: {
                int i; cin>>i;
                st.erase(i);
            } break;
            case 2: {
                int l,r; cin>>l>>r;
                st.reverse(l,r);
            } break;
            case 3: {
                int l,r,b,c; cin>>l>>r>>b>>c;
                st.update(l,r,{b,c});
            } break;
            case 4: {   
                int l,r; cin>>l>>r;
                cout << st.prod(l,r).a << endl;
            } break;
            default: break;
        }
    }
}