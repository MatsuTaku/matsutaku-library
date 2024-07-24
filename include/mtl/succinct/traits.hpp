#pragma once
#include "bv.hpp"
#include "bitmap.hpp"
#include "rrr.hpp"

template<class T>
struct RankSelectTraits : std::false_type {};

template<>
struct RankSelectTraits<Bitmap> {
  using rank_select_type = BV<Bitmap, 64>;
};

template<unsigned SSize, class SType, class MapType>
struct RankSelectTraits<RRR<SSize, SType, MapType>> {
    using rank_select_type = BV<RRR<SSize, SType, MapType>, SSize>;
};