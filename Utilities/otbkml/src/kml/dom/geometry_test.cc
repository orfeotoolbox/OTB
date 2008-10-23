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

// This file contains the unit tests for Vec3, Coordinates, Geometry,
// Point, LineString, LinearRing, Polygon, InnerBoundaryIs, OuterBoundaryIs,
// and MultiGeometry.

#include "kml/dom/geometry.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/base/unit_test.h"

namespace kmldom {

// Test Vec3.
class Vec3Test : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(Vec3Test);
  CPPUNIT_TEST(TestConstructor);
  CPPUNIT_TEST(TestSerializer);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestConstructor();
  void TestSerializer();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Vec3Test);

void Vec3Test::TestConstructor() {
  // Construct a Vec3 with initial arguments:
  const double longitude = -122.345;
  const double latitude =  37.123;
  const double altitude =  101.202;
  Vec3 vec3(longitude, latitude, altitude);

  // Verify constructor set the right fields:
  CPPUNIT_ASSERT(longitude == vec3.get_longitude());
  CPPUNIT_ASSERT(latitude == vec3.get_latitude());
  CPPUNIT_ASSERT(altitude == vec3.get_altitude());

  // Construct a Vec3 with no initializers:
  Vec3 vec0;

  // Verify all fields are zero:
  CPPUNIT_ASSERT(0 == vec0.get_longitude());
  CPPUNIT_ASSERT(0 == vec0.get_latitude());
  CPPUNIT_ASSERT(0 == vec0.get_altitude());
}

void Vec3Test::TestSerializer() {
}

// Test Coordinates.
class CoordinatesTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(CoordinatesTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestAddLatLng);
  CPPUNIT_TEST(TestAddLatLngAlt);
  CPPUNIT_TEST(TestAddLatLngAltMany);
  CPPUNIT_TEST(TestParseVec3);
  CPPUNIT_TEST(TestParsePoint);
  CPPUNIT_TEST(TestParseLine);
  CPPUNIT_TEST(TestParseBadSeparator);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    coordinates_ = KmlFactory::GetFactory()->CreateCoordinates();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestAddLatLng();
  void TestAddLatLngAlt();
  void TestAddLatLngAltMany();
  void TestParseVec3();
  void TestParsePoint();
  void TestParseLine();
  void TestParseBadSeparator();

 private:
  CoordinatesPtr coordinates_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CoordinatesTest);

void CoordinatesTest::TestType() {
  CPPUNIT_ASSERT(Type_coordinates == coordinates_->Type());
  CPPUNIT_ASSERT(true == coordinates_->IsA(Type_coordinates));
}

// Verify proper defaults:
void CoordinatesTest::TestDefaults() {
  CPPUNIT_ASSERT(0 == coordinates_->get_coordinates_array_size());
}

// Verify the add_latlng() setter.
void CoordinatesTest::TestAddLatLng() {
  const double kLat(-22.22);
  const double kLon(44.44);
  coordinates_->add_latlng(kLat, kLon);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1),
                       coordinates_->get_coordinates_array_size());
  Vec3 vec3 = coordinates_->get_coordinates_array_at(0);
  CPPUNIT_ASSERT_EQUAL(kLat, vec3.get_latitude());
  CPPUNIT_ASSERT_EQUAL(kLon, vec3.get_longitude());
  CPPUNIT_ASSERT_EQUAL(0.0, vec3.get_altitude());
}

// Verify the add_latlngalt() setter.
void CoordinatesTest::TestAddLatLngAlt() {
  const double kLat(-22.22);
  const double kLon(44.44);
  const double kAlt(10001.2002);
  coordinates_->add_latlngalt(kLat, kLon, kAlt);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1),
                       coordinates_->get_coordinates_array_size());
  Vec3 vec3 = coordinates_->get_coordinates_array_at(0);
  CPPUNIT_ASSERT_EQUAL(kLat, vec3.get_latitude());
  CPPUNIT_ASSERT_EQUAL(kLon, vec3.get_longitude());
  CPPUNIT_ASSERT_EQUAL(kAlt, vec3.get_altitude());
}

