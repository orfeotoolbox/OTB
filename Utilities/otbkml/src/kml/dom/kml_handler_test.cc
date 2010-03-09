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

// This file contains the unit tests for the KmlHandler class.

#include "kml/dom/kml_handler.h"
#include <stdlib.h>  // For calloc() and free().
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/dom/element.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/placemark.h"
#include "kml/dom/parser.h"
#include "kml/dom/parser_observer.h"
#include "gtest/gtest.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmldom {

typedef std::vector<ElementPtr> element_vector_t;

// This class is the unit test fixture for the KmlHandler class.
class KmlHandlerTest : public testing::Test {
 protected:
  virtual void SetUp() {
    kml_handler_.reset(new KmlHandler(observers_));
  }

  virtual void TearDown() {
  }

  kmlbase::StringVector atts_;
  parser_observer_vector_t observers_;
  boost::scoped_ptr<KmlHandler> kml_handler_;
  void VerifyFolderParse(const ElementPtr& root) const;
  void VerifyElementTypes(const KmlDomType* types_array,
                          const element_vector_t& element_vector) const;
  void MultipleObserverTestCommon(size_t max_elements,
                                  size_t expected_elements,
                                  size_t expected_pairs) const;
};

// This verifies the initial state of a freshly constructed KmlHandler.
TEST_F(KmlHandlerTest, TestInitialState) {
  // No elements have been processed, but the PopRoot() method should
  // be well behaved.
  ASSERT_TRUE(NULL == kml_handler_->PopRoot());
}

// This is a test of the StartElement() method for a known simple element.
TEST_F(KmlHandlerTest, TestStartSimpleElement) {
  // This is what expat sends to StartElement() on "<name>".
  kml_handler_->StartElement("name", atts_);

  // Since "name" is known we will find it as the root element.
  ElementPtr root = kml_handler_->PopRoot();
  ASSERT_EQ(root->Type(), Type_name);

  // PopRoot() is destructive so now there is nothing.
  ASSERT_TRUE(NULL == kml_handler_->PopRoot());
}

// This is a test of the EndElement() method for a known simple element.
TEST_F(KmlHandlerTest, TestEndSimpleElement) {
  // This is what expat sends to KmlHandler on "<name/>":
  kml_handler_->StartElement("name", atts_);
  kml_handler_->EndElement("name");

  ElementPtr root = kml_handler_->PopRoot();
  ASSERT_EQ(root->Type(), Type_name);

  // PopRoot() is destructive so now there is nothing.
  ASSERT_TRUE(NULL == kml_handler_->PopRoot());
}

// This is a test of the CharData() method for a known simple element.
TEST_F(KmlHandlerTest, TestBasicCharData) {
  // This is what expat sends to KmlHandler on "<name>what is in a</name>":
  const char* kTagName = "name";
  const char* kContent = "what is in a name";

  kml_handler_->StartElement(kTagName, atts_);
  kml_handler_->CharData(kContent);
  kml_handler_->EndElement(kTagName);

  ElementPtr root = kml_handler_->PopRoot();
  ASSERT_EQ(root->Type(), Type_name);
  ASSERT_TRUE(NULL == kml_handler_->PopRoot());
  ASSERT_EQ(kContent, root->get_char_data());
}

// This is a test of the StartElement() method for a known complex element.
TEST_F(KmlHandlerTest, TestStartComplexElement) {
  kml_handler_->StartElement("Placemark", atts_);
  ElementPtr root = kml_handler_->PopRoot();
  ASSERT_EQ(root->Type(), Type_Placemark);
  ASSERT_TRUE(NULL == kml_handler_->PopRoot());
}

// This is a test of the EndElement() method for a known complex element.
TEST_F(KmlHandlerTest, TestEndComplexElement) {
  // This is what expat does for "<Placemark/>".
  kml_handler_->StartElement("Placemark", atts_);
  kml_handler_->EndElement("Placemark");
  ElementPtr root = kml_handler_->PopRoot();
  ASSERT_EQ(root->Type(), Type_Placemark);
  ASSERT_TRUE(NULL == kml_handler_->PopRoot());
}

