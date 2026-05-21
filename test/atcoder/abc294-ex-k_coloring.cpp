// https://atcoder.jp/contests/abc294/tasks/abc294_h
#include "../../include/mtl/bitwise_convolution.hpp"
#include "../../include/mtl/sps.hpp"
#include "../../include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;

void solve() {
    int n,m,k; cin>>n>>m>>k;
    /// n <= 30, m <= 30, k <= 1e9
    /// Just only using k_colorings is TLE solution O(2^n n^2)
    vector<uint> E(n);
    for (int i = 0; i < m; i++) {
        int u,v; cin>>u>>v; u--; v--;
        E[u] |= 1<<v;
        E[v] |= 1<<u;
    }
    if (k==1 and m>0) {
        cout<<0<<endl;
        return;
    }
    auto k_colorings = [k](vector<uint> E) {
        int n = E.size();
        assert(n <= 20);
        vector<int> edges(1<<n);
        edges[0] = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i+1; j < n; j++) {
                if ((E[i]>>j)&1)
                    edges[1ull<<i | 1ull<<j] = 1;
            }
        }
        SubsetZetaInline(edges);
        vector<int> ind(1<<n);
        for (int i = 1; i < 1<<n; i++) {
            ind[i] = edges[i]==0;
            // cerr<<bitset<20>(i)<<": "<<edges[i]<<" "<<ind[i]<<endl;
        }
        vector<int> y(1<<n);
        y.back() = 1;
        auto x = TransposedSpsCompositionEgf<mint,20>(ind, y);
        mint ans = 0;
        mint p = 1;
        for (int i = 1; i <= min(n,k); i++) {
            p *= k-(i-1);
            // cerr<<i<<' '<<p.val()<<' '<<x[i].val()<<endl;
            ans += p * x[i];
        }
        return ans;
    };
    auto del = [](vector<uint> E, int v) {
        int N = E.size();
        vector<uint> nE(E.size()-1);
        for (int i = 0; i < N; i++) {
            if (i==v) continue;
            uint low = E[i] & ((1u<<v)-1u);
            uint high = (E[i] >> (v+1)) << v;
            nE[i - (i>v)] = low | high;
        }
        for (int i = 0; i < N-1; i++) {
            for (int j = i+1; j < N-1; j++) {
                auto ij = (E[i]>>j)&1;
                auto ji = (E[j]>>i)&1;
                assert(ij==ji);
            }
        }
        return nE;
    };
    auto merge = [](vector<uint> E, int u, int v) {
        int N = E.size();
        auto& nE = E;
        auto muv = (1u<<u)|(1u<<v);
        auto mco = 0u;
        for (int i = 0; i < N; i++) {
            if ((E[i]&muv)==0) continue;
            nE[i] |= muv;  
            mco |= 1u<<i;
        }
        nE[u] |= mco;
        nE[v] |= mco;
        return nE;
    };
    auto rec = [&](this auto&& self, vector<uint> E) -> mint {
        for (int i = 0; i < E.size(); i++)
            if ((E[i]>>i)&1)
                return 0;
        if (E.size()==1)
            return k;
        int v = min_element(E.begin(), E.end(), [](auto l, auto r) {
            return std::popcount(l) < std::popcount(r);
        }) - E.begin();
        auto degv = std::popcount(E[v]);
        // cerr<<E.size()<<' '<<v<<' '<<degv<<endl;
        // for (auto e:E) cerr<<bitset<20>(e)<<endl;
        mint ret = 0;
        if (degv == 0) {
            ret = self(del(E, v)) * k;
        } else if (degv == 1) {
            ret = self(del(E, v)) * (k-1);
        } else if (degv == 2) {
            auto s = std::countr_zero(E[v]);
            auto t = std::countr_zero(E[v] >> (s+1)) + s+1;
            auto E2 = del(E, v);
            auto mst = max(s,t);
            auto E4 = del(del(merge(E, s, t), max(mst,v)), min(mst,v));
            ret = (k-2) * self(E2) + self(E4);
        } else {
            ret = k_colorings(E);
        }
        // cerr<<"rec "<<E.size()<<endl;
        // for (auto e:E) cerr<<bitset<20>(e)<<endl;
        // cerr<<ret.val()<<endl;
        return ret;
    };
    auto ans = rec(E);
    // cerr<<k_colorings(E).val()<<endl;
    cout<<ans.val()<<endl;
}

int main() {
    solve();
    return 0;
}