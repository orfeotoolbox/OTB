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
#include "gtest/gtest.h"

namespace kmldom {

class HotSpotTest : public testing::Test {
 protected:
  virtual void SetUp() {
    hotspot_ = KmlFactory::GetFactory()->CreateHotSpot();
  }

  HotSpotPtr hotspot_;
};

TEST_F(HotSpotTest, TestType) {
  ASSERT_EQ(Type_hotSpot, hotspot_->Type());
  ASSERT_TRUE(hotspot_->IsA(Type_hotSpot));
  ASSERT_TRUE(hotspot_->IsA(Type_Vec2));
}

TEST_F(HotSpotTest, TestParse) {
  string errors;
  ElementPtr root = Parse(
    "<hotSpot x=\"32\" y=\"1\" xunits=\"pixels\" yunits=\"pixels\"/>",
    &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
#if 0
  const HotSpot* hotspot = AsHotSpot(root);
  ASSERT_TRUE(hotspot);
  ASSERT_EQ(32., hotspot->x());
  ASSERT_EQ(1., hotspot->y());
  ASSERT_EQ(static_cast<int>(UNITS_PIXELS), hotspot->xunits());
  ASSERT_EQ(static_cast<int>(UNITS_PIXELS), hotspot->yunits());
#endif
}

TEST_F(HotSpotTest, TestSerialize) {
  hotspot_->set_x(32);
  hotspot_->set_y(1);
  hotspot_->set_xunits(UNITS_PIXELS);
  hotspot_->set_yunits(UNITS_PIXELS);
  string expected =
    "<hotSpot x=\"32\" xunits=\"pixels\" y=\"1\" yunits=\"pixels\"/>";
  ASSERT_EQ(expected, SerializeRaw(hotspot_));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
