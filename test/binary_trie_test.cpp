#include <binary_trie.hpp>
#include "set_test.hpp"

int main() {
  mtl::integer_set_test<BinaryTrieSet<unsigned, 20>, 1<<20>();
  mtl::integer_set_test<BinaryTrieSet<unsigned, 20>, 1<<20, false>();
  std::cout << "OK" << std::endl;
}
