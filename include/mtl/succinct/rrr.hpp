#pragma once
#include "../bit_manip.hpp"
#include "bit_vector.hpp"
#include "ty.hpp"
#include "bv.hpp"
#include <map>
#include <array>
#include <cstdint>
#include <limits>
#include <cstddef>
#include <cstdint>
#include <vector>

constexpr unsigned need_bits(uint64_t n) {
    return 64-bm::clz(n);
}

template<unsigned N>
struct BinomialTable {
    static_assert(N < 64, 
        "Too large N for BinomialTable. N must be less than 64");
    using number_type = uint64_t;
    static struct impl {
        std::array<std::array<number_type, N+1>, N+1> tb;
        constexpr impl() {
            tb[0][0] = 1;
            for (size_t i = 1; i <= N; i++) {
                tb[i][0] = tb[i-1][0];
                for (size_t j = 1; j <= i; j++) 
                    tb[i][j] = tb[i-1][j-1] + tb[i-1][j];
            }
        }
    } data;
    static constexpr number_type binom(size_t n, size_t k) {
        assert(n <= N and k <= N);
        return data.tb[n][k];
    }
};

template<unsigned N>
typename BinomialTable<N>::impl BinomialTable<N>::data;

template<class Def>
struct RRRTable {
    using def = Def;
    static constexpr unsigned s_size = def::s_size;
    using s_type = typename def::s_type;
    static constexpr unsigned n_bits = def::n_bits;
    using binomial_table_type = BinomialTable<s_size>;
    using number_type = typename binomial_table_type::number_type;
    static constexpr s_type get_int(unsigned n, number_type k, unsigned bits = s_size) {
        s_type res = 0;
        const auto offset = bits;
        s_type mask = ((s_type(1)<<bits)-1);
        auto nn = n;
        unsigned i = 0;
        // Expected length of consecutive zeros E = \sum j binom(w-j, nn) / binom(w, nn), j=1..w-nn
        // Binary search time B = \log_2 w
        // Approximate simple function from E < B to be nn > min(20, w-1)
        for (; i < offset and nn > 20; i++) {
            auto w = s_size - i;
            if (nn == w) {
                res |= ((s_type(1)<<nn)-1) << i;
                return res & mask;
            }
            // Linear search
            auto binom = binomial_table_type::binom(w-1, nn);
            if (k >= binom) {
                res |= s_type(1) << i;
                k -= binom;
                nn--;
            }
        }
        for (; i < offset and nn > 1; i++) {
            auto w = s_size - i;
            if (nn == w) {
                res |= ((s_type(1)<<nn)-1) << i;
                return res & mask;
            }
            // Binary search
            auto l = i, r = offset+1;
            while (l+1<r) {
                auto c = l+(r-l)/2;
                if (k < binomial_table_type::binom(s_size-c, nn))
                    l = c;
                else 
                    r = c;
            }
            if (l < offset) {
                res |= s_type(1) << l;
                k -= binomial_table_type::binom(s_size-l-1, nn);
                nn--;
            }
            i = l;
        }
        if (nn == 0) 
            return res;
        if (nn == 1) {
            res |= s_type(1) << (s_size-1-k);
            return res & mask;
        }
        if (k >= binomial_table_type::binom(s_size-offset-1, nn))
            res |= s_type(1) << offset;
        return res & ((s_type(1)<<bits)-1);
    }
    static constexpr bool get_bit(unsigned n, number_type k, unsigned offset) {
        auto nn = n;
        unsigned i = 0;
        // E: expected continuous zeros = \sum j binom(w-j, nn) / binom(w, nn), j=0..w-nn
        // binary search time for \log_2 w
        // Approximate function for (1) is nn > min(20, w-1)
        for (; i < offset and nn > 20; i++) {
            auto w = s_size - i;
            if (nn == w) {
                return 1;
            }
            // linear search
            auto binom = binomial_table_type::binom(w-1, nn);
            if (k >= binom) {
                k -= binom;
                nn--;
            }
        }
        for (; i < offset and nn > 1; i++) {
            auto w = s_size - i;
            if (nn == w) {
                return 1;
            }
            // binary search
            auto l = i, r = offset+1;
            while (l+1<r) {
                auto c = l+(r-l)/2;
                if (k < binomial_table_type::binom(s_size-c, nn))
                    l = c;
                else 
                    r = c;
            }
            if (l < offset) {
                k -= binomial_table_type::binom(s_size-l-1, nn);
                nn--;
            }
            i = l;
        }
        if (nn == 0) 
            return 0;
        if (nn == 1)
            return offset == s_size-1-k;
        return k >= binomial_table_type::binom(s_size-offset-1, nn);
    }
    static std::pair<unsigned, number_type> get_pc_and_number(s_type bitmap) {
        unsigned pc = bm::popcnt(bitmap);
        number_type number = 0;
        auto m = bitmap;
        auto n = pc;
        while (m) {
            auto i = bm::ctz(m);
            number += binomial_table_type::binom(s_size-i-1, n);
            n--;
            m ^= (s_type(1)<<i);
        }
        return {pc, number};
    }
    static number_type number_size(unsigned n) {
        return binomial_table_type::binom(s_size, n);
    }
    static struct number_bits_table {
        std::array<unsigned, s_size+1> tb;
        number_bits_table() {
            for (unsigned i = 0; i <= s_size; i++)
                tb[i] = need_bits(number_size(i)-1);
        }
    } n_len;
    static unsigned number_bits(unsigned n) {
        return n_len.tb[n];
    }

};

