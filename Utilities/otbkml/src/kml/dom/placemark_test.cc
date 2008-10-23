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

// This file contains the unit tests for the Placemark element.

#include "kml/dom/placemark.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/serializer.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class PlacemarkTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(PlacemarkTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST(TestParse);
  CPPUNIT_TEST(TestSerialize);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    placemark_ = KmlFactory::GetFactory()->CreatePlacemark();
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
  void TestSerialize();

 private:
  PlacemarkPtr placemark_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(PlacemarkTest);

void PlacemarkTest::TestType() {
  CPPUNIT_ASSERT_EQUAL(Type_Placemark, placemark_->Type());
  CPPUNIT_ASSERT(true == placemark_->IsA(Type_Placemark));
  CPPUNIT_ASSERT(true == placemark_->IsA(Type_Feature));
  CPPUNIT_ASSERT(true == placemark_->IsA(Type_Object));
}

// Verify proper defaults:
void PlacemarkTest::TestDefaults() {
  CPPUNIT_ASSERT(false == placemark_->has_geometry());
  CPPUNIT_ASSERT(NULL == placemark_->get_geometry());
}

// Verify setting default makes has_xxx() true:
void PlacemarkTest::TestSetToDefaultValues() {
  TestDefaults();
  placemark_->set_geometry(NULL);  // should not crash
  CPPUNIT_ASSERT(false == placemark_->has_geometry());  // ptr is null
}

// Verify set, get, has, clear:
void PlacemarkTest::TestSetGetHasClear() {
  // Non-default values:
  PointPtr point = KmlFactory::GetFactory()->CreatePoint();

  // Set geometry to point:
  placemark_->set_geometry(point);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(placemark_->has_geometry());
  CPPUNIT_ASSERT_EQUAL(Type_Point, placemark_->get_geometry()->Type());

  // Set geometry to model.  This frees point.
  ModelPtr model= KmlFactory::GetFactory()->CreateModel();
  placemark_->set_geometry(model);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(placemark_->has_geometry());
  CPPUNIT_ASSERT_EQUAL(Type_Model, placemark_->get_geometry()->Type());

  // Clear all fields:
  placemark_->clear_geometry();

  // Verify now in default state:
  TestDefaults();
}

void PlacemarkTest::TestParse() {
  std::string kName = "My Favorite Place";
  std::string kSnippet = "Left panel stuff about my favorite place...";
  std::string kPlacemark =
    "<Placemark>"
    "<name>" + kName + "</name>"
    "<Snippet>" + kSnippet + "</Snippet>"
    "<Point><coordinates>1.1,2.2,3.3</coordinates></Point>"
    "</Placemark>";
  std::string errors;
  ElementPtr root = Parse(kPlacemark, &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const PlacemarkPtr placemark = AsPlacemark(root);
  CPPUNIT_ASSERT(placemark);
  CPPUNIT_ASSERT(false == placemark->has_id());
  CPPUNIT_ASSERT(false == placemark->has_targetid());
  CPPUNIT_ASSERT(placemark->has_name());
  CPPUNIT_ASSERT_EQUAL(kName, placemark->get_name());
  CPPUNIT_ASSERT(false == placemark->has_visibility());
  CPPUNIT_ASSERT(false == placemark->has_open());
  CPPUNIT_ASSERT(false == placemark->has_address());
  CPPUNIT_ASSERT(placemark->has_snippet());
  CPPUNIT_ASSERT(placemark->get_snippet()->has_text());
  CPPUNIT_ASSERT_EQUAL(kSnippet, placemark->get_snippet()->get_text());
  CPPUNIT_ASSERT(false == placemark->has_description());
  CPPUNIT_ASSERT(false == placemark->has_abstractview());
  CPPUNIT_ASSERT(false == placemark->has_timeprimitive());
  CPPUNIT_ASSERT(false == placemark->has_styleurl());
  CPPUNIT_ASSERT(false == placemark->has_styleselector());
  CPPUNIT_ASSERT(false == placemark->has_region());
  CPPUNIT_ASSERT(false == placemark->has_extendeddata());
  const PointPtr point = AsPoint(placemark->get_geometry());
  Vec3 vec3 = point->get_coordinates()->get_coordinates_array_at(0);
  CPPUNIT_ASSERT_EQUAL(1.1, vec3.get_longitude());
  CPPUNIT_ASSERT_EQUAL(2.2, vec3.get_latitude());
  CPPUNIT_ASSERT_EQUAL(3.3, vec3.get_altitude());
}

void PlacemarkTest::TestSerialize() {
  placemark_->set_name("name");
  placemark_->set_visibility(true);
  placemark_->set_description("description");
  placemark_->set_styleurl("#styleurl");
  placemark_->set_region(KmlFactory::GetFactory()->CreateRegion());
  placemark_->set_geometry(KmlFactory::GetFactory()->CreatePoint());

  std::string expected(
    "<Placemark>"
    "<name>name</name>"
    "<visibility>1</visibility>"
    "<description>description</description>"
    "<styleUrl>#styleurl</styleUrl>"
    "<Region/>"
    "<Point/>"
    "</Placemark>"
  );
  CPPUNIT_ASSERT_EQUAL(expected, SerializeRaw(placemark_));
}

}  // end namespace kmldom

TEST_MAIN
