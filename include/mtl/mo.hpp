#pragma once
#include <algorithm>
#include <cmath>
#include <tuple>
#include <vector>

template <typename T, T (*pushl)(T, int), T (*pushr)(T, int), T (*popl)(T, int),
          T (*popr)(T, int), T (*e)()>
class MoSolver {
 private:
  std::vector<std::tuple<int, int, int>> segs;
  int n = 0;

 public:
  MoSolver() = default;
  void add_segment(int l, int r) { segs.emplace_back(n++, l, r); }
  void solve(std::vector<T> &dst) {
    using std::get;
    sort(segs.begin(), segs.end(), [m = (int)std::sqrt(n)](auto &l, auto &r) {
      int ll = get<1>(l), rl = get<2>(l), lr = get<1>(r), rr = get<2>(r);
      int bl = ll / m;
      int br = lr / m;
      return (bl != br) ? bl < br : (rl < rr) ^ (bl % 2);
    });
    dst.resize(n);
    int _l = 0, _r = 0;
    T val = e();
    for (auto &ilr : segs) {
      int i = get<0>(ilr), l = get<1>(ilr), r = get<2>(ilr);
      while (l < _l)
        val = pushl(val, --_l);
      while (_r < r)
        val = pushr(val, _r++);
      while (_l < l)
        val = popl(val, _l++);
      while (r < _r)
        val = popr(val, --_r);
      dst[i] = val;
    }
  }
};
