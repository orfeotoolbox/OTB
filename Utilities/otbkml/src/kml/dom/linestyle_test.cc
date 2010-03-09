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

#include "kml/dom/linestyle.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/kmldom.h"
#include "gtest/gtest.h"

namespace kmldom {

class LineStyleTest : public testing::Test {
 protected:
  virtual void SetUp() {
    linestyle_ = KmlFactory::GetFactory()->CreateLineStyle();
  }

  LineStylePtr linestyle_;
};

TEST_F(LineStyleTest, TestType) {
  ASSERT_TRUE(linestyle_->IsA(Type_LineStyle));
  ASSERT_EQ(Type_LineStyle, linestyle_->Type());
  ASSERT_TRUE(linestyle_->IsA(Type_ColorStyle));
}

// Verify proper defaults:
TEST_F(LineStyleTest, TestDefaults) {
  ASSERT_FALSE(linestyle_->has_width());
  ASSERT_DOUBLE_EQ(1.0, linestyle_->get_width());
}

// Verify setting default makes has_xxx() true:
TEST_F(LineStyleTest, TestSetToDefaultValues) {
  linestyle_->set_width(linestyle_->get_width());
  ASSERT_TRUE(linestyle_->has_width());
}

// Verify set, get, has, clear:
TEST_F(LineStyleTest, TestSetGetHasClear) {
  // Non-default values:
  double width = 0.0;

  // Set all fields:
  linestyle_->set_width(width);

  // Verify getter and has_xxx():
  ASSERT_TRUE(linestyle_->has_width());
  ASSERT_DOUBLE_EQ(width, linestyle_->get_width());

  // Clear all fields:
  linestyle_->clear_width();
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
