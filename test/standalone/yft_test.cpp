#define STANDALONE
#include "../../include/mtl/yft.hpp"
#include "set_test.hpp"

int main() {
  mtl::integer_set_test<YFastTrieSet<unsigned, 20>, 1<<20>();
  mtl::integer_set_test<YFastTrieSet<unsigned, 20>, 1<<20, false>();
  mtl::map_emplace_test<YFastTrieMap<unsigned, std::vector<int>>>();
  std::cout << "OK" << std::endl;
}
