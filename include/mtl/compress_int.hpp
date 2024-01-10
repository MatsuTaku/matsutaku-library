#pragma once
#include <set>
#include <unordered_map>
#include <vector>
#include <algorithm>

template<class T, class MapContainer=std::unordered_map<T, int>>
class Compressor {
 public:
  using map_type = MapContainer;
 private:
  std::vector<T> vs;
 public:
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
  map_type release() {
    std::sort(vs.begin(), vs.end());
    vs.erase(std::unique(vs.begin(), vs.end()), vs.end());
    map_type mp;
    mp.reserve(vs.size());
    int k = 0;
    for (auto v : vs) mp[v] = k++;
    return mp;
  }
  std::pair<map_type, std::vector<T>> release_tie() {
    return std::make_pair(release(), std::move(vs));
  }
  template<typename It>
  static map_type compress(It begin, It end) {
    return Compressor(begin, end).release();
  }
};
