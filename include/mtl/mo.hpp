#pragma once
#include <algorithm>
#include <numeric>
#include <cmath>
#include <tuple>
#include <vector>
#include <iostream>

/**
 * @brief Mo's algorithm: solve offline segment queries on a sequence
 * @note  This implementation is optimized by noshi's idea
 *          complexity of N sqrt(Q) + O(N).
 *        - 定数倍が最適な Mo's Algorithm. noshi91のメモ. 2023/04/13.
 *          https://noshi91.hatenablog.com/entry/2023/04/13/224811
*/
template <typename T, 
          T (*pushl)(T, int), 
          T (*pushr)(T, int), 
          T (*popl)(T, int),
          T (*popr)(T, int), 
          T (*e)()>
class MoSolver {
 private:
  std::vector<std::tuple<int, int, int>> segs;
  int q = 0;

 public:
  void add_segment(int l, int r) { 
    segs.emplace_back(q++, l, r);
  }
  void calc_mos_move(std::vector<int>& dst) {
    using std::get;
    int n = 0;
    for (auto s:segs)
      n = std::max(n, get<2>(s));
    auto rtq = sqrt(q);
    int b = ceil((double)n / rtq);
    auto bf = b-b/2;
    auto get_bo = [&](int x) {
      if (x < bf) return 0;
      return (x-bf)/b+1;
    };
    auto EvenComp = [&](int u, int v) {
      auto &s = segs[u], &t = segs[v];
      auto ls = get<1>(s), rs = get<2>(s), lt = get<1>(t), rt = get<2>(t);
      auto bs = ls / b, bt = lt / b;
      return bs != bt ? ls < lt : (bs%2==0 ? rs < rt : rs > rt);
    };
    auto OddComp = [&](int u, int v) {
      auto &s = segs[u], &t = segs[v];
      auto ls = get<1>(s), rs = get<2>(s), lt = get<1>(t), rt = get<2>(t);
      auto bs = get_bo(ls), bt = get_bo(lt);
      return bs != bt ? ls < lt : (bs%2==0 ? rs < rt : rs > rt);
    };
    auto& IE = dst;
    IE.resize(q);
    std::iota(IE.begin(), IE.end(), 0);
    sort(IE.begin(), IE.end(), EvenComp);
    auto IO = IE;
    sort(IO.begin(), IO.end(), OddComp);
    auto move_distance = [&](const std::vector<int>& ids) {
      long long d = 0;
      for (int i = 0; i < q-1; i++) {
        int j = ids[i], k = ids[i+1];
        d += abs(get<1>(segs[j]) - get<1>(segs[k]));
        d += abs(get<2>(segs[j]) - get<2>(segs[k]));
      }
      return d;
    };
    if (move_distance(IE) > move_distance(IO))
      std::swap(IE, IO); // IE is reference of dst
  }
  void solve(std::vector<T> &dst) {
    if (q == 0) return;
    std::vector<int> I;
    calc_mos_move(I);
    dst.resize(q);
    int _l = 0, _r = 0;
    T val = e();
    for (int i:I) {
      int t,l,r;
      std::tie(t,l,r) = segs[i];
      while (l < _l)
        val = pushl(val, --_l);
      while (_r < r)
        val = pushr(val, _r++);
      while (_l < l)
        val = popl(val, _l++);
      while (r < _r)
        val = popr(val, --_r);
      dst[t] = val;
    }
  }
};
