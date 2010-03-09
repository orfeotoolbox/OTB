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

// This file contains the unit tests for the SimpleData, SchemaData, Datai
// and ExtendedData elements.

#include "kml/dom/extendeddata.h"
#include "kml/dom.h"
#include "gtest/gtest.h"

namespace kmldom {

// <SimpleData>
class SimpleDataTest : public testing::Test {
 protected:
  virtual void SetUp() {
    simpledata_ = KmlFactory::GetFactory()->CreateSimpleData();
  }

  SimpleDataPtr simpledata_;
};

TEST_F(SimpleDataTest, TestType) {
  ASSERT_TRUE(Type_SimpleData == simpledata_->Type());
  ASSERT_TRUE(simpledata_->IsA(Type_SimpleData));
}

TEST_F(SimpleDataTest, TestDefaults) {
  ASSERT_EQ(string(""), simpledata_->get_name());
  ASSERT_FALSE(simpledata_->has_name());
  ASSERT_EQ(string(""), simpledata_->get_text());
  ASSERT_FALSE(simpledata_->has_text());
}

TEST_F(SimpleDataTest, TestSetToDefaultValues) {
  simpledata_->set_name(simpledata_->get_name());
  ASSERT_TRUE(simpledata_->has_name());
  simpledata_->set_text(simpledata_->get_text());
  ASSERT_TRUE(simpledata_->has_text());
}

TEST_F(SimpleDataTest, TestSetGetHasClear) {
  string name("tom");
  simpledata_->set_name(name);
  ASSERT_TRUE(simpledata_->has_name());
  ASSERT_TRUE(name == simpledata_->get_name());
  simpledata_->clear_name();

  string text("dick");
  simpledata_->set_text(text);
  ASSERT_TRUE(simpledata_->has_text());
  ASSERT_TRUE(text == simpledata_->get_text());
  simpledata_->clear_text();

}

TEST_F(SimpleDataTest, TestSerializeCdata) {
  simpledata_->set_text("&");
  ASSERT_EQ(string("<SimpleData><![CDATA[&]]></SimpleData>"),
                       SerializeRaw(simpledata_));

  simpledata_->set_text("a");
  ASSERT_EQ(string("<SimpleData>a</SimpleData>"),
                       SerializeRaw(simpledata_));
}


// <SchemaData>
class SchemaDataTest : public testing::Test {
 protected:
  virtual void SetUp() {
    schemadata_ = KmlFactory::GetFactory()->CreateSchemaData();
  }

  SchemaDataPtr schemadata_;
};

TEST_F(SchemaDataTest, TestType) {
  ASSERT_TRUE(Type_SchemaData == schemadata_->Type());
  ASSERT_TRUE(schemadata_->IsA(Type_SchemaData));
}

TEST_F(SchemaDataTest, TestSchemaUrl) {
  ASSERT_FALSE(schemadata_->has_schemaurl());
  ASSERT_EQ(string(""), schemadata_->get_schemaurl());
  string schemaurl("#icanhasurl");
  schemadata_->set_schemaurl(schemaurl);
  ASSERT_TRUE(schemadata_->has_schemaurl());
  ASSERT_TRUE(schemaurl == schemadata_->get_schemaurl());
  schemadata_->clear_schemaurl();
  ASSERT_FALSE(schemadata_->has_schemaurl());
  ASSERT_EQ(string(""), schemadata_->get_schemaurl());
}

TEST_F(SchemaDataTest, TestLists) {
  // Vector is empty.
  ASSERT_EQ(static_cast<size_t>(0), schemadata_->get_simpledata_array_size());
  // Add three <SimpleData> elements:
  schemadata_->add_simpledata(KmlFactory::GetFactory()->CreateSimpleData());
  schemadata_->add_simpledata(KmlFactory::GetFactory()->CreateSimpleData());
  schemadata_->add_simpledata(KmlFactory::GetFactory()->CreateSimpleData());
  // We have three items in the array:
  ASSERT_EQ(static_cast<size_t>(3), schemadata_->get_simpledata_array_size());
  for (size_t i = 0; i < schemadata_->get_simpledata_array_size(); ++i) {
    ASSERT_EQ(Type_SimpleData,schemadata_->get_simpledata_array_at(i)->Type());
  }
}

// <Data>
class DataTest : public testing::Test {
 protected:
  virtual void SetUp() {
    data_ = KmlFactory::GetFactory()->CreateData();
  }

  DataPtr data_;
};

TEST_F(DataTest, TestType) {
  ASSERT_TRUE(Type_Data == data_->Type());
  ASSERT_TRUE(data_->IsA(Type_Data));
}

TEST_F(DataTest, TestDefaults) {
  ASSERT_FALSE(data_->has_name());
  ASSERT_EQ(string(""), data_->get_name());
  ASSERT_FALSE(data_->has_displayname());
  ASSERT_EQ(string(""), data_->get_displayname());
  ASSERT_FALSE(data_->has_value());
  ASSERT_EQ(string(""), data_->get_value());
}

TEST_F(DataTest, TestSetToDefaultValues) {
  data_->set_name(data_->get_name());
  ASSERT_TRUE(data_->has_name());
  data_->set_displayname(data_->get_displayname());
  ASSERT_TRUE(data_->has_displayname());
  data_->set_value(data_->get_value());
  ASSERT_TRUE(data_->has_value());
}

TEST_F(DataTest, TestSetGetHasClear) {
  string name("tom");
  data_->set_name(name);
  ASSERT_TRUE(data_->has_name());
  ASSERT_TRUE(name == data_->get_name());
  data_->clear_name();

  string displayname("dick");
  data_->set_displayname(displayname);
  ASSERT_TRUE(data_->has_displayname());
  ASSERT_TRUE(displayname == data_->get_displayname());
  data_->clear_displayname();

  string value("harry");
  data_->set_value(value);
  ASSERT_TRUE(data_->has_value());
  ASSERT_TRUE(value == data_->get_value());
  data_->clear_value();

}

// <ExtendedData>
class ExtendedDataTest : public testing::Test {
 protected:
  virtual void SetUp() {
    extendeddata_ = KmlFactory::GetFactory()->CreateExtendedData();
  }

