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

// This file contains the unit tests for the XmlSerializer class and
// the SerializePretty and SerializeRaw public API functions.

#include "kml/dom/xml_serializer.h"
#include <string>
#include "kml/dom/attributes.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kmldom.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class XmlSerializerTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(XmlSerializerTest);
  CPPUNIT_TEST(TestToString);
  CPPUNIT_TEST(TestWriteString);
  CPPUNIT_TEST(TestSaveStringFieldById);
  CPPUNIT_TEST(TestCdataHandling);
  CPPUNIT_TEST(TestCdataEscaping);
  CPPUNIT_TEST(TestSaveBoolFieldByIdAsBool);
  CPPUNIT_TEST(TestSaveBoolFieldByIdAsInt);
  CPPUNIT_TEST(TestSaveContent);
  CPPUNIT_TEST(TestPrecision);
  CPPUNIT_TEST(TestSerializePretty);
  CPPUNIT_TEST(TestSerializePrettyNil);
  CPPUNIT_TEST(TestSerializePrettyNilWithAttrs);
  CPPUNIT_TEST(TestSerializeRaw);
  CPPUNIT_TEST(TestSerializeRawNil);
  CPPUNIT_TEST(TestSerializeRawNilWithAttrs);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestToString();
  void TestWriteString();
  void TestSaveStringFieldById();
  void TestCdataHandling();
  void TestCdataEscaping();
  void TestSaveBoolFieldByIdAsBool();
  void TestSaveBoolFieldByIdAsInt();
  void TestSaveContent();
  void TestPrecision();
  void TestSerializePretty();
  void TestSerializePrettyNil();
  void TestSerializePrettyNilWithAttrs();
  void TestSerializeRaw();
  void TestSerializeRawNil();
  void TestSerializeRawNilWithAttrs();

 public:
  // Called before each test.
  void setUp() {
    xml_serializer_ = new XmlSerializer("","");
    placemark_ = KmlFactory::GetFactory()->CreatePlacemark();
  }

  // Called after each test.
  void tearDown() {
    delete xml_serializer_;
    // PlacemarkPtr's destructor releases the underlying Placemark storage.
  }

 private:
  XmlSerializer* xml_serializer_;
  PlacemarkPtr placemark_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(XmlSerializerTest);

void XmlSerializerTest::TestToString() {
  double pi = 3.14159;
  unsigned int dna = 42;
  CPPUNIT_ASSERT("3.14159" == ToString(pi));
  CPPUNIT_ASSERT("42" == ToString(dna));
}

void XmlSerializerTest::TestWriteString() {
  // Write string clears ptr before writing, does not append.
  std::string output("foo");
  xml_serializer_->WriteString(&output);
  const std::string expected_result("");
  CPPUNIT_ASSERT_EQUAL(expected_result, output);
}

void XmlSerializerTest::TestSaveStringFieldById() {
  // Assert that the <name> field serializes as expected.
  const int type_id = Type_name;
  const std::string txt("some feature name");
  const std::string expected_result("<name>some feature name</name>");
  xml_serializer_->SaveFieldById(type_id, txt);
  std::string output;
  xml_serializer_->WriteString(&output);
  CPPUNIT_ASSERT_EQUAL(expected_result, output);
}

void XmlSerializerTest::TestCdataHandling() {
  // If the parser sees <![CDATA ... ]]> around the character data of an
  // element, it will preserve it. MaybeQuoteString() contains logic to
  // determine if we should wrap XML-invalid field characters.
  struct TestStruct {
    const std::string chardata;
    const std::string expected;
  } testdata[] = {
    {"simple text", "<name>simple text</name>\n"},
    {"<![CDATA[...]]>", "<name><![CDATA[...]]></name>\n"},
    {"invalid & char", "<name><![CDATA[invalid & char]]></name>\n"},
    {"invalid ' char", "<name><![CDATA[invalid ' char]]></name>\n"},
    {"invalid < char", "<name><![CDATA[invalid < char]]></name>\n"},
    {"invalid > char", "<name><![CDATA[invalid > char]]></name>\n"},
    {"invalid \" char", "<name><![CDATA[invalid \" char]]></name>\n"}
  };

  const size_t size = sizeof(testdata) / sizeof(testdata[0]);

  for (size_t i = 0; i < size; ++i) {
    XmlSerializer s_("\n","");
    std::string output;
    s_.SaveFieldById(Type_name, testdata[i].chardata);
    s_.WriteString(&output);
    CPPUNIT_ASSERT_EQUAL(testdata[i].expected, output);
  }
}