// This is a test of StartElement() for a known complex element with known
// attributes.
TEST_F(KmlHandlerTest, TestStartComplexElementWithAtts) {
  const string kAttrName("id");
  const string kAttrVal("foo");
  atts_.push_back(kAttrName);
  atts_.push_back(kAttrVal);
  kml_handler_->StartElement("Placemark", atts_);
  ElementPtr root = kml_handler_->PopRoot();
  ASSERT_EQ(root->Type(), Type_Placemark);
  ASSERT_TRUE(NULL == kml_handler_->PopRoot());
  PlacemarkPtr placemark = AsPlacemark(root);
  ASSERT_EQ(kAttrVal, placemark->get_id());
}

// This ParserObserver simply appends each Element passed to its NewElement
// to the vector passed to the constructor.  The max_elements constructor arg
// terminates the parse if the specified number of elements have been parsed.
class SimpleNewElementObserver : public ParserObserver {
 public:
  SimpleNewElementObserver(element_vector_t* new_element_vector,
                           size_t max_elements)
    : new_element_vector_(new_element_vector), max_elements_(max_elements) {
  }

  // ParserObserver::NewElement().  Append the new element to our vector.
  virtual bool NewElement(const kmldom::ElementPtr& element) {
    if (new_element_vector_->size() == max_elements_) {
      return false;  // Terminates parse.
    }
    new_element_vector_->push_back(element);
    return true;  // Keep parsing.
  }

  // Default implementation of AddChild() returns true.

 private:
  element_vector_t* new_element_vector_;
  size_t max_elements_;
};

// This ParserObserver appends each parent-child pair to the supplied vectors.
// Plain vectors are used to simplify testing which is based on
// VerifyElementTypes().  The max_elements constructor arg specifies to
// terminate the parse if the specified number of pairs have been parsed.
class SimpleAddChildObserver : public ParserObserver {
 public:
  SimpleAddChildObserver(element_vector_t* parent_vector,
                         element_vector_t* child_vector, size_t max_elements)
    : parent_vector_(parent_vector),
      child_vector_(child_vector),
      max_elements_(max_elements) {
  }

  // Default implementation of NewElement() returns true.

  virtual bool AddChild(const kmldom::ElementPtr& parent,
                        const kmldom::ElementPtr& child) {
    if (parent_vector_->size() == max_elements_) {
      return false;  // Terminate parse.
    }
    parent_vector_->push_back(parent);
    child_vector_->push_back(child);
    return true;  // Keep parsing.
  }

 private:
  element_vector_t* parent_vector_;
  element_vector_t* child_vector_;
  size_t max_elements_;
};

// This KML document and test are kept here together.
static const char kKmlFolder[] =
  "<kml>"
  "<Folder><name/><description/><Region/>"
  "<Placemark><Point/></Placemark>"
  "</Folder>"
  "</kml>";

static const size_t kNumElements = 7;  // Number of elements in kKmlFolder.

// This is the order of the elements from kKmlFolder see in NewElement().
static const KmlDomType kKmlFolderNewElementOrder[] = {
  Type_kml, Type_Folder, Type_name, Type_description, Type_Region,
  Type_Placemark, Type_Point };

// This is the order of the elements from kKmlFolder seen in AddChild().
static const KmlDomType kKmlFolderParentOrder[] = {
  Type_Folder, Type_Folder, Type_Folder, Type_Placemark, Type_Folder, Type_kml
};
static const KmlDomType kKmlFolderChildOrder[] = {
  Type_name, Type_description, Type_Region, Type_Point, Type_Placemark,
  Type_Folder
};

// Verify that each element in the vector is of the corresponding type
// in the types_array.
void KmlHandlerTest::VerifyElementTypes(
    const KmlDomType* types_array,
    const element_vector_t& element_vector) const {
  for (size_t i = 0; i < element_vector.size(); ++i) {
    ASSERT_EQ(*(types_array+i), element_vector[i]->Type());
  }
}

// This helper function verifies the proper state of kKmlFolder's DOM.
void KmlHandlerTest::VerifyFolderParse(const ElementPtr& root) const {
  KmlPtr kml = AsKml(root);
  ASSERT_TRUE(kml);
  FolderPtr folder = AsFolder(kml->get_feature());
  ASSERT_TRUE(folder);
  ASSERT_TRUE(folder->has_name());
  ASSERT_FALSE(folder->has_visibility());
  ASSERT_FALSE(folder->has_open());
  ASSERT_TRUE(folder->has_description());
  ASSERT_TRUE(folder->has_region());
  ASSERT_EQ(static_cast<size_t>(1), folder->get_feature_array_size());
  PlacemarkPtr placemark = AsPlacemark(folder->get_feature_array_at(0));
  PointPtr point = AsPoint(placemark->get_geometry());
  ASSERT_FALSE(point->has_coordinates());
}

