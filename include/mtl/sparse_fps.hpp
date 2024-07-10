#pragma once
#include "modular.hpp"
#include "fps.hpp"
#include <cstddef>
#include <vector>
#include <utility>
#include <initializer_list>
#include <algorithm>

template<int Mod = 998244353>
class SparseFps : public std::vector<std::pair<size_t, Modular<Mod>>> {
    using _base = std::vector<std::pair<size_t, Modular<Mod>>>;
    public:
    using mint = Modular<Mod>;
    using value_type = typename _base::value_type;
    using fps_type = Fps<Mod>;

    SparseFps() = default;
    template<class InputIt>
    SparseFps(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it) {
            if (it->second != mint(0)) {
                this->emplace_back(*it);
            }
        }
    }
    SparseFps(std::initializer_list<value_type> init) : SparseFps(init.begin(), init.end()) {}
    SparseFps(std::initializer_list<mint> init) {
        for (auto it = init.begin(); it != init.end(); ++it) {
            if (*it != mint(0)) {
                auto k = it - init.begin();
                this->emplace_back(k, *it);
            }
        }
    }
    SparseFps(const fps_type& rhs, size_t n = Mod) {
        n = std::min(n, rhs.size());
        for (size_t i = 0; i < n; ++i) {
            if (rhs[i] != mint(0)) {
                this->emplace_back(i, rhs[i]);
            }
        }
    }
    size_t deg() const {
        return this->empty() ? 0 : this->back().first+1;
    }
    SparseFps& inline_pre(int n) {
        for (size_t i = 0; i < this->size(); i++) {
            if ((*this)[i].first >= n) {
                this->resize(i);
                break;
            }
        }
    }
    SparseFps pre(int n) const {
        auto it = std::lower_bound(this->begin(), this->end(), std::make_pair(n, mint(0)));
        return SparseFps(this->begin(), it);
    }
    template<class Value>
    void push(Value&& x) {
        if (x.second == mint(0)) return;
        this->emplace_back(std::forward<Value>(x));
    }
    void emplace(size_t deg, mint val) {
        push(std::make_pair(deg, val));
    }
    fps_type to_fps() const {
        if (this->empty()) return fps_type();
        fps_type res(deg(), 0);
        for (auto& x : *this) {
            res[x.first] = x.second;
        }
        return res;
    }
    operator fps_type() const {
        return to_fps();
    }
    bool operator==(const SparseFps& rhs) const {
        auto it1 = this->begin();
        auto it2 = rhs.begin();
        while (it1 != this->end() and it2 != rhs.end()) {
            while (it1 != this->end() and it1->second == mint(0)) ++it1;
            while (it2 != rhs.end() and it2->second == mint(0)) ++it2;
            if (it1 == this->end() or it2 == rhs.end()) break;
            if (*it1 != *it2) return false;
            ++it1; ++it2;
        }
        return it1 == this->end() and it2 == rhs.end();
    }

    SparseFps operator+(const SparseFps& rhs) const {
        // degrees must be sorted
        SparseFps res;
        auto it1 = this->begin(), it2 = rhs.begin();
        while (it1 != this->end() && it2 != rhs.end()) {
            if (it1->first == it2->first) {
                res.push_back(*it1 + *it2);
                ++it1; ++it2;
            } else if (it1->first < it2->first) {
                res.push_back(*it1);
                ++it1;
            } else {
                res.push_back(*it2);
                ++it2;
            }
        }
        while (it1 != this->end()) {
            res.push_back(*it1);
            ++it1;
        }
        while (it2 != rhs.end()) {
            res.push_back(*it2);
            ++it2;
        }
        return res;
    }
    SparseFps operator-(const SparseFps& rhs) const {
        // degrees must be sorted
        SparseFps res;
        auto it1 = this->begin(), it2 = rhs.begin();
        while (it1 != this->end() && it2 != rhs.end()) {
            if (it1->first == it2->first) {
                res.push_back(*it1 - *it2);
                ++it1; ++it2;
            } else if (it1->first < it2->first) {
                res.push_back(*it1);
                ++it1;
            } else {
                res.push_back(-*it2);
                ++it2;
            }
        }
        while (it1 != this->end()) {
            res.push_back(*it1);
            ++it1;
        }
        while (it2 != rhs.end()) {
            res.push_back(-*it2);
            ++it2;
        }
        return res;
    }
    SparseFps operator-() const {
        SparseFps res;
        for (auto& x : *this) {
            res.push_back(x.first, -x.second);
        }
        return res;
    }
    SparseFps operator*(const SparseFps& rhs) const {
        if (this->empty() or rhs.empty()) return SparseFps();
        SparseFps res;
        for (auto& x : *this) {
            for (auto& y : rhs) {
                res.push_back(x.first + y.first, x.second * y.second);
            }
        }
        return res;
    }

    fps_type operator*(const fps_type& rhs) const {
        if (this->empty() or rhs.empty()) return fps_type{};
        fps_type res(deg() - 1 + rhs.size(), 0);
        for (auto& x : *this) {
            for (size_t i = 0; i < rhs.size(); ++i) {
                res[i+x.first] += x.second * rhs[i];
            }
        }
        return res;
    }
    friend fps_type operator*(const fps_type& lhs, const SparseFps& rhs) {
        return rhs * lhs;
    }
    fps_type inv(int n) const {
        assert((!this->empty() and this->front().first == 0 and this->front().second != mint(0)));
        if (n == 0) return fps_type();
        // fg = 1 => (n>0) sum_i f_i g_{n-i} = 0
        // f_0 g_n = - sum_{i=1}^n f_i g_{n-i}
        fps_type g(n);
        auto ifz = this->front().second.inv();
        g[0] = ifz;
        for (size_t i = 1; i < (size_t) n; i++) {
            mint s = 0;
            for (size_t j = 1; j < this->size(); j++) {
                auto& x = this->operator[](j);
                if (x.first > i) break;
                s += x.second * g[i-x.first];
            }
            g[i] = -s * ifz;
        }
        // assert((pre(n)*g).pre(n) == Fps{1});
        return g;
    }
    friend fps_type operator/(const fps_type& lhs, const SparseFps& rhs) {
        if (lhs == fps_type()) return fps_type(); 
        assert(!rhs.empty() and rhs[0].first == 0 and rhs[0].second != mint(0));
        if (rhs[0].second == mint(1)) {
            return div_one(lhs, rhs);
        } else {
            auto icoeff = rhs[0].second.inv();
            return div_one(lhs, rhs * icoeff) * icoeff;
        }
    }
    private:
    friend fps_type div_one(const fps_type& lhs, const SparseFps& rhs) {
        assert(lhs != fps_type());
        assert(!rhs.empty() and rhs[0].second == mint(1));
        fps_type ret(lhs.size() + rhs.deg() - 1);
        std::copy(lhs.begin(), lhs.end(), ret.begin());
        for (size_t i = 1; i < ret.size(); i++) {
            for (size_t j = 1; j < rhs.size(); j++) {
                auto& x = rhs[j];
                if (i < x.first) break;
                ret[i] += ret[i-x.first] * -x.second;
            }
        }
        return ret;
    }
    public:
    SparseFps diff(int n = Mod) const {
        SparseFps res;
        for (auto& x : *this) {
            if (x.first == 0) continue;
            if (x.first >= n) break;
            res.push_back(x.first - 1, x.second * x.first);
        }
        return res;
    }
    SparseFps& inline_diff() {
        for (auto& x : *this) {
            x.second *= x.first;
            --x.first;
        }
        return *this;
    }
    SparseFps inte(int n = Mod) const {
        SparseFps res;
        for (auto& x : *this) {
            if (x.first + 1 >= n) break;
            res.push_back(x.first + 1, x.second / (x.first + 1));
        }
        return res;
    }
    SparseFps inline_inte() const {
        for (auto& x : *this) {
            x.second /= x.first + 1;
            ++x.first;
        }
        return *this;
    }
    fps_type exp(int deg) const;
    fps_type log(int n = Mod) const {
        assert(!_base::empty() and _base::operator[](0) == 1);
        // integral(f' / f)
        return (diff(n).to_fps().inline_pre(n-1) / pre(n-1)).inline_pre(n-1).inline_inte().inline_pre(n);
    }
    fps_type pow(long long x, int n) const;

    SparseFps operator+(mint x) const {
        if (!this->empty() and this->front().first == 0) {
            auto res = *this;
            res[0].second += x;
            return res;
        } else {
            SparseFps res{x};
            res.insert(res.end(), this->begin(), this->end());
            return res;
        }
    }
    SparseFps operator-(mint x) const {
        return *this + (-x);
    }
    SparseFps operator*(mint x) const {
        assert(x != mint(0));
        auto res = *this;
        for (auto& y : res) {
            y.second *= x;
        }
        return res;
    }
    SparseFps operator/(mint x) const {
        assert(x != mint(0));
        return *this * x.inv();
    }
    SparseFps& operator*=(mint x) {
        assert(x != mint(0));
        for (auto& y : *this) {
            y.second *= x;
        }
        return *this;
    }
    SparseFps& operator/=(mint x) {
        assert(x != mint(0));
        return *this *= x.inv();
    }
};