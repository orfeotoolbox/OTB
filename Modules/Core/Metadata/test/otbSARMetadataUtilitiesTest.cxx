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
#include <stdexcept>

#define BOOST_TEST_MODULE OTB SARMetadataUtilities unit testing
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>
#include "otbBoostTestHelpers.h"
#include "itkMacro.h" // for itkExceptionObject...
#include <optional>

namespace tt = boost::test_tools;

static const otb::dict_t k_map {
  {"ui8", "27"},
  {"i16", "-132"},
  {"ui16", "132"},
  {"dbl", "12.45"},
  {"nan", "nan"},
  {"str", "string_value"},
};

namespace std {
template <typename T>
std::ostream& operator<<(std::ostream& os, std::optional<T> const& o) {
  if (o)
    return os << *o;
  else
    return os << "(none)";
}
}  // namespace std

/*-------------------------------[ Simplify value_or instantiation ]---------*/
namespace otb::OnDecodingError {
  using WarnAndReturnNone = WarnAndReturn::Default<std::optional>;
} // otb namespace

using namespace otb;

template <
  template <typename> class MissingKeyPolicy,
  template <typename> class DecodingPolicy
>
struct MakeGetterII
{
  template <typename T>
  auto value_or(std::string const& key) const
  {
    return ::otb::value_or<T, MissingKeyPolicy, DecodingPolicy>(k_map, key, m_context);
  }

  std::string m_context;
};

template <
  template <typename> class MissingKeyPolicy,
  typename                  DecodingPolicy
>
struct MakeGetterID
{
  template <typename T>
  auto value_or(std::string const& key) const
  {
    return ::otb::value_or<T, MissingKeyPolicy, DecodingPolicy>(k_map, key, m_context);
  }

  std::string m_context;
};

template <
  template <typename> class MissingKeyPolicy,
  template <typename> class DecodingPolicy
>
auto getter(std::string context) {
  return MakeGetterII<MissingKeyPolicy, DecodingPolicy>{std::move(context)};
}

template <
  template <typename> class MissingKeyPolicy,
  typename                  DecodingPolicy
>
auto getter(std::string context) {
  return MakeGetterID<MissingKeyPolicy, DecodingPolicy>{std::move(context)};
}

#define TEST_SUCCESS(type, key, expected)                                              \
  do {                                                                                 \
    tt::output_test_stream output;                                                     \
    {                                                                                  \
      tt::cout_redirect guard(output.rdbuf());                                         \
      auto const res = get.value_or<type>(key);                                        \
      otb::Logger::Instance()->Flush();                                                \
      BOOST_TEST(res == expected);                                                     \
    }                                                                                  \
    BOOST_TEST(! tt::contains(output.str(), "(WARNING):"));                            \
  } while (0)

#define TEST_IGNORE(type, key)                                                         \
  do {                                                                                 \
    TEST_SUCCESS(type, key, std::optional<type>{});                                    \
  } while (0)

#define TEST_WARN(type, key, expected_warning)                                         \
  do {                                                                                 \
    tt::output_test_stream output;                                                     \
    {                                                                                  \
      tt::cout_redirect guard(output.rdbuf());                                         \
      auto const res = get.value_or<type>(key);                                        \
      otb::Logger::Instance()->Flush();                                                \
      BOOST_TEST(res == std::optional<type>{});                                        \
    }                                                                                  \
    BOOST_TEST(tt::contains(output.str(), "(WARNING): " expected_warning));            \
  } while (0)

#define TEST_THROW(type, key, exception_exception_type, expected_exception_msg)        \
  do {                                                                                 \
    tt::output_test_stream output;                                                     \
    {                                                                                  \
      tt::cout_redirect guard(output.rdbuf());                                         \
      BOOST_CHECK_EXCEPTION(                                                           \
          (void)get.value_or<type>(key),                                               \
          exception_exception_type,                                                    \
          tt::check_exception_against(expected_exception_msg));                        \
    }                                                                                  \
    BOOST_TEST(! tt::contains(output.str(), "(WARNING):"));                            \
  } while (0)

#define TEST_THROW_std(type, key, expected_exception)                                  \
  TEST_THROW(type, key, std::runtime_error, expected_exception)

#define TEST_THROW_otb(type, key, expected_exception)                                  \
  TEST_THROW(type, key, itk::ExceptionObject, expected_exception)


