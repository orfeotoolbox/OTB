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
#include "kml/base/unit_test.h"

namespace kmldom {

class ColorStyleTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ColorStyleTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  // ColorStyle is abstract, hence its constructor is protected.
  class TestColorStyle : public ColorStyle {
  };
  TestColorStyle colorstyle_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ColorStyleTest);

void ColorStyleTest::TestType() {
  CPPUNIT_ASSERT(true == colorstyle_.IsA(Type_ColorStyle));
  CPPUNIT_ASSERT(true == colorstyle_.IsA(Type_Object));
}

// Verify proper defaults:
void ColorStyleTest::TestDefaults() {
  CPPUNIT_ASSERT("ffffffff" == colorstyle_.get_color());
  CPPUNIT_ASSERT(false == colorstyle_.has_color());
  CPPUNIT_ASSERT(COLORMODE_NORMAL == colorstyle_.get_colormode());
  CPPUNIT_ASSERT(false == colorstyle_.has_colormode());
}

// Verify setting default makes has_xxx() true:
void ColorStyleTest::TestSetToDefaultValues() {
  TestDefaults();
  colorstyle_.set_color(colorstyle_.get_color());
  CPPUNIT_ASSERT(true == colorstyle_.has_color());
  colorstyle_.set_colormode(colorstyle_.get_colormode());
  CPPUNIT_ASSERT(true == colorstyle_.has_colormode());
}

// Verify set, get, has, clear:
void ColorStyleTest::TestSetGetHasClear() {
  // Non-default values:
  std::string color("00112233");
  ColorModeEnum colormode = COLORMODE_RANDOM;

  // Set all fields:
  colorstyle_.set_color(color);
  colorstyle_.set_colormode(colormode);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == colorstyle_.has_color());
  CPPUNIT_ASSERT(color == colorstyle_.get_color());
  CPPUNIT_ASSERT(true == colorstyle_.has_colormode());
  CPPUNIT_ASSERT(colormode == colorstyle_.get_colormode());

  // Clear all fields:
  colorstyle_.clear_color();
  colorstyle_.clear_colormode();

  // Verify now in default state:
  TestDefaults();
}

}  // end namespace kmldom

TEST_MAIN
