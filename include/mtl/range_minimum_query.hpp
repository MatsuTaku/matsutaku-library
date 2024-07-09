#pragma once
#include "succinct/bit_vector.hpp"
#include "sparse_table.hpp"
#include "bit_manip.hpp"
#include <vector>
#include <array>
#include <stack>
#include <cstdint>
#include <bit>

namespace rmq {
constexpr int table_width = 8;
constexpr size_t table_size = 1u << table_width;
struct make_rmq_table {
  std::array<uint8_t, table_size> idx;
  std::array<int8_t, table_size> val;
  constexpr make_rmq_table() : idx(), val() {
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
constexpr rmq::make_rmq_table rmq_tb;

namespace rmq {
// constexpr int block_width = 8;
constexpr int block_width = 64;
std::pair<unsigned, int> rmq64(uint64_t mask) {
  auto p_e8 = bm::popcnt_e8(mask) * 2;
  auto cp_e8 = p_e8 * 0x0101010101010100ull;
  constexpr auto cm_e8 = 8ull * 0x0706050403020100ull;
  constexpr int offset = 64;
  auto cc_e8 = offset * 0x0101010101010101ull + cp_e8 - cm_e8;

  unsigned argmin = 0;
  int min_s = block_width+1;
  // int sum = 0;
  constexpr uint64_t table_mask = (1ull << table_width) - 1;
  for (unsigned bi = 0; bi < block_width; bi += table_width) {
    auto submask = (mask >> bi) & table_mask;
    auto bsum = (int)((cc_e8 >> bi) & table_mask) - offset;
    // assert(sum == bsum);
    auto s = rmq_tb.val[submask] + bsum;
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
    size_t l, r, p;
  };
  size_t root_;
  std::vector<Node> nodes_;
  std::vector<T> a_;

  DecaltTree() : root_(null_idx) {}
  void push_back(const T& val) {
    a_.push_back(val);
    if (nodes_.empty()) {
      nodes_.push_back({null_idx, null_idx, null_idx});
      root_ = 0;
      return;
    }
    auto u = nodes_.size()-1;
    auto v = nodes_.size();
    while (u != null_idx and a_[u] > a_[v]) {
      u = nodes_[u].p;
    }
    if (u == null_idx) {
      nodes_.push_back({root_, null_idx, null_idx});
      nodes_[root_].p = v;
      root_ = v;
    } else {
      auto ur = nodes_[u].r;
      nodes_.push_back({ur, null_idx, u});
      if (ur != null_idx)
        nodes_[ur].p = v;
      nodes_[u].r = v;
    }
  }
  size_t size() const {
    return nodes_.size();
  }
};

template<class T, unsigned RecursionHeight>
class RmqToLca;
template<unsigned RecursionHeight>
class LcaToRmq;

template<class T, unsigned RecursionHeight = 3>
class RmqToLca {
  static constexpr size_t minimum_rmq_size = 1u<<4;
  using st_index_type = uint32_t;
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
      DecaltTree<T> tree;
      for (auto it = begin; it != end; ++it) {
        tree.push_back(*it);
      }
      lca_a_ = decltype(lca_a_)(tree);
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
  std::vector<size_t> d_;
  std::vector<size_t> l_;
  BitVector bv_;
  RmqToLca<size_t, RecursionHeight - 1> rmq_a_;

 public:
  LcaToRmq() = default;
  template<class U>
  LcaToRmq(const DecaltTree<U>& decalt_tree) : d_(decalt_tree.size(), -1), l_(2*decalt_tree.size(), -1) {
    std::vector<bool> de(2*decalt_tree.size());
    // std::vector<size_t> f(decalt_tree.size(), -1);
    size_t t = 0;
    std::stack<std::pair<size_t, bool>> st;
    st.emplace(decalt_tree.root_, true);
    st.emplace(decalt_tree.root_, false);
    while (!st.empty()) {
      auto [u, is_right] = st.top(); st.pop();
      if (!is_right) {
        d_[u] = t;
        // e.push_back(h);
        de[t] = 1;
        // l_[t] = -1; // dummy
        auto cl = decalt_tree.nodes_[u].l, cr = decalt_tree.nodes_[u].r;
        if (cr != DecaltTree<U>::null_idx) {
          st.emplace(cr, true);
          st.emplace(cr, false);
        }
        if (cl != DecaltTree<U>::null_idx) {
          st.emplace(cl, true);
          st.emplace(cl, false);
        }
      } else {
        // f[u] = t;
        // e.push_back(h-1);
        de[t] = 0;
        l_[t] = decalt_tree.nodes_[u].p;
      }
      t++;
    }
    
    auto num_blocks = (de.size() + rmq::block_width - 1) / rmq::block_width;
    bv_.resize(de.size());
    std::vector<size_t> a(num_blocks);
    {
      size_t block_sum = 0;
      size_t ei = 0, bi = 0;
      while (ei < de.size()) {
        int esum = 0;
        uint64_t mask = 0;
        int ri = 0;
        auto width = std::min(rmq::block_width, (int)(de.size()-ei));
        while (ri < width) {
          bool bit = de[ei];
          bv_[ei] = bit;
          mask |= (uint64_t) bit << ri;
          esum += bit ? 1 : -1;
          ei++;
          ri++;
        }
        if (ri < rmq::block_width) {
          auto rem = rmq::block_width - ri;
          mask |= ((1ull<<rem)-1) << ri;
          esum += rem;
          ei += rem;
          ri = rmq::block_width;
        }
        // for (int i = 0; i < rmq::block_width; i++) {
        //   bool bit = ei < de.size() ? de[ei] : 1;
        //   if (ei < de.size())
        //     bv_[ei] = bit;
        //   mask |= (uint64_t) bit << i;
        //   esum += bit ? 1 : -1;
        //   ei++;
        // }
        // a[bi] = (long long) rmq_tb.val[mask] + block_sum;
        a[bi] = (long long) rmq::rmq64(mask).second + block_sum;
        block_sum += esum;
        bi++;
      }
    }
    bv_.build();
    if (a.size() >= 3) {
      rmq_a_ = decltype(rmq_a_)(a.begin()+1, a.end()-1);
    }
  }

  size_t get_e(size_t i) const {
    return bv_.rank1(i+1) * 2 - (i+1);
  }
  
  size_t lca(size_t x, size_t y) const {
    if (d_[x] == d_[y]) return x;
    if (d_[x] > d_[y]) std::swap(x, y);
    size_t i = d_[x] / rmq::block_width;
    size_t j = d_[y] / rmq::block_width;
    size_t m;
    size_t min_e;
    { // block i
      auto r = std::min(d_[y]+1, (i+1)*rmq::block_width);
      auto mask = bv_.bitmap().range_get(d_[x], r);
      auto dist = r - d_[x];
      auto rem = rmq::block_width - dist;
      if (rem)
        mask |= ((1ull<<rem)-1) << dist;
      // auto idx = rmq_tb.idx[mask] + d_[x];
      auto idx = rmq::rmq64(mask).first + d_[x];
      auto e = get_e(idx);
      m = idx;
      min_e = e;
    }
    if (i+2 <= j) { // block i+1...j-1
      auto k = rmq_a_.rmq(i+1 - 1, j - 1) + 1; // -1 means ignoring i={0,num_block-1}
      auto mask = bv_.bitmap().range_get(k*rmq::block_width, (k+1)*rmq::block_width);
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
      auto mask = bv_.bitmap().range_get(l, d_[y]+1);
      auto dist = d_[y]+1 - l;
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
    auto ret = m == d_[x] ? x : l_[m];
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
};

template<class T>
using Rmq = RmqToLca<T>;