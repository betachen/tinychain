/*
 * Happy Coding&Trading (0_0).
 */
#ifndef TOKENISER_HPP
#define TOKENISER_HPP

#include <algorithm>

/**
 * @addtogroup Util
 * @{
 */

namespace tinychain {

template <char DelimN>
class Tokeniser {
 public:
  explicit Tokeniser(std::string_view buf) noexcept { reset(buf); }
  explicit Tokeniser() noexcept { reset(""); }
  ~Tokeniser() noexcept = default;

  // Copy.
  Tokeniser(const Tokeniser& rhs) noexcept = default;
  Tokeniser& operator=(const Tokeniser& rhs) noexcept = default;

  // Move.
  Tokeniser(Tokeniser&&) noexcept = default;
  Tokeniser& operator=(Tokeniser&&) noexcept = default;

  std::string_view top() const noexcept { return buf_.substr(i_ - buf_.cbegin(), j_ - i_); }
  bool empty() const noexcept { return i_ == buf_.cend(); }
  void reset(std::string_view buf) noexcept
  {
    buf_ = buf;
    i_ = buf_.cbegin();
    j_ = std::find(i_, buf_.cend(), DelimN);
  }
  void pop() noexcept
  {
    if (j_ != buf_.cend()) {
      i_ = j_ + 1;
      j_ = std::find(i_, buf_.cend(), DelimN);
    } else {
      i_ = j_;
    }
  }

 private:
  std::string_view buf_;
  std::string_view::const_iterator i_, j_;
};

} // swirly

/** @} */

#endif // TOKENISER_HPP
