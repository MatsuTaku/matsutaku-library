#pragma once
#include <cstdint>
#include <vector>
#include <cstddef>
#include <cassert>

/**
 * @brief Integer Vector store ElementSize bits per element
 * @param ElementSize number of bits per element
*/
template<unsigned ElementSize>
struct IntVector {
    using value_type = uint64_t;
    using element_type = uint64_t;
    static constexpr unsigned element_size = ElementSize;
    static constexpr unsigned word_size = 64;
    std::vector<element_type> arr;
    size_t size_;
    IntVector(size_t n = 0) : size_(n), arr(needs_size(n)) {}
    size_t needs_size(size_t needs) const {
        return (needs * element_size + word_size - 1) / word_size;
    }
    size_t container_size() const {
        return arr.size() * word_size / element_size;
    }
    size_t size() const { return size_; }
    bool empty() const { return size() == 0; }
    void resize(size_t new_size) {
        size_ = new_size;
        arr.resize(needs_size(new_size));
    }
    void clear() { resize(0); }
    void set(size_t i, const value_type& v) {
        size_t j = i * element_size;
        size_t k = j / word_size;
        size_t l = j % word_size;
        arr[k] = (arr[k] & ~(value_type(-1) << l)) | (v << l);
        if (l + element_size > word_size) {
            arr[k+1] = (arr[k+1] & (value_type(-1) << (l + element_size - word_size))) 
                | (v >> (word_size - l));
        }
    }
    void push_back(const value_type& v) {
        auto i = size();
        resize(size()+1);
        set(i, v);
    }
    value_type get(size_t i) const {
        size_t j = i * element_size;
        size_t k = j / word_size;
        size_t l = j % word_size;
        value_type res = arr[k] >> l;
        if (l + element_size > word_size) {
            res |= (arr[k+1] & ~(value_type(-1) << (l + element_size - word_size))) << (word_size - l);
        }
        return res;
    }
    value_type front() const { return get(0); }
    value_type back() const { return get(size()-1); }
};