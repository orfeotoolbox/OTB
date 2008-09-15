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

// This file contains the unit tests for the public API wrappers for URI
// resolution and parsing.  Unit tests for the internals of URI resolution
// and parsing are part of the 3rd party uriparser library itself.  These tests
// focus on the API behavior of the kml_uri.h functions themselves.

#include "kml/engine/kml_uri.h"
#include "kml/base/net_cache_test_util.h"
#include "kml/dom.h"
#include "kml/engine/find.h"
#include "kml/engine/kml_file.h"
#include "kml/engine/kmz_cache.h"
#include "kml/engine/link_util.h"
#include "kml/base/unit_test.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlengine {

class KmlUriTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(KmlUriTest);
  CPPUNIT_TEST(TestResolveUri);
  CPPUNIT_TEST(TestSplitUri);
  CPPUNIT_TEST(TestKmzSplit);
  CPPUNIT_TEST(TestBasicResolveModelTargetHref);
  CPPUNIT_TEST(TestModelTargetHrefOnKmz);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestResolveUri();
  void TestSplitUri();
  void TestKmzSplit();
  void TestBasicResolveModelTargetHref();
  void TestModelTargetHrefOnKmz();

 private:
  kmlbase::TestDataNetFetcher testdata_net_fetcher_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(KmlUriTest);

void KmlUriTest::TestResolveUri() {
  const std::string kBase("http://foo.com");
  const std::string kRelative("file.kml");
  // NULL args returns false.
  CPPUNIT_ASSERT(!ResolveUri("", "", NULL));
  // NULL output arg returns false.
  CPPUNIT_ASSERT(!ResolveUri(kBase, kRelative, NULL));
  // Proper resolution of normal input to a given output returns true.
  std::string uri;
  CPPUNIT_ASSERT(ResolveUri(kBase, kRelative, &uri));
  CPPUNIT_ASSERT_EQUAL(kBase + "/" + kRelative, uri);
}

void KmlUriTest::TestSplitUri() {
  // Verify behavior of NULL args.
  CPPUNIT_ASSERT(SplitUri("", NULL, NULL, NULL, NULL, NULL, NULL));
  // Verify behavior of a URI with all desired components.
  const std::string kScheme("http");
  const std::string kHost("example.com");
  const std::string kPort("8081");
  const std::string kPath("x/y/z");
  const std::string kQuery("name=val");
  const std::string kFragment("some-fragment");
  std::string scheme;
  std::string host;
  std::string port;
  std::string path;
  std::string query;
  std::string fragment;
  CPPUNIT_ASSERT(SplitUri(kScheme + "://" + kHost + ":" + kPort + "/" +
                          kPath + "?" + kQuery + "#" + kFragment,
                          &scheme, &host, &port, &path, &query, &fragment));
  CPPUNIT_ASSERT_EQUAL(kScheme, scheme);
  CPPUNIT_ASSERT_EQUAL(kHost, host);
  CPPUNIT_ASSERT_EQUAL(kPort, port);
  CPPUNIT_ASSERT_EQUAL(kPath, path);
  CPPUNIT_ASSERT_EQUAL(kQuery, query);
  CPPUNIT_ASSERT_EQUAL(kFragment, fragment);
}

void KmlUriTest::TestKmzSplit() {
  const std::string kFetchUrl("http://host.com/bldgs/model-macky.kmz");
  const std::string kKmzPath("photos/bh-flowers.jpg");
  const std::string kUrl(kFetchUrl + "/" + kKmzPath);
  std::string fetchable_url;
  std::string path_in_kmz;
  CPPUNIT_ASSERT(KmzSplit(kUrl, &fetchable_url, &path_in_kmz));
  CPPUNIT_ASSERT_EQUAL(kFetchUrl, fetchable_url);
  CPPUNIT_ASSERT_EQUAL(kKmzPath, path_in_kmz);
}

void KmlUriTest::TestBasicResolveModelTargetHref() {
  // Verify behavior for a common case.
  const std::string kBase("http://host.com/dir/foo.kmz/doc.kml");
  const std::string kModelHref("dir/bldg.dae");
  const std::string kTargetHref("../textures/brick.jpg");
  const std::string kResult("http://host.com/dir/foo.kmz/textures/brick.jpg");
  std::string result;
  CPPUNIT_ASSERT(
      ResolveModelTargetHref(kBase, kModelHref, kTargetHref, &result));
  CPPUNIT_ASSERT_EQUAL(kResult, result);

  // Verify sane behavior with null args.
  CPPUNIT_ASSERT(!ResolveModelTargetHref("", "", "", NULL));
}

// This is a real-world test of ResolveModelTargetHref on all targetHref's
// in the model-macky.kmz test file.
void KmlUriTest::TestModelTargetHrefOnKmz() {
  // Create a KmzCache instance with NetFetcher into testdata area.
  KmzCache kmz_cache(&testdata_net_fetcher_, 1);

  // Make up a reasonable enough URL for the benefit of KmzCache and
  // TestDataNetFetcher.
  const std::string kMackyUrl("http://host.com/kmz/model-macky.kmz/doc.kml");

  // Fetch the model-macky.kmz file into the KmzCache.
  std::string kml_data;
  CPPUNIT_ASSERT(kmz_cache.FetchUrl(kMackyUrl, &kml_data));
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), kmz_cache.Size());

  // Parse the default KML file.
  KmlFilePtr kml_file = KmlFile::CreateFromParse(kml_data, NULL);
  CPPUNIT_ASSERT(kml_file.get());

  // Find the one Model we know is there.
  ElementVector all_models;
  GetElementsById(kml_file->root(), kmldom::Type_Model, &all_models);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), all_models.size());
  const kmldom::ModelPtr& model = AsModel(all_models[0]);
  // Find the Model's ResourceMap
  CPPUNIT_ASSERT(model->has_resourcemap());
  const kmldom::ResourceMapPtr& resourcemap = model->get_resourcemap();

  // Find the value of the Model's Link/href
  std::string geometry_href;
  GetLinkParentHref(model, &geometry_href);

  std::string geometry_url;
  CPPUNIT_ASSERT(ResolveUri(kMackyUrl, geometry_href, &geometry_url));

  std::string data;
  CPPUNIT_ASSERT(kmz_cache.FetchUrl(geometry_url, &data));
  CPPUNIT_ASSERT(!data.empty());

  // Walk through the Alias's fetching each targetHref all of which are known
  // to be paths into the model-macky.kmz and all of which exist.
  size_t alias_array_size = resourcemap->get_alias_array_size();
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(20), alias_array_size);
  for (size_t i = 0; i < alias_array_size; ++i) {
    const kmldom::AliasPtr& alias = resourcemap->get_alias_array_at(i);
    std::string targethref_url;
    // This is the method under test.  Assert successful resolution.
    CPPUNIT_ASSERT(ResolveModelTargetHref(kMackyUrl, geometry_href,
                                          alias->get_targethref(),
                                          &targethref_url));
    // This presumes KmzCache::FetchUrl works and that the resolved URL
    // of the targetHref succeeds in fetching the data in the KmzCache.
    data.clear();
    CPPUNIT_ASSERT(kmz_cache.FetchUrl(targethref_url, &data));
    CPPUNIT_ASSERT(!data.empty());
  }
}

}  // end namespace kmlengine

TEST_MAIN
