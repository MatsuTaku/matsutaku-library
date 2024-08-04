#define STANDALONE
#include "include/mtl/succinct/ty.hpp"
#include <bits/stdc++.h>

using namespace std;
constexpr int n = 1e6;

int main() {
    vector<int> A(n);
    constexpr int max_diff = (1<<10)-1;
    A[0] = rand()%max_diff;
    for (int i = 1; i < n; i++) {
        A[i] = A[i-1] + rand()%max_diff;
    }
    TY<int> ty(A.begin(), A.end());
    for (int i = 0; i < n; i++) {
        auto v = ty[i];
        if (v != A[i]) {
            std::cout << "Failed get: " << i << " ty.get " << v << " != A " << A[i] << std::endl;
            return 1;
        }
    }
    std::cout << "OK" << std::endl;

}