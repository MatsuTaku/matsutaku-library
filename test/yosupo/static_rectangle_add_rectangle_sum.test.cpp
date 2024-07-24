#define PROBLEM "https://judge.yosupo.jp/problem/static_rectangle_add_rectangle_sum"
// #define IGNORE
#define TLE 100
#include "include/mtl/ordinal_range_search.hpp"
#include "include/mtl/modular.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;
using ors_type = ORS<int, mint>;

int main() {
    cin.tie(0); ios::sync_with_stdio(0);
    int n,q; cin>>n>>q;
    vector<array<int, 5>> N(n);
    vector<array<int, 4>> Q(q);
    int index_max = 0;
    auto chmax = [](int& a, int b) { a = max(a, b); };
    for (int i = 0; i < n; i++) {
        int l,d,r,u,w; cin>>l>>d>>r>>u>>w;
        N[i] = {l,d,r,u,w};
        chmax(index_max, max(r,u));
    }
    for (int i = 0; i < q; i++) {
        int l,d,r,u; cin>>l>>d>>r>>u;
        Q[i] = {l,d,r,u};
        chmax(index_max, max(r,u));
    }
    // add [a, inf) times [b, inf) ->
    // for each xy, add (x-a)(y-b) = xy - ay - bx + ab
    array<ors_type, 4> ors{}; // XY, X, Y, const
    auto add_topleft = [&](int a, int b, int w) {
        ors[0].add({a, b, (mint)w});
        ors[1].add({a, b, (mint)w * -a});
        ors[2].add({a, b, (mint)w * -b});
        ors[3].add({a, b, (mint)w * a * b});
    };
    for (auto [l,d,r,u,w]:N) {
        add_topleft(l, d, w);
        add_topleft(l, u, -w);
        add_topleft(r, d, -w);
        add_topleft(r, u, w);
    }
    array<FenwickTree<mint>, 4> rsq{};
    for (int i = 0; i < 4; i++) {
        ors[i].build(
            [&](size_t size) {rsq[i] = FenwickTree<mint>(size);},
            [&](size_t j, mint w) {rsq[i].add(j, w);});
    }
    auto prefix_sum = [&](int x, int y) {
        return ors[0].sum(0, x, 0, y, [&](size_t l, size_t r) {return rsq[0].sum(l, r);}) * x * y +
               ors[1].sum(0, x, 0, y, [&](size_t l, size_t r) {return rsq[1].sum(l, r);}) * y +
               ors[2].sum(0, x, 0, y, [&](size_t l, size_t r) {return rsq[2].sum(l, r);}) * x +
               ors[3].sum(0, x, 0, y, [&](size_t l, size_t r) {return rsq[3].sum(l, r);});
    };
    auto sum = [&](int l, int d, int r, int u) {
        return  prefix_sum(l, d) +
               -prefix_sum(l, u) +
               -prefix_sum(r, d) +
                prefix_sum(r, u);
    };
    for (auto [l,d,r,u]:Q) {
        cout << sum(l, d, r, u) << endl;
    }
}