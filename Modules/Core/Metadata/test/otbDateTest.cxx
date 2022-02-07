/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define BOOST_TEST_MODULE "SarSensorModel unit testing"
#define BOOST_TEST_DYN_LINK
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include <boost/test/unit_test.hpp>
#pragma GCC diagnostic pop
#else
#include <boost/test/unit_test.hpp>
#endif

#include "otbDateTime.h"
#include "otbMissingMetadataException.h"


BOOST_AUTO_TEST_CASE(Duration)
{
  BOOST_TEST(otb::MetaData::Duration::Seconds(5.5) + otb::MetaData::Duration::Seconds(1.5) 
                == otb::MetaData::Duration::Seconds(7));
  BOOST_TEST(otb::MetaData::Duration::Seconds(5.5) - otb::MetaData::Duration::Seconds(0.2) 
                == otb::MetaData::Duration::Seconds(5.3));
  BOOST_TEST(otb::MetaData::Duration::Seconds(5.5) * 3.5
                == otb::MetaData::Duration::Seconds(19.25));
  BOOST_TEST(otb::MetaData::Duration::Seconds(5.5) / 2 
                == otb::MetaData::Duration::Seconds(2.75));
  BOOST_TEST(otb::MetaData::Duration::Seconds(18) / otb::MetaData::Duration::Seconds(6) 
                == 3);
  BOOST_TEST(otb::MetaData::Duration::Seconds(5.5) + otb::MetaData::Duration::Nanoseconds(10) 
                == otb::MetaData::Duration::Seconds(5.50000001) );

  // Getter tests
  BOOST_TEST(otb::MetaData::Duration::Nanoseconds(10).TotalSeconds() == 10e-9);
  BOOST_TEST(otb::MetaData::Duration::Nanoseconds(10).NumberOfTicks() == 10);
}

BOOST_AUTO_TEST_CASE(TimePoint)
{
  auto date1 = otb::MetaData::ReadFormattedDate("2021-06-22T00:01:04.42578987");

  // Test date part getters
  BOOST_TEST(date1.GetYear() == 2021);
  BOOST_TEST(date1.GetMonth() == 06);
  BOOST_TEST(date1.GetDay() == 22);
  BOOST_TEST(date1.GetHour() == 0);
  BOOST_TEST(date1.GetMinute() == 1);
  BOOST_TEST(date1.GetSecond() == 4.42578987);

  auto date2 = otb::MetaData::ReadFormattedDate("2021-06-21T00:01:04.42578987");

  BOOST_TEST(date1 > date2);
  BOOST_TEST(date1 - date2 == otb::MetaData::Duration::Seconds(86400));
  BOOST_TEST(date1 + otb::MetaData::Duration::Seconds(0.1) == 
    otb::MetaData::ReadFormattedDate("2021-06-22T00:01:04.52578987"));

  // Test parsing with a UTC time identifier
  BOOST_TEST(otb::MetaData::ReadFormattedDate("2021-06-22T00:01:04.42Z") 
              == otb::MetaData::ReadFormattedDate("2021-06-22T00:01:04.42"));

  // Test Parsing with a different format
  otb::MetaData::ReadFormattedDate("2021-06-22 00:01:04.42578987", "%Y-%m-%d %H:%M:%S");
  
  // Test some invalid cases
  BOOST_REQUIRE_THROW(otb::MetaData::ReadFormattedDate("2021-06-21 00:01:04.42578987"),
                      otb::MissingMetadataException);

  BOOST_REQUIRE_THROW(otb::MetaData::ReadFormattedDate("2021-06-21"),
                      otb::MissingMetadataException);

  BOOST_REQUIRE_THROW(otb::MetaData::ReadFormattedDate("2021-6-21 00:1:4.42578987"),
                      otb::MissingMetadataException);

  // Julian day and Modified julian day
  auto mjdOrigin = otb::MetaData::ReadFormattedDate("1858-11-17T00:00:00");

  BOOST_TEST(mjdOrigin.GetJulianDay() == 2400000.5);
  BOOST_TEST(mjdOrigin.GetModifiedJulianDay() == 0.);

  // Serialization test
  std::ostringstream iss;
  iss << date1;
  BOOST_TEST(otb::MetaData::ReadFormattedDate(iss.str()) == date1);
}

BOOST_AUTO_TEST_CASE(TimePointPrecision)
{
  auto date = otb::MetaData::ReadFormattedDate("2021-06-22T00:01:02");
  auto dateRef = date;

  // Number of seconds in a day
  const int N = 86400;
  const double step = 0.1;

  for (int i = 0; i < N/step; i++)
  {
    date += otb::MetaData::Duration::Seconds(step);
  }

  BOOST_TEST(date - dateRef ==  otb::MetaData::Duration::Seconds(N));
}