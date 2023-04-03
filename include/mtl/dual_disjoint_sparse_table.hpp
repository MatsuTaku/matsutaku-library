#pragma once
#include "bit_manip.hpp"
#include <vector>
#include <algorithm>

template<typename T, T (*op)(T, T), T (*e)()>
class DualDisjointSparseTable {
private:
    int log_n_;
    size_t n_;
    std::vector<std::vector<T>> tb_;
public:
    DualDisjointSparseTable(size_t n) : 
            log_n_(64-bm::clz(n-1)),
            n_(1ull<<log_n_), 
            tb_(std::max(log_n_, 1), std::vector<T>(n_, e())) {}
    template<typename It>
    DualDisjointSparseTable(It begin, It end) : 
            DualDisjointSparseTable(std::distance(begin, end)) {
        std::transform(begin, end, tb_[0].begin(), [](const auto& v) {return v;});
    }

    void apply(size_t l, size_t r, T a) {
        if (l >= r) return;
        if (r-l==1) {
            tb_[0][l] = op(tb_[0][l], a);
            return;
        }
        auto d = 63-bm::clz((r-1)^l);
        tb_[d][l] = op(tb_[d][l], a);
        tb_[d][r-1] = op(tb_[d][r-1], a);
    }

    void build() {
        for (int i = 1; i < log_n_; i++) {
            auto d = 1<<i;
            for (size_t j = 0; j < n_; j += d*2) {
                T ml = e();
                for (size_t k = j; k < j+d; k++) {
                    ml = op(ml, tb_[i][k]);
                    tb_[0][k] = op(tb_[0][k], ml);
                }
                T mr = e();
                for (size_t k = j+d*2-1; k >= j+d; k--) {
                    mr = op(mr, tb_[i][k]);
                    tb_[0][k] = op(tb_[0][k], mr);
                }
            }
        }
    }

    T get(size_t i) const {
        return tb_[0][i];
    }
};