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
#include "kml/base/file.h"
#include "kml/base/net_cache.h"
#include "kml/base/tempfile.h"
#include "gtest/gtest.h"
#include "kml/dom.h"
#include "kml/engine/kml_cache.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmldom::ElementPtr;
using kmldom::FolderPtr;
using kmldom::KmlFactory;
using kmldom::KmlPtr;
using kmldom::ObjectPtr;
using kmldom::PlacemarkPtr;
using kmldom::StyleMapPtr;
using kmldom::StyleSelectorPtr;

namespace kmlengine {

class KmlFileTest : public testing::Test {
 protected:
  void VerifyIsPlacemarkWithName(const ElementPtr& root,
                                 const string& name);
  void KmlToKmz(const string& kml_data, string* kmz_data);

  KmlFilePtr kml_file_;
};

// Verify the encoding appears properly in the xml header.
TEST_F(KmlFileTest, TestEncoding) {
  kml_file_ = KmlFile::CreateFromParse("<kml/>", NULL);
  ASSERT_TRUE(kml_file_);
  ASSERT_EQ(string("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"),
            kml_file_->CreateXmlHeader());

  const string kIso_8859_1("iso-8859-1");
  kml_file_->set_encoding(kIso_8859_1);
  ASSERT_EQ(kIso_8859_1, kml_file_->get_encoding());

  ASSERT_EQ(string("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n"),
            kml_file_->CreateXmlHeader());
}

// Verify basic usage of the ParseFromString() method.
TEST_F(KmlFileTest, TestBasicParseFromString) {
  kml_file_ = KmlFile::CreateFromParse("<kml/>", NULL);
  ASSERT_TRUE(kml_file_);
  ElementPtr root = kml_file_->get_root();
  // Verify old API exists and functions.
  ASSERT_EQ(root, kml_file_->root());
  ASSERT_TRUE(root);
  ASSERT_EQ(kmldom::Type_kml, root->Type());

  string errors;
  kml_file_ = KmlFile::CreateFromParse("bad stuff", &errors);
  ASSERT_FALSE(kml_file_);
  ASSERT_FALSE(errors.empty());
}

TEST_F(KmlFileTest, TestRoot) {
  kml_file_ = KmlFile::CreateFromParse("<kml/>", NULL);
  ElementPtr root = kml_file_->get_root();
  ASSERT_TRUE(root);
  ASSERT_EQ(kmldom::Type_kml, root->Type());

  // Verify that any complex element can be used as root.
  kml_file_ = KmlFile::CreateFromParse("<Placemark/>", NULL);
  ASSERT_EQ(kmldom::Type_Placemark, kml_file_->get_root()->Type());
  // Verify old API exists and functions.
  ASSERT_EQ(kmldom::Type_Placemark, kml_file_->root()->Type());
}

TEST_F(KmlFileTest, TestBasicObjectIdParse) {
  kml_file_ = KmlFile::CreateFromParse(
    "<Folder id=\"folder\">"
    "<Placemark id=\"placemark\"/>"
    "</Folder>",
    NULL);
  ASSERT_TRUE(kml_file_);
  ObjectPtr f = kml_file_->GetObjectById("folder");
  ASSERT_EQ(kmldom::Type_Folder, f->Type());
  ObjectPtr p = kml_file_->GetObjectById("placemark");
  ASSERT_EQ(kmldom::Type_Placemark, p->Type());
}

TEST_F(KmlFileTest, TestObjectIdDupePassing) {
  string errors;
  const string kDupeId("DUPE");
  kml_file_ = KmlFile::CreateFromParse(
    "<Folder id=\"" + kDupeId + "\">"
    "<Placemark id=\"" + kDupeId + "\"/>"
    "</Folder>",
    &errors);
  // By default the duplicate ids do not cause the parse to fail.
  ASSERT_TRUE(kml_file_);
  // Verify an element of the duplicate id exists.
  ASSERT_TRUE(kml_file_->GetObjectById(kDupeId));
}

// TODO: how/if to bring back strict mode for import from xml
#if 0
TEST_F(KmlFileTest, TestObjectIdDupeFailing) {
  string errors;
  const string kDupeId("DUPE");
  kml_file_->set_strict_parse(true);
  kmldom::ElementPtr root = kml_file_->ParseFromString(
    "<Folder id=\"" + kDupeId + "\">"
    "<Placemark id=\"" + kDupeId + "\"/>"
    "</Folder>",
    &errors);
  // Verify failure of the parse.
  ASSERT_FALSE(root);
  // Verify no element of the duplicate id exists.
  ASSERT_FALSE(kml_file_->GetObjectById(kDupeId));
}
#endif

// Verify NULL is returned for a non-existent shared style.
TEST_F(KmlFileTest, TestNullGetSharedStyleById) {
  kml_file_ = KmlFile::CreateFromParse("<kml/>", NULL);
  ASSERT_FALSE(kml_file_->GetSharedStyleById("no-such-id"));
}

// Verify a basic shared style is found and a local style is not found.
TEST_F(KmlFileTest, TestBasicGetSharedStyleById) {
  // Use ParseFromString to insert shared styles into the shared style map.
  const string kStyleId("share-me");
  const string kStyleMapId("me-too");
  const string kFolderStyleId("not-me");
  kml_file_ = KmlFile::CreateFromParse(
    "<Document>"
      "<Style id=\"" + kStyleId + "\"/>"
      "<StyleMap id=\"" + kStyleMapId + "\"/>"
      "<Folder>"
        "<Style id=\"" + kFolderStyleId + "\"/>"
      "</Folder>"
    "</Document>", NULL);
  ASSERT_TRUE(kml_file_);  // Verify the parse succeeded.

  // Verify both shared style selectors were found.
  StyleSelectorPtr style = kml_file_->GetSharedStyleById(kStyleId);
  ASSERT_TRUE(AsStyle(style));  // Verify it's a <Style>
  ASSERT_EQ(kStyleId, style->get_id());
  StyleSelectorPtr stylemap = kml_file_->GetSharedStyleById(kStyleMapId);
  ASSERT_TRUE(AsStyleMap(stylemap));  // Verify it's a <StyleMap>
  ASSERT_EQ(kStyleMapId, stylemap->get_id());

  // Verify that the local style is found as an Object...
  ObjectPtr object = kml_file_->GetObjectById(kFolderStyleId);
  ASSERT_TRUE(AsStyle(object));  // Verify it's a <Style>
  ASSERT_EQ(kFolderStyleId, object->get_id());
  // ...but is not found as a shared style.
  ASSERT_FALSE(kml_file_->GetSharedStyleById(kFolderStyleId));
}

// This is an internal helper function to verify that the passed element
// is a Placemark with the given name.
void KmlFileTest::VerifyIsPlacemarkWithName(const ElementPtr& root,
                                            const string& name) {
  ASSERT_TRUE(root);
  ASSERT_TRUE(AsPlacemark(root));
  ASSERT_EQ(name, AsPlacemark(root)->get_name());
}

// Verify the CreateFromParse() static method using KML data.
TEST_F(KmlFileTest, TestCreateFromParseOfKml) {
  const string kName("my name");
  const string kKml("<Placemark><name>" + kName + "</name></Placemark>");
  string errors;
  kml_file_ = KmlFile::CreateFromParse(kKml, &errors);
  ASSERT_TRUE(errors.empty());
  VerifyIsPlacemarkWithName(kml_file_->get_root(), kName);
  VerifyIsPlacemarkWithName(kml_file_->root(), kName);
}

// Verify the CreateFromParse() static method on junk input.
TEST_F(KmlFileTest, TestCreateFromParseOfJunk) {
  const string kJunk("this is obviously neither KML nor KMZ");
  string errors;
  kml_file_ = KmlFile::CreateFromParse(kJunk, &errors);
}

// This is an internal helper function to create a KMZ memory buffer for the
// input KML memory buffer.
void KmlFileTest::KmlToKmz(const string& kml_data,
                           string* kmz_data) {
  kmlbase::TempFilePtr tempfile = kmlbase::TempFile::CreateTempFile();
  const char* tempname = tempfile->name().c_str();
  ASSERT_TRUE(KmzFile::WriteKmz(tempname, kml_data));
  KmzFilePtr kmz_file = KmzFile::OpenFromFile(tempname);
  ASSERT_TRUE(kmz_file);
  ASSERT_TRUE(kmz_file->ReadKml(kmz_data));
}

// Verify the CreateFromParse() static method using KMZ data.
TEST_F(KmlFileTest, TestCreateFromParseOfKmz) {
  const string kName("my name");
  const string kKml("<Placemark><name>" + kName + "</name></Placemark>");
  string kmz_data;
  KmlToKmz(kKml, &kmz_data);
  string errors;
  kml_file_ = KmlFile::CreateFromParse(kmz_data, &errors);
  ASSERT_TRUE(errors.empty());
  VerifyIsPlacemarkWithName(kml_file_->get_root(), kName);
  VerifyIsPlacemarkWithName(kml_file_->root(), kName);
}

// Verify that GetParentLinkParserObservers finds all kinds of parents of
// links in a KML file.
TEST_F(KmlFileTest, TestGetLinkParents) {
  const string kAllLinks = string(DATADIR) + "/links/alllinks.kml";
  string kml;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(kAllLinks, &kml));
  string errors;
  kml_file_ = KmlFile::CreateFromParse(kml, &errors);
  ASSERT_TRUE(errors.empty());
  ASSERT_TRUE(kml_file_);
  const ElementVector& link_parents = kml_file_->get_link_parent_vector();
  // This is obviously exactly matched to the content of alllinks.kml.
  ASSERT_EQ(static_cast<size_t>(7), link_parents.size());
  ASSERT_EQ(kmldom::Type_IconStyle, link_parents[0]->Type());
  ASSERT_EQ(kmldom::Type_ItemIcon, link_parents[1]->Type());
  ASSERT_EQ(kmldom::Type_NetworkLink, link_parents[2]->Type());
  ASSERT_EQ(kmldom::Type_GroundOverlay, link_parents[3]->Type());
  ASSERT_EQ(kmldom::Type_ScreenOverlay, link_parents[4]->Type());
  ASSERT_EQ(kmldom::Type_PhotoOverlay, link_parents[5]->Type());
  ASSERT_EQ(kmldom::Type_Model, link_parents[6]->Type());
#if 0
  // TODO: handle styleUrl(?) and SchemaData
  ASSERT_EQ(string("style.kml#style"), href_vector[6]->Type());
  ASSERT_EQ(string("#myschema"), href_vector[7]);
#endif
}

