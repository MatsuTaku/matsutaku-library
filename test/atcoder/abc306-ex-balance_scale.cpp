#define PROBLEM "https://atcoder.jp/contests/abc306/tasks/abc306_h"
#include "../../include/mtl/sps.hpp"
#include "../../include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;

void solve() {
    int n,m; cin>>n>>m;
    vector<int> mat(n);
    for (int i = 0; i < m; i++) { 
        int a,b; cin>>a>>b; a--; b--;
        mat[a] |= 1<<b;
        mat[b] |= 1<<a;
    }
    vector<mint> comp(1<<n);
    for (int s = 0; s < 1<<n; s++) {
        auto t = s;
        int c = 0;
        while (t) {
            c++;
            auto vm = t&(-t);
            t ^= vm;
            auto que = vm;
            while (que) {
                auto w = bm::ctz(que);
                que ^= 1<<w;
                auto nxt = mat[w]&t;
                while (nxt) {
                    auto xm = nxt&(-nxt);
                    t ^= xm;
                    nxt ^= xm;
                    que |= xm;
                }
            }
        }
        comp[s] = c%2 ? -1 : 1;
    }
    auto ans = SpsInv<mint,17>(n, comp);
    cout<<ans.back()<<endl;
}

int main() {
    solve();
    return 0;
}