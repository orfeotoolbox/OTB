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
#include "kml/base/date_time.h"
#include "gtest/gtest.h"

using kmlbase::DateTime;
using kmlbase::Vec3;
using kmldom::CameraPtr;
using kmldom::ChangePtr;
using kmldom::CoordinatesPtr;
using kmldom::DataPtr;
using kmldom::GxAnimatedUpdatePtr;
using kmldom::GxFlyToPtr;
using kmldom::GxWaitPtr;
using kmldom::KmlFactory;
using kmldom::LineStringPtr;
using kmldom::LinearRingPtr;
using kmldom::LookAtPtr;
using kmldom::OuterBoundaryIsPtr;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::PolygonPtr;
using kmldom::RegionPtr;
using kmldom::UpdatePtr;

namespace kmlconvenience {

// This tests the AddExtendedDataValue() function.
TEST(ConvenienceTest, TestAddExtendedDataValue) {
  const string kName("population");
  const string kValue("42000");
  PlacemarkPtr placemark = KmlFactory::GetFactory()->CreatePlacemark();
  AddExtendedDataValue(kName, kValue, placemark);
  ASSERT_TRUE(placemark->has_extendeddata());
  ASSERT_EQ(static_cast<size_t>(1),
            placemark->get_extendeddata()->get_data_array_size());
  ASSERT_EQ(kName,
            placemark->get_extendeddata()->get_data_array_at(0)->get_name());
  ASSERT_EQ(kValue,
            placemark->get_extendeddata()->get_data_array_at(0)->get_value());
}

// This tests the CreateAnimatedUpdateChangePoint() function.
TEST(ConvenienceTest, TestCreateAnimatedUpdateChangePoint) {
  const string kTargetId("targetId");
  const kmlbase::Vec3 kVec3(1.1, 2.2, 3.3);
  const double kDuration = 12.3;
  GxAnimatedUpdatePtr animated_update =
    CreateAnimatedUpdateChangePoint(kTargetId, kVec3, kDuration);
  ASSERT_TRUE(animated_update);
  ASSERT_TRUE(animated_update->has_update());
  UpdatePtr update = animated_update->get_update();
  ASSERT_TRUE(update);
  ASSERT_TRUE(update->has_targethref());
  ASSERT_EQ(string(""), update->get_targethref());
  ASSERT_EQ(static_cast<size_t>(1), update->get_updateoperation_array_size());
  ASSERT_EQ(kmldom::Type_Change,
            update->get_updateoperation_array_at(0)->Type());
  ChangePtr change = kmldom::AsChange(update->get_updateoperation_array_at(0));
  ASSERT_TRUE(change);
  ASSERT_EQ(static_cast<size_t>(1), change->get_object_array_size());
  ASSERT_EQ(kmldom::Type_Placemark, change->get_object_array_at(0)->Type());
  PlacemarkPtr placemark = kmldom::AsPlacemark(change->get_object_array_at(0));
  ASSERT_TRUE(placemark);
  ASSERT_TRUE(placemark->has_targetid());
  ASSERT_EQ(kTargetId, placemark->get_targetid());
  ASSERT_TRUE(placemark->has_geometry());
  ASSERT_EQ(kmldom::Type_Point, placemark->get_geometry()->Type());
  PointPtr point = kmldom::AsPoint(placemark->get_geometry());
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_coordinates());
  const CoordinatesPtr& coordinates = point->get_coordinates();
  ASSERT_TRUE(coordinates);
  ASSERT_EQ(static_cast<size_t>(1), coordinates->get_coordinates_array_size());
  const kmlbase::Vec3& vec = coordinates->get_coordinates_array_at(0);
  ASSERT_TRUE(kVec3 == vec);
}

