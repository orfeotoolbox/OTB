/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**\file otbTestHelpers.h
 * Various utility classes and function to help write unit tests based on Boot.Test.
 *
 * \ingroup OTBTestKernel
 */
#ifndef otbBoostTestHelpers_h
#define otbBoostTestHelpers_h

#include "otbStringUtilities.h"
#include <boost/test/tools/output_test_stream.hpp>
#include <boost/test/unit_test_log.hpp>
#include <iostream>

namespace boost::test_tools
{

struct cout_redirect
{
  explicit cout_redirect(std::streambuf* new_buffer)
    : m_old(std::cout.rdbuf(new_buffer)) {}
  ~cout_redirect() { std::cout.rdbuf(m_old); }

  cout_redirect(const cout_redirect&) = delete;
  cout_redirect& operator=(const cout_redirect&) = delete;

private:
  std::streambuf* m_old;
};

[[nodiscard]]
inline assertion_result contains(const std::string& captured, const std::string& expected)
{
  assertion_result ar(otb::contains(captured, expected));
  ar.message() << "\"" << expected << "\" not found in captured log:\n"
               << "--- log start ---\n" << captured << "--- log end ---";
  return ar;
}

[[nodiscard]]
inline auto check_exception_against(otb::string_view pattern)
{
  return [=](std::exception const& e) {
    BOOST_TEST_MESSAGE("exception caught: " << e.what());
    return otb::contains(e.what(), pattern);
  };
};

}

#endif // otbBoostTestHelpers_h
