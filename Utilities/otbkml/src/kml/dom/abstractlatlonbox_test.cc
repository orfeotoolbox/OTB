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

#include "kml/base/unit_test.h"
#include "kml/dom/abstractlatlonbox.h"

namespace kmldom {

class AbstractLatLonBoxTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(AbstractLatLonBoxTest);
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
  // AbstractLatLonBox is abstract, hence its constructor is protected.
  class TestLatLonBox : public AbstractLatLonBox {
  };
  TestLatLonBox latlonbox;
};

CPPUNIT_TEST_SUITE_REGISTRATION(AbstractLatLonBoxTest);

void AbstractLatLonBoxTest::TestType() {
  CPPUNIT_ASSERT(true == latlonbox.IsA(Type_AbstractLatLonBox));
  CPPUNIT_ASSERT(true == latlonbox.IsA(Type_Object));
}

// Verify proper defaults:
void AbstractLatLonBoxTest::TestDefaults() {
  CPPUNIT_ASSERT(false == latlonbox.has_north());
  CPPUNIT_ASSERT(0.0 == latlonbox.get_north());
  CPPUNIT_ASSERT(false == latlonbox.has_south());
  CPPUNIT_ASSERT(0.0 == latlonbox.get_south());
  CPPUNIT_ASSERT(false == latlonbox.has_east());
  CPPUNIT_ASSERT(0.0 == latlonbox.get_east());
  CPPUNIT_ASSERT(false == latlonbox.has_west());
  CPPUNIT_ASSERT(0.0 == latlonbox.get_west());
}

// Verify setting default makes has_xxx() true:
void AbstractLatLonBoxTest::TestSetToDefaultValues() {
  // Verify the latlonbox is in default state:
  CPPUNIT_ASSERT(false == latlonbox.has_north());
  CPPUNIT_ASSERT(false == latlonbox.has_south());
  CPPUNIT_ASSERT(false == latlonbox.has_east());
  CPPUNIT_ASSERT(false == latlonbox.has_west());
  latlonbox.set_north(latlonbox.get_north());
  latlonbox.set_south(latlonbox.get_south());
  latlonbox.set_east(latlonbox.get_east());
  latlonbox.set_west(latlonbox.get_west());
  CPPUNIT_ASSERT(true == latlonbox.has_north());
  CPPUNIT_ASSERT(true == latlonbox.has_south());
  CPPUNIT_ASSERT(true == latlonbox.has_east());
  CPPUNIT_ASSERT(true == latlonbox.has_west());
}

// Verify set, get, has, clear:
void AbstractLatLonBoxTest::TestSetGetHasClear() {
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
  CPPUNIT_ASSERT(true == latlonbox.has_north());
  CPPUNIT_ASSERT(north == latlonbox.get_north());
  CPPUNIT_ASSERT(true == latlonbox.has_south());
  CPPUNIT_ASSERT(south == latlonbox.get_south());
  CPPUNIT_ASSERT(true == latlonbox.has_east());
  CPPUNIT_ASSERT(east == latlonbox.get_east());
  CPPUNIT_ASSERT(true == latlonbox.has_west());
  CPPUNIT_ASSERT(west == latlonbox.get_west());

  // Clear all fields:
  latlonbox.clear_north();
  latlonbox.clear_south();
  latlonbox.clear_east();
  latlonbox.clear_west();

  // Verify now in default state:
  TestDefaults();
}

}  // end namespace kmldom

TEST_MAIN
