// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,//     this list of conditions and the following disclaimer in the documentation//     and/or other materials provided with the distribution.
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

// This file contains the unit tests for the ParseOldSchema function.

#include "kml/engine/parse_old_schema.h"
#include "kml/dom/kml_factory.h"
#include "kml/engine/engine_types.h"
#include "gtest/gtest.h"

using kmldom::PlacemarkPtr;
using kmldom::KmlFactory;
using kmldom::SchemaPtr;

namespace kmlengine {

class ParseOldSchemaTest : public testing::Test {
 protected:
  void InitSchemaNameMap();
  SchemaNameMap schema_name_map_;
};

void ParseOldSchemaTest::InitSchemaNameMap() {
  const string kSchemaName("S_park_boundaries_S");
  SchemaPtr schema = KmlFactory::GetFactory()->CreateSchema();
  schema->set_name(kSchemaName);
  schema_name_map_[kSchemaName] = schema;
}

// Test ConvertOldSchema().
TEST_F(ParseOldSchemaTest, TestConvertOldSchema) {
  InitSchemaNameMap();
  const string kXml(
      "<S_park_boundaries_S>"
      "<name>Yosemite</name>"
      "<Polygon/>"
      "</S_park_boundaries_S>");
  string output_xml;
  ASSERT_TRUE(ConvertOldSchema(kXml, schema_name_map_, &output_xml));
  const string kExpected(
      "<Placemark>"
      "<name>Yosemite</name>"
      "<Polygon/>"
      "</Placemark>");
  ASSERT_EQ(kExpected, output_xml);
}

// Test ParseOldSchema().
TEST_F(ParseOldSchemaTest, TestParseOldSchema) {
  InitSchemaNameMap();
  const string kName("Arches NP");
  const string kXml(
      "<S_park_boundaries_S>"
      "<name>" + kName + "</name>"
      "<Polygon/>"
      "</S_park_boundaries_S>");
  PlacemarkPtr placemark = ParseOldSchema(kXml, schema_name_map_, NULL);
  ASSERT_TRUE(placemark);
  ASSERT_TRUE(placemark->has_name());
  ASSERT_EQ(kName, placemark->get_name());
  ASSERT_TRUE(placemark->has_geometry());
  ASSERT_EQ(kmldom::Type_Polygon, placemark->get_geometry()->Type());
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
