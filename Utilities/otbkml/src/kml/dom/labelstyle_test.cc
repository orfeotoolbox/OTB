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

#include "kml/dom/labelstyle.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/kmldom.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class LabelStyleTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(LabelStyleTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    labelstyle_ = KmlFactory::GetFactory()->CreateLabelStyle();
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
  LabelStylePtr labelstyle_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(LabelStyleTest);

void LabelStyleTest::TestType() {
  CPPUNIT_ASSERT(true == labelstyle_->IsA(Type_LabelStyle));
  CPPUNIT_ASSERT(Type_LabelStyle == labelstyle_->Type());
  CPPUNIT_ASSERT(true == labelstyle_->IsA(Type_ColorStyle));
}

// Verify proper defaults:
void LabelStyleTest::TestDefaults() {
  CPPUNIT_ASSERT(false == labelstyle_->has_scale());
  CPPUNIT_ASSERT(1.0 == labelstyle_->get_scale());
}

// Verify setting default makes has_xxx() true:
void LabelStyleTest::TestSetToDefaultValues() {
  TestDefaults();
  labelstyle_->set_scale(labelstyle_->get_scale());
  CPPUNIT_ASSERT(true == labelstyle_->has_scale());
}

// Verify set, get, has, clear:
void LabelStyleTest::TestSetGetHasClear() {
  // Non-default values:
  double scale = 0.0;

  // Set all fields:
  labelstyle_->set_scale(scale);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == labelstyle_->has_scale());
  CPPUNIT_ASSERT(scale == labelstyle_->get_scale());

  // Clear all fields:
  labelstyle_->clear_scale();

  // Verify now in default state:
  TestDefaults();
}

}  // end namespace kmldom

TEST_MAIN