// This is a simple test of the NewElement() for an observer which does not
// terminate the parse.
TEST_F(KmlHandlerTest, SimpleNewElementObserverTest) {
  Parser parser;
  element_vector_t element_vector;
  // This specifies to let the parse complete all 7 elements which are saved
  // in the order encountered to element_vector.
  SimpleNewElementObserver simple_new_element_observer(&element_vector,
                                                       kNumElements);
  parser.AddObserver(&simple_new_element_observer);
  ElementPtr root = parser.Parse(kKmlFolder, NULL);

  // Verify that the entire document parsed properly.
  VerifyFolderParse(root);

  // Verify that the observer's NewElement() saw the expected elements in
  // the expected order.
  VerifyElementTypes(kKmlFolderNewElementOrder, element_vector);
}

// This verifies that an observer returning false from NewElement() terminates
// the parse.
TEST_F(KmlHandlerTest, NewElementObserverTerminationTest) {
  Parser parser;
  element_vector_t element_vector;
  // This specifies to stop parsing after 2 elements.
  SimpleNewElementObserver simple_new_element_observer(&element_vector, 2);
  parser.AddObserver(&simple_new_element_observer);
  string errors;
  ElementPtr root = parser.Parse(kKmlFolder, &errors);

  // Verify that the parse was terminated.
  ASSERT_FALSE(root);
  ASSERT_FALSE(errors.empty());

  // Verify that exactly the first 2 elements were gathered.
  ASSERT_EQ(static_cast<size_t>(2), element_vector.size());
  ASSERT_EQ(Type_kml, element_vector[0]->Type());
  ASSERT_EQ(Type_Folder, element_vector[1]->Type());
}

// This is a simple test of the AddChild() for an observer which does not
// terminate the parse.
TEST_F(KmlHandlerTest, SimpleAddChildObserverTest) {
  Parser parser;
  element_vector_t parent_vector;
  element_vector_t child_vector;
  // This specifies to let the parse complete all 7 elements which are saved
  // in the order encountered to element_vector.
  SimpleAddChildObserver simple_add_child_observer(&parent_vector,
                                                   &child_vector,
                                                   kNumElements);
  parser.AddObserver(&simple_add_child_observer);
  ElementPtr root = parser.Parse(kKmlFolder, NULL);

  // Verify that the observer did not interfere with the parse as normal.
  VerifyFolderParse(root);

  // Verify that the observer's AddChild() saw the expected elements in
  // the expected order.
  VerifyElementTypes(kKmlFolderParentOrder, parent_vector);
  VerifyElementTypes(kKmlFolderChildOrder, child_vector);
}

// This verifies that an observer returning false from AddChild() terminates
// the parse.
TEST_F(KmlHandlerTest, AddChildObserverTerminationTest) {
  Parser parser;
  element_vector_t parent_vector;
  element_vector_t child_vector;
  // This specifies to stop parsing after 4 parent-child pairs.
  SimpleAddChildObserver simple_add_child_observer(&parent_vector,
                                                   &child_vector,
                                                   4);
  parser.AddObserver(&simple_add_child_observer);
  string errors;
  ElementPtr root = parser.Parse(kKmlFolder, &errors);

  // Verify that the parse was terminated.
  ASSERT_FALSE(root);
  ASSERT_FALSE(errors.empty());

  // Verify that exactly the first 4 parent-child pairs were gathered.
  ASSERT_EQ(static_cast<size_t>(4),parent_vector.size());
  ASSERT_EQ(static_cast<size_t>(4), child_vector.size());
  VerifyElementTypes(kKmlFolderParentOrder, parent_vector);
  VerifyElementTypes(kKmlFolderChildOrder, child_vector);
}

