#pragma once
#include "succinct/bit_vector.hpp"
#include "sparse_table.hpp"
#include "bit_manip.hpp"
#include <vector>
#include <array>
#include <stack>
#include <tuple>
#include <cstdint>
#include <bit>

#ifndef MTL_ARRAY_SET_CONSTEXPR
#if __cplusplus >= 201703L
#define MTL_ARRAY_SET_CONSTEXPR constexpr
#else
#define MTL_ARRAY_SET_CONSTEXPR
#endif
#endif

namespace rmq {
constexpr int table_width = 8;
constexpr size_t table_size = 1u << table_width;
struct make_rmq_table {
  std::array<uint8_t, table_size> idx;
  std::array<int8_t, table_size> val;
  MTL_ARRAY_SET_CONSTEXPR
  make_rmq_table() : idx(), val() {
    for (unsigned int mask = 0; mask < table_size; mask++) {
      uint8_t argmin = 0;
      int8_t min_s = table_width+1, sum = 0;
      for (uint8_t i = 0; i < table_width; i++) {
        if ((mask>>i) & 1) sum++;
        else sum--;
        if (sum < min_s) {
          min_s = sum;
          argmin = i;
        }
      }
      idx[mask] = argmin;
      val[mask] = min_s;
    }
  }
};
} // namespace rmq
MTL_ARRAY_SET_CONSTEXPR
rmq::make_rmq_table rmq_tb;

#ifndef MTL_PAIR_CONSTEXPR
#if __cplusplus >= 201402L
#define MTL_PAIR_CONSTEXPR constexpr
#else
#define MTL_PAIR_CONSTEXPR
#endif
#endif

namespace rmq {
// constexpr int block_width = 8;
constexpr int block_width = 64;

MTL_PAIR_CONSTEXPR
std::pair<unsigned, int> rmq64(uint64_t mask) {
  // Calc E[i-1] as ( rank1_B(i) * 2 - i ) { forall i| i % 8 = 0 }
  auto pos_e8 = bm::popcnt_e8(mask) * 2;
  auto cpos_e8 = pos_e8 * 0x0101010101010100ull;
  constexpr auto cneg_e8 = 8ull * 0x0706050403020100ull;
  constexpr int offset = 64; // to keep values non-negative
  auto e_e8 = offset * 0x0101010101010101ull + cpos_e8 - cneg_e8;

  unsigned argmin = 0;
  int min_s = block_width+1;
  // int sum = 0;
  constexpr uint64_t table_mask = (1ull << table_width) - 1;
  for (unsigned bi = 0; bi < block_width; bi += table_width) {
    auto submask = (mask >> bi) & table_mask;
    auto estart = (int)((e_e8 >> bi) & table_mask) - offset;
    // assert(sum == estart);
    auto s = rmq_tb.val[submask] + estart;
    if (s < min_s) {
      min_s = s;
      argmin = rmq_tb.idx[submask] + bi;
    }
    // int subsum = bm::popcnt(submask) * 2 - table_width;
    // sum += subsum;
  }
  return std::make_pair(argmin, min_s);
}
} // namespace rmq

template<class T>
struct DecaltTree {
  static constexpr size_t null_idx = -1;
  struct Node {
    size_t p, l, r;
  };
  size_t root_;
  std::vector<Node> nodes_;

  DecaltTree() : root_(null_idx) {}
  template<class It>
  explicit DecaltTree(It begin, It end) : DecaltTree() {
    nodes_.reserve(std::distance(begin, end));
    std::vector<T> a;
    for (auto it = begin; it != end; ++it) {
      auto val = *it;
      a.push_back(val);
      if (nodes_.empty()) {
        nodes_.push_back({null_idx, null_idx, null_idx});
        root_ = 0;
        continue;
      }
      auto u = nodes_.size()-1;
      auto v = nodes_.size();
      while (u != null_idx and a[u] > a[v]) {
        u = nodes_[u].p;
      }
      if (u == null_idx) {
        nodes_.push_back({null_idx, root_, null_idx});
        nodes_[root_].p = v;
        root_ = v;
      } else {
        auto ur = nodes_[u].r;
        nodes_.push_back({u, ur, null_idx});
        if (ur != null_idx)
          nodes_[ur].p = v;
        nodes_[u].r = v;
      }
    }
  }
  size_t size() const {
    return nodes_.size();
  }
};

template<class T, class InFn, class PostFn>
void traverse_right_path_to_children(const DecaltTree<T>& tree, InFn in_fn, PostFn post_fn) {
  std::stack<std::tuple<size_t, size_t, bool>> st; // (u, left_child, is_inorder)
  st.emplace(DecaltTree<T>::null_idx, DecaltTree<T>::null_idx, false);
  st.emplace(DecaltTree<T>::null_idx, tree.root_, true);
  std::vector<size_t> c;
  while (!st.empty()) {
    auto [u,lc,in] = st.top(); st.pop();
    if (in) {
      c.clear();
      auto v = lc;
      while (v != DecaltTree<T>::null_idx) {
        c.push_back(v);
        v = tree.nodes_[v].r;
      }
      for (auto it = c.rbegin(); it != c.rend(); it++) {
        auto v = *it;
        st.emplace(v, DecaltTree<T>::null_idx, false);
        st.emplace(v, tree.nodes_[v].l, true);
      }
      in_fn(u);
    } else {
      post_fn(u);
    }
  }
};

