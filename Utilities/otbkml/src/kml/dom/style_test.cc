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

// This file contains the unit test for the Style element.

#include "kml/dom/style.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class StyleTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(StyleTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST(TestSetParent);
  CPPUNIT_TEST(TestSerialize);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    style_ = KmlFactory::GetFactory()->CreateStyle();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();
  void TestSetParent();
  void TestSerialize();

 private:
  StylePtr style_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(StyleTest);

void StyleTest::TestType() {
  CPPUNIT_ASSERT(true == style_->IsA(Type_Style));
  CPPUNIT_ASSERT(Type_Style == style_->Type());
  CPPUNIT_ASSERT(true == style_->IsA(Type_StyleSelector));
  CPPUNIT_ASSERT(true == style_->IsA(Type_Object));
}

// Verify proper defaults:
void StyleTest::TestDefaults() {
  CPPUNIT_ASSERT(false == style_->has_iconstyle());
  CPPUNIT_ASSERT(NULL == style_->get_iconstyle());
  CPPUNIT_ASSERT(false == style_->has_labelstyle());
  CPPUNIT_ASSERT(NULL == style_->get_labelstyle());
  CPPUNIT_ASSERT(false == style_->has_linestyle());
  CPPUNIT_ASSERT(NULL == style_->get_linestyle());
  CPPUNIT_ASSERT(false == style_->has_polystyle());
  CPPUNIT_ASSERT(NULL == style_->get_polystyle());
  CPPUNIT_ASSERT(false == style_->has_balloonstyle());
  CPPUNIT_ASSERT(NULL == style_->get_balloonstyle());
  CPPUNIT_ASSERT(false == style_->has_liststyle());
  CPPUNIT_ASSERT(NULL == style_->get_liststyle());
}

// Verify setting default makes has_xxx() true:
void StyleTest::TestSetToDefaultValues() {
  TestDefaults();
  style_->set_iconstyle(NULL);  // should not crash
  CPPUNIT_ASSERT(false == style_->has_iconstyle());  // ptr is null
  style_->set_labelstyle(NULL);
  CPPUNIT_ASSERT(false == style_->has_labelstyle());
  style_->set_linestyle(NULL);
  CPPUNIT_ASSERT(false == style_->has_linestyle());
  style_->set_polystyle(NULL);
  CPPUNIT_ASSERT(false == style_->has_polystyle());
  style_->set_balloonstyle(NULL);
  CPPUNIT_ASSERT(false == style_->has_balloonstyle());
  style_->set_liststyle(NULL);
  CPPUNIT_ASSERT(false == style_->has_liststyle());
}

// Verify set, get, has, clear:
void StyleTest::TestSetGetHasClear() {
  // Non-default values:
  IconStylePtr iconstyle = KmlFactory::GetFactory()->CreateIconStyle();
  LabelStylePtr labelstyle = KmlFactory::GetFactory()->CreateLabelStyle();
  LineStylePtr linestyle = KmlFactory::GetFactory()->CreateLineStyle();
  PolyStylePtr polystyle = KmlFactory::GetFactory()->CreatePolyStyle();
  BalloonStylePtr balloonstyle = KmlFactory::GetFactory()->CreateBalloonStyle();
  ListStylePtr liststyle = KmlFactory::GetFactory()->CreateListStyle();

  // Set all fields:
  style_->set_iconstyle(iconstyle);
  style_->set_labelstyle(labelstyle);
  style_->set_linestyle(linestyle);
  style_->set_polystyle(polystyle);
  style_->set_balloonstyle(balloonstyle);
  style_->set_liststyle(liststyle);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == style_->has_iconstyle());
  CPPUNIT_ASSERT(iconstyle == style_->get_iconstyle());
  CPPUNIT_ASSERT(true == style_->has_labelstyle());
  CPPUNIT_ASSERT(labelstyle == style_->get_labelstyle());
  CPPUNIT_ASSERT(true == style_->has_linestyle());
  CPPUNIT_ASSERT(linestyle == style_->get_linestyle());
  CPPUNIT_ASSERT(true == style_->has_polystyle());
  CPPUNIT_ASSERT(polystyle == style_->get_polystyle());
  CPPUNIT_ASSERT(true == style_->has_balloonstyle());
  CPPUNIT_ASSERT(balloonstyle == style_->get_balloonstyle());
  CPPUNIT_ASSERT(true == style_->has_liststyle());
  CPPUNIT_ASSERT(liststyle == style_->get_liststyle());

  // Clear all fields:
  style_->clear_iconstyle();
  style_->clear_labelstyle();
  style_->clear_polystyle();
  style_->clear_linestyle();
  style_->clear_balloonstyle();
  style_->clear_liststyle();

  // Verify now in default state:
  TestDefaults();
}

