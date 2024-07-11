#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_point_get"
#include "include/mtl/dual_segment_tree.hpp"
#include "include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;
struct Poly {
    mint a,b;
    Poly() : a(1), b(0) {}
    Poly(int x) : a(0), b(x) {}
    Poly(int a, int b) : a(a), b(b) {}
    Poly& operator*=(const Poly& r) {
        a *= r.a;
        b = b * r.a + r.b;
        return *this;
    }
    auto val() const {return b;}
};
using RA = DualSegmentTree<Poly>;

int main() {
    int n,q; cin>>n>>q;
    vector<int> A(n);
    for (int i = 0; i < n; i++) cin>>A[i];
    RA ra(A.begin(), A.end());
    for (int i = 0; i < q; i++) {
        int t; cin>>t;
        if (t == 0) {
            int l,r,b,c; cin>>l>>r>>b>>c;
            ra.update(l,r,Poly{b,c});
        } else {
            int i; cin>>i;
            cout << ra.get(i).val() << endl;
        }
    }
}