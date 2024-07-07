---
data:
  _extendedDependsOn:
  - icon: ':x:'
    path: include/mtl/persistent_queue.hpp
    title: Persistent Queue
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/persistent_queue
    links:
    - https://judge.yosupo.jp/problem/persistent_queue
  bundledCode: "#line 1 \"test/yosupo/persistent_queue.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.yosupo.jp/problem/persistent_queue\"\n#include <iostream> \n\
    #line 2 \"include/mtl/persistent_queue.hpp\"\n#include <memory>\n#include <cstdint>\n\
    #include <algorithm>\n#include <cassert>\n\n/**\n * @brief Persistent Queue\n\
    */\ntemplate<class T>\nclass PersistentQueue {\npublic:\n    using value_type\
    \ = T;\nprivate:\n    struct Node;\n    struct Stream;\n    using node_ptr = std::shared_ptr<Node>;\n\
    \    using raw_ptr = Node*;\n    using stream_ptr = std::shared_ptr<Stream>;\n\
    \    template<class... Args>\n    static node_ptr make_node(Args&&... args) {\n\
    \        return std::make_shared<Node>(std::forward<Args>(args)...);\n    }\n\
    \    template<class... Args>\n    static stream_ptr make_stream(Args&&... args)\
    \ {\n        return std::make_shared<Stream>(std::forward<Args>(args)...);\n \
    \   }\n    struct Node {\n        node_ptr next;\n        value_type v;\n    \
    \    template<class N, class... Args>\n        Node(N&& n, Args&&... args) \n\
    \            : next(std::forward<N>(n)), v(std::forward<Args>(args)...) {}\n \
    \   };\n    struct Stream {\n        node_ptr scan, rotate;\n        template<class\
    \ Rptr>\n        Stream(const node_ptr& nptr, Rptr&& rptr) \n            : scan(nptr),\
    \ rotate(std::forward<Rptr>(rptr)) {}\n        \n        node_ptr next() {\n \
    \           node_ptr ret;\n            if (scan) {\n                ret = make_node(nullptr,\
    \ scan->v);\n                scan = scan->next;\n            } else {\n      \
    \          while (rotate) {\n                    ret = make_node(ret, rotate->v);\n\
    \                    rotate = rotate->next;\n                }\n            }\n\
    \            return ret;\n        }\n    };\n\n    node_ptr f_;\n    raw_ptr proc_;\n\
    \    stream_ptr stream_;\n    size_t size_;\n    node_ptr r_;\n\n    PersistentQueue(stream_ptr&&\
    \ stream, size_t size) : \n        f_(stream->next()), \n        proc_(f_.get()),\
    \ \n        stream_(std::move(stream)), \n        size_(size),\n        r_(nullptr)\
    \ {}\n    template<class Nptr>\n    PersistentQueue(const node_ptr& f, const raw_ptr&&\
    \ proc, const stream_ptr& stream, size_t size, \n                    Nptr&& r)\
    \ \n        : f_(f), proc_(proc), stream_(stream), size_(size), r_(std::forward<Nptr>(r))\
    \ {}\n\n    template<class... Args>\n    [[nodiscard]] PersistentQueue _push(Args&&...\
    \ args) const {\n        if (!proc_) {\n            return PersistentQueue(\n\
    \                make_stream(f_, make_node(r_, std::forward<Args>(args)...)),\
    \ size()+1);\n        }\n        if (!proc_->next) {\n            proc_->next\
    \ = stream_->next();\n        }\n        return PersistentQueue(f_, proc_->next.get(),\
    \ stream_, size()+1,\n                               make_node(r_, std::forward<Args>(args)...));\n\
    \    }\n    [[nodiscard]] PersistentQueue _pop() const {\n        assert(!empty());\n\
    \        if (!proc_) {\n            return PersistentQueue(make_stream(f_->next,\
    \ r_), size()-1);\n        }\n        if (!proc_->next) {\n            proc_->next\
    \ = stream_->next();\n        }\n        return PersistentQueue(f_->next, proc_->next.get(),\
    \ stream_, size()-1, r_);\n    }\n\npublic:\n    PersistentQueue() \n        :\
    \ f_(nullptr), proc_(nullptr), stream_(nullptr), size_(0), r_(nullptr) {}\n  \
    \  size_t size() const { return size_; }\n    bool empty() const { return size()\
    \ == 0; }\n    const value_type& front() const {\n        assert(f_);\n      \
    \  return f_->v;\n    }\n    template<class V>\n    [[nodiscard]] PersistentQueue\
    \ push(V&& x) const {\n        return _push(std::forward<V>(x));\n    }\n    [[nodiscard]]\
    \ PersistentQueue push(const T& x) const {\n        return _push(x);\n    }\n\
    \    [[nodiscard]] PersistentQueue push(T&& x) const {\n        return _push(std::move(x));\n\
    \    }\n    template<class... Args>\n    [[nodiscard]] PersistentQueue emplace(Args&&...\
    \ args) const {\n        return _push(std::forward<Args>(args)...);\n    }\n \
    \   [[nodiscard]] PersistentQueue pop() const {\n        return _pop();\n    }\n\
    };\n#line 4 \"test/yosupo/persistent_queue.test.cpp\"\n#include <bits/stdc++.h>\n\
    using namespace std;\n\nint main() {\n    int q; cin>>q;\n    vector<PersistentQueue<int>>\
    \ S(q+1);\n    for (int i = 0; i < q; i++) {\n        int c; cin>>c;\n       \
    \ if (c == 0) {\n            int t,x; cin>>t>>x; t++;\n            S[i+1] = S[t].push(x);\n\
    \        } else {\n            int t; cin>>t; t++;\n            auto v = S[t].front();\n\
    \            S[i+1] = S[t].pop();\n            cout << v << endl;\n        }\n\
    \    }\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/persistent_queue\"\n#include\
    \ <iostream> \n#include \"../../include/mtl/persistent_queue.hpp\"\n#include <bits/stdc++.h>\n\
    using namespace std;\n\nint main() {\n    int q; cin>>q;\n    vector<PersistentQueue<int>>\
    \ S(q+1);\n    for (int i = 0; i < q; i++) {\n        int c; cin>>c;\n       \
    \ if (c == 0) {\n            int t,x; cin>>t>>x; t++;\n            S[i+1] = S[t].push(x);\n\
    \        } else {\n            int t; cin>>t; t++;\n            auto v = S[t].front();\n\
    \            S[i+1] = S[t].pop();\n            cout << v << endl;\n        }\n\
    \    }\n}"
  dependsOn:
  - include/mtl/persistent_queue.hpp
  isVerificationFile: true
  path: test/yosupo/persistent_queue.test.cpp
  requiredBy: []
  timestamp: '2024-07-08 03:47:08+09:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: test/yosupo/persistent_queue.test.cpp
layout: document
redirect_from:
- /verify/test/yosupo/persistent_queue.test.cpp
- /verify/test/yosupo/persistent_queue.test.cpp.html
title: test/yosupo/persistent_queue.test.cpp
---
