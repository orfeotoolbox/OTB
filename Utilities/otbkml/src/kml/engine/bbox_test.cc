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
#include "gtest/gtest.h"

namespace kmlengine {

class BboxTest : public testing::Test {
 protected:
  void VerifyBounds(const Bbox& bbox, double north, double south, double east,
                    double west);
};

// Verify the state of a default Bbox.
TEST_F(BboxTest, TestDefault) {
  Bbox default_bbox;
  ASSERT_EQ(kMinLat, default_bbox.get_north());
  ASSERT_EQ(kMaxLat, default_bbox.get_south());
  ASSERT_EQ(kMinLon, default_bbox.get_east());
  ASSERT_EQ(kMaxLon, default_bbox.get_west());
  ASSERT_EQ(0, default_bbox.GetCenterLat());
  ASSERT_EQ(0, default_bbox.GetCenterLon());
}

void BboxTest::VerifyBounds(const Bbox& bbox, double north, double south,
                            double east, double west) {
  // Verify the getters.
  ASSERT_EQ(north, bbox.get_north());
  ASSERT_EQ(south, bbox.get_south());
  ASSERT_EQ(east, bbox.get_east());
  ASSERT_EQ(west, bbox.get_west());

  const double kWantLat((north + south)/2.0);
  const double kWantLon((east + west)/2.0);

  // Verify GetCenter().
  double mid_lat, mid_lon;
  bbox.GetCenter(&mid_lat, &mid_lon);
  ASSERT_EQ(kWantLat, mid_lat);
  ASSERT_EQ(kWantLon, mid_lon);

  // Verify GetCenterLat,Lon().
  ASSERT_EQ(kWantLat, bbox.GetCenterLat());
  ASSERT_EQ(kWantLon, bbox.GetCenterLon());

  // Verify Contains().
  ASSERT_TRUE(bbox.Contains(mid_lat, mid_lon));
  ASSERT_TRUE(bbox.Contains(north, east));
  ASSERT_TRUE(bbox.Contains(south, east));
  ASSERT_TRUE(bbox.Contains(south, west));
  ASSERT_TRUE(bbox.Contains(north, west));
}

// Verify copy construction and assignment.
TEST_F(BboxTest, TestCopy) {
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
TEST_F(BboxTest, TestBasic) {
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
TEST_F(BboxTest, TestConstructor) {
  const double kNorth = 89.123;
  const double kSouth = -2.222;
  const double kEast = -88.888;
  const double kWest = -154.6789;
  Bbox bbox(kNorth, kSouth, kEast, kWest);
  VerifyBounds(bbox, kNorth, kSouth, kEast, kWest);
}

// Verify bounding box of multiple points.
TEST_F(BboxTest, TestMultiple) {
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

  ASSERT_EQ(kNorthExpected, bbox.get_north());
  ASSERT_EQ(kSouthExpected, bbox.get_south());
  ASSERT_EQ(kEastExpected, bbox.get_east());
  ASSERT_EQ(kWestExpected, bbox.get_west());

  const double kLatExpected = (kNorthExpected + kSouthExpected)/2.0;
  const double kLonExpected = (kEastExpected + kWestExpected)/2.0;
  double mid_lat, mid_lon;
  bbox.GetCenter(&mid_lat, &mid_lon);
  ASSERT_EQ(kLatExpected, mid_lat);
  ASSERT_EQ(kLonExpected, mid_lon);
  ASSERT_EQ(kLatExpected, bbox.GetCenterLat());
  ASSERT_EQ(kLonExpected, bbox.GetCenterLon());

  for (size_t i = 0; i < sizeof(kPoints)/sizeof(kPoints[0]); ++i) {
    ASSERT_TRUE(bbox.Contains(kPoints[i].lat, kPoints[i].lon));
  }

  VerifyBounds(bbox, kNorthExpected, kSouthExpected, kEastExpected,
               kWestExpected);
}

TEST_F(BboxTest, TestExpandFromBbox) {
  const double kNorth = 89.123;
  const double kSouth = -2.222;
  const double kEast = -88.888;
  const double kWest = -154.6789;
  Bbox bbox(kNorth, kSouth, kEast, kWest);
  Bbox another_bbox;
  another_bbox.ExpandFromBbox(bbox);
  VerifyBounds(another_bbox, kNorth, kSouth, kEast, kWest);
}

TEST_F(BboxTest, TestSetNSEW) {
  Bbox b;
  b.set_north(37.786807);
  ASSERT_EQ(37.786807, b.get_north());
  b.set_south(37.781563);
  ASSERT_EQ(37.781563, b.get_south());
  b.set_east(-122.494135);
  ASSERT_EQ(-122.494135, b.get_east());
  b.set_west(-122.504031);
  ASSERT_EQ(-122.504031, b.get_west());
}

TEST_F(BboxTest, TestAlignBbox) {
  Bbox b;
  b.set_north(37.786807);  // Lincoln Park 3
  b.set_south(37.781563);  // Lincoln Park 7
  b.set_east(-122.494135);  // Lincoln Park 18
  b.set_west(-122.504031);  // Lincoln Park 5
  Bbox qt(180, -180, 180, -180);
  b.AlignBbox(&qt, 24);
  ASSERT_EQ(37.79296875, qt.get_north());
  ASSERT_EQ(37.7490234375, qt.get_south());
  ASSERT_EQ(-122.4755859375, qt.get_east());
  ASSERT_EQ(-122.51953125, qt.get_west());
}

TEST_F(BboxTest, TestContainedByBox) {
  Bbox r(180, -180, 180, -180);
  Bbox a(1,-1,1,-1);
  ASSERT_TRUE(a.ContainedByBox(180, -180, 180, -180));
  ASSERT_TRUE(a.ContainedByBbox(r));
  Bbox b(1000,-1,1,-1);
  ASSERT_FALSE(b.ContainedByBox(180, -180, 180, -180));
  ASSERT_FALSE(b.ContainedByBbox(r));
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
