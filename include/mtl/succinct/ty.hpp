#pragma once
#include <vector>
#include <cstdint>
#include <limits>
#include <cstddef>
#include <cassert>

/**
 * @brief TY: Store increasing sequence of integers.
 *            Memory needs for store nth integers O(n log d) bits 
 *            which d is max diff of consecutive elements.
*/
template<class T>
struct TY {
    using value_type = T;
    static constexpr auto block_size = sizeof(value_type) * 8;
    using diff_value_type = uint16_t;
    static constexpr unsigned max_diff = std::numeric_limits<diff_value_type>::max();
    using head_type = std::vector<value_type>;
    using diff_type = std::vector<diff_value_type>;
    head_type head;
    diff_type diff;

    TY() = default;
    size_t size() const {
        return head.size() + diff.size();
    }
    bool empty() const { return size() == 0; }
    value_type raw_element(const value_type& v) {
        return v;
    }
    value_type diff_element(const value_type& v) {
        return v;
    }
    void push_back(const value_type& v) {
        assert(head.empty() or v-head.back() <= (value_type)max_diff);
        if (size()% block_size == 0) {
            head.push_back(raw_element(v));
        } else {
            diff.push_back(diff_element(v - head.back()));
        }
    }
    value_type get(size_t i) const {
        return (i % block_size == 0) ? 
            head[i / block_size] : 
            head[i / block_size] + diff[i / block_size * (block_size-1) + i % block_size - 1];
    }
    value_type operator[](size_t i) const { return get(i); }
    value_type front() const { return get(0); }
    value_type back() const { return get(size()-1); }
};