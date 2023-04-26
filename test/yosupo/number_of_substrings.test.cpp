#define PROBLEM "https://judge.yosupo.jp/problem/number_of_substrings"
#include "../../include/mtl/string/suffix_array.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    string s; cin>>s;
    auto sa = SuffixArray(s.begin(), s.end());
    auto lcp = LCPArray(s.begin(), s.end(), sa);
    long long ans = 0;
    for (int i = 0; i < (int)s.size(); i++)
        ans += s.size() - sa[i] - lcp[i];
    cout << ans << endl;
}