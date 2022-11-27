#pragma once
#include <vector>
#include <iterator>

template<typename It>
std::vector<int> z_algorithm(It begin, It end) {
  using trait = std::iterator_traits<It>;
  static_assert(std::is_base_of<std::random_access_iterator_tag, typename trait::iterator_category>::value, "");
  int n = end - begin;
  std::vector<int> z(n);
  z[0] = n;
  int i = 1, j = 0;
  while (i < n) {
    while (i+j < n and *(begin + i+j) == *(begin + j))
      ++j;
    if (j == 0) {
      ++i;
      continue;
    }
    z[i] = j;
    int k = 1;
    while (k < j and k + z[k] < j) {
      z[i+k] = z[k];
      ++k;
    }
    i += k;
    j -= k;
  }
  return z;
}