---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 1 \"test/string/ac_machine_test.cpp\"\n#include <string/ac_machine.hpp>\r\
    \n\r\n#include <iostream>\r\n#include <cassert>\r\n#include <vector>\r\n#include\
    \ <cstring>\r\n\r\nvoid test(const std::vector<std::string>& keys, const std::vector<std::vector<std::string>>&\
    \ outputs) {\r\n  AcMachine<int> ac;\r\n  int id = 0;\r\n  for (auto& key : keys)\r\
    \n    ac.insert(key, id++);\r\n  ac.build();\r\n  auto ret = ac.all_output();\r\
    \n//  for (auto o : ret) {\r\n//    std::cout<<\"{ \";\r\n//    for (auto k :\
    \ o)\r\n//      std::cout<<k<<' ';\r\n//    std::cout<<\"}\"<<std::endl;\r\n//\
    \  }\r\n\r\n  bool check = ret.size() == outputs.size();\r\n  if (check) {\r\n\
    \    for (int i = 0; i < ret.size(); i++) {\r\n      if (check &= ret[i].size()\
    \ == outputs[i].size()) {\r\n        for (int j = 0; j < ret[i].size(); j++) {\r\
    \n          check &= ret[i][j].first == outputs[i][j];\r\n        }\r\n      }\r\
    \n    }\r\n  }\r\n  if (!check) {\r\n    exit(EXIT_FAILURE);\r\n  }\r\n}\r\n\r\
    \nint main() {\r\n  test({\"ab\", \"bc\", \"bab\", \"d\", \"abcde\"}, {\r\n  \
    \    {},\r\n      {},\r\n      {\"ab\"},\r\n      {},\r\n      {\"bc\"},\r\n \
    \     {},\r\n      {\"bab\", \"ab\"},\r\n      {\"d\"},\r\n      {\"bc\"},\r\n\
    \      {\"d\"},\r\n      {\"abcde\"}\r\n  });\r\n  std::cout << \"OK\" << std::endl;\r\
    \n}\r\n"
  code: "#include <string/ac_machine.hpp>\r\n\r\n#include <iostream>\r\n#include <cassert>\r\
    \n#include <vector>\r\n#include <cstring>\r\n\r\nvoid test(const std::vector<std::string>&\
    \ keys, const std::vector<std::vector<std::string>>& outputs) {\r\n  AcMachine<int>\
    \ ac;\r\n  int id = 0;\r\n  for (auto& key : keys)\r\n    ac.insert(key, id++);\r\
    \n  ac.build();\r\n  auto ret = ac.all_output();\r\n//  for (auto o : ret) {\r\
    \n//    std::cout<<\"{ \";\r\n//    for (auto k : o)\r\n//      std::cout<<k<<'\
    \ ';\r\n//    std::cout<<\"}\"<<std::endl;\r\n//  }\r\n\r\n  bool check = ret.size()\
    \ == outputs.size();\r\n  if (check) {\r\n    for (int i = 0; i < ret.size();\
    \ i++) {\r\n      if (check &= ret[i].size() == outputs[i].size()) {\r\n     \
    \   for (int j = 0; j < ret[i].size(); j++) {\r\n          check &= ret[i][j].first\
    \ == outputs[i][j];\r\n        }\r\n      }\r\n    }\r\n  }\r\n  if (!check) {\r\
    \n    exit(EXIT_FAILURE);\r\n  }\r\n}\r\n\r\nint main() {\r\n  test({\"ab\", \"\
    bc\", \"bab\", \"d\", \"abcde\"}, {\r\n      {},\r\n      {},\r\n      {\"ab\"\
    },\r\n      {},\r\n      {\"bc\"},\r\n      {},\r\n      {\"bab\", \"ab\"},\r\n\
    \      {\"d\"},\r\n      {\"bc\"},\r\n      {\"d\"},\r\n      {\"abcde\"}\r\n\
    \  });\r\n  std::cout << \"OK\" << std::endl;\r\n}\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/string/ac_machine_test.cpp
  requiredBy: []
  timestamp: '2022-11-27 14:51:20+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/string/ac_machine_test.cpp
layout: document
redirect_from:
- /library/test/string/ac_machine_test.cpp
- /library/test/string/ac_machine_test.cpp.html
title: test/string/ac_machine_test.cpp
---
