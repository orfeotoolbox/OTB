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

#include "otbStringUtilities.h"

#define BOOST_TEST_MODULE OTB String Utilities unit testing
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>
#include <istream>
#include <ostream>

namespace tt = boost::test_tools;

/*-------------------------------[ Test to<> ]-------------------------------*/
#define SUCCESS_FOR_EACH_INTEGRAL(test, function, sign, value)                             \
  test(function<sign char>     (#value, "decoding " #sign " char for tests") == value);    \
  test(function<sign short>    (#value, "decoding " #sign " short for tests") == value);   \
  test(function<sign int>      (#value, "decoding " #sign " int for tests") == value);     \
  test(function<sign long>     (#value, "decoding " #sign " long for tests") == value);    \
  test(function<sign long long>(#value, "decoding " #sign " long long for tests") == value);

#define SUCCESS_FOR_EACH_FLOAT(function, value)                                                  \
  BOOST_CHECK_CLOSE(function<float>      (#value, "decoding float for tests"), value, 1e-5);     \
  BOOST_CHECK_CLOSE(function<double>     (#value, "decoding double for tests"), value, 1e-5);    \
  BOOST_CHECK_CLOSE(function<long double>(#value, "decoding long double for tests"), value, 1e-5);

#define FAIL_WITH_EXCEPTION_FOR_EACH_INTEGRAL(test, function, sign, value, exception)        \
  test(function<sign char>     (#value, "decoding " #sign " char for tests"), exception);    \
  test(function<sign short>    (#value, "decoding " #sign " short for tests"), exception);   \
  test(function<sign int>      (#value, "decoding " #sign " int for tests"), exception);     \
  test(function<sign long>     (#value, "decoding " #sign " long for tests"), exception);    \
  test(function<sign long long>(#value, "decoding " #sign " long long for tests"), exception);


// Test positive int
BOOST_AUTO_TEST_CASE(test_convert_positive_with_to)
{
  SUCCESS_FOR_EACH_INTEGRAL(BOOST_TEST, otb::to,         , 107);
  SUCCESS_FOR_EACH_INTEGRAL(BOOST_TEST, otb::to, unsigned, 107);

  SUCCESS_FOR_EACH_FLOAT(otb::to, 107);
}

// Test negative int
BOOST_AUTO_TEST_CASE(test_convert_negative_with_to)
{
  SUCCESS_FOR_EACH_INTEGRAL(BOOST_TEST, otb::to,         , -107);
  BOOST_CHECK_THROW((void)otb::to<unsigned>("-107", "ctx"), std::runtime_error);
  FAIL_WITH_EXCEPTION_FOR_EACH_INTEGRAL(BOOST_CHECK_THROW, (void)otb::to, unsigned, -107, std::runtime_error);

  SUCCESS_FOR_EACH_FLOAT(otb::to, -107);
}

// Test user-defined type
struct MyType {
  int i;
  friend std::istream & operator>>(std::istream & is, MyType& v)
  {
    return is >> v.i;
  }
  friend std::ostream & operator<<(std::ostream & os, MyType const& v)
  {
    return os << "MyType{" << v.i << "}";
  }
  friend bool operator==(MyType, MyType) = default;
};

BOOST_AUTO_TEST_CASE(test_convert_user_defined_type_with_to)
{
  BOOST_TEST(otb::to<MyType>("107", "testing on my user defined type") == MyType{107});
  BOOST_CHECK_THROW((void)otb::to<MyType>("10 7", "testing on my user defined type"), std::runtime_error);
}

// Test insufficient precision
BOOST_AUTO_TEST_CASE(test_convert_not_narrow_with_to)
{
  BOOST_TEST(otb::to<short>("1000", "conducting overflow tests") == 1000);

  BOOST_TEST(otb::to<signed char>("-128", "conducting overflow tests") == -128);
  BOOST_TEST(otb::to<signed char>("127", "conducting overflow tests") == 127);

  BOOST_CHECK_THROW((void)otb::to<signed char>("-129", "conducting overflow tests"), std::runtime_error);
  BOOST_CHECK_THROW((void)otb::to<signed char>("128", "conducting overflow tests"), std::runtime_error);

  BOOST_TEST(otb::to<signed short>("-32768", "conducting overflow tests") == -32768);
  BOOST_TEST(otb::to<signed short>("32767", "conducting overflow tests") == 32767);
  BOOST_CHECK_THROW((void)otb::to<signed short>("-32769", "conducting overflow tests"), std::runtime_error);
  BOOST_CHECK_THROW((void)otb::to<signed short>("32768", "conducting overflow tests"), std::runtime_error);

  BOOST_TEST(otb::to<unsigned char>("255", "conducting overflow tests") == 255);
  BOOST_CHECK_THROW((void)otb::to<unsigned char>("256", "conducting overflow tests"), std::runtime_error);
  BOOST_TEST(otb::to<unsigned short>("65535", "conducting overflow tests") == 65535);
  BOOST_CHECK_THROW((void)otb::to<unsigned short>("65536", "conducting overflow tests"), std::runtime_error);
}


// Test not numbers
BOOST_AUTO_TEST_CASE(test_invalid_numbers_with_to)
{
  BOOST_CHECK_THROW((void)otb::to<int>("ab1000", "conducting tests on invalid numbers"), std::runtime_error);
  BOOST_CHECK_THROW((void)otb::to<int>("1000ab", "conducting tests on invalid numbers"), std::runtime_error);
  BOOST_CHECK_THROW((void)otb::to<int>("10 00", "conducting tests on invalid numbers"), std::runtime_error);
  BOOST_CHECK_THROW((void)otb::to<long>("10 00", "conducting tests on invalid numbers"), std::runtime_error);
}

// Tests floats
BOOST_AUTO_TEST_CASE(test_floatting_point_numbers_with_to)
{
  SUCCESS_FOR_EACH_FLOAT(otb::to, 10.7);
  SUCCESS_FOR_EACH_FLOAT(otb::to, -10.7);
  SUCCESS_FOR_EACH_FLOAT(otb::to, -1.7e5);
  SUCCESS_FOR_EACH_FLOAT(otb::to, 1.7e+5);

  BOOST_TEST((std::isnan(otb::to<float>      ("nan", "decoding nan as float"))));
  BOOST_TEST((std::isnan(otb::to<double>     ("nan", "decoding nan as double"))));
  BOOST_TEST((std::isnan(otb::to<long double>("nan", "decoding nan as long double"))));
}

/*-------------------------------[ Test to_with_default<> ]------------------*/
#define SUCCESS_FOR_EACH_INTEGRAL_WITH_DEFAULT(test, function, sign, value, def) \
  test(function<sign char>     (#value, def) == value);                          \
  test(function<sign short>    (#value, def) == value);                          \
  test(function<sign int>      (#value, def) == value);                          \
  test(function<sign long>     (#value, def) == value);                          \
  test(function<sign long long>(#value, def) == value);

#define SUCCESS_FOR_EACH_FLOAT_WITH_DEFAULT(function, value, def)     \
  BOOST_CHECK_CLOSE(function<float>      (#value, def), value, 1e-5); \
  BOOST_CHECK_CLOSE(function<double>     (#value, def), value, 1e-5); \
  BOOST_CHECK_CLOSE(function<long double>(#value, def), value, 1e-5);

#define FAIL_WITH_DEFAULT(test, function, sign, value, def) \
  test(function<sign char>     (#value, def) == def);       \
  test(function<sign short>    (#value, def) == def);       \
  test(function<sign int>      (#value, def) == def);       \
  test(function<sign long>     (#value, def) == def);       \
  test(function<sign long long>(#value, def) == def);

// Test positive int
BOOST_AUTO_TEST_CASE(test_convert_positive_with_to_with_default)
{
  SUCCESS_FOR_EACH_INTEGRAL_WITH_DEFAULT(BOOST_TEST, otb::to_with_default,         , 107, 42);
  SUCCESS_FOR_EACH_INTEGRAL_WITH_DEFAULT(BOOST_TEST, otb::to_with_default, unsigned, 107, 42);

  SUCCESS_FOR_EACH_FLOAT_WITH_DEFAULT(otb::to_with_default, 107, 4.2);
}

// Test negative int
BOOST_AUTO_TEST_CASE(test_convert_negative_with_to_with_default)
{
  SUCCESS_FOR_EACH_INTEGRAL_WITH_DEFAULT(BOOST_TEST, otb::to_with_default,         , -107, 42);
  BOOST_TEST(otb::to_with_default<unsigned>("-107", 42) == 42);
  FAIL_WITH_DEFAULT(BOOST_TEST, otb::to_with_default, unsigned, -107, 42);

  SUCCESS_FOR_EACH_FLOAT_WITH_DEFAULT(otb::to_with_default, -107, 4.2);
}

// Test user-defined type
BOOST_AUTO_TEST_CASE(test_convert_user_defined_type_with_to_with_default)
{
  BOOST_TEST(otb::to_with_default<MyType>("107", MyType{42}) == MyType{107});
  BOOST_TEST(otb::to_with_default<MyType>("10 7", MyType{42}) == MyType{42});
}

// Test insufficient precision
BOOST_AUTO_TEST_CASE(test_convert_not_narrow_with_to_with_default)
{
  BOOST_TEST(otb::to_with_default<short>("1000", 42) == 1000);

  BOOST_TEST(otb::to_with_default<signed char>("-128", 42) == -128);
  BOOST_TEST(otb::to_with_default<signed char>("127", 42) == 127);

  BOOST_CHECK(otb::to_with_default<signed char>("-129", 42) == 42);
  BOOST_CHECK(otb::to_with_default<signed char>("128", 42) == 42);

  BOOST_TEST(otb::to_with_default<signed short>("-32768", 42) == -32768);
  BOOST_TEST(otb::to_with_default<signed short>("32767", 42) == 32767);
  BOOST_CHECK(otb::to_with_default<signed short>("-32769", 42) == 42);
  BOOST_CHECK(otb::to_with_default<signed short>("32768", 42) == 42);

  BOOST_TEST(otb::to_with_default<unsigned char>("255", 42) == 255);
  BOOST_CHECK(otb::to_with_default<unsigned char>("256", 42) == 42);
  BOOST_TEST(otb::to_with_default<unsigned short>("65535", 42) == 65535);
  BOOST_CHECK(otb::to_with_default<unsigned short>("65536", 42) == 42);
}

// Test not numbers
BOOST_AUTO_TEST_CASE(test_invalid_numbers_with_to_with_default)
{
  BOOST_CHECK(otb::to_with_default<int>("ab1000", 42) == 42);
  BOOST_CHECK(otb::to_with_default<int>("1000ab", 42) == 42);
  BOOST_CHECK(otb::to_with_default<int>("10 00", 42) == 42);
  BOOST_CHECK(otb::to_with_default<long>("10 00", 42) == 42);
}

// Tests floats
BOOST_AUTO_TEST_CASE(test_floatting_point_numbers_with_to_with_default)
{
  SUCCESS_FOR_EACH_FLOAT_WITH_DEFAULT(otb::to_with_default, 10.7, 4.2);
  SUCCESS_FOR_EACH_FLOAT_WITH_DEFAULT(otb::to_with_default, -10.7, 4.2);
  SUCCESS_FOR_EACH_FLOAT_WITH_DEFAULT(otb::to_with_default, -1.7e5, 4.2);
  SUCCESS_FOR_EACH_FLOAT_WITH_DEFAULT(otb::to_with_default, 1.7e+5, 4.2);

  BOOST_TEST((std::isnan(otb::to_with_default<float>      ("nan", 4.2))));
  BOOST_TEST((std::isnan(otb::to_with_default<double>     ("nan", 4.2))));
  BOOST_TEST((std::isnan(otb::to_with_default<long double>("nan", 4.2))));
}

/*-------------------------------[ Test OnDecodingError ]--------------------*/