// This tests the CreateBasicPolygonPlacemark() function.
TEST(ConvenienceTest, TestCreateBasicPolygonPlacemark) {
  LinearRingPtr linearring = KmlFactory::GetFactory()->CreateLinearRing();
  PlacemarkPtr placemark = CreateBasicPolygonPlacemark(linearring);
  ASSERT_TRUE(placemark);
  ASSERT_TRUE(placemark->has_geometry());
  ASSERT_TRUE(AsPolygon(placemark->get_geometry()));
  PolygonPtr poly = AsPolygon(placemark->get_geometry());
  ASSERT_TRUE(poly->has_outerboundaryis());
  ASSERT_TRUE(AsOuterBoundaryIs(poly->get_outerboundaryis()));
  OuterBoundaryIsPtr ob = AsOuterBoundaryIs(poly->get_outerboundaryis());
  ASSERT_TRUE(ob->has_linearring());
  ASSERT_TRUE(AsLinearRing(ob->get_linearring()));
}

// This tests the CreateCamera() function.
TEST(ConvenienceTest, TestCreateCamera) {
  const double kLatitude = 37.0;
  const double kLongitude = -122.0;
  const double kAltitude = 12.3;
  const double kHeading = 180.0;
  const double kTilt = 32.1;
  const double kRoll = 0.0;
  const kmldom::AltitudeModeEnum kAltitudeMode = kmldom::ALTITUDEMODE_ABSOLUTE;
  CameraPtr camera = CreateCamera(kLatitude, kLongitude, kAltitude,
                                  kHeading, kTilt, kRoll, kAltitudeMode);
  ASSERT_TRUE(camera->has_longitude());
  ASSERT_TRUE(camera->has_latitude());
  ASSERT_TRUE(camera->has_altitude());
  ASSERT_TRUE(camera->has_heading());
  ASSERT_TRUE(camera->has_tilt());
  ASSERT_TRUE(camera->has_roll());
  ASSERT_TRUE(camera->has_altitudemode());
  ASSERT_EQ(kLatitude, camera->get_latitude());
  ASSERT_EQ(kLongitude, camera->get_longitude());
  ASSERT_EQ(kAltitude, camera->get_altitude());
  ASSERT_EQ(kHeading, camera->get_heading());
  ASSERT_EQ(kTilt, camera->get_tilt());
  ASSERT_EQ(kRoll, camera->get_roll());
  ASSERT_EQ(kAltitudeMode, camera->get_altitudemode());
}

// This tests the CreateCoordinatesCircle() functio.
TEST(ConvenienceTest, TestCreateCoordinatesCircle) {
  const double kLat = 0.0;
  const double kLng = 0.0;
  const double kRadius = 100;
  const size_t kSegments = 360;
  CoordinatesPtr coords = CreateCoordinatesCircle(kLat, kLng, kRadius,
                                                  kSegments);
  ASSERT_EQ(kSegments, coords->get_coordinates_array_size());
}

// This tests the CreateDataNameValue() function.
TEST(ConvenienceTest, TestCreateDataNameValue) {
  const string kName("par");
  const string kValue("5");
  DataPtr data = CreateDataNameValue(kName, kValue);
  ASSERT_TRUE(data);
  ASSERT_EQ(kName, data->get_name());
  ASSERT_EQ(kValue, data->get_value());
}