// Verify const behavior.
TEST_F(KmlFileTest, TestConstNull) {
  const KmlFilePtr kml_file = KmlFile::CreateFromParse("<kml/>", NULL);
  ASSERT_TRUE(kml_file);
  ASSERT_FALSE(kml_file->GetObjectById("blah"));
  ASSERT_FALSE(kml_file->GetSharedStyleById("blah"));
}

// Verify basic usage of the CreateFromString() static method.
TEST_F(KmlFileTest, TestBasicCreateFromString) {
  const string kName("my name");
  const string kPlacemark("<Placemark><name>" + kName +
                               "</name></Placemark>");
  kml_file_ = KmlFile::CreateFromString(kPlacemark);
  ASSERT_TRUE(kml_file_);
}

// Verify basic usage of the CreateFromStringWithUrl() static method.
TEST_F(KmlFileTest, TestBasicCreateFromStringWithUrl) {
  const string kName("my name");
  const string kPlacemark("<Placemark><name>" + kName +
                               "</name></Placemark>");
  const string kUrl("http://foo.com/goo/baz.kml");
  // There's no requirement a NetCache need exist.
  kml_file_ = KmlFile::CreateFromStringWithUrl(kPlacemark, kUrl, NULL);
  ASSERT_TRUE(kml_file_);
  ASSERT_EQ(kUrl, kml_file_->get_url());
  PlacemarkPtr placemark = AsPlacemark(kml_file_->get_root());
  ASSERT_EQ(placemark, AsPlacemark(kml_file_->root()));
  ASSERT_TRUE(placemark);
  ASSERT_EQ(kName, placemark->get_name());
  ASSERT_FALSE(kml_file_->get_kml_cache());
}

