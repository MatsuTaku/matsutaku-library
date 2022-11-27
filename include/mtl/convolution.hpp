#pragma once
#include <vector>

template <typename Transformer>
class Convolution {
 public:
  using Polynomial = std::vector<typename Transformer::value_type>;
 private:
  Transformer tf_;
 public:
  explicit Convolution(size_t size) : tf_(size) {}

  Polynomial operator()(const Polynomial& g, const Polynomial& h) const {
    auto fg = tf_.fft(g);
    auto fh = tf_.fft(h);
    for (size_t i = 0; i < tf_.n(); i++) {
      fg[i] *= fh[i];
    }
    tf_.ifft_inline(fg);
    auto n_inv = (typename Polynomial::value_type)(1) / tf_.n();
    for (size_t i = 0; i < tf_.n(); i++)
      fg[i] *= n_inv;
    return fg;
  }

};