template<class Def>
typename RRRTable<Def>::number_bits_table RRRTable<Def>::n_len;

template<unsigned SSize, class SType, unsigned LSize, class LType>
struct RRRDefinition {
    static constexpr unsigned s_size = SSize;
    using s_type = SType;
    static constexpr unsigned n_bits = need_bits(s_size);
    static constexpr unsigned l_size = LSize;
    using l_type = LType;
};

/** 
 * @brief Succinct bit vector in memory of B(n, u) + O(u log log n / log n) bits 
 *        where n is number of 1s and $u$ is length of bit vector
*/
template<
    unsigned SSize = 63,
    class SType = uint64_t,
    unsigned LSize = 512,
    class LType = uint64_t,
    class MapType = std::map<size_t, SType>
    >
struct RRR {
    using def = RRRDefinition<SSize, SType, LSize, LType>;
    using s_type = typename def::s_type;
    using l_type = typename def::l_type;
    using rrr_table_type = RRRTable<def>;
    using map_type = MapType;
    using ty_type = TY<size_t>;
    using rs_type = BV<RRR, def::s_size>;

    map_type s_map;
    ty_type heads;
    Bitmap bm;
    std::vector<l_type> l_map;

    RRR() = default;

    void set(size_t i, bool b) {
        if (b)
            s_map[i/def::s_size] |= (s_type)1<<(i%def::s_size);
        else
            s_map[i/def::s_size] &= ~((s_type)1<<(i%def::s_size));
    }
    void build_bitmap() {
        size_t h = 0;
        size_t pq = 0;
        for (auto qm : s_map) {
            auto qidx = qm.first;
            auto mask = qm.second;
            while (pq < qidx) {
                heads.push_back(h);
                auto w = def::n_bits;
                bm.resize(h+w);
                bm.range_set(h, h+w, 0);
                h += w;
                pq++;
            }
            heads.push_back(h);
            auto np = rrr_table_type::get_pc_and_number(mask);
            auto n = np.first;
            auto p = np.second;
            assert(rrr_table_type::get_int(n, p) == mask);
            auto w = def::n_bits + rrr_table_type::number_bits(n);
            bm.resize(h+w);
            bm.range_set(h, h+def::n_bits, n);
            bm.range_set(h+def::n_bits, h+w, p);
            assert(rrr_table_type::get_int(
                bm.range_get(h, h+def::n_bits), bm.range_get(h+def::n_bits, h+w)) == mask);
            h += w;
            pq++;
        }
        s_map.clear();
    }
    // void build_rank() {
    //     auto num_l = (heads.size() + s_per_l - 1) / s_per_l;
    //     auto l_arr_size = (num_l + 63) / 64;
    //     l_map.resize(l_arr_size);
    //     size_t lsum = 0;
    //     for (size_t l = 0; l < num_l; l++) {
    //         size_t ssum = 0;
    //         for (size_t s = 0; s < s_per_l; s++) {
    //             auto si = s*s_per_l + s;
    //             auto m = si < heads.size() ? get_mask(si) : 0;
    //             auto pc = bm::popcnt(m);
    //             ssum += pc;
    //         }
    //         lsum += ssum;
    //     }
    // }
    void build() {
        build_bitmap();
        // build_rank();
        // build_select1();
        // build_select0();
    }
    bool get_bit(size_t si, unsigned off) const {
        if (si >= heads.size())
            return false;
        auto a = heads.get(si);
        auto b = a+def::n_bits;
        auto n = bm.range_get(a, b);
        auto p = bm.range_get(b, b+rrr_table_type::number_bits(n));
        return rrr_table_type::get_bit(n, p, off);
    }
    s_type get_mask(size_t si) const {
        if (si >= heads.size())
            return 0;
        auto a = heads.get(si);
        auto b = a+def::n_bits;
        auto n = bm.range_get(a, b);
        auto p = bm.range_get(b, b+rrr_table_type::number_bits(n));
        return rrr_table_type::get_int(n, p);
    }
    uint64_t get_word(size_t si) const {
        return get_mask(si);
    }
    size_t word_size() const {
        return heads.size();
    }
    size_t size() const {
        return heads.size()*def::s_size;
    }
    bool empty() const {
        return size() == 0;
    }

    bool get(size_t i) const {
        return get_bit(i/def::s_size, i%def::s_size);
    }

};