// Verify CreateFromStringWithUrl() with a KmlCache.
TEST_F(KmlFileTest, TestCreateWithKmlCache) {
  kmlbase::NetFetcher null_net_fetcher;
  KmlCache kml_cache(&null_net_fetcher, 1);
  const string kName("my name");
  const string kPlacemark("<Placemark><name>" + kName +
                               "</name></Placemark>");
  const string kUrl("http://foo.com/goo/baz.kml");
  // Typically the KmlCache is the one that created the KmlFile.
  kml_file_ = KmlFile::CreateFromStringWithUrl(kPlacemark, kUrl, &kml_cache);
  ASSERT_EQ(kUrl, kml_file_->get_url());
  ASSERT_EQ(&kml_cache, kml_file_->get_kml_cache());
}

// Verify KmlFile's default xmlns is set on the root element.
TEST_F(KmlFileTest, TestXmlnsOnRoot) {
  string xml;
  const string kExpectedKml(
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
      "<kml xmlns=\"http://www.opengis.net/kml/2.2\"/>\n");
  kml_file_ = KmlFile::CreateFromString("<kml/>");
  ASSERT_TRUE(kml_file_->SerializeToString(&xml));
  ASSERT_EQ(kExpectedKml, xml);

  const string kExpectedPlacemark(
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
      "<Placemark xmlns=\"http://www.opengis.net/kml/2.2\"/>\n");
  kml_file_ = KmlFile::CreateFromString("<Placemark/>");
  xml.clear();
  ASSERT_TRUE(kml_file_->SerializeToString(&xml));
  ASSERT_EQ(kExpectedPlacemark, xml);
}