// This tests the CreateFlyToFromAbstractView function.
TEST(ConvenienceTest, TestCreateFlyTo) {
  const double kLat = 37.1;
  const double kLng = 122.2;
  const double kAlt = 123.4;
  const double kHeading = -32.1;
  const double kTilt = 89.9;
  const double kRange = 3456.7;
  const int kAltitudeMode = kmldom::ALTITUDEMODE_ABSOLUTE;
  const LookAtPtr lookat = CreateLookAt(kLat, kLng, kAlt, kHeading, kTilt,
                                        kRange, kAltitudeMode);
  const double duration = 38.3;
  const GxFlyToPtr flyto = CreateFlyTo(lookat, duration);

  ASSERT_TRUE(flyto);
  ASSERT_TRUE(flyto->has_abstractview());
  ASSERT_EQ(kmldom::Type_LookAt, flyto->get_abstractview()->Type());
  LookAtPtr flyto_lookat = kmldom::AsLookAt(flyto->get_abstractview());
  ASSERT_EQ(kLat, flyto_lookat->get_latitude());
  ASSERT_EQ(kLng, flyto_lookat->get_longitude());
  ASSERT_EQ(kAlt, flyto_lookat->get_altitude());
  ASSERT_EQ(kHeading, flyto_lookat->get_heading());
  ASSERT_EQ(kTilt, flyto_lookat->get_tilt());
  ASSERT_EQ(kRange, flyto_lookat->get_range());
  ASSERT_EQ(kAltitudeMode, flyto_lookat->get_altitudemode());
  ASSERT_TRUE(flyto->has_gx_duration());
  ASSERT_DOUBLE_EQ(duration, flyto->get_gx_duration());
}

// This tests the CreateFlyToForFeature function.
TEST(ConvenienceTest, TestCreateFlyToForFeature) {
  ASSERT_FALSE(CreateFlyToForFeature(NULL, 0));
  const double kLat = 37.0;
  const double kLng = -122.0;
  const CoordinatesPtr coords =
    CreateCoordinatesCircle(kLat, kLng, 50000, 360);
  ASSERT_TRUE(coords);
  LineStringPtr linestring = KmlFactory::GetFactory()->CreateLineString();
  linestring->set_coordinates(coords);
  PlacemarkPtr line_placemark = KmlFactory::GetFactory()->CreatePlacemark();
  line_placemark->set_geometry(linestring);
  const double kDuration = 34.2;
  const GxFlyToPtr flyto = CreateFlyToForFeature(line_placemark, kDuration);

  ASSERT_TRUE(flyto);
  ASSERT_TRUE(flyto->has_abstractview());
  ASSERT_EQ(kmldom::Type_LookAt, flyto->get_abstractview()->Type());
  LookAtPtr l = kmldom::AsLookAt(flyto->get_abstractview());
  ASSERT_DOUBLE_EQ(kLat, l->get_latitude());
  ASSERT_DOUBLE_EQ(kLng, l->get_longitude());
  ASSERT_DOUBLE_EQ(0.0, l->get_altitude());
  ASSERT_DOUBLE_EQ(0.0, l->get_heading());
  ASSERT_DOUBLE_EQ(0.0, l->get_tilt());
  ASSERT_NEAR(134721.0512, l->get_range(), 0.0001);
  ASSERT_EQ(kmldom::ALTITUDEMODE_RELATIVETOGROUND, l->get_altitudemode());
  ASSERT_TRUE(flyto->has_gx_duration());
  ASSERT_DOUBLE_EQ(kDuration, flyto->get_gx_duration());
}

// This tests the CreateLookAt() function.
TEST(ConvenienceTest, TestCreateLookAt) {
  const double kLatitude = 37.0;
  const double kLongitude = -122.0;
  const double kAltitude = 12.3;
  const double kHeading = 180.0;
  const double kTilt = 32.1;
  const double kRange = 1000.3;
  const kmldom::AltitudeModeEnum kAltitudeMode = kmldom::ALTITUDEMODE_ABSOLUTE;
  LookAtPtr lookat = CreateLookAt(kLatitude, kLongitude, kAltitude,
                                  kHeading, kTilt, kRange, kAltitudeMode);
  ASSERT_TRUE(lookat->has_longitude());
  ASSERT_TRUE(lookat->has_latitude());
  ASSERT_TRUE(lookat->has_altitude());
  ASSERT_TRUE(lookat->has_heading());
  ASSERT_TRUE(lookat->has_tilt());
  ASSERT_TRUE(lookat->has_range());
  ASSERT_TRUE(lookat->has_altitudemode());
  ASSERT_EQ(kLatitude, lookat->get_latitude());
  ASSERT_EQ(kLongitude, lookat->get_longitude());
  ASSERT_EQ(kAltitude, lookat->get_altitude());
  ASSERT_EQ(kHeading, lookat->get_heading());
  ASSERT_EQ(kTilt, lookat->get_tilt());
  ASSERT_EQ(kRange, lookat->get_range());
  ASSERT_EQ(kAltitudeMode, lookat->get_altitudemode());
}