// Verify that 2 Styles can't take the same IconStyle, LabelStyle, LineStyle,
// PolyStyle, BalloonStyle or ListStyle.
// (This tests the internal set_parent() method.)
void StyleTest::TestSetParent() {
  KmlFactory* factory = KmlFactory::GetFactory();
  IconStylePtr iconstyle = factory->CreateIconStyle();
  LabelStylePtr labelstyle = factory->CreateLabelStyle();
  LineStylePtr linestyle = factory->CreateLineStyle();
  PolyStylePtr polystyle = factory->CreatePolyStyle();
  BalloonStylePtr balloonstyle = factory->CreateBalloonStyle();
  ListStylePtr liststyle = factory->CreateListStyle();

  // Give these all to style_.
  style_->set_iconstyle(iconstyle);
  style_->set_labelstyle(labelstyle);
  style_->set_linestyle(linestyle);
  style_->set_polystyle(polystyle);
  style_->set_balloonstyle(balloonstyle);
  style_->set_liststyle(liststyle);

  // Try to give these all to another style.
  StylePtr style2 = factory->CreateStyle();
  style2->set_iconstyle(iconstyle);
  style2->set_labelstyle(labelstyle);
  style2->set_linestyle(linestyle);
  style2->set_polystyle(polystyle);
  style2->set_balloonstyle(balloonstyle);
  style2->set_liststyle(liststyle);

  // Verify that style_ has each child.
  CPPUNIT_ASSERT(style_->has_iconstyle());
  CPPUNIT_ASSERT(style_->has_labelstyle());
  CPPUNIT_ASSERT(style_->has_linestyle());
  CPPUNIT_ASSERT(style_->has_polystyle());
  CPPUNIT_ASSERT(style_->has_balloonstyle());
  CPPUNIT_ASSERT(style_->has_liststyle());

  // Verify that style2 has no children.
  CPPUNIT_ASSERT(false == style2->has_iconstyle());
  CPPUNIT_ASSERT(false == style2->has_labelstyle());
  CPPUNIT_ASSERT(false == style2->has_linestyle());
  CPPUNIT_ASSERT(false == style2->has_polystyle());
  CPPUNIT_ASSERT(false == style2->has_balloonstyle());
  CPPUNIT_ASSERT(false == style2->has_liststyle());

  // Unit test tearDown deletes style_ and the children created here.
}

void StyleTest::TestSerialize() {
  KmlFactory* factory = KmlFactory::GetFactory();
  style_->set_id("styleid");
  style_->set_iconstyle(factory->CreateIconStyle());
  style_->set_labelstyle(factory->CreateLabelStyle());
  style_->set_linestyle(factory->CreateLineStyle());
  style_->set_polystyle(factory->CreatePolyStyle());
  style_->set_balloonstyle(factory->CreateBalloonStyle());
  style_->set_liststyle(factory->CreateListStyle());

  std::string expected =
    "<Style id=\"styleid\">"
    "<IconStyle/>"
    "<LabelStyle/>"
    "<LineStyle/>"
    "<PolyStyle/>"
    "<BalloonStyle/>"
    "<ListStyle/>"
    "</Style>";
  CPPUNIT_ASSERT_EQUAL(expected, SerializeRaw(style_));
}

}  // end namespace kmldom

TEST_MAIN
