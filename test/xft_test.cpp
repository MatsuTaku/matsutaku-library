#include <xft.hpp>
#include "set_test.hpp"

int main() {
  mtl::integer_set_test<XFastTrieSet<unsigned, 20>, (int)4e5>();
  std::cout << "OK" << std::endl;
}