// This tests the CreatePointFromLatLonAtts() function.
TEST(ConvenienceTest, TestCreatePointFromLatLonAtts) {
  const char* atts[] = { "lat", "38.38", "lon", "-121.456", NULL };
  PointPtr point = CreatePointFromLatLonAtts(atts);
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_coordinates());
  ASSERT_EQ(static_cast<size_t>(1),
                       point->get_coordinates()->get_coordinates_array_size());
  ASSERT_EQ(
    strtod(atts[1], NULL),
    point->get_coordinates()->get_coordinates_array_at(0).get_latitude());
  ASSERT_EQ(
    strtod(atts[3], NULL),
    point->get_coordinates()->get_coordinates_array_at(0).get_longitude());
  ASSERT_EQ(0.0,
    point->get_coordinates()->get_coordinates_array_at(0).get_altitude());
}

// This tests the CreatePointFromVec3() function.
TEST(ConvenienceTest, TestCreatePointFromVec3) {
  kmlbase::Vec3 vec3(1.1, 2.2, 3.3);
  PointPtr point = CreatePointFromVec3(vec3);
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_coordinates());
  ASSERT_EQ(static_cast<size_t>(1),
            point->get_coordinates()->get_coordinates_array_size());
  ASSERT_EQ(
    2.2, point->get_coordinates()->get_coordinates_array_at(0).get_latitude());
  ASSERT_EQ(
    1.1, point->get_coordinates()->get_coordinates_array_at(0).get_longitude());
  ASSERT_EQ(
    3.3, point->get_coordinates()->get_coordinates_array_at(0).get_altitude());
}

// This tests the CreatePointLatLon() function.
TEST(ConvenienceTest, TestCreatePointLatLon) {
  PointPtr point = CreatePointLatLon(-42.42, 150.160);
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_coordinates());
  ASSERT_EQ(static_cast<size_t>(1),
                       point->get_coordinates()->get_coordinates_array_size());
  ASSERT_EQ(
    -42.42,
    point->get_coordinates()->get_coordinates_array_at(0).get_latitude());
  ASSERT_EQ(
    150.160,
    point->get_coordinates()->get_coordinates_array_at(0).get_longitude());
  ASSERT_EQ(
    0.0, point->get_coordinates()->get_coordinates_array_at(0).get_altitude());
}

// This tests the CreatePointPlacemark() function.
TEST(ConvenienceTest, TestCreatePointPlacemark) {
  const string kName("my point placemark");
  const double kLat = 38.0987123;
  const double kLon = -123.123;
  PlacemarkPtr placemark = CreatePointPlacemark(kName, kLat, kLon);
  ASSERT_EQ(kName, placemark->get_name());
  ASSERT_TRUE(placemark->has_geometry());
  PointPtr point = kmldom::AsPoint(placemark->get_geometry());
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_coordinates());
  CoordinatesPtr coordinates = point->get_coordinates();
  ASSERT_EQ(static_cast<size_t>(1),
                       coordinates->get_coordinates_array_size());
  Vec3 vec = coordinates->get_coordinates_array_at(0);
  ASSERT_EQ(kLat, vec.get_latitude());
  ASSERT_EQ(kLon, vec.get_longitude());
}

