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

// This file contains the unit tests for the abstract Object element.

#include "kml/dom/object.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_ptr.h"
#include "gtest/gtest.h"

namespace kmldom {

class ObjectTest : public testing::Test {
 protected:
  virtual void SetUp() {
    object_.reset(new TestObject());
  }

  // Object is abstract, hence its constructor is protected.
  class TestObject : public Object {};
  boost::scoped_ptr<TestObject> object_;
};

TEST_F(ObjectTest, TestType) {
  ASSERT_TRUE(object_->IsA(Type_Object));
  ASSERT_FALSE(object_->IsA(Type_Unknown));
}

// Verify proper defaults:
TEST_F(ObjectTest, TestDefaults) {
  ASSERT_FALSE(object_->has_id());
  ASSERT_EQ(string(""), object_->get_id());
  ASSERT_FALSE(object_->has_targetid());
  ASSERT_EQ(string(""), object_->get_targetid());
}

// Verify setting default makes has_xxx() true:
TEST_F(ObjectTest, TestSetToDefaultValues) {
  object_->set_id(object_->get_id());
  ASSERT_TRUE(object_->has_id());
  object_->set_targetid(object_->get_targetid());
  ASSERT_TRUE(object_->has_targetid());
}

// Verify set, get, has, clear:
TEST_F(ObjectTest, TestSetGetHasClear) {
  // Non-default values:
  string id("id");
  string targetid("targetid");

  // Set all fields:
  object_->set_id(id);
  object_->set_targetid(targetid);

  // Verify getter and has_xxx():
  ASSERT_EQ(id, object_->get_id());
  ASSERT_EQ(targetid, object_->get_targetid());

  // Clear all fields:
  object_->clear_id();
  object_->clear_targetid();
}

TEST_F(ObjectTest, TestParse) {
  const string kId("foo");
  const string kTargetId("bar");
  const string kKml(
      "<Placemark id=\"" + kId + "\" targetId=\"" + kTargetId + "\" />");
  string errors;
  ElementPtr root = Parse(kKml, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const PlacemarkPtr placemark = AsPlacemark(root);
  ASSERT_TRUE(placemark);
  ASSERT_TRUE(placemark->has_id());
  ASSERT_TRUE(placemark->has_targetid());
  ASSERT_EQ(kId, placemark->get_id());
  ASSERT_EQ(kTargetId, placemark->get_targetid());
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

