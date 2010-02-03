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
#include "kml/base/unit_test.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"

namespace kmldom {

class LookAtTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(LookAtTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    lookat_ = KmlFactory::GetFactory()->CreateLookAt();
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
  LookAtPtr lookat_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(LookAtTest);

void LookAtTest::TestType() {
  CPPUNIT_ASSERT(Type_LookAt == lookat_->Type());
  CPPUNIT_ASSERT(true == lookat_->IsA(Type_LookAt));
  CPPUNIT_ASSERT(true == lookat_->IsA(Type_AbstractView));
  CPPUNIT_ASSERT(true == lookat_->IsA(Type_Object));
}

// Verify proper defaults:
void LookAtTest::TestDefaults() {
  CPPUNIT_ASSERT(0.0 == lookat_->get_longitude());
  CPPUNIT_ASSERT(false == lookat_->has_longitude());
  CPPUNIT_ASSERT(0.0 == lookat_->get_latitude());
  CPPUNIT_ASSERT(false == lookat_->has_latitude());
  CPPUNIT_ASSERT(0.0 == lookat_->get_altitude());
  CPPUNIT_ASSERT(false == lookat_->has_altitude());
  CPPUNIT_ASSERT(0.0 == lookat_->get_heading());
  CPPUNIT_ASSERT(false == lookat_->has_heading());
  CPPUNIT_ASSERT(0.0 == lookat_->get_tilt());
  CPPUNIT_ASSERT(false == lookat_->has_tilt());
  CPPUNIT_ASSERT(0.0 == lookat_->get_range());
  CPPUNIT_ASSERT(false == lookat_->has_range());
  CPPUNIT_ASSERT(ALTITUDEMODE_CLAMPTOGROUND == lookat_->get_altitudemode());
  CPPUNIT_ASSERT(false == lookat_->has_altitudemode());
}

// Verify setting default makes has_xxx() true:
void LookAtTest::TestSetToDefaultValues() {
  TestDefaults();
  lookat_->set_longitude(lookat_->get_longitude());
  CPPUNIT_ASSERT(true == lookat_->has_longitude());
  lookat_->set_latitude(lookat_->get_latitude());
  CPPUNIT_ASSERT(true == lookat_->has_latitude());
  lookat_->set_altitude(lookat_->get_altitude());
  CPPUNIT_ASSERT(true == lookat_->has_altitude());
  lookat_->set_heading(lookat_->get_heading());
  CPPUNIT_ASSERT(true == lookat_->has_heading());
  lookat_->set_tilt(lookat_->get_tilt());
  CPPUNIT_ASSERT(true == lookat_->has_tilt());
  lookat_->set_range(lookat_->get_range());
  CPPUNIT_ASSERT(true == lookat_->has_range());
  lookat_->set_altitudemode(lookat_->get_altitudemode());
  CPPUNIT_ASSERT(true == lookat_->has_altitudemode());
}

// Verify set, get, has, clear:
void LookAtTest::TestSetGetHasClear() {
  // Non-default values:
  double longitude = 1.0;
  double latitude = 2.0;
  double altitude = 3.0;
  double heading = 4.0;
  double tilt = 5.0;
  double range = 6.0;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_ABSOLUTE;

  // Set all fields:
  lookat_->set_longitude(longitude);
  lookat_->set_latitude(latitude);
  lookat_->set_altitude(altitude);
  lookat_->set_heading(heading);
  lookat_->set_tilt(tilt);
  lookat_->set_range(range);
  lookat_->set_altitudemode(altitudemode);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(longitude == lookat_->get_longitude());
  CPPUNIT_ASSERT(true == lookat_->has_longitude());
  CPPUNIT_ASSERT(latitude == lookat_->get_latitude());
  CPPUNIT_ASSERT(true == lookat_->has_latitude());
  CPPUNIT_ASSERT(altitude == lookat_->get_altitude());
  CPPUNIT_ASSERT(true == lookat_->has_altitude());
  CPPUNIT_ASSERT(heading == lookat_->get_heading());
  CPPUNIT_ASSERT(true == lookat_->has_heading());
  CPPUNIT_ASSERT(tilt == lookat_->get_tilt());
  CPPUNIT_ASSERT(true == lookat_->has_tilt());
  CPPUNIT_ASSERT(range == lookat_->get_range());
  CPPUNIT_ASSERT(true == lookat_->has_range());
  CPPUNIT_ASSERT(altitudemode == lookat_->get_altitudemode());
  CPPUNIT_ASSERT(true == lookat_->has_altitudemode());

  // Clear all fields:
  lookat_->clear_longitude();
  lookat_->clear_latitude();
  lookat_->clear_altitude();
  lookat_->clear_heading();
  lookat_->clear_tilt();
  lookat_->clear_range();
  lookat_->clear_altitudemode();

  // Verify now in default state:
  TestDefaults();
}

class CameraTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(CameraTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    camera_ = KmlFactory::GetFactory()->CreateCamera();
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
  CameraPtr camera_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CameraTest);

void CameraTest::TestType() {
  CPPUNIT_ASSERT(Type_Camera == camera_->Type());
  CPPUNIT_ASSERT(true == camera_->IsA(Type_Camera));
  CPPUNIT_ASSERT(true == camera_->IsA(Type_AbstractView));
  CPPUNIT_ASSERT(true == camera_->IsA(Type_Object));
}

// Verify proper defaults:
void CameraTest::TestDefaults() {
  CPPUNIT_ASSERT(0.0 == camera_->get_longitude());
  CPPUNIT_ASSERT(false == camera_->has_longitude());
  CPPUNIT_ASSERT(0.0 == camera_->get_latitude());
  CPPUNIT_ASSERT(false == camera_->has_latitude());
  CPPUNIT_ASSERT(0.0 == camera_->get_altitude());
  CPPUNIT_ASSERT(false == camera_->has_altitude());
  CPPUNIT_ASSERT(0.0 == camera_->get_heading());
  CPPUNIT_ASSERT(false == camera_->has_heading());
  CPPUNIT_ASSERT(0.0 == camera_->get_tilt());
  CPPUNIT_ASSERT(false == camera_->has_tilt());
  CPPUNIT_ASSERT(0.0 == camera_->get_roll());
  CPPUNIT_ASSERT(false == camera_->has_roll());
  CPPUNIT_ASSERT(ALTITUDEMODE_CLAMPTOGROUND == camera_->get_altitudemode());
  CPPUNIT_ASSERT(false == camera_->has_altitudemode());
}

// Verify setting default makes has_xxx() true:
void CameraTest::TestSetToDefaultValues() {
  TestDefaults();
  camera_->set_longitude(camera_->get_longitude());
  CPPUNIT_ASSERT(true == camera_->has_longitude());
  camera_->set_latitude(camera_->get_latitude());
  CPPUNIT_ASSERT(true == camera_->has_latitude());
  camera_->set_altitude(camera_->get_altitude());
  CPPUNIT_ASSERT(true == camera_->has_altitude());
  camera_->set_heading(camera_->get_heading());
  CPPUNIT_ASSERT(true == camera_->has_heading());
  camera_->set_tilt(camera_->get_tilt());
  CPPUNIT_ASSERT(true == camera_->has_tilt());
  camera_->set_roll(camera_->get_roll());
  CPPUNIT_ASSERT(true == camera_->has_roll());
  camera_->set_altitudemode(camera_->get_altitudemode());
  CPPUNIT_ASSERT(true == camera_->has_altitudemode());
}

// Verify set, get, has, clear:
void CameraTest::TestSetGetHasClear() {
  // Non-default values:
  double longitude = 1.0;
  double latitude = 2.0;
  double altitude = 3.0;
  double heading = 4.0;
  double tilt = 5.0;
  double roll = 6.0;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_ABSOLUTE;

  // Set all fields:
  camera_->set_longitude(longitude);
  camera_->set_latitude(latitude);
  camera_->set_altitude(altitude);
  camera_->set_heading(heading);
  camera_->set_tilt(tilt);
  camera_->set_roll(roll);
  camera_->set_altitudemode(altitudemode);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(longitude == camera_->get_longitude());
  CPPUNIT_ASSERT(true == camera_->has_longitude());
  CPPUNIT_ASSERT(latitude == camera_->get_latitude());
  CPPUNIT_ASSERT(true == camera_->has_latitude());
  CPPUNIT_ASSERT(altitude == camera_->get_altitude());
  CPPUNIT_ASSERT(true == camera_->has_altitude());
  CPPUNIT_ASSERT(heading == camera_->get_heading());
  CPPUNIT_ASSERT(true == camera_->has_heading());
  CPPUNIT_ASSERT(tilt == camera_->get_tilt());
  CPPUNIT_ASSERT(true == camera_->has_tilt());
  CPPUNIT_ASSERT(roll == camera_->get_roll());
  CPPUNIT_ASSERT(true == camera_->has_roll());
  CPPUNIT_ASSERT(altitudemode == camera_->get_altitudemode());
  CPPUNIT_ASSERT(true == camera_->has_altitudemode());

  // Clear all fields:
  camera_->clear_longitude();
  camera_->clear_latitude();
  camera_->clear_altitude();
  camera_->clear_heading();
  camera_->clear_tilt();
  camera_->clear_roll();
  camera_->clear_altitudemode();

  // Verify now in default state:
  TestDefaults();
}

}  // end namespace kmldom

TEST_MAIN
