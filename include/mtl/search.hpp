#pragma once
#include <utility>
#include <type_traits>
#include <functional>
#include <cmath>
#include <vector>
#include <cassert>

constexpr double EPS_DEFAULT = 1e-9;

template<typename I, typename F>
typename std::remove_reference<I>::type
bisect_int(I ok, I ng, F f) {
  while (std::abs(ng - ok) > 1) {
    auto c = ok + (ng - ok) / 2;
    if (f(c))
      ok = c;
    else
      ng = c;
  }
  return ok;
}

template<typename F>
double
bisect_float(double ok, double ng, F fn, double eps = EPS_DEFAULT) {
  while (std::abs(ok - ng) > eps) {
    double c = ok + (ng - ok) / 2;
    if (fn(c))
      ok = c;
    else
      ng = c;
  }
  return ok;
}

template<class Cmp = std::less<>>
class FibonacciSearch {
 public:
  using idx_type = long long;
 private:
  std::vector<long long> fib_;
  Cmp cmp_;
  void setup_fib(long long max_distance) {
    while (fib_.back() < max_distance) fib_.push_back(fib_[fib_.size()-2] + fib_[fib_.size()-1]);
  }
 public:
  FibonacciSearch(long long max_distance = 1, const Cmp& cmp = Cmp()) : fib_{1,1}, cmp_(cmp) {
    setup_fib(max_distance);
  }

  template<class F>
  auto operator()(idx_type l, idx_type r, F fn) const 
    -> std::pair<idx_type, decltype(fn(l))> 
  {
    assert(r - l >= 2);
    idx_type d = r-l;
    setup_fib(d);
    auto k = --fib_.cend();
    idx_type il = l + *(k-2);
    idx_type ir = l + *(k-1);
    auto lv = fn(il);
    auto rv = fn(ir);
    auto ret = cmp(lv, rv) ? std::make_pair((idx_type)ir, rv) : std::make_pair((idx_type)il, lv);
    while (*k > 3) {
      if (cmp(lv, rv)) {
        l += *(k-2);
        lv = rv;
        auto i = std::min(r-1, l + *(k-2));
        rv = fn(i);
        if (cmp(ret.second, rv))
          ret = std::make_pair(i, rv);
      } else {
        rv = lv;
        auto i = std::min(r-1, l + *(k-3));
        lv = fn(i);
        if (cmp(ret.second, lv))
          ret = std::make_pair(i, lv);
      }
      --k;
    }
    return ret;
  }
};

template<typename I, typename F, typename C = std::less<>>
auto fibonacci_search(I l, I r, F fn, const C& cmp = C()) 
  -> std::pair<typename std::remove_reference<I>::type,
               decltype(fn(l))> {
  return FibonacciSearch<C>(r - l, cmp)(l, r, fn);
}
template<typename I, typename F, typename C = std::less<>>
auto trisect_int(I l, I r, F fn, const C& cmp = C())
  -> std::pair<typename std::remove_reference<I>::type, 
               decltype(fn(l))> {
  return fibonacci_search(l, r, fn, cmp);
}

constexpr double GOLDEN_RATIO = 1.61803398875;
template<typename F, typename C = std::less<>>
auto golden_ratio_search(double l, double r, F fn, double eps = EPS_DEFAULT, const C& cmp = C())
  -> std::pair<double, decltype(fn(l))> {
  using value_type = decltype(fn(l));
  std::pair<double, value_type> ret;
  while (std::abs(r - l) > eps) {
    double il = (l * GOLDEN_RATIO + r) / (GOLDEN_RATIO + 1.);
    double ir = (l + r * GOLDEN_RATIO) / (GOLDEN_RATIO + 1.);
    auto vl = fn(il);
    auto vr = fn(ir);
    if (cmp(vr, vl)) {
      ret = std::make_pair(il, vl);
      r = ir;
    } else {
      ret = std::make_pair(ir, vr);
      l = il;
    }
  }
  return ret;
}
template<typename F, typename C = std::less<>>
auto trisect_float(double l, double r, F fn, double eps = EPS_DEFAULT, const C& cmp = C())
  -> std::pair<double, decltype(fn(l))> {
  return golden_ratio_search(l, r, fn, eps, cmp);
}
