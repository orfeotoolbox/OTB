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
#include "kml/dom/element.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/placemark.h"
#include "kml/dom/parser.h"
#include "kml/dom/parser_observer.h"
#include "kml/base/unit_test.h"

namespace kmldom {

typedef std::vector<ElementPtr> element_vector_t;

// This class is the unit test fixture for the KmlHandler class.
class KmlHandlerTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(KmlHandlerTest);
  CPPUNIT_TEST(TestInitialState);
  CPPUNIT_TEST(TestStartSimpleElement);
  CPPUNIT_TEST(TestEndSimpleElement);
  CPPUNIT_TEST(TestBasicCharData);
  CPPUNIT_TEST(TestStartComplexElement);
  CPPUNIT_TEST(TestEndComplexElement);
  CPPUNIT_TEST(TestStartComplexElementWithAtts);
  CPPUNIT_TEST(SimpleNewElementObserverTest);
  CPPUNIT_TEST(NewElementObserverTerminationTest);
  CPPUNIT_TEST(SimpleAddChildObserverTest);
  CPPUNIT_TEST(AddChildObserverTerminationTest);
  CPPUNIT_TEST(MultipleObserverNormalTest);
  CPPUNIT_TEST(MultipleObserverTerminationTest);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    // Emulate expat's xmlparse.c:startAtts().
    // 16 == xmlparse.c's INIT_ATTS_SIZE
    atts_ = static_cast<const char**>(calloc(16, sizeof(char*)));
    kml_handler_ = new KmlHandler(observers_);
  }

  // Called after each test.
  void tearDown() {
    free(atts_);
    delete kml_handler_;
  }

 protected:
  void TestInitialState();
  void TestStartSimpleElement();
  void TestEndSimpleElement();
  void TestBasicCharData();
  void TestStartComplexElement();
  void TestEndComplexElement();
  void TestStartComplexElementWithAtts();
  void SimpleNewElementObserverTest();
  void NewElementObserverTerminationTest();
  void SimpleAddChildObserverTest();
  void AddChildObserverTerminationTest();
  void MultipleObserverNormalTest();
  void MultipleObserverTerminationTest();

 private:
  const char** atts_;
  parser_observer_vector_t observers_;
  KmlHandler* kml_handler_;
  void VerifyFolderParse(const ElementPtr& root) const;
  void VerifyElementTypes(const KmlDomType* types_array,
                          const element_vector_t& element_vector) const;
  void MultipleObserverTestCommon(size_t max_elements,
                                  size_t expected_elements,
                                  size_t expected_pairs) const;
};

CPPUNIT_TEST_SUITE_REGISTRATION(KmlHandlerTest);

// This verifies the initial state of a freshly constructed KmlHandler.
void KmlHandlerTest::TestInitialState() {
  // No elements have been processed, but the PopRoot() method should
  // be well behaved.
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
}

// This is a test of the StartElement() method for a known simple element.
void KmlHandlerTest::TestStartSimpleElement() {
  // This is what expat sends to StartElement() on "<name>".
  kml_handler_->StartElement("name", atts_);

  // Since "name" is known we will find it as the root element.
  ElementPtr root = kml_handler_->PopRoot();
  CPPUNIT_ASSERT(root->Type() == Type_name);

  // PopRoot() is destructive so now there is nothing.
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
}

// This is a test of the EndElement() method for a known simple element.
void KmlHandlerTest::TestEndSimpleElement() {
  // This is what expat sends to KmlHandler on "<name/>":
  kml_handler_->StartElement("name", atts_);
  kml_handler_->EndElement("name");

  ElementPtr root = kml_handler_->PopRoot();
  CPPUNIT_ASSERT(root->Type() == Type_name);

  // PopRoot() is destructive so now there is nothing.
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
}

// This is a test of the CharData() method for a known simple element.
void KmlHandlerTest::TestBasicCharData() {
  // This is what expat sends to KmlHandler on "<name>what is in a</name>":
  const char* kTagName = "name";
  const char* kContent = "what is in a name";

  kml_handler_->StartElement(kTagName, atts_);
  kml_handler_->CharData(kContent, strlen(kContent));
  kml_handler_->EndElement(kTagName);

  ElementPtr root = kml_handler_->PopRoot();
  CPPUNIT_ASSERT(root->Type() == Type_name);
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
  CPPUNIT_ASSERT(kContent == root->get_char_data());
}

// This is a test of the StartElement() method for a known complex element.
void KmlHandlerTest::TestStartComplexElement() {
  kml_handler_->StartElement("Placemark", atts_);
  ElementPtr root = kml_handler_->PopRoot();
  CPPUNIT_ASSERT(root->Type() == Type_Placemark);
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
}

// This is a test of the EndElement() method for a known complex element.
void KmlHandlerTest::TestEndComplexElement() {
  // This is what expat does for "<Placemark/>".
  kml_handler_->StartElement("Placemark", atts_);
  kml_handler_->EndElement("Placemark");
  ElementPtr root = kml_handler_->PopRoot();
  CPPUNIT_ASSERT(root->Type() == Type_Placemark);
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
}

