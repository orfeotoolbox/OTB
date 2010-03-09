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

// This file contains the unit tests for the XsdHandler class.

#include "kml/xsd/xsd_handler.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "gtest/gtest.h"
#include "kml/xsd/xsd_file.h"
#include "kml/xsd/xsd_util.h"

using kmlbase::File;

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error DATADIR must be defined!
#endif

namespace kmlxsd {

// This class is the unit test fixture for the XsdHandler class.
class XsdHandlerTest : public testing::Test {
 protected:
  virtual void SetUp() {
    xsd_file_.reset(new XsdFile);
    xsd_handler_.reset(new XsdHandler(xsd_file_.get()));
  }
  virtual void TearDown() {
  }

  void ParseKml21Xsd();
  kmlbase::StringVector atts_;
  boost::scoped_ptr<XsdFile> xsd_file_;
  boost::scoped_ptr<XsdHandler> xsd_handler_;
};

// Verify basic usage of the expat handlers.
TEST_F(XsdHandlerTest, TestBasicBeginDataEnd) {
  const char* kElement = "vanilla";
  xsd_handler_->StartElement(kElement, atts_);  // <vanilla>
//  xsd_handler_->CharData(NULL);  // no content
  xsd_handler_->EndElement(kElement);  // </vanilla>
}

// Verify processing of <xs:schema>.
TEST_F(XsdHandlerTest, TestSchema) {
  // Send down:
  //   <schema xmlns:prefix="my:cool:namespace"
  //           targetNamespace="my:cool:namespace"/>
  atts_.push_back("xmlns:prefix");
  atts_.push_back("my:cool:namespace");
  atts_.push_back("targetNamespace");
  atts_.push_back(atts_[1]);
  xsd_handler_->StartElement(kSchema, atts_);
  xsd_handler_->EndElement(kSchema);
  ASSERT_EQ(string(atts_[1]),
                       xsd_file_->get_target_namespace());
} 

// Verify processing of (global) <xs:element>.
TEST_F(XsdHandlerTest, TestElement) {
  const char* kVanilla = "vanilla";
  const char* kChocolate = "chocolate";

  // Verify initial conditions.
  XsdElementVector element_names;
  xsd_file_->GetAllElements(&element_names);
  ASSERT_TRUE(element_names.empty());

  // <xs:schema>
  xsd_handler_->StartElement(kSchema, atts_);

  // <xs:element name="..."/>
  atts_.push_back(kName);
  // <xs:element name="vanilla"/>
  atts_.push_back(kVanilla);
  xsd_handler_->StartElement(kElement, atts_);
  xsd_handler_->EndElement(kElement);

  // XsdHandler stores the elements by name in the associated XsdFile.
  xsd_file_->GetAllElements(&element_names);

  // An <xs:element> of <xs:schema> puts it in the element name list.
  ASSERT_EQ(static_cast<size_t>(1), element_names.size());
  ASSERT_EQ(string(kVanilla), element_names[0]->get_name());

  // Send down a 2nd
  // <xs:element name="chocolate"/>
  atts_[1] = kChocolate;
  xsd_handler_->StartElement(kElement, atts_);

  element_names.clear();
  xsd_file_->GetAllElements(&element_names);
  ASSERT_EQ(static_cast<size_t>(2), element_names.size());
  // NOTE: This ordering is due to the internal STL map.
  ASSERT_EQ(string(kChocolate), element_names[0]->get_name());
  ASSERT_EQ(string(kVanilla), element_names[1]->get_name());
}

void XsdHandlerTest::ParseKml21Xsd() {
  const string kKml21Xsd(File::JoinPaths(DATADIR,
                                              File::JoinPaths("xsd",
                                                              "kml21.xsd")));
  string xsd;
  ASSERT_TRUE(File::ReadFileToString(kKml21Xsd, &xsd));
  xsd_file_.reset(XsdFile::CreateFromParse(xsd, NULL));
  ASSERT_TRUE(xsd_file_.get());
}

// Verify processing of (global) <xs:element> on a real XSD.
TEST_F(XsdHandlerTest, TestGetKml21ElementNames) {
  ParseKml21Xsd();
  XsdElementVector element_names;
  xsd_file_->GetAllElements(&element_names);
  ASSERT_EQ(static_cast<size_t>(40), element_names.size());
  ASSERT_EQ(string("BalloonStyle"), element_names[0]->get_name());
  ASSERT_EQ(string("Document"), element_names[1]->get_name());
  ASSERT_EQ(string("styleUrl"), element_names[39]->get_name());
}

// Verify processing of <xs:element> and <xs:complexType> on a real XSD.
//
//  <element name="Feature" type="kml:FeatureType" abstract="true"/>
//  <complexType name="FeatureType" abstract="true">
//    <complexContent>
//      <extension base="kml:ObjectType">
//        <sequence>
//          <element name="name" type="string" minOccurs="0"/>
//          <element name="visibility" type="boolean" default="1"
//            minOccurs="0"/>
//          <element name="open" type="boolean" default="1" minOccurs="0"/>
//          <element name="address" type="string" minOccurs="0"/>
//          <element name="phoneNumber" type="string" minOccurs="0"/>
//          <element name="Snippet" type="kml:SnippetType" minOccurs="0"/>
//          <element name="description" type="string" minOccurs="0"/>
//          <element ref="kml:LookAt" minOccurs="0"/>
//          <element ref="kml:TimePrimitive" minOccurs="0"/>
//          <element ref="kml:styleUrl" minOccurs="0"/>
//          <element ref="kml:StyleSelector" minOccurs="0"
//            maxOccurs="unbounded"/>
//          <element ref="kml:Region" minOccurs="0"/>
//          <element name="Metadata" type="kml:MetadataType" minOccurs="0"/>
//        </sequence>
//      </extension>
//    </complexContent>
//  </complexType>

TEST_F(XsdHandlerTest, TestGetKml21ElementChildren) {
  ParseKml21Xsd();
  XsdElementVector feature_children;
  xsd_file_->GetChildElements("Feature", &feature_children);
  ASSERT_EQ(static_cast<size_t>(13), feature_children.size());
  ASSERT_EQ(string("name"), feature_children[0]->get_name());
  ASSERT_EQ(string("visibility"),
                       feature_children[1]->get_name());
  ASSERT_EQ(string("open"), feature_children[2]->get_name());
  ASSERT_EQ(string("address"), feature_children[3]->get_name());
  ASSERT_EQ(string("phoneNumber"),
                       feature_children[4]->get_name());
  ASSERT_EQ(string("Snippet"), feature_children[5]->get_name());
  ASSERT_EQ(string("description"),
                       feature_children[6]->get_name());
  ASSERT_EQ(string("LookAt"), feature_children[7]->get_name());
  ASSERT_EQ(string("TimePrimitive"),
                       feature_children[8]->get_name());
  ASSERT_EQ(string("styleUrl"),
                       feature_children[9]->get_name());
  ASSERT_EQ(string("StyleSelector"),
                       feature_children[10]->get_name());
  ASSERT_EQ(string("Region"), feature_children[11]->get_name());
  ASSERT_EQ(string("Metadata"),
                       feature_children[12]->get_name());
}

// Verify processing of <xs:complexType> and <xs:extension base="...">
// on a real XSD.
TEST_F(XsdHandlerTest, TestGetKml21ExtensionBase) {
  ParseKml21Xsd();
  XsdElementPtr element = xsd_file_->FindElement("GroundOverlay");
  ASSERT_TRUE(element);
  XsdComplexTypePtr groundoverlay = XsdComplexType::AsComplexType(
      xsd_file_->FindElementType(element));
  ASSERT_TRUE(groundoverlay);
  std::vector<XsdComplexTypePtr> type_hier;
  ASSERT_TRUE(xsd_file_->GetTypeHierarchy(groundoverlay, &type_hier));
  ASSERT_EQ(static_cast<size_t>(3), type_hier.size());
  ASSERT_EQ(string("OverlayType"), type_hier[0]->get_name());
  ASSERT_EQ(string("FeatureType"), type_hier[1]->get_name());
  ASSERT_EQ(string("ObjectType"), type_hier[2]->get_name());
}

// Verify processing of <xs:simpleType>, <xs:restriction base="..."/>,
// and <xs:enumeration value=".../> on a real XSD.
TEST_F(XsdHandlerTest, TestGetKml21Enumeration) {
  ParseKml21Xsd();
  XsdSimpleTypePtr altitude_mode_enum = XsdSimpleType::AsSimpleType(
      xsd_file_->FindType("altitudeModeEnum"));
  ASSERT_TRUE(altitude_mode_enum);
  ASSERT_TRUE(altitude_mode_enum->IsEnumeration());
  ASSERT_EQ(string("string"), altitude_mode_enum->get_restriction_base());
  ASSERT_EQ(static_cast<size_t>(3), altitude_mode_enum->get_enumeration_size());
  ASSERT_EQ(string("clampToGround"),
            altitude_mode_enum->get_enumeration_at(0));
  ASSERT_EQ(string("relativeToGround"),
                       altitude_mode_enum->get_enumeration_at(1));
  ASSERT_EQ(string("absolute"), altitude_mode_enum->get_enumeration_at(2));
}

}  // end namespace kmlxsd

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
