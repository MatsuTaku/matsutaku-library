#define STANDALONE
#include "../../include/mtl/string/ac_machine.hpp"

#include <iostream>
#include <cassert>
#include <vector>
#include <cstring>

void test(const std::vector<std::string>& keys, const std::vector<std::vector<std::string>>& outputs) {
  AcMachine<int> ac;
  int id = 0;
  for (auto& key : keys)
    ac.insert(key, id++);
  ac.build();
  auto ret = ac.all_output();
//  for (auto o : ret) {
//    std::cout<<"{ ";
//    for (auto k : o)
//      std::cout<<k<<' ';
//    std::cout<<"}"<<std::endl;
//  }

  bool check = ret.size() == outputs.size();
  if (check) {
    for (int i = 0; i < ret.size(); i++) {
      if (check &= ret[i].size() == outputs[i].size()) {
        for (int j = 0; j < ret[i].size(); j++) {
          check &= ret[i][j].first == outputs[i][j];
        }
      }
    }
  }
  if (!check) {
    exit(EXIT_FAILURE);
  }
}

int main() {
  test({"ab", "bc", "bab", "d", "abcde"}, {
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
      {"abcde"}
  });
  std::cout << "OK" << std::endl;
}
