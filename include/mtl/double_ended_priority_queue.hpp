#pragma once
#include <cassert>
#include <vector>
#include <iostream>
#include <algorithm>

/** 
 * @brief Double Ended Priority Queue
 * @query
 *   - initalize: $O(n)$
 *   - push: $O(\log n)$
 *   - get_min: $O(1)$
 *   - get_max: $O(1)$
 *   - pop_min: $O(\log n)$
 *   - pop_max: $O(\log n)$
*/
template<typename T>
struct DoubleEndedPriorityQueue {
  // index = [id | (0 if forward, 1 backward)]
  size_t index(size_t id, bool b) const { return (id<<1)|b; }
  size_t parent(size_t i) const { return index(((i>>1)-1)/2, i&1); }
  size_t left(size_t i) const { return index((i>>1)*2+1, i&1); }
  size_t right(size_t i) const { return index((i>>1)*2+2, i&1); }
  size_t sibling(size_t i) const { return i^1; }

  std::vector<T> arr;

  DoubleEndedPriorityQueue() {}
  template<typename It>
  DoubleEndedPriorityQueue(It begin, It end) : arr(begin, end) {
    size_t n = arr.size();
    if (n <= 1) return;
    if (n == 2) {
      if (arr[index(0,0)] < arr[index(0,1)])
        std::swap(arr[index(0,0)], arr[index(0,1)]);
      return;
    }
    for (long long i = arr.size()-1; i >= 0; i--) {
      if ((i & 1) and arr[i-1] < arr[i])
        std::swap(arr[i-1], arr[i]);
      heapify(i, true);
    }
  }

  template<class Compare>
  size_t heap_down(size_t i) {
    Compare comp;
    size_t k;
    while ((k = left(i)) < arr.size()) {
      auto r = right(i);
      if ((r&1) and r==size()) --r;
      if (r < arr.size() and comp(arr[k], arr[r])) k = r;
      if (comp(arr[i], arr[k])) {
        std::swap(arr[i], arr[k]);
        i = k;
      } else break;
    }
    return i;
  }
  size_t min_heap_down(size_t i) {
    assert(i&1);
    return heap_down<std::greater<>>(i);
  }
  size_t max_heap_down(size_t i) {
    assert((i&1)==0);
    return heap_down<std::less<>>(i);
  }

  size_t heap_leaf(size_t i) {
    if ((i|1) < arr.size() and arr[i&~1ull] < arr[i|1]) {
      std::swap(arr[i], arr[i^1]);
      i ^= 1;
    }
    return i;
  }

  size_t min_heap_up(size_t i, size_t root) {
    size_t p;
    while (i>>1 > root>>1 and (p = parent(i)|1) >= root and arr[i] < arr[p]) {
      std::swap(arr[p], arr[i]);
      i = p;
    }
    return i;
  }
  size_t max_heap_up(size_t i, size_t root) {
    size_t p;
    while (i>>1 > root>>1 and (p = parent(i)&~1ull) >= root and arr[p] < arr[i]) {
      std::swap(arr[p], arr[i]);
      i = p;
    }
    return i;
  }

  void heapify(size_t i, bool limited) {
    auto j = (i&1) ? min_heap_down(i) : max_heap_down(i);
    auto k = heap_leaf(j);
    auto root = limited ? i : 0;
    max_heap_up(k, root);
    min_heap_up(k, root);
  }

  template<class U>
  void push(U&& val) {
    static_assert(std::is_convertible<U,T>::value, "");
    auto id = arr.size();
    arr.push_back(std::forward<U>(val));
    heapify(id, false);
  }
  void pop_max() {
    assert(!empty());
    std::swap(arr[index(0,0)], arr.back());
    arr.pop_back();
    if (index(0,0) < arr.size())
      heapify(index(0,0), false);
  }
  void pop_min() {
    assert(!empty());
    if (size() == 1) {
      arr.pop_back();
      return;
    }
    std::swap(arr[index(0,1)], arr.back());
    arr.pop_back();
    if (index(0,1) < arr.size())
      heapify(index(0,1), false);
  }

  const T& max() const {
    assert(!empty());
    return arr[index(0,0)];
  }
  const T& min() const {
    assert(!empty());
    return arr[index(0, size() > 1 ? 1 : 0)];
  }

  size_t size() const { return arr.size(); }
  bool empty() const { return size() == 0; }
  void clear() { arr.clear(); }

};