// This verifies that multiple ParserObservers function properly and that
// the expected number of new elements and element pairs are seen for the
// given value of max_elements.
void KmlHandlerTest::MultipleObserverTestCommon(size_t max_elements,
                                                size_t expected_element_count,
                                                size_t expected_pair_count)
                                                const {
  element_vector_t element_vector;
  SimpleNewElementObserver simple_new_element_observer(&element_vector,
                                                       max_elements);
  element_vector_t parent_vector;
  element_vector_t child_vector;
  SimpleAddChildObserver simple_parent_child_observer(&parent_vector,
                                                      &child_vector,
                                                      max_elements);
  ParserObserver null_observer;

  Parser parser;
  parser.AddObserver(&null_observer);
  parser.AddObserver(&simple_new_element_observer);
  parser.AddObserver(&simple_parent_child_observer);
  string errors;
  ElementPtr root = parser.Parse(kKmlFolder, &errors);

  if (expected_element_count >= kNumElements) {
    // Verify that the observers did not interfere with the parse as normal.
    ASSERT_TRUE(errors.empty());
    VerifyFolderParse(root);
  } else {
    // Verify that an observer teminated the parse.
    ASSERT_FALSE(root);
    ASSERT_FALSE(errors.empty());
  }

  // Verify that the observers functioned properly.
  ASSERT_TRUE(expected_element_count == element_vector.size());
  ASSERT_TRUE(expected_pair_count == parent_vector.size());
  ASSERT_TRUE(expected_pair_count == child_vector.size());
  VerifyElementTypes(kKmlFolderNewElementOrder, element_vector);
  VerifyElementTypes(kKmlFolderParentOrder, parent_vector);
  VerifyElementTypes(kKmlFolderChildOrder, child_vector);
}

// Verify proper operation with multiple ParseObservers when no observer
// terminates the parse.
TEST_F(KmlHandlerTest, MultipleObserverNormalTest) {
  KmlHandlerTest::MultipleObserverTestCommon(kNumElements, kNumElements,
                                             kNumElements-1);
}

// Verify proper operation with multiple ParseObservers when an observer
// terminates the parse.
TEST_F(KmlHandlerTest, MultipleObserverTerminationTest) {
  KmlHandlerTest::MultipleObserverTestCommon(0, 0, 0);
  // Accepting just one element results in seeing no pairs.
  KmlHandlerTest::MultipleObserverTestCommon(1, 1, 0);
  // These are highly dependent on the exact form of kKmlFolder!
  KmlHandlerTest::MultipleObserverTestCommon(2, 2, 1);
  KmlHandlerTest::MultipleObserverTestCommon(6, 6, 4);
}

// This ParserObserver collects all Features in the parse.
class FeatureCollector : public ParserObserver {
 public:
  FeatureCollector(element_vector_t* element_vector)
    : element_vector_(element_vector) {
  }
  // This EndElement saves each non-Container Feature and returns false to
  // request that the parser not give this feature to the given parent.
  // All other parent-child relationships are preserved (such as all children
  // of the collected feature).
  virtual bool EndElement(const kmldom::ElementPtr& parent,
                          const kmldom::ElementPtr& child) {
    if (child->IsA(Type_Feature) && !child->IsA(Type_Container)) {
      element_vector_->push_back(child);
      return false;
    }
    return true;
  }
 private:
  element_vector_t* element_vector_;
};

TEST_F(KmlHandlerTest, InhibitingEndElement) {
  element_vector_t features;
  FeatureCollector feature_collector(&features);
  observers_.push_back(&feature_collector);
  KmlHandler kml_handler(observers_);
  kml_handler.StartElement("kml", atts_);
  kml_handler.StartElement("Document", atts_);
  kml_handler.StartElement("Placemark", atts_);
  kml_handler.StartElement("name", atts_);
  kml_handler.EndElement("name");
  kml_handler.StartElement("Point", atts_);
  kml_handler.StartElement("coordinates", atts_);
  kml_handler.EndElement("coordinates");
  kml_handler.EndElement("Point");
  kml_handler.EndElement("Placemark");
  kml_handler.EndElement("Document");
  kml_handler.StartElement("NetworkLinkControl", atts_);
  kml_handler.EndElement("NetworkLinkControl");
  kml_handler.EndElement("kml");
  ElementPtr root = kml_handler.PopRoot();
  ASSERT_TRUE(root);
  KmlPtr kml = AsKml(root);
  ASSERT_TRUE(kml);
  // Document is a Container and is not collected.
  ASSERT_TRUE(kml->has_feature());
  ASSERT_TRUE(AsDocument(kml->get_feature()));
  // NetworkLinkControl is not a Feature is not collected.
  ASSERT_TRUE(kml->has_networklinkcontrol());
  // One non-Container Feature is collected.
  ASSERT_EQ(static_cast<size_t>(1), features.size());
  PlacemarkPtr placemark = AsPlacemark(features[0]);
  ASSERT_TRUE(placemark);
  // Verify the collected feature has all expected children.
  ASSERT_TRUE(placemark->has_name());
  ASSERT_TRUE(placemark->has_geometry());
  PointPtr point = AsPoint(placemark->get_geometry());
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_coordinates());
}

