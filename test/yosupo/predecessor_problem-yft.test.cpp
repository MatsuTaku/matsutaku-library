#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"
#define IGNORE
#include "include/mtl/yft.hpp"
#include <bits/stdc++.h>
using namespace std; 

constexpr int max_value = 1e7;
constexpr int bits = 64 - bm::clz(max_value);

int main() {
    int n,q; cin>>n>>q;
    string t; cin>>t;
    vector<int> V;
    for (int i = 0; i < n; i++) if (t[i]=='1') V.push_back(i);
    YFastTrieSet<unsigned int, bits> S(V.begin(), V.end());
    for (int i = 0; i < q; i++) {
        int t; cin>>t;
        int k; cin>>k;
        switch (t) {
            case 0: {
                S.insert(k);
            }; break;
            case 1: {
                S.erase(S.find(k));
            }; break;
            case 2: {
                cout << S.count(k) << endl;
            }; break;
            case 3: {
                auto i = S.lower_bound(k);
                cout << (i != S.end() ? (int)*i : -1) << endl;
            }; break;
            case 4: {
                auto i = S.upper_bound(k);
                cout << (i != S.begin() ? (int)*--i : -1) << endl;
            }; break;
            default: break;
        }
    }
}