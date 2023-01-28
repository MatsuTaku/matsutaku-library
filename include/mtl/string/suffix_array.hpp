#pragma once
#include <vector>
#include <iterator>
#include <limits>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <cstring>
#include <iostream>

void sa_is(const std::vector<int>& S, std::vector<std::pair<int, int>>& bucket, std::vector<int>::iterator dst, bool definitely_distinct_lmss) {
  const int n = (int) S.size();
  assert(S.back() == 0);
  assert(*max_element(S.begin(), S.end()) < n);
  // Induced Suffix Sorting
  if (n <= 3) {
    if (n == 1)
      dst[0] = 0;
    else if (n == 2) {
      dst[0] = 1;
      dst[1] = 0;
    } else if (n == 3) {
      dst[0] = 2;
      if (S[0] < S[1]) {
        dst[1] = 0;
        dst[2] = 1;
      } else {
        dst[1] = 1;
        dst[2] = 0;
      }
    }
    return;
  }
  std::vector<bool> is_s(n);
  is_s[n-1] = true;
  for (int i = n-3; i >= 0; i--) {
    int l = S[i], r = S[i+1];
    is_s[i] = l < r or (l == r and is_s[i+1]);
  }
  auto is_lms = [&](int i) {
    return i and is_s[i] and !is_s[i-1];
  };

  std::vector<int> cnt(n);
  for (int c : S)
    cnt[c]++;
  assert(bucket.size() >= (size_t)n);
  auto init_bucket = [&bucket, n](const std::vector<int>& cnt) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
      bucket[i] = {sum, sum+cnt[i]-1};
      sum += cnt[i];
    }
  };
  init_bucket(cnt);

  auto A = &*dst;
  memset(A, -1, sizeof(int)*n);
  int cnt_lms = 0;
  for (int i = n-1; i >= 1; i--) if (is_lms(i)) {
      A[bucket[S[i]].second--] = i;
      cnt_lms++;
  }
  auto induced_sort = [&]() {
    for (int i = 0; i < n; i++) {
      if (A[i] == -1) continue;
      int p = A[i] ? A[i]-1 : n-1;
      if (!is_s[p])
        A[bucket[S[p]].first++] = p;
      if (is_lms(A[i])) {
        assert(bucket[S[A[i]]].second+1 == i);
        A[++bucket[S[A[i]]].second] = -1;
      }
    }
    for (int i = n-1; i >= 0; i--) {
      assert(A[i] != -1);
      int p = A[i] ? A[i]-1 : n-1;
      if (is_s[p])
        A[bucket[S[p]].second--] = p;
    }
  };
  induced_sort();

  if (definitely_distinct_lmss)
    return;

  // Build SA of LMS-substrings
  std::vector<int> sa_lmss(cnt_lms), lmss(n);
  {
    auto are_same_lms_substr = [&](int i, int j) {
      assert(is_lms(i) and is_lms(j));
      if (S[i] != S[j])
        return false;
      i++; j++;
      while(true) {
        if (S[i] != S[j] or is_lms(i) != is_lms(j))
          return false;
        if (is_lms(i))
          break;
        i++; j++;
      }
      return true;
    };
    int lms_ord = 0;
    int plms = n-1;
    auto cmp = [&](int i) -> int& {
      return lmss[cnt_lms + i/2];
    };
    assert(A[0] == n-1);
    cmp(A[0]) = 0;
    for (int i = 1; i < n; i++) if (is_lms(A[i])) {
      if (!are_same_lms_substr(plms, A[i]))
        ++lms_ord;
      cmp(A[i]) = lms_ord;
      plms = A[i];
    }
    int k = 0;
    for (int i = 1; i < n; i++) if (is_lms(i))
      lmss[k++] = cmp(i);
    lmss.resize(cnt_lms);
    sa_is(lmss, bucket, sa_lmss.begin(), cnt_lms - 1 == lms_ord);
  }

  // Induced-sort by ordered LMSs
  auto& citi = lmss; // Reuse memory
  citi.clear();
  for (int i = 1; i < n; i++) if (is_lms(i))
    citi.push_back(i);
  init_bucket(cnt);
  memset(A, -1, sizeof(int)*n);
  for (auto it = sa_lmss.rbegin(); it != sa_lmss.rend(); ++it)
    A[bucket[S[citi[*it]]].second--] = citi[*it];
  induced_sort();
}

template<typename It>
[[nodiscard]]
std::vector<int> SuffixArray(It begin, It end) {
  using trait = std::iterator_traits<It>;
  static_assert(std::is_base_of<std::random_access_iterator_tag, typename trait::iterator_category>::value, "");

  int n = end - begin;
  std::vector<int> S(n+1);
  S[n] = 0;
  auto mp = std::minmax_element(begin, end);
  if (0 < (int)*mp.first and (int)*mp.second <= n) {
    std::copy(begin, end, S.begin());
  } else {
    std::vector<int> ord(n);
    std::iota(ord.begin(), ord.end(), 0);
    std::sort(ord.begin(), ord.end(), [&](int l, int r) {
      return *(begin+l) < *(begin+r);
    });
    int k = 1;
    for (int i = 0; i < n; i++)
      S[ord[i]] = i and *(begin+ord[i]) != *(begin+ord[i-1]) ? ++k : k;
  }
  std::vector<int> A(n+1);
  std::vector<std::pair<int,int>> bucket(n+1);
  sa_is(S, bucket, A.begin(), false);
  return std::vector<int>(A.begin()+1, A.end());
}