// This tests the PointPlacemarkWithTimeStamp() function.
TEST(ConvenienceTest, TestCreatePointPlacemarkWithTimeStamp) {
  const string kWhen("2008-10-03T09:25:42Z");
  PointPtr point = KmlFactory::GetFactory()->CreatePoint();
  boost::scoped_ptr<DateTime> date_time(DateTime::Create(kWhen));
  const char* style_id("my-shared-style");
  PlacemarkPtr placemark = CreatePointPlacemarkWithTimeStamp(point, *date_time,
                                                             style_id);
  ASSERT_TRUE(placemark);
  ASSERT_TRUE(placemark->has_geometry());
  ASSERT_TRUE(AsPoint(placemark->get_geometry()));
  ASSERT_TRUE(placemark->has_timeprimitive());
  ASSERT_TRUE(AsTimeStamp(placemark->get_timeprimitive()));
  ASSERT_TRUE(placemark->has_styleurl());
}

// This tests the CreateRegion2d() function.
TEST(ConvenienceTest, TestCreateRegion2d) {
  const double kNorth(67.87);
  const double kSouth(-56.78);
  const double kEast(98.12);
  const double kWest(12.34);
  const double kMinlodpixels(128);
  const double kMaxlodpixels(512);
  RegionPtr region = CreateRegion2d(kNorth, kSouth, kEast, kWest,
                                    kMinlodpixels, kMaxlodpixels);
  ASSERT_TRUE(region->has_latlonaltbox());
  ASSERT_TRUE(region->has_lod());
  ASSERT_EQ(kNorth, region->get_latlonaltbox()->get_north());
  ASSERT_EQ(kSouth, region->get_latlonaltbox()->get_south());
  ASSERT_EQ(kEast, region->get_latlonaltbox()->get_east());
  ASSERT_EQ(kWest, region->get_latlonaltbox()->get_west());
  ASSERT_FALSE(region->get_latlonaltbox()->has_minaltitude());
  ASSERT_FALSE(region->get_latlonaltbox()->has_maxaltitude());
  ASSERT_FALSE(region->get_latlonaltbox()->has_altitudemode());
  ASSERT_EQ(kMinlodpixels, region->get_lod()->get_minlodpixels());
  ASSERT_EQ(kMaxlodpixels, region->get_lod()->get_maxlodpixels());
  ASSERT_FALSE(region->get_lod()->has_minfadeextent());
  ASSERT_FALSE(region->get_lod()->has_maxfadeextent());
}

TEST(ConvenienceTest, TestCreateWait) {
  const double kWait(12.3);
  GxWaitPtr wait = CreateWait(kWait);
  ASSERT_TRUE(wait);
  ASSERT_TRUE(wait->has_gx_duration());
  ASSERT_DOUBLE_EQ(kWait, wait->get_gx_duration());
}

// This tests the GetExtendedDataValue() function.
TEST(ConvenienceTest, TestGetExtendedDataValue) {
  const string kName("yardage");
  const string kValue("0");
  PlacemarkPtr placemark = CreatePointPlacemark("19", 38, -122);
  AddExtendedDataValue(kName, kValue, placemark);
  string value;
  ASSERT_TRUE(GetExtendedDataValue(placemark, kName, &value));
  ASSERT_EQ(kValue, value);
  const string kNoSuch("no-such-name");
  ASSERT_FALSE(GetExtendedDataValue(placemark, kNoSuch, &value));
}

// This tests the SetExtendedDataValue() function.
TEST(ConvenienceTest, TestSetExtendedDataValue) {
  const string kName0("name0");
  const string kValue0("value0");
  const string kName1("name1");
  const string kValue1("value1");
  PlacemarkPtr placemark = CreatePointPlacemark("19", 38, -122);
  SetExtendedDataValue(kName0, kValue0, placemark);
  string value;
  ASSERT_TRUE(GetExtendedDataValue(placemark, kName0, &value));
  ASSERT_EQ(kValue0, value);
  SetExtendedDataValue(kName1, kValue1, placemark);
  ASSERT_FALSE(GetExtendedDataValue(placemark, kName0, &value));
  ASSERT_TRUE(GetExtendedDataValue(placemark, kName1, &value));
  ASSERT_EQ(kValue1, value);
}

