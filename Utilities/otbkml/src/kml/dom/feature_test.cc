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

// This file contains the unit test for the abstract Feature element.

#include "boost/intrusive_ptr.hpp"
#include "kml/dom/feature.h"
#include "kml/dom/kmldom.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class FeatureTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(FeatureTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    feature_ = new TestFeature();
  }
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  // Feature is abstract, hence its constructor is protected.
  class TestFeature : public Feature {
  };
  boost::intrusive_ptr<TestFeature> feature_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(FeatureTest);

void FeatureTest::TestType() {
  CPPUNIT_ASSERT(true == feature_->IsA(Type_Feature));
  CPPUNIT_ASSERT(true == feature_->IsA(Type_Object));
}

// Verify proper defaults:
void FeatureTest::TestDefaults() {
  CPPUNIT_ASSERT(false == feature_->has_name());
  CPPUNIT_ASSERT("" == feature_->get_name());
  CPPUNIT_ASSERT(false == feature_->has_visibility());
  CPPUNIT_ASSERT(true == feature_->get_visibility());
  CPPUNIT_ASSERT(false == feature_->has_open());
  CPPUNIT_ASSERT(false == feature_->get_open());
  CPPUNIT_ASSERT(false == feature_->has_address());
  CPPUNIT_ASSERT("" == feature_->get_address());
  CPPUNIT_ASSERT(false == feature_->has_phonenumber());
  CPPUNIT_ASSERT("" == feature_->get_phonenumber());
  CPPUNIT_ASSERT(false == feature_->has_snippet());
  CPPUNIT_ASSERT(NULL == feature_->get_snippet());
  CPPUNIT_ASSERT(false == feature_->has_description());
  CPPUNIT_ASSERT("" == feature_->get_description());
  CPPUNIT_ASSERT(false == feature_->has_abstractview());
  CPPUNIT_ASSERT(NULL == feature_->get_abstractview());
  CPPUNIT_ASSERT(false == feature_->has_timeprimitive());
  CPPUNIT_ASSERT(NULL == feature_->get_timeprimitive());
  CPPUNIT_ASSERT(false == feature_->has_styleurl());
  CPPUNIT_ASSERT("" == feature_->get_styleurl());
  CPPUNIT_ASSERT(false == feature_->has_styleselector());
  CPPUNIT_ASSERT(NULL == feature_->get_styleselector());
  CPPUNIT_ASSERT(false == feature_->has_region());
  CPPUNIT_ASSERT(NULL == feature_->get_region());
}

// Verify setting default makes has_xxx() true:
void FeatureTest::TestSetToDefaultValues() {
  TestDefaults();
  feature_->set_name(feature_->get_name());
  CPPUNIT_ASSERT(true == feature_->has_name());
  feature_->set_visibility(feature_->get_visibility());
  CPPUNIT_ASSERT(true == feature_->has_visibility());
  feature_->set_open(feature_->get_open());
  CPPUNIT_ASSERT(true == feature_->has_open());
  feature_->set_address(feature_->get_address());
  CPPUNIT_ASSERT(true == feature_->has_address());
  feature_->set_phonenumber(feature_->get_phonenumber());
  CPPUNIT_ASSERT(true == feature_->has_phonenumber());
  feature_->set_snippet(KmlFactory::GetFactory()->CreateSnippet());
  CPPUNIT_ASSERT(true == feature_->has_snippet());  // ptr is still null
  feature_->set_description(feature_->get_description());
  CPPUNIT_ASSERT(true == feature_->has_description());
  feature_->set_abstractview(KmlFactory::GetFactory()->CreateLookAt());
  CPPUNIT_ASSERT(true == feature_->has_abstractview());
  feature_->set_timeprimitive(KmlFactory::GetFactory()->CreateTimeSpan());
  CPPUNIT_ASSERT(true == feature_->has_timeprimitive());
  feature_->set_styleurl(feature_->get_styleurl());
  CPPUNIT_ASSERT(true == feature_->has_styleurl());
  feature_->set_styleselector(KmlFactory::GetFactory()->CreateStyle());
  CPPUNIT_ASSERT(true == feature_->has_styleselector());
  feature_->set_region(KmlFactory::GetFactory()->CreateRegion());
  CPPUNIT_ASSERT(true == feature_->has_region());
}

// Verify set, get, has, clear:
void FeatureTest::TestSetGetHasClear() {
  // Non-default values:
  std::string name("a");
  bool visibility = false;
  bool open = true;
  std::string address("b");
  std::string phonenumber("c");
  SnippetPtr snippet = KmlFactory::GetFactory()->CreateSnippet();
  std::string description("d");
  LookAtPtr lookat = KmlFactory::GetFactory()->CreateLookAt();
  TimeSpanPtr timespan = KmlFactory::GetFactory()->CreateTimeSpan();
  std::string styleurl("e");
  StylePtr style = KmlFactory::GetFactory()->CreateStyle();
  RegionPtr region = KmlFactory::GetFactory()->CreateRegion();

  // Set all fields:
  feature_->set_name(name);
  feature_->set_visibility(visibility);
  feature_->set_open(open);
  feature_->set_address(address);
  feature_->set_phonenumber(phonenumber);
  feature_->set_snippet(snippet);
  feature_->set_description(description);
  feature_->set_abstractview(lookat);
  feature_->set_timeprimitive(timespan);
  feature_->set_styleurl(styleurl);
  feature_->set_styleselector(style);
  feature_->set_region(region);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(name == feature_->get_name());
  CPPUNIT_ASSERT(true == feature_->has_name());
  CPPUNIT_ASSERT(visibility == feature_->get_visibility());
  CPPUNIT_ASSERT(true == feature_->has_visibility());
  CPPUNIT_ASSERT(open == feature_->get_open());
  CPPUNIT_ASSERT(true == feature_->has_open());
  CPPUNIT_ASSERT(address == feature_->get_address());
  CPPUNIT_ASSERT(true == feature_->has_address());
  CPPUNIT_ASSERT(phonenumber == feature_->get_phonenumber());
  CPPUNIT_ASSERT(true == feature_->has_phonenumber());
  CPPUNIT_ASSERT(snippet == feature_->get_snippet());
  CPPUNIT_ASSERT(true == feature_->has_snippet());
  CPPUNIT_ASSERT(description == feature_->get_description());
  CPPUNIT_ASSERT(true == feature_->has_description());
  CPPUNIT_ASSERT(lookat == feature_->get_abstractview());
  CPPUNIT_ASSERT(true == feature_->has_abstractview());
  CPPUNIT_ASSERT(timespan == feature_->get_timeprimitive());
  CPPUNIT_ASSERT(true == feature_->has_timeprimitive());
  CPPUNIT_ASSERT(styleurl == feature_->get_styleurl());
  CPPUNIT_ASSERT(true == feature_->has_styleurl());
  CPPUNIT_ASSERT(style == feature_->get_styleselector());
  CPPUNIT_ASSERT(true == feature_->has_styleselector());
  CPPUNIT_ASSERT(region == feature_->get_region());
  CPPUNIT_ASSERT(true == feature_->has_region());

  // Clear all fields:
  feature_->clear_name();
  feature_->clear_visibility();
  feature_->clear_open();
  feature_->clear_address();
  feature_->clear_phonenumber();
  feature_->clear_snippet();
  feature_->clear_description();
  feature_->clear_abstractview();
  feature_->clear_timeprimitive();
  feature_->clear_styleurl();
  feature_->clear_styleselector();
  feature_->clear_region();

  // Verify now in default state:
  TestDefaults();
}

}  // end namespace kmldom

TEST_MAIN
