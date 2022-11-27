#pragma once
#include <iostream>

template<typename T, int ID>
class boundint {
 private:
  static T sup_;
  T v_;
 public:
  static void set_sup(T sup) { sup_ = sup; }
  static T sup() { return sup_; }
  boundint(T v=0) : v_(v) {
    if (v_ < 0)
      v_ = 0;
    if (v_ > sup_)
      v_ = sup_;
  }
  T val() const { return v_; }
  bool operator==(const boundint& r) const { return v_ == r.v_; }
  bool operator<(const boundint& r) const { return v_ < r.v_; }
  bool operator<=(const boundint& r) const { return v_ <= r.v_; }
  bool operator>(const boundint& r) const { return v_ > r.v_; }
  bool operator>=(const boundint& r) const { return v_ >= r.v_; }
  template<typename U> bool operator==(const U& r) const { return v_ == r; }
  template<typename U> bool operator<(const U& r) const { return v_ < r; }
  template<typename U> bool operator<=(const U& r) const { return v_ <= r; }
  template<typename U> bool operator>(const U& r) const { return v_ > r; }
  template<typename U> bool operator>=(const U& r) const { return v_ >= r; }
  boundint operator=(const boundint& r) {
    v_ = r.v_;
    return *this;
  }
  boundint operator+=(const boundint& r) {
    v_ = v_ <= sup_ - r.v_ ? v_ + r.v_ : sup_;
    return *this;
  }
  boundint operator-=(const boundint& r) {
    v_ = v_ >= r.v_ ? v_ - r.v_ : 0;
    return *this;
  }
  boundint operator*=(const boundint& r) {
    v_ = v_ <= sup_ / r.v_ ? v_ * r.v_ : sup_;
    return *this;
  }
  boundint operator/=(const boundint& r) {
    v_ /= r.v_;
    return *this;
  }
  boundint operator+(const boundint& r) const { return boundint(*this) += r; }
  boundint operator-(const boundint& r) const { return boundint(*this) -= r; }
  boundint operator*(const boundint& r) const { return boundint(*this) *= r; }
  boundint operator/(const boundint& r) const { return boundint(*this) /= r; }

  friend std::istream& operator>>(std::istream& is, boundint& x) {
    return is >> x.v_;
  }
  friend std::ostream& operator<<(std::ostream& os, const boundint& x) {
    return os << x.val();
  }
};
template<typename T, int ID>
T boundint<T, ID>::sup_;
