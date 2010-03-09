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

// This file contains the unit tests for the XsdComplexType class.

#include "kml/xsd/xsd_complex_type.h"
#include "gtest/gtest.h"
#include "kml/base/attributes.h"
#include "kml/xsd/xsd_simple_type.h"
#include "kml/xsd/xsd_util.h"

using kmlbase::Attributes;

namespace kmlxsd {

// This class is the unit test fixture for the KmlHandler class.
class XsdComplexTypeTest : public testing::Test {
 protected:
  Attributes attributes_;
  XsdComplexTypePtr xsd_complex_type_;
};

TEST_F(XsdComplexTypeTest, TestBasicCreate) {
  // <xs:complexType name="chocolateType"/>
  const string kChocolateType("chocolateType");
  xsd_complex_type_ = CreateXsdComplexType(kChocolateType);
  ASSERT_TRUE(xsd_complex_type_);
  ASSERT_EQ(kChocolateType, xsd_complex_type_->get_name());
}

TEST_F(XsdComplexTypeTest, TestBadCreate) {
  // <xs:complexType notname="chocolateType"/>
  const string kChocolateType("chocolateType");
  attributes_.SetString("notname", kChocolateType);

  xsd_complex_type_ = XsdComplexType::Create(attributes_);

  // No name, no XsdComplexType.
  ASSERT_FALSE(xsd_complex_type_);
}

// Verify set_extension_base() and get_extension_base().
TEST_F(XsdComplexTypeTest, TestExtension) {
  // Create the XsdComplexType: <xs:complexType name="chocolateType"/>
  const string kChocolateType("chocolateType");
  xsd_complex_type_ = CreateXsdComplexType(kChocolateType);
  // Set the extension base
  // <xs:extension base="kml:ObjectType">
  const string kExtensionBase("kml:ObjectType");
  xsd_complex_type_->set_extension_base(kExtensionBase);
  ASSERT_EQ(kExtensionBase, xsd_complex_type_->get_extension_base());
}

TEST_F(XsdComplexTypeTest, TestAddElement) {
  // Create the XsdComplexType: <xs:complexType name="chocolateType"/>.
  const string kChocolateType("chocolateType");
  xsd_complex_type_ = CreateXsdComplexType(kChocolateType);
  // Add a child XsdElement: // <element name="latitude" type="double"/>.
  const string kLatitude("latitude");
  XsdElementPtr latitude = CreateXsdElement(kLatitude, "double");
  ASSERT_TRUE(latitude);
  xsd_complex_type_->add_element(latitude);
  ASSERT_EQ(static_cast<size_t>(1), xsd_complex_type_->get_sequence_size());
  ASSERT_EQ(kLatitude, xsd_complex_type_->get_sequence_at(0)->get_name());
  // Add another child XsdElement: // <element name="longitude" type="double"/>.
  const string kLongitude("longitude");
  XsdElementPtr longitude = CreateXsdElement(kLongitude, "double");
  ASSERT_TRUE(longitude);
  xsd_complex_type_->add_element(longitude);
  ASSERT_EQ(static_cast<size_t>(2), xsd_complex_type_->get_sequence_size());
  ASSERT_EQ(kLatitude, xsd_complex_type_->get_sequence_at(0)->get_name());
  ASSERT_EQ(kLongitude, xsd_complex_type_->get_sequence_at(1)->get_name());
}

}  // end namespace kmlxsd

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
