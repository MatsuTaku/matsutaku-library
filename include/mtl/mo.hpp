#pragma once
#include <algorithm>
#include <numeric>
#include <cmath>
#include <tuple>
#include <vector>
#include <iostream>
#include <cassert>

/**
 * @brief Mo's algorithm: solve offline segment queries on a sequence
 * @note  This implementation is optimized by noshi's idea
 *          complexity of N sqrt(Q) + O(N).
 *        - 定数倍が最適な Mo's Algorithm. noshi91のメモ. 2023/04/13.
 *          https://noshi91.hatenablog.com/entry/2023/04/13/224811
 * @note  Mo's algorithm with rollback is based on snuke's idea
 *        - Mo's algorithm とその上位互換の話. あなたは嘘つきですかと聞かれたら「YES」と答えるブログ. 2016/07/01.
 *          https://snuke.hatenablog.com/entry/2016/07/01/000000
*/
struct MoSolver {
  std::vector<std::tuple<int, int, int>> segs;
  int q = 0;

  void add_segment(int l, int r) { 
    assert(l <= r);
    segs.emplace_back(q++, l, r);
  }

  void calc_mos_move(std::vector<int>& dst) const {
    using std::get;
    int n = 0;
    for (auto s:segs)
      n = std::max(n, get<2>(s));
    auto rtq = std::sqrt(q);
    int b = std::ceil((double)n / rtq);
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
    std::sort(IE.begin(), IE.end(), 
      EvenComp);
    auto IO = IE;
    std::sort(IO.begin(), IO.end(), 
      OddComp);
    auto move_distance = [&](const std::vector<int>& ids) {
      long long d = 0;
      for (int i = 0; i < q-1; i++) {
        int j = ids[i], k = ids[i+1];
        d += std::abs(get<1>(segs[j]) - get<1>(segs[k]));
        d += std::abs(get<2>(segs[j]) - get<2>(segs[k]));
      }
      return d;
    };
    if (move_distance(IE) > move_distance(IO))
      dst = std::move(IO); // IE is reference of dst
  }

  template<class PUSH_L, class PUSH_R, class POP_L, class POP_R, class REM>
  void solve(PUSH_L pushl, PUSH_R pushr, POP_L popl, POP_R popr, REM rem) const {
    if (q == 0) return;
    std::vector<int> I;
    calc_mos_move(I);
    int _l = 0, _r = 0;
    for (int i:I) {
      int t,l,r;
      std::tie(t,l,r) = segs[i];
      while (l < _l)
        pushl(--_l);
      while (_r < r)
        pushr(_r++);
      while (_l < l)
        popl(_l++);
      while (r < _r)
        popr(--_r);
      rem(t);
    }
  }

  template<class Block, class Bend>
  long long calc_mos_rollback_move(std::vector<int>& idx, std::vector<std::pair<int,int>>& blocks, Block block, Bend bend) const {
    std::sort(idx.begin(), idx.end(), [&](auto a, auto b) {
      auto [ta, la, ra] = segs[a];
      auto [tb, lb, rb] = segs[b];
      auto ba = block(la), bb = block(lb);
      return ba != bb ? la < lb : ra < rb;
    });
    long long dist = 0;
    int cb = -1;
    int _l = 0, _r = 0;
    for (size_t i = 0; i < idx.size(); i++) {
      auto [t,l,r] = segs[idx[i]];
      auto bi = block(l);
      auto be = bend(bi);
      blocks[i] = std::make_pair(bi, be);
      if (bi != cb) {
        _r = be;
        cb = bi;
      }
      dist += r-_r;
      _r = r;
      _l = be;
      dist += _l-l;
    }
    return dist;
  }

  template<class PUSH_L, class PUSH_R, class REM, 
           class INIT, class SNAPSHOT, class ROLLBACK> 
  void solve_rollback(PUSH_L pushl, PUSH_R pushr, REM rem, 
             INIT init, SNAPSHOT snapshot, ROLLBACK rollback) const {
    if (q == 0) return;
    int n = 0;
    for (auto s:segs)
      n = std::max(n, std::get<2>(s));
//  * (2|xi-c|+b/2)q, |xi-c| < b/2, (mean |xi-c| = b/4) -> bq
//  * min bq + n^2/b, 
//    from AMGM, bq = n^2/b => b^2 = n^2 /q => b = n / sqrt(q)
//  * F = (bq + n^2/b)/2
//      = bq
//      = n sqrt(q)
    const int b = std::ceil((double)n / std::sqrt(q));
    std::vector<int> J;
    for (int i = 0; i < q; i++) {
      auto [t,l,r] = segs[i];
      if (r-l < b) {
        init();
        for (int j = l; j < r; j++)
          pushr(j);
        rem(t);
      } else {
        J.push_back(i);
      }
    }
    
    std::vector<std::pair<int,int>> B(J.size());
    {
      auto& b_even = B;
      auto block_even = [&](int x) {
        return x / b;
      };
      auto bend_even = [&](int bi) {
        return (bi+1)*b;
      };
      auto dist_e = calc_mos_rollback_move(J, b_even, block_even, bend_even);

      auto K = J;
      auto b_odd = B;
      const auto bf = b-b/2;
      auto block_odd = [&](int x) {
        return x < bf ? 0 : (x-bf)/b+1;
      };
      auto bend_odd = [&](int bi) {
        return bf+bi*b;
      };
      auto dist_o = calc_mos_rollback_move(K, b_odd, block_odd, bend_odd);

      if (dist_e > dist_o) {
        J = std::move(K);
        B = std::move(b_odd);
      }
    }

    int cb = -1;
    int _l = 0, _r = 0;
    for (auto it = J.begin(); it != J.end(); ++it) {
      auto [t,l,r] = segs[*it];
      auto [bi,be] = B[it-J.begin()];
      if (bi != cb) {
        init();
        _r = be;
        cb = bi;
      }
      assert(_r <= r);
      while (_r < r) {
        pushr(_r++);
      }
      snapshot();
      _l = be;
      assert(l <= _l);
      assert(_l-l <= b);
      while (l < _l) {
        pushl(--_l);
      }
      rem(t);
      rollback();
    }
  }
};
