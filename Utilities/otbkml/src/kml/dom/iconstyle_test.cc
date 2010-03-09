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
#include "gtest/gtest.h"

namespace kmldom {

class IconStyleTest : public testing::Test {
 protected:
  virtual void SetUp() {
    iconstyle_ = KmlFactory::GetFactory()->CreateIconStyle();
  }

  IconStylePtr iconstyle_;
};

TEST_F(IconStyleTest, TestType) {
  ASSERT_TRUE(iconstyle_->IsA(Type_IconStyle));
  ASSERT_EQ(Type_IconStyle, iconstyle_->Type());
  ASSERT_TRUE(iconstyle_->IsA(Type_ColorStyle));
}

// Verify proper defaults:
TEST_F(IconStyleTest, TestDefaults) {
  ASSERT_FALSE(iconstyle_->has_scale());
  ASSERT_DOUBLE_EQ(1.0, iconstyle_->get_scale());
  ASSERT_FALSE(iconstyle_->has_heading());
  ASSERT_DOUBLE_EQ(0.0, iconstyle_->get_heading());
  ASSERT_FALSE(iconstyle_->has_icon());
  ASSERT_TRUE(NULL == iconstyle_->get_icon());
  ASSERT_FALSE(iconstyle_->has_hotspot());
  ASSERT_TRUE(NULL == iconstyle_->get_hotspot());
}

// Verify setting default makes has_xxx() true:
TEST_F(IconStyleTest, TestSetToDefaultValues) {
  iconstyle_->set_scale(iconstyle_->get_scale());
  ASSERT_TRUE(iconstyle_->has_scale());
  iconstyle_->set_heading(iconstyle_->get_heading());
  ASSERT_TRUE(iconstyle_->has_heading());
  iconstyle_->set_icon(NULL);
  ASSERT_FALSE(iconstyle_->get_icon());
  iconstyle_->set_hotspot(NULL);
  // Note: setting to default sets pointer to NULL, so has_hotspot is false.
  // This is different to field behaviour where has_xxx is to equivalent to
  // "field has been set outside of ctor" and is true.
  ASSERT_FALSE(iconstyle_->has_hotspot());
}

// Verify set, get, has, clear:
TEST_F(IconStyleTest, TestSetGetHasClear) {
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
  ASSERT_TRUE(iconstyle_->has_scale());
  ASSERT_DOUBLE_EQ(scale, iconstyle_->get_scale());
  ASSERT_TRUE(iconstyle_->has_heading());
  ASSERT_DOUBLE_EQ(heading, iconstyle_->get_heading());
  ASSERT_TRUE(iconstyle_->has_icon());
  ASSERT_EQ(icon, iconstyle_->get_icon());
  ASSERT_TRUE(iconstyle_->has_hotspot());
  ASSERT_EQ(hotspot, iconstyle_->get_hotspot());

  // Clear all fields:
  iconstyle_->clear_scale();
  iconstyle_->clear_heading();
  iconstyle_->clear_icon();
  iconstyle_->clear_hotspot();
}

// Verify parse
TEST_F(IconStyleTest, TestParse) {
  ASSERT_FALSE(iconstyle_->has_icon());
  const char* kIconStyleIcon =
    "<IconStyle>"
    "<Icon><href>image.jpg</href></Icon>"
    "</IconStyle>";
  string errors;
  ElementPtr root = Parse(kIconStyleIcon, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const IconStylePtr iconstyle = AsIconStyle(root);
  ASSERT_TRUE(iconstyle->has_icon());
  // Verify that this is IconStyle's brand of Icon (not Type_Icon).
  ASSERT_EQ(Type_IconStyleIcon, iconstyle->get_icon()->Type());
  ASSERT_EQ(string("image.jpg"), iconstyle->get_icon()->get_href());
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
