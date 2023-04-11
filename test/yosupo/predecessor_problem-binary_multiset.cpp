#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"
#include "../../include/mtl/treap.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n,q; cin>>n>>q;
    string t; cin>>t;
    vector<int> V;
    for (int i = 0; i < n; i++) if (t[i]=='1') V.push_back(i);
    Treap<int> S(V.begin(), V.end());
    for (int i = 0; i < q; i++) {
        int t; cin>>t;
        switch (t) {
            case 0: {
                int k; cin>>k;
                S.insert(k);
            }; break;
            case 1: {
                int k; cin>>k;
                S.erase(S.find(k));
            }; break;
            case 2: {
                int k; cin>>k;
                cout << S.count(k) << endl;
            }; break;
            case 3: {
                int k; cin>>k;
                auto i = S.lower_bound(k);
                cout << (i != S.end() ? *i : -1) << endl;
            }; break;
            case 4: {
                int k; cin>>k;
                auto i = S.upper_bound(k);
                cout << (i != S.begin() ? *--i : -1) << endl;
            }; break;
            default: break;
        }
    }
}