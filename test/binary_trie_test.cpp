#include <binary_trie.hpp>
#include "set_test.hpp"

int main() {
  mtl::integer_set_test<BinaryTrieSet<unsigned, 20>, (int)4e5>();
  std::cout << "OK" << std::endl;
}
