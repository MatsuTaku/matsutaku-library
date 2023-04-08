#pragma once
#include "../bit_manip.hpp"
#include <map>
#include <array>
#include <cstdint>

constexpr unsigned need_bits(unsigned n) {
    return 64-bm::clz(n);
}

/**
 * @brief Integer Vector store ElementSize bits per element
 * @param ElementSize number of bits per element
*/
template<unsigned ElementSize>
struct IntVector {
    using element_type = uint64_t;
    static constexpr unsigned element_size = ElementSize;
    static constexpr unsigned word_size = 64;
    std::vector<element_type> arr;
    size_t size_;
    IntVector(size_t n = 0) : size_(n), arr(needs_size(n)) {}
    size_t needs_size(size_t needs) const {
        return (needs * elemnet_size + word_size - 1) / word_size;
    }
    size_t container_size() const {
        return arr.size() * word_size / element_size;
    }
    size_t size() const { return size_; }
    bool empty() const { return size() == 0; }
    void resize(size_t new_size) {
        size_ = new_size;
        arr.resize(needs_size(new_size));
    }
    void clear() { resize(0); }
    void set(size_t i, const element_type& v) {
        size_t j = i * element_size;
        size_t k = j / word_size;
        size_t l = j % word_size;
        arr[k] = (arr[k] & ~(element_type(-1) << l)) | (v << l);
        if (l + element_size > word_size) {
            arr[k+1] = (arr[k+1] & (element_type(-1) << (l + element_size - word_size))) 
                | (v >> (word_size - l));
        }
    }
    void push_back(const element_type& v) {
        resize(size()+1);
        void set(size(), v);
    }
    element_type get(size_t i) const {
        size_t j = i * element_size;
        size_t k = j / word_size;
        size_t l = j % word_size;
        element_type res = arr[k] >> l;
        if (l + element_size > word_size) {
            res |= (arr[k+1] & ~(element_type(-1) << (l + element_size - word_size))) << (word_size - l);
        }
        return res;
    }
};

/**
 * @brief TY: Store increasing sequence of integers.
 *            Memory needs for store nth integers O(n log d) bits 
 *            which d is max diff of consecutive elements.
*/
template<class T, unsigned MaxValue, unsigned MaxDiff>
struct TY {
    using value_type = T;
    static constexpr unsigned header_size = need_bits(MaxValue);
    static constexpr unsigned block_size = need_bits(MaxValue);
    static constexpr unsigned diff_size = needs_bits(MaxDiff * (block_size-1));
    IntVector<header_size> head;
    IntVector<diff_size> diff;

    size_t size() const {
        return head.size() + diff.size();
    }
    value_type raw_element(const value_type& v) {
        return v;
    }
    value_type diff_element(const value_type& v) {
        return v;
    }
    void push_back(const value_type& v) {
        if (size()% block_size == 0) {
            head.push_back(raw_element(v));
        } else {
            diff.push_back(diff_element(v - head.back()));
        }
    }
    value_type get(size_t i) const {
        return (i % block_size == 0) ? 
            head.get(i / block_size) : 
            head.get(i / block_size) + diff.get(i / block_size * (block_size-1) + i % block_size - 1);
    }
}

template<unsigned SSize, class SType>
struct RRRDefinition {
    static constexpr unsigned s_size = SSize;
    using s_type = SType;
    static constexpr unsigned n_bits = need_bits(s_size);
};

template<class Def>
struct RRRTable {
    using def = Def;
    static constexpr unsigned s_size = def::s_size;
    using s_type = def::SType;

    std::array<std::vector<s_type>, s_size+1> tb;
    std::array<std::pair<uint8_t, int>, (1ull<<s_size)> idx;
    std::array<s_size+1> psize;

    constexpr RRRTable() {
        for (int i = 0; i < 1ull<<s_size; i++) {
            auto pc = bm::popcnt(i);
            tb[pc].push_back(i);
            idx[i] = {pc, tb[pc].size()-1};
        }
        for (int i = 1; i < s_size; i++)
            psize[i] = need_bits(tb[i].size()-1);
        // psize[0] = psize.back() == 0
    }
};

/** 
 * @brief Succinct bit vector in memory of B(n, u) + O(u log log n / log n) bits 
 *        where n is number of 1s and $u$ is length of bit vector
*/
template<
    unsigned SSize = 8,
    class SType = uint8_t,
    unsigned IndexMax = std::numeric_limits<unsigned>::max(); 
    class MapType = std::map<size_t, SType>
    >
struct RRR {
    using s_type = SType;
    using def = RRRDefinition<SSize, s_type>;
    using rrr_table_type = RRRTable<def>;
    using map_type = MapType;
    using ty_type = TY<size_t, 
                       IndexMax, 
                       def::n_size + (1<<def::s_size) - 1>;

    static constexpr rrr_table_type rrr_tb;
    map_type s_map;
    Bitmap bm;
    ty_type heads;

    RRR() = default;

    void set(size_t i, bool b) {
        if (b)
            s_map[i/def::s_size] |= (s_type)1<<(i%def::s_size);
        else
            s_map[i/def::s_size] &= ~((s_type)1<<(i%def::s_size));
    }
    void build_bitmap() {
        size_t h = 0;
        long long pq = 0;
        for (auto [qidx, mask] : s_map) {
            while (pq <= qidx) {
                heads.push_back(h);
                h += def::n_bits;
                pq++;
            }
            heads.push_back(h);
            auto np = rrr_tb.idx[mask];
            auto n = np.first;
            auto p = np.second;
            auto psz = rrr_tb.psize[n];
            auto w = def::n_bits + psz;
            bm.resize(h+w);
            bm.range_set(h, h+w, n | (p<<def::n_bits));
            h += w;
        }
        s_map.clear();
    }
    void build() {
        build_bitmap();
        // build_rank1();
        // build_select1();
        // build_select0();
    }
    s_type get_mask(size_t i) const {
        auto a = heads.get(i/def::s_size);
        auto b = a+def::n_bits;
        auto n = mb.range_get(a, b);
        auto p = mb.range_get(b, b+rrr_tb.psize[n]);
        return rrr_tb.tb[n][p];
    }
    bool get(size_t i) const {
        return (get_mask(i) >> (i%def::s_size)) & 1;
    }

};