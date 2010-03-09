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

// This file contains the unit tests for the LookAt and Camera elements.

#include "kml/dom/abstractview.h"
#include "gtest/gtest.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"

namespace kmldom {

class LookAtTest : public testing::Test {
 protected:
  virtual void SetUp() {
    lookat_ = KmlFactory::GetFactory()->CreateLookAt();
  }

  LookAtPtr lookat_;
};

TEST_F(LookAtTest, TestType) {
  ASSERT_EQ(Type_LookAt, lookat_->Type());
  ASSERT_TRUE(lookat_->IsA(Type_LookAt));
  ASSERT_TRUE(lookat_->IsA(Type_AbstractView));
  ASSERT_TRUE(lookat_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(LookAtTest, TestDefaults) {
  ASSERT_TRUE(NULL == lookat_->get_gx_timeprimitive());
  ASSERT_FALSE(lookat_->has_gx_timeprimitive());
  ASSERT_DOUBLE_EQ(0.0, lookat_->get_longitude());
  ASSERT_FALSE(lookat_->has_longitude());
  ASSERT_DOUBLE_EQ(0.0, lookat_->get_latitude());
  ASSERT_FALSE(lookat_->has_latitude());
  ASSERT_DOUBLE_EQ(0.0, lookat_->get_altitude());
  ASSERT_FALSE(lookat_->has_altitude());
  ASSERT_DOUBLE_EQ(0.0, lookat_->get_heading());
  ASSERT_FALSE(lookat_->has_heading());
  ASSERT_DOUBLE_EQ(0.0, lookat_->get_tilt());
  ASSERT_FALSE(lookat_->has_tilt());
  ASSERT_DOUBLE_EQ(0.0, lookat_->get_range());
  ASSERT_FALSE(lookat_->has_range());
  ASSERT_EQ(ALTITUDEMODE_CLAMPTOGROUND, lookat_->get_altitudemode());
  ASSERT_FALSE(lookat_->has_altitudemode());
  ASSERT_EQ(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR, lookat_->get_gx_altitudemode());
  ASSERT_FALSE(lookat_->has_gx_altitudemode());
}

// Verify setting default makes has_xxx() true:
TEST_F(LookAtTest, TestSetToDefaultValues) {
  lookat_->set_gx_timeprimitive(lookat_->get_gx_timeprimitive());
  ASSERT_FALSE(lookat_->has_gx_timeprimitive());
  lookat_->set_longitude(lookat_->get_longitude());
  ASSERT_TRUE(lookat_->has_longitude());
  lookat_->set_latitude(lookat_->get_latitude());
  ASSERT_TRUE(lookat_->has_latitude());
  lookat_->set_altitude(lookat_->get_altitude());
  ASSERT_TRUE(lookat_->has_altitude());
  lookat_->set_heading(lookat_->get_heading());
  ASSERT_TRUE(lookat_->has_heading());
  lookat_->set_tilt(lookat_->get_tilt());
  ASSERT_TRUE(lookat_->has_tilt());
  lookat_->set_range(lookat_->get_range());
  ASSERT_TRUE(lookat_->has_range());
  lookat_->set_altitudemode(lookat_->get_altitudemode());
  ASSERT_TRUE(lookat_->has_altitudemode());
  lookat_->set_gx_altitudemode(lookat_->get_gx_altitudemode());
  ASSERT_TRUE(lookat_->has_gx_altitudemode());
}

// Verify set, get, has, clear:
TEST_F(LookAtTest, TestSetGetHasClear) {
  // Non-default values:
  GxTimeSpanPtr gx_timespan = KmlFactory::GetFactory()->CreateGxTimeSpan();
  double longitude = 1.0;
  double latitude = 2.0;
  double altitude = 3.0;
  double heading = 4.0;
  double tilt = 5.0;
  double range = 6.0;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_ABSOLUTE;
  GxAltitudeModeEnum gx_altitudemode = GX_ALTITUDEMODE_RELATIVETOSEAFLOOR;

  // Set all fields:
  lookat_->set_gx_timeprimitive(gx_timespan);
  lookat_->set_longitude(longitude);
  lookat_->set_latitude(latitude);
  lookat_->set_altitude(altitude);
  lookat_->set_heading(heading);
  lookat_->set_tilt(tilt);
  lookat_->set_range(range);
  lookat_->set_altitudemode(altitudemode);
  lookat_->set_gx_altitudemode(gx_altitudemode);

  // Verify getter and has_xxx():
  ASSERT_EQ(gx_timespan, lookat_->get_gx_timeprimitive());
  ASSERT_TRUE(lookat_->has_gx_timeprimitive());
  ASSERT_DOUBLE_EQ(longitude, lookat_->get_longitude());
  ASSERT_TRUE(lookat_->has_longitude());
  ASSERT_DOUBLE_EQ(latitude, lookat_->get_latitude());
  ASSERT_TRUE(lookat_->has_latitude());
  ASSERT_DOUBLE_EQ(altitude, lookat_->get_altitude());
  ASSERT_TRUE(lookat_->has_altitude());
  ASSERT_DOUBLE_EQ(heading, lookat_->get_heading());
  ASSERT_TRUE(lookat_->has_heading());
  ASSERT_DOUBLE_EQ(tilt, lookat_->get_tilt());
  ASSERT_TRUE(lookat_->has_tilt());
  ASSERT_DOUBLE_EQ(range, lookat_->get_range());
  ASSERT_TRUE(lookat_->has_range());
  ASSERT_DOUBLE_EQ(altitudemode, lookat_->get_altitudemode());
  ASSERT_TRUE(lookat_->has_altitudemode());
  ASSERT_DOUBLE_EQ(gx_altitudemode, lookat_->get_gx_altitudemode());
  ASSERT_TRUE(lookat_->has_gx_altitudemode());

  // Clear all fields:
  lookat_->clear_gx_timeprimitive();
  lookat_->clear_longitude();
  lookat_->clear_latitude();
  lookat_->clear_altitude();
  lookat_->clear_heading();
  lookat_->clear_tilt();
  lookat_->clear_range();
  lookat_->clear_altitudemode();
  lookat_->clear_gx_altitudemode();

  // Verify default state:
  ASSERT_TRUE(NULL == lookat_->get_gx_timeprimitive());
  ASSERT_FALSE(lookat_->has_gx_timeprimitive());
  ASSERT_DOUBLE_EQ(0.0, lookat_->get_longitude());
  ASSERT_FALSE(lookat_->has_longitude());
  ASSERT_DOUBLE_EQ(0.0, lookat_->get_latitude());
  ASSERT_FALSE(lookat_->has_latitude());
  ASSERT_DOUBLE_EQ(0.0, lookat_->get_altitude());
  ASSERT_FALSE(lookat_->has_altitude());
  ASSERT_DOUBLE_EQ(0.0, lookat_->get_heading());
  ASSERT_FALSE(lookat_->has_heading());
  ASSERT_DOUBLE_EQ(0.0, lookat_->get_tilt());
  ASSERT_FALSE(lookat_->has_tilt());
  ASSERT_DOUBLE_EQ(0.0, lookat_->get_range());
  ASSERT_FALSE(lookat_->has_range());
  ASSERT_EQ(ALTITUDEMODE_CLAMPTOGROUND, lookat_->get_altitudemode());
  ASSERT_FALSE(lookat_->has_altitudemode());
  ASSERT_EQ(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR, lookat_->get_gx_altitudemode());
  ASSERT_FALSE(lookat_->has_gx_altitudemode());
}

class CameraTest : public testing::Test {
 protected:
  virtual void SetUp() {
    camera_ = KmlFactory::GetFactory()->CreateCamera();
  }

  CameraPtr camera_;
};

TEST_F(CameraTest, TestType) {
  ASSERT_EQ(Type_Camera, camera_->Type());
  ASSERT_TRUE(camera_->IsA(Type_Camera));
  ASSERT_TRUE(camera_->IsA(Type_AbstractView));
  ASSERT_TRUE(camera_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(CameraTest, TestDefaults) {
  ASSERT_TRUE(NULL == camera_->get_gx_timeprimitive());
  ASSERT_FALSE(camera_->has_gx_timeprimitive());
  ASSERT_DOUBLE_EQ(0.0, camera_->get_longitude());
  ASSERT_FALSE(camera_->has_longitude());
  ASSERT_DOUBLE_EQ(0.0, camera_->get_latitude());
  ASSERT_FALSE(camera_->has_latitude());
  ASSERT_DOUBLE_EQ(0.0, camera_->get_altitude());
  ASSERT_FALSE(camera_->has_altitude());
  ASSERT_DOUBLE_EQ(0.0, camera_->get_heading());
  ASSERT_FALSE(camera_->has_heading());
  ASSERT_DOUBLE_EQ(0.0, camera_->get_tilt());
  ASSERT_FALSE(camera_->has_tilt());
  ASSERT_DOUBLE_EQ(0.0, camera_->get_roll());
  ASSERT_FALSE(camera_->has_roll());
  ASSERT_EQ(ALTITUDEMODE_CLAMPTOGROUND, camera_->get_altitudemode());
  ASSERT_FALSE(camera_->has_altitudemode());
  ASSERT_EQ(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR, camera_->get_gx_altitudemode());
  ASSERT_FALSE(camera_->has_gx_altitudemode());
}

// Verify setting default makes has_xxx() true:
TEST_F(CameraTest, TestSetToDefaultValues) {
  camera_->set_gx_timeprimitive(camera_->get_gx_timeprimitive());
  ASSERT_FALSE(camera_->has_gx_timeprimitive());
  camera_->set_longitude(camera_->get_longitude());
  ASSERT_TRUE(camera_->has_longitude());
  camera_->set_latitude(camera_->get_latitude());
  ASSERT_TRUE(camera_->has_latitude());
  camera_->set_altitude(camera_->get_altitude());
  ASSERT_TRUE(camera_->has_altitude());
  camera_->set_heading(camera_->get_heading());
  ASSERT_TRUE(camera_->has_heading());
  camera_->set_tilt(camera_->get_tilt());
  ASSERT_TRUE(camera_->has_tilt());
  camera_->set_roll(camera_->get_roll());
  ASSERT_TRUE(camera_->has_roll());
  camera_->set_altitudemode(camera_->get_altitudemode());
  ASSERT_TRUE(camera_->has_altitudemode());
  camera_->set_gx_altitudemode(camera_->get_gx_altitudemode());
  ASSERT_TRUE(camera_->has_gx_altitudemode());
}

// Verify set, get, has, clear:
TEST_F(CameraTest, TestSetGetHasClear) {
  // Non-default values:
  GxTimeStampPtr gx_timestamp = KmlFactory::GetFactory()->CreateGxTimeStamp();
  double longitude = 1.0;
  double latitude = 2.0;
  double altitude = 3.0;
  double heading = 4.0;
  double tilt = 5.0;
  double roll = 6.0;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_ABSOLUTE;
  GxAltitudeModeEnum gx_altitudemode = GX_ALTITUDEMODE_RELATIVETOSEAFLOOR;

  // Set all fields:
  camera_->set_gx_timeprimitive(gx_timestamp);
  camera_->set_longitude(longitude);
  camera_->set_latitude(latitude);
  camera_->set_altitude(altitude);
  camera_->set_heading(heading);
  camera_->set_tilt(tilt);
  camera_->set_roll(roll);
  camera_->set_altitudemode(altitudemode);
  camera_->set_gx_altitudemode(gx_altitudemode);

  // Verify getter and has_xxx():
  ASSERT_EQ(gx_timestamp, camera_->get_gx_timeprimitive());
  ASSERT_TRUE(camera_->has_gx_timeprimitive());
  ASSERT_DOUBLE_EQ(longitude, camera_->get_longitude());
  ASSERT_TRUE(camera_->has_longitude());
  ASSERT_DOUBLE_EQ(latitude, camera_->get_latitude());
  ASSERT_TRUE(camera_->has_latitude());
  ASSERT_DOUBLE_EQ(altitude, camera_->get_altitude());
  ASSERT_TRUE(camera_->has_altitude());
  ASSERT_DOUBLE_EQ(heading, camera_->get_heading());
  ASSERT_TRUE(camera_->has_heading());
  ASSERT_DOUBLE_EQ(tilt, camera_->get_tilt());
  ASSERT_TRUE(camera_->has_tilt());
  ASSERT_DOUBLE_EQ(roll, camera_->get_roll());
  ASSERT_TRUE(camera_->has_roll());
  ASSERT_DOUBLE_EQ(altitudemode, camera_->get_altitudemode());
  ASSERT_TRUE(camera_->has_altitudemode());
  ASSERT_DOUBLE_EQ(gx_altitudemode, camera_->get_gx_altitudemode());
  ASSERT_TRUE(camera_->has_gx_altitudemode());

  // Clear all fields:
  camera_->clear_gx_timeprimitive();
  camera_->clear_longitude();
  camera_->clear_latitude();
  camera_->clear_altitude();
  camera_->clear_heading();
  camera_->clear_tilt();
  camera_->clear_roll();
  camera_->clear_altitudemode();
  camera_->clear_gx_altitudemode();

  // Verify default state.
  ASSERT_TRUE(NULL == camera_->get_gx_timeprimitive());
  ASSERT_FALSE(camera_->has_gx_timeprimitive());
  ASSERT_DOUBLE_EQ(0.0, camera_->get_longitude());
  ASSERT_FALSE(camera_->has_longitude());
  ASSERT_DOUBLE_EQ(0.0, camera_->get_latitude());
  ASSERT_FALSE(camera_->has_latitude());
  ASSERT_DOUBLE_EQ(0.0, camera_->get_altitude());
  ASSERT_FALSE(camera_->has_altitude());
  ASSERT_DOUBLE_EQ(0.0, camera_->get_heading());
  ASSERT_FALSE(camera_->has_heading());
  ASSERT_DOUBLE_EQ(0.0, camera_->get_tilt());
  ASSERT_FALSE(camera_->has_tilt());
  ASSERT_DOUBLE_EQ(0.0, camera_->get_roll());
  ASSERT_FALSE(camera_->has_roll());
  ASSERT_EQ(ALTITUDEMODE_CLAMPTOGROUND, camera_->get_altitudemode());
  ASSERT_FALSE(camera_->has_altitudemode());
  ASSERT_EQ(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR, camera_->get_gx_altitudemode());
  ASSERT_FALSE(camera_->has_gx_altitudemode());
}

TEST(AbstractViewTest, TestParseSerializeGxAltitudeMode) {
  KmlFactory* factory = KmlFactory::GetFactory();
  CameraPtr camera = factory->CreateCamera();
  ASSERT_TRUE(camera);
  camera->set_latitude(37.0);
  camera->set_longitude(-122.0);
  camera->set_gx_altitudemode(GX_ALTITUDEMODE_RELATIVETOSEAFLOOR);
  const string kCameraExpected =
    "<Camera>"
    "<longitude>-122</longitude>"
    "<latitude>37</latitude>"
    "<gx:altitudeMode>relativeToSeaFloor</gx:altitudeMode>"
    "</Camera>";
  ASSERT_EQ(kCameraExpected, SerializeRaw(camera));
  ASSERT_EQ(kCameraExpected, SerializeRaw(Parse(kCameraExpected, NULL)));

  LookAtPtr lookat = factory->CreateLookAt();
  ASSERT_TRUE(lookat);
  lookat->set_latitude(37.0);
  lookat->set_longitude(-122.0);
  lookat->set_gx_altitudemode(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR);
  const string kLookAtExpected =
    "<LookAt>"
    "<longitude>-122</longitude>"
    "<latitude>37</latitude>"
    "<gx:altitudeMode>clampToSeaFloor</gx:altitudeMode>"
    "</LookAt>";
  ASSERT_EQ(kLookAtExpected, SerializeRaw(lookat));
  ASSERT_EQ(kLookAtExpected, SerializeRaw(Parse(kLookAtExpected, NULL)));
}

TEST(AbstractViewTest, TestGxTimePrimitives) {
  KmlFactory* factory = KmlFactory::GetFactory();

  CameraPtr camera = factory->CreateCamera();
  ASSERT_TRUE(camera);
  camera->set_latitude(37.0);
  camera->set_longitude(-122.0);
  GxTimeSpanPtr gx_timespan = factory->CreateGxTimeSpan();
  camera->set_gx_timeprimitive(gx_timespan);
  const string kCameraExpected =
    "<Camera>"
    "<gx:TimeSpan/>"
    "<longitude>-122</longitude>"
    "<latitude>37</latitude>"
    "</Camera>";
  ASSERT_EQ(kCameraExpected, SerializeRaw(camera));
  ASSERT_EQ(kCameraExpected, SerializeRaw(Parse(kCameraExpected, NULL)));

  LookAtPtr lookat = factory->CreateLookAt();
  ASSERT_TRUE(lookat);
  lookat->set_latitude(37.0);
  lookat->set_longitude(-122.0);
  GxTimeStampPtr gx_timestamp = factory->CreateGxTimeStamp();
  lookat->set_gx_timeprimitive(gx_timestamp);
  const string kLookAtExpected =
    "<LookAt>"
    "<gx:TimeStamp/>"
    "<longitude>-122</longitude>"
    "<latitude>37</latitude>"
    "</LookAt>";
  ASSERT_EQ(kLookAtExpected, SerializeRaw(lookat));
  ASSERT_EQ(kLookAtExpected, SerializeRaw(Parse(kLookAtExpected, NULL)));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
