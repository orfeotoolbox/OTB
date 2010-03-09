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
#include "kml/dom/kml22.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "gtest/gtest.h"

namespace kmldom {

// <SimpleField>
class SimpleFieldTest : public testing::Test {
 protected:
  virtual void SetUp() {
    simplefield_ = KmlFactory::GetFactory()->CreateSimpleField();
  }

  SimpleFieldPtr simplefield_;
};

TEST_F(SimpleFieldTest, TestType) {
  ASSERT_EQ(Type_SimpleField, simplefield_->Type());
  ASSERT_TRUE(simplefield_->IsA(Type_SimpleField));
}

TEST_F(SimpleFieldTest, TestDefaults) {
  ASSERT_EQ(string(""), simplefield_->get_type());
  ASSERT_FALSE(simplefield_->has_type());
  ASSERT_EQ(string(""), simplefield_->get_name());
  ASSERT_FALSE(simplefield_->has_name());
  ASSERT_EQ(string(""), simplefield_->get_displayname());
  ASSERT_FALSE(simplefield_->has_displayname());
}

TEST_F(SimpleFieldTest, TestSetToDefaultValues) {
  simplefield_->set_type(simplefield_->get_type());
  ASSERT_TRUE(simplefield_->has_type());
  simplefield_->set_name(simplefield_->get_name());
  ASSERT_TRUE(simplefield_->has_name());
  simplefield_->set_displayname(simplefield_->get_displayname());
  ASSERT_TRUE(simplefield_->has_displayname());
}

TEST_F(SimpleFieldTest, TestSetGetHasClear) {
  string type("tom");
  simplefield_->set_type(type);
  ASSERT_TRUE(simplefield_->has_type());
  ASSERT_TRUE(type == simplefield_->get_type());
  simplefield_->clear_type();

  string name("tom");
  simplefield_->set_name(name);
  ASSERT_TRUE(simplefield_->has_name());
  ASSERT_TRUE(name == simplefield_->get_name());
  simplefield_->clear_name();

  string displayname("dick");
  simplefield_->set_displayname(displayname);
  ASSERT_TRUE(simplefield_->has_displayname());
  ASSERT_TRUE(displayname == simplefield_->get_displayname());
  simplefield_->clear_displayname();

}

// <Schema>
class SchemaTest : public testing::Test {
 protected:
  virtual void SetUp() {
    schema_ = KmlFactory::GetFactory()->CreateSchema();
  }

  SchemaPtr schema_;
};

TEST_F(SchemaTest, TestType) {
  ASSERT_EQ(Type_Schema, schema_->Type());
  ASSERT_TRUE(schema_->IsA(Type_Schema));
  ASSERT_TRUE(schema_->IsA(Type_Object));
}

TEST_F(SchemaTest, TestDefaults) {
  ASSERT_EQ(string(""), schema_->get_name());
  ASSERT_FALSE(schema_->has_name());
  ASSERT_EQ(string(""), schema_->get_id());
  ASSERT_FALSE(schema_->has_id());
}

TEST_F(SchemaTest, TestSetToDefaultValues) {
  schema_->set_name(schema_->get_name());
  ASSERT_TRUE(schema_->has_name());
  schema_->set_id(schema_->get_id());
  ASSERT_TRUE(schema_->has_id());
}

TEST_F(SchemaTest, TestSetGetHasClear) {
  string name("tom");
  schema_->set_name(name);
  ASSERT_TRUE(schema_->has_name());
  ASSERT_TRUE(name == schema_->get_name());
  schema_->clear_name();

  string id("dick");
  schema_->set_id(id);
  ASSERT_TRUE(schema_->has_id());
  ASSERT_TRUE(id == schema_->get_id());
  schema_->clear_id();

}

TEST_F(SchemaTest, TestLists) {
  // Vector is empty.
  ASSERT_EQ(static_cast<size_t>(0), schema_->get_simplefield_array_size());
  // Add three <SimpleField> elements:
  schema_->add_simplefield(KmlFactory::GetFactory()->CreateSimpleField());
  schema_->add_simplefield(KmlFactory::GetFactory()->CreateSimpleField());
  schema_->add_simplefield(KmlFactory::GetFactory()->CreateSimpleField());
  // We have three items in the array:
  ASSERT_EQ(static_cast<size_t>(3), schema_->get_simplefield_array_size());
  for (size_t i = 0; i < schema_->get_simplefield_array_size(); ++i) {
    ASSERT_EQ(Type_SimpleField, schema_->get_simplefield_array_at(i)->Type());
  }
}

TEST_F(SchemaTest, TestSerialize) {
  schema_->set_id("schema-id");
  schema_->set_name("schema-name");
  schema_->add_simplefield(KmlFactory::GetFactory()->CreateSimpleField());
  schema_->add_simplefield(KmlFactory::GetFactory()->CreateSimpleField());

  string expected(
    "<Schema id=\"schema-id\" name=\"schema-name\">"
    "<SimpleField/>"
    "<SimpleField/>"
    "</Schema>"
  );
  ASSERT_EQ(expected, SerializeRaw(schema_));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
