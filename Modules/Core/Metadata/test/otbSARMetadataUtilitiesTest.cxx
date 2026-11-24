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

#include "otbSARMetadataUtilities.h"
#include <optional>

#define BOOST_TEST_MODULE OTB String Utilities unit testing
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>
// #include "otbBoostTestHelpers.h"

static const otb::dict_t k_map {
  {"ui8", "27"},
  {"i16", "-132"},
  {"ui16", "132"},
  {"dbl", "12.45"},
  {"nan", "nan"},
  {"str", "string_value"},
};

namespace otb::OnDecodingError {
  using WarnAndReturnNone = WarnAndReturn::Default<std::optional>;
} // otb namespace

using namespace otb;

template <template <typename> typename MP, typename DEP>
struct MakeGetter
{
  MakeGetter(std::string context) : m_context(std::move(context)) {}

  template <typename T>
  auto value_or(std::string const& key) const
  {
    return ::otb::value_or<T, MP, DEP>(k_map, key, m_context);
  }
private:
  std::string m_context;
};

BOOST_AUTO_TEST_CASE(test_IgnoreMissing_WarnIncorrect)
{
  auto get = MakeGetter<OnMissing::IgnoreAndReturnNone, OnDecodingError::WarnAndReturnNone>{"deserializing metadata"};

  BOOST_CHECK(( get.value_or<unsigned char>("ui8") == 27));
  BOOST_CHECK(( get.value_or<unsigned char>("ui8") == 27));

  // Invalid range
  BOOST_CHECK(( get.value_or<signed char>("ui16") == std::optional<int>{}));

  // Invalid range: sign
  BOOST_CHECK(( get.value_or<unsigned short>("u16") == std::optional<int>{}));

  // Invalid type: floatting point value
  BOOST_CHECK(( get.value_or<long>("dbl") == std::optional<int>{}));

  // Invalid type: string
  BOOST_CHECK(( get.value_or<long>("str") == std::optional<int>{}));

  BOOST_CHECK(( get.value_or<unsigned char>("missing") == std::optional<char>{}));


}
