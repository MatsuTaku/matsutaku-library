#include <binary_trie.hpp>
#include "set_test.hpp"

int main() {
  mtl::integer_set_test<BinaryTrieSet<unsigned, 20>, 1<<20>();
  mtl::integer_set_test<BinaryTrieSet<unsigned, 20>, 1<<20, false>();
  mtl::map_emplace_test<BinaryTrieMap<unsigned, std::vector<int>>>();
  std::cout << "OK" << std::endl;
}
