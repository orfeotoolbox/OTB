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

// This file contains the unit tests for the OldSchemaParserObserver class.
// TODO: The OldSchemaParserObserver class is still under development.

#include "kml/engine/old_schema_parser_observer.h"
#include "boost/scoped_ptr.hpp"
#include "kml/dom/kml_factory.h"
#include "gtest/gtest.h"

namespace kmlengine {

class OldSchemaParserObserverTest : public testing::Test {
 protected:
  virtual void SetUp() {
    // Make some elements used in most tests.
    kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
    document_ = factory->CreateDocument();
    kSchema0Name_ = "schema0";
    schema0_ = factory->CreateSchema();
    schema0_->set_name(kSchema0Name_);
    kSchema1Name_ = "schema1";
    schema1_ = factory->CreateSchema();
    schema1_->set_name(kSchema1Name_);
    schema_no_name_ = factory->CreateSchema();
    schema_parser_observer_.reset(
        new OldSchemaParserObserver(schema_name_map_));
  }

  kmldom::DocumentPtr document_;
  string kSchema0Name_;
  kmldom::SchemaPtr schema0_;
  string kSchema1Name_;
  kmldom::SchemaPtr schema1_;
  kmldom::SchemaPtr schema_no_name_;
  SchemaNameMap schema_name_map_;
  boost::scoped_ptr<OldSchemaParserObserver> schema_parser_observer_;
};

#if 0
// Verify that AddChild() usage with <Schema> as child of <Document>.
TEST_F(OldSchemaParserObserverTest, TestAddChildSchema) {
  // Verify that AddChild() does not detect a dupe.
  ASSERT_TRUE(schema_parser_observer_->AddChild(document_, schema0_));
  // Verify that there is only one entry in the map.
  ASSERT_EQ(static_cast<size_t>(1), schema_name_map_.size());

  // Verify that the name maps to the Schema.
  kmldom::SchemaPtr object = schema_name_map_[kSchema0Name_];
  ASSERT_EQ(kSchema0Name_, object->get_name());
  ASSERT_EQ(kmldom::Type_Schema, object->Type());

  // Verify that AddChild() does not detect a dupe.
  ASSERT_TRUE(schema_parser_observer_->AddChild(document_, schema1_));
  // Verify that there are now 2 entries in the map.
  ASSERT_EQ(static_cast<size_t>(2), schema_name_map_.size());
  // Verify that the id maps to the SchemaMap
  object = schema_name_map_[kSchema0Name_];
  ASSERT_EQ(kSchema0Name_, object->get_name());
  ASSERT_EQ(kmldom::Type_Schema, object->Type());
}

// Verify that the destructor does not effect the map.
TEST_F(OldSchemaParserObserverTest, TestDestructor) {
  // Use AddChild() to put some entries in the map.
  ASSERT_TRUE(schema_parser_observer_->AddChild(document_, schema0_));
  ASSERT_TRUE(schema_parser_observer_->AddChild(document_, schema1_));

  // Verify that deleting the observer does not affect the map.
  schema_parser_observer_.reset();

  // Verify that the object map has exactly the 2 expected mappings.
  ASSERT_EQ(static_cast<size_t>(2), schema_name_map_.size());
  ASSERT_EQ(kSchema0Name_, schema_name_map_[kSchema0Name_]->get_name());
  ASSERT_EQ(kSchema1Name_, schema_name_map_[kSchema1Name_]->get_name());
}
#endif

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