/*-------------------------------[ Tests ]-----------------------------------*/
BOOST_AUTO_TEST_CASE(test_IgnoreMissing_WarnIncorrect)
{
  auto get = getter<OnMissing::IgnoreAndReturnNone, OnDecodingError::WarnAndReturnNone>("from metadata");

  TEST_SUCCESS(unsigned char, "ui8", 27);
  TEST_SUCCESS(  signed char, "ui8", 27);
  TEST_SUCCESS( signed short, "ui8", 27);
  TEST_SUCCESS(long long    , "ui8", 27);

  // Invalid range
  TEST_WARN(signed char, "ui16", "Cannot decode '");

  // Invalid range: sign
  TEST_WARN(unsigned short, "i16", "Cannot decode '");

  // Invalid type: floatting point value
  TEST_WARN(long, "dbl", "Cannot decode '");

  // Invalid type: string
  TEST_WARN(long, "str", "Cannot decode '");

  TEST_IGNORE(unsigned char, "missing");
}

BOOST_AUTO_TEST_CASE(test_IgnoreMissing_ThrowIncorrect)
{
  auto get = getter<OnMissing::IgnoreAndReturnNone, OnDecodingError::Throw>("from metadata");

  TEST_SUCCESS(unsigned char, "ui8", 27);
  TEST_SUCCESS(  signed char, "ui8", 27);
  TEST_SUCCESS( signed short, "ui8", 27);
  TEST_SUCCESS(long long    , "ui8", 27);

  // Invalid range
  TEST_THROW_std(signed char, "ui16", "Cannot decode '");

  // Invalid range: sign
  TEST_THROW_std(unsigned short, "i16", "Cannot decode '");

  // Invalid type: floatting point value
  TEST_THROW_std(long, "dbl", "Cannot decode '");

  // Invalid type: string
  TEST_THROW_std(long, "str", "Cannot decode '");

  TEST_IGNORE(unsigned char, "missing");
}

BOOST_AUTO_TEST_CASE(test_WarnMissing_ThrowIncorrect)
{
  auto get = getter<OnMissing::WarnAndReturnNone, OnDecodingError::Throw>("from metadata");

  TEST_SUCCESS(unsigned char, "ui8", 27);
  TEST_SUCCESS(  signed char, "ui8", 27);
  TEST_SUCCESS( signed short, "ui8", 27);
  TEST_SUCCESS(long long    , "ui8", 27);

  // Invalid range
  TEST_THROW_std(signed char, "ui16", "Cannot decode '");

  // Invalid range: sign
  TEST_THROW_std(unsigned short, "i16", "Cannot decode '");

  // Invalid type: floatting point value
  TEST_THROW_std(long, "dbl", "Cannot decode '");

  // Invalid type: string
  TEST_THROW_std(long, "str", "Cannot decode '");

  TEST_WARN(unsigned char, "missing", "Unable to find 'missing' while extracting metadata key 'missing' from metadata");
}

BOOST_AUTO_TEST_CASE(test_ThrowMissing_WarnIncorrect)
{
  auto get = getter<OnMissing::Throw, OnDecodingError::WarnAndReturnNone>("from metadata");

  TEST_SUCCESS(unsigned char, "ui8", 27);
  TEST_SUCCESS(  signed char, "ui8", 27);
  TEST_SUCCESS( signed short, "ui8", 27);
  TEST_SUCCESS(long long    , "ui8", 27);

  // Invalid range
  TEST_WARN(signed char, "ui16", "Cannot decode '");

  // Invalid range: sign
  TEST_WARN(unsigned short, "i16", "Cannot decode '");

  // Invalid type: floatting point value
  TEST_WARN(long, "dbl", "Cannot decode '");

  // Invalid type: string
  TEST_WARN(long, "str", "Cannot decode '");

  TEST_THROW_otb(unsigned char, "missing", "Unable to find 'missing' while extracting metadata key 'missing' from metadata");
}

BOOST_AUTO_TEST_CASE(test_ThrowMissing_ThrowIncorrect)
{
  auto get = getter<OnMissing::Throw, OnDecodingError::Throw>("from metadata");

  TEST_SUCCESS(unsigned char, "ui8", 27);
  TEST_SUCCESS(  signed char, "ui8", 27);
  TEST_SUCCESS( signed short, "ui8", 27);
  TEST_SUCCESS(long long    , "ui8", 27);

  // Invalid range
  TEST_THROW_std(signed char, "ui16", "Cannot decode '");

  // Invalid range: sign
  TEST_THROW_std(unsigned short, "i16", "Cannot decode '");

  // Invalid type: floatting point value
  TEST_THROW_std(long, "dbl", "Cannot decode '");

  // Invalid type: string
  TEST_THROW_std(long, "str", "Cannot decode '");

  TEST_THROW_otb(unsigned char, "missing", "Unable to find 'missing' while extracting metadata key 'missing' from metadata");
}

