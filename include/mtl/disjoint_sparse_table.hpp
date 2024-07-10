#pragma once
#include "bit_manip.hpp"
#include <vector>
#include <algorithm>

template<typename T, T (*op)(T, T), T (*e)()>
class DisjointSparseTable {
private:
    int log_n_;
    size_t n_;
    std::vector<std::vector<T>> tb_;
public:
    template<typename It> 
    DisjointSparseTable(It begin, It end) : 
            log_n_(64-bm::clz(std::distance(begin, end)-1)),
            n_(1ull<<log_n_), 
            table_(std::max(log_n_, 1), std::vector<T>(n_, e())) {
        std::transform(begin, end, tb_[0].begin());
        for (int i = 1; i < log_n_; i++) {
            auto d = 1<<i;
            for (size_t j = 0; j < n_; j += d*2) {
                tb_[i][j+d-1] = tb_[i-1][j+d-1];
                for (long long k = j+d-2; k >= j; k--)
                    tb_[i][k] = op(tb_[i-1][k], tb_[i][k+1]);
                tb_[i][j+d] = tb_[i-1][j+d];
                for (long long k = j+d+1; k < j+d*2; k++)
                    tb_[i][k] = op(tb_[i-1][k], tb_[i][k-1]);
            }
        }
    }

    T query(size_t l, size_t r) const {
        if (l >= r) return E;
        if (l+1 == r) return tb_[0][l];
        size_t p = 63-bm::clz((r-1)^l);
        return op(tb_[p][l], tb_[p][r-1]);
    }
};