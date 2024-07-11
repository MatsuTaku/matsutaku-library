#define STANDALONE
#include "../../include/mtl/double_ended_priority_queue.hpp"
#include <vector>
#include <random>
#include <iostream>
#include <set>
#include <cassert>

using P = std::pair<int,int>;
constexpr int size = 1<<16;

bool external_sorting_step_test() {
  std::vector<int> A(size);
  for (int i = 0; i < size; i++) A[i] = rand()%size;
  std::multiset<int> s;
  DoubleEndedPriorityQueue<int> qs;
  constexpr int memory_size = size>>3;
  for (int i = 0; i < memory_size; i++) {
    s.insert(A[i]);
    qs.push(A[i]);
  }
  for (int i = memory_size; i < size; i++) {
    if (s.size() != qs.size()) {
      std::cerr<<"s.size != qs.size: "<<s.size()<<", "<<qs.size()<<std::endl;
      return false;
    }
    if (*--s.end() != qs.max()) {
      std::cerr<<"s.max() != qs.max(): "<<(*--s.end())<<", "<<qs.max()<<std::endl;
      return false;
    }
    if (*s.begin() != qs.min()) {
      std::cerr<<"s.min() != qs.min(): "<<(*s.begin())<<", "<<qs.min()<<std::endl;
      return false;
    }
    if (A[i] < qs.min()) {
      // Push A[i] to left array
    } else if (A[i] > qs.max()) {
      // Push A[i] to right array
    } else {
      auto b = rand()%2;
      if (b) {
        s.erase(--s.end());
        qs.pop_max();
        // Push max pivot to right array
      } else {
        s.erase(s.begin());
        qs.pop_min();
        // Push min pivot to left array
      }
      s.insert(A[i]);
      qs.push(A[i]);
    }
  }
  // Push remaining values to middle array
  // Return external_sorting(left_array) + middle_array + external_sorting(right_array)
  return true;
}

int main() {
  if (!external_sorting_step_test()) {
    assert(false);
    exit(EXIT_FAILURE);
  }

  std::cout << "OK" << std::endl;
}
