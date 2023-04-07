#define PROBLEM "https://judge.yosupo.jp/problem/rectangle_sum"
#include "../../include/mtl/ordinal_range_search.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n,q; cin>>n>>q;
    ORS<int, long long> ors;
    for (int i = 0; i < n; i++) {
        int x,y,w; cin>>x>>y>>w;
        ors.add(x, y, w);
    }
    ors.build();
    for (int i = 0; i < q; i++) {
        int l,d,r,u; cin>>l>>d>>r>>u;
        cout << ors.sum(l, r, d, u) << endl;
    }
}