#pragma once
#include <vector>
#include <cstdint>
#include <limits>
#include <cstddef>
#include <algorithm>
#include <cassert>

/**
 * @brief TY: Store increasing sequence of integers.
 *            Memory needs for store nth integers O(n log d) bits 
 *            which d is max diff of consecutive elements.
*/
template<class T, class DiffType = int16_t>
class TY {
    using value_type = T;
    static constexpr auto block_size = sizeof(value_type) * 8;
    using diff_value_type = DiffType;
    static constexpr unsigned max_diff = std::numeric_limits<diff_value_type>::max();
private:
    std::vector<value_type> head;
    std::vector<diff_value_type> diff;

public:
    TY() = default;
    template<class It>
    TY(It first, It last) {
        assert(std::is_sorted(first, last));
        reserve(std::distance(first, last));
        for (auto it = first; it != last; it++) {
            push_back(*it);
        }
    }
    size_t size() const {
        return head.size() + diff.size();
    }
    bool empty() const { return size() == 0; }
    void reserve(size_t n) {
        head.reserve((n + block_size - 1) / block_size);
        diff.reserve(n / block_size * (block_size - 1) + n % block_size);
    }
    template<class... Args>
    void emplace_back(Args&&... args) {
        if (size() % block_size == 0) {
            head.emplace_back(std::forward<Args>(args)...);
        } else {
            value_type v(std::forward<Args>(args)...);
            assert(v >= head.back());
            assert(v - head.back() <= (value_type)max_diff);
            diff.push_back((diff_value_type)(v - head.back()));
        }
    }
    void push_back(const value_type& v) {
        if (size() % block_size == 0) {
            head.push_back(v);
        } else {
            assert(v >= head.back());
            assert(v - head.back() <= (value_type)max_diff);
            diff.push_back(v - head.back());
        }
    }
    void push_back(value_type&& v) {
        emplace_back(std::move(v));
    }
    value_type get(size_t i) const {
        if (i % block_size == 0) 
            return head[i / block_size];
        else 
            return head[i / block_size] + 
                   (value_type)diff[i / block_size * (block_size-1) + i % block_size - 1];
    }
    value_type operator[](size_t i) const { return get(i); }
    value_type front() const { return get(0); }
    value_type back() const { return get(size()-1); }
};