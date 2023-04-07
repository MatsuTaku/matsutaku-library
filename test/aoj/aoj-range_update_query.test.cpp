#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_D"
#include "../../include/mtl/dual_segment_tree.hpp"
#include <bits/stdc++.h>
using namespace std;

struct OR {
    int a,b;
    OR() : a(1), b(0) {}
    OR(int b) : a(0), b(b) {}
    OR& operator=(int x) {
        a = 0;
        b = x;
        return *this;
    }
    OR& operator*=(const OR& r) {
        a *= r.a;
        b = b * r.a + r.b;
        return *this;
    }
};

int main() {
    int n,q; cin>>n>>q;
    vector init(n, (1u<<31)-1);
    DualSegmentTree<OR> dst(init.begin(), init.end());
    for (int i = 0; i < q; i++) {
        int t; cin>>t;
        if (t == 0) {
            int s,t,x; cin>>s>>t>>x;
            ++t;
            dst.update(s, t, x);
        } else {
            int i; cin>>i;
            cout << dst.get(i).b << endl;
        }
    }
}