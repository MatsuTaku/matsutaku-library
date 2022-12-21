#include <yft.hpp>
#include "set_test.hpp"

int main() {
  mtl::integer_set_test<YFastTrieSet<unsigned, 20>, 1<<20>();
  mtl::integer_set_test<YFastTrieSet<unsigned, 20>, 1<<20, false>();
  std::cout << "OK" << std::endl;
}
