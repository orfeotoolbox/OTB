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
#include "kml/dom/kml22.h"
#include "gtest/gtest.h"

namespace kmldom {

class XsdTest : public testing::Test {
};

// Verify known good values for ElementId(), ElementType() and ElementName().
TEST_F(XsdTest, TestGoodElement) {
  ASSERT_EQ(static_cast<int>(Type_Placemark),
            Xsd::GetSchema()->ElementId("Placemark"));
  ASSERT_EQ(XSD_COMPLEX_TYPE,
            Xsd::GetSchema()->ElementType(Type_Placemark));
  ASSERT_EQ(string("Placemark"),
            Xsd::GetSchema()->ElementName(Type_Placemark));

  ASSERT_EQ(static_cast<int>(Type_Snippet),
            Xsd::GetSchema()->ElementId("Snippet"));
  ASSERT_EQ(XSD_COMPLEX_TYPE,
            Xsd::GetSchema()->ElementType(Type_Snippet));
  ASSERT_EQ(string("Snippet"),
            Xsd::GetSchema()->ElementName(Type_Snippet));

  ASSERT_EQ(static_cast<int>(Type_z), Xsd::GetSchema()->ElementId("z"));
  ASSERT_EQ(XSD_SIMPLE_TYPE, Xsd::GetSchema()->ElementType(Type_z));
  ASSERT_EQ(string("z"), Xsd::GetSchema()->ElementName(Type_z));

  ASSERT_EQ(static_cast<int>(Type_XalAddressDetails),
            Xsd::GetSchema()->ElementId("xal:AddressDetails"));
  ASSERT_EQ(XSD_COMPLEX_TYPE,
            Xsd::GetSchema()->ElementType(Type_XalAddressDetails));
  ASSERT_EQ(string("xal:AddressDetails"),
            Xsd::GetSchema()->ElementName(Type_XalAddressDetails));

  ASSERT_EQ(static_cast<int>(Type_XalPostalCode),
            Xsd::GetSchema()->ElementId("xal:PostalCode"));
  ASSERT_EQ(XSD_COMPLEX_TYPE,
            Xsd::GetSchema()->ElementType(Type_XalPostalCode));
  ASSERT_EQ(string("xal:PostalCode"),
            Xsd::GetSchema()->ElementName(Type_XalPostalCode));

  ASSERT_EQ(static_cast<int>(Type_XalAdministrativeArea),
            Xsd::GetSchema()->ElementId("xal:AdministrativeArea"));
  ASSERT_EQ(XSD_COMPLEX_TYPE,
            Xsd::GetSchema()->ElementType(Type_XalAdministrativeArea));
  ASSERT_EQ(string("xal:AdministrativeArea"),
            Xsd::GetSchema()->ElementName(Type_XalAdministrativeArea));

  ASSERT_EQ(static_cast<int>(Type_XalSubAdministrativeArea),
            Xsd::GetSchema()->ElementId("xal:SubAdministrativeArea"));
  ASSERT_EQ(XSD_COMPLEX_TYPE,
            Xsd::GetSchema()->ElementType(Type_XalSubAdministrativeArea));
  ASSERT_EQ(string("xal:SubAdministrativeArea"),
            Xsd::GetSchema()->ElementName(Type_XalSubAdministrativeArea));

  ASSERT_EQ(static_cast<int>(Type_xalThoroughfareName),
            Xsd::GetSchema()->ElementId("xal:ThoroughfareName"));
  ASSERT_EQ(XSD_SIMPLE_TYPE,
            Xsd::GetSchema()->ElementType(Type_xalThoroughfareName));
  ASSERT_EQ(string("xal:ThoroughfareName"),
            Xsd::GetSchema()->ElementName(Type_xalThoroughfareName));

  // <gx:playMode> is known to be the last element.
  ASSERT_EQ(static_cast<int>(Type_GxPlayMode),
            Xsd::GetSchema()->ElementId("gx:playMode"));
  ASSERT_EQ(XSD_SIMPLE_TYPE,
            Xsd::GetSchema()->ElementType(Type_GxPlayMode));
  ASSERT_EQ(string("gx:playMode"),
            Xsd::GetSchema()->ElementName(Type_GxPlayMode));
}

// Verify that ElementId(), ElementType() and ElementName() are well
// behaved given bad values.
TEST_F(XsdTest, TestBadElement) {
  ASSERT_EQ((int)Type_Unknown, Xsd::GetSchema()->ElementId("unknown"));
  // Type_Invalid + 1 is known to be a bad value for a type id.
  ASSERT_EQ(XSD_UNKNOWN, Xsd::GetSchema()->ElementType(Type_Invalid + 1));
  ASSERT_EQ(string(""), Xsd::GetSchema()->ElementName(Type_Invalid + 1));
}

// Verify that ElementId(), ElementType() and ElementName() are well
// behaved given ugly values.
TEST_F(XsdTest, TestUglyElement) {
  ASSERT_EQ((int)Type_Unknown, Xsd::GetSchema()->ElementId(""));
  ASSERT_EQ(XSD_UNKNOWN, Xsd::GetSchema()->ElementType(0));
  ASSERT_EQ(string(""), Xsd::GetSchema()->ElementName(0));
}

// Verify that a known enum val has the proper id and vice versa.
// Tests the EnumId() and EnumValue() for known good values.
TEST_F(XsdTest, TestGoodEnum) {
  ASSERT_EQ((int)ALTITUDEMODE_CLAMPTOGROUND,
    Xsd::GetSchema()->EnumId(Type_altitudeMode, "clampToGround"));
  ASSERT_EQ(string("clampToGround"),
    Xsd::GetSchema()->EnumValue(Type_altitudeMode, ALTITUDEMODE_CLAMPTOGROUND));
}

// Verify that EnumId() is well behaved for an enum value known to be bad.
TEST_F(XsdTest, TestBadEnum) {
  // This was an actual crash.
  ASSERT_EQ(-1, Xsd::GetSchema()->EnumId(Type_state, "closed open"));
}

TEST_F(XsdTest, TestBadEnumValue) {
  // This crashed libkml because Xsd::EnumValue, if passed a negative enum_id,
  // would try to initialize a string from a NULL pointer.
  ASSERT_EQ(string(), Xsd::GetSchema()->EnumValue(0, -1));
}

// Verify that EnumId() is well behaved for an enum value known to be ugly.
TEST_F(XsdTest, TestUglyEnum) {
  ASSERT_EQ(-1, Xsd::GetSchema()->EnumId(Type_state, ""));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
