#pragma once
#include <cstdint>
#include <cassert>

namespace bm {

inline constexpr uint64_t popcnt_e8(uint64_t x) {
  x = (x & 0x5555555555555555) + ((x>>1) & 0x5555555555555555);
  x = (x & 0x3333333333333333) + ((x>>2) & 0x3333333333333333);
  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x>>4) & 0x0F0F0F0F0F0F0F0F);
  return x;
}
// Count 1s
inline constexpr unsigned popcnt(uint64_t x) {
  return (popcnt_e8(x) * 0x0101010101010101) >> 56;
}
// Count trailing 0s. s.t. *11011000 -> 3
inline constexpr unsigned ctz(uint64_t x) {
  return popcnt((x & (-x)) - 1);
}
inline constexpr unsigned ctz8(uint8_t x) {
  return x == 0 ? 8 : popcnt_e8((x & (-x)) - 1);
}
// [00..0](8bit) -> 0, [**..*](not only 0) -> 1
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
// Extract target area of bits
inline constexpr uint64_t bextr(uint64_t x, unsigned start, unsigned len) {
  uint64_t mask = len < 64 ? (1ull<<len)-1 : 0xFFFFFFFFFFFFFFFFull;
  return (x >> start) & mask;
}
// 00101101 -> 00111111 -count_1s-> 6
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
// 00101100 -mask_mssb-> 00100000 -to_index-> 5
inline constexpr unsigned mssb8(uint8_t x) {
  assert(x != 0);
  return log2p1(x) - 1;
}
// 00101100 -mask_lssb-> 00000100 -to_index-> 2
inline constexpr unsigned lssb8(uint8_t x) {
  assert(x != 0);
  return popcnt_e8((x & -x) - 1);
}
// Count leading 0s. 00001011... -> 4
inline constexpr unsigned clz(uint64_t x) {
  if (x == 0)
    return 64;
  auto i = mssb8(summary(x));
  auto j = mssb8(bextr(x, 8 * i, 8));
  return 63 - (8 * i + j);
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

} // namespace bm
