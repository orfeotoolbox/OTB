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

// This file contains the unit tests for the XsdSimpleType class.

#include "kml/xsd/xsd_simple_type.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/base/attributes.h"
#include "kml/xsd/xsd_util.h"

namespace kmlxsd {

// This class is the unit test fixture for the KmlHandler class.
class XsdSimpleTypeTest : public testing::Test {
 protected:
  kmlbase::Attributes attributes_;
  XsdSimpleTypePtr xsd_simple_type_;
};

TEST_F(XsdSimpleTypeTest, TestBasicCreate) {
  // <xs:simpleType name="anglepos90Type">
  const string kTypeName("anglepos90Type");
  attributes_.SetString(kName, kTypeName);

  xsd_simple_type_ = XsdSimpleType::Create(attributes_);

  ASSERT_TRUE(xsd_simple_type_);
  ASSERT_EQ(kTypeName, xsd_simple_type_->get_name());
}

TEST_F(XsdSimpleTypeTest, TestEnumeration) {
  // <simpleType name="altitudeModeEnumType">
  attributes_.SetString(kName, "altitudeModeEnumType");
  xsd_simple_type_ = XsdSimpleType::Create(attributes_);
  ASSERT_TRUE(xsd_simple_type_);
  ASSERT_FALSE(xsd_simple_type_->IsEnumeration());
  ASSERT_EQ(static_cast<size_t>(0),
                       xsd_simple_type_->get_enumeration_size());

  const string kClampToGround("clampToGround");
  const string kRelativeToGround("relativeToGround");
  const string kAbsolute("absolute");
  xsd_simple_type_->add_enumeration(kClampToGround);
  xsd_simple_type_->add_enumeration(kRelativeToGround);
  xsd_simple_type_->add_enumeration(kAbsolute);

  ASSERT_EQ(static_cast<size_t>(3),
                       xsd_simple_type_->get_enumeration_size());
  ASSERT_EQ(kClampToGround,
                       xsd_simple_type_->get_enumeration_at(0));
  ASSERT_EQ(kRelativeToGround,
                       xsd_simple_type_->get_enumeration_at(1));
  ASSERT_EQ(kAbsolute,
                       xsd_simple_type_->get_enumeration_at(2));

  // Still not an enumeration given lack of restriction base.
  ASSERT_FALSE(xsd_simple_type_->IsEnumeration());

  // Add a restriction base and verify IsEnumeration().
  xsd_simple_type_->set_restriction_base("string");
  ASSERT_TRUE(xsd_simple_type_->IsEnumeration());
}

// This verifies the == operator.
TEST_F(XsdSimpleTypeTest, TestEqualsOperator) {
  XsdSimpleTypePtr altitude_mode_type_;
  XsdSimpleTypePtr angle_pos90_type_;
  // <simpleType name="altitudeModeEnumType">
  attributes_.SetString(kName, "altitudeModeEnumType");
  altitude_mode_type_ = XsdSimpleType::Create(attributes_);
  ASSERT_TRUE(altitude_mode_type_);

  ASSERT_TRUE(altitude_mode_type_ == altitude_mode_type_);

  // <xs:simpleType name="anglepos90Type">
  const string kTypeName("anglepos90Type");
  attributes_.SetString(kName, kTypeName);
  angle_pos90_type_ = XsdSimpleType::Create(attributes_);
  ASSERT_TRUE(angle_pos90_type_);

  ASSERT_FALSE(angle_pos90_type_ == altitude_mode_type_);
}

}  // end namespace kmlxsd

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