// Verify a bunch of points in a <coordinates> element.
void CoordinatesTest::TestAddLatLngAltMany() {
  const size_t kNumPoints(1001);
  size_t i;
  for (i = 0; i < kNumPoints; ++i) {
    coordinates_->add_latlngalt(static_cast<double>(i % 90),
                                static_cast<double>(i % 180),
                                static_cast<double>(i));
  }
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(kNumPoints),
                       coordinates_->get_coordinates_array_size());
  for (i = 0; i < kNumPoints; ++i) {
    Vec3 vec3 = coordinates_->get_coordinates_array_at(i);
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(i % 90), vec3.get_latitude());
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(i % 180), vec3.get_longitude());
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(i), vec3.get_altitude());
  }
}

void CoordinatesTest::TestParseVec3() {
  const char* basic_3d_point = "1.123,-2.789,3000.5919";
  char *endp;
  Vec3 vec;
  CPPUNIT_ASSERT(true == Coordinates::ParseVec3(basic_3d_point, &endp, &vec));
  CPPUNIT_ASSERT(-2.789 == vec.get_latitude());
  CPPUNIT_ASSERT(1.123 == vec.get_longitude());
  CPPUNIT_ASSERT(3000.5919 == vec.get_altitude());
  CPPUNIT_ASSERT(basic_3d_point + strlen(basic_3d_point) == endp);

  const char* basic_3d_line = "-122.123,38.789,1050.0987 "
                              "-122.123,39.789,1050.098";
  CPPUNIT_ASSERT(true == Coordinates::ParseVec3(basic_3d_line, &endp, &vec));
  CPPUNIT_ASSERT(38.789 == vec.get_latitude());
  CPPUNIT_ASSERT(-122.123 == vec.get_longitude());
  CPPUNIT_ASSERT(1050.0987 == vec.get_altitude());
  CPPUNIT_ASSERT(true == Coordinates::ParseVec3(endp, &endp, &vec));
  CPPUNIT_ASSERT(39.789 == vec.get_latitude());
  CPPUNIT_ASSERT(-122.123 == vec.get_longitude());
  CPPUNIT_ASSERT(1050.098 == vec.get_altitude());

  const char* basic_2d_point = "10.10,-20.20";
  CPPUNIT_ASSERT(true == Coordinates::ParseVec3(basic_2d_point, &endp, &vec));
  CPPUNIT_ASSERT(-20.20 == vec.get_latitude());
  CPPUNIT_ASSERT(10.10 == vec.get_longitude());
  CPPUNIT_ASSERT(0.0 == vec.get_altitude());

  const char* point2d_with_1space = "15.10, -24.20";
  CPPUNIT_ASSERT(true == Coordinates::ParseVec3(point2d_with_1space, &endp,
                                                &vec));
  CPPUNIT_ASSERT(-24.20 == vec.get_latitude());
  CPPUNIT_ASSERT(15.10 == vec.get_longitude());
  CPPUNIT_ASSERT(0.0 == vec.get_altitude());

  const char* point2d_with_2spaces = "15.11 , -24.25";
  CPPUNIT_ASSERT(true == Coordinates::ParseVec3(point2d_with_2spaces, &endp,
                                                &vec));
  CPPUNIT_ASSERT(-24.25 == vec.get_latitude());
  CPPUNIT_ASSERT(15.11 == vec.get_longitude());
  CPPUNIT_ASSERT(0.0 == vec.get_altitude());

  const char* basic_2d_line = "122.123,-38.789 "
                              "122.123,-39.789";
  CPPUNIT_ASSERT(true == Coordinates::ParseVec3(basic_2d_line, &endp, &vec));
  CPPUNIT_ASSERT(-38.789 == vec.get_latitude());
  CPPUNIT_ASSERT(122.123 == vec.get_longitude());
  CPPUNIT_ASSERT(0.0 == vec.get_altitude());
  CPPUNIT_ASSERT(true == Coordinates::ParseVec3(endp, &endp, &vec));
  CPPUNIT_ASSERT(-39.789 == vec.get_latitude());
  CPPUNIT_ASSERT(122.123 == vec.get_longitude());
  CPPUNIT_ASSERT(0.0 == vec.get_altitude());

  // How our own serializer emits <coordinates>
  const char* line_with_newlines = "\n"
                                   "  -160.073803556017,22.0041773078075\n"
                                   "  -160.121962433575,21.9639787234984\n"
                                   "  -160.22633646805,21.8915919620539\n"
                                   "  ";
  CPPUNIT_ASSERT(true == Coordinates::ParseVec3(line_with_newlines, &endp,
                                                &vec));

  const char* exponential_2d_pt = "1E-02, 2E-02";  // 0.01, 0.02
  CPPUNIT_ASSERT(true == Coordinates::ParseVec3(exponential_2d_pt, &endp,
                                                &vec));
  CPPUNIT_ASSERT(0.02 == vec.get_latitude());
  CPPUNIT_ASSERT(0.01 == vec.get_longitude());
  CPPUNIT_ASSERT(0.0 == vec.get_altitude());

  // Ensure junk data is handled gracefully.
  const char* junk_coords1 = "this will not parse correctly";
  CPPUNIT_ASSERT(false== Coordinates::ParseVec3(junk_coords1, &endp, &vec));

  const char* junk_coords2 = "0,foo";  // Will parse successfully.
  CPPUNIT_ASSERT(true == Coordinates::ParseVec3(junk_coords2, &endp, &vec));
  CPPUNIT_ASSERT(0.0 == vec.get_latitude());
  CPPUNIT_ASSERT(0.0 == vec.get_longitude());
  CPPUNIT_ASSERT(0.0 == vec.get_altitude());

  const char* junk_coords3 = "bar,0";  // Will parse successfully.
  CPPUNIT_ASSERT(true == Coordinates::ParseVec3(junk_coords3, &endp, &vec));
  CPPUNIT_ASSERT(0.0 == vec.get_latitude());
  CPPUNIT_ASSERT(0.0 == vec.get_longitude());
  CPPUNIT_ASSERT(0.0 == vec.get_altitude());

  const char* junk_coords4 = "\n";  // Will fail parsing.
  CPPUNIT_ASSERT(false == Coordinates::ParseVec3(junk_coords4, &endp, &vec));
}

