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

// This file contains the unit tests for the Folder element.

#include "kml/dom/folder.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "gtest/gtest.h"

namespace kmldom {

class FolderTest : public testing::Test {
 protected:
  virtual void SetUp() {
    folder_ = KmlFactory::GetFactory()->CreateFolder();
  }

  FolderPtr folder_;
};

TEST_F(FolderTest, TestType) {
  ASSERT_EQ(Type_Folder, folder_->Type());
  ASSERT_TRUE(folder_->IsA(Type_Folder));
  ASSERT_TRUE(folder_->IsA(Type_Container));
  ASSERT_TRUE(folder_->IsA(Type_Feature));
  ASSERT_TRUE(folder_->IsA(Type_Object));
}

TEST_F(FolderTest, TestParse) {
  const char* kFolder =
    "<Folder id=\"folder123\">"
    "<name>My Favorite Folder</name>"
    "<Style/>"
    "<Region/>"
    "<Document/>"
    "<Folder/>"
    "<GroundOverlay/>"
    "<NetworkLink/>"
    "<PhotoOverlay/>"
    "<Placemark/>"
    "<ScreenOverlay/>"
    "</Folder>";
  string errors;
  ElementPtr root = Parse(kFolder, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const FolderPtr folder = AsFolder(root);
  ASSERT_TRUE(folder);
  // Verify the Object-ness of Folder.
  ASSERT_EQ(string("folder123"), folder->get_id());
  ASSERT_FALSE(folder->has_targetid());
  // Verify the Feature-ness of Folder.
  ASSERT_EQ(string("My Favorite Folder"), folder->get_name());
  ASSERT_FALSE(folder->has_description());
  ASSERT_TRUE(folder->has_styleselector());
  ASSERT_EQ(Type_Style, folder->get_styleselector()->Type());
  ASSERT_TRUE(folder->has_region());
  ASSERT_FALSE(folder->has_extendeddata());
  // Verify the Conatiner-ness of Folder.
  ASSERT_EQ(static_cast<size_t>(7),
                       folder->get_feature_array_size());
  ASSERT_EQ(Type_Document, folder->get_feature_array_at(0)->Type());
  ASSERT_EQ(Type_Folder, folder->get_feature_array_at(1)->Type());
  ASSERT_EQ(Type_GroundOverlay,
                       folder->get_feature_array_at(2)->Type());
  ASSERT_EQ(Type_NetworkLink,
                       folder->get_feature_array_at(3)->Type());
  ASSERT_EQ(Type_PhotoOverlay,
                       folder->get_feature_array_at(4)->Type());
  ASSERT_EQ(Type_Placemark,
                       folder->get_feature_array_at(5)->Type());
  ASSERT_EQ(Type_ScreenOverlay,
                       folder->get_feature_array_at(6)->Type());
  // ElementPtr root goes out of scope and releases Folder which in turn
  // releases all child elements.
}

// Test that a folder with an attribute, simple children and some Features
// serializes properly.
TEST_F(FolderTest, TestBasicSerialize) {
  KmlFactory* factory = KmlFactory::GetFactory();
  folder_->add_feature(factory->CreatePlacemark());
  folder_->add_feature(factory->CreateFolder());
  folder_->set_id("f789");
  folder_->set_name("some folder");
  folder_->set_region(factory->CreateRegion());
  const string kExpected =
    "<Folder id=\"f789\">"
    "<name>some folder</name>"
    "<Region/>"
    "<Placemark/>"
    "<Folder/>"
    "</Folder>";
  ASSERT_EQ(kExpected, SerializeRaw(folder_));
  // Delete of folder_ deletes all child elements.
}

// Test that an empty folder with no children serializes to a nil element.
TEST_F(FolderTest, TestNilSerialize) {
  ASSERT_EQ(string("<Folder/>"), SerializeRaw(folder_));
  folder_->set_id("xyz");
  ASSERT_EQ(string("<Folder id=\"xyz\"/>"),
                       SerializeRaw(folder_));
}

// Test SerializerPretty() on a Folder.
TEST_F(FolderTest, TestSerializePretty) {
  KmlFactory* factory = KmlFactory::GetFactory();
  folder_->add_feature(factory->CreatePlacemark());
  folder_->add_feature(factory->CreateFolder());
  folder_->set_id("f789");
  folder_->set_name("some folder");
  folder_->set_region(factory->CreateRegion());
  const string kExpected =
    "<Folder id=\"f789\">\n"
    "  <name>some folder</name>\n"
    "  <Region/>\n"
    "  <Placemark/>\n"
    "  <Folder/>\n"
    "</Folder>\n";
  ASSERT_EQ(kExpected, SerializePretty(folder_));
}

// Verify that a Folder won't take the same Feature more than once.
// (This tests the internal set_parent() method.)
TEST_F(FolderTest, TestAddFeatureTwiceToSameFolder) {
  PlacemarkPtr placemark = KmlFactory::GetFactory()->CreatePlacemark();
  ASSERT_EQ(static_cast<size_t>(0),
                       folder_->get_feature_array_size());
  folder_->add_feature(placemark);
  ASSERT_EQ(static_cast<size_t>(1),
                       folder_->get_feature_array_size());
  folder_->add_feature(placemark);  // Ignored
  ASSERT_EQ(static_cast<size_t>(1),
                       folder_->get_feature_array_size());
  // placemark is deleted when folder_ is deleted.
}

// Verify that only one Container will take a given Feature.
// (This tests the internal set_parent() method.)
TEST_F(FolderTest, TestAddFeatureToTwoContainers) {
  DocumentPtr document = KmlFactory::GetFactory()->CreateDocument();
  // Both containers initially empty.
  ASSERT_EQ(static_cast<size_t>(0),
                       folder_->get_feature_array_size());
  ASSERT_EQ(static_cast<size_t>(0),
                       document->get_feature_array_size());
  // The folder takes ownership of the placemark.
  PlacemarkPtr placemark = KmlFactory::GetFactory()->CreatePlacemark();
  folder_->add_feature(placemark);
  ASSERT_EQ(static_cast<size_t>(1),
                       folder_->get_feature_array_size());
  // The document ignores this placemark.
  document->add_feature(placemark);
  ASSERT_EQ(static_cast<size_t>(0),
                       document->get_feature_array_size());
  // placemark is deleted when placemark is deleted.
}

// Verify serialization follows XSD order.
TEST_F(FolderTest, TestParseSerialize) {
  const string kFolder(
      "<Folder>"
      "<name>hi</name>"
      "<description>hello</description>"
      "<styleUrl>#style-id</styleUrl>"
      "<Style/>"
      "<Placemark/>"
      "<Document/>"
      "<GroundOverlay/>"
      "</Folder>");
  ASSERT_EQ(kFolder, kmldom::SerializeRaw(kmldom::Parse(kFolder, NULL)));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
