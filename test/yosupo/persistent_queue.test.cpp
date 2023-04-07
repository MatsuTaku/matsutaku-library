#define PROBLEM "https://judge.yosupo.jp/problem/persistent_queue"
#define IGNORE "mle"
#include <iostream>
#include "../../include/mtl/persistent_queue.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int q; cin>>q;
    vector<PersistentQueue<int>> S(q+1);
    for (int i = 0; i < q; i++) {
        int c; cin>>c;
        if (c == 0) {
            int t,x; cin>>t>>x; t++;
            S[i+1] = S[t].push(x);
        } else {
            int t; cin>>t; t++;
            auto v = S[t].front();
            S[i+1] = S[t].pop();
            cout << v << endl;
        }
    }
}