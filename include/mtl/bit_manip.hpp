#pragma once
#include <cstdint>
#include <cassert>
#if __cplusplus >= 202002L
#ifndef MTL_CPP20
#define MTL_CPP20
#endif
#include <bit>
#endif

namespace bm {

/// Count 1s for each 8 bits
inline constexpr uint64_t popcnt_e8(uint64_t x) {
  x = (x & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);
  x = (x & 0x3333333333333333) + ((x>>2) & 0x3333333333333333);
  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) & 0x0F0F0F0F0F0F0F0F);
  return x;
}
/// Count 1s
inline constexpr unsigned popcnt(uint64_t x) {
#ifdef MTL_CPP20
  return std::popcount(x);
#else
  return (popcnt_e8(x) * 0x0101010101010101) >> 56;
#endif
}
/// Alias to mtl::popcnt(x)
constexpr unsigned popcount(uint64_t x) {
  return popcnt(x);
}
/// Count trailing 0s. s.t. *11011000 -> 3
inline constexpr unsigned ctz(uint64_t x) {
#ifdef MTL_CPP20
  return std::countr_zero(x);
#else
  return popcnt((x & (-x)) - 1);
#endif
}
/// Alias to mtl::ctz(x)
constexpr unsigned countr_zero(uint64_t x) {
  return ctz(x);
}
/// Count trailing 1s. s.t. *11011011 -> 2
inline constexpr unsigned cto(uint64_t x) {
#ifdef MTL_CPP20
  return std::countr_one(x);
#else
  return ctz(~x);
#endif
}
/// Alias to mtl::cto(x)
constexpr unsigned countr_one(uint64_t x) {
  return cto(x);
}
inline constexpr unsigned ctz8(uint8_t x) {
  return x == 0 ? 8 : popcnt_e8((x & (-x)) - 1);
}
/// [00..0](8bit) -> 0, [**..*](not only 0) -> 1
inline constexpr uint8_t summary(uint64_t x) {
  constexpr uint64_t hmask = 0x8080808080808080ull;
  constexpr uint64_t lmask = 0x7F7F7F7F7F7F7F7Full;
  auto a = x & hmask;
  auto b = x & lmask;
  b = hmask - b;
  b = ~b;
  auto c = (a | b) & hmask;
  c *= 0x0002040810204081ull;
  return uint8_t(c >> 56);
}
/// Extract target area of bits
inline constexpr uint64_t bextr(uint64_t x, unsigned start, unsigned len) {
  uint64_t mask = len < 64 ? (1ull<<len)-1 : 0xFFFFFFFFFFFFFFFFull;
  return (x >> start) & mask;
}
/// 00101101 -> 00111111 -count_1s-> 6
inline constexpr unsigned log2p1(uint8_t x) {
  if (x & 0x80)
    return 8;
  uint64_t p = uint64_t(x) * 0x0101010101010101ull;
  p -= 0x8040201008040201ull;
  p = ~p & 0x8080808080808080ull;
  p = (p >> 7) * 0x0101010101010101ull;
  p >>= 56;
  return p;
}
/// 00101100 -mask_mssb-> 00100000 -to_index-> 5
inline constexpr unsigned mssb8(uint8_t x) {
  assert(x != 0);
  return log2p1(x) - 1;
}
/// 00101100 -mask_lssb-> 00000100 -to_index-> 2
inline constexpr unsigned lssb8(uint8_t x) {
  assert(x != 0);
  return popcnt_e8((x & -x) - 1);
}
/// Count leading 0s. 00001011... -> 4
inline constexpr unsigned clz(uint64_t x) {
#ifdef MTL_CPP20
  return std::countl_zero(x);
#else
  if (x == 0)
    return 64;
  auto i = mssb8(summary(x));
  auto j = mssb8(bextr(x, 8 * i, 8));
  return 63 - (8 * i + j);
#endif
}
/// Alias to mtl::clz(x)
constexpr unsigned countl_zero(uint64_t x) {
  return clz(x);
}
/// Count leading 1s. 11110100... -> 4
inline constexpr unsigned clo(uint64_t x) {
#ifdef MTL_CPP20
  return std::countl_one(x);
#else
  return clz(~x);
#endif
}
/// Alias to mtl::clo(x)
constexpr unsigned countl_one(uint64_t x) {
  return clo(x);
}

inline constexpr unsigned clz8(uint8_t x) {
  return x == 0 ? 8 : 7 - mssb8(x);
}
inline constexpr uint64_t bit_reverse(uint64_t x) {
  x = ((x & 0x00000000FFFFFFFF) << 32) | ((x & 0xFFFFFFFF00000000) >> 32);
  x = ((x & 0x0000FFFF0000FFFF) << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);
  x = ((x & 0x00FF00FF00FF00FF) << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);
  x = ((x & 0x0F0F0F0F0F0F0F0F) << 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);
  x = ((x & 0x3333333333333333) << 2) | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);
  x = ((x & 0x5555555555555555) << 1) | ((x & 0xAAAAAAAAAAAAAAAA) >> 1);
  return x;
}

/// Check if x is power of 2. 00100000 -> true, 00100001 -> false
constexpr bool has_single_bit(uint64_t x) noexcept {
#ifdef MTL_CPP20
  return std::has_single_bit(x);
#else
  return x != 0 && (x & (x - 1)) == 0;
#endif
}

/// Bit width needs to represent x. 00110110 -> 6
constexpr int bit_width(uint64_t x) noexcept {
#ifdef MTL_CPP20
  return std::bit_width(x);
#else
  return 64 - clz(x);
#endif
}

/// Ceil power of 2. 00110110 -> 01000000
constexpr uint64_t bit_ceil(uint64_t x) {
#ifdef MTL_CPP20
  return std::bit_ceil(x);
#else
  if (x == 0) return 1;
  return 1ull << bit_width(x - 1);
#endif
}

/// Floor power of 2. 00110110 -> 00100000
constexpr uint64_t bit_floor(uint64_t x) {
#ifdef MTL_CPP20
  return std::bit_floor(x);
#else
  if (x == 0) return 0;
  return 1ull << (bit_width(x) - 1);
#endif
}

} // namespace bm
