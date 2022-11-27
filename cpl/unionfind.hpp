#pragma once
#include <vector>
#include <numeric>
#include <cstddef>

class UnionFind {
 private:
  // par_[i] = { size if par_[i] < 0, root otherwise }
  std::vector<int> par_;

 public:
  UnionFind() = default;
  explicit UnionFind(size_t size) : par_(size, -1) {}

  int root(int u) {
    if (par_[u] < 0)
      return u;
    return par_[u] = root(par_[u]);
  }

  bool are_union(int u, int v) {
    return root(u) == root(v);
  }

  int size_of(int u) {
    return -par_[root(u)];
  }

  bool unite(int u, int v) {
    if (are_union(u,v))
      return false;
    if (size_of(u) < size_of(v))
      std::swap(u,v);
    par_[root(u)] -= size_of(v);
    par_[root(v)] = root(u);
    return true;
  }
};
