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

// This file contains the unit tests for the location utility functions.

#include "kml/engine/location_util.h"
#include "kml/dom.h"
#include "kml/base/unit_test.h"

using kmldom::KmlFactory;
using kmldom::LatLonBoxPtr;
using kmldom::LatLonAltBoxPtr;

namespace kmlengine {

class LocationUtilTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(LocationUtilTest);
  CPPUNIT_TEST(TestGetCenter);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestGetCenter();
};

// This tests the GetCenter() function.
void LocationUtilTest::TestGetCenter() {
  KmlFactory* factory = KmlFactory::GetFactory();
  // NULL output pointer(s) should not crash.
  LatLonBoxPtr llb = factory->CreateLatLonBox();
  GetCenter(llb, NULL, NULL);
  double lat, lon;
  GetCenter(llb, &lat, NULL);
  // Missing lon pointer still saves a result for lat.
  CPPUNIT_ASSERT_EQUAL(0.0, lat);
  GetCenter(llb, NULL, &lon);
  // Missing lat pointer still saves a result for lon.
  CPPUNIT_ASSERT_EQUAL(0.0, lat);
  // A default LatLonBox is well defined thus so is its center.
  GetCenter(llb, &lat, &lon);
  CPPUNIT_ASSERT_EQUAL(0.0, lat);
  CPPUNIT_ASSERT_EQUAL(0.0, lon);
  // A default LatLonAltBox is well defined thus so is its center.
  LatLonAltBoxPtr llab = factory->CreateLatLonAltBox();
  GetCenter(llab, &lat, &lon);
  CPPUNIT_ASSERT_EQUAL(0.0, lat);
  CPPUNIT_ASSERT_EQUAL(0.0, lon);
}

}  // end namespace kmlengine

TEST_MAIN