  ExtendedDataPtr extendeddata_;
};

TEST_F(ExtendedDataTest, TestType) {
  ASSERT_TRUE(Type_ExtendedData == extendeddata_->Type());
  ASSERT_TRUE(extendeddata_->IsA(Type_ExtendedData));
}

TEST_F(ExtendedDataTest, TestLists) {
  // Vectors are empty.
  ASSERT_EQ(static_cast<size_t>(0), extendeddata_->get_data_array_size());
  ASSERT_EQ(static_cast<size_t>(0), extendeddata_->get_schemadata_array_size());
  // Add three <Data> and three <SchemaData> elements:
  extendeddata_->add_data(
      KmlFactory::GetFactory()->CreateData());
  extendeddata_->add_data(
      KmlFactory::GetFactory()->CreateData());
  extendeddata_->add_schemadata(
      KmlFactory::GetFactory()->CreateSchemaData());
  extendeddata_->add_schemadata(
      KmlFactory::GetFactory()->CreateSchemaData());
  extendeddata_->add_data(
      KmlFactory::GetFactory()->CreateData());
  extendeddata_->add_schemadata(
      KmlFactory::GetFactory()->CreateSchemaData());
  // We have six items in the array:
  ASSERT_EQ(static_cast<size_t>(3), extendeddata_->get_data_array_size());
  ASSERT_EQ(static_cast<size_t>(3), extendeddata_->get_schemadata_array_size());
  // Assert elements can be read from vector.
  ASSERT_EQ(Type_Data, extendeddata_->get_data_array_at(0)->Type());
  ASSERT_EQ(Type_SchemaData,
                       extendeddata_->get_schemadata_array_at(0)->Type());
}

TEST_F(ExtendedDataTest, TestParse) {
  const string schemaurl("http://example.com/index.kml#schemaId");
  const string d_name("myCoolDataName");
  const string displayname("my cool displayName");
  const string value("my cool value");
  const string sd_name("myCoolSimpleDataName");
  const string chardata("some char data");
  const string kml =
    "<ExtendedData>"
    "<Data name=\"" + d_name + "\">"
    "<displayName>" + displayname + "</displayName>"
    "<value>" + value + "</value>"
    "</Data>"
    "<SchemaData schemaUrl=\"" + schemaurl + "\">"
    "<SimpleData name=\"" + sd_name + "\">" + chardata + "</SimpleData>"
    "</SchemaData>"
    "</ExtendedData>";
  string errors;
  ElementPtr root = Parse(kml, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const ExtendedDataPtr extendeddata = AsExtendedData(root);
  ASSERT_TRUE(extendeddata);
  ASSERT_EQ(static_cast<size_t>(1),
                       extendeddata->get_data_array_size());
  ASSERT_EQ(static_cast<size_t>(1),
                       extendeddata->get_schemadata_array_size());
  const DataPtr data = AsData(extendeddata->get_data_array_at(0));
  ASSERT_TRUE(data);
  ASSERT_TRUE(data->has_name());
  ASSERT_EQ(d_name, data->get_name());
  ASSERT_TRUE(data->has_displayname());
  ASSERT_EQ(displayname, data->get_displayname());
  ASSERT_TRUE(data->has_value());
  ASSERT_EQ(value, data->get_value());
  const SchemaDataPtr schemadata = AsSchemaData(
      extendeddata->get_schemadata_array_at(0));
  ASSERT_TRUE(schemadata);
  ASSERT_TRUE(schemadata->has_schemaurl());
  ASSERT_EQ(schemaurl, schemadata->get_schemaurl());
  ASSERT_EQ(static_cast<size_t>(1), schemadata->get_simpledata_array_size());
  const SimpleDataPtr simpledata = AsSimpleData(
      schemadata->get_simpledata_array_at(0));
  ASSERT_TRUE(simpledata);
  ASSERT_TRUE(simpledata->has_name());
  ASSERT_TRUE(simpledata->has_text());
  ASSERT_EQ(sd_name, simpledata->get_name());
  ASSERT_EQ(chardata, simpledata->get_text());
}

// <Metadata>
class MetadataTest : public testing::Test {
 protected:
  virtual void SetUp() {
    metadata_ = KmlFactory::GetFactory()->CreateMetadata();
  }

  MetadataPtr metadata_;
};

TEST_F(MetadataTest, TestType) {
  ASSERT_TRUE(Type_Metadata == metadata_->Type());
  ASSERT_TRUE(metadata_->IsA(Type_Metadata));
}

TEST_F(MetadataTest, TestParseSerialize) {
  const string kMetadata(
    "<Metadata>"
    "<extra><special>stuff</special></extra>\n"  // TODO: remove this newline...
    "</Metadata>");

  metadata_ = AsMetadata(Parse(kMetadata, NULL));
  ASSERT_TRUE(metadata_);
  ASSERT_EQ(kMetadata, SerializeRaw(metadata_));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
