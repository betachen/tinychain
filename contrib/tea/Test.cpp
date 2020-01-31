/*
 * The Restful Matching-Engine.
 * Copyright (C) 2013, 2016 Swirly Cloud Limited.
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
#include <contrib/tea/Test.hpp>

#include <cstring>
#include <iostream>
#include <map>
#include <regex>
#include <string>

using namespace std;

namespace tinychain {
namespace test {
namespace {

using TestCases = map<string, void (*)(void)>;

// Myers' singleton ensures that testCases is initiased when accessed from other translation units
// during static initialisation.
TestCases& testCases()
{
  static TestCases testCases;
  return testCases;
}

bool runTestCase(const string& name, void (*fn)(void))
{
  using namespace std;

  bool pass{false};

  cout << "checking " << name << " ... ";
  cout.flush();
  try {
    fn();
    cout << "pass" << endl;
    pass = true;
  } catch (const TestException& e) {
    cout << "fail" << endl;
    cerr << e.file() << ':' << e.line() << ": " << e.what() << endl;
  } catch (const exception& e) {
    cout << "fail" << endl;
    cerr << "exception: " << e.what() << endl;
  }
  return pass;
}
} // anonymous

TestException::TestException(const char* file, int line, const char* msg) noexcept
{
  file_ = file;
  line_ = line;
  strncpy(msg_, msg, ErrMax);
  msg_[ErrMax] = '\0';
}

TestException::~TestException() noexcept = default;

const char* TestException::what() const noexcept
{
  return msg_;
}

// Test-cases use this function to register themselves during static initialisation.
void addTestCase(const char* name, void (*fn)(void))
{
  testCases().emplace(name, fn);
}

int run(int argc, char* argv[])
{
  srand(time(nullptr));

  int ret{1};
  try {
    int failed{0}, total{0};
    if (argc > 1) {
      // Run specific test-cases according to regex arguments.
      for (int i{1}; i < argc; ++i) {
        const regex pattern{argv[i]};
        for (auto& tc : testCases()) {
          if (regex_match(tc.first, pattern)) {
            if (!runTestCase(tc.first, tc.second)) {
              ++failed;
            }
            ++total;
          }
        }
      }
    } else {
      // Run all test-cases.
      for (auto& tc : testCases()) {
        if (!runTestCase(tc.first, tc.second)) {
          ++failed;
        }
        ++total;
      }
    }
    if (total == 0) {
      cerr << "no tests found\n";
    } else if (failed > 0) {
      cerr << failed << " test(s) out of " << total << " failed\n";
    } else {
      cout << "all tests passed\n";
      ret = 0;
    }
  } catch (const exception& e) {
    // Most likely due to a regex compilation error.
    cerr << "exception: " << e.what() << endl;
  }
  return ret;
}

} // test
} // swirly
