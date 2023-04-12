#pragma once
#include "rrr.hpp"
#include <cstdint>
#include <vector>
#include <algorithm>
#include <limits>

template<class BitmapType = RRR<>>
class BinarySet {
public:
    using value_type = uint32_t;
private:
    using bitmap_type = BitmapType;
    using rs_type = typename RankSelectTraits<bitmap_type>::rank_select_type;
    bitmap_type bm_;
    rs_type rs_;
public:
    BinarySet() = default;
    BinarySet(const BinarySet& rhs) : bm_(rhs.bm_), rs_(rhs.rs_) {
        rs_.set_ptr(&bm_);
    }
    BinarySet& operator=(const BinarySet& rhs) {
        bm_ = rhs.bm_;
        rs_ = rhs.rs_;
        rs_.set_ptr(&bm_);
        return *this;
    }
    BinarySet(BinarySet&& rhs) : bm_(std::move(rhs.bm_)), rs_(std::move(rhs.rs_)) {
        rs_.set_ptr(&bm_);
    }
    BinarySet& operator=(BinarySet&& rhs) {
        bm_ = std::move(rhs.bm_);
        rs_ = std::move(rhs.rs_);
        rs_.set_ptr(&bm_);
        return *this;
    }
    ~BinarySet() = default;

    void insert(value_type x) {
        bm_.set(x, 1);
    }
    void build() {
        bm_.build();
        rs_.build(&bm_);
    }
    size_t size() const { return rs_.rank1(bm_.size()); }
    bool empty() const { return size() == 0;}
    bool contains(value_type x) const {
        return bm_.get(x);
    }
    value_type lower_bound(value_type x) const {
        if (contains(x)) return x;
        return select(rank(x));
    }
    size_t rank(value_type x) const {
        return rs_.rank1(x);
    }
    value_type select(size_t i) const {
        return rs_.select1(i);
    }
};

template<class BitmapType = RRR<>>
class BinaryMultiset {
public:
    using value_type = uint32_t;
    static constexpr value_type ValueMax = std::numeric_limits<value_type>::max();
private:
    using bitmap_type = BitmapType;
    using rs_type = typename RankSelectTraits<bitmap_type>::rank_select_type;
    bitmap_type bm_;
    rs_type rs_;
    std::vector<value_type> values_;
public:
    BinaryMultiset() = default;
    BinaryMultiset(const BinaryMultiset& rhs) : bm_(rhs.bm_), rs_(rhs.rs_), values_(rhs.values_) {
        rs_.set_ptr(&bm_);
    }
    BinaryMultiset& operator=(const BinaryMultiset& rhs) {
        bm_ = rhs.bm_;
        rs_ = rhs.rs_;
        values_ = rhs.values_;
        rs_.set_ptr(&bm_);
        return *this;
    }
    BinaryMultiset(BinaryMultiset&& rhs) noexcept 
        : bm_(std::move(rhs.bm_)), rs_(std::move(rhs.rs_)), values_(std::move(rhs.values_)) {
        rs_.set_ptr(&bm_);
    }
    BinaryMultiset& operator=(BinaryMultiset&& rhs) noexcept {
        bm_ = std::move(rhs.bm_);
        rs_ = std::move(rhs.rs_);
        values_ = std::move(rhs.values_);
        rs_.set_ptr(&bm_);
        return *this;
    }
    ~BinaryMultiset() = default;

    void insert(value_type x) {
        values_.push_back(x);
    }
    void build() {
        std::sort(values_.begin(), values_.end());
        for (size_t i = 0; i < values_.size(); i++)
            bm_.set((unsigned long long)values_[i] + i, 1);
        values_.clear();
        values_.shrink_to_fit();
        bm_.build();
        rs_.build(&bm_);
    }
    size_t size() const { return rs_.rank1(bm_.size()); }
    bool empty() const { return size() == 0;}
    size_t count(value_type x) const {
        auto s = rs_.select0(x);
        return s - (x == 0 ? 0 : (rs_.select0(x-1)+1));
    }
    bool contains(value_type x) const {
        auto s = rs_.select0(x);
        return s and bm_.get(s-1);
    }
    value_type lower_bound(value_type x) const {
        return select(rank(x));
    }
    size_t rank(value_type x) const {
        return x == 0 ? 0 : (rs_.select0(x-1) - (x-1));
    }
    value_type select(size_t i) const {
        return rs_.select1(i) - i;
    }
};