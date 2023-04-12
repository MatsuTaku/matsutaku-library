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
        /*
        Binary search time B = ceil(\log_2 w)
        Expected length of consecutive zeros Ez = \sum j binom(w-j, nn) / binom(w, nn), j=1..w-nn
        Expected length of consecutive ones  Eo = \sum j binom(w-j, nn-j) / binom(w, nn), j=1..nn
        Approximate simple function from Ez > B to be nn <= min(20, w-1)
        Approximate simple function from Eo > B to be nn > min(40, w)
        */
        // TODO: When nn > 40, use binary search to find length of consecutive ones
        for (; i < offset and nn > 20; i++) {
            auto w = s_size - i;
            if (nn == w) {
                res |= ((s_type(1)<<nn)-1) << i;
                return res & mask;
            }
            if (nn == w-1) {
                res |= (((s_type(1)<<w)-1) ^ (s_type(1) << k)) << i;
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
            if (nn == w-1) {
                res |= (((s_type(1)<<w)-1) ^ (s_type(1) << k)) << i;
                return res & mask;
            }
            // Binary search to find length of consecutive zeros
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
        if (nn == 1) {
            res |= s_type(1) << (s_size-1-k);
            return res & mask;
        }
        if (nn == 0) 
            return res;
        if (k >= binomial_table_type::binom(s_size-offset-1, nn))
            res |= s_type(1) << offset;
        return res & ((s_type(1)<<bits)-1);
    }
    static constexpr bool get_bit(unsigned n, number_type k, unsigned offset) {
        auto nn = n;
        unsigned i = 0;
        /*
        Binary search time B = ceil(\log_2 w)
        Expected length of consecutive zeros Ez = \sum j binom(w-j, nn) / binom(w, nn), j=1..w-nn
        Expected length of consecutive ones  Eo = \sum j binom(w-j, nn-j) / binom(w, nn), j=1..nn
        Approximate simple function from Ez > B to be nn <= min(20, w-1)
        Approximate simple function from Eo > B to be nn > min(40, w)
        */
        // TODO: When nn > 40, use binary search to find length of consecutive ones
        for (; i < offset and nn > 20; i++) {
            auto w = s_size - i;
            if (nn == w) {
                return 1;
            }
            if (nn == w-1) {
                return offset != i+k;
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
            if (nn == w-1) {
                return offset != i+k;
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
        if (nn == 1)
            return offset == s_size-1-k;
        if (nn == 0) 
            return 0;
        return k >= binomial_table_type::binom(s_size-offset-1, nn);
    }
    static number_type get_number_for_popcnt(s_type bitmap, unsigned pc) {
        number_type number = 0;
        auto m = bitmap;
        auto n = pc;
        while (m) {
            auto i = bm::ctz(m);
            number += binomial_table_type::binom(s_size-i-1, n);
            n--;
            m ^= (s_type(1)<<i);
        }
        return number;
    }
    static std::pair<unsigned, number_type> get_pc_and_number(s_type bitmap) {
        unsigned pc = bm::popcnt(bitmap);
        auto number = pc <= s_size-pc ? get_number_for_popcnt(bitmap, pc) 
                                      : (number_size(pc)-1-get_number_for_popcnt(
                                            ~bitmap & ((s_type(1)<<s_size)-1), s_size-pc));
        return std::make_pair(pc, number);
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

template<unsigned SSize, class SType>
struct RRRDefinition {
    static constexpr unsigned s_size = SSize;
    using s_type = SType;
    static constexpr unsigned n_bits = need_bits(s_size);
};

/** 
 * @brief Succinct bit vector in memory of B(n, u) + O(u log log n / log n) bits 
 *        where u is number of bits and n is number of 1s
*/
template<
    unsigned SSize = 63,
    class SType = uint64_t,
    class MapType = std::map<size_t, SType>
    >
struct RRR {
    using def = RRRDefinition<SSize, SType>;
    using s_type = typename def::s_type;
    using rrr_table_type = RRRTable<def>;
    using map_type = MapType;
    using ty_type = TY<size_t>;

    map_type s_map;
    ty_type heads;
    Bitmap bm;

    RRR() = default;
    void set(size_t i, bool b) {
        if (b)
            s_map[i/def::s_size] |= (s_type)1<<(i%def::s_size);
        else
            s_map[i/def::s_size] &= ~((s_type)1<<(i%def::s_size));
    }
    void build() {
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
        return heads.size() * def::s_size;
    }
    bool empty() const {
        return size() == 0;
    }

    bool get(size_t i) const {
        return get_bit(i/def::s_size, i%def::s_size);
    }

};

template<unsigned SSize, class SType, class MapType>
struct RankSelectTraits<RRR<SSize, SType, MapType>> {
    using rank_select_type = BV<RRR<SSize, SType, MapType>, SSize>;
};