---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes: {}
  bundledCode: "Traceback (most recent call last):\n  File \"/opt/hostedtoolcache/Python/3.11.2/x64/lib/python3.11/site-packages/onlinejudge_verify/documentation/build.py\"\
    , line 71, in _render_source_code_stat\n    bundled_code = language.bundle(stat.path,\
    \ basedir=basedir, options={'include_paths': [basedir]}).decode()\n          \
    \         ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\
    \  File \"/opt/hostedtoolcache/Python/3.11.2/x64/lib/python3.11/site-packages/onlinejudge_verify/languages/cplusplus.py\"\
    , line 187, in bundle\n    bundler.update(path)\n  File \"/opt/hostedtoolcache/Python/3.11.2/x64/lib/python3.11/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py\"\
    , line 401, in update\n    self.update(self._resolve(pathlib.Path(included), included_from=path))\n\
    \                ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n \
    \ File \"/opt/hostedtoolcache/Python/3.11.2/x64/lib/python3.11/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py\"\
    , line 260, in _resolve\n    raise BundleErrorAt(path, -1, \"no such header\"\
    )\nonlinejudge_verify.languages.cplusplus_bundle.BundleErrorAt: ../include/mtl/dual_sparse_table.hpp:\
    \ line -1: no such header\n"
  code: "#define PROBLEM \"https://yukicoder.me/problems/no/945\"\n#include \"../include/mtl/dual_sparse_table.hpp\"\
    \n#include <bits/stdc++.h>\nusing namespace std;\n\nusing P = pair<int,int>;\n\
    P f(P a, P b) {return min(a,b);}\nconstexpr int INF = 1e9;\nP e() {return {INF,\
    \ 3};}\nusing DST = DualSparseTable<P, f, e>;\n\nint main() {\n    int n,m; cin>>n>>m;\n\
    \    map<char,int> mp;\n    mp['Y']=0;\n    mp['K']=1;\n    mp['C']=2;\n    DST\
    \ dst(n);\n    for (int i = 0; i < m; i++) {\n        int l,r;\n        char t;\n\
    \        cin>>l>>r>>t;\n        l--;\n        dst.apply(l, r, {i, mp[t]});\n \
    \   } \n    dst.build();\n    array<int,4> ans{};\n    for (int i = 0; i < n;\
    \ i++) {\n        auto [id,t] = dst.get(i);\n        ans[t]++;\n    }\n    cout\
    \ << ans[0] << ' ' << ans[1] << ' ' << ans[2] << endl;\n}"
  dependsOn: []
  isVerificationFile: true
  path: test/yuki/yuki-no945_ykc_manju.test.cpp
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: test/yuki/yuki-no945_ykc_manju.test.cpp
layout: document
redirect_from:
- /verify/test/yuki/yuki-no945_ykc_manju.test.cpp
- /verify/test/yuki/yuki-no945_ykc_manju.test.cpp.html
title: test/yuki/yuki-no945_ykc_manju.test.cpp
---
