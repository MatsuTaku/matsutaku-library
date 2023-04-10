#include <succinct/ty.hpp>
#include <iostream>

using namespace std;
constexpr int n = 1e7;

int main() {
    vector<int> A(n);
    constexpr int max_diff = 1<<10-1;
    A[0] = rand()%max_diff;
    for (int i = 1; i < n; i++) {
        A[i] = A[i-1] + rand()%max_diff;
    }
    TY<int> ty;
    for (auto a:A) ty.push_back(a);
    for (int i = 0; i < n; i++) {
        auto v = ty[i];
        if (v != A[i]) {
            std::cout << "Failed get: " << i << " ty.get " << v << " != A " << A[i] << std::endl;
            return 1;
        }
    }
    std::cout << "OK" << std::endl;

}