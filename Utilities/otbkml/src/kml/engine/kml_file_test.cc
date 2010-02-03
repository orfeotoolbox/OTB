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

// This file contains the unit tests for the KmlFile class.

#include "kml/engine/kml_file.h"
#include <string>
#include "kml/base/net_cache.h"
#include "kml/base/unit_test.h"
#include "kml/dom.h"
#include "kml/engine/kml_cache.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmldom::ElementPtr;

namespace kmlengine {

class KmlFileTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(KmlFileTest);
  CPPUNIT_TEST(TestInitialState);
  CPPUNIT_TEST(TestEncoding);
  CPPUNIT_TEST(TestBasicParseFromString);
  CPPUNIT_TEST(TestRoot);
  CPPUNIT_TEST(TestBasicObjectIdParse);
  CPPUNIT_TEST(TestObjectIdDupe);
  CPPUNIT_TEST(TestObjectIdMapReplaced);
  CPPUNIT_TEST(TestNullGetSharedStyleById);
  CPPUNIT_TEST(TestBasicGetSharedStyleById);
  CPPUNIT_TEST(TestReparse);
  CPPUNIT_TEST(TestCreateFromParseOfKml);
  CPPUNIT_TEST(TestCreateFromParseOfJunk);
  CPPUNIT_TEST(TestCreateFromParseOfKmz);
  CPPUNIT_TEST(TestGetLinkParents);
  CPPUNIT_TEST(TestConstNull);
  CPPUNIT_TEST(TestBasicCreateFromString);
  CPPUNIT_TEST(TestBasicCreateFromStringWithUrl);
  CPPUNIT_TEST(TestCreateWithKmlCache);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    kml_file_ = KmlFile::Create();
  }

  // Called after each test.
  void tearDown() {
    // intrusive_ptr deletes kml_file_
  }

 protected:
  void TestInitialState();
  void TestEncoding();
  void TestBasicParseFromString();
  void TestRoot();
  void TestBasicObjectIdParse();
  void TestObjectIdDupe();
  void TestObjectIdMapReplaced();
  void TestNullGetSharedStyleById();
  void TestBasicGetSharedStyleById();
  void TestReparse();
  void TestCreateFromParseOfKml();
  void TestCreateFromParseOfJunk();
  void TestCreateFromParseOfKmz();
  void TestGetLinkParents();
  void TestConstNull();
  void TestBasicCreateFromString();
  void TestBasicCreateFromStringWithUrl();
  void TestCreateWithKmlCache();

 private:
  void VerifyIsPlacemarkWithName(const ElementPtr& root,
                                 const std::string& name);
  void KmlToKmz(const std::string& kml_data, std::string* kmz_data);
  KmlFilePtr kml_file_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(KmlFileTest);

// Verify the initial state of a freshly created empty KmlFile.
void KmlFileTest::TestInitialState() {
  CPPUNIT_ASSERT_EQUAL(kmldom::ElementPtr(), kml_file_->root());
  CPPUNIT_ASSERT_EQUAL(std::string(kDefaultEncoding),
                       kml_file_->get_encoding());
}

// Verify the encoding appears properly in the xml header.
void KmlFileTest::TestEncoding() {
  CPPUNIT_ASSERT_EQUAL(std::string(
                         "<?xml version=\"1.0\" encoding=\"utf-8\"?>"),
                       kml_file_->CreateXmlHeader());

  const std::string kIso_8859_1("iso-8859-1");
  kml_file_->set_encoding(kIso_8859_1);
  CPPUNIT_ASSERT_EQUAL(kIso_8859_1, kml_file_->get_encoding());

  CPPUNIT_ASSERT_EQUAL(std::string(
                         "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>"),
                       kml_file_->CreateXmlHeader());
}

// Verify basic usage of the ParseFromString() method.
void KmlFileTest::TestBasicParseFromString() {
  kmldom::ElementPtr root = kml_file_->ParseFromString("<kml/>", NULL);
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_kml, root->Type());

  std::string errors;
  root = kml_file_->ParseFromString("bad stuff", &errors);
  CPPUNIT_ASSERT(!root);
  CPPUNIT_ASSERT(!errors.empty());
}

void KmlFileTest::TestRoot() {
  CPPUNIT_ASSERT_EQUAL(kmldom::ElementPtr(), kml_file_->root());
  kmldom::ElementPtr root = kml_file_->ParseFromString("<kml/>", NULL);
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_kml, kml_file_->root()->Type());

  // Verify that a 2nd ParseFromString() on the same KmlFile resets root.
  root = kml_file_->ParseFromString("<Placemark/>", NULL);
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Placemark, kml_file_->root()->Type());
}

void KmlFileTest::TestBasicObjectIdParse() {
  kmldom::ElementPtr root = kml_file_->ParseFromString(
    "<Folder id=\"folder\">"
    "<Placemark id=\"placemark\"/>"
    "</Folder>",
    NULL);
  CPPUNIT_ASSERT(root);
  kmldom::ObjectPtr f = kml_file_->GetObjectById("folder");
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Folder, f->Type());
  kmldom::ObjectPtr p = kml_file_->GetObjectById("placemark");
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Placemark, p->Type());
}

