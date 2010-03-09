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

// This file contains the unit tests for Coordinates, Geometry, Point,
// LineString, LinearRing, Polygon, InnerBoundaryIs, OuterBoundaryIs, and
// MultiGeometry.

#include "kml/dom/geometry.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/serializer.h"
#include "gtest/gtest.h"

using kmlbase::Vec3;

namespace kmldom {

// Test Coordinates.
class CoordinatesTest : public testing::Test {
 protected:
  virtual void SetUp() {
    coordinates_ = KmlFactory::GetFactory()->CreateCoordinates();
  }

  CoordinatesPtr coordinates_;
};

TEST_F(CoordinatesTest, TestType) {
  ASSERT_EQ(Type_coordinates, coordinates_->Type());
  ASSERT_TRUE(coordinates_->IsA(Type_coordinates));
}

// Verify proper defaults:
TEST_F(CoordinatesTest, TestDefaults) {
  ASSERT_EQ(static_cast<size_t>(0), coordinates_->get_coordinates_array_size());
}

// Verify the add_latlng() setter.
TEST_F(CoordinatesTest, TestAddLatLng) {
  const double kLat(-22.22);
  const double kLon(44.44);
  coordinates_->add_latlng(kLat, kLon);
  ASSERT_EQ(static_cast<size_t>(1),
            coordinates_->get_coordinates_array_size());
  Vec3 vec3 = coordinates_->get_coordinates_array_at(0);
  ASSERT_DOUBLE_EQ(kLat, vec3.get_latitude());
  ASSERT_DOUBLE_EQ(kLon, vec3.get_longitude());
  ASSERT_FALSE(vec3.has_altitude());
  ASSERT_DOUBLE_EQ(0.0, vec3.get_altitude());
}

// Verify the add_latlngalt() setter.
TEST_F(CoordinatesTest, TestAddLatLngAlt) {
  const double kLat(-22.22);
  const double kLon(44.44);
  const double kAlt(10001.2002);
  coordinates_->add_latlngalt(kLat, kLon, kAlt);
  ASSERT_EQ(static_cast<size_t>(1),
            coordinates_->get_coordinates_array_size());
  Vec3 vec3 = coordinates_->get_coordinates_array_at(0);
  ASSERT_DOUBLE_EQ(kLat, vec3.get_latitude());
  ASSERT_DOUBLE_EQ(kLon, vec3.get_longitude());
  ASSERT_TRUE(vec3.has_altitude());
  ASSERT_DOUBLE_EQ(kAlt, vec3.get_altitude());
}

// Verify the add_vec3() setter.
TEST_F(CoordinatesTest, TestAddVec3) {
  const double kLat(-22.22);
  const double kLon(44.44);
  const double kAlt(10001.2002);
  Vec3 v(kLon, kLat, kAlt);
  coordinates_->add_vec3(v);
  ASSERT_EQ(static_cast<size_t>(1),
            coordinates_->get_coordinates_array_size());
  Vec3 vec3_0 = coordinates_->get_coordinates_array_at(0);
  ASSERT_DOUBLE_EQ(kLat, vec3_0.get_latitude());
  ASSERT_DOUBLE_EQ(kLon, vec3_0.get_longitude());
  ASSERT_TRUE(vec3_0.has_altitude());
  ASSERT_DOUBLE_EQ(kAlt, vec3_0.get_altitude());

  coordinates_->add_vec3(Vec3());
  ASSERT_EQ(static_cast<size_t>(2),
            coordinates_->get_coordinates_array_size());
  Vec3 vec3_1 = coordinates_->get_coordinates_array_at(1);
  ASSERT_DOUBLE_EQ(0.0, vec3_1.get_latitude());
  ASSERT_DOUBLE_EQ(0.0, vec3_1.get_longitude());
  ASSERT_DOUBLE_EQ(0.0, vec3_1.get_altitude());

  coordinates_->add_vec3(Vec3(kLon, kLat));
  ASSERT_EQ(static_cast<size_t>(3),
            coordinates_->get_coordinates_array_size());
  Vec3 vec3_2 = coordinates_->get_coordinates_array_at(2);
  ASSERT_DOUBLE_EQ(kLat, vec3_2.get_latitude());
  ASSERT_DOUBLE_EQ(kLon, vec3_2.get_longitude());
  ASSERT_DOUBLE_EQ(0.0, vec3_2.get_altitude());
}

// Verify a bunch of points in a <coordinates> element.
TEST_F(CoordinatesTest, TestAddLatLngAltMany) {
  const size_t kNumPoints(1001);
  size_t i;
  for (i = 0; i < kNumPoints; ++i) {
    coordinates_->add_latlngalt(static_cast<double>(i % 90),
                                static_cast<double>(i % 180),
                                static_cast<double>(i));
  }
  ASSERT_EQ(static_cast<size_t>(kNumPoints),
            coordinates_->get_coordinates_array_size());
  for (i = 0; i < kNumPoints; ++i) {
    Vec3 vec3 = coordinates_->get_coordinates_array_at(i);
    ASSERT_DOUBLE_EQ(static_cast<double>(i % 90), vec3.get_latitude());
    ASSERT_DOUBLE_EQ(static_cast<double>(i % 180), vec3.get_longitude());
    ASSERT_TRUE(vec3.has_altitude());
    ASSERT_DOUBLE_EQ(static_cast<double>(i), vec3.get_altitude());
  }
}

TEST_F(CoordinatesTest, TestParseVec3WithAltitude) {
  const char* basic_3d_point = "1.123,-2.789,3000.5919";
  char *endp;
  Vec3 vec;
  ASSERT_TRUE(Coordinates::ParseVec3(basic_3d_point, &endp, &vec));
  ASSERT_DOUBLE_EQ(-2.789, vec.get_latitude());
  ASSERT_DOUBLE_EQ(1.123, vec.get_longitude());
  ASSERT_TRUE(vec.has_altitude());
  ASSERT_DOUBLE_EQ(3000.5919, vec.get_altitude());
  ASSERT_EQ(basic_3d_point + strlen(basic_3d_point), endp);

  const char* basic_3d_line = "-122.123,38.789,1050.0987 "
                              "-122.123,39.789,1050.098";
  ASSERT_TRUE(Coordinates::ParseVec3(basic_3d_line, &endp, &vec));
  ASSERT_DOUBLE_EQ(38.789, vec.get_latitude());
  ASSERT_DOUBLE_EQ(-122.123, vec.get_longitude());
  ASSERT_TRUE(vec.has_altitude());
  ASSERT_DOUBLE_EQ(1050.0987, vec.get_altitude());
  ASSERT_TRUE(Coordinates::ParseVec3(endp, &endp, &vec));
  ASSERT_DOUBLE_EQ(39.789, vec.get_latitude());
  ASSERT_DOUBLE_EQ(-122.123, vec.get_longitude());
  ASSERT_TRUE(vec.has_altitude());
  ASSERT_DOUBLE_EQ(1050.098, vec.get_altitude());
}

TEST_F(CoordinatesTest, TestParseVec3NoAltitude) {
  const char* basic_2d_point = "10.10,-20.20";
  char *endp;
  Vec3 vec;
  ASSERT_TRUE(Coordinates::ParseVec3(basic_2d_point, &endp, &vec));
  ASSERT_DOUBLE_EQ(-20.20, vec.get_latitude());
  ASSERT_DOUBLE_EQ(10.10, vec.get_longitude());
  ASSERT_FALSE(vec.has_altitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  const char* point2d_with_1space = "15.10, -24.20";
  ASSERT_TRUE(Coordinates::ParseVec3(point2d_with_1space, &endp, &vec));
  ASSERT_DOUBLE_EQ(-24.20, vec.get_latitude());
  ASSERT_DOUBLE_EQ(15.10, vec.get_longitude());
  ASSERT_FALSE(vec.has_altitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  const char* point2d_with_2spaces = "15.11 , -24.25";
  ASSERT_TRUE(Coordinates::ParseVec3(point2d_with_2spaces, &endp, &vec));
  ASSERT_DOUBLE_EQ(-24.25, vec.get_latitude());
  ASSERT_DOUBLE_EQ(15.11, vec.get_longitude());
  ASSERT_FALSE(vec.has_altitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  const char* basic_2d_line = "122.123,-38.789 "
                              "122.123,-39.789";
  ASSERT_TRUE(Coordinates::ParseVec3(basic_2d_line, &endp, &vec));
  ASSERT_DOUBLE_EQ(-38.789, vec.get_latitude());
  ASSERT_DOUBLE_EQ(122.123, vec.get_longitude());
  ASSERT_FALSE(vec.has_altitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());
  ASSERT_TRUE(Coordinates::ParseVec3(endp, &endp, &vec));
  ASSERT_DOUBLE_EQ(-39.789, vec.get_latitude());
  ASSERT_DOUBLE_EQ(122.123, vec.get_longitude());
  ASSERT_FALSE(vec.has_altitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  // How our own serializer emits <coordinates>
  const char* line_with_newlines = "\n"
                                   "  -160.073803556017,22.0041773078075\n"
                                   "  -160.121962433575,21.9639787234984\n"
                                   "  -160.22633646805,21.8915919620539\n"
                                   "  ";
  ASSERT_TRUE(Coordinates::ParseVec3(line_with_newlines, &endp, &vec));

  const char* exponential_2d_pt = "1E-02, 2E-02";  // 0.01, 0.02
  ASSERT_TRUE(Coordinates::ParseVec3(exponential_2d_pt, &endp, &vec));
  ASSERT_DOUBLE_EQ(0.02, vec.get_latitude());
  ASSERT_DOUBLE_EQ(0.01, vec.get_longitude());
  ASSERT_FALSE(vec.has_altitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  // Ensure junk data is handled gracefully.
  const char* junk_coords1 = "this will not parse correctly";
  ASSERT_FALSE(Coordinates::ParseVec3(junk_coords1, &endp, &vec));

  const char* junk_coords2 = "0,foo";  // Will parse successfully.
  ASSERT_TRUE(Coordinates::ParseVec3(junk_coords2, &endp, &vec));
  ASSERT_DOUBLE_EQ(0.0, vec.get_latitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_longitude());
  ASSERT_FALSE(vec.has_altitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  const char* junk_coords3 = "bar,0";  // Will parse successfully.
  ASSERT_TRUE(Coordinates::ParseVec3(junk_coords3, &endp, &vec));
  ASSERT_DOUBLE_EQ(0.0, vec.get_latitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_longitude());
  ASSERT_FALSE(vec.has_altitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  const char* junk_coords4 = "\n";  // Will fail parsing.
  ASSERT_FALSE(Coordinates::ParseVec3(junk_coords4, &endp, &vec));
}

TEST_F(CoordinatesTest, TestParsePoint) {
  // Parser gathers character data and sets element
  coordinates_->Parse("1.1,-2.2,3.3");
  ASSERT_EQ(static_cast<size_t>(1), coordinates_->get_coordinates_array_size());
  Vec3 vec = coordinates_->get_coordinates_array_at(0);
  ASSERT_DOUBLE_EQ(-2.2, vec.get_latitude());
  ASSERT_DOUBLE_EQ(1.1, vec.get_longitude());
  ASSERT_TRUE(vec.has_altitude());
  ASSERT_DOUBLE_EQ(3.3, vec.get_altitude());
}

TEST_F(CoordinatesTest, TestParseLine) {
  // Parser gathers character data and sets element
  coordinates_->Parse("1.1,-2.2,3.3 -5.1,32.9872,10000.3");
  ASSERT_EQ(static_cast<size_t>(2), coordinates_->get_coordinates_array_size());
  ASSERT_DOUBLE_EQ(-2.2,
                   coordinates_->get_coordinates_array_at(0).get_latitude());
  ASSERT_DOUBLE_EQ(1.1,
                   coordinates_->get_coordinates_array_at(0).get_longitude());
  ASSERT_TRUE(coordinates_->get_coordinates_array_at(0).has_altitude());
  ASSERT_DOUBLE_EQ(3.3,
                   coordinates_->get_coordinates_array_at(0).get_altitude());
}

TEST_F(CoordinatesTest, TestParseBadSeparator) {
  // Ensure graceful handling of bad data.
  coordinates_->Parse("130.999*66.56083,75");
  ASSERT_EQ(static_cast<size_t>(1), coordinates_->get_coordinates_array_size());
  ASSERT_DOUBLE_EQ(75.0,
                   coordinates_->get_coordinates_array_at(0).get_latitude());
  ASSERT_DOUBLE_EQ(130.999,
                   coordinates_->get_coordinates_array_at(0).get_longitude());
  ASSERT_DOUBLE_EQ(0.0,
                   coordinates_->get_coordinates_array_at(0).get_altitude());
}

TEST_F(CoordinatesTest, TestClear) {
  // Clearing nothing results in nothing.
  coordinates_->Clear();
  ASSERT_EQ(static_cast<size_t>(0), coordinates_->get_coordinates_array_size());
  // Clearing one thing results in nothing.
  coordinates_->add_latlng(1,2);
  ASSERT_EQ(static_cast<size_t>(1), coordinates_->get_coordinates_array_size());
  coordinates_->Clear();
  ASSERT_EQ(static_cast<size_t>(0), coordinates_->get_coordinates_array_size());
  // Clearing many things results in nothing.
  const size_t kCount(1001);
  for (size_t i = 0; i < kCount; ++i) {
    coordinates_->add_latlngalt(i,i,i);
  }
  ASSERT_EQ(kCount, coordinates_->get_coordinates_array_size());
  coordinates_->Clear();
  ASSERT_EQ(static_cast<size_t>(0), coordinates_->get_coordinates_array_size());
}

// This typedef is a convenience for use with the CoordinatesSerializerStub.
typedef std::vector<kmlbase::Vec3> Vec3Vector;

// This class provides a mock Serializer specifically designed to capture
// the output of the Coordinates::Serialize() method.  Serialization of
// <coordinates> follows a pattern no other KML element follows (it's a
// simple element as far as XML is concerned, but the content does follow a
// structure with its repeated coordinates tuples.
class MockCoordinatesSerializer : public Serializer {
 public:
  // The MockCoordinatesSerializer appends each Vec3 emitted by the
  // coordinates serialize method.  It is up to the caller to ensure that the
  // vector is in the desired state before using this class (empty, for
  // example).
  MockCoordinatesSerializer(Vec3Vector* vec3_vector)
    : vec3_vector_(vec3_vector),
      element_count_(0) {
  }
  // Each tuple in the <coordinates> content is emitted as a kmlbase::Vec3.
  virtual void SaveVec3(const kmlbase::Vec3& vec3) {
    vec3_vector_->push_back(vec3);
  }
  // This is called once before the Vec3's are emitted.
  virtual void BeginElementArray(int type_id, size_t element_count) {
    element_count_ += static_cast<int>(element_count);
  }
  // This is called once after all Vec3's are emitted.
  virtual void EndElementArray(int type_id) {}
  int get_element_count() const {
    return element_count_;
  }
 private:
  Vec3Vector* vec3_vector_;
  int element_count_;
};

// Test serialization of <coordinates/>
TEST_F(CoordinatesTest, TestSerializeNone) {
  Vec3Vector vec3_vector;
  MockCoordinatesSerializer mock(&vec3_vector);
  // This calls Coordinates::Serialize().
  mock.SaveElement(coordinates_);
  ASSERT_TRUE(vec3_vector.empty());
  ASSERT_EQ(0, mock.get_element_count());
}

// Test serialization of <coordinates>1.1,2.2,3.3</coordinates>.
TEST_F(CoordinatesTest, TestSerializeOne) {
  Vec3 vec3(1.1, 2.2, 3.3);
  coordinates_->add_vec3(vec3);
  Vec3Vector vec3_vector;
  MockCoordinatesSerializer mock(&vec3_vector);
  // This calls Coordinates::Serialize().
  mock.SaveElement(coordinates_);
  ASSERT_EQ(static_cast<size_t>(1), vec3_vector.size());
  ASSERT_EQ(1, mock.get_element_count());
  ASSERT_EQ(vec3.get_latitude(), vec3_vector[0].get_latitude());
  ASSERT_EQ(vec3.get_longitude(), vec3_vector[0].get_longitude());
  ASSERT_EQ(vec3.has_altitude(), vec3_vector[0].has_altitude());
  ASSERT_EQ(vec3.get_altitude(), vec3_vector[0].get_altitude());
}

// Test serialization of:
// <coordinates>0.2,0.1,0.3 1.2,1.1,1.3 2.2,2.1,2.3 ... </coordinates>/
TEST_F(CoordinatesTest, TestSerializeMany) {
  const size_t kNumTuples = 47;
  const double kLatFrac = 0.1;
  const double kLngFrac = 0.2;
  const double kAltFrac = 0.3;
  for (size_t i = 0; i < kNumTuples; ++i) {
    coordinates_->add_latlngalt(i + kLatFrac, i + kLngFrac, i + kAltFrac);
  }
  Vec3Vector vec3_vector;
  MockCoordinatesSerializer mock(&vec3_vector);
  // This calls Coordinates::Serialize().
  mock.SaveElement(coordinates_);
  ASSERT_EQ(kNumTuples, vec3_vector.size());
  ASSERT_EQ(static_cast<int>(kNumTuples), mock.get_element_count());
  for (size_t i = 0; i < kNumTuples; ++i) {
    ASSERT_EQ(i + kLatFrac, vec3_vector[i].get_latitude());
    ASSERT_EQ(i + kLngFrac, vec3_vector[i].get_longitude());
    ASSERT_TRUE(vec3_vector[i].has_altitude());
    ASSERT_EQ(i + kAltFrac, vec3_vector[i].get_altitude());
  }
}

// Coordinate tuples must be separated by a space. However, the world is
// imperfect and thus we try to do the right thing with this:
// <coordinates>1,2,3,4,5,6,7,8,9</coordinates>, where the "right thing" is
// to take it as three lng,lat,alt tuples. This is Google Earth's behavior.
TEST_F(CoordinatesTest, TestCommaSeparators) {
  coordinates_->Parse("1,2,3,4,5,6,7,8,9");
  ASSERT_EQ(static_cast<size_t>(3), coordinates_->get_coordinates_array_size());
  ASSERT_DOUBLE_EQ(2.0,
                   coordinates_->get_coordinates_array_at(0).get_latitude());
  ASSERT_DOUBLE_EQ(1.0,
                   coordinates_->get_coordinates_array_at(0).get_longitude());
  ASSERT_DOUBLE_EQ(3.0,
                   coordinates_->get_coordinates_array_at(0).get_altitude());
  ASSERT_DOUBLE_EQ(5.0,
                   coordinates_->get_coordinates_array_at(1).get_latitude());
  ASSERT_DOUBLE_EQ(4.0,
                   coordinates_->get_coordinates_array_at(1).get_longitude());
  ASSERT_DOUBLE_EQ(6.0,
                   coordinates_->get_coordinates_array_at(1).get_altitude());
  ASSERT_DOUBLE_EQ(8.0,
                   coordinates_->get_coordinates_array_at(2).get_latitude());
  ASSERT_DOUBLE_EQ(7.0,
                   coordinates_->get_coordinates_array_at(2).get_longitude());
  ASSERT_DOUBLE_EQ(9.0,
                   coordinates_->get_coordinates_array_at(2).get_altitude());
}

// Test Point.
class PointTest : public testing::Test {
 protected:
  virtual void SetUp() {
    point_ = KmlFactory::GetFactory()->CreatePoint();
  }

  PointPtr point_;
};

TEST_F(PointTest, TestType) {
  ASSERT_EQ(Type_Point, point_->Type());
  ASSERT_TRUE(point_->IsA(Type_Point));
  ASSERT_TRUE(point_->IsA(Type_Geometry));
  ASSERT_TRUE(point_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(PointTest, TestDefaults) {
  ASSERT_FALSE(point_->has_id());
  ASSERT_FALSE(point_->has_targetid());
  ASSERT_FALSE(point_->has_extrude());
  ASSERT_FALSE(point_->get_extrude());
  ASSERT_FALSE(point_->has_altitudemode());
  ASSERT_TRUE(ALTITUDEMODE_CLAMPTOGROUND == point_->get_altitudemode());
  ASSERT_FALSE(point_->has_gx_altitudemode());
  ASSERT_TRUE(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR == point_->get_gx_altitudemode());
  ASSERT_FALSE(point_->has_coordinates());
  ASSERT_TRUE(NULL == point_->get_coordinates());
}

// Verify setting default makes has_xxx() true:
TEST_F(PointTest, TestSetToDefaultValues) {
  point_->set_extrude(point_->get_extrude());
  point_->set_altitudemode(point_->get_altitudemode());
  point_->set_gx_altitudemode(point_->get_gx_altitudemode());
  point_->set_coordinates(NULL);  // should not crash
  ASSERT_TRUE(point_->has_extrude());
  ASSERT_TRUE(point_->has_altitudemode());
  ASSERT_TRUE(point_->has_gx_altitudemode());
  ASSERT_FALSE(point_->has_coordinates());  // ptr is null
}

// Verify set, get, has, clear:
TEST_F(PointTest, TestSetGetHasClear) {
  // Non-default values:
  bool extrude = true;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_RELATIVETOGROUND;
  GxAltitudeModeEnum gx_altitudemode = GX_ALTITUDEMODE_RELATIVETOSEAFLOOR;
  CoordinatesPtr coordinates = KmlFactory::GetFactory()->CreateCoordinates();
  const char* id = "point-id";
  const char* targetid = "point-targetid";

  // Set all fields:
  point_->set_id(id);
  point_->set_targetid(targetid);
  point_->set_extrude(extrude);
  point_->set_altitudemode(altitudemode);
  point_->set_gx_altitudemode(gx_altitudemode);
  point_->set_coordinates(coordinates);

  // Verify getter and has_xxx():
  ASSERT_TRUE(point_->has_id());
  ASSERT_EQ(id, point_->get_id());
  ASSERT_TRUE(point_->has_targetid());
  ASSERT_EQ(targetid, point_->get_targetid());
  ASSERT_TRUE(point_->has_extrude());
  ASSERT_EQ(extrude, point_->get_extrude());
  ASSERT_TRUE(point_->has_altitudemode());
  ASSERT_EQ(altitudemode, point_->get_altitudemode());
  ASSERT_TRUE(point_->has_gx_altitudemode());
  ASSERT_EQ(gx_altitudemode, point_->get_gx_altitudemode());
  ASSERT_TRUE(point_->has_coordinates());
  ASSERT_EQ(coordinates, point_->get_coordinates());

  // Clear all fields:
  point_->clear_id();
  point_->clear_targetid();
  point_->clear_extrude();
  point_->clear_altitudemode();
  point_->clear_gx_altitudemode();
  point_->clear_coordinates();

}

TEST_F(PointTest, TestSerialize) {
  point_->set_id("point-id");
  point_->set_extrude(true);
  point_->set_coordinates(KmlFactory::GetFactory()->CreateCoordinates());
  string expected(
    "<Point id=\"point-id\">"
    "<extrude>1</extrude>"
    "<coordinates/>"
    "</Point>"
  );
  ASSERT_EQ(expected, SerializeRaw(point_));
}

TEST_F(PointTest, TestSerializeParseAll) {
  point_->set_id("point-id");
  point_->set_extrude(false);
  point_->set_gx_altitudemode(GX_ALTITUDEMODE_RELATIVETOSEAFLOOR);
  point_->set_coordinates(KmlFactory::GetFactory()->CreateCoordinates());
  string expected(
    "<Point id=\"point-id\">"
    "<extrude>0</extrude>"
    "<gx:altitudeMode>relativeToSeaFloor</gx:altitudeMode>"
    "<coordinates/>"
    "</Point>"
  );
  ASSERT_EQ(expected, SerializeRaw(point_));
  string errors;
  ElementPtr element = Parse(expected, &errors);
  ASSERT_TRUE(element);
  ASSERT_TRUE(errors.empty());
  PointPtr point = AsPoint(element);
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_id());
  ASSERT_EQ(string("point-id"), point->get_id());
  ASSERT_TRUE(point->has_extrude());
  ASSERT_EQ(0, point->get_extrude());
  ASSERT_FALSE(point->has_altitudemode());
  ASSERT_TRUE(point->has_gx_altitudemode());
  ASSERT_EQ(GX_ALTITUDEMODE_RELATIVETOSEAFLOOR, point->get_gx_altitudemode());
}

// Test LineString.
class LineStringTest : public testing::Test {
 protected:
  virtual void SetUp() {
    linestring_ = KmlFactory::GetFactory()->CreateLineString();
  }

  LineStringPtr linestring_;
};

TEST_F(LineStringTest, TestType) {
  ASSERT_EQ(Type_LineString, linestring_->Type());
  ASSERT_TRUE(linestring_->IsA(Type_LineString));
  ASSERT_TRUE(linestring_->IsA(Type_Geometry));
  ASSERT_TRUE(linestring_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(LineStringTest, TestDefaults) {
  ASSERT_FALSE(linestring_->has_id());
  ASSERT_FALSE(linestring_->has_targetid());
  ASSERT_FALSE(linestring_->has_extrude());
  ASSERT_FALSE(linestring_->get_extrude());
  ASSERT_FALSE(linestring_->has_tessellate());
  ASSERT_FALSE(linestring_->get_tessellate());
  ASSERT_FALSE(linestring_->has_altitudemode());
  ASSERT_EQ(ALTITUDEMODE_CLAMPTOGROUND, linestring_->get_altitudemode());
  ASSERT_FALSE(linestring_->has_gx_altitudemode());
  ASSERT_EQ(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR,
            linestring_->get_gx_altitudemode());
  ASSERT_FALSE(linestring_->has_coordinates());
}

// Verify setting default makes has_xxx() true:
TEST_F(LineStringTest, TestSetToDefaultValues) {
  linestring_->set_extrude(linestring_->get_extrude());
  linestring_->set_tessellate(linestring_->get_tessellate());
  linestring_->set_altitudemode(linestring_->get_altitudemode());
  linestring_->set_gx_altitudemode(linestring_->get_gx_altitudemode());
  linestring_->set_coordinates(NULL);  // should not crash
  ASSERT_TRUE(linestring_->has_extrude());
  ASSERT_TRUE(linestring_->has_tessellate());
  ASSERT_TRUE(linestring_->has_altitudemode());
  ASSERT_TRUE(linestring_->has_gx_altitudemode());
  ASSERT_FALSE(linestring_->has_coordinates());  // ptr is null
}

// Verify set, get, has, clear:
TEST_F(LineStringTest, TestSetGetHasClear) {
  // Non-default values:
  bool extrude = true;
  bool tessellate = true;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_RELATIVETOGROUND;
  GxAltitudeModeEnum gx_altitudemode = GX_ALTITUDEMODE_RELATIVETOSEAFLOOR;
  CoordinatesPtr coordinates = KmlFactory::GetFactory()->CreateCoordinates();

  // Set all fields:
  linestring_->set_extrude(extrude);
  linestring_->set_tessellate(tessellate);
  linestring_->set_altitudemode(altitudemode);
  linestring_->set_gx_altitudemode(gx_altitudemode);
  linestring_->set_coordinates(coordinates);

  // Verify getter and has_xxx():
  ASSERT_TRUE(linestring_->has_extrude());
  ASSERT_EQ(extrude, linestring_->get_extrude());
  ASSERT_TRUE(linestring_->has_tessellate());
  ASSERT_EQ(tessellate, linestring_->get_tessellate());
  ASSERT_TRUE(linestring_->has_altitudemode());
  ASSERT_EQ(altitudemode, linestring_->get_altitudemode());
  ASSERT_TRUE(linestring_->has_gx_altitudemode());
  ASSERT_EQ(gx_altitudemode, linestring_->get_gx_altitudemode());
  ASSERT_TRUE(linestring_->has_coordinates());
  ASSERT_EQ(coordinates, linestring_->get_coordinates());

  // Clear all fields:
  linestring_->clear_extrude();
  linestring_->clear_tessellate();
  linestring_->clear_altitudemode();
  linestring_->clear_gx_altitudemode();
  linestring_->clear_coordinates();
}

TEST_F(LineStringTest, TestSerialize) {
  linestring_->set_id("linestring-id");
  linestring_->set_tessellate(true);
  linestring_->set_coordinates(KmlFactory::GetFactory()->CreateCoordinates());
  string expected(
    "<LineString id=\"linestring-id\">"
    "<tessellate>1</tessellate>"
    "<coordinates/>"
    "</LineString>"
  );
  ASSERT_EQ(expected, SerializeRaw(linestring_));
}

TEST_F(LineStringTest, TestSerializeParseAll) {
  linestring_->set_id("linestring-id");
  linestring_->set_extrude(true);
  linestring_->set_tessellate(true);
  linestring_->set_gx_altitudemode(GX_ALTITUDEMODE_RELATIVETOSEAFLOOR);
  linestring_->set_coordinates(KmlFactory::GetFactory()->CreateCoordinates());
  string expected(
    "<LineString id=\"linestring-id\">"
    "<extrude>1</extrude>"
    "<tessellate>1</tessellate>"
    "<gx:altitudeMode>relativeToSeaFloor</gx:altitudeMode>"
    "<coordinates/>"
    "</LineString>"
  );
  ASSERT_EQ(expected, SerializeRaw(linestring_));
  string errors;
  ElementPtr element = Parse(expected, &errors);
  ASSERT_TRUE(element);
  ASSERT_TRUE(errors.empty());
  LineStringPtr linestring = AsLineString(element);
  ASSERT_TRUE(linestring);
  ASSERT_TRUE(linestring->has_id());
  ASSERT_EQ(string("linestring-id"), linestring->get_id());
  ASSERT_TRUE(linestring->has_extrude());
  ASSERT_EQ(1, linestring->get_extrude());
  ASSERT_TRUE(linestring->has_tessellate());
  ASSERT_EQ(1, linestring->get_tessellate());
  ASSERT_FALSE(linestring->has_altitudemode());
  ASSERT_TRUE(linestring->has_gx_altitudemode());
  ASSERT_EQ(GX_ALTITUDEMODE_RELATIVETOSEAFLOOR,
            linestring->get_gx_altitudemode());
}

// Test LinearRing.
class LinearRingTest : public testing::Test {
 protected:
  virtual void SetUp() {
    linearring_ = KmlFactory::GetFactory()->CreateLinearRing();
  }

  LinearRingPtr linearring_;
};

TEST_F(LinearRingTest, TestType) {
  ASSERT_EQ(Type_LinearRing, linearring_->Type());
  ASSERT_TRUE(linearring_->IsA(Type_LinearRing));
  ASSERT_TRUE(linearring_->IsA(Type_Geometry));
  ASSERT_TRUE(linearring_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(LinearRingTest, TestDefaults) {
  ASSERT_FALSE(linearring_->has_id());
  ASSERT_FALSE(linearring_->has_targetid());
  ASSERT_FALSE(linearring_->has_extrude());
  ASSERT_FALSE(linearring_->get_extrude());
  ASSERT_FALSE(linearring_->has_tessellate());
  ASSERT_FALSE(linearring_->get_tessellate());
  ASSERT_FALSE(linearring_->has_altitudemode());
  ASSERT_EQ(ALTITUDEMODE_CLAMPTOGROUND, linearring_->get_altitudemode());
  ASSERT_FALSE(linearring_->has_gx_altitudemode());
  ASSERT_EQ(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR,
            linearring_->get_gx_altitudemode());
  ASSERT_FALSE(linearring_->has_coordinates());
}

// Verify setting default makes has_xxx() true:
TEST_F(LinearRingTest, TestSetToDefaultValues) {
  linearring_->set_extrude(linearring_->get_extrude());
  linearring_->set_tessellate(linearring_->get_tessellate());
  linearring_->set_altitudemode(linearring_->get_altitudemode());
  linearring_->set_gx_altitudemode(linearring_->get_gx_altitudemode());
  linearring_->set_coordinates(NULL);  // should not crash
  ASSERT_TRUE(linearring_->has_extrude());
  ASSERT_TRUE(linearring_->has_tessellate());
  ASSERT_TRUE(linearring_->has_altitudemode());
  ASSERT_TRUE(linearring_->has_gx_altitudemode());
  ASSERT_FALSE(linearring_->has_coordinates());  // ptr is null
}

// Verify set, get, has, clear:
TEST_F(LinearRingTest, TestSetGetHasClear) {
  // Non-default values:
  bool extrude = true;
  bool tessellate = true;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_RELATIVETOGROUND;
  GxAltitudeModeEnum gx_altitudemode = GX_ALTITUDEMODE_RELATIVETOSEAFLOOR;
  CoordinatesPtr coordinates = KmlFactory::GetFactory()->CreateCoordinates();

  // Set all fields:
  linearring_->set_extrude(extrude);
  linearring_->set_tessellate(tessellate);
  linearring_->set_altitudemode(altitudemode);
  linearring_->set_gx_altitudemode(gx_altitudemode);
  linearring_->set_coordinates(coordinates);

  // Verify getter and has_xxx():
  ASSERT_TRUE(linearring_->has_extrude());
  ASSERT_EQ(extrude, linearring_->get_extrude());
  ASSERT_TRUE(linearring_->has_tessellate());
  ASSERT_EQ(tessellate, linearring_->get_tessellate());
  ASSERT_TRUE(linearring_->has_altitudemode());
  ASSERT_EQ(altitudemode, linearring_->get_altitudemode());
  ASSERT_TRUE(linearring_->has_gx_altitudemode());
  ASSERT_EQ(gx_altitudemode, linearring_->get_gx_altitudemode());
  ASSERT_TRUE(linearring_->has_coordinates());
  ASSERT_EQ(coordinates, linearring_->get_coordinates());

  // Clear all fields:
  linearring_->clear_extrude();
  linearring_->clear_tessellate();
  linearring_->clear_altitudemode();
  linearring_->clear_gx_altitudemode();
  linearring_->clear_coordinates();
}

TEST_F(LinearRingTest, TestSerialize) {
  linearring_->set_id("linearring-id");
  linearring_->set_tessellate(false);
  linearring_->set_coordinates(KmlFactory::GetFactory()->CreateCoordinates());
  string expected(
    "<LinearRing id=\"linearring-id\">"
    "<tessellate>0</tessellate>"
    "<coordinates/>"
    "</LinearRing>"
  );
  ASSERT_EQ(expected, SerializeRaw(linearring_));
}

TEST_F(LinearRingTest, TestSerializeParseAll) {
  linearring_->set_id("linearring-id");
  linearring_->set_extrude(false);
  linearring_->set_tessellate(false);
  linearring_->set_gx_altitudemode(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR);
  linearring_->set_coordinates(KmlFactory::GetFactory()->CreateCoordinates());
  string expected(
    "<LinearRing id=\"linearring-id\">"
    "<extrude>0</extrude>"
    "<tessellate>0</tessellate>"
    "<gx:altitudeMode>clampToSeaFloor</gx:altitudeMode>"
    "<coordinates/>"
    "</LinearRing>"
  );
  ASSERT_EQ(expected, SerializeRaw(linearring_));
  string errors;
  ElementPtr element = Parse(expected, &errors);
  ASSERT_TRUE(element);
  ASSERT_TRUE(errors.empty());
  LinearRingPtr linearring = AsLinearRing(element);
  ASSERT_TRUE(linearring);
  ASSERT_TRUE(linearring->has_id());
  ASSERT_EQ(string("linearring-id"), linearring->get_id());
  ASSERT_TRUE(linearring->has_extrude());
  ASSERT_EQ(0, linearring->get_extrude());
  ASSERT_TRUE(linearring->has_tessellate());
  ASSERT_EQ(0, linearring->get_tessellate());
  ASSERT_FALSE(linearring->has_altitudemode());
  ASSERT_TRUE(linearring->has_gx_altitudemode());
  ASSERT_EQ(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR,
            linearring->get_gx_altitudemode());
}

// Test OuterBoundaryIs.
class OuterBoundaryIsTest : public testing::Test {
 protected:
  virtual void SetUp() {
    outerboundaryis_ = KmlFactory::GetFactory()->CreateOuterBoundaryIs();
  }

  OuterBoundaryIsPtr outerboundaryis_;
};

TEST_F(OuterBoundaryIsTest, TestType) {
  ASSERT_EQ(Type_outerBoundaryIs, outerboundaryis_->Type());
  ASSERT_TRUE(outerboundaryis_->IsA(Type_outerBoundaryIs));
}

// Verify proper defaults:
TEST_F(OuterBoundaryIsTest, TestDefaults) {
  ASSERT_FALSE(outerboundaryis_->has_linearring());
}

TEST_F(OuterBoundaryIsTest, TestSetGetHasClear) {
  // Give it the only possible child:
  outerboundaryis_->set_linearring(
      KmlFactory::GetFactory()->CreateLinearRing());
  ASSERT_TRUE(outerboundaryis_->has_linearring());
  ASSERT_TRUE(outerboundaryis_->get_linearring());
  // Clear it and verify we're back to the default state.
  outerboundaryis_->clear_linearring();
}

// Test InnerBoundaryIs.
class InnerBoundaryIsTest : public testing::Test {
 protected:
  virtual void SetUp() {
    innerboundaryis_ = KmlFactory::GetFactory()->CreateInnerBoundaryIs();
  }

  InnerBoundaryIsPtr innerboundaryis_;
};

TEST_F(InnerBoundaryIsTest, TestType) {
  ASSERT_EQ(Type_innerBoundaryIs, innerboundaryis_->Type());
  ASSERT_TRUE(innerboundaryis_->IsA(Type_innerBoundaryIs));
}

// Verify proper defaults:
TEST_F(InnerBoundaryIsTest, TestDefaults) {
  ASSERT_FALSE(innerboundaryis_->has_linearring());
}

TEST_F(InnerBoundaryIsTest, TestSetGetHasClear) {
  // Give it the only possible child:
  innerboundaryis_->set_linearring(
      KmlFactory::GetFactory()->CreateLinearRing());
  ASSERT_TRUE(innerboundaryis_->has_linearring());
  ASSERT_TRUE(innerboundaryis_->get_linearring());
  // Clear it and verify we're back to the default state.
  innerboundaryis_->clear_linearring();
}

// TestPolygon
class PolygonTest : public testing::Test {
 protected:
  virtual void SetUp() {
    polygon_ = KmlFactory::GetFactory()->CreatePolygon();
  }

  PolygonPtr polygon_;
};

// Verify proper Type.
TEST_F(PolygonTest, TestType) {
  ASSERT_EQ(Type_Polygon, polygon_->Type());
  ASSERT_TRUE(polygon_->IsA(Type_Polygon));
  ASSERT_TRUE(polygon_->IsA(Type_Geometry));
  ASSERT_TRUE(polygon_->IsA(Type_Object));
}

// Verify proper defaults.
TEST_F(PolygonTest, TestDefaults) {
  ASSERT_FALSE(polygon_->has_id());
  ASSERT_FALSE(polygon_->has_targetid());
  ASSERT_FALSE(polygon_->has_extrude());
  ASSERT_FALSE(polygon_->get_extrude());
  ASSERT_FALSE(polygon_->has_tessellate());
  ASSERT_FALSE(polygon_->get_tessellate());
  ASSERT_FALSE(polygon_->has_altitudemode());
  ASSERT_EQ(ALTITUDEMODE_CLAMPTOGROUND, polygon_->get_altitudemode());
  ASSERT_FALSE(polygon_->has_gx_altitudemode());
  ASSERT_EQ(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR, polygon_->get_gx_altitudemode());
  ASSERT_FALSE(polygon_->has_outerboundaryis());
  ASSERT_EQ(static_cast<size_t>(0), polygon_->get_innerboundaryis_array_size());
}

// Verify setting default makes has_xxx() true:
TEST_F(PolygonTest, TestSetToDefaultValues) {
  polygon_->set_extrude(polygon_->get_extrude());
  polygon_->set_tessellate(polygon_->get_tessellate());
  polygon_->set_altitudemode(polygon_->get_altitudemode());
  polygon_->set_gx_altitudemode(polygon_->get_gx_altitudemode());
  polygon_->set_outerboundaryis(NULL);
  ASSERT_TRUE(polygon_->has_extrude());
  ASSERT_TRUE(polygon_->has_tessellate());
  ASSERT_TRUE(polygon_->has_altitudemode());
  ASSERT_TRUE(polygon_->has_gx_altitudemode());
  ASSERT_FALSE(polygon_->has_outerboundaryis());  // ptr is null
}

// Verify set, get, has, clear:
TEST_F(PolygonTest, TestSetGetHasClear) {
  // Non-default values:
  bool extrude = true;
  bool tessellate = true;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_RELATIVETOGROUND;
  GxAltitudeModeEnum gx_altitudemode = GX_ALTITUDEMODE_RELATIVETOSEAFLOOR;
  OuterBoundaryIsPtr outerboundaryis =
    KmlFactory::GetFactory()->CreateOuterBoundaryIs();

  // Set all fields:
  polygon_->set_extrude(extrude);
  polygon_->set_tessellate(tessellate);
  polygon_->set_altitudemode(altitudemode);
  polygon_->set_gx_altitudemode(gx_altitudemode);
  polygon_->set_outerboundaryis(outerboundaryis);

  // Verify getter and has_xxx():
  ASSERT_TRUE(polygon_->has_extrude());
  ASSERT_EQ(extrude, polygon_->get_extrude());
  ASSERT_TRUE(polygon_->has_tessellate());
  ASSERT_EQ(tessellate, polygon_->get_tessellate());
  ASSERT_TRUE(polygon_->has_altitudemode());
  ASSERT_EQ(altitudemode, polygon_->get_altitudemode());
  ASSERT_TRUE(polygon_->has_gx_altitudemode());
  ASSERT_EQ(gx_altitudemode, polygon_->get_gx_altitudemode());
  ASSERT_TRUE(polygon_->has_outerboundaryis());
  ASSERT_EQ(outerboundaryis, polygon_->get_outerboundaryis());

  // Clear all fields:
  polygon_->clear_extrude();
  polygon_->clear_tessellate();
  polygon_->clear_altitudemode();
  polygon_->clear_gx_altitudemode();
  polygon_->clear_outerboundaryis();
}

TEST_F(PolygonTest, TestSerialize) {
  polygon_->set_id("polygon-id");
  polygon_->set_altitudemode(ALTITUDEMODE_ABSOLUTE);
  polygon_->set_outerboundaryis(
      KmlFactory::GetFactory()->CreateOuterBoundaryIs());
  string expected(
    "<Polygon id=\"polygon-id\">"
    "<altitudeMode>absolute</altitudeMode>"
    "<outerBoundaryIs/>"
    "</Polygon>"
  );
  ASSERT_EQ(expected, SerializeRaw(polygon_));
}

TEST_F(PolygonTest, TestSerializeParseAll) {
  polygon_->set_id("polygon-id");
  polygon_->set_extrude(true);
  polygon_->set_tessellate(true);
  polygon_->set_gx_altitudemode(GX_ALTITUDEMODE_RELATIVETOSEAFLOOR);
  polygon_->set_outerboundaryis(
      KmlFactory::GetFactory()->CreateOuterBoundaryIs());
  string expected(
    "<Polygon id=\"polygon-id\">"
    "<extrude>1</extrude>"
    "<tessellate>1</tessellate>"
    "<gx:altitudeMode>relativeToSeaFloor</gx:altitudeMode>"
    "<outerBoundaryIs/>"
    "</Polygon>"
  );
  ASSERT_EQ(expected, SerializeRaw(polygon_));
  string errors;
  ElementPtr element = Parse(expected, &errors);
  ASSERT_TRUE(element);
  ASSERT_TRUE(errors.empty());
  PolygonPtr polygon = AsPolygon(element);
  ASSERT_TRUE(polygon);
  ASSERT_TRUE(polygon->has_id());
  ASSERT_EQ(string("polygon-id"), polygon->get_id());
  ASSERT_TRUE(polygon->has_extrude());
  ASSERT_EQ(1, polygon->get_extrude());
  ASSERT_TRUE(polygon->has_tessellate());
  ASSERT_EQ(1, polygon->get_tessellate());
  ASSERT_FALSE(polygon->has_altitudemode());
  ASSERT_TRUE(polygon->has_gx_altitudemode());
  ASSERT_EQ(GX_ALTITUDEMODE_RELATIVETOSEAFLOOR,
            polygon->get_gx_altitudemode());
  ASSERT_TRUE(polygon->has_outerboundaryis());
}

// Test MultiGeometry.
class MultiGeometryTest : public testing::Test {
 protected:
  virtual void SetUp() {
    multigeometry_ = KmlFactory::GetFactory()->CreateMultiGeometry();
  }

  MultiGeometryPtr multigeometry_;
};

TEST_F(MultiGeometryTest, TestType) {
  ASSERT_EQ(Type_MultiGeometry, multigeometry_->Type());
  ASSERT_TRUE(multigeometry_->IsA(Type_MultiGeometry));
  ASSERT_TRUE(multigeometry_->IsA(Type_Geometry));
  ASSERT_TRUE(multigeometry_->IsA(Type_Object));
}

TEST_F(MultiGeometryTest, TestDefaults) {
  ASSERT_FALSE(multigeometry_->has_id());
  ASSERT_FALSE(multigeometry_->has_targetid());
  ASSERT_EQ(static_cast<size_t>(0), multigeometry_->get_geometry_array_size());
}

TEST_F(MultiGeometryTest, TestAddGetGeometries) {
  // Create some Geometries and give them to the MultiGeometry.
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreatePoint());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateMultiGeometry());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreatePolygon());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateModel());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateLineString());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateLinearRing());

  // Verify the proper size and order of the geometry array:
  ASSERT_EQ(static_cast<size_t>(6), multigeometry_->get_geometry_array_size());
  ASSERT_EQ(Type_Point, multigeometry_->get_geometry_array_at(0)->Type());
  ASSERT_EQ(Type_MultiGeometry,
            multigeometry_->get_geometry_array_at(1)->Type());
  ASSERT_EQ(Type_Polygon, multigeometry_->get_geometry_array_at(2)->Type());
  ASSERT_EQ(Type_Model, multigeometry_->get_geometry_array_at(3)->Type());
  ASSERT_EQ(Type_LineString, multigeometry_->get_geometry_array_at(4)->Type());
  ASSERT_EQ(Type_LinearRing, multigeometry_->get_geometry_array_at(5)->Type());
}

TEST_F(MultiGeometryTest, TestSerialize) {
  multigeometry_->set_id("multigeometry-id");
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreatePoint());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateLineString());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateLinearRing());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreatePolygon());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateModel());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateMultiGeometry());
  string expected(
    "<MultiGeometry id=\"multigeometry-id\">"
    "<Point/>"
    "<LineString/>"
    "<LinearRing/>"
    "<Polygon/>"
    "<Model/>"
    "<MultiGeometry/>"
    "</MultiGeometry>"
  );
  ASSERT_EQ(expected, SerializeRaw(multigeometry_));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
