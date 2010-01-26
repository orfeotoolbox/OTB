// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This file contains the unit tests for the KML convenience functions.

#include "kml/convenience/convenience.h"
#include "kml/base/unit_test.h"

using kmldom::CoordinatesPtr;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::RegionPtr;
using kmldom::Vec3;

namespace kmlconvenience {

class ConvenienceTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ConvenienceTest);
  CPPUNIT_TEST(TestCreatePointPlacemark);
  CPPUNIT_TEST(TestCreateRegion2d);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestCreatePointPlacemark();
  void TestCreateRegion2d();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ConvenienceTest);

// This tests the CreatePointPlacemark() function.
void ConvenienceTest::TestCreatePointPlacemark() {
  const std::string kName("my point placemark");
  const double kLat = 38.0987123;
  const double kLon = -123.123;
  PlacemarkPtr placemark = CreatePointPlacemark(kName, kLat, kLon);
  CPPUNIT_ASSERT_EQUAL(kName, placemark->get_name());
  CPPUNIT_ASSERT(placemark->has_geometry());
  PointPtr point = kmldom::AsPoint(placemark->get_geometry());
  CPPUNIT_ASSERT(point);
  CPPUNIT_ASSERT(point->has_coordinates());
  CoordinatesPtr coordinates = point->get_coordinates();
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1),
                       coordinates->get_coordinates_array_size());
  Vec3 vec = coordinates->get_coordinates_array_at(0);
  CPPUNIT_ASSERT_EQUAL(kLat, vec.get_latitude());
  CPPUNIT_ASSERT_EQUAL(kLon, vec.get_longitude());
}

// This tests the CreateRegion2d() function.
void ConvenienceTest::TestCreateRegion2d() {
  const double kNorth(67.87);
  const double kSouth(-56.78);
  const double kEast(98.12);
  const double kWest(12.34);
  const double kMinlodpixels(128);
  const double kMaxlodpixels(512);
  RegionPtr region = CreateRegion2d(kNorth, kSouth, kEast, kWest,
                                    kMinlodpixels, kMaxlodpixels);
  CPPUNIT_ASSERT(region->has_latlonaltbox());
  CPPUNIT_ASSERT(region->has_lod());
  CPPUNIT_ASSERT_EQUAL(kNorth, region->get_latlonaltbox()->get_north());
  CPPUNIT_ASSERT_EQUAL(kSouth, region->get_latlonaltbox()->get_south());
  CPPUNIT_ASSERT_EQUAL(kEast, region->get_latlonaltbox()->get_east());
  CPPUNIT_ASSERT_EQUAL(kWest, region->get_latlonaltbox()->get_west());
  CPPUNIT_ASSERT(!region->get_latlonaltbox()->has_minaltitude());
  CPPUNIT_ASSERT(!region->get_latlonaltbox()->has_maxaltitude());
  CPPUNIT_ASSERT(!region->get_latlonaltbox()->has_altitudemode());
  CPPUNIT_ASSERT_EQUAL(kMinlodpixels, region->get_lod()->get_minlodpixels());
  CPPUNIT_ASSERT_EQUAL(kMaxlodpixels, region->get_lod()->get_maxlodpixels());
  CPPUNIT_ASSERT(!region->get_lod()->has_minfadeextent());
  CPPUNIT_ASSERT(!region->get_lod()->has_maxfadeextent());
}

}  // end namespace kmlconvenience

TEST_MAIN

