#include <range_minimum_query.hpp>
#include <bits/stdc++.h>
using namespace std;

int main() {
  if (!LcaToRmq<1>::test_right_path_to_children_construction()) {
    assert(false);
  }
  std::cout<<"OK"<<std::endl;
}