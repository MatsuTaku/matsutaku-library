#define STANDALONE
#include "set_test.hpp"
#include <set>

int main() {
  mtl::integer_set_test<std::set<unsigned>, 1<<20>();
  mtl::integer_set_test<std::set<unsigned>, 1<<20, false>();
  std::cout << "OK" << std::endl;
}
