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

// This file contains the unit tests for the XSD utility functions.

#include "kml/xsd/xsd_util.h"
#include "gtest/gtest.h"
#include "kml/xsd/xsd_complex_type.h"
#include "kml/xsd/xsd_element.h"
#include "kml/xsd/xsd_schema.h"

namespace kmlxsd {

class XsdUtilTest : public testing::Test {
};

TEST_F(XsdUtilTest, TestCreateXsdComplexType) {
  const string kTypeName("PlacemarkType");
  XsdComplexTypePtr complex_type = CreateXsdComplexType(kTypeName);
  ASSERT_TRUE(complex_type);
  ASSERT_EQ(kTypeName, complex_type->get_name());
}

TEST_F(XsdUtilTest, TestCreateXsdElement) {
  const string kName("Placemark");
  const string kType("PlacemarkType");
  XsdElementPtr element = CreateXsdElement(kName, kType);
  ASSERT_TRUE(element);
  ASSERT_EQ(kName, element->get_name());
  ASSERT_EQ(kType, element->get_type());
}

TEST_F(XsdUtilTest, TestCreateXsdSchema) {
  const string kPrefix("fooml");
  const string kTargetNamespace("http://foo.com/ml");
  XsdSchemaPtr schema = CreateXsdSchema(kPrefix, kTargetNamespace);
  ASSERT_TRUE(schema);
  ASSERT_EQ(kPrefix, schema->get_target_namespace_prefix());
  ASSERT_EQ(kTargetNamespace, schema->get_target_namespace());
} 

}  // end namespace kmlxsd

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
