#include <bit_manip.hpp>
#include <iostream>
#include <bitset>

int main() {
  static constexpr size_t cases = 1e6;
  for (int i = 0; i < cases; i++) {
    uint64_t x = ((unsigned long long)rand() << 32) | rand();
    int cnt = 0;
    for (int i = 0; i < 64; i++) {
      cnt += (x & (1ull<<i)) != 0;
    }
    auto val = bm::popcnt(x);
    if (cnt != val) {
      std::cerr<<"popcnt("<<(std::bitset<64>(x))<<") is "<<cnt
      <<". calculated: "<<val<<std::endl;
    }
  }
}