template<class T, unsigned RecursionHeight>
class RmqToLca;
template<unsigned RecursionHeight>
class LcaToRmq;

template<class T, unsigned RecursionHeight = 3>
class RmqToLca {
  static constexpr size_t minimum_rmq_size = 1u<<3;
  using st_index_type = size_t;
 private:
  size_t size_;
  min_SparseTable<std::pair<T, st_index_type>> st_;
  LcaToRmq<RecursionHeight> lca_a_;
 public:
  RmqToLca() = default;
  template<class It>
  RmqToLca(It begin, It end) : size_(std::distance(begin, end)) {
    // std::cerr<<RecursionHeight<<' '<<size_<<std::endl;
    if (size_ <= minimum_rmq_size) {
      std::vector<std::pair<T, st_index_type>> st_data(size_);
      auto it = begin;
      for (size_t i = 0; i < size_; i++) {
        st_data[i] = std::make_pair(*(it++), i);
      }
      st_ = decltype(st_)(st_data.begin(), st_data.end());
    } else {
      lca_a_ = decltype(lca_a_)(DecaltTree<T>(begin, end));
    }
  }
  size_t rmq(size_t l, size_t r) const {
    if (l >= r) {
      throw std::invalid_argument("r-l must be positive");
    }
    if (size() <= minimum_rmq_size)
      return st_.query(l, r).second;
    else
      return lca_a_.lca(l, r-1);
  }
  size_t size() const {
    return size_;
  }
};

template<class T>
class RmqToLca<T, 0> {
  using st_index_type = size_t;
 private:
  size_t size_;
  min_SparseTable<std::pair<T, st_index_type>> st_;
 public:
  RmqToLca() = default;
  template<class It>
  RmqToLca(It begin, It end) : size_(std::distance(begin, end)) {
    std::vector<std::pair<T, st_index_type>> st_data(size_);
    auto it = begin;
    for (size_t i = 0; i < size_; i++) {
      st_data[i] = std::make_pair(*(it++), i);
    }
    st_ = decltype(st_)(st_data.begin(), st_data.end());
  }
  size_t rmq(size_t l, size_t r) const {
    return st_.query(l, r).second;
  }
  size_t size() const {
    return size_;
  }
};

template<unsigned RecursionHeight>
class LcaToRmq {
 private:
  // std::vector<size_t> d_;
  // std::vector<size_t> l_;
  BitVector b_;
  RmqToLca<size_t, RecursionHeight - 1> rmq_a_;

 public:
  LcaToRmq() = default;
  template<class U>
  LcaToRmq(const DecaltTree<U>& decalt_tree) 
  // : d_(decalt_tree.size(), -1), l_(2*decalt_tree.size(), -1) 
  {
    // std::vector<bool> de(2*decalt_tree.size());
    b_.resize(2*(decalt_tree.size()+1));
    // std::vector<size_t> f(decalt_tree.size(), -1);
    size_t t = 0;
    traverse_right_path_to_children(decalt_tree, 
      [&](size_t u) {
        // d_[u] = t;
        // e.push_back(h);
        b_[t] = 1;
        // l_[t] = -1; // dummy
        t++;
      }, 
      [&](size_t u) {
        // f[u] = t;
        // e.push_back(h-1);
        b_[t] = 0;
        // l_[t] = decalt_tree.nodes_[u].p;
        t++;
      });
    // Construcr from normal decalt-tree
    // std::stack<std::pair<size_t, bool>> st;
    // st.emplace(decalt_tree.root_, false);
    // st.emplace(decalt_tree.root_, true);
    // while (!st.empty()) {
    //   auto [u, is_in] = st.top(); st.pop();
    //   if (is_in) {
    //     d_[u] = t;
    //     // e.push_back(h);
    //     de[t] = 1;
    //     // l_[t] = -1; // dummy
    //     auto cl = decalt_tree.nodes_[u].l, cr = decalt_tree.nodes_[u].r;
    //     if (cr != DecaltTree<U>::null_idx) {
    //       st.emplace(cr, false);
    //       st.emplace(cr, true);
    //     }
    //     if (cl != DecaltTree<U>::null_idx) {
    //       st.emplace(cl, false);
    //       st.emplace(cl, true);
    //     }
    //   } else {
    //     // f[u] = t;
    //     // e.push_back(h-1);
    //     de[t] = 0;
    //     l_[t] = decalt_tree.nodes_[u].p;
    //   }
    //   t++;
    // }
    
    b_.build();
    auto num_blocks = (b_.size() + rmq::block_width - 1) / rmq::block_width;
    if (num_blocks >= 3) {
      std::vector<size_t> a(num_blocks);
      size_t block_sum = 0;
      size_t ei = 0, bi = 0;
      while (ei + rmq::block_width < b_.size()) {
        auto width = std::min(rmq::block_width, (int)(b_.size()-ei));
        auto mask = b_.bitmap().range_get(ei, ei+width);
        if (width < rmq::block_width)
          mask |= ((1ull<<(rmq::block_width-width))-1) << width;
        ei += rmq::block_width;
        // a[bi] = (long long) rmq_tb.val[mask] + block_sum;
        a[bi] = (long long) rmq::rmq64(mask).second + block_sum;
        block_sum += bm::popcnt(mask) * 2;
        block_sum -= rmq::block_width;
        bi++;
      }
      rmq_a_ = decltype(rmq_a_)(a.begin()+1, a.end()-1);
    }
  }

