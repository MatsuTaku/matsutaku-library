#define PROBLEM "https://yukicoder.me/problems/no/945"
#include "include/mtl/dual_sparse_table.hpp"
#include <bits/stdc++.h>
using namespace std;

using P = pair<int,int>;
P f(P a, P b) {return min(a,b);}
constexpr int INF = 1e9;
P e() {return {INF, 3};}
using DST = DualSparseTable<P, f, e>;

int main() {
    int n,m; cin>>n>>m;
    map<char,int> mp;
    mp['Y']=0;
    mp['K']=1;
    mp['C']=2;
    DST dst(n);
    for (int i = 0; i < m; i++) {
        int l,r;
        char t;
        cin>>l>>r>>t;
        l--;
        dst.apply(l, r, {i, mp[t]});
    } 
    dst.build();
    array<int,4> ans{};
    for (int i = 0; i < n; i++) {
        auto [id,t] = dst.get(i);
        ans[t]++;
    }
    cout << ans[0] << ' ' << ans[1] << ' ' << ans[2] << endl;
}