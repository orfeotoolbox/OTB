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

// This file contains the unit tests for the Xsd class.

#include "kml/dom/xsd.h"
#include <string>
#include "kml/dom/kml22.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class XsdTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(XsdTest);
  CPPUNIT_TEST(TestGoodElement);
  CPPUNIT_TEST(TestBadElement);
  CPPUNIT_TEST(TestUglyElement);
  CPPUNIT_TEST(TestGoodEnum);
  CPPUNIT_TEST(TestBadEnum);
  CPPUNIT_TEST(TestUglyEnum);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestGoodElement();
  void TestBadElement();
  void TestUglyElement();
  void TestGoodEnum();
  void TestBadEnum();
  void TestUglyEnum();
};

CPPUNIT_TEST_SUITE_REGISTRATION(XsdTest);

// Verify known good values for ElementId(), ElementType() and ElementName().
void XsdTest::TestGoodElement() {
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(Type_Placemark),
                       Xsd::GetSchema()->ElementId("Placemark"));
  CPPUNIT_ASSERT_EQUAL(XSD_COMPLEX_TYPE,
                       Xsd::GetSchema()->ElementType(Type_Placemark));
  CPPUNIT_ASSERT_EQUAL(std::string("Placemark"),
                       Xsd::GetSchema()->ElementName(Type_Placemark));

  CPPUNIT_ASSERT_EQUAL(static_cast<int>(Type_Snippet),
                       Xsd::GetSchema()->ElementId("Snippet"));
  CPPUNIT_ASSERT_EQUAL(XSD_COMPLEX_TYPE,
                       Xsd::GetSchema()->ElementType(Type_Snippet));
  CPPUNIT_ASSERT_EQUAL(std::string("Snippet"),
                       Xsd::GetSchema()->ElementName(Type_Snippet));

  // <z> is known to be the last element.
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(Type_z),
                       Xsd::GetSchema()->ElementId("z"));
  CPPUNIT_ASSERT_EQUAL(XSD_SIMPLE_TYPE,
                       Xsd::GetSchema()->ElementType(Type_z));
  CPPUNIT_ASSERT_EQUAL(std::string("z"),
                       Xsd::GetSchema()->ElementName(Type_z));
}

// Verify that ElementId(), ElementType() and ElementName() are well
// behaved given bad values.
void XsdTest::TestBadElement() {
  CPPUNIT_ASSERT_EQUAL((int)Type_Unknown,
                       Xsd::GetSchema()->ElementId("unknown"));
  // Type_Invalid + 1 is known to be a bad value for a type id.
  CPPUNIT_ASSERT_EQUAL(XSD_UNKNOWN,
                       Xsd::GetSchema()->ElementType(Type_Invalid + 1));
  CPPUNIT_ASSERT_EQUAL(std::string(""),
                       Xsd::GetSchema()->ElementName(Type_Invalid + 1));
}

// Verify that ElementId(), ElementType() and ElementName() are well
// behaved given ugly values.
void XsdTest::TestUglyElement() {
  CPPUNIT_ASSERT_EQUAL((int)Type_Unknown, Xsd::GetSchema()->ElementId(""));
  CPPUNIT_ASSERT_EQUAL(XSD_UNKNOWN, Xsd::GetSchema()->ElementType(0));
  CPPUNIT_ASSERT_EQUAL(std::string(""), Xsd::GetSchema()->ElementName(0));
}

// Verify that a known enum val has the proper id and vice versa.
// Tests the EnumId() and EnumValue() for known good values.
void XsdTest::TestGoodEnum() {
  CPPUNIT_ASSERT_EQUAL((int)ALTITUDEMODE_CLAMPTOGROUND,
    Xsd::GetSchema()->EnumId(Type_altitudeMode, "clampToGround"));
  CPPUNIT_ASSERT_EQUAL(std::string("clampToGround"),
    Xsd::GetSchema()->EnumValue(Type_altitudeMode, ALTITUDEMODE_CLAMPTOGROUND));
}

// Verify that EnumId() is well behaved for an enum value known to be bad.
void XsdTest::TestBadEnum() {
  // This was an actual crash.
  CPPUNIT_ASSERT_EQUAL(-1, Xsd::GetSchema()->EnumId(Type_state, "closed open"));
}

// Verify that EnumId() is well behaved for an enum value known to be ugly.
void XsdTest::TestUglyEnum() {
  CPPUNIT_ASSERT_EQUAL(-1, Xsd::GetSchema()->EnumId(Type_state, ""));
}

}  // end namespace kmldom

TEST_MAIN