  size_t get_e(size_t i) const {
    return b_.rank1(i+1) * 2 - (i+1);
  }
  
  size_t lca(size_t x, size_t y) const {
    if (x == y) return x;
    auto xx = b_.select0(x);
    auto yy = b_.select0(y);
    // if (d_[x] > d_[y]) std::swap(x, y);
    if (xx > yy) std::swap(xx, yy);
    // size_t i = d_[x] / rmq::block_width;
    // size_t j = d_[y] / rmq::block_width;
    size_t i = xx / rmq::block_width;
    size_t j = yy / rmq::block_width;
    size_t m;
    size_t min_e;
    { // block i
      auto r = std::min(yy+1, (i+1)*rmq::block_width);
      auto mask = b_.bitmap().range_get(xx, r);
      auto dist = r - xx;
      auto rem = rmq::block_width - dist;
      if (rem)
        mask |= ((1ull<<rem)-1) << dist;
      // auto idx = rmq_tb.idx[mask] + d_[x];
      auto idx = rmq::rmq64(mask).first + xx;
      auto e = get_e(idx);
      m = idx;
      min_e = e;
    }
    if (i+2 <= j) { // block i+1...j-1
      auto k = rmq_a_.rmq(i+1 - 1, j - 1) + 1; // -1 means ignoring i={0,num_block-1}
      auto mask = b_.bitmap().range_get(k*rmq::block_width, (k+1)*rmq::block_width);
      // auto idx = rmq_tb.idx[mask] + k*rmq::block_width;
      auto idx = rmq::rmq64(mask).first + k*rmq::block_width;
      auto e = get_e(idx);
      if (e < min_e) {
        min_e = e;
        m = idx;
      }
    }
    if (i < j) { // block j
      auto l = j*rmq::block_width;
      auto mask = b_.bitmap().range_get(l, yy+1);
      auto dist = yy+1 - l;
      auto rem = rmq::block_width - dist;
      if (rem)
        mask |= ((1ull<<rem)-1) << dist;
      // auto idx = rmq_tb.idx[mask] + j*rmq::block_width;
      auto idx = rmq::rmq64(mask).first + j*rmq::block_width;
      auto e = get_e(idx);
      if (e < min_e) {
        min_e = e;
        m = idx;
      }
    }
    // auto ret = m == d_[x] ? x : l_[m];
    auto ret = m == xx ? x : b_.rank0(m);
    // std::cerr<<"d[x] d[y] rmq_e(d[x],d[y]) "<<d_[x]<<' '<<d_[y]<<' '<<m<<std::endl;
    // std::cerr<<"x y lca_t(x,y) "<<x<<' '<<y<<' '<<ret<<std::endl;
    // std::cerr<<"E[d[x]] E[m] "<<get_e(d_[x])<<' '<<get_e(m)<<std::endl;
    // if (m == d_[x]) {
    // } else {
    //   assert(get_e(m) < get_e(d_[x]));
    //   assert(l_[m] != x);
    // }
    return ret;
  }

  static bool test_right_path_to_children_construction() {
    std::vector<int> A{2,4,3,5,1,8,6,7,9};
    std::vector<int> B{1,1,1,0,1,1,0,0,1,0,0,1,1,0,0,1,0,1,0,0};
    DecaltTree<int> tree(A.begin(), A.end());
    LcaToRmq ltr(tree);
    bool ok = B.size()==ltr.b_.size();
    for (size_t i = 0; i < B.size(); i++) {
      ok &= B[i]==ltr.b_[i];
    }
    if (!ok) {
      std::cerr<<B.size()<<' '<<ltr.b_.size()<<std::endl;
      for (size_t i = 0; i < B.size(); i++) {
        std::cerr<<B[i]<<' '<<ltr.b_[i]<<std::endl;
      }
    }
    return ok;
  }
};

template<class T>
using Rmq = RmqToLca<T>;