void CoordinatesTest::TestParsePoint() {
  // Parser gathers character data and sets element
  coordinates_->Parse("1.1,-2.2,3.3");
  CPPUNIT_ASSERT(1 == coordinates_->get_coordinates_array_size());
  Vec3 vec = coordinates_->get_coordinates_array_at(0);
  CPPUNIT_ASSERT(-2.2 == vec.get_latitude());
  CPPUNIT_ASSERT(1.1 == vec.get_longitude());
  CPPUNIT_ASSERT(3.3 == vec.get_altitude());
}

void CoordinatesTest::TestParseLine() {
  // Parser gathers character data and sets element
  coordinates_->Parse("1.1,-2.2,3.3 -5.1,32.9872,10000.3");
  CPPUNIT_ASSERT(2 == coordinates_->get_coordinates_array_size());
  CPPUNIT_ASSERT(-2.2 ==
                 coordinates_->get_coordinates_array_at(0).get_latitude());
  CPPUNIT_ASSERT(1.1 ==
                 coordinates_->get_coordinates_array_at(0).get_longitude());
  CPPUNIT_ASSERT(3.3 ==
                 coordinates_->get_coordinates_array_at(0).get_altitude());
}

void CoordinatesTest::TestParseBadSeparator() {
  // Ensure graceful handling of bad data.
  coordinates_->Parse("130.999*66.56083,75");
  CPPUNIT_ASSERT(1 == coordinates_->get_coordinates_array_size());
  CPPUNIT_ASSERT(75 ==
                 coordinates_->get_coordinates_array_at(0).get_latitude());
  CPPUNIT_ASSERT(130.999 ==
                 coordinates_->get_coordinates_array_at(0).get_longitude());
  CPPUNIT_ASSERT(0 ==
                 coordinates_->get_coordinates_array_at(0).get_altitude());
}

