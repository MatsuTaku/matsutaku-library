#define PROBLEM "https://judge.yosupo.jp/problem/persistent_unionfind"
#include <iostream>
#include "../../include/mtl/persistent_unionfind.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n,q; cin>>n>>q;
    vector<PersistentUnionfind> U(q+1);
    for (int i = 0; i < q; i++) {
        int t; cin>>t;
        if (t == 0) {
            int k,u,v; cin>>k>>u>>v;
            U[i+1] = U[k+1].merge(u,v);
        } else {
            int k,u,v; cin>>k>>u>>v;
            cout << U[k+1].same(u,v) << endl;
        }
    }
}