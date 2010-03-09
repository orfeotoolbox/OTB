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

// This file contains the unit tests for the Color32 class.

#include "kml/base/color32.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"

namespace kmlbase {

class ColorTest : public testing::Test {
 protected:
  boost::scoped_ptr<Color32> color_;
};

// Colors we'll use in multiple tests, given as AABBGGRR.
static const uint32_t kOpaqueWhite = 0xffffffff;
static const uint32_t kOpaqueBlack = 0xff000000;
static const uint32_t kOpaqueBlue = 0xffff0000;
static const uint32_t kOpaqueGreen = 0xff00ff00;
static const uint32_t kOpaqueRed = 0xff0000ff;

TEST_F(ColorTest, TestConstruction) {
  // Verify basic instantiation of a default Color32 object.
  color_.reset(new Color32());
  ASSERT_EQ(kOpaqueWhite, color_->get_color_abgr());

  // Verify construction from an int.
  const int32_t kRedInt = 0xff0000ff;
  color_.reset(new Color32(kRedInt));
  ASSERT_EQ(kOpaqueRed, color_->get_color_abgr());

  // Verify construction from a specific color.
  color_.reset(new Color32(kOpaqueBlack));
  ASSERT_EQ(kOpaqueBlack, color_->get_color_abgr());

  // Verify construction from a bunch of RGBA uchars.
  const unsigned char kA = 0xff;
  const unsigned char kB = 0x00;
  const unsigned char kG = 0x00;
  const unsigned char kR = 0xff;  // The components of kOpaqueRed.
  color_.reset(new Color32(kA, kB, kG, kR));
  ASSERT_EQ(kOpaqueRed, color_->get_color_abgr());

  // Verify construction from another Color32.
  Color32 color(kOpaqueBlue);
  color_.reset(new Color32(color));
  ASSERT_EQ(kOpaqueBlue, color_->get_color_abgr());

  // Verify construction from a string.
  const string kOpaqueRedStr("ff0000ff");
  color_.reset(new Color32(kOpaqueRedStr));
  ASSERT_EQ(kOpaqueRed, color_->get_color_abgr());

  // Verify correct behaviour with poorly formed string data.
  //
  // Any string supplied that is less than 8 chars is filled from the front
  // with zeros (and will thus be completely transparent).

  // An fully empty string initalizes to all zeroes (transparent black).
  const string kEmptyString("");
  color_.reset(new Color32(kEmptyString));
  string expected = "00000000";
  ASSERT_EQ(expected, color_->to_string_abgr());

  const string kOnlySixCharsGiven("ffffff");
  color_.reset(new Color32(kOnlySixCharsGiven));
  expected = "00ffffff";
  ASSERT_EQ(expected, color_->to_string_abgr());

  const string kOnly2CharsGiven("ff");
  color_.reset(new Color32(kOnly2CharsGiven));
  expected = "000000ff";
  ASSERT_EQ(expected, color_->to_string_abgr());

  // Only the first eight chars are used for construction from string. Extra
  // chars at the end of the input string are ignored.
  const string kTenCharsGiven("aabbccddee");
  color_.reset(new Color32(kTenCharsGiven));
  expected = "aabbccdd";
  ASSERT_EQ(expected, color_->to_string_abgr());

  // The input string here has two valid hex values in the first eight chars.
  // ( the "a" and "c" in "Not a c") and those are the only chars that
  // won't be replaced with zeroes.
  const string kBadString("Not a color value");
  color_.reset(new Color32(kBadString));
  expected = "0000a0c0";
  ASSERT_EQ(expected, color_->to_string_abgr());
}

TEST_F(ColorTest, TestGetSet) {
  // Verify getters of default state.
  color_.reset(new Color32());
  const uint32_t kFF = 0xff;
  ASSERT_EQ(kFF, color_->get_alpha());
  ASSERT_EQ(kFF, color_->get_blue());
  ASSERT_EQ(kFF, color_->get_green());
  ASSERT_EQ(kFF, color_->get_red());

  // Verify getters of newly set state.
  const uint32_t kAB = 0xab;
  color_->set_alpha(kAB);
  color_->set_blue(kAB);
  color_->set_green(kAB);
  color_->set_red(kAB);
  ASSERT_EQ(kAB, color_->get_alpha());
  ASSERT_EQ(kAB, color_->get_blue());
  ASSERT_EQ(kAB, color_->get_green());
  ASSERT_EQ(kAB, color_->get_red());

  // Verify get_color_abgr and get_color_rgba.
  color_.reset(new Color32(kOpaqueGreen));
  const uint32_t kOpaqueGreenARGB = 0xff00ff00;
  ASSERT_EQ(kOpaqueGreen, color_->get_color_abgr());
  ASSERT_EQ(kOpaqueGreenARGB, color_->get_color_argb());

  // Verify reconstruction to AABBGGRR string.
  const string kOpaqueRedStr("ff0000ff");
  color_.reset(new Color32(kOpaqueRedStr));
  ASSERT_EQ(kOpaqueRedStr, color_->to_string_abgr());

  // Verify to_argb_string.
  ASSERT_EQ(kOpaqueRedStr, color_->to_string_abgr());
  const string kOpaqueRedStr_argb("ffff0000");
  ASSERT_EQ(kOpaqueRedStr_argb, color_->to_string_argb());
}

TEST_F(ColorTest, TestOperators) {
  // Verify correct behavior of overidden class operators.
  // Assignment from uint32_t.
  color_.reset(new Color32());
  *color_.get() = kOpaqueGreen;  // Same as: Color32 c; c = kOpaqueGreen;
  ASSERT_EQ(kOpaqueGreen, color_->get_color_abgr());

  // Assignment from int.
  color_.reset(new Color32());
  *color_.get() = kOpaqueRed;
  ASSERT_EQ(kOpaqueRed, color_->get_color_abgr());

  // Assignment from reference to object.
  color_.reset(new Color32());
  Color32 color(kOpaqueGreen);
  *color_.get() = color;
  ASSERT_EQ(kOpaqueGreen, color_->get_color_abgr());

  // Inequality operator.
  Color32 black(kOpaqueBlack);
  Color32 green(kOpaqueGreen);
  ASSERT_TRUE(black != green);

  // Equality operator.
  ASSERT_TRUE(black == black);
  ASSERT_TRUE(green == green);

  // Greater-than operator.
  ASSERT_TRUE(green > black);

  // Less-than operator.
  ASSERT_TRUE(black < green);
}

TEST_F(ColorTest, TestSetFromUint32Argb) {
  color_.reset(new Color32());
  color_->set_color_argb(0x7f112233);
  ASSERT_EQ(static_cast<uint32_t>(0x7f), color_->get_alpha());
  ASSERT_EQ(static_cast<uint32_t>(0x11), color_->get_red());
  ASSERT_EQ(static_cast<uint32_t>(0x22), color_->get_green());
  ASSERT_EQ(static_cast<uint32_t>(0x33), color_->get_blue());
}

TEST_F(ColorTest, TestSetColorArgbFromString) {
  // The first 8 characters are a valid color (green). The remainder of the
  // string is garbage. Assert we correctly set the color.
  const string kLongStr("01234567nowSomeNonsense");
  const string kStr("01234567");
  color_.reset(new Color32());
  color_->set_color_abgr(kLongStr);
  ASSERT_EQ(kStr, color_->to_string_abgr());
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
