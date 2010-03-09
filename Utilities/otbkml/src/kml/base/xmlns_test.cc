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

// This file contains the unit tests for the Xmlns class.

#include "kml/base/xmlns.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"

namespace kmlbase {

class XmlnsTest : public testing::Test {
 protected:
  boost::scoped_ptr<Attributes> attributes_;
  boost::scoped_ptr<Xmlns> xmlns_;
};

// Just to pick a random example test case... (this is from ogckml22.xsd).
// <schema xmlns="http://www.w3.org/2001/XMLSchema"
//         xmlns:kml="http://www.opengis.net/kml/2.2"
//         xmlns:atom="http://www.w3.org/2005/Atom"
//         xmlns:xal="urn:oasis:names:tc:ciq:xsdschema:xAL:2.0"
//         targetNamespace="http://www.opengis.net/kml/2.2"
//         elementFormDefault="qualified"
//         version="2.2.0">
// Expat turns the above into this list.
static const char* kSchemaAttrs[] = {
  "xmlns", "http://www.w3.org/2001/XMLSchema",
  "xmlns:kml", "http://www.opengis.net/kml/2.2",
  "xmlns:atom", "http://www.w3.org/2005/Atom",
  "xmlns:xal", "urn:oasis:names:tc:ciq:xsdschema:xAL:2.0",
  "targetNamespace", "http://www.opengis.net/kml/2.2",
  "elementFormDefault", "qualified",
  "version", "2.2.0",
  NULL
};

TEST_F(XmlnsTest, TestCreate) {
  attributes_.reset(Attributes::Create(kSchemaAttrs));
  ASSERT_TRUE(attributes_.get());
  // This is the method under test.
  xmlns_.reset(Xmlns::Create(*attributes_));
  // The default namespace is the value of the "xmlns" attribute.
  ASSERT_EQ(string(kSchemaAttrs[1]), xmlns_->get_default());
  ASSERT_EQ(string(kSchemaAttrs[3]),
                       xmlns_->GetNamespace("kml"));
  ASSERT_EQ(string(kSchemaAttrs[5]),
                       xmlns_->GetNamespace("atom"));
}

// Verify the NULL return path of Create().
TEST_F(XmlnsTest, TestNullCreate) {
  attributes_.reset(new Attributes);  // Empty attributes.
  xmlns_.reset(Xmlns::Create(*attributes_));
  // No attributes, no Xmlns.
  ASSERT_FALSE(xmlns_.get());
}

TEST_F(XmlnsTest, TestGetKey) {
  attributes_.reset(new Attributes);
  const string kPrefix("mcn");
  const string kNamespace("my:cool:namespace");
  attributes_->SetString(string("xmlns:") + kPrefix, kNamespace);
  xmlns_.reset(Xmlns::Create(*attributes_));
  ASSERT_EQ(kPrefix, xmlns_->GetKey(kNamespace));
}

// Verify the GetPrefixes() method.
TEST_F(XmlnsTest, TestGetPrefixes) {
  attributes_.reset(Attributes::Create(kSchemaAttrs));
  ASSERT_TRUE(attributes_.get());
  xmlns_.reset(Xmlns::Create(*attributes_));
  // This is the method under test.
  std::vector<string> prefix_vector;
  xmlns_->GetPrefixes(&prefix_vector);
  ASSERT_EQ(static_cast<size_t>(3), prefix_vector.size());
  ASSERT_EQ(string("atom"), prefix_vector[0]);
  ASSERT_EQ(string("kml"), prefix_vector[1]);
  ASSERT_EQ(string("xal"), prefix_vector[2]);
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
