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

#include "kml/xsd/xsd_primitive_type.h"
#include "gtest/gtest.h"

namespace kmlxsd {

// This class is the unit test fixture for the XsdTypes class.
class XsdPrimitiveTypeTest : public testing::Test {
};

// Verify NULL is returned if no attributes are supplied.
TEST_F(XsdPrimitiveTypeTest, TestXsdPrimitiveTypeNameId) {
  const string kBoolean("boolean");
  const string kDouble("double");
  const string kInt("int");
  const string kString("string");

  ASSERT_EQ(kBoolean,
      XsdPrimitiveType::GetTypeName(XsdPrimitiveType::XSD_BOOLEAN));
  ASSERT_EQ(kDouble,
      XsdPrimitiveType::GetTypeName(XsdPrimitiveType::XSD_DOUBLE));
  ASSERT_EQ(kInt,
      XsdPrimitiveType::GetTypeName(XsdPrimitiveType::XSD_INT));
  ASSERT_EQ(kString,
      XsdPrimitiveType::GetTypeName(XsdPrimitiveType::XSD_STRING));

  ASSERT_EQ(XsdPrimitiveType::XSD_BOOLEAN,
                       XsdPrimitiveType::GetTypeId(kBoolean));
  ASSERT_EQ(XsdPrimitiveType::XSD_DOUBLE,
                       XsdPrimitiveType::GetTypeId(kDouble));
  ASSERT_EQ(XsdPrimitiveType::XSD_INT,
                       XsdPrimitiveType::GetTypeId(kInt));
  ASSERT_EQ(XsdPrimitiveType::XSD_STRING,
                       XsdPrimitiveType::GetTypeId(kString));
  ASSERT_EQ(XsdPrimitiveType::XSD_INVALID,
                       XsdPrimitiveType::GetTypeId("no-such-xsd-type"));
  ASSERT_EQ(XsdPrimitiveType::XSD_INVALID,
                       XsdPrimitiveType::GetTypeId("PlacemarkType"));
}

}  // end namespace kmlxsd

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