// Test Point.
class PointTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(PointTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    point_ = KmlFactory::GetFactory()->CreatePoint();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  PointPtr point_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(PointTest);

void PointTest::TestType() {
  CPPUNIT_ASSERT(Type_Point == point_->Type());
  CPPUNIT_ASSERT(true == point_->IsA(Type_Point));
  CPPUNIT_ASSERT(true == point_->IsA(Type_Geometry));
  CPPUNIT_ASSERT(true == point_->IsA(Type_Object));
}

// Verify proper defaults:
void PointTest::TestDefaults() {
  CPPUNIT_ASSERT(false == point_->has_id());
  CPPUNIT_ASSERT(false == point_->has_targetid());
  CPPUNIT_ASSERT(false == point_->has_extrude());
  CPPUNIT_ASSERT(false == point_->get_extrude());
  CPPUNIT_ASSERT(false == point_->has_altitudemode());
  CPPUNIT_ASSERT(ALTITUDEMODE_CLAMPTOGROUND == point_->get_altitudemode());
  CPPUNIT_ASSERT(false == point_->has_coordinates());
  CPPUNIT_ASSERT(NULL == point_->get_coordinates());
}

// Verify setting default makes has_xxx() true:
void PointTest::TestSetToDefaultValues() {
  TestDefaults();
  point_->set_extrude(point_->get_extrude());
  point_->set_altitudemode(point_->get_altitudemode());
  point_->set_coordinates(NULL);  // should not crash
  CPPUNIT_ASSERT(true == point_->has_extrude());
  CPPUNIT_ASSERT(true == point_->has_altitudemode());
  CPPUNIT_ASSERT(false == point_->has_coordinates());  // ptr is null
}

// Verify set, get, has, clear:
void PointTest::TestSetGetHasClear() {
  // Non-default values:
  bool extrude = true;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_RELATIVETOGROUND;
  CoordinatesPtr coordinates = KmlFactory::GetFactory()->CreateCoordinates();
  const char* id = "point-id";
  const char* targetid = "point-targetid";

  // Set all fields:
  point_->set_id(id);
  point_->set_targetid(targetid);
  point_->set_extrude(extrude);
  point_->set_altitudemode(altitudemode);
  point_->set_coordinates(coordinates);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == point_->has_id());
  CPPUNIT_ASSERT(id == point_->get_id());
  CPPUNIT_ASSERT(true == point_->has_targetid());
  CPPUNIT_ASSERT(targetid == point_->get_targetid());
  CPPUNIT_ASSERT(true == point_->has_extrude());
  CPPUNIT_ASSERT(extrude == point_->get_extrude());
  CPPUNIT_ASSERT(true == point_->has_altitudemode());
  CPPUNIT_ASSERT(altitudemode == point_->get_altitudemode());
  CPPUNIT_ASSERT(true == point_->has_coordinates());
  CPPUNIT_ASSERT(coordinates == point_->get_coordinates());

  // Clear all fields:
  point_->clear_id();
  point_->clear_targetid();
  point_->clear_extrude();
  point_->clear_altitudemode();
  point_->clear_coordinates();

  // Verify now in default state:
  TestDefaults();
}

// Test LineString.
class LineStringTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(LineStringTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    linestring_ = KmlFactory::GetFactory()->CreateLineString();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  LineStringPtr linestring_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(LineStringTest);

void LineStringTest::TestType() {
  CPPUNIT_ASSERT(Type_LineString == linestring_->Type());
  CPPUNIT_ASSERT(true == linestring_->IsA(Type_LineString));
  CPPUNIT_ASSERT(true == linestring_->IsA(Type_Geometry));
  CPPUNIT_ASSERT(true == linestring_->IsA(Type_Object));
}

// Verify proper defaults:
void LineStringTest::TestDefaults() {
  CPPUNIT_ASSERT(false == linestring_->has_id());
  CPPUNIT_ASSERT(false == linestring_->has_targetid());
  CPPUNIT_ASSERT(false == linestring_->has_extrude());
  CPPUNIT_ASSERT(false == linestring_->get_extrude());
  CPPUNIT_ASSERT(false == linestring_->has_tessellate());
  CPPUNIT_ASSERT(false == linestring_->get_tessellate());
  CPPUNIT_ASSERT(false == linestring_->has_altitudemode());
  CPPUNIT_ASSERT(ALTITUDEMODE_CLAMPTOGROUND == linestring_->get_altitudemode());
  CPPUNIT_ASSERT(false == linestring_->has_coordinates());
}

// Verify setting default makes has_xxx() true:
void LineStringTest::TestSetToDefaultValues() {
  TestDefaults();
  linestring_->set_extrude(linestring_->get_extrude());
  linestring_->set_tessellate(linestring_->get_tessellate());
  linestring_->set_altitudemode(linestring_->get_altitudemode());
  linestring_->set_coordinates(NULL);  // should not crash
  CPPUNIT_ASSERT(true == linestring_->has_extrude());
  CPPUNIT_ASSERT(true == linestring_->has_tessellate());
  CPPUNIT_ASSERT(true == linestring_->has_altitudemode());
  CPPUNIT_ASSERT(false == linestring_->has_coordinates());  // ptr is null
}

// Verify set, get, has, clear:
void LineStringTest::TestSetGetHasClear() {
  // Non-default values:
  bool extrude = true;
  bool tessellate = true;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_RELATIVETOGROUND;
  CoordinatesPtr coordinates = KmlFactory::GetFactory()->CreateCoordinates();

  // Set all fields:
  linestring_->set_extrude(extrude);
  linestring_->set_tessellate(tessellate);
  linestring_->set_altitudemode(altitudemode);
  linestring_->set_coordinates(coordinates);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == linestring_->has_extrude());
  CPPUNIT_ASSERT(extrude == linestring_->get_extrude());
  CPPUNIT_ASSERT(true == linestring_->has_tessellate());
  CPPUNIT_ASSERT(tessellate == linestring_->get_tessellate());
  CPPUNIT_ASSERT(true == linestring_->has_altitudemode());
  CPPUNIT_ASSERT(altitudemode == linestring_->get_altitudemode());
  CPPUNIT_ASSERT(true == linestring_->has_coordinates());
  CPPUNIT_ASSERT(coordinates == linestring_->get_coordinates());

  // Clear all fields:
  linestring_->clear_extrude();
  linestring_->clear_tessellate();
  linestring_->clear_altitudemode();
  linestring_->clear_coordinates();

  // Verify now in default state:
  TestDefaults();
}

