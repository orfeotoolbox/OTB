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

// This file contains unit tests for unknown element handling.

#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_ptr.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class UnknownTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(UnknownTest);
  CPPUNIT_TEST(TestNotXml);
  CPPUNIT_TEST(TestUnknownElement);
  CPPUNIT_TEST(TestMisplaced);
  CPPUNIT_TEST(TestUnknownAttribute);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestNotXml();
  void TestUnknownElement();
  void TestMisplaced();
  void TestUnknownAttribute();
};

CPPUNIT_TEST_SUITE_REGISTRATION(UnknownTest);

// Verify that parsing bad data is well behaved: no crash, NULL return,
// error message.
void UnknownTest::TestNotXml() {
  std::string errors;
  ElementPtr root = Parse("this is not even xml", &errors);
  CPPUNIT_ASSERT(NULL == root);
  CPPUNIT_ASSERT(false == errors.empty());
}

// Verify that a fully unknown element round-trips fine.
void UnknownTest::TestUnknownElement() {
  std::string errors;
  // <unknown> is not known to be KML element, but its content is preserved.
  ElementPtr root = Parse(
    "<Placemark>"
      "<unknown nosuch=\"attribute\">\n"
      "\tchar data<child>stuff</child>\n"
      "</unknown>"
      "<name>placemark</name>"
      "<another>who am I</another>"
      "<visibility>0</visibility>"
      "<GroundOverlay><name>go</name></GroundOverlay>"
    "</Placemark>",
    &errors);
  // This is XML valid and hence parses fine.
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  // The root element is a Placemark.
  const PlacemarkPtr placemark = AsPlacemark(root);
  // Known children are in XSD order.  All content within the unknown element
  // is preserved including child elements, character data, and attributes.
  // The relative order of unknown elements is preserved and all appear
  // after the known and valid elements and after the misplaced elements.
  std::string expected =
    "<Placemark>"
      "<name>placemark</name>"
      "<visibility>0</visibility>"
      "<GroundOverlay><name>go</name></GroundOverlay>"
      "<unknown nosuch=\"attribute\">\n"
      "\tchar data<child>stuff</child>\n"
      "</unknown>\n"
      "<another>who am I</another>\n"
    "</Placemark>";
  CPPUNIT_ASSERT_EQUAL(expected, SerializeRaw(placemark));
}

// Verify that a misplaced element round-trips fine.
void UnknownTest::TestMisplaced() {
  std::string errors;
  // <Folder> is a known KML element, but not a valid child of <Placemark>.
  ElementPtr root = Parse(
    "<Placemark>"
      "<Folder><name>folder</name></Folder>"
      "<name>placemark</name>"
    "</Placemark>",
    &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const PlacemarkPtr placemark = AsPlacemark(root);
  CPPUNIT_ASSERT_EQUAL(std::string("placemark"), placemark->get_name());
  // TODO: add test to find Folder via the unknown element api

  // Serialized output puts unknown/misplaced elements after known elements.
  std::string expected =
    "<Placemark>"
      "<name>placemark</name>"
      "<Folder><name>folder</name></Folder>"
    "</Placemark>";
  CPPUNIT_ASSERT_EQUAL(expected, SerializeRaw(placemark));
}

// Verify that unknown attributes on known elements round trip fine.
void UnknownTest::TestUnknownAttribute() {
  std::string errors;
  ElementPtr root = Parse(
    "<GroundOverlay unknown=\"who knows\" abc=\"zzz\" >"
      "<name>groundoverlay</name>"
    "</GroundOverlay>",
    &errors);
  // This is XML valid so it parses fine.
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  // The root is a GroundOverlay.
  const GroundOverlayPtr groundoverlay = AsGroundOverlay(root);
  // Unknown attributes don't interefere with known children.
  CPPUNIT_ASSERT_EQUAL(std::string("groundoverlay"), groundoverlay->get_name());
  // Serializer perserves unknown attributes, but not their order.
  std::string expected =
    "<GroundOverlay abc=\"zzz\" unknown=\"who knows\">"
      "<name>groundoverlay</name>"
    "</GroundOverlay>";
  CPPUNIT_ASSERT_EQUAL(expected, SerializeRaw(groundoverlay));
}

}  // end namespace kmldom

TEST_MAIN
