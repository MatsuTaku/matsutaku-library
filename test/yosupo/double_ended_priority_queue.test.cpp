#define PROBLEM "https://judge.yosupo.jp/problem/double_ended_priority_queue"
#include "../../include/mtl/double_ended_priority_queue.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n,q; cin>>n>>q;
    vector<int> S(n);
    for (int i = 0; i < n; i++) cin>>S[i];
    DoubleEndedPriorityQueue<int> depq(S.begin(), S.end());
    for (int i = 0; i < q; i++) {
        int t; cin>>t;
        if (t == 0) {
            int x; cin>>x;
            depq.push(x);
        } else if (t == 1) {
            cout << depq.min() << endl;
            depq.pop_min();
        } else {
            cout << depq.max() << endl;
            depq.pop_max();
        }
    }
}