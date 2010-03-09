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
#include "gtest/gtest.h"

namespace kmldom {

class StyleTest : public testing::Test {
 protected:
  virtual void SetUp() {
    style_ = KmlFactory::GetFactory()->CreateStyle();
  }

  StylePtr style_;
};

TEST_F(StyleTest, TestType) {
  ASSERT_EQ(Type_Style, style_->Type());
  ASSERT_TRUE(style_->IsA(Type_Style));
  ASSERT_TRUE(style_->IsA(Type_StyleSelector));
  ASSERT_TRUE(style_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(StyleTest, TestDefaults) {
  ASSERT_FALSE(style_->has_iconstyle());
  ASSERT_TRUE(NULL == style_->get_iconstyle());
  ASSERT_FALSE(style_->has_labelstyle());
  ASSERT_TRUE(NULL == style_->get_labelstyle());
  ASSERT_FALSE(style_->has_linestyle());
  ASSERT_TRUE(NULL == style_->get_linestyle());
  ASSERT_FALSE(style_->has_polystyle());
  ASSERT_TRUE(NULL == style_->get_polystyle());
  ASSERT_FALSE(style_->has_balloonstyle());
  ASSERT_TRUE(NULL == style_->get_balloonstyle());
  ASSERT_FALSE(style_->has_liststyle());
  ASSERT_TRUE(NULL == style_->get_liststyle());
}

// Verify setting default makes has_xxx() true:
TEST_F(StyleTest, TestSetToDefaultValues) {
  style_->set_iconstyle(NULL);  // should not crash
  ASSERT_FALSE(style_->has_iconstyle());  // ptr is null
  style_->set_labelstyle(NULL);
  ASSERT_FALSE(style_->has_labelstyle());
  style_->set_linestyle(NULL);
  ASSERT_FALSE(style_->has_linestyle());
  style_->set_polystyle(NULL);
  ASSERT_FALSE(style_->has_polystyle());
  style_->set_balloonstyle(NULL);
  ASSERT_FALSE(style_->has_balloonstyle());
  style_->set_liststyle(NULL);
  ASSERT_FALSE(style_->has_liststyle());
}

// Verify set, get, has, clear:
TEST_F(StyleTest, TestSetGetHasClear) {
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
  ASSERT_TRUE(style_->has_iconstyle());
  ASSERT_EQ(iconstyle, style_->get_iconstyle());
  ASSERT_TRUE(style_->has_labelstyle());
  ASSERT_EQ(labelstyle, style_->get_labelstyle());
  ASSERT_TRUE(style_->has_linestyle());
  ASSERT_EQ(linestyle, style_->get_linestyle());
  ASSERT_TRUE(style_->has_polystyle());
  ASSERT_EQ(polystyle, style_->get_polystyle());
  ASSERT_TRUE(style_->has_balloonstyle());
  ASSERT_EQ(balloonstyle, style_->get_balloonstyle());
  ASSERT_TRUE(style_->has_liststyle());
  ASSERT_EQ(liststyle, style_->get_liststyle());

  // Clear all fields:
  style_->clear_iconstyle();
  style_->clear_labelstyle();
  style_->clear_polystyle();
  style_->clear_linestyle();
  style_->clear_balloonstyle();
  style_->clear_liststyle();
}

// Verify that 2 Styles can't take the same IconStyle, LabelStyle, LineStyle,
// PolyStyle, BalloonStyle or ListStyle.
// (This tests the internal set_parent() method.)
TEST_F(StyleTest, TestSetParent) {
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
  ASSERT_TRUE(style_->has_iconstyle());
  ASSERT_TRUE(style_->has_labelstyle());
  ASSERT_TRUE(style_->has_linestyle());
  ASSERT_TRUE(style_->has_polystyle());
  ASSERT_TRUE(style_->has_balloonstyle());
  ASSERT_TRUE(style_->has_liststyle());

  // Verify that style2 has no children.
  ASSERT_FALSE(style2->has_iconstyle());
  ASSERT_FALSE(style2->has_labelstyle());
  ASSERT_FALSE(style2->has_linestyle());
  ASSERT_FALSE(style2->has_polystyle());
  ASSERT_FALSE(style2->has_balloonstyle());
  ASSERT_FALSE(style2->has_liststyle());

  // Unit test tearDown deletes style_ and the children created here.
}

TEST_F(StyleTest, TestSerialize) {
  KmlFactory* factory = KmlFactory::GetFactory();
  style_->set_id("styleid");
  style_->set_iconstyle(factory->CreateIconStyle());
  style_->set_labelstyle(factory->CreateLabelStyle());
  style_->set_linestyle(factory->CreateLineStyle());
  style_->set_polystyle(factory->CreatePolyStyle());
  style_->set_balloonstyle(factory->CreateBalloonStyle());
  style_->set_liststyle(factory->CreateListStyle());

  string expected =
    "<Style id=\"styleid\">"
    "<IconStyle/>"
    "<LabelStyle/>"
    "<LineStyle/>"
    "<PolyStyle/>"
    "<BalloonStyle/>"
    "<ListStyle/>"
    "</Style>";
  ASSERT_EQ(expected, SerializeRaw(style_));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
