#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_mod"
#include "../../include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;
using mint = DynamicModular<>;

int main() {
    int t; cin>>t;
    for (int i = 0; i < t; i++) {
        int y,p; cin>>y>>p;
        mint::set_mod(p);
        mint Y = y;
        if (Y.is_square()) {
            cout << Y.sqrt() << endl;
        } else {
            cout << -1 << endl;
        }
    }
}