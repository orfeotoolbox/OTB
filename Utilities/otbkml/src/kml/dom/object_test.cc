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
#include "kml/base/unit_test.h"

namespace kmldom {

class ObjectTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ObjectTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST(TestParse);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    object_ = new TestObject();
  }

  // Called after all tests.
  void tearDown() {
    delete object_;
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();
  void TestParse();

 private:
  // Object is abstract, hence its constructor is protected.
  class TestObject : public Object {};
  TestObject* object_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ObjectTest);

void ObjectTest::TestType() {
  CPPUNIT_ASSERT(true == object_->IsA(Type_Object));
}

// Verify proper defaults:
void ObjectTest::TestDefaults() {
  CPPUNIT_ASSERT(false == object_->has_id());
  CPPUNIT_ASSERT("" == object_->get_id());
  CPPUNIT_ASSERT(false == object_->has_targetid());
  CPPUNIT_ASSERT("" == object_->get_targetid());
}

// Verify setting default makes has_xxx() true:
void ObjectTest::TestSetToDefaultValues() {
  TestDefaults();
  object_->set_id(object_->get_id());
  CPPUNIT_ASSERT(true == object_->has_id());
  object_->set_targetid(object_->get_targetid());
  CPPUNIT_ASSERT(true == object_->has_targetid());
}

// Verify set, get, has, clear:
void ObjectTest::TestSetGetHasClear() {
  // Non-default values:
  std::string id("id");
  std::string targetid("targetid");

  // Set all fields:
  object_->set_id(id);
  object_->set_targetid(targetid);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(id == object_->get_id());
  CPPUNIT_ASSERT(targetid == object_->get_targetid());

  // Clear all fields:
  object_->clear_id();
  object_->clear_targetid();

  // Verify now in default state:
  TestDefaults();
}

void ObjectTest::TestParse() {
  const std::string kId("foo");
  const std::string kTargetId("bar");
  const std::string kKml(
      "<Placemark id=\"" + kId + "\" targetId=\"" + kTargetId + "\" />");
  std::string errors;
  ElementPtr root = Parse(kKml, &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const PlacemarkPtr placemark = AsPlacemark(root);
  CPPUNIT_ASSERT(placemark);
  CPPUNIT_ASSERT(placemark->has_id());
  CPPUNIT_ASSERT(placemark->has_targetid());
  CPPUNIT_ASSERT_EQUAL(kId, placemark->get_id());
  CPPUNIT_ASSERT_EQUAL(kTargetId, placemark->get_targetid());
}

}  // end namespace kmldom

TEST_MAIN