// Verify KmlFile::CreateFromImport()
TEST_F(KmlFileTest, TestCreateFromImport) {
  ASSERT_FALSE(KmlFile::CreateFromImport(NULL));
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  PlacemarkPtr placemark = kml_factory->CreatePlacemark();
  const string kName("my name");
  placemark->set_name(kName);
  KmlPtr kml = KmlFactory::GetFactory()->CreateKml();
  kml->set_feature(placemark);
  KmlFilePtr kml_file = KmlFile::CreateFromImport(kml);
  ASSERT_TRUE(kml_file);
  ElementPtr root = kml_file->get_root();
  ASSERT_EQ(root, kml_file->root());
  ASSERT_TRUE(root);
  ASSERT_TRUE(kmldom::AsKml(root));
  ASSERT_EQ(kName, kmldom::AsKml(root)->get_feature()->get_name());
}

// Verify KmlFile::CreateFromImport()
TEST_F(KmlFileTest, TestCreateFromImportFailsOnDupeIds) {
  ASSERT_FALSE(KmlFile::CreateFromImport(kmldom::Parse(
    "<Folder id=\"foo\"><Placemark id=\"foo\"/></Folder>",
    NULL)));
}

// Verify Import-Serialize
TEST_F(KmlFileTest, TestCreateFromImportSerialize) {
  KmlPtr kml = KmlFactory::GetFactory()->CreateKml();
  kml_file_ = KmlFile::CreateFromImport(kml);
  ASSERT_TRUE(kml_file_);
  string xml;
  ASSERT_TRUE(kml_file_->SerializeToString(&xml));
  ASSERT_EQ(string(
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<kml xmlns=\"http://www.opengis.net/kml/2.2\"/>\n"),
    xml);
}

