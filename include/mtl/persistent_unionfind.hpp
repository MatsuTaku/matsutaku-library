#pragma once
#include "persistent_array.hpp"
#include <cassert>

class PersistentUnionfind {
private:
    PersistentArray<long long> par_;
public:
    PersistentUnionfind() : par_(-1) {}
    size_t leader(size_t u) {
        auto pu = par_.get(u);
        if (pu < 0)
            return u;
        auto ret = leader(pu);
        par_ = par_.set(u, ret);
        return ret;
    }
    size_t size_of(size_t u) {
        return -par_.get(leader(u));
    }
    bool same(size_t a, size_t b) {
        return leader(a) == leader(b);
    }
    [[nodiscard]] PersistentUnionfind merge(size_t a, size_t b) {
        auto ra = leader(a);
        auto rb = leader(b);
        if (ra == rb)
            return *this;
        if (size_of(ra) < size_of(rb))
            std::swap(ra, rb);
        PersistentUnionfind ret = *this;
        ret.par_ = ret.par_.set(rb, ra);
        ret.par_ = ret.par_.set(ra, -(long long)(size_of(ra) + size_of(rb)));
        return ret;
    }
};