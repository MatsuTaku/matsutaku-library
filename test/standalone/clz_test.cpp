#define STANDALONE
#include "../../include/mtl/bit_manip.hpp"
#include <iostream>
#include <bitset>

int main() {
  static constexpr size_t cases = 1e6;
  for (int i = 0; i < cases; i++) {
    uint64_t x = ((unsigned long long)rand() << 32) | rand();
    int cnt = 0;
    while (cnt < 64 and (x & (1ull<<(63-cnt))) == 0)
      cnt++;
    auto val = bm::clz(x);
    if (cnt != val) {
      std::cerr<<"clz("<<(std::bitset<64>(x))<<") is "<<cnt
      <<". calculated: "<<val<<std::endl;
      assert(false);
    }
  }
}