void XmlSerializerTest::TestCdataEscaping() {
  // Assert that data that should be escaped in a CDATA is so quoted.
  placemark_->set_name("<i>One</i> two");
  std::string xml = SerializePretty(placemark_);
  std::string expected("<Placemark>\n  "
                       "<name><![CDATA[<i>One</i> two]]></name>\n"
                       "</Placemark>\n");
  CPPUNIT_ASSERT_EQUAL(expected, xml);
}

void XmlSerializerTest::TestSaveBoolFieldByIdAsBool() {
  // Assert that <open> is serialized correctly.
  const bool bool_state = true;
  std::string expected_result("<open>1</open>");
  std::string output;
  // A parsed bool is serialized as an int:
  xml_serializer_->SaveFieldById(Type_open, bool_state);
  xml_serializer_->WriteString(&output);
  CPPUNIT_ASSERT_EQUAL(expected_result, output);
}

void XmlSerializerTest::TestSaveBoolFieldByIdAsInt() {
  // Assert that <open> is serialized correctly.
  const unsigned int int_state = 1;
  std::string expected_result("<open>1</open>");
  std::string output;
  // A parsed int is serialized as an int:
  xml_serializer_->SaveFieldById(Type_open, int_state);
  xml_serializer_->WriteString(&output);
  CPPUNIT_ASSERT_EQUAL(expected_result, output);
}

void XmlSerializerTest::TestSaveContent() {
  // Ensure a simple string is serialized exactly.
  const std::string s("tom, dick");
  xml_serializer_->SaveContent(s, false);
  std::string output;
  xml_serializer_->WriteString(&output);
  CPPUNIT_ASSERT_EQUAL(s, output);
  // SaveContent will append continued calls.
  std::string t(" and harry");
  xml_serializer_->SaveContent(t, false);
  std::string expected_result(s + t);
  xml_serializer_->WriteString(&output);
  CPPUNIT_ASSERT_EQUAL(expected_result, output);
}

void XmlSerializerTest::TestPrecision() {
  double a = 1.0;
  // Will round down to int:
  std::string expected = "1";
  CPPUNIT_ASSERT_EQUAL(expected, ToString(a));
  double b = 1.1;
  // Will preserve at current level of precision:
  expected = "1.1";
  CPPUNIT_ASSERT_EQUAL(expected, ToString(b));
  double c = 1.2345678901234567890;
  // Will round down to 15 decimals of precision:
  expected = "1.23456789012346";
  CPPUNIT_ASSERT_EQUAL(expected, ToString(c));
}

// Tests the internal Indent() method.
void XmlSerializerTest::TestSerializePretty() {
  placemark_->set_name("hello");
  std::string xml = SerializePretty(placemark_);
  std::string expected("<Placemark>\n  <name>hello</name>\n</Placemark>\n");
  CPPUNIT_ASSERT_EQUAL(expected, xml);
}

// This tests the pretty serialization of an element with no content.
void XmlSerializerTest::TestSerializePrettyNil() {
  CPPUNIT_ASSERT_EQUAL(std::string("<Placemark/>\n"),
                       SerializePretty(placemark_));
}

// This tests the pretty serialization of an element with attributes but
// no content.
void XmlSerializerTest::TestSerializePrettyNilWithAttrs() {
  placemark_->set_id("hi");  // Adds the id= attribute.
  CPPUNIT_ASSERT_EQUAL(std::string("<Placemark id=\"hi\"/>\n"),
                       SerializePretty(placemark_));
}

// This tests the raw serialization of an element a child element.
void XmlSerializerTest::TestSerializeRaw() {
  placemark_->set_name("hello");
  std::string xml = SerializeRaw(placemark_);
  std::string expected("<Placemark><name>hello</name></Placemark>");
  CPPUNIT_ASSERT_EQUAL(expected, xml);
}

// This tests the raw serialization of an element with no content.
void XmlSerializerTest::TestSerializeRawNil() {
  CPPUNIT_ASSERT_EQUAL(std::string("<Placemark/>"),
                       SerializeRaw(placemark_));
}

// This tests the raw serialization of an element with attributes but
// no content.
void XmlSerializerTest::TestSerializeRawNilWithAttrs() {
  placemark_->set_id("hi");  // Adds the id= attribute.
  CPPUNIT_ASSERT_EQUAL(std::string("<Placemark id=\"hi\"/>"),
                       SerializeRaw(placemark_));
}

}  // end namespace kmldom

TEST_MAIN