// This tests the SimplifyCoordinates() function.
TEST(ConvenienceTest, TestSimplifyCoordinates) {
  // For reference, the hypotenuse (in meters) of an N by N triangle at the
  // equator is:
  // N        = Distance
  // 0.000001 = 0.157147 m
  // 0.000010 = 1.571475 m
  // 0.000100 = 15.714748 m
  // 0.001000 = 157.147485 m
  // 0.010000 = 1571.474842 m
  // 0.100000 = 15714.744473 m

  CoordinatesPtr c1 = KmlFactory::GetFactory()->CreateCoordinates();
  c1->add_vec3(Vec3(0.000000, 0.000000, 0.000000));
  c1->add_vec3(Vec3(0.000001, 0.000001, 0.000001));
  c1->add_vec3(Vec3(0.000003, 0.000003, 0.000003));

  c1->add_vec3(Vec3(0.000030, 0.000030, 0.000030));
  c1->add_vec3(Vec3(0.000040, 0.000040, 0.000040));
  c1->add_vec3(Vec3(0.000070, 0.000070, 0.000070));

  c1->add_vec3(Vec3(0.000500, 0.000500, 0.000500));
  c1->add_vec3(Vec3(0.000700, 0.000700, 0.000700));
  c1->add_vec3(Vec3(0.000900, 0.000900, 0.000900));

  // This will not elide near-coincident points.
  CoordinatesPtr notmerged = KmlFactory::GetFactory()->CreateCoordinates();
  SimplifyCoordinates(c1, notmerged, 0.0);
  ASSERT_EQ(static_cast<size_t>(9), notmerged->get_coordinates_array_size()); 

  // This will elide the first and second coordinate tuples since they are less
  // than 0.2 meters apart.
  CoordinatesPtr merged1 = KmlFactory::GetFactory()->CreateCoordinates();
  SimplifyCoordinates(c1, merged1, 0.2);
  ASSERT_EQ(static_cast<size_t>(8), merged1->get_coordinates_array_size()); 
  // Assert that the second coordinate was the one elided.
  ASSERT_DOUBLE_EQ(
      0.0, merged1->get_coordinates_array_at(0).get_latitude());
  ASSERT_DOUBLE_EQ(
      0.000003, merged1->get_coordinates_array_at(1).get_latitude());
  ASSERT_DOUBLE_EQ(
      0.000030, merged1->get_coordinates_array_at(2).get_latitude());

  // A tolerance of 20 meters will elide the first 6 coordinates.
  CoordinatesPtr merged2 = KmlFactory::GetFactory()->CreateCoordinates();
  SimplifyCoordinates(c1, merged2, 20.0);
  ASSERT_EQ(static_cast<size_t>(4), merged2->get_coordinates_array_size()); 
  // Assert that the remaining coordinates.
  ASSERT_DOUBLE_EQ(
      0.0, merged2->get_coordinates_array_at(0).get_latitude());
  ASSERT_DOUBLE_EQ(
      0.0005, merged2->get_coordinates_array_at(1).get_latitude());
  ASSERT_DOUBLE_EQ(
      0.0007, merged2->get_coordinates_array_at(2).get_latitude());
  ASSERT_DOUBLE_EQ(
      0.0009, merged2->get_coordinates_array_at(3).get_latitude());

  // This distance is greater than the separation of all coordinate elements
  // and thus all elements after the first will be elided.
  CoordinatesPtr merged3 = KmlFactory::GetFactory()->CreateCoordinates();
  SimplifyCoordinates(c1, merged3, 200.0);
  ASSERT_EQ(static_cast<size_t>(1), merged3->get_coordinates_array_size()); 
  // Assert that the only coordinate remaining is the first. 
  ASSERT_DOUBLE_EQ(
      0.0, merged2->get_coordinates_array_at(0).get_latitude());
}

}  // end namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
