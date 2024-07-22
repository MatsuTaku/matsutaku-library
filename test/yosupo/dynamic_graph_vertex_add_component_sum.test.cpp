#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum"

#include "include/mtl/segment_tree_divide_and_conquer.hpp"
#include <bits/stdc++.h>
using namespace std;

struct UndoableUnionFind {
  vector<int> par;
  vector<long long> data;
  stack<vector<pair<int, long long>>> history;
  UndoableUnionFind(int n) : par(n, -1), data(n) {}
  int leader(int x) const {
    return par[x] < 0 ? x : leader(par[x]);
  }
  bool same(int x, int y) const {
    return leader(x) == leader(y);
  }
  int size(int x) const {
    return -par[leader(x)];
  }
  void merge(int x, int y) {
    auto X = leader(x);
    auto Y = leader(y);
    if (X == Y) {
      return;
    }
    if (size(X) < size(Y)) swap(X, Y);
    history.top().emplace_back(X, par[X]);
    par[X] += par[Y];
    history.top().emplace_back(Y, par[Y]);
    par[Y] = X;
    history.top().emplace_back(-X-1, data[X]);
    data[X] += data[Y];
  }
  void add(int v, int x) {
    auto V = leader(v);
    history.top().emplace_back(-V-1, data[V]);
    data[V] += x;
  }
  void snapshot() {
    history.emplace();
  }
  void undo() {
    for (int i = (int)history.top().size()-1; i >= 0; i--) {
      auto [x, y] = history.top()[i];
      if (x < 0) {
        data[-x-1] = y;
      } else {
        par[x] = y;
      }
    }
    history.pop();
  }
};
  
constexpr int NMAX = 3e5+1;

int main() {
  int n,q; cin>>n>>q;
  vector<int> A(n);
  for (int i = 0; i < n; i++) cin>>A[i];
  vector<array<int, 3>> Q(q);
  unordered_map<long long, int> S;
  SegmentTreeDivideAndConquer<vector<pair<int,int>>> G(q);
  for (int i = 0; i < q; i++) {
    int t; cin>>t;
    switch (t) {
      case 0: {
        int u,v; cin>>u>>v;
        Q[i] = {t,u,v};
        if (u > v) swap(u,v);
        S.emplace((long long)NMAX*u+v, i);
        break;
      }
      case 1: {
        int u,v; cin>>u>>v;
        Q[i] = {t,u,v};
        if (u > v) swap(u,v);
        auto it = S.find((long long)NMAX*u+v);
        assert(it != S.end());
        auto s = it->second;
        S.erase(it);
        G.set(s, i, [&](auto& qs) {
          qs.emplace_back(u, v); 
        });
        break;
      }
      case 2: {
        int v,x; cin>>v>>x;
        Q[i] = {t,v,x};
        G.set(i, q, [&](auto& qs) {
          qs.emplace_back(-v-1, x);
        });
        break;
      }
      case 3: {
        int v; cin>>v;
        Q[i] = {t,v};
        break;
      }
    }
  }
  for (auto [id,s] : S) {
    int u = id/NMAX;
    int v = id%NMAX;
    G.set(s, q, [&](auto& qs) {
      qs.emplace_back(u, v);
    });
  }
  UndoableUnionFind uf(n);
  for (int i = 0; i < n; i++) uf.data[i] = A[i];
  G.dfs([&](int i) {
    if (Q[i][0] == 3) {
      cout << uf.data[uf.leader(Q[i][1])] << endl;
    }
  }, [&](auto& vs) {
    uf.snapshot();
    for (auto [a,b]: vs) {
      if (a < 0) {
        uf.add(-a-1, b);
      } else {
        uf.merge(a, b);
      }
    }
  }, [&] {
    uf.undo();
  });
}