// Test LinearRing.
class LinearRingTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(LinearRingTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    linearring_ = KmlFactory::GetFactory()->CreateLinearRing();
  }

  // Called after each test.
  void tearDown() {
  }


 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  LinearRingPtr linearring_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(LinearRingTest);

void LinearRingTest::TestType() {
  CPPUNIT_ASSERT(Type_LinearRing == linearring_->Type());
  CPPUNIT_ASSERT(true == linearring_->IsA(Type_LinearRing));
  CPPUNIT_ASSERT(true == linearring_->IsA(Type_Geometry));
  CPPUNIT_ASSERT(true == linearring_->IsA(Type_Object));
}

// Verify proper defaults:
void LinearRingTest::TestDefaults() {
  CPPUNIT_ASSERT(false == linearring_->has_id());
  CPPUNIT_ASSERT(false == linearring_->has_targetid());
  CPPUNIT_ASSERT(false == linearring_->has_extrude());
  CPPUNIT_ASSERT(false == linearring_->get_extrude());
  CPPUNIT_ASSERT(false == linearring_->has_tessellate());
  CPPUNIT_ASSERT(false == linearring_->get_tessellate());
  CPPUNIT_ASSERT(false == linearring_->has_altitudemode());
  CPPUNIT_ASSERT(ALTITUDEMODE_CLAMPTOGROUND == linearring_->get_altitudemode());
  CPPUNIT_ASSERT(false == linearring_->has_coordinates());
}

// Verify setting default makes has_xxx() true:
void LinearRingTest::TestSetToDefaultValues() {
  TestDefaults();
  linearring_->set_extrude(linearring_->get_extrude());
  linearring_->set_tessellate(linearring_->get_tessellate());
  linearring_->set_altitudemode(linearring_->get_altitudemode());
  linearring_->set_coordinates(NULL);  // should not crash
  CPPUNIT_ASSERT(true == linearring_->has_extrude());
  CPPUNIT_ASSERT(true == linearring_->has_tessellate());
  CPPUNIT_ASSERT(true == linearring_->has_altitudemode());
  CPPUNIT_ASSERT(false == linearring_->has_coordinates());  // ptr is null
}

// Verify set, get, has, clear:
void LinearRingTest::TestSetGetHasClear() {
  // Non-default values:
  bool extrude = true;
  bool tessellate = true;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_RELATIVETOGROUND;
  CoordinatesPtr coordinates = KmlFactory::GetFactory()->CreateCoordinates();

  // Set all fields:
  linearring_->set_extrude(extrude);
  linearring_->set_tessellate(tessellate);
  linearring_->set_altitudemode(altitudemode);
  linearring_->set_coordinates(coordinates);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == linearring_->has_extrude());
  CPPUNIT_ASSERT(extrude == linearring_->get_extrude());
  CPPUNIT_ASSERT(true == linearring_->has_tessellate());
  CPPUNIT_ASSERT(tessellate == linearring_->get_tessellate());
  CPPUNIT_ASSERT(true == linearring_->has_altitudemode());
  CPPUNIT_ASSERT(altitudemode == linearring_->get_altitudemode());
  CPPUNIT_ASSERT(true == linearring_->has_coordinates());
  CPPUNIT_ASSERT(coordinates == linearring_->get_coordinates());

  // Clear all fields:
  linearring_->clear_extrude();
  linearring_->clear_tessellate();
  linearring_->clear_altitudemode();
  linearring_->clear_coordinates();

  // Verify now in default state:
  TestDefaults();
}

