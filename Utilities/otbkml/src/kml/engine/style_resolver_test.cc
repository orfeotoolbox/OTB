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

// This file contains the unit tests for the CreateResolvedStyle() function.

#include "kml/engine/style_resolver.h"
#include <string>
#include "kml/dom.h"
#include "kml/base/file.h"
#include "kml/base/net_cache_test_util.h"
#include "kml/base/unit_test.h"
#include "kml/engine/kml_cache.h"
#include "kml/engine/kml_file.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::StylePtr;

namespace kmlengine {

static const size_t kKmlCacheSize = 11;

class StyleResolverTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(StyleResolverTest);
  CPPUNIT_TEST(TestFiles);
  CPPUNIT_TEST(TestBasicCreateNetworkResolvedStyle);
  CPPUNIT_TEST(TestRemoteFiles);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestFiles();
  void TestBasicCreateNetworkResolvedStyle();
  void TestRemoteFiles();

 public:
  // Called before each test.
  void setUp() {
    kml_cache_.reset(new KmlCache(&test_data_net_fetcher_, kKmlCacheSize));
  }

  // Called after each test.
  void tearDown() {
  }

 private:
  // This is an internal utility to read a testdata file.
  bool ReadDataDirFileToString(const std::string& filename,
                               std::string* content) const;
  // Parse the given KML file into the test fixture's KmlFile.
  void ParseFromDataDirFile(const std::string& filename);
  // This returns 0 if element serializes pretty to exactly the contents of
  // the check file.
  int ComparePretty(const ElementPtr& element, const char* check_file) const;
  // KmlFile is used for its GetSharedStyleById().
  KmlFilePtr kml_file_;
  kmlbase::TestDataNetFetcher test_data_net_fetcher_;
  boost::scoped_ptr<KmlCache> kml_cache_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(StyleResolverTest);

// This is a table of style resolution test cases.  The resolved style for the
// the feature given by the id in feature_id_ for the given style state in the
// source_file_ is as found in the check_file_.  SerializePretty() is used as
// the comparison so the check_file_ should be "pretty".  All styleUrl's
// here are within the same file (fragment only).
static const struct {
  const char* source_file_;
  const char* feature_id_;
  const kmldom::StyleStateEnum style_state_;
  const char* check_file_;
} kTestCases [] = {
  { "/style/simple.kml", "pm0", kmldom::STYLESTATE_NORMAL,
    "/style/simple-pm0-check.kml" },
  { "/style/inline-style.kml", "f0", kmldom::STYLESTATE_NORMAL,
    "/style/inline-style-f0-check.kml" },
  { "/style/inline-stylemap.kml", "pm0", kmldom::STYLESTATE_HIGHLIGHT,
    "/style/inline-stylemap-pm0-highlight-check.kml" },
  { "/style/inline-stylemap.kml", "pm0", kmldom::STYLESTATE_NORMAL,
    "/style/inline-stylemap-pm0-normal-check.kml" },
  { "/style/shared-stylemap.kml", "pm0", kmldom::STYLESTATE_HIGHLIGHT,
    "/style/shared-stylemap-pm0-highlight-check.kml" },
  { "/style/shared-stylemap.kml", "pm0", kmldom::STYLESTATE_NORMAL,
    "/style/shared-stylemap-pm0-normal-check.kml" },
  { "/style/allstyles.kml", "f0", kmldom::STYLESTATE_NORMAL,
    "/style/allstyles-f0-normal-check.kml" },
#if 0 // TODO
  { "/style/allstyles.kml", "f0", kmldom::STYLESTATE_HIGHLIGHT,
    "/style/allstyles-f0-highlight-check.kml" },
#endif
  { "/style/allstyles.kml", "f1", kmldom::STYLESTATE_NORMAL,
    "/style/empty-style-check.kml" },
  { "/style/allstyles.kml", "f1", kmldom::STYLESTATE_HIGHLIGHT,
    "/style/empty-style-check.kml" }
};

// This is a utility function to read a file relative to the testdata directory.
bool StyleResolverTest::ReadDataDirFileToString(const std::string& filename,
                                                std::string* content) const {
  return kmlbase::File::ReadFileToString(std::string(DATADIR) + filename,
                                         content);
}

// This is a utility function to parse the given string of KML data into
// the test fixture's KmlFile.
void StyleResolverTest::ParseFromDataDirFile(const std::string& filename) {
  std::string kml_data;
  bool status = ReadDataDirFileToString(filename, &kml_data);
  CPPUNIT_ASSERT(status);
  kml_file_ = KmlFile::CreateFromParse(kml_data, NULL);
  CPPUNIT_ASSERT(kml_file_);
  CPPUNIT_ASSERT(kml_file_->root());
}

// This is a utility function to compare the given element to the KML in the
// given file.  The comparison uses "pretty" XML serialization.
int StyleResolverTest::ComparePretty(const ElementPtr& element,
                                     const char* check_file) const {
  std::string kml_data;
  CPPUNIT_ASSERT(ReadDataDirFileToString(check_file, &kml_data));
  return kml_data.compare(kmldom::SerializePretty(element));
}

// This function verifies all test cases in the kTestCases table.
void StyleResolverTest::TestFiles() {
  const size_t size = sizeof(kTestCases)/sizeof(kTestCases[0]);
  for (size_t i = 0; i < size; ++i) {
    // Read the file and find the feature.
    ParseFromDataDirFile(kTestCases[i].source_file_);
    FeaturePtr feature = kmldom::AsFeature(
        kml_file_->GetObjectById(kTestCases[i].feature_id_));
    CPPUNIT_ASSERT(feature);  // This is internal to the test.

    // This is the function under test.
    StylePtr style = CreateResolvedStyle(feature, kml_file_,
                                         kTestCases[i].style_state_);
    CPPUNIT_ASSERT(style);  // This helps debugging.

    // A text comparison is used as that detects issues with unknown elements.
    CPPUNIT_ASSERT(!ComparePretty(style, kTestCases[i].check_file_));
  }
}

void StyleResolverTest::TestBasicCreateNetworkResolvedStyle() {
  const std::string kPath("style/weather/point-sarnen.kml");
  const std::string kUrl("http://host.com/" + kPath);
  KmlFilePtr kml_file = kml_cache_->FetchKml(kUrl);
  CPPUNIT_ASSERT(kml_file);
  CPPUNIT_ASSERT_EQUAL(kml_cache_.get(), kml_file->get_kml_cache());
  const std::string kFeatureId("SZXX0026");
  const FeaturePtr& feature = kmldom::AsFeature(
      kml_file->GetObjectById(kFeatureId));
  CPPUNIT_ASSERT(feature);
  // Verify the feature has a styleUrl to another KML file.
  CPPUNIT_ASSERT(feature->has_styleurl());
  const std::string kStyleUrl("style.kml#i27");
  CPPUNIT_ASSERT_EQUAL(kStyleUrl, feature->get_styleurl());
  const kmldom::StyleStateEnum style_state = kmldom::STYLESTATE_NORMAL;
  StylePtr style = CreateResolvedStyle(feature, kml_file,
                                              style_state);
  CPPUNIT_ASSERT(style);
  CPPUNIT_ASSERT_EQUAL(std::string("i27"), style->get_id());
  CPPUNIT_ASSERT(style->has_iconstyle());
#if 0 // TODO: Merge doesn't know about IconStyle/IconStyleIcon...
  CPPUNIT_ASSERT(style->get_iconstyle()->has_icon());
  CPPUNIT_ASSERT(style->get_iconstyle()->get_icon());
  CPPUNIT_ASSERT(style->get_iconstyle()->get_icon()->has_href());
#endif
  CPPUNIT_ASSERT(style->has_labelstyle());
  CPPUNIT_ASSERT(style->has_balloonstyle());

#if 0  // TODO: proceed to resolve and fetch IconStyle/Icon/href
  std::string iconstyle_icon_url;
  CPPUNIT_ASSERT(ResolveUri(kml_file->get_url(),
                            style->get_iconstyle()->get_icon()->get_href(),
                            &iconstyle_icon_url));
  std::string icon_data;
  CPPUNIT_ASSERT(kmz_cache.FetchUrl(iconstyle_icon_url, &icon_data));
  CPPUNIT_ASSERT(!icon_data.empty());
#endif
}

// All styleUrl's references here are to other files either absolute or
// relative.
static const struct {
  const char* source_url_;
  const char* feature_id_;
  const kmldom::StyleStateEnum style_state_;
  const char* check_file_;
} kRemoteTestCases [] = {
  { "http://host.com/style/remote-styleurl.kml", "doc",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-document-check.kml" },
  { "http://host.com/style/remote-styleurl.kmz", "doc",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-document-check.kml" },
#if 0 // TODO resolve styleUrl's (and hrefs) in remote style.kml
  { "http://host.com/style/remote-styleurl.kml", "placemark",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-placemark-normal-check.kml" },
  { "http://host.com/style/remote-styleurl.kml", "placemark",
    kmldom::STYLESTATE_HIGHLIGHT,
    "/style/remote-styleurl-placemark-highlight-check.kml" },
#endif
  { "http://host.com/style/remote-styleurl.kml", "placemark-noid",
    kmldom::STYLESTATE_NORMAL,
    "/style/empty-style-check.kml" },
  { "http://host.com/style/remote-styleurl.kmz", "placemark-noid",
    kmldom::STYLESTATE_NORMAL,
    "/style/empty-style-check.kml" },
  { "http://host.com/style/remote-styleurl.kml", "placemark-nofile",
    kmldom::STYLESTATE_NORMAL,
    "/style/empty-style-check.kml" },
  { "http://host.com/style/remote-styleurl.kmz", "placemark-nofile",
    kmldom::STYLESTATE_NORMAL,
    "/style/empty-style-check.kml" },
  { "http://host.com/style/remote-styleurl.kml", "placemark-inline",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-placemark-inline-check.kml" },
  { "http://host.com/style/remote-styleurl.kmz", "placemark-inline",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-placemark-inline-check.kml" },
  { "http://host.com/style/remote-styleurl.kml", "placemark-style-both",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-placemark-style-both-check.kml" },
  { "http://host.com/style/remote-styleurl.kmz", "placemark-style-both",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-placemark-style-both-check.kml" },
  { "http://host.com/style/remote-styleurl.kml", "groundoverlay",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-groundoverlay-check.kml" },
  { "http://host.com/style/remote-styleurl.kmz", "groundoverlay",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-groundoverlay-check.kml" },
  { "http://host.com/style/remote-styleurl.kml", "screenoverlay",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-screenoverlay-check.kml" },
  { "http://host.com/style/remote-styleurl.kmz", "screenoverlay",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-screenoverlay-check.kml" },
#if 0 // TODO resolve styleUrl's (and hrefs) in remote style.kml
  { "http://host.com/style/remote-styleurl.kml", "photooverlay",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-photooverlay-normal-check.kml" },
  { "http://host.com/style/remote-styleurl.kml", "photooverlay",
    kmldom::STYLESTATE_HIGHLIGHT,
    "/style/remote-styleurl-photooverlay-highlight-check.kml" },
#endif
  { "http://host.com/style/remote-styleurl.kml", "networklink",
    kmldom::STYLESTATE_NORMAL,
    "/style/empty-style-check.kml" },
  { "http://host.com/style/remote-styleurl.kmz", "networklink",
    kmldom::STYLESTATE_NORMAL,
    "/style/empty-style-check.kml" },
  { "http://host.com/style/remote-styleurl.kml", "folder",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-folder-check.kml" },
  { "http://host.com/style/remote-styleurl.kmz", "folder",
    kmldom::STYLESTATE_NORMAL,
    "/style/remote-styleurl-folder-check.kml" },
  // These have styleUrls to weather/style_new.kmz#id.
  { "http://host.com/style/weather/points-puntas.kmz", "CIXX0017",
    kmldom::STYLESTATE_NORMAL,
    "/style/weather/points-puntas-CIXX0017-check.kml" },
  { "http://host.com/style/weather/points-puntas.kmz", "ARSC0046",
    kmldom::STYLESTATE_NORMAL,
    "/style/weather/points-puntas-ARSC0046-check.kml" },
  { "http://host.com/style/weather/points-puntas.kmz", "ARSC0133",
    kmldom::STYLESTATE_NORMAL,
    "/style/weather/points-puntas-ARSC0133-check.kml" },
};

void StyleResolverTest::TestRemoteFiles() {
  const size_t size = sizeof(kRemoteTestCases)/sizeof(kRemoteTestCases[0]);
  for (size_t i = 0; i < size; ++i) {
    // Read the file and find the feature.
    kml_file_ = kml_cache_->FetchKml(kRemoteTestCases[i].source_url_);
    CPPUNIT_ASSERT(kml_file_);
    FeaturePtr feature = kmldom::AsFeature(
        kml_file_->GetObjectById(kRemoteTestCases[i].feature_id_));
    CPPUNIT_ASSERT(feature);  // This is internal to the test.

    // This is the function under test.
    StylePtr style = CreateResolvedStyle(feature, kml_file_,
                                         kRemoteTestCases[i].style_state_);
    CPPUNIT_ASSERT(style);

    // A text comparison is used as that detects issues with unknown elements.
    CPPUNIT_ASSERT(!ComparePretty(style, kRemoteTestCases[i].check_file_));
  }
}

}  // end namespace kmlengine

TEST_MAIN
