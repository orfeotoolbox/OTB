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

// This file contains the unit test for the hotSpot element.

#include "kml/dom/hotspot.h"
#include "kml/dom/element.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class HotSpotTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(HotSpotTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestParse);
  CPPUNIT_TEST(TestSerialize);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    hotspot_ = KmlFactory::GetFactory()->CreateHotSpot();
  }

  void tearDown() {
  }

 protected:
  void TestType();
  void TestParse();
  void TestSerialize();

 private:
  HotSpotPtr hotspot_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(HotSpotTest);

void HotSpotTest::TestType() {
  CPPUNIT_ASSERT_EQUAL(Type_hotSpot, hotspot_->Type());
  CPPUNIT_ASSERT(true == hotspot_->IsA(Type_hotSpot));
  CPPUNIT_ASSERT(true == hotspot_->IsA(Type_Vec2));
}

void HotSpotTest::TestParse() {
  std::string errors;
  ElementPtr root = Parse(
    "<hotSpot x=\"32\" y=\"1\" xunits=\"pixels\" yunits=\"pixels\"/>",
    &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
#if 0
  const HotSpot* hotspot = AsHotSpot(root);
  CPPUNIT_ASSERT(hotspot);
  CPPUNIT_ASSERT_EQUAL(32., hotspot->x());
  CPPUNIT_ASSERT_EQUAL(1., hotspot->y());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(UNITS_PIXELS), hotspot->xunits());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(UNITS_PIXELS), hotspot->yunits());
#endif
}

void HotSpotTest::TestSerialize() {
  hotspot_->set_x(32);
  hotspot_->set_y(1);
  hotspot_->set_xunits(UNITS_PIXELS);
  hotspot_->set_yunits(UNITS_PIXELS);
  std::string expected =
    "<hotSpot x=\"32\" xunits=\"pixels\" y=\"1\" yunits=\"pixels\"/>";
  CPPUNIT_ASSERT_EQUAL(expected, SerializeRaw(hotspot_));
}

}  // end namespace kmldom

TEST_MAIN
