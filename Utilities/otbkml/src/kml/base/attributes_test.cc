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

// This file contains the unit tests for the internal Attributes class.

#include "kml/base/attributes.h"
#include <algorithm>
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"

namespace kmlbase {

static const char kAttr0[] = "id";
static const char kAttr1[] = "fraction";
static const char kAttr2[] = "xunits";
static const char kNoSuchAttr[] = "no-such-attr";

class AttributesTest : public testing::Test {
 protected:
  virtual void SetUp() {
    attributes_.reset(new Attributes);
  }
  boost::scoped_ptr<Attributes> attributes_;
};

TEST_F(AttributesTest, TestCreate) {
  // A list of name-value pairs as expat might produce.
  const char* atts[] = {
    "name",
    "Placemark",
    "type",
    "kml:PlacemarkType",
    "substitutionGroup",
    "kml:AbstractFeatureGroup",
    NULL
  };
  // Call the method under test.
  attributes_.reset(Attributes::Create(atts));
  ASSERT_TRUE(attributes_.get());
  string got_val;
  ASSERT_TRUE(attributes_->GetValue(atts[0], &got_val));
  ASSERT_EQ(string(atts[1]), got_val);
  ASSERT_TRUE(attributes_->GetValue(atts[2], &got_val));
  ASSERT_EQ(string(atts[3]), got_val);
  ASSERT_TRUE(attributes_->GetValue(atts[4], &got_val));
  ASSERT_EQ(string(atts[5]), got_val);
  ASSERT_FALSE(attributes_->GetValue("no-such-attr", &got_val));
  // Verify null output is well behaved.
  string* p = NULL;
  ASSERT_TRUE(attributes_->GetValue(atts[0], p));
  ASSERT_FALSE(attributes_->GetValue("no-such_attr", p));
}

TEST_F(AttributesTest, TestCreateOdd) {
  // A bad atts list.
  const char* atts[] = { "a0", "a1", "b0", 0 };
  // Call the method under test.
  attributes_.reset(Attributes::Create(atts));
  ASSERT_TRUE(attributes_.get());
  string got_val;
  ASSERT_TRUE(attributes_->GetValue(atts[0], &got_val));
  ASSERT_EQ(string(atts[1]), got_val);
  ASSERT_FALSE(attributes_->GetValue(atts[2], &got_val));
}

TEST_F(AttributesTest, TestGetStringDesctructive) {
  const char* atts[] = { "id", "placemark-123", "unknown", "somevalue", 0 };
  attributes_.reset(Attributes::Create(atts));
  string got_val;
  ASSERT_TRUE(attributes_->GetValue(atts[0], &got_val));
  string got_val_again;
  const string kAtt0(atts[0]);
  ASSERT_TRUE(attributes_->CutValue(kAtt0, &got_val_again));
  ASSERT_FALSE(attributes_->GetValue(atts[0], &got_val_again));
}

TEST_F(AttributesTest, TestSetGetString) {
  const string kVal0 = "val0";
  const string kVal1 = "val1";
  attributes_->SetValue(kAttr0, kVal0);
  attributes_->SetValue(kAttr1, kVal1);
  string got_val;
  ASSERT_TRUE(attributes_->GetValue(kAttr0, &got_val));
  ASSERT_EQ(kVal0, got_val);
  ASSERT_TRUE(attributes_->GetValue(kAttr1, &got_val));
  ASSERT_EQ(kVal1, got_val);
  ASSERT_TRUE(false == attributes_->GetValue(kNoSuchAttr, &got_val));
  ASSERT_EQ(kVal1, got_val);
}

TEST_F(AttributesTest, TestSetGetDouble) {
  const double kVal0 = 123.456;
  const double kVal1 = 987.654321;
  attributes_->SetValue(kAttr0, kVal0);
  attributes_->SetValue(kAttr1, kVal1);
  double got_val;
  ASSERT_TRUE(attributes_->GetValue(kAttr0, &got_val));
  ASSERT_EQ(kVal0, got_val);
  ASSERT_TRUE(attributes_->GetValue(kAttr1, &got_val));
  ASSERT_EQ(kVal1, got_val);
  ASSERT_TRUE(false == attributes_->GetValue(kNoSuchAttr, &got_val));
  ASSERT_EQ(kVal1, got_val);
}

TEST_F(AttributesTest, TestSetGetInt) {
  const int kVal0 = 123;
  const int kVal1 = -987;
  attributes_->SetValue(kAttr0, kVal0);
  attributes_->SetValue(kAttr1, kVal1);
  int got_val;
  ASSERT_TRUE(attributes_->GetValue(kAttr0, &got_val));
  ASSERT_EQ(kVal0, got_val);
  ASSERT_TRUE(attributes_->GetValue(kAttr1, &got_val));
  ASSERT_EQ(kVal1, got_val);
  ASSERT_TRUE(false == attributes_->GetValue(kNoSuchAttr, &got_val));
  ASSERT_EQ(kVal1, got_val);
}

TEST_F(AttributesTest, TestClone) {
  const string kVal0 = "val0";
  const double kVal1 = 123.456789;
  attributes_->SetValue(kAttr0, kVal0);
  attributes_->SetValue(kAttr1, kVal1);
  Attributes* clone = attributes_->Clone();
  string got_string;
  ASSERT_TRUE(clone->GetValue(kAttr0, &got_string));
  ASSERT_EQ(kVal0, got_string);
  double got_double;
  ASSERT_TRUE(clone->GetValue(kAttr1, &got_double));
  ASSERT_EQ(kVal1, got_double);
  delete clone;
}

TEST_F(AttributesTest, TestMerge) {
  const string kVal0 = "val0";
  const double kVal1a = 123.456789;
  const double kVal1b = 78.90123;
  const string kVal2 = "fraction";
  attributes_->SetValue(kAttr0, kVal0);
  attributes_->SetValue(kAttr1, kVal1a);
  Attributes attributes;
  attributes.SetValue(kAttr1, kVal1b);
  attributes.SetValue(kAttr2, kVal2);
  attributes_->MergeAttributes(attributes);
  string got_string;
  ASSERT_TRUE(attributes_->GetValue(kAttr0, &got_string));
  ASSERT_EQ(kVal0, got_string);
  double got_double;
  ASSERT_TRUE(attributes_->GetValue(kAttr1, &got_double));
  ASSERT_EQ(kVal1b, got_double);
  ASSERT_TRUE(attributes_->GetValue(kAttr2, &got_string));
  ASSERT_EQ(kVal2, got_string);
}

TEST_F(AttributesTest, TestSerialize) {
  const string kVal0 = "val0";
  const double kVal1 = 123.456789;
  attributes_->SetValue(kAttr0, kVal0);
  attributes_->SetValue(kAttr1, kVal1);
  string serialized;
  attributes_->Serialize(&serialized);
  const string expecting = " " + string(kAttr1) + "=\"123.456789\" "
    + string(kAttr0) + "=\"" + kVal0 + "\"";
  ASSERT_EQ(expecting, serialized);
}

TEST_F(AttributesTest, TestSplit) {
  // <kml xmlns="http://www.opengis.net/kml/2.2"
  //      xmlns:ex="http://vendor.com/kml/2.2ext">
  // Expat turns the above XML into this:
  const char* atts[] = {
    "xmlns", "http://www.opengis.net/kml/2.2",
    "xmlns:ex", "http://vendor.com/kml/2.2ext",
    NULL
  };
  attributes_.reset(Attributes::Create(atts));
  ASSERT_TRUE(attributes_.get());
  // This is the method under test.
  boost::scoped_ptr<Attributes> xmlns_(attributes_->SplitByPrefix("xmlns"));
  ASSERT_TRUE(xmlns_.get());
  ASSERT_EQ(static_cast<size_t>(1), xmlns_->GetSize());
  string val;
  ASSERT_TRUE(xmlns_->GetValue("ex", &val));
  ASSERT_EQ(string(atts[3]), val);
}

TEST_F(AttributesTest, TestMatchNoDefault) {
  const char* atts[] = {
    "xmlns:kml", "http://www.opengis.net/kml/2.2",
    "xmlns:ex", "http://vendor.com/kml/2.2ext",
    NULL
  };
  attributes_.reset(Attributes::Create(atts));
  ASSERT_TRUE(attributes_.get());
  // This is the method under test.
  boost::scoped_ptr<Attributes> xmlns_(attributes_->SplitByPrefix("xmlns"));
  ASSERT_TRUE(xmlns_.get());
  ASSERT_EQ(static_cast<size_t>(2), xmlns_->GetSize());
  string val;
  ASSERT_TRUE(xmlns_->GetValue("kml", &val));
  ASSERT_EQ(string(atts[1]), val);
  ASSERT_TRUE(xmlns_->GetValue("ex", &val));
  ASSERT_EQ(string(atts[3]), val);
}

TEST_F(AttributesTest, TestGetAttrNames) {
  // A list of name-value pairs as expat might produce.
  const char* atts[] = {
    "name",
    "Placemark",
    "type",
    "kml:PlacemarkType",
    "substitutionGroup",
    "kml:AbstractFeatureGroup",
    NULL
  };
  attributes_.reset(Attributes::Create(atts));
  std::vector<string> attr_names;
  attributes_->GetAttrNames(&attr_names);
  ASSERT_EQ(static_cast<size_t>(3), attr_names.size());
  ASSERT_FALSE(attr_names.end() ==
               std::find(attr_names.begin(), attr_names.end(), "name"));
  ASSERT_FALSE(attr_names.end() ==
               std::find(attr_names.begin(), attr_names.end(), "type"));
  ASSERT_FALSE(attr_names.end() == std::find(attr_names.begin(),
                                             attr_names.end(),
                                             "substitutionGroup"));
}

TEST_F(AttributesTest, TestIterator) {
  // NOTE: the keys are in map order.
  const char* atts[] = { "a", "z", "b", "y", "c", "x", NULL };
  attributes_.reset(Attributes::Create(atts));
  ASSERT_TRUE(attributes_.get());
  StringMapIterator iter = attributes_->CreateIterator();
  ASSERT_EQ(string(atts[0]), iter.Data().first);
  ASSERT_EQ(string(atts[1]), iter.Data().second);
  iter.Advance();
  ASSERT_FALSE(iter.AtEnd());
  ASSERT_EQ(string(atts[2]), iter.Data().first);
  ASSERT_EQ(string(atts[3]), iter.Data().second);
  iter.Advance();
  ASSERT_FALSE(iter.AtEnd());
  ASSERT_EQ(string(atts[4]), iter.Data().first);
  ASSERT_EQ(string(atts[5]), iter.Data().second);
  iter.Advance();
  ASSERT_TRUE(iter.AtEnd());
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