void KmlFileTest::TestObjectIdDupe() {
  std::string errors;
  const std::string kDupeId("DUPE");
  kmldom::ElementPtr root = kml_file_->ParseFromString(
    "<Folder id=\"" + kDupeId + "\">"
    "<Placemark id=\"" + kDupeId + "\"/>"
    "</Folder>",
    &errors);
  // Verify failure of the parse.
  CPPUNIT_ASSERT(!root);
  // Verify no element of the duplicate id exists.
  CPPUNIT_ASSERT(!kml_file_->GetObjectById(kDupeId));
}

void KmlFileTest::TestObjectIdMapReplaced() {
  const std::string kTestId("the-id");

  // Verify that the given id is not found.
  CPPUNIT_ASSERT(!kml_file_->GetObjectById(kTestId));

  // Parse a file with the id and verify that it now is found.
  kmldom::ElementPtr root = kml_file_->ParseFromString(
    "<Placemark id=\"the-id\"/>", NULL);
  kmldom::ObjectPtr p = kml_file_->GetObjectById(kTestId);
  CPPUNIT_ASSERT_EQUAL(kTestId, p->get_id());

  // Parse another file without the id and verify that the test id is no
  // longer found.
  root = kml_file_->ParseFromString("<Placemark id=\"some-other-id\"/>", NULL);
  CPPUNIT_ASSERT(!kml_file_->GetObjectById(kTestId));
}

// Verify NULL is returned for a non-existent shared style.
void KmlFileTest::TestNullGetSharedStyleById() {
  CPPUNIT_ASSERT(!kml_file_->GetSharedStyleById("no-such-id"));
}

// Verify a basic shared style is found and a local style is not found.
void KmlFileTest::TestBasicGetSharedStyleById() {
  // Use ParseFromString to insert shared styles into the shared style map.
  const std::string kStyleId("share-me");
  const std::string kStyleMapId("me-too");
  const std::string kFolderStyleId("not-me");
  kmldom::ElementPtr root = kml_file_->ParseFromString(
    "<Document>"
      "<Style id=\"" + kStyleId + "\"/>"
      "<StyleMap id=\"" + kStyleMapId + "\"/>"
      "<Folder>"
        "<Style id=\"" + kFolderStyleId + "\"/>"
      "</Folder>"
    "</Document>", NULL);
  CPPUNIT_ASSERT(root);  // Verify the parse succeeded.

  // Verify both shared style selectors were found.
  kmldom::StyleSelectorPtr style = kml_file_->GetSharedStyleById(kStyleId);
  CPPUNIT_ASSERT(AsStyle(style));  // Verify it's a <Style>
  CPPUNIT_ASSERT_EQUAL(kStyleId, style->get_id());
  kmldom::StyleSelectorPtr stylemap =
      kml_file_->GetSharedStyleById(kStyleMapId);
  CPPUNIT_ASSERT(AsStyleMap(stylemap));  // Verify it's a <StyleMap>
  CPPUNIT_ASSERT_EQUAL(kStyleMapId, stylemap->get_id());

  // Verify that the local style is found as an Object...
  kmldom::ObjectPtr object = kml_file_->GetObjectById(kFolderStyleId);
  CPPUNIT_ASSERT(AsStyle(object));  // Verify it's a <Style>
  CPPUNIT_ASSERT_EQUAL(kFolderStyleId, object->get_id());
  // ...but is not found as a shared style.
  CPPUNIT_ASSERT(!kml_file_->GetSharedStyleById(kFolderStyleId));
}

// Verify that a 2nd parse of the same KML with a shared style succeeds.  This
// verifies that both the object_id and shared_style_map are properly reset.
void KmlFileTest::TestReparse() {
  const std::string kStyleWithId("<Style id=\"id\"/>");
  kmldom::ElementPtr root = kml_file_->ParseFromString(kStyleWithId, NULL);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(AsStyle(root));
  // Parse the same thing again.
  root = kml_file_->ParseFromString(kStyleWithId, NULL);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(AsStyle(root));
}

// This is an internal helper function to verify that the passed element
// is a Placemark with the given name.
void KmlFileTest::VerifyIsPlacemarkWithName(const ElementPtr& root,
                                            const std::string& name) {
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(AsPlacemark(root));
  CPPUNIT_ASSERT_EQUAL(name, AsPlacemark(root)->get_name());
}

// Verify the CreateFromParse() static method using KML data.
void KmlFileTest::TestCreateFromParseOfKml() {
  const std::string kName("my name");
  const std::string kKml("<Placemark><name>" + kName + "</name></Placemark>");
  std::string errors;
  kml_file_ = KmlFile::CreateFromParse(kKml, &errors);
  CPPUNIT_ASSERT(errors.empty());
  VerifyIsPlacemarkWithName(kml_file_->root(), kName);
}

// Verify the CreateFromParse() static method on junk input.
void KmlFileTest::TestCreateFromParseOfJunk() {
  const std::string kJunk("this is obviously neither KML nor KMZ");
  std::string errors;
  kml_file_ = KmlFile::CreateFromParse(kJunk, &errors);
}

