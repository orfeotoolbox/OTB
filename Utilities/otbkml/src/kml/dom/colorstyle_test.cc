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

#include "kml/dom/colorstyle.h"
#include "gtest/gtest.h"

namespace kmldom {

class ColorStyleTest : public testing::Test {
 protected:
  // ColorStyle is abstract, hence its constructor is protected.
  class TestColorStyle : public ColorStyle {
  };
  TestColorStyle colorstyle_;
};

TEST_F(ColorStyleTest, TestType) {
  ASSERT_TRUE(colorstyle_.IsA(Type_ColorStyle));
  ASSERT_TRUE(colorstyle_.IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(ColorStyleTest, TestDefaults) {
  ASSERT_TRUE("ffffffff" == colorstyle_.get_color().to_string_abgr());
  ASSERT_FALSE(colorstyle_.has_color());
  ASSERT_TRUE(COLORMODE_NORMAL == colorstyle_.get_colormode());
  ASSERT_FALSE(colorstyle_.has_colormode());
}

// Verify setting default makes has_xxx() true:
TEST_F(ColorStyleTest, TestSetToDefaultValues) {
  colorstyle_.set_color(colorstyle_.get_color());
  ASSERT_TRUE(colorstyle_.has_color());
  colorstyle_.set_colormode(colorstyle_.get_colormode());
  ASSERT_TRUE(colorstyle_.has_colormode());
}

// Verify set, get, has, clear:
TEST_F(ColorStyleTest, TestSetGetHasClear) {
  // Non-default values:
  string color("00112233");
  ColorModeEnum colormode = COLORMODE_RANDOM;

  // Set all fields:
  colorstyle_.set_color(color);
  colorstyle_.set_colormode(colormode);

  // Verify getter and has_xxx():
  ASSERT_TRUE(colorstyle_.has_color());
  ASSERT_EQ(color, colorstyle_.get_color().to_string_abgr());
  ASSERT_TRUE(colorstyle_.has_colormode());
  ASSERT_TRUE(colormode == colorstyle_.get_colormode());

  // Clear all fields:
  colorstyle_.clear_color();
  colorstyle_.clear_colormode();

}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
