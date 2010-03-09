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
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/xsd.h"
#include "gtest/gtest.h"

namespace kmldom {

// Verify that parsing bad data is well behaved: no crash, NULL return,
// error message.
TEST(UnknownTest, TestNotXml) {
  string errors;
  ElementPtr root = Parse("this is not even xml", &errors);
  ASSERT_TRUE(NULL == root);
  ASSERT_FALSE(errors.empty());
}

// Verify that a fully unknown element round-trips fine.
TEST(UnknownTest, TestUnknownElement) {
  string errors;
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
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  // The root element is a Placemark.
  const PlacemarkPtr placemark = AsPlacemark(root);
  // Known children are in XSD order.  All content within the unknown element
  // is preserved including child elements, character data, and attributes.
  // The relative order of unknown elements is preserved and all appear
  // after the known and valid elements and after the misplaced elements.
  string expected =
    "<Placemark>"
      "<name>placemark</name>"
      "<visibility>0</visibility>"
      "<GroundOverlay><name>go</name></GroundOverlay>"
      "<unknown nosuch=\"attribute\">\n"
      "\tchar data<child>stuff</child>\n"
      "</unknown>\n"
      "<another>who am I</another>\n"
    "</Placemark>";
  ASSERT_EQ(expected, SerializeRaw(placemark));
}

// Verify that a misplaced element round-trips fine.
TEST(UnknownTest, TestMisplaced) {
  string errors;
  // <Folder> is a known KML element, but not a valid child of <Placemark>.
  ElementPtr root = Parse(
    "<Placemark>"
      "<Folder><name>folder</name></Folder>"
      "<name>placemark</name>"
    "</Placemark>",
    &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const PlacemarkPtr placemark = AsPlacemark(root);
  ASSERT_EQ(string("placemark"), placemark->get_name());
  // TODO: add test to find Folder via the unknown element api

  // Serialized output puts unknown/misplaced elements after known elements.
  string expected =
    "<Placemark>"
      "<name>placemark</name>"
      "<Folder><name>folder</name></Folder>"
    "</Placemark>";
  ASSERT_EQ(expected, SerializeRaw(placemark));
}

// Verify that unknown attributes on known elements round trip fine.
TEST(UnknownTest, TestUnknownAttribute) {
  string errors;
  ElementPtr root = Parse(
    "<GroundOverlay unknown=\"who knows\" abc=\"zzz\" >"
      "<name>groundoverlay</name>"
    "</GroundOverlay>",
    &errors);
  // This is XML valid so it parses fine.
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  // The root is a GroundOverlay.
  const GroundOverlayPtr groundoverlay = AsGroundOverlay(root);
  // Unknown attributes don't interefere with known children.
  ASSERT_EQ(string("groundoverlay"), groundoverlay->get_name());
  // Serializer perserves unknown attributes, but not their order.
  string expected =
    "<GroundOverlay abc=\"zzz\" unknown=\"who knows\">"
      "<name>groundoverlay</name>"
    "</GroundOverlay>";
  ASSERT_EQ(expected, SerializeRaw(groundoverlay));
}

// Every complex element preserves unknown children.
TEST(UnknownTest, TestSaveUnknown) {
  Xsd* xsd_ = Xsd::GetSchema();
  int complex_count = 0;
  int element_type_id = static_cast<int>(Type_Unknown) + 1;
  const int end_id = static_cast<int>(Type_Invalid);
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  // This presumes "<unknown>" is a fully unknown element within libkml.
  const string kUnknownSimple("<unknown>unknown content</unknown>\n");
  // This presumes "<Unknown>" is a fully unknown element within libkml.
  const string kUnknownComplex("<Unknown>a<b><c>d</c>z</b></Unknown>\n");
  // This presumse "<kml>" is never the child of any other element.
  for (; element_type_id != end_id; ++element_type_id) {
    // Only complex elements return non-NULL.
    if (ElementPtr element = kml_factory->CreateElementById(
        static_cast<KmlDomType>(element_type_id))) {
      // Except these which aren't really complex.
      if (element->Type() == Type_coordinates ||
          element->Type() == Type_Snippet ||
          element->Type() == Type_linkSnippet) {
        continue;
      }
      const string kTagName(xsd_->ElementName(element_type_id));
      element = ParseKml(string("<") + kTagName + ">" +
                         kUnknownSimple + "<kml/>" +
                         kUnknownComplex + "<kml/>" +
                         "</" + kTagName + ">");
      ASSERT_TRUE(element) << kTagName;
      ASSERT_EQ(static_cast<size_t>(2),
                element->get_unknown_elements_array_size());
      ASSERT_EQ(kUnknownSimple, element->get_unknown_elements_array_at(0));
      ASSERT_EQ(kUnknownComplex, element->get_unknown_elements_array_at(1));
      ASSERT_EQ(static_cast<size_t>(2),
                element->get_misplaced_elements_array_size()) << kTagName;
      ASSERT_EQ(Type_kml, element->get_misplaced_elements_array_at(0)->Type());
      ASSERT_EQ(Type_kml, element->get_misplaced_elements_array_at(1)->Type());
      ++complex_count;
    }
  }
  ASSERT_EQ(85, complex_count);  // Yes, must exactly match kml22.h
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