TEST_F(KmlHandlerTest, TestParserHandlesGrossDescriptions) {
  // HTML markup in <description> MUST be wrapped with CDATA elements like so:
  // <description><![CDATA[<h1>title</h1>]]></description>
  // However, the web has files with markup like this:
  // <description><table><tr>...</tr><table></description>
  // Historically, Google Earth has preserved the author's intent with this
  // type of invalid markup. And hence, we try to as well.
  const string kInvalidDescriptions(
      kmlbase::File::JoinPaths(DATADIR, kmlbase::File::JoinPaths(
          "kml", "invalid_descriptions.kml")));
  string data;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(kInvalidDescriptions, &data));
  ElementPtr root = Parse(data, NULL);
  ASSERT_TRUE(root);
  KmlPtr kml = AsKml(root);
  ASSERT_TRUE(kml);
  DocumentPtr document = AsDocument(kml->get_feature());
  ASSERT_TRUE(document);
  ASSERT_EQ(static_cast<size_t>(3), document->get_feature_array_size());

  PlacemarkPtr placemark0 = AsPlacemark(document->get_feature_array_at(0));
  const string kExpected0("<b>bold</b>");
  ASSERT_EQ(kExpected0, placemark0->get_description());

  PlacemarkPtr placemark1 = AsPlacemark(document->get_feature_array_at(1));
  const string kExpected1("foo<b>bold</b>bar");
  ASSERT_EQ(kExpected1, placemark1->get_description());

  PlacemarkPtr placemark2 = AsPlacemark(document->get_feature_array_at(2));
  const string kExpected2("<description>foo<b>bold</b>bar</description>");
  ASSERT_EQ(kExpected2, placemark2->get_description());
}

TEST_F(KmlHandlerTest, TestParserHandlesBoolWhitespace) {
  const string kOutlineSpace(
      kmlbase::File::JoinPaths(DATADIR, kmlbase::File::JoinPaths(
          "kml", "outline_space.kml")));
  string data;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(kOutlineSpace, &data));
  ElementPtr root = Parse(data, NULL);
  ASSERT_TRUE(root);
  DocumentPtr document = AsDocument(AsKml(root)->get_feature());
  StylePtr style = AsStyle(document->get_styleselector_array_at(0));
  PolyStylePtr polystyle = style->get_polystyle();
  ASSERT_EQ(false, polystyle->get_fill());
  ASSERT_EQ(true, polystyle->get_outline());
  PlacemarkPtr placemark = AsPlacemark(document->get_feature_array_at(0));
  polystyle = AsStyle(placemark->get_styleselector())->get_polystyle();
  ASSERT_EQ(false, polystyle->get_fill());
  ASSERT_EQ(true, polystyle->get_outline());
}

// 100 nested folders is equal to our default nesting limit.
TEST_F(KmlHandlerTest, TestMaxNestingOf100Folders) {
  const string k100Folders(
      kmlbase::File::JoinPaths(DATADIR, kmlbase::File::JoinPaths(
          "kml", "100_nested_folders.kml")));
  string data;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(k100Folders, &data));
  ElementPtr root = Parse(data, NULL);
  ASSERT_TRUE(root);  // Parse succeeded.
}

// 101 nested folders exceeds our default nesting limit of 100.
TEST_F(KmlHandlerTest, TestMaxNestingOf101Folders) {
  const string k101Folders(
      kmlbase::File::JoinPaths(DATADIR, kmlbase::File::JoinPaths(
          "kml", "101_nested_folders.kml")));
  string data;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(k101Folders, &data));
  ElementPtr root = Parse(data, NULL);
  ASSERT_FALSE(root);  // Parse was stopped.
}

// 101 nested elements exceeds our default nesting limit of 100.
TEST_F(KmlHandlerTest, TestMaxNestingOf101Elements) {
  const string k101Elements(
      kmlbase::File::JoinPaths(DATADIR, kmlbase::File::JoinPaths(
          "kml", "101_nested_elements.kml")));
  string data;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(k101Elements, &data));
  ElementPtr root = Parse(data, NULL);
  ASSERT_FALSE(root);  // Parse was stopped.
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

