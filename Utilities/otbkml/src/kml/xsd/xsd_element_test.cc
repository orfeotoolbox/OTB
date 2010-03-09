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

// This file contains the unit tests for the XsdElement class.

#include "kml/xsd/xsd_element.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/base/attributes.h"
#include "kml/xsd/xsd_util.h"

namespace kmlxsd {

// This class is the unit test fixture for the XsdElement class.
class XsdElementTest : public testing::Test {
 protected:
  kmlbase::Attributes attributes_;
  boost::scoped_ptr<XsdElement> xsd_element_;
};

TEST_F(XsdElementTest, TestBasicCreate) {
  // <xs:element name="sjokolade"/>
  const string kSjokolade("sjokolade");
  attributes_.SetString(kName, kSjokolade);
  xsd_element_.reset(XsdElement::Create(attributes_));
  ASSERT_TRUE(xsd_element_.get());
  ASSERT_EQ(kSjokolade, xsd_element_->get_name());
  ASSERT_FALSE(xsd_element_->is_ref());
}

TEST_F(XsdElementTest, TestCreateNameType) {
  // <element name="address" type="string"/>, for example.
  const string kAddress("address");
  const string kString("string");
  attributes_.SetString(kName, kAddress);
  attributes_.SetString(kType, kString);
  xsd_element_.reset(XsdElement::Create(attributes_));
  ASSERT_TRUE(xsd_element_.get());
  ASSERT_EQ(kAddress, xsd_element_->get_name());
  ASSERT_EQ(kString, xsd_element_->get_type());
  ASSERT_FALSE(xsd_element_->is_ref());
}

TEST_F(XsdElementTest, TestCreateNameTypeDefault) {
  // <element name="altitude" type="double" default="0.0"/>
  const string kAltitude("altitude");
  const string kDouble("double");
  const string kZZ("0.0");
  attributes_.SetString(kName, kAltitude);
  attributes_.SetString(kType, kDouble);
  attributes_.SetString(kDefault, kZZ);

  xsd_element_.reset(XsdElement::Create(attributes_));

  ASSERT_TRUE(xsd_element_.get());
  ASSERT_EQ(kAltitude, xsd_element_->get_name());
  ASSERT_EQ(kDouble, xsd_element_->get_type());
  ASSERT_EQ(kZZ, xsd_element_->get_default());
  ASSERT_FALSE(xsd_element_->is_ref());
}

TEST_F(XsdElementTest, TestCreateNameAbstract) {
  // <element name="altitudeModeGroup" abstract="true"/>
  const string kAltitudeModeGroup("altitudeModeGroup");
  attributes_.SetString(kName, kAltitudeModeGroup);
  attributes_.SetString(kAbstract, "true");

  xsd_element_.reset(XsdElement::Create(attributes_));

  ASSERT_TRUE(xsd_element_.get());
  ASSERT_EQ(kAltitudeModeGroup, xsd_element_->get_name());
  ASSERT_EQ(true, xsd_element_->is_abstract());
  ASSERT_FALSE(xsd_element_->is_ref());
}

TEST_F(XsdElementTest, TestCreateNameTypeDefaultSubstitutionGroup) {
  // <element name="altitudeMode" type="kml:altitudeModeEnumType"
  //  default="clampToGround" substitutionGroup="kml:altitudeModeGroup"/>
  const string kAltitudeMode("altitudeMode");
  const string kAltitudeEnum("kml:altitudeModeEnumType");
  const string kClampToGround("clampToGround");
  const string kAltitudeModeGroup("kml:altitudeModeGroup");
  attributes_.SetString(kName, kAltitudeMode);
  attributes_.SetString(kType, kAltitudeEnum);
  attributes_.SetString(kDefault, kClampToGround);
  attributes_.SetString(kSubstitutionGroup, kAltitudeModeGroup);

  xsd_element_.reset(XsdElement::Create(attributes_));

  ASSERT_TRUE(xsd_element_.get());
  ASSERT_EQ(kAltitudeMode, xsd_element_->get_name());
  ASSERT_EQ(kAltitudeEnum, xsd_element_->get_type());
  ASSERT_EQ(kClampToGround, xsd_element_->get_default());
  ASSERT_EQ(kAltitudeModeGroup, xsd_element_->get_substitution_group());
  ASSERT_FALSE(xsd_element_->is_ref());
}

// Verify the parse of <xs:element type="..."/> and get_type_id().
TEST_F(XsdElementTest, TestGetTypeId) {
  xsd_element_.reset(CreateXsdElement("somethingIntegral", "int"));
  ASSERT_EQ(XsdPrimitiveType::XSD_INT, xsd_element_->get_type_id());
  xsd_element_.reset(CreateXsdElement("somethingDouble", "double"));
  ASSERT_EQ(XsdPrimitiveType::XSD_DOUBLE, xsd_element_->get_type_id());
  xsd_element_.reset(CreateXsdElement("yesOrNo", "boolean"));
  ASSERT_EQ(XsdPrimitiveType::XSD_BOOLEAN, xsd_element_->get_type_id());
  xsd_element_.reset(CreateXsdElement("saySomething", "string"));
  ASSERT_EQ(XsdPrimitiveType::XSD_STRING, xsd_element_->get_type_id());
  // A user defined type such as a complexType is properly not a primitive.
  xsd_element_.reset(CreateXsdElement("Placemark", "PlacemarkType"));
  ASSERT_EQ(XsdPrimitiveType::XSD_INVALID, xsd_element_->get_type_id());
}

}  // end namespace kmlxsd

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