// This is an internal helper function to create a KMZ memory buffer for the
// input KML memory buffer.
void KmlFileTest::KmlToKmz(const std::string& kml_data,
                           std::string* kmz_data) {
  kmlbase::TempFilePtr tempfile = kmlbase::TempFile::CreateTempFile();
  const char* tempname = tempfile->name().c_str();
  CPPUNIT_ASSERT(KmzFile::WriteKmz(tempname, kml_data));
  KmzFilePtr kmz_file = KmzFile::OpenFromFile(tempname);
  CPPUNIT_ASSERT(kmz_file);
  CPPUNIT_ASSERT(kmz_file->ReadKml(kmz_data));
}

// Verify the CreateFromParse() static method using KMZ data.
void KmlFileTest::TestCreateFromParseOfKmz() {
  const std::string kName("my name");
  const std::string kKml("<Placemark><name>" + kName + "</name></Placemark>");
  std::string kmz_data;
  KmlToKmz(kKml, &kmz_data);
  std::string errors;
  kml_file_ = KmlFile::CreateFromParse(kmz_data, &errors);
  CPPUNIT_ASSERT(errors.empty());
  VerifyIsPlacemarkWithName(kml_file_->root(), kName);
}

// Verify that GetParentLinkParserObservers finds all kinds of parents of
// links in a KML file.
void KmlFileTest::TestGetLinkParents() {
  const std::string kAllLinks = std::string(DATADIR) + "/links/alllinks.kml";
  std::string kml;
  CPPUNIT_ASSERT(kmlbase::File::ReadFileToString(kAllLinks, &kml));
  std::string errors;
  kml_file_ = KmlFile::CreateFromParse(kml, &errors);
  CPPUNIT_ASSERT(errors.empty());
  CPPUNIT_ASSERT(kml_file_);
  const ElementVector& link_parents = kml_file_->get_link_parent_vector();
  // This is obviously exactly matched to the content of alllinks.kml.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(7), link_parents.size());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_IconStyle, link_parents[0]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_ItemIcon, link_parents[1]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_NetworkLink, link_parents[2]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_GroundOverlay, link_parents[3]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_ScreenOverlay, link_parents[4]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_PhotoOverlay, link_parents[5]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Model, link_parents[6]->Type());
#if 0
  // TODO: handle styleUrl(?) and SchemaData
  CPPUNIT_ASSERT_EQUAL(std::string("style.kml#style"), href_vector[6]->Type());
  CPPUNIT_ASSERT_EQUAL(std::string("#myschema"), href_vector[7]);
#endif
}

// Verify const behavior.
void KmlFileTest::TestConstNull() {
  const KmlFilePtr kml_file = KmlFile::Create();
  CPPUNIT_ASSERT(!kml_file->GetObjectById("blah"));
  CPPUNIT_ASSERT(!kml_file->GetSharedStyleById("blah"));
}

// Verify basic usage of the CreateFromString() static method.
void KmlFileTest::TestBasicCreateFromString() {
  const std::string kName("my name");
  const std::string kPlacemark("<Placemark><name>" + kName +
                               "</name></Placemark>");
  kml_file_ = KmlFile::CreateFromString(kPlacemark);
  CPPUNIT_ASSERT(kml_file_);
}

// Verify basic usage of the CreateFromStringWithUrl() static method.
void KmlFileTest::TestBasicCreateFromStringWithUrl() {
  const std::string kName("my name");
  const std::string kPlacemark("<Placemark><name>" + kName +
                               "</name></Placemark>");
  const std::string kUrl("http://foo.com/goo/baz.kml");
  // There's no requirement a NetCache need exist.
  kml_file_ = KmlFile::CreateFromStringWithUrl(kPlacemark, kUrl, NULL);
  CPPUNIT_ASSERT(kml_file_);
  CPPUNIT_ASSERT_EQUAL(kUrl, kml_file_->get_url());
  kmldom::PlacemarkPtr placemark = AsPlacemark(kml_file_->root());
  CPPUNIT_ASSERT(placemark);
  CPPUNIT_ASSERT_EQUAL(kName, placemark->get_name());
  CPPUNIT_ASSERT(!kml_file_->get_kml_cache());
}

// Verify CreateFromStringWithUrl() with a KmlCache.
void KmlFileTest::TestCreateWithKmlCache() {
  kmlbase::NetFetcher null_net_fetcher;
  KmlCache kml_cache(&null_net_fetcher, 1);
  const std::string kName("my name");
  const std::string kPlacemark("<Placemark><name>" + kName +
                               "</name></Placemark>");
  const std::string kUrl("http://foo.com/goo/baz.kml");
  // Typically the KmlCache is the one that created the KmlFile.
  kml_file_ = KmlFile::CreateFromStringWithUrl(kPlacemark, kUrl, &kml_cache);
  CPPUNIT_ASSERT_EQUAL(kUrl, kml_file_->get_url());
  CPPUNIT_ASSERT_EQUAL(&kml_cache, kml_file_->get_kml_cache());
}

}  // end namespace kmlengine

TEST_MAIN
