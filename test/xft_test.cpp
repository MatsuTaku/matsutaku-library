#include <xft.hpp>
#include "set_test.hpp"

int main() {
  mtl::integer_set_test<XFastTrieSet<unsigned, 20>, 1<<20>();
  mtl::integer_set_test<XFastTrieSet<unsigned, 20>, 1<<20, false>();
  std::cout << "OK" << std::endl;
}
