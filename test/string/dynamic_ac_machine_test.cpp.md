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
  bundledCode: "#line 1 \"test/string/dynamic_ac_machine_test.cpp\"\n#include <string/dynamic_ac_machine.hpp>\r\
    \n\r\n#include <iostream>\r\n#include <cassert>\r\n#include <vector>\r\n#include\
    \ <cstring>\r\n\r\nbool compare_output(std::vector<std::vector<std::string>>&\
    \ a, std::vector<std::vector<std::string>>& b) {\r\n  std::vector<std::string>\
    \ empty;\r\n  for (size_t i = 0; i < std::max(a.size(), b.size()); i++) {\r\n\
    \    std::vector<std::string>& l = ((i >= a.size()) ? empty : a[i]);\r\n    std::vector<std::string>&\
    \ r = ((i >= b.size()) ? empty : b[i]);\r\n    if (l != r) {\r\n      return true;\r\
    \n    }\r\n  }\r\n  return false;\r\n}\r\n\r\nint main() {\r\n  std::vector<std::string>\
    \ keys{\"ab\", \"bc\", \"bab\", \"d\", \"abcde\"};\r\n  std::vector<std::vector<std::string>>\
    \ outputs{\r\n      {},\r\n      {},\r\n      {\"ab\"},\r\n      {},\r\n     \
    \ {\"bc\"},\r\n      {},\r\n      {\"bab\", \"ab\"},\r\n      {\"d\"},\r\n   \
    \   {\"bc\"},\r\n      {\"d\"},\r\n      {\"abcde\"},\r\n  };\r\n  std::vector<std::vector<std::string>>\
    \ outputs2{\r\n      {},\r\n      {},\r\n      {\"ab\"},\r\n      {},\r\n    \
    \  {\"bc\"},\r\n      {},\r\n      {\"bab\", \"ab\"},\r\n      {\"d\"},\r\n  \
    \    {\"bc\"},\r\n      {\"d\"},\r\n      {\"abcde\", \"bcde\"},\r\n      {\"\
    d\"},\r\n      {\"bcde\"}\r\n  };\r\n\r\n  DynamicAcMachine<int> ac;\r\n  int\
    \ id = 0;\r\n  for (auto& key : keys) ac.insert(key, id++);\r\n  auto _ret = ac.all_output();\r\
    \n  std::vector<std::vector<std::string>> ret;\r\n  for (auto& l : _ret) {\r\n\
    \    ret.emplace_back();\r\n    for (auto& [k,v] : l) ret.back().push_back(k);\r\
    \n  }\r\n  auto v = compare_output(outputs, ret);\r\n  if (compare_output(outputs,\
    \ ret)) {\r\n    assert(false);\r\n    exit(EXIT_FAILURE);\r\n  }\r\n\r\n  ac.insert(\"\
    bcde\", id++);\r\n  _ret = ac.all_output();\r\n  ret = {};\r\n  for (auto& l :\
    \ _ret) {\r\n    ret.emplace_back();\r\n    for (auto& [k,v] : l) ret.back().push_back(k);\r\
    \n  }\r\n  if (compare_output(outputs2, ret)) {\r\n    assert(false);\r\n    exit(EXIT_FAILURE);\r\
    \n  }\r\n\r\n  ac.erase(\"bcde\");\r\n  _ret = ac.all_output();\r\n  ret = {};\r\
    \n  for (auto& l : _ret) {\r\n    ret.emplace_back();\r\n    for (auto& [k,v]\
    \ : l) ret.back().push_back(k);\r\n  }\r\n//  for (auto o : ret) {\r\n//    std::cout<<\"\
    { \";\r\n//    for (auto k : o)\r\n//      std::cout<<k<<' ';\r\n//    std::cout<<\"\
    }\"<<std::endl;\r\n//  }\r\n  if (compare_output(outputs, ret)) {\r\n    assert(false);\r\
    \n    exit(EXIT_FAILURE);\r\n  }\r\n\r\n  std::cout << \"OK\" << std::endl;\r\n\
    }\r\n"
  code: "#include <string/dynamic_ac_machine.hpp>\r\n\r\n#include <iostream>\r\n#include\
    \ <cassert>\r\n#include <vector>\r\n#include <cstring>\r\n\r\nbool compare_output(std::vector<std::vector<std::string>>&\
    \ a, std::vector<std::vector<std::string>>& b) {\r\n  std::vector<std::string>\
    \ empty;\r\n  for (size_t i = 0; i < std::max(a.size(), b.size()); i++) {\r\n\
    \    std::vector<std::string>& l = ((i >= a.size()) ? empty : a[i]);\r\n    std::vector<std::string>&\
    \ r = ((i >= b.size()) ? empty : b[i]);\r\n    if (l != r) {\r\n      return true;\r\
    \n    }\r\n  }\r\n  return false;\r\n}\r\n\r\nint main() {\r\n  std::vector<std::string>\
    \ keys{\"ab\", \"bc\", \"bab\", \"d\", \"abcde\"};\r\n  std::vector<std::vector<std::string>>\
    \ outputs{\r\n      {},\r\n      {},\r\n      {\"ab\"},\r\n      {},\r\n     \
    \ {\"bc\"},\r\n      {},\r\n      {\"bab\", \"ab\"},\r\n      {\"d\"},\r\n   \
    \   {\"bc\"},\r\n      {\"d\"},\r\n      {\"abcde\"},\r\n  };\r\n  std::vector<std::vector<std::string>>\
    \ outputs2{\r\n      {},\r\n      {},\r\n      {\"ab\"},\r\n      {},\r\n    \
    \  {\"bc\"},\r\n      {},\r\n      {\"bab\", \"ab\"},\r\n      {\"d\"},\r\n  \
    \    {\"bc\"},\r\n      {\"d\"},\r\n      {\"abcde\", \"bcde\"},\r\n      {\"\
    d\"},\r\n      {\"bcde\"}\r\n  };\r\n\r\n  DynamicAcMachine<int> ac;\r\n  int\
    \ id = 0;\r\n  for (auto& key : keys) ac.insert(key, id++);\r\n  auto _ret = ac.all_output();\r\
    \n  std::vector<std::vector<std::string>> ret;\r\n  for (auto& l : _ret) {\r\n\
    \    ret.emplace_back();\r\n    for (auto& [k,v] : l) ret.back().push_back(k);\r\
    \n  }\r\n  auto v = compare_output(outputs, ret);\r\n  if (compare_output(outputs,\
    \ ret)) {\r\n    assert(false);\r\n    exit(EXIT_FAILURE);\r\n  }\r\n\r\n  ac.insert(\"\
    bcde\", id++);\r\n  _ret = ac.all_output();\r\n  ret = {};\r\n  for (auto& l :\
    \ _ret) {\r\n    ret.emplace_back();\r\n    for (auto& [k,v] : l) ret.back().push_back(k);\r\
    \n  }\r\n  if (compare_output(outputs2, ret)) {\r\n    assert(false);\r\n    exit(EXIT_FAILURE);\r\
    \n  }\r\n\r\n  ac.erase(\"bcde\");\r\n  _ret = ac.all_output();\r\n  ret = {};\r\
    \n  for (auto& l : _ret) {\r\n    ret.emplace_back();\r\n    for (auto& [k,v]\
    \ : l) ret.back().push_back(k);\r\n  }\r\n//  for (auto o : ret) {\r\n//    std::cout<<\"\
    { \";\r\n//    for (auto k : o)\r\n//      std::cout<<k<<' ';\r\n//    std::cout<<\"\
    }\"<<std::endl;\r\n//  }\r\n  if (compare_output(outputs, ret)) {\r\n    assert(false);\r\
    \n    exit(EXIT_FAILURE);\r\n  }\r\n\r\n  std::cout << \"OK\" << std::endl;\r\n\
    }\r\n"
  dependsOn: []
  isVerificationFile: false
  path: test/string/dynamic_ac_machine_test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: test/string/dynamic_ac_machine_test.cpp
layout: document
redirect_from:
- /library/test/string/dynamic_ac_machine_test.cpp
- /library/test/string/dynamic_ac_machine_test.cpp.html
title: test/string/dynamic_ac_machine_test.cpp
---
