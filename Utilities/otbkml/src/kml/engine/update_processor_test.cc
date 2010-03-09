// Copyright 2009, Google Inc. All rights reserved.
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

// This file contains the unit test for the internal UpdateProcessor class.

#include "kml/engine/update_processor.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/base/string_util.h"
#include "kml/dom.h"
#include "kml/engine/kml_file.h"

namespace kmlengine {

class UpdateProcessorTest : public testing::Test {
 protected:
  void SetUp() {
    kml_file_.reset(KmlFile::CreateFromString("<kml/>"));
    update_processor_.reset(new UpdateProcessor(*kml_file_.get(), NULL));
  }
  boost::scoped_ptr<KmlFile> kml_file_;
  boost::scoped_ptr<UpdateProcessor> update_processor_;
};

TEST_F(UpdateProcessorTest, TestConstructor) {
  ASSERT_TRUE(kml_file_.get());
  ASSERT_TRUE(update_processor_.get());

  kmlbase::StringMap id_map;
  update_processor_.reset(new UpdateProcessor(*kml_file_.get(), &id_map));
  ASSERT_TRUE(update_processor_.get());
}

// Test basic use of the ProcessUpdate method.  See update_test.cc for a full
// set of <Update> test cases.
TEST_F(UpdateProcessorTest, TestProcessUpdate) {
  kmldom::UpdatePtr update = kmldom::KmlFactory::GetFactory()->CreateUpdate();
  update_processor_->ProcessUpdate(update);
}

TEST_F(UpdateProcessorTest, TestProcessUpdateChange) {
  kmldom::ChangePtr change = kmldom::KmlFactory::GetFactory()->CreateChange();
  update_processor_->ProcessUpdateChange(change);
}

TEST_F(UpdateProcessorTest, TestProcessUpdateCreate) {
  kmldom::CreatePtr create = kmldom::KmlFactory::GetFactory()->CreateCreate();
  update_processor_->ProcessUpdateCreate(create);
}

TEST_F(UpdateProcessorTest, TestProcessUpdateDelete) {
  kmldom::DeletePtr del = kmldom::KmlFactory::GetFactory()->CreateDelete();
  update_processor_->ProcessUpdateDelete(del);
}

TEST_F(UpdateProcessorTest, TestProcessGetTargetId) {
  kmldom::PlacemarkPtr placemark =
      kmldom::KmlFactory::GetFactory()->CreatePlacemark();
  ASSERT_FALSE(update_processor_->GetTargetId(placemark, NULL));
}

TEST_F(UpdateProcessorTest, TestProcessUpdateChangeWithMappedId) {
  // A KML file with a Placemark with an internal id.
  kml_file_.reset(KmlFile::CreateFromString(
      "<Placemark id=\"internal-id\">"
      "  <name>my name</name>"
      "</Placemark>"));
  ASSERT_TRUE(kml_file_.get());

  // A map of external ids to internal ids.
  kmlbase::StringMap id_map;
  id_map["external-id"] = "internal-id";

  // Create an UpdateProcessor for the KML file with id map.
  update_processor_.reset(new UpdateProcessor(*kml_file_.get(), &id_map));
  ASSERT_TRUE(update_processor_.get());

  // Create a <Change> with an external id.
  kmldom::ChangePtr change = kmldom::AsChange(kmldom::ParseKml(
      "<Change>"
      "  <Placemark targetId=\"external-id\">"
      "    <name>new name</name>"
      "  </Placemark>"
      "</Change>"));
  ASSERT_TRUE(change);

  // Run the method under test.
  update_processor_->ProcessUpdateChange(change);

  // Verify all is as expected.
  kmldom::PlacemarkPtr placemark = kmldom::AsPlacemark(kml_file_->get_root());
  ASSERT_TRUE(placemark);
  // The <Placemark>'s id= is _not_ effected.
  ASSERT_EQ(string("internal-id"), placemark->get_id());
  // No targetId= is set in the target Object.
  ASSERT_FALSE(placemark->has_targetid());
  // The <Placemark>'s <name> _is_ changed.
  ASSERT_EQ(string("new name"), placemark->get_name());
}

}  // namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
