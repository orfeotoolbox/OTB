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

// This file contains the unit tests for the IconStyle element.

#include "kml/dom/iconstyle.h"
#include "kml/dom/element.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/kmldom.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class IconStyleTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(IconStyleTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST(TestParse);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    iconstyle_ = KmlFactory::GetFactory()->CreateIconStyle();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();
  void TestParse();

 private:
  IconStylePtr iconstyle_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(IconStyleTest);

void IconStyleTest::TestType() {
  CPPUNIT_ASSERT(true == iconstyle_->IsA(Type_IconStyle));
  CPPUNIT_ASSERT(Type_IconStyle == iconstyle_->Type());
  CPPUNIT_ASSERT(true == iconstyle_->IsA(Type_ColorStyle));
}

// Verify proper defaults:
void IconStyleTest::TestDefaults() {
  CPPUNIT_ASSERT(false == iconstyle_->has_scale());
  CPPUNIT_ASSERT(1.0 == iconstyle_->get_scale());
  CPPUNIT_ASSERT(false == iconstyle_->has_heading());
  CPPUNIT_ASSERT(0.0 == iconstyle_->get_heading());
  CPPUNIT_ASSERT(false == iconstyle_->has_icon());
  CPPUNIT_ASSERT(NULL == iconstyle_->get_icon());
  CPPUNIT_ASSERT(false == iconstyle_->has_hotspot());
  CPPUNIT_ASSERT(NULL == iconstyle_->get_hotspot());
}

// Verify setting default makes has_xxx() true:
void IconStyleTest::TestSetToDefaultValues() {
  TestDefaults();
  iconstyle_->set_scale(iconstyle_->get_scale());
  CPPUNIT_ASSERT(true == iconstyle_->has_scale());
  iconstyle_->set_heading(iconstyle_->get_heading());
  CPPUNIT_ASSERT(true == iconstyle_->has_heading());
  iconstyle_->set_icon(NULL);
  CPPUNIT_ASSERT(false == iconstyle_->get_icon());
  iconstyle_->set_hotspot(NULL);
  // Note: setting to default sets pointer to NULL, so has_hotspot is false.
  // This is different to field behaviour where has_xxx is to equivalent to
  // "field has been set outside of ctor" and is true.
  CPPUNIT_ASSERT(false == iconstyle_->has_hotspot());
}

// Verify set, get, has, clear:
void IconStyleTest::TestSetGetHasClear() {
  // Non-default values:
  double scale = 1.0;
  double heading = 2.0;
  IconStyleIconPtr icon = KmlFactory::GetFactory()->CreateIconStyleIcon();
  HotSpotPtr hotspot = KmlFactory::GetFactory()->CreateHotSpot();

  // Set all fields:
  iconstyle_->set_scale(scale);
  iconstyle_->set_heading(heading);
  iconstyle_->set_icon(icon);
  iconstyle_->set_hotspot(hotspot);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == iconstyle_->has_scale());
  CPPUNIT_ASSERT(scale == iconstyle_->get_scale());
  CPPUNIT_ASSERT(true == iconstyle_->has_heading());
  CPPUNIT_ASSERT(heading == iconstyle_->get_heading());
  CPPUNIT_ASSERT(true == iconstyle_->has_icon());
  CPPUNIT_ASSERT(icon == iconstyle_->get_icon());
  CPPUNIT_ASSERT(true == iconstyle_->has_hotspot());
  CPPUNIT_ASSERT(hotspot == iconstyle_->get_hotspot());

  // Clear all fields:
  iconstyle_->clear_scale();
  iconstyle_->clear_heading();
  iconstyle_->clear_icon();
  iconstyle_->clear_hotspot();

  // Verify now in default state:
  TestDefaults();
}

// Verify parse
void IconStyleTest::TestParse() {
  CPPUNIT_ASSERT(false == iconstyle_->has_icon());
  const char* kIconStyleIcon =
    "<IconStyle>"
    "<Icon><href>image.jpg</href></Icon>"
    "</IconStyle>";
  std::string errors;
  ElementPtr root = Parse(kIconStyleIcon, &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const IconStylePtr iconstyle = AsIconStyle(root);
  CPPUNIT_ASSERT(iconstyle->has_icon());
  // Verify that this is IconStyle's brand of Icon (not Type_Icon).
  CPPUNIT_ASSERT_EQUAL(Type_IconStyleIcon, iconstyle->get_icon()->Type());
  CPPUNIT_ASSERT_EQUAL(std::string("image.jpg"),
                       iconstyle->get_icon()->get_href());
}

}  // end namespace kmldom

TEST_MAIN
