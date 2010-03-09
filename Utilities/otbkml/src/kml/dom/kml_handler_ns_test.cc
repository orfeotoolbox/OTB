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

// This file contains the unit tests for the KmlHandlerNS class.

#include "kml/dom/kml_handler_ns.h"
#include <stdlib.h>  // For calloc() and free().
#include "kml/dom/element.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/placemark.h"
#include "kml/dom/parser.h"
#include "kml/dom/parser_observer.h"
#include "gtest/gtest.h"

namespace kmldom {

// This class is the unit test fixture for the KmlHandlerNS class.
class KmlHandlerNSTest : public testing::Test {
 protected:
  virtual void SetUp() {
    kml_handler_ns_ = new KmlHandlerNS(observers_);
  }

  virtual void TearDown() {
    delete kml_handler_ns_;
  }

  kmlbase::StringVector atts_;
  parser_observer_vector_t observers_;
  KmlHandlerNS* kml_handler_ns_;
};

// This verifies the initial state of a freshly constructed KmlHandlerNS.
TEST_F(KmlHandlerNSTest, TestInitialState) {
  // No elements have been processed, but the PopRoot() method should
  // be well behaved.
  ASSERT_TRUE(NULL == kml_handler_ns_->PopRoot());
}

TEST_F(KmlHandlerNSTest, TestStartEndNamespace) {
  const string kDefaultPrefix("");
  const string kKmlXmlns("http://www.opegis.net/kml/2.2");
  const string kAtomPrefix("atom");
  const string kAtomXmlns("http://www.w3.org/2005/Atom");
  const string kGxPrefix("gx");
  const string kGxtXmlns("http://earth.google.com/kml/2.2/ext");
  // Given this KML:
  // <kml xmlns="http://www.opengis.net/kml/2.2"
  //      xmlns:atom="http://www.w3.org/2005/Atom"
  //      xmlns:gx="http://earth.google.com/kml/2.2/ext">
  // StartNamespace will be invoked as follows:

  // A NULL prefix means a default namespace begins at this scope.
//  kml_handler_ns_->StartNamespace(NULL, kKmlXmlns);
  // The xmlns:atom declaration is passed as follows:
  kml_handler_ns_->StartNamespace(kAtomPrefix, kAtomXmlns);
  // The xmlns:gx declaration is passed as follows:
  kml_handler_ns_->StartNamespace(kGxPrefix, kGxtXmlns);

  // TODO: Verify that the namespace-prefix map has been populated correctly.

  // The namespace declarations will be unwound in EndNamespace as follows:
  kml_handler_ns_->EndNamespace("atom");
  kml_handler_ns_->EndNamespace("gx");
//  kml_handler_ns_->EndNamespace(NULL);
}

// This is a test of the StartElement() method for a known simple element.
TEST_F(KmlHandlerNSTest, TestStartSimpleElement) {
  // This is what expat sends to StartElement() on "<kml:name>".
  kml_handler_ns_->StartElement("http://www.opengis.net/kml/2.2|name", atts_);

  // Since "kml:name" is known we will find it as the root element.
  ElementPtr root = kml_handler_ns_->PopRoot();
  ASSERT_TRUE(root->Type() == Type_name);

  // PopRoot() is destructive so now there is nothing.
  ASSERT_TRUE(NULL == kml_handler_ns_->PopRoot());
}

// This is a test of the EndElement() method for a known simple element.
TEST_F(KmlHandlerNSTest, TestEndSimpleElement) {
  // This is what expat sends to KmlHandlerNS on "<kml:name/>":
  kml_handler_ns_->StartElement("http://www.opengis.net/kml/2.2|name", atts_);
  kml_handler_ns_->EndElement("http://www.opengis.net/kml/2.2|name");

  ElementPtr root = kml_handler_ns_->PopRoot();
  ASSERT_TRUE(root->Type() == Type_name);

  // PopRoot() is destructive so now there is nothing.
  ASSERT_TRUE(NULL == kml_handler_ns_->PopRoot());
}

// This is a test of the CharData() method for a known simple element.
TEST_F(KmlHandlerNSTest, TestBasicCharData) {
  // This is what expat sends to KmlHandlerNS on "<name>what is in a</name>":
  const char* kTagName = "http://www.opengis.net/kml/2.2|name";
  const char* kContent = "what is in a name";

  kml_handler_ns_->StartElement(kTagName, atts_);
  kml_handler_ns_->CharData(kContent);
  kml_handler_ns_->EndElement(kTagName);

  ElementPtr root = kml_handler_ns_->PopRoot();
  ASSERT_EQ(root->Type(), Type_name);
  ASSERT_TRUE(NULL == kml_handler_ns_->PopRoot());
  ASSERT_EQ(kContent, root->get_char_data());
}

// This is a test of the StartElement() method for a known complex element.
TEST_F(KmlHandlerNSTest, TestStartComplexElement) {
  kml_handler_ns_->StartElement("http://www.opengis.net/kml/2.2|Placemark",
                                atts_);
  ElementPtr root = kml_handler_ns_->PopRoot();
  ASSERT_TRUE(root->Type() == Type_Placemark);
  ASSERT_TRUE(NULL == kml_handler_ns_->PopRoot());
}

// This is a test of the EndElement() method for a known complex element.
TEST_F(KmlHandlerNSTest, TestEndComplexElement) {
  // This is what expat does for "<Placemark/>".
  kml_handler_ns_->StartElement("http://www.opengis.net/kml/2.2|Placemark",
                                atts_);
  kml_handler_ns_->EndElement("http://www.opengis.net/kml/2.2|Placemark");
  ElementPtr root = kml_handler_ns_->PopRoot();
  ASSERT_TRUE(root->Type() == Type_Placemark);
  ASSERT_TRUE(NULL == kml_handler_ns_->PopRoot());
}

// This is a test of StartElement() for a known complex element with known
// attributes.
TEST_F(KmlHandlerNSTest, TestStartComplexElementWithAtts) {
  const char* kAttrName = "id";
  const char* kAttrVal = "foo";
  atts_.push_back(kAttrName);
  atts_.push_back(kAttrVal);
  kml_handler_ns_->StartElement("http://www.opengis.net/kml/2.2|Placemark",
                                atts_);
  ElementPtr root = kml_handler_ns_->PopRoot();
  ASSERT_EQ(root->Type(), Type_Placemark);
  ASSERT_TRUE(NULL == kml_handler_ns_->PopRoot());
  PlacemarkPtr placemark = AsPlacemark(root);
  ASSERT_TRUE(kAttrVal == placemark->get_id());
}

// A basic test of namespace-aware parsing.
TEST_F(KmlHandlerNSTest, TestNamespaceParsing) {
  const string kNamespaceKml =
    "<kml xmlns=\"http://www.opengis.net/kml/2.2\""
    "     xmlns:atom=\"http://www.w3.org/2005/Atom\""
    "     xmlns:gx=\"http://earth.google.com/kml/2.2/ext\">"
    "<Folder>"
    " <name>a KML folder</name>"
    " <atom:author><atom:name>an atom author name</atom:name></atom:author>"
    " <gx:Tour><name>an extension tour name</name></gx:Tour>"
    "</Folder>"
    "</kml>";

  Parser parser;
  string errors;
  ElementPtr root = parser.ParseNS(kNamespaceKml, &errors);
  ASSERT_TRUE(errors.empty());
  ASSERT_TRUE(root);

  // TODO: ultimately the parse is preserved 1:1. Currently the parse will
  // drop the xmlns attrs and the element prefixes so this is a test of an
  // incomplete implementation.
  const string kExpectedSerializedKml =
    "<kml>\n"
    "  <Folder>\n"
    "    <name>a KML folder</name>\n"
    "    <author><name>an atom author name</name></author>\n"
    "    <Tour><name>an extension tour name</name></Tour>\n"
    "  </Folder>\n"
    "</kml>\n";
  const string kSerializedKml = SerializePretty(root);
  ASSERT_EQ(kExpectedSerializedKml, SerializePretty(root));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
