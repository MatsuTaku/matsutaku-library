#include "../../include/mtl/binary_trie.hpp"
#include <iostream>
#include <vector>

void test_constructor() {
    BinaryTrie<uint32_t, uint32_t> trie;
    assert(trie.empty());
    assert(trie.size() == 0);
}

void test_insert() {
    BinaryTrie<uint32_t, uint32_t> trie;
    auto result1 = trie.insert(std::make_pair(5, 10));
    auto result2 = trie.insert(std::make_pair(5, 20));
    assert(trie.size() == 1);
    assert(result1.second);
    assert(!result2.second);
    assert(result1.first->second == 10);
}

void test_find() {
    BinaryTrie<uint32_t, uint32_t> trie;
    trie.insert(std::make_pair(5, 10));
    trie.insert(std::make_pair(3, 20));

    auto result1 = trie.find(5);
    auto result2 = trie.find(3);
    auto result3 = trie.find(7);

    assert(result1 != trie.end());
    assert(result1->second == 10);
    assert(result2 != trie.end());
    assert(result2->second == 20);
    assert(result3 == trie.end());
}

void test_erase() {
    BinaryTrie<uint32_t, uint32_t> trie;
    trie.insert(std::make_pair(5, 10));
    trie.insert(std::make_pair(3, 20));

    bool result1 = trie.erase(5);
    bool result2 = trie.erase(7);

    assert(result1);
    assert(!result2);
    assert(trie.size() == 1);
    assert(trie.find(5) == trie.end());
    assert(trie.find(3) != trie.end());
}

void test_range_constructor() {
    std::vector<std::pair<uint32_t, uint32_t>> values = {
        {1, 10},
        {3, 30},
        {5, 50},
    };

    BinaryTrie<uint32_t, uint32_t> trie(values.begin(), values.end());
    assert(trie.size() == 3);
    assert(trie.find(1) != trie.end());
    assert(trie.find(3) != trie.end());
    assert(trie.find(5) != trie.end());
}

int main() {
    test_constructor();
    test_insert();
    test_find();
    test_erase();
    test_range_constructor();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
