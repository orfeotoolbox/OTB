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

#include "kml/dom/schema.h"
#include <string>
#include "kml/dom/kml22.h"
#include "kml/dom/kml_factory.h"
#include "kml/base/unit_test.h"

namespace kmldom {

// <SimpleField>
class SimpleFieldTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(SimpleFieldTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    simplefield_ = KmlFactory::GetFactory()->CreateSimpleField();
  }
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  SimpleFieldPtr simplefield_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SimpleFieldTest);

void SimpleFieldTest::TestType() {
  CPPUNIT_ASSERT(Type_SimpleField == simplefield_->Type());
  CPPUNIT_ASSERT(true == simplefield_->IsA(Type_SimpleField));
}

void SimpleFieldTest::TestDefaults() {
  CPPUNIT_ASSERT("" == simplefield_->get_type());
  CPPUNIT_ASSERT(false == simplefield_->has_type());
  CPPUNIT_ASSERT("" == simplefield_->get_name());
  CPPUNIT_ASSERT(false == simplefield_->has_name());
  CPPUNIT_ASSERT("" == simplefield_->get_displayname());
  CPPUNIT_ASSERT(false == simplefield_->has_displayname());
}

void SimpleFieldTest::TestSetToDefaultValues() {
  TestDefaults();
  simplefield_->set_type(simplefield_->get_type());
  CPPUNIT_ASSERT(true == simplefield_->has_type());
  simplefield_->set_name(simplefield_->get_name());
  CPPUNIT_ASSERT(true == simplefield_->has_name());
  simplefield_->set_displayname(simplefield_->get_displayname());
  CPPUNIT_ASSERT(true == simplefield_->has_displayname());
}

void SimpleFieldTest::TestSetGetHasClear() {
  std::string type("tom");
  simplefield_->set_type(type);
  CPPUNIT_ASSERT(true == simplefield_->has_type());
  CPPUNIT_ASSERT(type == simplefield_->get_type());
  simplefield_->clear_type();

  std::string name("tom");
  simplefield_->set_name(name);
  CPPUNIT_ASSERT(true == simplefield_->has_name());
  CPPUNIT_ASSERT(name == simplefield_->get_name());
  simplefield_->clear_name();

  std::string displayname("dick");
  simplefield_->set_displayname(displayname);
  CPPUNIT_ASSERT(true == simplefield_->has_displayname());
  CPPUNIT_ASSERT(displayname == simplefield_->get_displayname());
  simplefield_->clear_displayname();

  TestDefaults();
}

// <Schema>
class SchemaTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(SchemaTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST(TestLists);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    schema_ = KmlFactory::GetFactory()->CreateSchema();
  }
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();
  void TestLists();

 private:
  SchemaPtr schema_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SchemaTest);

void SchemaTest::TestType() {
  CPPUNIT_ASSERT(Type_Schema == schema_->Type());
  CPPUNIT_ASSERT(true == schema_->IsA(Type_Schema));
  CPPUNIT_ASSERT(true == schema_->IsA(Type_Object));
}

void SchemaTest::TestDefaults() {
  CPPUNIT_ASSERT("" == schema_->get_name());
  CPPUNIT_ASSERT(false == schema_->has_name());
  CPPUNIT_ASSERT("" == schema_->get_id());
  CPPUNIT_ASSERT(false == schema_->has_id());
}

void SchemaTest::TestSetToDefaultValues() {
  TestDefaults();
  schema_->set_name(schema_->get_name());
  CPPUNIT_ASSERT(true == schema_->has_name());
  schema_->set_id(schema_->get_id());
  CPPUNIT_ASSERT(true == schema_->has_id());
}

void SchemaTest::TestSetGetHasClear() {
  std::string name("tom");
  schema_->set_name(name);
  CPPUNIT_ASSERT(true == schema_->has_name());
  CPPUNIT_ASSERT(name == schema_->get_name());
  schema_->clear_name();

  std::string id("dick");
  schema_->set_id(id);
  CPPUNIT_ASSERT(true == schema_->has_id());
  CPPUNIT_ASSERT(id == schema_->get_id());
  schema_->clear_id();

  TestDefaults();
}

void SchemaTest::TestLists() {
  // Vector is empty.
  CPPUNIT_ASSERT(0 == schema_->get_simplefield_array_size());
  // Add three <SimpleField> elements:
  schema_->add_simplefield(KmlFactory::GetFactory()->CreateSimpleField());
  schema_->add_simplefield(KmlFactory::GetFactory()->CreateSimpleField());
  schema_->add_simplefield(KmlFactory::GetFactory()->CreateSimpleField());
  // We have three items in the array:
  CPPUNIT_ASSERT(3 == schema_->get_simplefield_array_size());
  for (size_t i = 0; i < schema_->get_simplefield_array_size(); ++i) {
    CPPUNIT_ASSERT(
        Type_SimpleField == schema_->get_simplefield_array_at(i)->Type());
  }
}


}  // end namespace kmldom

TEST_MAIN
