#include "../../include/mtl/bitwise_convolution.hpp"
#include "../../include/mtl/sps.hpp"
#include "../../include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;

void solve() {
    int n,m; cin>>n>>m;
    vector<int> h(1<<n);
    for (int i = 0; i < m; i++) {
        int a,b; cin>>a>>b; a--; b--;
        h[(1<<a)|(1<<b)]++;
    }
    // h[s]: count of edges in s
    SubsetZetaInline(h);
    // F[s]: sum_s=ta||tb 2^(h[s]-h[ta]-h[tb])
    vector<mint> F(1<<n);
    for (int i = 0; i < 1<<n; i++) {
        F[i] = mint(2).inv().pow(h[i]);
    }
    SubsetConvolutionSquare<mint,17>(F.cbegin(), F.cend(), F.begin());
    for (int i = 0; i < 1<<n; i++) {
        F[i] *= mint(2).pow(h[i]);
    }
    auto G = SpsLog<mint, 17>(n, F);
    auto ans = G[(1<<n)-1] / 2;
    cout<<ans<<endl;
}

int main() {
    solve();
    return 0;
}