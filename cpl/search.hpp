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

template<typename T, typename I, typename F, typename C>
std::pair<typename std::remove_reference<I>::type, T>
fibonacci_search(I l, I r, F fn, C cmp = std::less<T>()) {
  using idx_type = typename std::remove_reference<I>::type;
  assert(r - l >= 2);
  idx_type d = r-l;
  std::vector<idx_type> fib{1,1};
  while (fib.back() < d) fib.push_back(fib[fib.size()-2] + fib[fib.size()-1]);
  auto k = --fib.cend();
  idx_type il = l + *(k-2);
  idx_type ir = l + *(k-1);
  T lv = fn(il);
  T rv = fn(ir);
  std::pair<idx_type, T> ret = cmp(lv, rv) ? std::make_pair(ir, rv) : std::make_pair(il, lv);
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
template<typename T, typename I, typename F, typename C = std::less<T>>
std::pair<typename std::remove_reference<I>::type, T>
trisect_int(I l, I r, F fn, C cmp = C()) {
  return fibonacci_search<T>(l, r, fn, cmp);
}

constexpr double GOLDEN_RATIO = 1.61803398875;
template<typename T, typename F, typename C>
std::pair<double, T>
golden_ratio_search(double l, double r, F fn, double eps = EPS_DEFAULT, C cmp = std::less<T>()) {
  std::pair<double, T> ret;
  while (std::abs(r - l) > eps) {
    double il = (l * GOLDEN_RATIO + r) / (GOLDEN_RATIO + 1.);
    double ir = (l + r * GOLDEN_RATIO) / (GOLDEN_RATIO + 1.);
    T vl = fn(il);
    T vr = fn(ir);
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
template<typename T, typename F, typename C>
std::pair<double, T>
trisect_float(double l, double r, F fn, double eps = EPS_DEFAULT, C cmp = std::less<T>()) {
  return golden_ratio_search<T>(l, r, fn, eps, cmp);
}
