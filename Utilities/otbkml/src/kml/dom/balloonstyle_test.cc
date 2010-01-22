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
#include "kml/base/unit_test.h"

namespace kmldom {

class BalloonStyleTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(BalloonStyleTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    balloonstyle_ = KmlFactory::GetFactory()->CreateBalloonStyle();
  }

  // Called after all tests.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  BalloonStylePtr balloonstyle_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(BalloonStyleTest);

void BalloonStyleTest::TestType() {
  CPPUNIT_ASSERT(true == balloonstyle_->IsA(Type_BalloonStyle));
  CPPUNIT_ASSERT(Type_BalloonStyle == balloonstyle_->Type());
  CPPUNIT_ASSERT(true == balloonstyle_->IsA(Type_SubStyle));
}

// Verify proper defaults:
void BalloonStyleTest::TestDefaults() {
  CPPUNIT_ASSERT(false == balloonstyle_->has_bgcolor());
  CPPUNIT_ASSERT("ffffffff" == balloonstyle_->get_bgcolor());
  CPPUNIT_ASSERT(false == balloonstyle_->has_textcolor());
  CPPUNIT_ASSERT("ff000000" == balloonstyle_->get_textcolor());
  CPPUNIT_ASSERT(false == balloonstyle_->has_text());
  CPPUNIT_ASSERT("" == balloonstyle_->get_text());
  CPPUNIT_ASSERT(false == balloonstyle_->has_displaymode());
  CPPUNIT_ASSERT(DISPLAYMODE_DEFAULT == balloonstyle_->get_displaymode());
}

// Verify setting default makes has_xxx() true:
void BalloonStyleTest::TestSetToDefaultValues() {
  TestDefaults();
  balloonstyle_->set_bgcolor(balloonstyle_->get_bgcolor());
  CPPUNIT_ASSERT(true == balloonstyle_->has_bgcolor());
  balloonstyle_->set_textcolor(balloonstyle_->get_textcolor());
  CPPUNIT_ASSERT(true == balloonstyle_->has_textcolor());
  balloonstyle_->set_text(balloonstyle_->get_text());
  CPPUNIT_ASSERT(true == balloonstyle_->has_text());
  balloonstyle_->set_displaymode(balloonstyle_->get_displaymode());
  CPPUNIT_ASSERT(true == balloonstyle_->has_displaymode());
}

// Verify set, get, has, clear:
void BalloonStyleTest::TestSetGetHasClear() {
  // Non-default values:
  std::string bgcolor("00112233");
  std::string textcolor("33221100");
  std::string text("amidoinitrite?");
  DisplayModeEnum displaymode = DISPLAYMODE_HIDE;

  // Set all fields:
  balloonstyle_->set_bgcolor(bgcolor);
  balloonstyle_->set_textcolor(textcolor);
  balloonstyle_->set_text(text);
  balloonstyle_->set_displaymode(displaymode);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == balloonstyle_->has_bgcolor());
  CPPUNIT_ASSERT(bgcolor == balloonstyle_->get_bgcolor());
  CPPUNIT_ASSERT(true == balloonstyle_->has_textcolor());
  CPPUNIT_ASSERT(textcolor == balloonstyle_->get_textcolor());
  CPPUNIT_ASSERT(true == balloonstyle_->has_text());
  CPPUNIT_ASSERT(text == balloonstyle_->get_text());
  CPPUNIT_ASSERT(true == balloonstyle_->has_displaymode());
  CPPUNIT_ASSERT(displaymode == balloonstyle_->get_displaymode());

  // Clear all fields:
  balloonstyle_->clear_bgcolor();
  balloonstyle_->clear_textcolor();
  balloonstyle_->clear_text();
  balloonstyle_->clear_displaymode();

  // Verify now in default state:
  TestDefaults();
}

}  // end namespace kmldom

TEST_MAIN
