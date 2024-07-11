#define STANDALONE
#include "include/mtl/xft.hpp"
#include "set_test.hpp"

int main() {
  mtl::integer_set_test<XFastTrieSet<unsigned, 20>, 1<<20>();
  mtl::integer_set_test<XFastTrieSet<unsigned, 20>, 1<<20, false>();
  mtl::map_emplace_test<XFastTrieMap<unsigned, std::vector<int>>>();
  std::cout << "OK" << std::endl;
}
