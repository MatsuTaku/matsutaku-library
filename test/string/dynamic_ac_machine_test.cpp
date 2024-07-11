#define STANDALONE
#include "include/mtl/string/dynamic_ac_machine.hpp"

#include <iostream>
#include <cassert>
#include <vector>
#include <cstring>

bool compare_output(std::vector<std::vector<std::string>>& a, std::vector<std::vector<std::string>>& b) {
  std::vector<std::string> empty;
  for (size_t i = 0; i < std::max(a.size(), b.size()); i++) {
    std::vector<std::string>& l = ((i >= a.size()) ? empty : a[i]);
    std::vector<std::string>& r = ((i >= b.size()) ? empty : b[i]);
    if (l != r) {
      return true;
    }
  }
  return false;
}

int main() {
  std::vector<std::string> keys{"ab", "bc", "bab", "d", "abcde"};
  std::vector<std::vector<std::string>> outputs{
      {},
      {},
      {"ab"},
      {},
      {"bc"},
      {},
      {"bab", "ab"},
      {"d"},
      {"bc"},
      {"d"},
      {"abcde"},
  };
  std::vector<std::vector<std::string>> outputs2{
      {},
      {},
      {"ab"},
      {},
      {"bc"},
      {},
      {"bab", "ab"},
      {"d"},
      {"bc"},
      {"d"},
      {"abcde", "bcde"},
      {"d"},
      {"bcde"}
  };

  DynamicAcMachine<int> ac;
  int id = 0;
  for (auto& key : keys) ac.insert(key, id++);
  auto _ret = ac.all_output();
  std::vector<std::vector<std::string>> ret;
  for (auto& l : _ret) {
    ret.emplace_back();
    for (auto& [k,v] : l) ret.back().push_back(k);
  }
  auto v = compare_output(outputs, ret);
  if (compare_output(outputs, ret)) {
    assert(false);
    exit(EXIT_FAILURE);
  }

  ac.insert("bcde", id++);
  _ret = ac.all_output();
  ret = {};
  for (auto& l : _ret) {
    ret.emplace_back();
    for (auto& [k,v] : l) ret.back().push_back(k);
  }
  if (compare_output(outputs2, ret)) {
    assert(false);
    exit(EXIT_FAILURE);
  }

  ac.erase("bcde");
  _ret = ac.all_output();
  ret = {};
  for (auto& l : _ret) {
    ret.emplace_back();
    for (auto& [k,v] : l) ret.back().push_back(k);
  }
//  for (auto o : ret) {
//    std::cout<<"{ ";
//    for (auto k : o)
//      std::cout<<k<<' ';
//    std::cout<<"}"<<std::endl;
//  }
  if (compare_output(outputs, ret)) {
    assert(false);
    exit(EXIT_FAILURE);
  }

  std::cout << "OK" << std::endl;
}
