#pragma once
#include "bitmap.hpp"
#include "bv.hpp"

template<class T>
struct RankSelectTraits : std::false_type {};

template<>
struct RankSelectTraits<Bitmap> {
  using rank_select_type = BV<Bitmap, 64>;
};