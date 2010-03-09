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

#include "kml/dom/abstractlatlonbox.h"
#include "gtest/gtest.h"

namespace kmldom {

class AbstractLatLonBoxTest : public testing::Test {
 protected:
  void CheckDefaults() {
  ASSERT_FALSE(latlonbox.has_north());
  ASSERT_DOUBLE_EQ(180.0, latlonbox.get_north());
  ASSERT_FALSE(latlonbox.has_south());
  ASSERT_DOUBLE_EQ(-180.0, latlonbox.get_south());
  ASSERT_FALSE(latlonbox.has_east());
  ASSERT_DOUBLE_EQ(180.0, latlonbox.get_east());
  ASSERT_FALSE(latlonbox.has_west());
  ASSERT_DOUBLE_EQ(-180.0, latlonbox.get_west());
  }

  // AbstractLatLonBox is abstract, hence its constructor is protected.
  class TestLatLonBox : public AbstractLatLonBox {
  };
  TestLatLonBox latlonbox;
};

TEST_F(AbstractLatLonBoxTest, TestType) {
  ASSERT_TRUE(latlonbox.IsA(Type_AbstractLatLonBox));
  ASSERT_TRUE(latlonbox.IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(AbstractLatLonBoxTest, TestDefaults) {
  CheckDefaults();
#if 0
  ASSERT_FALSE(latlonbox.has_north());
  ASSERT_DOUBLE_EQ(180.0, latlonbox.get_north());
  ASSERT_FALSE(latlonbox.has_south());
  ASSERT_DOUBLE_EQ(-180.0, latlonbox.get_south());
  ASSERT_FALSE(latlonbox.has_east());
  ASSERT_DOUBLE_EQ(180.0, latlonbox.get_east());
  ASSERT_FALSE(latlonbox.has_west());
  ASSERT_DOUBLE_EQ(-180.0, latlonbox.get_west());
#endif
}

// Verify setting default makes has_xxx() true:
TEST_F(AbstractLatLonBoxTest, TestSetToDefaultValues) {
  // Verify the latlonbox is in default state:
  ASSERT_FALSE(latlonbox.has_north());
  ASSERT_FALSE(latlonbox.has_south());
  ASSERT_FALSE(latlonbox.has_east());
  ASSERT_FALSE(latlonbox.has_west());
  latlonbox.set_north(latlonbox.get_north());
  latlonbox.set_south(latlonbox.get_south());
  latlonbox.set_east(latlonbox.get_east());
  latlonbox.set_west(latlonbox.get_west());
  ASSERT_TRUE(latlonbox.has_north());
  ASSERT_TRUE(latlonbox.has_south());
  ASSERT_TRUE(latlonbox.has_east());
  ASSERT_TRUE(latlonbox.has_west());
}

// Verify set, get, has, clear:
TEST_F(AbstractLatLonBoxTest, TestSetGetHasClear) {
  const double north = 38.123;
  const double south = -42.789123;
  const double east = 123.131313131;
  const double west = -2.2345987;

  // Set all fields:
  latlonbox.set_north(north);
  latlonbox.set_south(south);
  latlonbox.set_east(east);
  latlonbox.set_west(west);

  // Verify getter and has_xxx():
  ASSERT_TRUE(latlonbox.has_north());
  ASSERT_DOUBLE_EQ(north, latlonbox.get_north());
  ASSERT_TRUE(latlonbox.has_south());
  ASSERT_DOUBLE_EQ(south, latlonbox.get_south());
  ASSERT_TRUE(latlonbox.has_east());
  ASSERT_DOUBLE_EQ(east, latlonbox.get_east());
  ASSERT_TRUE(latlonbox.has_west());
  ASSERT_DOUBLE_EQ(west, latlonbox.get_west());

  // Clear all fields:
  latlonbox.clear_north();
  latlonbox.clear_south();
  latlonbox.clear_east();
  latlonbox.clear_west();
  CheckDefaults();
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
