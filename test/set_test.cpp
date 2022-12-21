#include <set>
#include "set_test.hpp"

int main() {
  mtl::integer_set_test<std::set<unsigned>>();
  std::cout << "OK" << std::endl;
}
