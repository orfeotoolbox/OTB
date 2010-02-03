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

#include "kml/dom/polystyle.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/kmldom.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class PolyStyleTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(PolyStyleTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    polystyle_ = KmlFactory::GetFactory()->CreatePolyStyle();
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
  PolyStylePtr polystyle_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(PolyStyleTest);

void PolyStyleTest::TestType() {
  CPPUNIT_ASSERT(true == polystyle_->IsA(Type_PolyStyle));
  CPPUNIT_ASSERT(Type_PolyStyle == polystyle_->Type());
  CPPUNIT_ASSERT(true == polystyle_->IsA(Type_ColorStyle));
}

// Verify proper defaults:
void PolyStyleTest::TestDefaults() {
  CPPUNIT_ASSERT(false == polystyle_->has_fill());
  CPPUNIT_ASSERT(true == polystyle_->get_fill());
  CPPUNIT_ASSERT(false == polystyle_->has_outline());
  CPPUNIT_ASSERT(true == polystyle_->get_outline());
}

// Verify setting default makes has_xxx() true:
void PolyStyleTest::TestSetToDefaultValues() {
  TestDefaults();
  polystyle_->set_fill(polystyle_->get_fill());
  CPPUNIT_ASSERT(true == polystyle_->has_fill());
  polystyle_->set_outline(polystyle_->get_outline());
  CPPUNIT_ASSERT(true == polystyle_->has_outline());
}

// Verify set, get, has, clear:
void PolyStyleTest::TestSetGetHasClear() {
  // Non-default values:
  bool fill = false;
  bool outline = false;

  // Set all fields:
  polystyle_->set_fill(fill);
  polystyle_->set_outline(outline);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == polystyle_->has_fill());
  CPPUNIT_ASSERT(fill == polystyle_->get_fill());
  CPPUNIT_ASSERT(true == polystyle_->has_outline());
  CPPUNIT_ASSERT(outline == polystyle_->get_outline());

  // Clear all fields:
  polystyle_->clear_fill();
  polystyle_->clear_outline();

  // Verify now in default state:
  TestDefaults();
}

}  // end namespace kmldom

TEST_MAIN
