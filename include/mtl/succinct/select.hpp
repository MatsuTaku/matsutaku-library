#pragma once
#include "../bit_manip.hpp"
#include <array>

struct select64 {
    using size_type = uint8_t;
    static struct make_select_table {
        using bitmap_type = uint8_t;
        std::array<std::array<size_type, 9>, 1<<8> tb;
        make_select_table() {
            for (int i = 0; i < 1<<8; i++) {
                int c = 0;
                int x = i;
                tb[i].fill(8);
                for (int j = 0; j < 8; j++) {
                    if (x & 1)
                    tb[i][++c] = j;
                    x >>= 1;
                }
            }
        }
        size_type operator()(bitmap_type bitmap, size_type ith) const {
            return tb[bitmap][ith];
        }
    } sel_tb;
    template<bool B>
    static constexpr size_type select(size_type ith, uint64_t bitmap) { // 0-indexed
        assert(ith < 64);
        ith++; // to 1-index
        // Find byte
        uint64_t w = bitmap;
        if constexpr (!B) w = ~w;
        auto _bs = (uint64_t) bm::popcnt_e8(w) * 0x0101010101010100ull;
        auto bs = (const uint8_t*) &_bs;
        size_type b = 0;
        auto o = ith;
        /* Following bit-manipulates code is same as ... */
        // {
        //     auto d = 8;
        //     while (d > 1) {
        //     auto c = b + d/2;
        //     if (bs[c] < o)
        //         b = c;
        //     d /= 2;
        //     }
        // }
        {
            uint64_t x = (uint64_t) o * 0x0101010101010101ull;
            uint64_t bmx = (_bs | 0x8080808080808080ull) - x;
            uint64_t y = ((bmx & 0x8080808080808080ull) * 0x02040810204081ull) >> (64-8);
            size_type nb = bm::ctz8(y) - 1;
            // assert(b == nb);
            b = nb;
        }
        // Calc select
        auto r = o - bs[b];
        uint8_t byte = ((const uint8_t*)(&w))[b];
        assert(r and r <= (size_type)bm::popcnt(byte));
        return b * 8 + sel_tb(byte, r);
    }
    static constexpr size_type select1(size_type ith, uint64_t bitmap) {
        return select<1>(ith, bitmap);
    }
    static constexpr size_type select0(size_type ith, uint64_t bitmap) {
        return select<0>(ith, bitmap);
    }
};

typename select64::make_select_table select64::sel_tb;