// Verify KmlFile::CreateFromImport() with GetObjectById() and
// GetSharedStyleById()
TEST_F(KmlFileTest, TestCreateFromImportAndGetById) {
  // Use a dom-level parse to create an element hierarchy.
  const string kAllStyles = string(DATADIR) + "/style/allstyles.kml";
  string kml;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(kAllStyles, &kml));
  ElementPtr element = kmldom::Parse(kml, NULL);
  ASSERT_TRUE(element);

  // Import this dom into a KmlFile.
  kml_file_ = KmlFile::CreateFromImport(element);
  ASSERT_TRUE(kml_file_);
  ASSERT_EQ(kmldom::Type_Style,
            kml_file_->GetSharedStyleById("allstyles")->Type());
  ASSERT_EQ(kmldom::Type_ListStyle,
            kml_file_->GetObjectById("liststyle")->Type());
  ASSERT_EQ(kmldom::Type_Folder, kml_file_->GetObjectById("f0")->Type());
  ASSERT_FALSE(kml_file_->GetObjectById("no-object-with-this-id"));
  ASSERT_FALSE(kml_file_->GetSharedStyleById("no-shared-style-with-this-id"));
}

// Verify KmlFile::CreateFromImportLax()
TEST_F(KmlFileTest, TestCreateFromImportLax) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  FolderPtr folder = kml_factory->CreateFolder();
  PlacemarkPtr placemark = kml_factory->CreatePlacemark();
  const string kId("some-id-to-duplicate");
  const string kFirstName("first name");
  placemark->set_id(kId);
  placemark->set_name(kFirstName);
  folder->add_feature(placemark);
  placemark = kml_factory->CreatePlacemark();
  const string kLastName("last name");
  placemark->set_id(kId);
  placemark->set_name(kLastName);
  folder->add_feature(placemark);
  // Plain CreateFromImport() is strict about duplicate ids.
  ASSERT_FALSE(KmlFile::CreateFromImport(folder));
  // CreateFromImportLax() permits duplicate ids...
  KmlFilePtr kml_file = KmlFile::CreateFromImportLax(folder);
  ASSERT_TRUE(kml_file);
  // ...and the id mapping hits the last object with that id...
  placemark = AsPlacemark(kml_file->GetObjectById(kId));
  ASSERT_TRUE(placemark);
  ASSERT_EQ(kLastName, placemark->get_name());
  // ...and the first one is the first feature in the folder
  folder = AsFolder(kml_file->get_root());
  ASSERT_EQ(folder, AsFolder(kml_file->root()));
  ASSERT_TRUE(folder);
  ASSERT_EQ(static_cast<size_t>(2), folder->get_feature_array_size());
  placemark = AsPlacemark(folder->get_feature_array_at(0));
  ASSERT_TRUE(placemark);
  ASSERT_EQ(kFirstName, placemark->get_name());
  ASSERT_EQ(kId, placemark->get_id());
}

TEST_F(KmlFileTest, TestForSerializeWithNamespaces) {
  kml_file_ = KmlFile::CreateFromString(
      "<Document><gx:Tour><atom:author/></gx:Tour></Document>");
  ASSERT_TRUE(kml_file_);
  string xml;
  ASSERT_TRUE(kml_file_->SerializeToString(&xml));
  const string kExpected =
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
      "<Document xmlns=\"http://www.opengis.net/kml/2.2\" "
      "xmlns:atom=\"http://www.w3.org/2005/Atom\" "
      "xmlns:gx=\"http://www.google.com/kml/ext/2.2\">\n"
      "  <gx:Tour>\n"
      "    <atom:author/>\n"
      "  </gx:Tour>\n"
      "</Document>\n";
  ASSERT_EQ(kExpected, xml);
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
