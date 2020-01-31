/**
 * Copyright (c) 2019-2020 CHENHAO.
 *
 * This file is part of tinychain.
 *
 * tinychain is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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

} // namespace

/** @} */

#endif // TOKENISER_HPP
