#pragma once
#include <cassert>
#include <vector>
#include <iostream>

template<typename T>
struct DoubleEndedPriorityQueue {
  // index = [id | (0 if forward, 1 backward)]
  std::vector<T> arr;
  DoubleEndedPriorityQueue() : arr(2) {}
  template<typename It>
  DoubleEndedPriorityQueue(It begin, It end) : arr(2) {
    arr.insert(arr.end(), begin, end);
    size_t n = end - begin;
    if (n <= 1) return;
    if (n == 2) {
      if (arr[2|0] < arr[2|1])
        std::swap(arr[2|0], arr[2|1]);
      return;
    }
    for (size_t i = n+2-1; i > 1; i--) {
      if (i^1) {
        auto l = (i^1) << 1 | 1;
        if (l >= arr.size()) {
          auto co = i-1;
          if (arr[co] < arr[i])
            std::swap(arr[co], arr[i]);
        }
        auto j = (i >> 1) | 1;
        if (j > 1 and arr[i] < arr[j])
          std::swap(arr[i], arr[j]);
      } else {
        auto j = (i >> 1) & ~size_t(1);
        if (j > 1 and arr[j] < arr[i])
          std::swap(arr[j], arr[i]);
      }
    }
  }

  void push(T val) {
    auto id = arr.size();
    arr.push_back(val);
    id = bubble_up(id);
    tricle_down(id);
  }
  void pop_max() {
    assert(!empty());
    std::swap(arr[2|0], arr.back());
    arr.pop_back();
    tricle_down(2|0);
  }
  void pop_min() {
    assert(!empty());
    if (size() == 1) {
      arr.pop_back();
      return;
    }
    std::swap(arr[2|1], arr.back());
    arr.pop_back();
    bubble_up(2|1);
  }
  T max() const {
    assert(!empty());
    return arr[2|0];
  }
  T min() const {
    assert(!empty());
    return size() > 1 ? arr[2|1] : arr[2|0];
  }
  size_t size() const { return arr.size() - 2; }
  bool empty() const { return size() == 0; }

  size_t lower_priority_parent(size_t id) const {
    if (id&1) {
      auto l = (id^1) << 1 | 1;
      auto r = (id^1) << 1 | 3;
      if (r < arr.size()) {
        auto s = arr[l] < arr[r] ? l : r;
        return s;
      } else if (l < arr.size()) {
        return l;
      } else {
        auto prev = id ^ 1;
        return prev;
      }
    } else {
      return (id >> 1) & ~size_t(1);
    }
  }

  size_t upper_priority_child(size_t id) const {
    if (id&1) {
      return (id >> 1) | 1;
    } else {
      auto l = id << 1;
      auto r = id << 1 | 2;
      if (r < arr.size()) {
        auto s = arr[l] < arr[r] ? r : l;
        return s;
      } else if (l < arr.size()) {
        return l;
      } else {
        auto next = id ^ 1;
        if (next < arr.size())
          return next;
        else {
          return (id >> 1) | 1;
        }
      }
    }
  }

  size_t bubble_up(size_t id) {
    if (id == (2|0)) return id;
    auto p = lower_priority_parent(id);
    if (arr[p] < arr[id]) {
      std::swap(arr[p], arr[id]);
      return bubble_up(p);
    } else {
      return id;
    }
  }
  size_t tricle_down(size_t id) {
    if (id == (size() > 1 ? (2|1) : (2|0))) return id;
    auto c = upper_priority_child(id);
    if (arr[id] < arr[c]) {
      std::swap(arr[id], arr[c]);
      return tricle_down(c);
    } else {
      return id;
    }
  }
};