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
#include "kml/engine/kml_uri_internal.h"
#include "kml/base/net_cache_test_util.h"
#include "kml/dom.h"
#include "kml/engine/find.h"
#include "kml/engine/kml_file.h"
#include "kml/engine/kmz_cache.h"
#include "kml/engine/link_util.h"
#include "gtest/gtest.h"

namespace kmlengine {

const size_t kCacheSize = 14;

class KmlUriTest : public testing::Test {
 protected:
  kmlbase::TestDataNetFetcher testdata_net_fetcher_;
  boost::scoped_ptr<KmlUri> kml_uri_;
};

// Verify basic normal usage of the KmlUri::CreateRelative() static method.
TEST_F(KmlUriTest, TestBasicKmlUriCreateRelative) {
  const string kBase("http://host.com/path/file.kml");
  const string kHref("image.jpg");
  const string kExpectedUrl("http://host.com/path/image.jpg");
  kml_uri_.reset(KmlUri::CreateRelative(kBase, kHref));
  ASSERT_TRUE(kml_uri_.get());
  ASSERT_EQ(kExpectedUrl, kml_uri_->get_url());
}

static struct {
  // These two are the inputs:
  const char* base;  // Typically from KmlFile::get_url().
  const char* target;  // Typically from <href>, etc.
  // These are the expected outputs:
  const char* resolved;  // NULL if not expected to resolve
  const char* kmz_base;  // NULL if not kmz
  const char* kmz_relative;  // NULL if not kmz
} kTestCases[] = {
  {
    "base/must/have/scheme/to/be/valid",
    "image.jpg",
    NULL,
    NULL,
    NULL
  },
  {
    "http://a.com/x",
    "y",
    "http://a.com/y",
    NULL,
    NULL
  },
  {
    "http://host.com/path/file.kml",
    "image.jpg",
    "http://host.com/path/image.jpg",
    NULL,
    NULL
  },
  {
    "http://host.com/path/file.kml",
    "http://otherhost.com/dir/image.jpg",
    "http://otherhost.com/dir/image.jpg",
    NULL,
    NULL
  },
  {
    "http://host.com/kmz/screenoverlay-continents.kmz/doc.kml",
    "pngs/africa.png",
    "http://host.com/kmz/screenoverlay-continents.kmz/pngs/africa.png",
    "http://host.com/kmz/screenoverlay-continents.kmz",
    "http://host.com/kmz/pngs/africa.png"
  },
  {
    "http://host.com/kmz/rumsey/kml/lc01.kmz/L_and_C/kml/01.kml",
    "../imagery/01_4.png",
    "http://host.com/kmz/rumsey/kml/lc01.kmz/L_and_C/imagery/01_4.png",
    "http://host.com/kmz/rumsey/kml/lc01.kmz",
    "http://host.com/kmz/rumsey/imagery/01_4.png"
  },
  {
    "http://host.com/path/file.kmz/doc.kml",
    "image.jpg",
    "http://host.com/path/file.kmz/image.jpg",
    "http://host.com/path/file.kmz",
    "http://host.com/path/image.jpg"
  }
};

TEST_F(KmlUriTest, TestKmlUriTestCases) {
  size_t size = sizeof(kTestCases)/sizeof(kTestCases[0]);
  for (size_t i = 0; i < size; ++i) {
    kml_uri_.reset(
        KmlUri::CreateRelative(kTestCases[i].base, kTestCases[i].target));
    if (kTestCases[i].resolved) {
      ASSERT_EQ(string(kTestCases[i].resolved), kml_uri_->get_url());
    } else {
      ASSERT_FALSE(kml_uri_.get());
    }
    if (kTestCases[i].kmz_base) {
      ASSERT_EQ(string(kTestCases[i].kmz_base), kml_uri_->get_kmz_url());
    }
    if (kTestCases[i].kmz_relative) {
      boost::scoped_ptr<KmlUri> kmz_relative(
          KmlUri::CreateRelative(kml_uri_->get_kmz_url(),
                                 kml_uri_->get_target()));
      ASSERT_TRUE(kmz_relative.get());
      ASSERT_EQ(string(kTestCases[i].kmz_relative),
                kmz_relative->get_url());
    }
  }
}

TEST_F(KmlUriTest, TestResolveUri) {
  const string kBase("http://foo.com");
  const string kRelative("file.kml");
  // NULL args returns false.
  ASSERT_FALSE(ResolveUri("", "", NULL));
  // NULL output arg returns false.
  ASSERT_FALSE(ResolveUri(kBase, kRelative, NULL));
  // Proper resolution of normal input to a given output returns true.
  string uri;
  ASSERT_TRUE(ResolveUri(kBase, kRelative, &uri));
  ASSERT_EQ(kBase + "/" + kRelative, uri);
}

TEST_F(KmlUriTest, TestSplitUri) {
  // Verify behavior of NULL args.
  ASSERT_TRUE(SplitUri("", NULL, NULL, NULL, NULL, NULL, NULL));
  // Verify behavior of a URI with all desired components.
  const string kScheme("http");
  const string kHost("example.com");
  const string kPort("8081");
  const string kPath("x/y/z");
  const string kQuery("name=val");
  const string kFragment("some-fragment");
  string scheme;
  string host;
  string port;
  string path;
  string query;
  string fragment;
  ASSERT_TRUE(SplitUri(kScheme + "://" + kHost + ":" + kPort + "/" +
                       kPath + "?" + kQuery + "#" + kFragment,
                       &scheme, &host, &port, &path, &query, &fragment));
  ASSERT_EQ(kScheme, scheme);
  ASSERT_EQ(kHost, host);
  ASSERT_EQ(kPort, port);
  ASSERT_EQ(kPath, path);
  ASSERT_EQ(kQuery, query);
  ASSERT_EQ(kFragment, fragment);
}

TEST_F(KmlUriTest, TestKmzSplit) {
  const string kFetchUrl("http://host.com/bldgs/model-macky.kmz");
  const string kKmzPath("photos/bh-flowers.jpg");
  const string kUrl(kFetchUrl + "/" + kKmzPath);
  string fetchable_url;
  string path_in_kmz;
  ASSERT_TRUE(KmzSplit(kUrl, &fetchable_url, &path_in_kmz));
  ASSERT_EQ(kFetchUrl, fetchable_url);
  ASSERT_EQ(kKmzPath, path_in_kmz);
}

TEST_F(KmlUriTest, TestBasicResolveModelTargetHref) {
  // Verify behavior for a common case.
  const string kBase("http://host.com/dir/foo.kmz/doc.kml");
  const string kModelHref("dir/bldg.dae");
  const string kTargetHref("../textures/brick.jpg");
  const string kResult("http://host.com/dir/foo.kmz/textures/brick.jpg");
  string result;
  ASSERT_TRUE(ResolveModelTargetHref(kBase, kModelHref, kTargetHref, &result));
  ASSERT_EQ(kResult, result);

  // Verify sane behavior with null args.
  ASSERT_FALSE(ResolveModelTargetHref("", "", "", NULL));
}

// This is a real-world test of ResolveModelTargetHref on all targetHref's
// in the model-macky.kmz test file.
TEST_F(KmlUriTest, TestModelTargetHrefOnKmz) {
  // Create a KmzCache instance with NetFetcher into testdata area.
  KmzCache kmz_cache(&testdata_net_fetcher_, 1);

  // Make up a reasonable enough URL for the benefit of KmzCache and
  // TestDataNetFetcher.
  const string kMackyUrl("http://host.com/kmz/model-macky.kmz/doc.kml");

  // Fetch the model-macky.kmz file into the KmzCache.
  string kml_data;
  kml_uri_.reset(KmlUri::CreateRelative(kMackyUrl, kMackyUrl));
  ASSERT_TRUE(kml_uri_.get());
  ASSERT_TRUE(kmz_cache.DoFetch(kml_uri_.get(), &kml_data));
  ASSERT_EQ(static_cast<size_t>(1), kmz_cache.Size());

  // Parse the default KML file.
  KmlFilePtr kml_file = KmlFile::CreateFromParse(kml_data, NULL);
  ASSERT_TRUE(kml_file.get());

  // Find the one Model we know is there.
  ElementVector all_models;
  GetElementsById(kml_file->get_root(), kmldom::Type_Model, &all_models);
  ASSERT_EQ(static_cast<size_t>(1), all_models.size());
  const kmldom::ModelPtr& model = AsModel(all_models[0]);
  // Find the Model's ResourceMap
  ASSERT_TRUE(model->has_resourcemap());
  const kmldom::ResourceMapPtr& resourcemap = model->get_resourcemap();

  // Find the value of the Model's Link/href
  string geometry_href;
  GetLinkParentHref(model, &geometry_href);

  string geometry_url;
  ASSERT_TRUE(ResolveUri(kMackyUrl, geometry_href, &geometry_url));

  kml_uri_.reset(KmlUri::CreateRelative(geometry_url, geometry_url));
  ASSERT_TRUE(kml_uri_.get());
  string data;
  ASSERT_TRUE(kmz_cache.DoFetch(kml_uri_.get(), &data));
  ASSERT_FALSE(data.empty());

  // Walk through the Alias's fetching each targetHref all of which are known
  // to be paths into the model-macky.kmz and all of which exist.
  size_t alias_array_size = resourcemap->get_alias_array_size();
  ASSERT_EQ(static_cast<size_t>(20), alias_array_size);
  for (size_t i = 0; i < alias_array_size; ++i) {
    const kmldom::AliasPtr& alias = resourcemap->get_alias_array_at(i);
    string targethref_url;
    // This is the method under test.  Assert successful resolution.
    ASSERT_TRUE(ResolveModelTargetHref(kMackyUrl, geometry_href,
                                       alias->get_targethref(),
                                       &targethref_url));
    // This presumes KmzCache::FetchUrl works and that the resolved URL
    // of the targetHref succeeds in fetching the data in the KmzCache.
    kml_uri_.reset(KmlUri::CreateRelative(targethref_url, targethref_url));
    ASSERT_TRUE(kml_uri_.get());
    data.clear();
    ASSERT_TRUE(kmz_cache.DoFetch(kml_uri_.get(), &data));
    ASSERT_FALSE(data.empty());
  }
}

TEST_F(KmlUriTest, TestNormalizeUri) {
  const string kUri("this/../is/a/relative/../uri.x");
  const string kNormalized("is/a/uri.x");
  string result;
  ASSERT_TRUE(NormalizeUri(kUri, &result));
  ASSERT_EQ(kNormalized, result);
}

TEST_F(KmlUriTest, TestNormalizeHref) {
  string href;
  string normalized;
#ifdef WIN32
  href = "this\\..\\a\\relative\\url.kmz\\..\\file.kml#id";
  normalized = "a\\relative\\file.kml#id";
#else
  href = "this/../a/relative/url.kmz/../file.kml#id";
  normalized = "a/relative/file.kml#id";
#endif
  string result;
  ASSERT_TRUE(NormalizeHref(href, &result));
  ASSERT_EQ(normalized, result);
}

TEST_F(KmlUriTest, TestUriToFilename) {
  // This simply tests that the call down to UriParser works as expected.
  string uri;
  string expected_filename;
#ifdef WIN32
  uri = "file:///C:/home/libkml/foo.bar";
  expected_filename = "C:\\home\\libkml\\foo.bar";
#else
  uri = "file:///home/libkml/foo.bar";
  expected_filename = "/home/libkml/foo.bar";
#endif
  string filename;
  ASSERT_TRUE(UriToFilename(uri, &filename));
  ASSERT_EQ(expected_filename, filename);
}

TEST_F(KmlUriTest, TestFilenameToUri) {
  // This simply tests that the call down to UriParser works as expected.
  string filename;
  string expected_uri;
#ifdef WIN32
  filename = "C:\\home\\libkml\\foo.bar";
  expected_uri = "file:///C:/home/libkml/foo.bar";
#else
  filename = "/home/libkml/foo.bar";
  expected_uri = "file:///home/libkml/foo.bar";
#endif
  string uri;
  ASSERT_TRUE(FilenameToUri(filename, &uri));
  ASSERT_EQ(expected_uri, uri);
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
