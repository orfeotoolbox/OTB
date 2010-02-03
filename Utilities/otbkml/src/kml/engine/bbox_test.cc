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

// This file contains the unit tests for the Bbox class.

#include "kml/engine/bbox.h"
#include "kml/base/unit_test.h"

namespace kmlengine {

class BboxTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(BboxTest);
  CPPUNIT_TEST(TestDefault);
  CPPUNIT_TEST(TestCopy);
  CPPUNIT_TEST(TestBasic);
  CPPUNIT_TEST(TestConstructor);
  CPPUNIT_TEST(TestMultiple);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestDefault();
  void TestCopy();
  void TestBasic();
  void TestConstructor();
  void TestMultiple();

 private:
  void VerifyBounds(const Bbox& bbox, double north, double south, double east,
                    double west);
};

CPPUNIT_TEST_SUITE_REGISTRATION(BboxTest);

// Verify the state of a default Bbox.
void BboxTest::TestDefault() {
  Bbox default_bbox;
  CPPUNIT_ASSERT_EQUAL(kMinLat, default_bbox.get_north());
  CPPUNIT_ASSERT_EQUAL(kMaxLat, default_bbox.get_south());
  CPPUNIT_ASSERT_EQUAL(kMinLon, default_bbox.get_east());
  CPPUNIT_ASSERT_EQUAL(kMaxLon, default_bbox.get_west());
}


void BboxTest::VerifyBounds(const Bbox& bbox, double north, double south,
                            double east, double west) {

  // Verify the getters.
  CPPUNIT_ASSERT_EQUAL(north, bbox.get_north());
  CPPUNIT_ASSERT_EQUAL(south, bbox.get_south());
  CPPUNIT_ASSERT_EQUAL(east, bbox.get_east());
  CPPUNIT_ASSERT_EQUAL(west, bbox.get_west());

  // Verify GetCenter().
  double mid_lat, mid_lon;
  bbox.GetCenter(&mid_lat, &mid_lon);
  CPPUNIT_ASSERT_EQUAL((north + south)/2.0, mid_lat);
  CPPUNIT_ASSERT_EQUAL((east + west)/2.0, mid_lon);

  // Verify Contains().
  CPPUNIT_ASSERT(bbox.Contains(mid_lat, mid_lon));
  CPPUNIT_ASSERT(bbox.Contains(north, east));
  CPPUNIT_ASSERT(bbox.Contains(south, east));
  CPPUNIT_ASSERT(bbox.Contains(south, west));
  CPPUNIT_ASSERT(bbox.Contains(north, west));
}

// Verify copy construction and assignment.
void BboxTest::TestCopy() {
  const double kNorth = 2;
  const double kSouth = 1;
  const double kEast = 4;
  const double kWest = 3;
  // Create a Bbox to copy.
  Bbox a(kNorth, kSouth, kEast, kWest);

  // Create a Bbox using the copy constructor.
  Bbox b(a);
  VerifyBounds(b, kNorth, kSouth, kEast, kWest);

  // Create a Bbox and assign to it.
  Bbox c;
  c = a;
  VerifyBounds(c, kNorth, kSouth, kEast, kWest);
}

// Verify some basic usage.
void BboxTest::TestBasic() {
  const double kNorth = 45.45;
  const double kSouth = -12.12;
  const double kEast = 123.123;
  const double kWest = -89.89;

  Bbox bbox;

  // Two points is sufficient to define a box.
  bbox.ExpandLatLon(kNorth, kEast);
  bbox.ExpandLatLon(kSouth, kWest);

  VerifyBounds(bbox, kNorth, kSouth, kEast, kWest);
}

// Verify the constructor.
void BboxTest::TestConstructor() {
  const double kNorth = 89.123;
  const double kSouth = -2.222;
  const double kEast = -88.888;
  const double kWest = -154.6789;
  Bbox bbox(kNorth, kSouth, kEast, kWest);
  VerifyBounds(bbox, kNorth, kSouth, kEast, kWest);
}

// Verify bounding box of multiple points.
void BboxTest::TestMultiple() {
  const struct {
    double lat, lon;
  } kPoints[] = {
    { 46.3941,10.1168 },
    { 46.6356,8.84678 },
    { 46.69,8.95711 },
    { 46.158,8.97531 },
    { 46.1719,8.79744 },
    { 46.1217,8.35152 },
    { 46.62,8.5706 },
    { 46.7067,8.953 },
    { 46.6087,8.82036 },
    { 46.1546,8.9633 },
    { 46.2368,10.1363 },
    { 46.7079,9.19907 },
    { 45.9296,8.92094 },
    { 46.1738,8.84359 },
    { 46.5616,8.34504 },
    { 46.7389,8.97314 },
    { 46.7493,8.23686 },
    { 46.7233,8.92272 },
    { 45.9528,8.95471 }
  };
  const double kNorthExpected = 46.7493;
  const double kSouthExpected = 45.9296;
  const double kEastExpected = 10.1363;
  const double kWestExpected = 8.23686;

  Bbox bbox;
  for (size_t i = 0; i < sizeof(kPoints)/sizeof(kPoints[0]); ++i) {
    bbox.ExpandLatLon(kPoints[i].lat, kPoints[i].lon);
  }

  CPPUNIT_ASSERT_EQUAL(kNorthExpected, bbox.get_north());
  CPPUNIT_ASSERT_EQUAL(kSouthExpected, bbox.get_south());
  CPPUNIT_ASSERT_EQUAL(kEastExpected, bbox.get_east());
  CPPUNIT_ASSERT_EQUAL(kWestExpected, bbox.get_west());

  double mid_lat, mid_lon;
  bbox.GetCenter(&mid_lat, &mid_lon);
  CPPUNIT_ASSERT_EQUAL((kNorthExpected + kSouthExpected)/2.0, mid_lat);
  CPPUNIT_ASSERT_EQUAL((kEastExpected + kWestExpected)/2.0, mid_lon);

  for (size_t i = 0; i < sizeof(kPoints)/sizeof(kPoints[0]); ++i) {
    CPPUNIT_ASSERT(bbox.Contains(kPoints[i].lat, kPoints[i].lon));
  }

  VerifyBounds(bbox, kNorthExpected, kSouthExpected, kEastExpected,
               kWestExpected);
}

}  // end namespace kmlengine

TEST_MAIN
