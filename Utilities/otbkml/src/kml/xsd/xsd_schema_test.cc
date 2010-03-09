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

// This file contains the unit tests for the XsdSchema class.

#include "kml/xsd/xsd_schema.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/attributes.h"
#include "gtest/gtest.h"
#include "kml/xsd/xsd_util.h"

using kmlbase::Attributes;

namespace kmlxsd {

// This class is the unit test fixture for the XsdSchema class.
class XsdSchemaTest : public testing::Test {
 protected:
  XsdSchemaPtr xsd_schema_;
  Attributes attributes_;
};

// Verify NULL is returned if no attributes are supplied.
TEST_F(XsdSchemaTest, TestNullCreateNoAttributes) {
  xsd_schema_ = XsdSchema::Create(attributes_);
  ASSERT_FALSE(xsd_schema_);
}

// Verify NULL is returned from Create() if attributes has no targetNamespace.
TEST_F(XsdSchemaTest, TestNullCreateNoTargetNamespace) {
  attributes_.SetString("random", "junk");
  xsd_schema_ = XsdSchema::Create(attributes_);
  ASSERT_FALSE(xsd_schema_);
}

// Verify processing of Create method.
TEST_F(XsdSchemaTest, TestCreate) {
  // Send down the attributes from:
  //   <schema xmlns:mcn="my:cool:namespace"
  //           targetNamespace="my:cool:namespace"/>
  const string kNamespace("my:cool:namespace");
  const string kPrefix("mcn");
  attributes_.SetString(string("xmlns:") + kPrefix, kNamespace);
  attributes_.SetString(kTargetNamespace, kNamespace);
  xsd_schema_ = XsdSchema::Create(attributes_);
  ASSERT_TRUE(xsd_schema_);
  ASSERT_EQ(kNamespace, xsd_schema_->get_target_namespace());
  ASSERT_EQ(kPrefix, xsd_schema_->get_target_namespace_prefix());
}

// Verify SplitNsName().
TEST_F(XsdSchemaTest, TestSplitNsName) {
  const string kPrefix("myml");
  xsd_schema_ = CreateXsdSchema(kPrefix, "who:cares");
  const string kMyElement("MyElement");
  string ncname;
  ASSERT_TRUE(xsd_schema_->SplitNsName(kPrefix + ":" + kMyElement, &ncname));
  ASSERT_EQ(kMyElement, ncname);
}

}  // end namespace kmlxsd

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
