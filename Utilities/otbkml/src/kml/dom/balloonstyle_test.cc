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

#include "kml/dom/balloonstyle.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "gtest/gtest.h"

using kmlbase::Color32;

namespace kmldom {

class BalloonStyleTest : public testing::Test {
 protected:
  virtual void SetUp() {
    balloonstyle_ = KmlFactory::GetFactory()->CreateBalloonStyle();
  }

  BalloonStylePtr balloonstyle_;
};

TEST_F(BalloonStyleTest, TestType) {
  ASSERT_TRUE(balloonstyle_->IsA(Type_BalloonStyle));
  ASSERT_TRUE(Type_BalloonStyle == balloonstyle_->Type());
  ASSERT_TRUE(balloonstyle_->IsA(Type_SubStyle));
}

// Verify proper defaults:
TEST_F(BalloonStyleTest, TestDefaults) {
  ASSERT_FALSE(balloonstyle_->has_bgcolor());
  ASSERT_TRUE(Color32(0xffffffff) == balloonstyle_->get_bgcolor());
  ASSERT_FALSE(balloonstyle_->has_textcolor());
  ASSERT_TRUE(Color32(0xff000000) == balloonstyle_->get_textcolor());
  ASSERT_FALSE(balloonstyle_->has_text());
  ASSERT_EQ(string(""), balloonstyle_->get_text());
  ASSERT_FALSE(balloonstyle_->has_displaymode());
  ASSERT_TRUE(DISPLAYMODE_DEFAULT == balloonstyle_->get_displaymode());
}

// Verify setting default makes has_xxx() true:
TEST_F(BalloonStyleTest, TestSetToDefaultValues) {
  balloonstyle_->set_bgcolor(balloonstyle_->get_bgcolor());
  ASSERT_TRUE(balloonstyle_->has_bgcolor());
  balloonstyle_->set_textcolor(balloonstyle_->get_textcolor());
  ASSERT_TRUE(balloonstyle_->has_textcolor());
  balloonstyle_->set_text(balloonstyle_->get_text());
  ASSERT_TRUE(balloonstyle_->has_text());
  balloonstyle_->set_displaymode(balloonstyle_->get_displaymode());
  ASSERT_TRUE(balloonstyle_->has_displaymode());
}

// Verify set, get, has, clear:
TEST_F(BalloonStyleTest, TestSetGetHasClear) {
  // Non-default values:
  Color32 bgcolor = Color32(0x00112233);
  Color32 textcolor = Color32(0x33221100);
  string text("amidoinitrite?");
  DisplayModeEnum displaymode = DISPLAYMODE_HIDE;

  // Set all fields:
  balloonstyle_->set_bgcolor(bgcolor);
  balloonstyle_->set_textcolor(textcolor);
  balloonstyle_->set_text(text);
  balloonstyle_->set_displaymode(displaymode);

  // Verify getter and has_xxx():
  ASSERT_TRUE(balloonstyle_->has_bgcolor());
  ASSERT_TRUE(bgcolor == balloonstyle_->get_bgcolor());
  ASSERT_TRUE(balloonstyle_->has_textcolor());
  ASSERT_TRUE(textcolor == balloonstyle_->get_textcolor());
  ASSERT_TRUE(balloonstyle_->has_text());
  ASSERT_TRUE(text == balloonstyle_->get_text());
  ASSERT_TRUE(balloonstyle_->has_displaymode());
  ASSERT_TRUE(displaymode == balloonstyle_->get_displaymode());

  // Clear all fields:
  balloonstyle_->clear_bgcolor();
  balloonstyle_->clear_textcolor();
  balloonstyle_->clear_text();
  balloonstyle_->clear_displaymode();

}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
