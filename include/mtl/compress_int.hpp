#pragma once
#include <set>
#include <unordered_map>
#include <vector>
#include <algorithm>

template<typename T>
struct Compressor {
  std::vector<T> vs;
  Compressor() = default;
  template<typename It>
  Compressor(It begin, It end) : vs(begin, end) {}
  void clear() { vs.clear(); }
  void add(T x) {
    vs.push_back(x);
  }
  template<typename It>
  void add(It begin, It end) {
    vs.insert(vs.end(), begin, end);
  }
  using map_type = std::unordered_map<T,int>;
  map_type release() {
    std::sort(vs.begin(), vs.end());
    vs.erase(std::unique(vs.begin(), vs.end()), vs.end());
    std::unordered_map<T,int> mp;
    mp.reserve(vs.size());
    int k = 0;
    for (auto v : vs) mp[v] = k++;
    return mp;
  }
  std::pair<map_type, std::vector<T>> release_tie() {
    return std::make_pair(release(), vs);
  }
  template<typename It>
  static std::unordered_map<T,int> compress(It begin, It end) {
    return Compressor(begin, end).release();
  }
};
