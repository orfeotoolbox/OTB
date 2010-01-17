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
#include "kml/base/unit_test.h"

namespace kmldom {

class FolderTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(FolderTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestParse);
  CPPUNIT_TEST(TestBasicSerialize);
  CPPUNIT_TEST(TestNilSerialize);
  CPPUNIT_TEST(TestSerializePretty);
  CPPUNIT_TEST(TestAddFeatureTwiceToSameFolder);
  CPPUNIT_TEST(TestAddFeatureToTwoContainers);
  CPPUNIT_TEST_SUITE_END();

 public:
  // This is called before each test.
  void setUp() {
    folder_ = KmlFactory::GetFactory()->CreateFolder();
  }

  void tearDown() {
  }

 protected:
  void TestType();
  void TestParse();
  void TestBasicSerialize();
  void TestNilSerialize();
  void TestSerializePretty();
  void TestAddFeatureTwiceToSameFolder();
  void TestAddFeatureToTwoContainers();

 private:
  FolderPtr folder_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(FolderTest);

void FolderTest::TestType() {
  CPPUNIT_ASSERT_EQUAL(Type_Folder, folder_->Type());
  CPPUNIT_ASSERT(true == folder_->IsA(Type_Folder));
  CPPUNIT_ASSERT(true == folder_->IsA(Type_Container));
  CPPUNIT_ASSERT(true == folder_->IsA(Type_Feature));
  CPPUNIT_ASSERT(true == folder_->IsA(Type_Object));
}

void FolderTest::TestParse() {
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
  std::string errors;
  ElementPtr root = Parse(kFolder, &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const FolderPtr folder = AsFolder(root);
  CPPUNIT_ASSERT(folder);
  // Verify the Object-ness of Folder.
  CPPUNIT_ASSERT_EQUAL(std::string("folder123"), folder->get_id());
  CPPUNIT_ASSERT(false == folder->has_targetid());
  // Verify the Feature-ness of Folder.
  CPPUNIT_ASSERT_EQUAL(std::string("My Favorite Folder"), folder->get_name());
  CPPUNIT_ASSERT(false == folder->has_description());
  CPPUNIT_ASSERT(folder->has_styleselector());
  CPPUNIT_ASSERT_EQUAL(Type_Style, folder->get_styleselector()->Type());
  CPPUNIT_ASSERT(folder->has_region());
  CPPUNIT_ASSERT(false == folder->has_extendeddata());
  // Verify the Conatiner-ness of Folder.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(7),
                       folder->get_feature_array_size());
  CPPUNIT_ASSERT_EQUAL(Type_Document, folder->get_feature_array_at(0)->Type());
  CPPUNIT_ASSERT_EQUAL(Type_Folder, folder->get_feature_array_at(1)->Type());
  CPPUNIT_ASSERT_EQUAL(Type_GroundOverlay,
                       folder->get_feature_array_at(2)->Type());
  CPPUNIT_ASSERT_EQUAL(Type_NetworkLink,
                       folder->get_feature_array_at(3)->Type());
  CPPUNIT_ASSERT_EQUAL(Type_PhotoOverlay,
                       folder->get_feature_array_at(4)->Type());
  CPPUNIT_ASSERT_EQUAL(Type_Placemark,
                       folder->get_feature_array_at(5)->Type());
  CPPUNIT_ASSERT_EQUAL(Type_ScreenOverlay,
                       folder->get_feature_array_at(6)->Type());
  // ElementPtr root goes out of scope and releases Folder which in turn
  // releases all child elements.
}

// Test that a folder with an attribute, simple children and some Features
// serializes properly.
void FolderTest::TestBasicSerialize() {
  KmlFactory* factory = KmlFactory::GetFactory();
  folder_->add_feature(factory->CreatePlacemark());
  folder_->add_feature(factory->CreateFolder());
  folder_->set_id("f789");
  folder_->set_name("some folder");
  folder_->set_region(factory->CreateRegion());
  const std::string kExpected =
    "<Folder id=\"f789\">"
    "<name>some folder</name>"
    "<Region/>"
    "<Placemark/>"
    "<Folder/>"
    "</Folder>";
  CPPUNIT_ASSERT_EQUAL(kExpected, SerializeRaw(folder_));
  // Delete of folder_ deletes all child elements.
}

// Test that an empty folder with no children serializes to a nil element.
void FolderTest::TestNilSerialize() {
  CPPUNIT_ASSERT_EQUAL(std::string("<Folder/>"), SerializeRaw(folder_));
  folder_->set_id("xyz");
  CPPUNIT_ASSERT_EQUAL(std::string("<Folder id=\"xyz\"/>"),
                       SerializeRaw(folder_));
}

// Test SerializerPretty() on a Folder.
void FolderTest::TestSerializePretty() {
  KmlFactory* factory = KmlFactory::GetFactory();
  folder_->add_feature(factory->CreatePlacemark());
  folder_->add_feature(factory->CreateFolder());
  folder_->set_id("f789");
  folder_->set_name("some folder");
  folder_->set_region(factory->CreateRegion());
  const std::string kExpected =
    "<Folder id=\"f789\">\n"
    "  <name>some folder</name>\n"
    "  <Region/>\n"
    "  <Placemark/>\n"
    "  <Folder/>\n"
    "</Folder>\n";
  CPPUNIT_ASSERT_EQUAL(kExpected, SerializePretty(folder_));
}

// Verify that a Folder won't take the same Feature more than once.
// (This tests the internal set_parent() method.)
void FolderTest::TestAddFeatureTwiceToSameFolder() {
  PlacemarkPtr placemark = KmlFactory::GetFactory()->CreatePlacemark();
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0),
                       folder_->get_feature_array_size());
  folder_->add_feature(placemark);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1),
                       folder_->get_feature_array_size());
  folder_->add_feature(placemark);  // Ignored
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1),
                       folder_->get_feature_array_size());
  // placemark is deleted when folder_ is deleted.
}

// Verify that only one Container will take a given Feature.
// (This tests the internal set_parent() method.)
void FolderTest::TestAddFeatureToTwoContainers() {
  DocumentPtr document = KmlFactory::GetFactory()->CreateDocument();
  // Both containers initially empty.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0),
                       folder_->get_feature_array_size());
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0),
                       document->get_feature_array_size());
  // The folder takes ownership of the placemark.
  PlacemarkPtr placemark = KmlFactory::GetFactory()->CreatePlacemark();
  folder_->add_feature(placemark);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1),
                       folder_->get_feature_array_size());
  // The document ignores this placemark.
  document->add_feature(placemark);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0),
                       document->get_feature_array_size());
  // placemark is deleted when placemark is deleted.
}

}  // end namespace kmldom

TEST_MAIN
