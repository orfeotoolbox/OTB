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
#include "gtest/gtest.h"

namespace kmlengine {

class SharedStyleParserObserverTest : public testing::Test {
 protected:
  virtual void SetUp() {
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
    // Create a non-strict SharedStyleParserObserver.
    shared_style_parser_observer_.reset(
        new SharedStyleParserObserver(&shared_style_map_, false));
  }

  kmldom::DocumentPtr document_;
  kmldom::FolderPtr folder_;
  string kStyle0Id_;
  kmldom::StylePtr style0_;
  string kStyle1Id_;
  kmldom::StylePtr style1_;
  kmldom::StylePtr style_no_id_;
  string kStyleMap0Id_;
  kmldom::StyleMapPtr stylemap0_;
  SharedStyleMap shared_style_map_;
  boost::scoped_ptr<SharedStyleParserObserver> shared_style_parser_observer_;
};

// Verify that AddChild() accepts shared StyleSelectors.
TEST_F(SharedStyleParserObserverTest, TestAddChildSharedStyle) {
  // Verify that AddChild() does not detect a dupe.
  ASSERT_TRUE(shared_style_parser_observer_->AddChild(document_, style0_));
  // Verify that there is only one entry in the map.
  ASSERT_EQ(static_cast<size_t>(1), shared_style_map_.size());

  // Verify that the id maps to the Style.
  kmldom::StyleSelectorPtr object = shared_style_map_[kStyle0Id_];
  ASSERT_EQ(kStyle0Id_, object->get_id());
  ASSERT_EQ(kmldom::Type_Style, object->Type());

  // Verify that AddChild() does not detect a dupe.
  ASSERT_TRUE(shared_style_parser_observer_->AddChild(document_, stylemap0_));
  // Verify that there are now 2 entries in the map.
  ASSERT_EQ(static_cast<size_t>(2), shared_style_map_.size());
  // Verify that the id maps to the StyleMap
  object = shared_style_map_[kStyleMap0Id_];
  ASSERT_EQ(kStyleMap0Id_, object->get_id());
  ASSERT_EQ(kmldom::Type_StyleMap, object->Type());
}

// Verify that AddChild() does _not_ accept inline StyleSelectors.
TEST_F(SharedStyleParserObserverTest, TestAddChildInlineStyle) {
  // Verify that AddChild() does not detect a dupe.
  ASSERT_TRUE(shared_style_parser_observer_->AddChild(folder_, style0_));
  // Verify that AddChild() does not detect a dupe even when passed the same
  // parent-child again.
  ASSERT_TRUE(shared_style_parser_observer_->AddChild(folder_, style0_));
  ASSERT_TRUE(shared_style_parser_observer_->AddChild(folder_, stylemap0_));
  // Verify that none of the above added anything to the map because although
  // these were both StyleSelectors with ids neither was a child of Document.
  ASSERT_TRUE(shared_style_map_.empty());
}

// Verify that NewElement() exists and alwyas returns true.
TEST_F(SharedStyleParserObserverTest, TestNewElement) {
  // Verify that NewElement() accepts StyleSelectors.
  ASSERT_TRUE(shared_style_parser_observer_->NewElement(style0_));
  // Verify that a 2nd call succeeds given that NewElement is a nop
  ASSERT_TRUE(shared_style_parser_observer_->NewElement(style0_));
  ASSERT_TRUE(shared_style_parser_observer_->NewElement(style1_));
  ASSERT_TRUE(shared_style_parser_observer_->NewElement(stylemap0_));
  // Style with no id accepted fine:
  ASSERT_TRUE(shared_style_parser_observer_->NewElement(style_no_id_));
  // Document, Folder both fine...
  ASSERT_TRUE(shared_style_parser_observer_->NewElement(folder_));
  ASSERT_TRUE(shared_style_parser_observer_->NewElement(document_));
}

// Verify that AddChild() properly detects a duplicate Object id when
// strict_parse is true.
TEST_F(SharedStyleParserObserverTest, TestAddChildDetectsDupeId) {
  boost::scoped_ptr<SharedStyleParserObserver> shared_style_parser_observer(
      new SharedStyleParserObserver(&shared_style_map_, true));
  // Pass a parent-child that will be added to the map.
  ASSERT_TRUE(shared_style_parser_observer->AddChild(document_, style0_));
  // Verify that AddChild() detected the dupe.
  ASSERT_FALSE(shared_style_parser_observer->AddChild(document_, style0_));
  // Verify that the map was not affected.
  ASSERT_EQ(static_cast<size_t>(1), shared_style_map_.size());
  kmldom::ObjectPtr object = shared_style_map_[kStyle0Id_];
  ASSERT_EQ(kStyle0Id_, object->get_id());
  ASSERT_EQ(kmldom::Type_Style, object->Type());
}

// Verify that the destructor does not effect the map.
TEST_F(SharedStyleParserObserverTest, TestDestructor) {
  // Use AddChild() to put some entries in the map.
  ASSERT_TRUE(shared_style_parser_observer_->AddChild(document_, style0_));
  ASSERT_TRUE(shared_style_parser_observer_->AddChild(document_, style1_));

  // Verify that deleting the observer does not affect the map.
  shared_style_parser_observer_.reset();

  // Verify that the object map has exactly the 2 expected mappings.
  ASSERT_EQ(static_cast<size_t>(2), shared_style_map_.size());
  ASSERT_EQ(kStyle0Id_, shared_style_map_[kStyle0Id_]->get_id());
  ASSERT_EQ(kStyle1Id_, shared_style_map_[kStyle1Id_]->get_id());
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
