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

#include "boost/test/tools/old/interface.hpp"
#include "otbXmlWrapper.h"

#define BOOST_TEST_MODULE "otb::xml wrappers unit testing"
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

auto check_exception(char const* expected_msg, bool debug = false)
{
  return [=](std::exception const& e)
  {
    if (debug)
    {
      std::cout << "strcmp = " << std::strcmp(e.what(), expected_msg) << " between:\n"
        << "- " << e.what() << "\n' " << expected_msg << "\n";
    }
    bool ok = std::strcmp(e.what(), expected_msg) == 0;
    if (!ok)
    {
      std::cerr << "Caught exception message mismatches expected text:"
        << "\n - Expected: " << expected_msg
        << "\n - Caught:   " << e.what()
        << "\n";
    }
    return ok;
  };
}

constexpr double const ref_X[] = {-253469.255758, -277640.397067};
constexpr double const ref_Z[] = {6555653.058485, 6581146.392877};

using namespace otb::xml;

auto create_document()
{
  static constexpr char const* xmltxt =
  "<Earth_Explorer_File>"
  "  <Variable_Header>"
  "    <Ref_Frame>EARTH_FIXED</Ref_Frame>"
  "  </Variable_Header>"
  "  <Data_Block type='xml'>"
  "    <List_of_OSVs count='2'>"
  "      <OSV>"
  "        <TAI>TAI=2023-12-31T23:00:19.000000</TAI>"
  "        <UTC>UTC=2023-12-31T22:59:42.000000</UTC>"
  "        <UT1>UT1=2023-12-31T22:59:42.008796</UT1>"
  "        <Absolute_Orbit>+51905</Absolute_Orbit>"
  "        <X unit='m'>-253469.255758</X>"
  "        <Y unit='m'>-2625698.144393</Y>"
  "        <Z unit='m'>6555653.058485</Z>"
  "        <VX unit='m/s'>-2423.475743</VX>"
  "        <VY unit='m/s'>6705.522196</VY>"
  "        <VZ unit='m/s'>2586.343409</VZ>"
  "        <Quality>NOMINAL</Quality>"
  "      </OSV>"
  "      <OSV>"
  "        <TAI>TAI=2023-12-31T23:00:29.000000</TAI>"
  "        <UTC>UTC=2023-12-31T22:59:52.000000</UTC>"
  "        <UT1>UT1=2023-12-31T22:59:52.008796</UT1>"
  "        <Absolute_Orbit>+51905</Absolute_Orbit>"
  "        <X unit='m'>-277640.397067</X>"
  "        <Y unit='m'>-2558479.575455</Y>"
  "        <Z unit='m'>6581146.392877</Z>"
  "        <VX unit='m/s'>-2410.699492</VX>"
  "        <VY unit='m/s'>6738.062994</VY>"
  "        <VZ unit='m/s'>2512.275827</VZ>"
  "        <Quality>NOMINAL</Quality>"
  "      </OSV>"
  "    </List_of_OSVs>"
  "  </Data_Block>"
  "</Earth_Explorer_File>"
  ;
  std::istringstream iss(xmltxt);

  Document xmldoc;
  iss >> xmldoc;

  return xmldoc;
}

/*===============================[ Tests ]===================================*/

BOOST_AUTO_TEST_CASE(FindNode1)
{
  auto xmldoc = create_document();

  auto eef = xmldoc.find("Earth_Explorer_File");
  BOOST_REQUIRE(eef);
}

BOOST_AUTO_TEST_CASE(DontFindNode1)
{
  auto xmldoc = create_document();

  char const* ref_error =  "No <nope> node found in '<stream>'";
  BOOST_REQUIRE_EXCEPTION(
      (void)xmldoc.find("nope"),
      std::runtime_error,
      check_exception(ref_error)
  );
}

BOOST_AUTO_TEST_CASE(FindNode2)
{
  auto xmldoc = create_document();

  auto osvs = xmldoc.find("Earth_Explorer_File", "Data_Block", "List_of_OSVs");
  BOOST_REQUIRE(osvs);
}

BOOST_AUTO_TEST_CASE(DontFindNode2)
{
  auto xmldoc = create_document();

  char const* ref_error =  "No <Earth_Explorer_File/Data_Block/nop> node found in '<stream>'";
  BOOST_REQUIRE_EXCEPTION(
      (void)xmldoc.find("Earth_Explorer_File", "Data_Block", "nop"),
      std::runtime_error,
      check_exception(ref_error)
  );
}

BOOST_AUTO_TEST_CASE(GetTextNode2)
{
  auto xmldoc = create_document();

  auto frame1 = xmldoc.find("Earth_Explorer_File", "Variable_Header", "Ref_Frame").text();
  BOOST_REQUIRE_EQUAL(frame1, "EARTH_FIXED");

  auto header = xmldoc.find("Earth_Explorer_File", "Variable_Header");
  auto frame2 = header.get("Ref_Frame");
  BOOST_REQUIRE_EQUAL(frame2, "EARTH_FIXED");
}

BOOST_AUTO_TEST_CASE(GetDoubleNode2)
{
  auto xmldoc = create_document();

  auto x = xmldoc.find("Earth_Explorer_File", "Data_Block", "List_of_OSVs", "OSV", "X").as<double>();
  BOOST_REQUIRE_CLOSE(x, ref_X[0], 1e-15);
}

BOOST_AUTO_TEST_CASE(BadGetDoubleNode2)
{
  auto xmldoc = create_document();

  char const* ref_error =
    "Cannot decode TAI=2023-12-31T23:00:19.000000 as float value while "
    "extracting <Earth_Explorer_File/Data_Block/List_of_OSVs/OSV/TAI> from '<stream>'";
  BOOST_REQUIRE_EXCEPTION(
      (void)xmldoc.find("Earth_Explorer_File", "Data_Block", "List_of_OSVs", "OSV", "TAI").as<double>(),
      std::runtime_error,
      check_exception(ref_error)
  );
}

BOOST_AUTO_TEST_CASE(IterateNodes)
{
  auto xmldoc = create_document();

  auto osvs = xmldoc.find("Earth_Explorer_File", "Data_Block", "List_of_OSVs");
  BOOST_REQUIRE(osvs);
  BOOST_REQUIRE_EQUAL("2", osvs->Attribute("count"));

  auto nb = 0u;
  for (auto && osv : osvs)
  {
    auto x = osv.find("X").as<double>();
    // auto z = osv.find("Z").as<double>();
    // auto x = osv.get<double>("X");
    auto z = osv.get<double>("Z");

    BOOST_REQUIRE_CLOSE(x, ref_X[nb], 1e-15);
    BOOST_REQUIRE_CLOSE(z, ref_Z[nb], 1e-15);

    ++nb;
  }
  BOOST_REQUIRE_EQUAL(nb, 2);
}
