/*
 * The Restful Matching-Engine.
 * Copyright (C) 2013, 2016 swirly Cloud Limited.
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */
#ifndef TINYCHAIN_TEA_TEST_HPP
#define TINYCHAIN_TEA_TEST_HPP

#ifndef TINYCHAIN_API
#define TINYCHAIN_API __attribute__((visibility("default")))
#endif // TINYCHAIN_API

#include <cmath>
#include <exception>

/**
 * @addtogroup Test
 * @{
 */

namespace tinychain {
namespace test {

class TINYCHAIN_API TestException : public std::exception {
  enum { ErrMax = 127 };
  const char* file_;
  int line_;
  char msg_[ErrMax + 1];

 public:
  TestException(const char* file, int line, const char* msg) noexcept;

  ~TestException() noexcept override;

  // Copy.
  TestException(const TestException& rhs) noexcept = default;
  TestException& operator=(const TestException& rhs) noexcept = default;

  // Move.
  TestException(TestException&&) noexcept = default;
  TestException& operator=(TestException&&) noexcept = default;

  const char* what() const noexcept override;
  const char* file() const noexcept { return file_; }
  int line() const noexcept { return line_; }
};

template <typename ExceptionT, typename FnT>
bool checkThrow(FnT fn)
{
  bool pass{false};
  try {
    fn();
  } catch (const ExceptionT&) {
    pass = true;
  } catch (...) {
  }
  return pass;
}

/**
 * Add test-case. Test-cases use this function to register themselves during static initialisation.
 */
TINYCHAIN_API void addTestCase(const char* name, void (*fn)(void));

/**
 * Run all test-cases.
 */
TINYCHAIN_API int run(int argc, char* argv[]);

constexpr auto Epsilon = 1e-7;

inline bool isSame(double lhs, double rhs, double delta = Epsilon) noexcept
{
  return std::abs(lhs - rhs) <= delta;
}

} // test
} // TINYCHAIN

#define TINYCHAIN_FAIL_(file, line, what) throw TINYCHAIN::test::TestException(file, line, what)

#define TINYCHAIN_FAIL(what) TINYCHAIN_FAIL_(__FILE__, __LINE__, what)

#define TINYCHAIN_CHECK(expr) (expr) ? (void)0 : TINYCHAIN_FAIL("assertion [" #expr "] failed")

#define TINYCHAIN_CHECK_THROW(expr, ExceptionT)                                                       \
  TINYCHAIN::test::checkThrow<ExceptionT>([&]() { expr; })                                            \
    ? (void)0                                                                                      \
    : TINYCHAIN_FAIL("throw expression [" #expr "] failed")

#define TINYCHAIN_TEST_CASE(name)                                                                     \
  void init##name() __attribute__((constructor));                                                  \
  void test##name();                                                                               \
  void init##name() { TINYCHAIN::test::addTestCase(#name, test##name); }                              \
  void test##name()

#define TINYCHAIN_FIXTURE_TEST_CASE(name, FixtureT)                                                   \
  struct name##Fixture : FixtureT {                                                                \
    void run();                                                                                    \
  };                                                                                               \
  TINYCHAIN_TEST_CASE(name)                                                                           \
  {                                                                                                \
    name##Fixture fixture;                                                                         \
    fixture.run();                                                                                 \
  }                                                                                                \
  void name##Fixture::run()

/** @} */

#endif // TINYCHAIN_TEA_TEST_HPP
