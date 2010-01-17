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

// This file contains the unit tests for the SharedStyleParserObserver class.

#include "kml/engine/shared_style_parser_observer.h"
#include "boost/scoped_ptr.hpp"
#include "kml/dom/kml_factory.h"
#include "kml/base/unit_test.h"

namespace kmlengine {

class SharedStyleParserObserverTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(SharedStyleParserObserverTest);
  CPPUNIT_TEST(TestAddChildSharedStyle);
  CPPUNIT_TEST(TestAddChildInlineStyle);
  CPPUNIT_TEST(TestAddChildDetectsDupeId);
  CPPUNIT_TEST(TestNewElement);
  CPPUNIT_TEST(TestDestructor);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    // Make some elements used in most tests.
    kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
    document_ = factory->CreateDocument();
    folder_ = factory->CreateFolder();
    kStyle0Id_ = "style0";
    style0_ = factory->CreateStyle();
    style0_->set_id(kStyle0Id_);
    kStyle1Id_ = "style1";
    style1_ = factory->CreateStyle();
    style1_->set_id(kStyle1Id_);
    kStyleMap0Id_ = "stylemap0";
    stylemap0_ = factory->CreateStyleMap();
    stylemap0_->set_id(kStyleMap0Id_);
    style_no_id_ = factory->CreateStyle();
    shared_style_parser_observer_.reset(
        new SharedStyleParserObserver(&shared_style_map_));
  }

  // Called after each test.
  void tearDown() {
    // All dynamic objects are managed by smart pointers.
  }

 protected:
  void TestAddChildSharedStyle();
  void TestAddChildInlineStyle();
  void TestAddChildDetectsDupeId();
  void TestNewElement();
  void TestDestructor();

 private:
  kmldom::DocumentPtr document_;
  kmldom::FolderPtr folder_;
  std::string kStyle0Id_;
  kmldom::StylePtr style0_;
  std::string kStyle1Id_;
  kmldom::StylePtr style1_;
  kmldom::StylePtr style_no_id_;
  std::string kStyleMap0Id_;
  kmldom::StyleMapPtr stylemap0_;
  SharedStyleMap shared_style_map_;
  boost::scoped_ptr<SharedStyleParserObserver> shared_style_parser_observer_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SharedStyleParserObserverTest);

// Verify that AddChild() accepts shared StyleSelectors.
void SharedStyleParserObserverTest::TestAddChildSharedStyle() {
  // Verify that AddChild() does not detect a dupe.
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->AddChild(document_,
                                                               style0_));
  // Verify that there is only one entry in the map.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), shared_style_map_.size());

  // Verify that the id maps to the Style.
  kmldom::StyleSelectorPtr object = shared_style_map_[kStyle0Id_];
  CPPUNIT_ASSERT_EQUAL(kStyle0Id_, object->get_id());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Style, object->Type());

  // Verify that AddChild() does not detect a dupe.
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->AddChild(document_,
                                                               stylemap0_));
  // Verify that there are now 2 entries in the map.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), shared_style_map_.size());
  // Verify that the id maps to the StyleMap
  object = shared_style_map_[kStyleMap0Id_];
  CPPUNIT_ASSERT_EQUAL(kStyleMap0Id_, object->get_id());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_StyleMap, object->Type());
}

// Verify that AddChild() does _not_ accept inline StyleSelectors.
void SharedStyleParserObserverTest::TestAddChildInlineStyle() {
  // Verify that AddChild() does not detect a dupe.
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->AddChild(folder_,
                                                               style0_));
  // Verify that AddChild() does not detect a dupe even when passed the same
  // parent-child again.
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->AddChild(folder_,
                                                               style0_));
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->AddChild(folder_,
                                                               stylemap0_));
  // Verify that none of the above added anything to the map because although
  // these were both StyleSelectors with ids neither was a child of Document.
  CPPUNIT_ASSERT(shared_style_map_.empty());
}

// Verify that NewElement() exists and alwyas returns true.
void SharedStyleParserObserverTest::TestNewElement() {
  // Verify that NewElement() accepts StyleSelectors.
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->NewElement(style0_));
  // Verify that a 2nd call succeeds given that NewElement is a nop
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->NewElement(style0_));
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->NewElement(style1_));
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->NewElement(stylemap0_));
  // Style with no id accepted fine:
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->NewElement(style_no_id_));
  // Document, Folder both fine...
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->NewElement(folder_));
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->NewElement(document_));
}

// Verify that AddChild() properly detects a duplicate Object id.
void SharedStyleParserObserverTest::TestAddChildDetectsDupeId() {
  // Pass a parent-child that will be added to the map.
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->AddChild(document_,
                                                               style0_));
  // Verify that AddChild() detected the dupe.
  CPPUNIT_ASSERT_EQUAL(false,
                       shared_style_parser_observer_->AddChild(document_,
                                                               style0_));
  // Verify that the map was not affected.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), shared_style_map_.size());
  kmldom::ObjectPtr object = shared_style_map_[kStyle0Id_];
  CPPUNIT_ASSERT_EQUAL(kStyle0Id_, object->get_id());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Style, object->Type());
}

// Verify that the destructor does not effect the map.
void SharedStyleParserObserverTest::TestDestructor() {
  // Use AddChild() to put some entries in the map.
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->AddChild(document_,
                                                               style0_));
  CPPUNIT_ASSERT_EQUAL(true,
                       shared_style_parser_observer_->AddChild(document_,
                                                               style1_));

  // Verify that deleting the observer does not affect the map.
  shared_style_parser_observer_.reset();

  // Verify that the object map has exactly the 2 expected mappings.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), shared_style_map_.size());
  CPPUNIT_ASSERT_EQUAL(kStyle0Id_, shared_style_map_[kStyle0Id_]->get_id());
  CPPUNIT_ASSERT_EQUAL(kStyle1Id_, shared_style_map_[kStyle1Id_]->get_id());
}

}  // end namespace kmlengine

TEST_MAIN