// This is a test of StartElement() for a known complex element with known
// attributes.
void KmlHandlerTest::TestStartComplexElementWithAtts() {
  const char* kAttrName = "id";
  const char* kAttrVal = "foo";
  atts_[0] = kAttrName;
  atts_[1] = kAttrVal;
  kml_handler_->StartElement("Placemark", atts_);
  ElementPtr root = kml_handler_->PopRoot();
  CPPUNIT_ASSERT(root->Type() == Type_Placemark);
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
  PlacemarkPtr placemark = AsPlacemark(root);
  CPPUNIT_ASSERT(kAttrVal == placemark->get_id());
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
    CPPUNIT_ASSERT_EQUAL(*(types_array+i),
                         element_vector[i]->Type());
  }
}

// This helper function verifies the proper state of kKmlFolder's DOM.
void KmlHandlerTest::VerifyFolderParse(const ElementPtr& root) const {
  KmlPtr kml = AsKml(root);
  CPPUNIT_ASSERT(kml);
  FolderPtr folder = AsFolder(kml->get_feature());
  CPPUNIT_ASSERT(folder);
  CPPUNIT_ASSERT(folder->has_name());
  CPPUNIT_ASSERT(!folder->has_visibility());
  CPPUNIT_ASSERT(!folder->has_open());
  CPPUNIT_ASSERT(folder->has_description());
  CPPUNIT_ASSERT(folder->has_region());
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1),
                       folder->get_feature_array_size());
  PlacemarkPtr placemark = AsPlacemark(folder->get_feature_array_at(0));
  PointPtr point = AsPoint(placemark->get_geometry());
  CPPUNIT_ASSERT(!point->has_coordinates());
}

// This is a simple test of the NewElement() for an observer which does not
// terminate the parse.
void KmlHandlerTest::SimpleNewElementObserverTest() {
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
void KmlHandlerTest::NewElementObserverTerminationTest() {
  Parser parser;
  element_vector_t element_vector;
  // This specifies to stop parsing after 2 elements.
  SimpleNewElementObserver simple_new_element_observer(&element_vector, 2);
  parser.AddObserver(&simple_new_element_observer);
  std::string errors;
  ElementPtr root = parser.Parse(kKmlFolder, &errors);

  // Verify that the parse was terminated.
  CPPUNIT_ASSERT(!root);
  CPPUNIT_ASSERT(!errors.empty());

  // Verify that exactly the first 2 elements were gathered.
  CPPUNIT_ASSERT(2 == element_vector.size());
  CPPUNIT_ASSERT_EQUAL(Type_kml, element_vector[0]->Type());
  CPPUNIT_ASSERT_EQUAL(Type_Folder, element_vector[1]->Type());
}

// This is a simple test of the AddChild() for an observer which does not
// terminate the parse.
void KmlHandlerTest::SimpleAddChildObserverTest() {
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
void KmlHandlerTest::AddChildObserverTerminationTest() {
  Parser parser;
  element_vector_t parent_vector;
  element_vector_t child_vector;
  // This specifies to stop parsing after 4 parent-child pairs.
  SimpleAddChildObserver simple_add_child_observer(&parent_vector,
                                                   &child_vector,
                                                   4);
  parser.AddObserver(&simple_add_child_observer);
  std::string errors;
  ElementPtr root = parser.Parse(kKmlFolder, &errors);

  // Verify that the parse was terminated.
  CPPUNIT_ASSERT(!root);
  CPPUNIT_ASSERT(!errors.empty());

  // Verify that exactly the first 4 parent-child pairs were gathered.
  CPPUNIT_ASSERT(4 == parent_vector.size());
  CPPUNIT_ASSERT(4 == child_vector.size());
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
  std::string errors;
  ElementPtr root = parser.Parse(kKmlFolder, &errors);

  if (expected_element_count >= kNumElements) {
    // Verify that the observers did not interfere with the parse as normal.
    CPPUNIT_ASSERT(errors.empty());
    VerifyFolderParse(root);
  } else {
    // Verify that an observer teminated the parse.
    CPPUNIT_ASSERT(!root);
    CPPUNIT_ASSERT(!errors.empty());
  }

  // Verify that the observers functioned properly.
  CPPUNIT_ASSERT(expected_element_count == element_vector.size());
  CPPUNIT_ASSERT(expected_pair_count == parent_vector.size());
  CPPUNIT_ASSERT(expected_pair_count == child_vector.size());
  VerifyElementTypes(kKmlFolderNewElementOrder, element_vector);
  VerifyElementTypes(kKmlFolderParentOrder, parent_vector);
  VerifyElementTypes(kKmlFolderChildOrder, child_vector);
}

// Verify proper operation with multiple ParseObservers when no observer
// terminates the parse.
void KmlHandlerTest::MultipleObserverNormalTest() {
  KmlHandlerTest::MultipleObserverTestCommon(kNumElements, kNumElements,
                                             kNumElements-1);
}

// Verify proper operation with multiple ParseObservers when an observer
// terminates the parse.
void KmlHandlerTest::MultipleObserverTerminationTest() {
  KmlHandlerTest::MultipleObserverTestCommon(0, 0, 0);
  // Accepting just one element results in seeing no pairs.
  KmlHandlerTest::MultipleObserverTestCommon(1, 1, 0);
  // These are highly dependent on the exact form of kKmlFolder!
  KmlHandlerTest::MultipleObserverTestCommon(2, 2, 1);
  KmlHandlerTest::MultipleObserverTestCommon(6, 6, 4);
}

}  // end namespace kmldom

TEST_MAIN
