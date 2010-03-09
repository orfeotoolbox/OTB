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

#include "kml/dom/vec2.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"

namespace kmldom {

class Vec2Test : public testing::Test {
 protected:
  virtual void SetUp() {
    vec2_.reset(new TestVec2());
  }

  // Vec2 is abstract, hence its constructor is protected.
  class TestVec2 : public Vec2 {};
  boost::scoped_ptr<TestVec2> vec2_;
};

TEST_F(Vec2Test, TestType) {
  ASSERT_TRUE(vec2_->IsA(Type_Vec2));
}

// Verify proper defaults:
TEST_F(Vec2Test, TestDefaults) {
  ASSERT_DOUBLE_EQ(1.0, vec2_->get_x());
  ASSERT_DOUBLE_EQ(1.0, vec2_->get_y());
  ASSERT_EQ(UNITS_FRACTION, vec2_->get_xunits());
  ASSERT_EQ(UNITS_FRACTION, vec2_->get_yunits());
}

// Verify setting default makes has_xxx() true:
TEST_F(Vec2Test, TestSetToDefaultValues) {
  vec2_->set_x(vec2_->get_x());
  ASSERT_TRUE(vec2_->has_x());
  vec2_->set_y(vec2_->get_y());
  ASSERT_TRUE(vec2_->has_y());
  vec2_->set_xunits(vec2_->get_xunits());
  ASSERT_TRUE(vec2_->has_xunits());
  vec2_->set_yunits(vec2_->get_yunits());
  ASSERT_TRUE(vec2_->has_yunits());
}

// Verify set, get, has, clear:
TEST_F(Vec2Test, TestSetGetHasClear) {
  // Non-default values:
  double x = 0.1;
  double y = 0.2;
  UnitsEnum xunits = UNITS_PIXELS;
  UnitsEnum yunits = UNITS_PIXELS;

  // Set all fields:
  vec2_->set_x(x);
  vec2_->set_y(y);
  vec2_->set_xunits(xunits);
  vec2_->set_yunits(yunits);

  // Verify getter and has_xxx():
  ASSERT_TRUE(vec2_->has_x());
  ASSERT_DOUBLE_EQ(x, vec2_->get_x());
  ASSERT_TRUE(vec2_->has_y());
  ASSERT_DOUBLE_EQ(y, vec2_->get_y());
  ASSERT_TRUE(vec2_->has_xunits());
  ASSERT_EQ(xunits, vec2_->get_xunits());
  ASSERT_TRUE(vec2_->has_yunits());
  ASSERT_EQ(yunits, vec2_->get_yunits());

  // Clear all fields:
  vec2_->clear_x();
  vec2_->clear_y();
  vec2_->clear_xunits();
  vec2_->clear_yunits();
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