// Test OuterBoundaryIs.
class OuterBoundaryIsTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(OuterBoundaryIsTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    outerboundaryis_ = KmlFactory::GetFactory()->CreateOuterBoundaryIs();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetGetHasClear();

 private:
  OuterBoundaryIsPtr outerboundaryis_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(OuterBoundaryIsTest);

void OuterBoundaryIsTest::TestType() {
  CPPUNIT_ASSERT(Type_outerBoundaryIs == outerboundaryis_->Type());
  CPPUNIT_ASSERT(true == outerboundaryis_->IsA(Type_outerBoundaryIs));
}

// Verify proper defaults:
void OuterBoundaryIsTest::TestDefaults() {
  CPPUNIT_ASSERT(false == outerboundaryis_->has_linearring());
}

void OuterBoundaryIsTest::TestSetGetHasClear() {
  // Give it the only possible child:
  outerboundaryis_->set_linearring(
      KmlFactory::GetFactory()->CreateLinearRing());
  CPPUNIT_ASSERT(outerboundaryis_->has_linearring());
  CPPUNIT_ASSERT(outerboundaryis_->get_linearring());
  // Clear it and verify we're back to the default state.
  outerboundaryis_->clear_linearring();
  TestDefaults();
}

// Test InnerBoundaryIs.
class InnerBoundaryIsTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(InnerBoundaryIsTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    innerboundaryis_ = KmlFactory::GetFactory()->CreateInnerBoundaryIs();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetGetHasClear();

 private:
  InnerBoundaryIsPtr innerboundaryis_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(InnerBoundaryIsTest);

void InnerBoundaryIsTest::TestType() {
  CPPUNIT_ASSERT(Type_innerBoundaryIs == innerboundaryis_->Type());
  CPPUNIT_ASSERT(true == innerboundaryis_->IsA(Type_innerBoundaryIs));
}

// Verify proper defaults:
void InnerBoundaryIsTest::TestDefaults() {
  CPPUNIT_ASSERT(false == innerboundaryis_->has_linearring());
}

void InnerBoundaryIsTest::TestSetGetHasClear() {
  // Give it the only possible child:
  innerboundaryis_->set_linearring(
      KmlFactory::GetFactory()->CreateLinearRing());
  CPPUNIT_ASSERT(innerboundaryis_->has_linearring());
  CPPUNIT_ASSERT(innerboundaryis_->get_linearring());
  // Clear it and verify we're back to the default state.
  innerboundaryis_->clear_linearring();
  TestDefaults();
}

// TestPolygon
class PolygonTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(PolygonTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    polygon_ = KmlFactory::GetFactory()->CreatePolygon();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  PolygonPtr polygon_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(PolygonTest);

// Verify proper Type.
void PolygonTest::TestType() {
  CPPUNIT_ASSERT(Type_Polygon == polygon_->Type());
  CPPUNIT_ASSERT(true == polygon_->IsA(Type_Polygon));
  CPPUNIT_ASSERT(true == polygon_->IsA(Type_Geometry));
  CPPUNIT_ASSERT(true == polygon_->IsA(Type_Object));
}

// Verify proper defaults.
void PolygonTest::TestDefaults() {
  CPPUNIT_ASSERT(false == polygon_->has_id());
  CPPUNIT_ASSERT(false == polygon_->has_targetid());
  CPPUNIT_ASSERT(false == polygon_->has_extrude());
  CPPUNIT_ASSERT(false == polygon_->get_extrude());
  CPPUNIT_ASSERT(false == polygon_->has_tessellate());
  CPPUNIT_ASSERT(false == polygon_->get_tessellate());
  CPPUNIT_ASSERT(false == polygon_->has_altitudemode());
  CPPUNIT_ASSERT(ALTITUDEMODE_CLAMPTOGROUND == polygon_->get_altitudemode());
  CPPUNIT_ASSERT(false == polygon_->has_outerboundaryis());
  CPPUNIT_ASSERT(0 == polygon_->get_innerboundaryis_array_size());
}

// Verify setting default makes has_xxx() true:
void PolygonTest::TestSetToDefaultValues() {
  TestDefaults();
  polygon_->set_extrude(polygon_->get_extrude());
  polygon_->set_tessellate(polygon_->get_tessellate());
  polygon_->set_altitudemode(polygon_->get_altitudemode());
  polygon_->set_outerboundaryis(NULL);
  CPPUNIT_ASSERT(true == polygon_->has_extrude());
  CPPUNIT_ASSERT(true == polygon_->has_tessellate());
  CPPUNIT_ASSERT(true == polygon_->has_altitudemode());
  CPPUNIT_ASSERT(false == polygon_->has_outerboundaryis());  // ptr is null
}

// Verify set, get, has, clear:
void PolygonTest::TestSetGetHasClear() {
  // Non-default values:
  bool extrude = true;
  bool tessellate = true;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_RELATIVETOGROUND;
  OuterBoundaryIsPtr outerboundaryis =
    KmlFactory::GetFactory()->CreateOuterBoundaryIs();

  // Set all fields:
  polygon_->set_extrude(extrude);
  polygon_->set_tessellate(tessellate);
  polygon_->set_altitudemode(altitudemode);
  polygon_->set_outerboundaryis(outerboundaryis);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == polygon_->has_extrude());
  CPPUNIT_ASSERT(extrude == polygon_->get_extrude());
  CPPUNIT_ASSERT(true == polygon_->has_tessellate());
  CPPUNIT_ASSERT(tessellate == polygon_->get_tessellate());
  CPPUNIT_ASSERT(true == polygon_->has_altitudemode());
  CPPUNIT_ASSERT(altitudemode == polygon_->get_altitudemode());
  CPPUNIT_ASSERT(true == polygon_->has_outerboundaryis());
  CPPUNIT_ASSERT(outerboundaryis == polygon_->get_outerboundaryis());

  // Clear all fields:
  polygon_->clear_extrude();
  polygon_->clear_tessellate();
  polygon_->clear_altitudemode();
  polygon_->clear_outerboundaryis();

  // Verify now in default state:
  TestDefaults();
}

// Test MultiGeometry.
class MultiGeometryTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(MultiGeometryTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestAddGetGeometries);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    multigeometry_ = KmlFactory::GetFactory()->CreateMultiGeometry();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestAddGetGeometries();

 private:
  MultiGeometryPtr multigeometry_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(MultiGeometryTest);

void MultiGeometryTest::TestType() {
  CPPUNIT_ASSERT(Type_MultiGeometry == multigeometry_->Type());
  CPPUNIT_ASSERT(true == multigeometry_->IsA(Type_MultiGeometry));
  CPPUNIT_ASSERT(true == multigeometry_->IsA(Type_Geometry));
  CPPUNIT_ASSERT(true == multigeometry_->IsA(Type_Object));
}

void MultiGeometryTest::TestDefaults() {
  CPPUNIT_ASSERT(false == multigeometry_->has_id());
  CPPUNIT_ASSERT(false == multigeometry_->has_targetid());
  CPPUNIT_ASSERT(0 == multigeometry_->get_geometry_array_size());
}

void MultiGeometryTest::TestAddGetGeometries() {
  // Create some Geometryies and give them to the MultiGeometry
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreatePoint());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateMultiGeometry());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreatePolygon());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateModel());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateLineString());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateLinearRing());

  // Verify the proper size and order of the geometry array:
  CPPUNIT_ASSERT(6 == multigeometry_->get_geometry_array_size());
  CPPUNIT_ASSERT(
      Type_Point == multigeometry_->get_geometry_array_at(0)->Type());
  CPPUNIT_ASSERT(
      Type_MultiGeometry == multigeometry_->get_geometry_array_at(1)->Type());
  CPPUNIT_ASSERT(
      Type_Polygon == multigeometry_->get_geometry_array_at(2)->Type());
  CPPUNIT_ASSERT(
      Type_Model == multigeometry_->get_geometry_array_at(3)->Type());
  CPPUNIT_ASSERT(
      Type_LineString == multigeometry_->get_geometry_array_at(4)->Type());
  CPPUNIT_ASSERT(
      Type_LinearRing == multigeometry_->get_geometry_array_at(5)->Type());
}

}  // end namespace kmldom

TEST_MAIN
